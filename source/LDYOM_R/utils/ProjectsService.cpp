#include "ProjectsService.h"

#include "Logger.h"
#include <filesystem>

#include "FileWatcher.h"
#include "utils.h"
#include "fmt/core.h"
#include "imgui_notify.h"
#include "LuaEngine.h"
#include "WindowsRenderService.h"
#include "../Windows/ObjectivesWindow.h"
#include "../Data/CheckpointObjective.h"
#include "../Data/KillActorOrGroupObjective.h"
#include "../Data/DamageActorObjective.h"
#include "../Data/GetInVehicleObjective.h"
#include "../Data/DestroyVehicleObjective.h"
#include "../Data/CutsceneObjective.h"
#include "../Data/CountdownObjective.h"
#include "../Data/TimeoutObjective.h"
#include "../Data/WeatherObjective.h"
#include "../Data/ClockTimeObjective.h"
#include "../Data/DamageObjectObjective.h"
#include "../Data/PhotographObjectObjective.h"
#include "../Data/TouchObjectObjective.h"
#include "../Data/CollectPickupObjective.h"
#include "../Data/TeleportPlayerObjective.h"
#include "../Data/AnimationPlayerObjective.h"
#include "../Data/TeleportToVehiclePlayerObjective.h"
#include "../Data/LevelWantedPlayerObjective.h"
#include "../Data/RemoveWeaponsObjective.h"
#include "../Data/AddWeaponsPlayerObjective.h"
#include "../Data/PhoneCallPlayerObjective.h"
#include "../Data/AddMoneyPlayerObjective.h"
#include "../Data/AddTimerObjective.h"
#include "../Data/RemoveTimerObjective.h"
#include "../Data/StartMissionObjective.h"
#include "../Data/EndMissionObjective.h"
#include "../Data/GoToSceneObjective.h"
#include "../Data/WaitSignalObjective.h"
#include "../Data/FollowPathActorObjective.h"
#include "../Data/AnimationActorObjective.h"
#include "../Data/EnterVehicleActorObjective.h"
#include "../Data/FollowPathVehicleObjective.h"
#include "../Data/JumpToObjectiveObjective.h"
#include "../Data/SetGlobalVariableObjective.h"
#include "../Data/SaveObjective.h"
#include "../Data/SceneSettings.h"

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/unique_ptr.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/array.hpp"
#include "boost/serialization/utility.hpp"
#include "boost/serialization/split_free.hpp"
#include "boost/uuid/uuid_serialize.hpp"
#include "zip.h"
#include "../lz4/lz4hc.h"
#include "easylogging/easylogging++.h"


const std::filesystem::path PROJECTS_PATH = L"LDYOM\\Projects\\";
const std::filesystem::path BACKUPS_PATH = L"LDYOM\\Backups\\";

void ProjectsService::update() {
	this->projectsInfos_.clear();
	this->productionProjectsInfos_.clear();
	for (const auto &entry : std::filesystem::directory_iterator(PROJECTS_PATH)) {
		if (entry.is_directory()) {
			auto missionFile = entry.path() / "index.dat";
			if (exists(missionFile)) {
				std::ifstream file(missionFile, std::ios_base::binary);
				boost::archive::binary_iarchive ia(file);
				ProjectInfo* projectInfo;
				ia >> projectInfo;
				file.close();
				if (auto icon = entry.path() / L"icon.png"; exists(icon)) {
					projectInfo->icon = utils::LoadTextureRequiredFromFile(icon);
				}

				projectInfo->directory = entry.path();

				this->projectsInfos_.emplace_back(std::unique_ptr<ProjectInfo>(projectInfo));
			}
		} else {
			if (entry.path().extension() == ".ldpack") {
				auto zip = zip_open(entry.path().string().c_str(), NULL, 'r');
				if (zip_entry_open(zip, "index.dat") == 0) {
					size_t indexSize;
					void* indexBytes;
					zip_entry_read(zip, &indexBytes, &indexSize);

					std::istringstream stream(std::string(static_cast<const char*>(indexBytes), indexSize));
					stream.rdbuf()->pubsetbuf(static_cast<char*>(indexBytes), indexSize);
					boost::archive::binary_iarchive ia(stream);
					ProjectInfo* projectInfo;
					ia >> projectInfo;

					free(indexBytes);
					zip_entry_close(zip);

					projectInfo->directory = entry.path();
					if (zip_entry_open(zip, "icon.png") == 0) {
						void* iconBytes;
						size_t iconBytesSize;
						zip_entry_read(zip, &iconBytes, &iconBytesSize);

						projectInfo->icon = utils::LoadTextureRequiredFromMemory(iconBytes, iconBytesSize);

						free(iconBytes);

						zip_entry_close(zip);
					}

					this->productionProjectsInfos_.emplace_back(std::unique_ptr<ProjectInfo>(projectInfo));
				}
				zip_close(zip);
 			}
		}
	}
}

void ProjectsService::Reset() {
	this->currentDirectory_ = std::nullopt;
}

void ProjectsService::createNewProject() {
	this->getCurrentProject().onChangedScene()();
	this->getCurrentProject().getCurrentScene()->unloadEditorScene();
	this->currentProject_ = ProjectData();
	this->currentDirectory_ = std::nullopt;
}

bool zipWalk(zip_t* zip, const std::filesystem::path& path) {
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
		if (entry.is_regular_file()) {

			zip_entry_open(zip, entry.path().lexically_relative(path).string().c_str());

			auto filePath = entry.path().string();
			if (const int resultWrite = zip_entry_fwrite(zip, filePath.c_str()); resultWrite < 0) {
				CLOG(ERROR, "LDYOM") << "Error archive directory - " << path.string() << ", error: " << resultWrite;
				return false;
			}

			zip_entry_close(zip);
		}
	}
	return true;
}

void ProjectsService::saveCurrentProject() {
	auto projectDirectoryName = std::filesystem::path(utils::stringToPathString(this->getCurrentProject().getProjectInfo()->name));
	const auto projectDirectory = PROJECTS_PATH / projectDirectoryName;
	auto scenesDirectory = projectDirectory / "scenes";

	if (this->currentDirectory_.has_value() && exists(this->currentDirectory_.value())) {
		auto checkError = [](const std::error_code& ec) {
			if (ec.value() != 0) {
				CLOG(ERROR, "LDYOM") << "invalid rename project, error: " << ec.message();
				ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Error, see log" });
				return true;
			}
			return false;
		};

		std::error_code ec;

		if (!exists(BACKUPS_PATH)) {
			create_directory(BACKUPS_PATH, ec);
			if (checkError(ec)) return;
		}

		auto backupPath = (BACKUPS_PATH / fmt::format("{}_{}.zip", projectDirectoryName.string(), time(nullptr))).string();
		auto zip = zip_open(backupPath.c_str(), 9, 'w');
		if (!zipWalk(zip, this->currentDirectory_.value()))
			return;
		zip_close(zip);

		std::filesystem::rename(this->currentDirectory_.value(), projectDirectory, ec);
		if (checkError(ec)) return;
	}

	if (!exists(projectDirectory)) {
		//create directories
		std::error_code error;
		if (!create_directory(projectDirectory, error)) {
			if (error.value() != 0)
				CLOG(ERROR, "LDYOM") << "Failed to create the project directory, error: " << error.message();
			ImGui::InsertNotification({ ImGuiToastType_Error, 1000, "Error, see log"});
			return;
		}

		if (!create_directory(scenesDirectory, error)) {
			if (error.value() != 0)
				CLOG(ERROR, "LDYOM") << "Failed to create scenes directory, error: {} " << error.message();
			ImGui::InsertNotification({ ImGuiToastType_Error, 1000, "Error, see log" });
			return;
		}
	}

	this->getCurrentProject().getProjectInfo()->directory = projectDirectory;

	{
		//save index file
		std::ofstream file(projectDirectory / "index.dat", std::ios_base::binary);
		boost::archive::binary_oarchive oa(file);
		oa << this->getCurrentProject().getProjectInfo().get();

		for (const auto& pairLua : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["saveProject"].get_or_create<sol::table>()) {
			if (auto result = pairLua.second.as<sol::function>()(); !result.valid()) {
				sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
			else {
				oa << result.get<std::string>();
			}
		}
		file.close();
	}

	//save scenes
	for (const auto& pair: this->getCurrentProject().getScenes()) {
		std::ostringstream uncompressedStream;
		boost::archive::binary_oarchive oa(uncompressedStream);
		oa << pair.second;
		for (auto pairLua : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["saveScene"].get_or_create<sol::table>()) {
			if (auto result = pairLua.second.as<sol::function>()(pair.first); !result.valid()) {
				sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			} else {
				oa << result.get<std::string>();
			}
		}

		auto ucString = uncompressedStream.str();

		std::vector<char> compressedVector;
		unsigned inputSize = ucString.size();
		compressedVector.resize(LZ4_compressBound(inputSize), '\0');
		int result = LZ4_compress_HC(ucString.c_str(), compressedVector.data(), static_cast<int>(inputSize), static_cast<int>(compressedVector.size()), LZ4HC_CLEVEL_MAX);
		if (result == 0) {
			CLOG(ERROR, "LDYOM") << "Failed compress";
			return;
		}

		std::ofstream file(scenesDirectory / fmt::format("{}.dat", pair.first), std::ios_base::binary);
		file.write(reinterpret_cast<char*>(&inputSize), sizeof inputSize);
		std::ranges::copy(compressedVector.begin(), compressedVector.begin() + result, std::ostreambuf_iterator(file));
		file.close();
	}

	this->currentDirectory_ = projectDirectory;

	ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Saved!" });
}

void ProjectsService::loadProjectData(const std::filesystem::path& projectDirectory) {
	auto scenesDirectory = projectDirectory / "scenes";

	{
		//load index file
		std::ifstream file(projectDirectory / "index.dat", std::ios_base::binary);
		boost::archive::binary_iarchive ia(file);
		ProjectInfo* projectInfo;
		ia >> projectInfo;
		this->getCurrentProject().getProjectInfo() = std::unique_ptr<ProjectInfo>(projectInfo);
		for (auto pairLua : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["loadProject"].get_or_create<sol::table>()) {
			std::string data;
			ia >> data;
			if (auto result = pairLua.second.as<sol::function>()(data); !result.valid()) {
				sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}
		file.close();
	}

	//load scenes
	for (auto& path : std::filesystem::directory_iterator(scenesDirectory)) {
		if (path.is_regular_file() && path.path().extension() == ".dat") {
			std::string compressed;
			unsigned uncompressedSize;
			{
				std::ifstream file(path.path(), std::ios_base::binary);
				file.read(reinterpret_cast<char*>(&uncompressedSize), sizeof uncompressedSize);
				std::ostringstream ss;
				ss << file.rdbuf();
				compressed = ss.str();
				file.close();
			}

			std::vector<char> uncompressedVector;
			uncompressedVector.resize(uncompressedSize, '\0');
			int result = LZ4_decompress_safe(compressed.c_str(), uncompressedVector.data(), static_cast<int>(compressed.size()), static_cast<int>(uncompressedSize));
			if (result == 0) {
				CLOG(ERROR, "LDYOM") << "Failed compress";
				return;
			}
			std::istringstream ss(std::string(uncompressedVector.begin(), uncompressedVector.end()));

			int sceneId = std::stoi(path.path().stem().wstring());

			boost::archive::binary_iarchive ia(ss);
			std::unique_ptr<Scene> scene;
			ia >> scene;
			for (auto pairLua : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["loadScene"].get_or_create<sol::table>()) {
				std::string data;
				ia >> data;
				if (auto result = pairLua.second.as<sol::function>()(sceneId, data); !result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
				}
			}


			getCurrentProject().getScenes().emplace(sceneId, std::move(scene));
		}
	}

	this->currentDirectory_ = projectDirectory;
}

void ProjectsService::loadProject(int projectIdx) {
	getCurrentProject().getCurrentScene()->unloadEditorScene();

	//clear all scenes
	getCurrentProject().getScenes().clear();

	const auto & projectDirectory = projectsInfos_.at(projectIdx)->directory;

	this->loadProjectData(projectDirectory);

	this->getCurrentProject().getCurrentSceneIndex() = projectsInfos_.at(projectIdx)->startScene;

	this->getCurrentProject().getCurrentScene()->loadEditorScene();

	getCurrentProject().onChangedScene()();

	Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectivesWindow>()->selectElement(this->getCurrentProject().getCurrentScene()->getObjectives().size() - 1);

	this->onUpdate_();

	ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Loaded!" });
}

void ProjectsService::loadProductionProject(int projectIdx) {
	getCurrentProject().getCurrentScene()->unloadEditorScene();

	//clear all scenes
	getCurrentProject().getScenes().clear();

	const auto& projectDirectory = std::filesystem::temp_directory_path() / productionProjectsInfos_.at(projectIdx)->directory.stem();
	if (exists(projectDirectory)) {
		remove_all(projectDirectory);
	}
	if (create_directory(projectDirectory)) {
		auto code = zip_extract(productionProjectsInfos_.at(projectIdx)->directory.string().c_str(), projectDirectory.string().c_str(), nullptr, nullptr);
		this->loadProjectData(projectDirectory);
		this->getCurrentProject().getCurrentSceneIndex() = productionProjectsInfos_.at(projectIdx)->startScene;
	}
}

void ProjectsService::deleteProject(int projectIdx) const {
	const auto& projectDirectory = projectsInfos_.at(projectIdx)->directory;
	remove_all(projectDirectory);
}

void ProjectsService::deleteProductionProject(int projectIdx) const {
	const auto& projectDirectory = productionProjectsInfos_.at(projectIdx)->directory;
	remove(projectDirectory);
}

void ProjectsService::makeProjectProduction(int projectIdx) {
	auto zip = zip_open((PROJECTS_PATH / fmt::format("{}.ldpack", this->projectsInfos_.at(projectIdx)->name)).string().c_str(), 9, 'w');
	if (!zipWalk(zip, this->projectsInfos_.at(projectIdx)->directory))
		return;
	zip_close(zip);

	ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Saved public project!" });
}

std::vector<std::unique_ptr<ProjectInfo>>& ProjectsService::getProjectsInfos() {
	return projectsInfos_;
}

std::vector<std::unique_ptr<ProjectInfo>>& ProjectsService::getProductionProjectsInfos() {
	return productionProjectsInfos_;
}

boost::signals2::signal<void()>& ProjectsService::onUpdate() {
	return onUpdate_;
}

void ProjectsService::Init() {
	update();
	Logger::getInstance().log("ProjectsManager is init.");
	FileWatcher::addWatch(PROJECTS_PATH,
		true,
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_DIR_NAME,
		[&] {
			update();
			this->onUpdate()();
		});
}

ProjectData& ProjectsService::getCurrentProject() {
	return currentProject_;
}

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive& ar, CQuaternion& p, const unsigned int version)
		{
			ar& p.real;
			ar& p.imag.x;
			ar& p.imag.y;
			ar& p.imag.z;
		}

		template<class Archive>
		void serialize(Archive& ar, Scene& p, const unsigned int version)
		{
			ar & make_array(p.getName(), NAME_SIZE);
			ar & p.getId();
			ar & p.getActors();
			ar & p.getVehicles();
			ar & p.getObjects();
			ar & p.getTrains();
			ar & p.getParticles();
			ar & p.getPickups();
			ar & p.getPyrotechnics();
			ar & p.getAudio();
			ar & p.getVisualEffects();
			ar & p.getCheckpoints();

			ar & p.getSceneSettings();
			ar & p.isToggleSceneSettings();

			ar.template register_type<CheckpointObjective>();
			ar.template register_type<KillActorOrGroupObjective>();
			ar.template register_type<DamageActorObjective>();
			ar.template register_type<GetInVehicleObjective>();
			ar.template register_type<DestroyVehicleObjective>();
			ar.template register_type<CutsceneObjective>();
			ar.template register_type<CountdownObjective>();
			ar.template register_type<TimeoutObjective>();
			ar.template register_type<WeatherObjective>();
			ar.template register_type<ClockTimeObjective>();
			ar.template register_type<DamageObjectObjective>();
			ar.template register_type<PhotographObjectObjective>();
			ar.template register_type<TouchObjectObjective>();
			ar.template register_type<CollectPickupObjective>();
			ar.template register_type<TeleportPlayerObjective>();
			ar.template register_type<AnimationPlayerObjective>();
			ar.template register_type<TeleportToVehiclePlayerObjective>();
			ar.template register_type<LevelWantedPlayerObjective>();
			ar.template register_type<RemoveWeaponsObjective>();
			ar.template register_type<AddWeaponsPlayerObjective>();
			ar.template register_type<PhoneCallPlayerObjective>();
			ar.template register_type<AddMoneyPlayerObjective>();
			ar.template register_type<AddTimerObjective>();
			ar.template register_type<RemoveTimerObjective>();
			ar.template register_type<StartMissionObjective>();
			ar.template register_type<EndMissionObjective>();
			ar.template register_type<GoToSceneObjective>();
			ar.template register_type<WaitSignalObjective>();
			ar.template register_type<FollowPathActorObjective>();
			ar.template register_type<AnimationActorObjective>();
			ar.template register_type<EnterVehicleActorObjective>();
			ar.template register_type<FollowPathVehicleObjective>();
			ar.template register_type<JumpToObjectiveObjective>();
			ar.template register_type<SetGlobalVariableObjective>();
			ar.template register_type<SaveObjective>();

			ar& p.getObjectives();
		}

		template<class Archive>
		void serialize(Archive& ar, SceneSettings& p, const unsigned int version)
		{
			ar & make_array(p.groupRelations.data(), p.groupRelations.size());
			ar & make_array(p.time.data(), p.time.size());
			ar & p.trafficPed;
			ar & p.trafficCar;
			ar & p.wantedMin;
			ar & p.wantedMax;
			ar & p.weather;
			ar & p.riot;
		}

		template<class Archive>
		void save(Archive& ar, const std::filesystem::path& p, const unsigned int version)
		{
			ar & p.string();
		}
		template<class Archive>
		void load(Archive& ar, std::filesystem::path& p, const unsigned int version)
		{
			std::string path;
			ar & path;
			p = path;
		}

	}
}

BOOST_SERIALIZATION_SPLIT_FREE(std::filesystem::path)