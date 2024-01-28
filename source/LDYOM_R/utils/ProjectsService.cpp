#include "ProjectsService.h"

#include <fast_dynamic_cast.h>
#include <filesystem>

#include "DiscordService.h"
#include "Logger.h"

#include "FileWatcher.h"
#include "imgui_notify.h"
#include "strUtils.h"
#include "utils.h"
#include "WindowsRenderService.h"
#include "../Data/AddMoneyPlayerObjective.h"
#include "../Data/AddTimerObjective.h"
#include "../Data/AddWeaponsPlayerObjective.h"
#include "../Data/AnimationActorObjective.h"
#include "../Data/AnimationPlayerObjective.h"
#include "../Data/CheckpointObjective.h"
#include "../Data/ClockTimeObjective.h"
#include "../Data/CollectPickupObjective.h"
#include "../Data/CountdownObjective.h"
#include "../Data/CutsceneObjective.h"
#include "../Data/DamageActorObjective.h"
#include "../Data/DamageObjectObjective.h"
#include "../Data/DestroyVehicleObjective.h"
#include "../Data/EndMissionObjective.h"
#include "../Data/EnterVehicleActorObjective.h"
#include "../Data/FollowPathActorObjective.h"
#include "../Data/FollowPathVehicleObjective.h"
#include "../Data/GetInVehicleObjective.h"
#include "../Data/GoToSceneObjective.h"
#include "../Data/JumpToObjectiveObjective.h"
#include "../Data/KillActorOrGroupObjective.h"
#include "../Data/LevelWantedPlayerObjective.h"
#include "../Data/PhoneCallPlayerObjective.h"
#include "../Data/PhotographObjectObjective.h"
#include "../Data/RemoveTimerObjective.h"
#include "../Data/RemoveWeaponsObjective.h"
#include "../Data/SaveObjective.h"
#include "../Data/SceneSettings.h"
#include "../Data/SetIncrementGlobalVariable.h"
#include "../Data/StartMissionObjective.h"
#include "../Data/TeleportPlayerObjective.h"
#include "../Data/TeleportToVehiclePlayerObjective.h"
#include "../Data/TimeoutObjective.h"
#include "../Data/TouchObjectObjective.h"
#include "../Data/TrafficObjective.h"
#include "../Data/WaitSignalObjective.h"
#include "../Data/WeatherObjective.h"
#include "../Windows/ObjectivesWindow.h"
#include "fmt/core.h"

#include "zip.h"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/serialization/utility.hpp"
#include "easylogging/easylogging++.h"


const std::filesystem::path PROJECTS_PATH = L"LDYOM\\Projects\\";
const std::filesystem::path BACKUPS_PATH = L"LDYOM\\Backups\\";

constexpr int FLAGS = boost::archive::archive_flags::no_header + boost::archive::archive_flags::no_tracking;

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Scene> {
		static void to_json(json &j, const Scene &cs) {
			auto &c = const_cast<Scene&>(cs);
			j = json{
				{"name", c.getName()},
				{"id", c.getId()},
				{"actors", c.getActors()},
				{"vehicles", c.getVehicles()},
				{"objects", c.getObjects()},
				{"trains", c.getTrains()},
				{"particles", c.getParticles()},
				{"pickups", c.getPickups()},
				{"pyrotechnics", c.getPyrotechnics()},
				{"audio", c.getAudio()},
				{"visualEffects", c.getVisualEffects()},
				{"checkpoints", c.getCheckpoints()},
				{"sceneSettings", c.getSceneSettings()},
				{"toggleSceneSettings", c.isToggleSceneSettings()},
			};
			auto jsonObjectives = json::array();
			for (auto &objective : c.getObjectives()) {
				switch (objective->getCategory()) {
					case 0:
						switch (objective->getTypeCategory()) {
							//World
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<CheckpointObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<CutsceneObjective&>(*objective));
								break;
							case 2:
								jsonObjectives.push_back(fast_dynamic_cast<CountdownObjective&>(*objective));
								break;
							case 3:
								jsonObjectives.push_back(fast_dynamic_cast<TimeoutObjective&>(*objective));
								break;
							case 4:
								jsonObjectives.push_back(fast_dynamic_cast<WeatherObjective&>(*objective));
								break;
							case 5:
								jsonObjectives.push_back(fast_dynamic_cast<ClockTimeObjective&>(*objective));
								break;
							case 6:
								jsonObjectives.push_back(fast_dynamic_cast<TrafficObjective&>(*objective));
								break;
							case 7:
								jsonObjectives.push_back(fast_dynamic_cast<AddTimerObjective&>(*objective));
								break;
							case 8:
								jsonObjectives.push_back(fast_dynamic_cast<RemoveTimerObjective&>(*objective));
								break;
							case 9:
								jsonObjectives.push_back(fast_dynamic_cast<SetIncrementGlobalVariable&>(*objective));
								break;

							default:
								break;
						}
						break;
					case 1:
						switch (objective->getTypeCategory()) {
							// Actor
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<KillActorOrGroupObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<DamageActorObjective&>(*objective));
								break;
							case 2:
								jsonObjectives.push_back(fast_dynamic_cast<FollowPathActorObjective&>(*objective));
								break;
							case 3:
								jsonObjectives.push_back(fast_dynamic_cast<AnimationActorObjective&>(*objective));
								break;
							case 4:
								jsonObjectives.push_back(fast_dynamic_cast<EnterVehicleActorObjective&>(*objective));
								break;

							default:
								break;
						}
						break;

					case 2:
						switch (objective->getTypeCategory()) {
							// Vehicle
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<GetInVehicleObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<DestroyVehicleObjective&>(*objective));
								break;
							case 2:
								jsonObjectives.push_back(fast_dynamic_cast<FollowPathVehicleObjective&>(*objective));
								break;

							default:
								break;
						}
						break;
					case 3:
						switch (objective->getTypeCategory()) {
							//Object
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<DamageObjectObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<PhotographObjectObjective&>(*objective));
								break;
							case 2:
								jsonObjectives.push_back(fast_dynamic_cast<TouchObjectObjective&>(*objective));
								break;

							default:
								break;
						}
						break;
					case 4:
						switch (objective->getTypeCategory()) {
							//Pickup
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<CollectPickupObjective&>(*objective));
								break;

							default:
								break;
						}
						break;
					case 5:
						switch (objective->getTypeCategory()) {
							// Player
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<TeleportPlayerObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<AnimationPlayerObjective&>(*objective));
								break;
							case 2:
								jsonObjectives.push_back(
									fast_dynamic_cast<TeleportToVehiclePlayerObjective&>(*objective));
								break;
							case 3:
								jsonObjectives.push_back(fast_dynamic_cast<LevelWantedPlayerObjective&>(*objective));
								break;
							case 4:
								jsonObjectives.push_back(fast_dynamic_cast<RemoveWeaponsObjective&>(*objective));
								break;
							case 5:
								jsonObjectives.push_back(fast_dynamic_cast<AddWeaponsPlayerObjective&>(*objective));
								break;
							case 6:
								jsonObjectives.push_back(fast_dynamic_cast<PhoneCallPlayerObjective&>(*objective));
								break;
							case 7:
								jsonObjectives.push_back(fast_dynamic_cast<AddMoneyPlayerObjective&>(*objective));
								break;

							default:
								break;
						}
						break;
					case 6:
						switch (objective->getTypeCategory()) {
							//Mission
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<StartMissionObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<EndMissionObjective&>(*objective));
								break;
							default:
								break;
						}
						break;
					case 7:
						switch (objective->getTypeCategory()) {
							//Other
							case 0:
								jsonObjectives.push_back(fast_dynamic_cast<SaveObjective&>(*objective));
								break;
							case 1:
								jsonObjectives.push_back(fast_dynamic_cast<GoToSceneObjective&>(*objective));
								break;
							case 2:
								jsonObjectives.push_back(fast_dynamic_cast<JumpToObjectiveObjective&>(*objective));
								break;
							case 3:
								jsonObjectives.push_back(fast_dynamic_cast<WaitSignalObjective&>(*objective));
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
			}
			j["objectives"] = jsonObjectives;
		}

		static Scene from_json(const json &j) {
			std::vector<std::unique_ptr<BaseObjective>> objectives;
			auto jsonObjectives = j.at("objectives");
			for (const auto &jsonObjective : jsonObjectives) {
				const int category = jsonObjective["category"].get<int>();
				const int typeCategory = jsonObjective["typeCategory"].get<int>();
				switch (category) {
					case 0:
						switch (typeCategory) {
							//World
							case 0: {
								objectives.emplace_back(
									std::make_unique<CheckpointObjective>(jsonObjective.get<CheckpointObjective>()));
							}
							break;
							case 1:
								objectives.emplace_back(
									std::make_unique<CutsceneObjective>(jsonObjective.get<CutsceneObjective>()));
								break;
							case 2:
								objectives.emplace_back(
									std::make_unique<CountdownObjective>(jsonObjective.get<CountdownObjective>()));
								break;
							case 3:
								objectives.emplace_back(
									std::make_unique<TimeoutObjective>(jsonObjective.get<TimeoutObjective>()));
								break;
							case 4:
								objectives.emplace_back(
									std::make_unique<WeatherObjective>(jsonObjective.get<WeatherObjective>()));
								break;
							case 5:
								objectives.emplace_back(
									std::make_unique<ClockTimeObjective>(jsonObjective.get<ClockTimeObjective>()));
								break;
							case 6:
								objectives.emplace_back(
									std::make_unique<TrafficObjective>(jsonObjective.get<TrafficObjective>()));
								break;
							case 7:
								objectives.emplace_back(
									std::make_unique<AddTimerObjective>(jsonObjective.get<AddTimerObjective>()));
								break;
							case 8:
								objectives.emplace_back(
									std::make_unique<RemoveTimerObjective>(jsonObjective.get<RemoveTimerObjective>()));
								break;
							case 9:
								objectives.emplace_back(
									std::make_unique<SetIncrementGlobalVariable>(
										jsonObjective.get<SetIncrementGlobalVariable>()));
								break;
							default:
								break;
						}
						break;
					case 1:
						switch (typeCategory) {
							// Actor
							case 0:
								objectives.emplace_back(
									std::make_unique<KillActorOrGroupObjective>(
										jsonObjective.get<KillActorOrGroupObjective>()));
								break;
							case 1:
								objectives.emplace_back(
									std::make_unique<DamageActorObjective>(jsonObjective.get<DamageActorObjective>()));
								break;
							case 2:
								objectives.emplace_back(
									std::make_unique<FollowPathActorObjective>(
										jsonObjective.get<FollowPathActorObjective>()));
								break;
							case 3:
								objectives.emplace_back(
									std::make_unique<AnimationActorObjective>(
										jsonObjective.get<AnimationActorObjective>()));
								break;
							case 4:
								objectives.emplace_back(
									std::make_unique<EnterVehicleActorObjective>(
										jsonObjective.get<EnterVehicleActorObjective>()));
								break;
							default:
								break;
						}
						break;


					case 2:
						switch (typeCategory) {
							// Vehicle
							case 0:
								objectives.emplace_back(
									std::make_unique<GetInVehicleObjective>(
										jsonObjective.get<GetInVehicleObjective>()));
								break;
							case 1:
								objectives.emplace_back(
									std::make_unique<DestroyVehicleObjective>(
										jsonObjective.get<DestroyVehicleObjective>()));
								break;
							case 2:
								objectives.emplace_back(
									std::make_unique<FollowPathVehicleObjective>(
										jsonObjective.get<FollowPathVehicleObjective>()));
								break;
							default:
								break;
						}
						break;

					case 3:
						switch (typeCategory) {
							// Object
							case 0:
								objectives.emplace_back(
									std::make_unique<DamageObjectObjective>(
										jsonObjective.get<DamageObjectObjective>()));
								break;
							case 1:
								objectives.emplace_back(
									std::make_unique<PhotographObjectObjective>(
										jsonObjective.get<PhotographObjectObjective>()));
								break;
							case 2:
								objectives.emplace_back(
									std::make_unique<TouchObjectObjective>(jsonObjective.get<TouchObjectObjective>()));
								break;
							default:
								break;
						}
						break;

					case 4:
						switch (typeCategory) {
							// Pickup
							case 0:
								objectives.emplace_back(
									std::make_unique<CollectPickupObjective>(
										jsonObjective.get<CollectPickupObjective>()));
								break;
							default:
								break;
						}
						break;

					case 5:
						switch (typeCategory) {
							// Player
							case 0:
								objectives.emplace_back(
									std::make_unique<TeleportPlayerObjective>(
										jsonObjective.get<TeleportPlayerObjective>()));
								break;
							case 1:
								objectives.emplace_back(
									std::make_unique<AnimationPlayerObjective>(
										jsonObjective.get<AnimationPlayerObjective>()));
								break;
							case 2:
								objectives.emplace_back(
									std::make_unique<TeleportToVehiclePlayerObjective>(
										jsonObjective.get<TeleportToVehiclePlayerObjective>()));
								break;
							case 3:
								objectives.emplace_back(
									std::make_unique<LevelWantedPlayerObjective>(
										jsonObjective.get<LevelWantedPlayerObjective>()));
								break;
							case 4:
								objectives.emplace_back(
									std::make_unique<RemoveWeaponsObjective>(
										jsonObjective.get<RemoveWeaponsObjective>()));
								break;
							case 5:
								objectives.emplace_back(
									std::make_unique<AddWeaponsPlayerObjective>(
										jsonObjective.get<AddWeaponsPlayerObjective>()));
								break;
							case 6:
								objectives.emplace_back(
									std::make_unique<PhoneCallPlayerObjective>(
										jsonObjective.get<PhoneCallPlayerObjective>()));
								break;
							case 7:
								objectives.emplace_back(
									std::make_unique<AddMoneyPlayerObjective>(
										jsonObjective.get<AddMoneyPlayerObjective>()));
								break;
							default:
								break;
						}
						break;

					case 6:
						switch (typeCategory) {
							// Mission
							case 0:
								objectives.emplace_back(
									std::make_unique<StartMissionObjective>(
										jsonObjective.get<StartMissionObjective>()));
								break;
							case 1:
								objectives.emplace_back(
									std::make_unique<EndMissionObjective>(jsonObjective.get<EndMissionObjective>()));
								break;
							default:
								break;
						}
						break;

					case 7:
						switch (typeCategory) {
							// Other
							case 0:
								objectives.emplace_back(
									std::make_unique<SaveObjective>(jsonObjective.get<SaveObjective>()));
								break;
							case 1:
								objectives.emplace_back(
									std::make_unique<GoToSceneObjective>(jsonObjective.get<GoToSceneObjective>()));
								break;
							case 2:
								objectives.emplace_back(
									std::make_unique<JumpToObjectiveObjective>(
										jsonObjective.get<JumpToObjectiveObjective>()));
								break;
							case 3:
								objectives.emplace_back(
									std::make_unique<WaitSignalObjective>(jsonObjective.get<WaitSignalObjective>()));
								break;
							default:
								break;
						}
						break;

					default:
						break;
				}
			}
			return {
				j.at("name").get<std::string>(),
				j.at("id").get<int>(),
				std::move(objectives),
				j.at("actors").get<std::vector<std::unique_ptr<Actor>>>(),
				j.at("vehicles").get<std::vector<std::unique_ptr<Vehicle>>>(),
				j.at("objects").get<std::vector<std::unique_ptr<Object>>>(),
				j.at("particles").get<std::vector<std::unique_ptr<Particle>>>(),
				j.at("trains").get<std::vector<std::unique_ptr<Train>>>(),
				j.at("pickups").get<std::vector<std::unique_ptr<Pickup>>>(),
				j.at("pyrotechnics").get<std::vector<std::unique_ptr<Pyrotechnics>>>(),
				j.at("audio").get<std::vector<std::unique_ptr<Audio>>>(),
				j.at("visualEffects").get<std::vector<std::unique_ptr<VisualEffect>>>(),
				j.at("checkpoints").get<std::vector<std::unique_ptr<Checkpoint>>>(),
				j.at("sceneSettings").get<SceneSettings>(),
				j.at("toggleSceneSettings").get<bool>()
			};
		}
	};

NLOHMANN_JSON_NAMESPACE_END

void ProjectsService::update() {
	this->projectsInfos_.clear();
	this->productionProjectsInfos_.clear();
	for (const auto &entry : std::filesystem::directory_iterator(PROJECTS_PATH)) {
		if (entry.is_directory()) {
			auto missionFile = entry.path() / "index.json";
			if (exists(missionFile)) {
				std::unique_ptr<ProjectInfo> projectInfo;

				std::ifstream file(missionFile);
				{
					json j = json::parse(file);
					j.get_to(projectInfo);
				}
				file.close();

				if (auto icon = entry.path() / L"icon.png"; exists(icon)) {
					projectInfo->icon = utils::LoadTextureRequiredFromFile(icon);
				}

				projectInfo->directory = entry.path();

				this->projectsInfos_.emplace_back(std::move(projectInfo));
			}
			auto a = 1;
		} else {
			if (entry.path().extension() == ".ldpack") {
				auto entryPath = wstrToUtf8Str(entry.path().wstring());
				auto zip = zip_open(entryPath.c_str(), NULL, 'r');
				if (zip_entry_open(zip, "index.json") == 0) {
					size_t indexSize;
					void *indexBytes;
					zip_entry_read(zip, &indexBytes, &indexSize);

					std::unique_ptr<ProjectInfo> projectInfo;
					std::istringstream stream(std::string(static_cast<const char*>(indexBytes), indexSize));
					stream.rdbuf()->pubsetbuf(static_cast<char*>(indexBytes), indexSize);
					{
						json j = json::parse(stream);
						j.get_to(projectInfo);
					}

					free(indexBytes);
					zip_entry_close(zip);

					projectInfo->directory = entry.path();
					if (zip_entry_open(zip, "icon.png") == 0) {
						void *iconBytes;
						size_t iconBytesSize;
						zip_entry_read(zip, &iconBytes, &iconBytesSize);

						projectInfo->icon = utils::LoadTextureRequiredFromMemory(iconBytes, iconBytesSize);

						free(iconBytes);

						zip_entry_close(zip);
					}

					this->productionProjectsInfos_.emplace_back(std::move(projectInfo));
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
	ProjectData projectData;
	projectData.onChangedScene().swap(this->getCurrentProject().onChangedScene());
	this->currentProject_ = std::move(projectData);
	this->currentDirectory_ = std::nullopt;

	GlobalVariablesService::getInstance().fromJson("[]");
}

bool zipWalk(zip_t *zip, const std::filesystem::path &path) {
	for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
		if (entry.is_regular_file()) {
			auto entryPath = wstrToUtf8Str(entry.path().lexically_relative(path).wstring());
			zip_entry_open(zip, entryPath.c_str());

			auto filePath = wstrToUtf8Str(entry.path().wstring());
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
	getInstance().getCurrentProject().getCurrentScene()->unloadEditorScene();

	auto projectDirectoryName = std::filesystem::path(
		utils::stringToPathString(this->getCurrentProject().getProjectInfo()->name));
	const auto projectDirectory = PROJECTS_PATH / projectDirectoryName;
	auto scenesDirectory = projectDirectory / "scenes";
	auto audioDirectory = projectDirectory / "audio";

	if (this->currentDirectory_.has_value() && exists(this->currentDirectory_.value())) {
		auto checkError = [](const std::error_code &ec) {
			if (ec.value() != 0) {
				CLOG(ERROR, "LDYOM") << "invalid rename project, error: " << ec.message();
				ImGui::InsertNotification({ImGuiToastType_Error, 3000, "Error, see log and FAQ"});
				return true;
			}
			return false;
		};

		std::error_code ec;

		if (!exists(BACKUPS_PATH)) {
			create_directory(BACKUPS_PATH, ec);
			if (checkError(ec)) return;
		}

		auto backupPath = (BACKUPS_PATH / fmt::format("{}_{}.zip", this->getCurrentProject().getProjectInfo()->name,
		                                              time(nullptr))).string();
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
			ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
			return;
		}

		if (!create_directory(scenesDirectory, error)) {
			if (error.value() != 0)
				CLOG(ERROR, "LDYOM") << "Failed to create scenes directory, error: {} " << error.message();
			ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
			return;
		}

		if (!create_directory(audioDirectory, error)) {
			if (error.value() != 0)
				CLOG(ERROR, "LDYOM") << "Failed to create audio directory, error: {} " << error.message();
			ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
			return;
		}
	}

	this->getCurrentProject().getProjectInfo()->directory = projectDirectory;

	{
		//save index file
		auto &projectInfo = this->getCurrentProject().getProjectInfo();

		std::ofstream file(projectDirectory / "index.json");
		nlohmann::json j = projectInfo;
		file << j.dump(4, ' ', false, json::error_handler_t::ignore);
		file.close();
	}

	{
		// save global variables
		auto j = GlobalVariablesService::getInstance().toJson();
		std::ofstream file(projectDirectory / "globalVariables.json");
		file << j;
	}

	//save scenes
	for (const auto &pair : this->getCurrentProject().getScenes()) {
		std::ofstream file(scenesDirectory / fmt::format("{}.json", pair.first));
		{
			json j = *pair.second;
			file << j.dump(4, ' ', false, json::error_handler_t::ignore);
		}
		file.close();
	}

	this->currentDirectory_ = projectDirectory;

	getInstance().getCurrentProject().getCurrentScene()->loadEditorScene();

	ImGui::InsertNotification({ImGuiToastType_Success, 3000, "Saved!"});
}

void ProjectsService::loadProjectData(const std::filesystem::path &projectDirectory) {
	auto scenesDirectory = projectDirectory / "scenes";

	{
		//load index file
		std::unique_ptr<ProjectInfo> projectInfo;
		std::ifstream file(projectDirectory / "index.json");
		{
			json j = json::parse(file);
			j.get_to(projectInfo);
		}
		file.close();
		this->getCurrentProject().getProjectInfo().swap(projectInfo);
		this->getCurrentProject().getProjectInfo()->directory = projectDirectory;
	}

	{
		// load global variables
		std::ifstream file(projectDirectory / "globalVariables.json");
		{
			std::stringstream ss;
			ss << file.rdbuf();
			GlobalVariablesService::getInstance().fromJson(ss.str());
		}
		file.close();
	}

	//load scenes
	for (auto &path : std::filesystem::directory_iterator(scenesDirectory)) {
		if (path.is_regular_file() && path.path().extension() == ".json") {
			auto scene = std::make_unique<Scene>();
			std::ifstream file(path.path(), std::ios_base::binary);
			{
				json j = json::parse(file);
				auto s = j.get<Scene>();
				scene->getName() = s.getName();
				scene->getId() = s.getId();
				scene->getActors().swap(s.getActors());
				scene->getVehicles().swap(s.getVehicles());
				scene->getObjects().swap(s.getObjects());
				scene->getParticles().swap(s.getParticles());
				scene->getTrains().swap(s.getTrains());
				scene->getPickups().swap(s.getPickups());
				scene->getPyrotechnics().swap(s.getPyrotechnics());
				scene->getAudio().swap(s.getAudio());
				scene->getVisualEffects().swap(s.getVisualEffects());
				scene->getCheckpoints().swap(s.getCheckpoints());
				scene->getSceneSettings() = s.getSceneSettings();
				scene->isToggleSceneSettings() = s.isToggleSceneSettings();
				scene->getObjectives().swap(s.getObjectives());
			}
			file.close();

			int sceneId = std::stoi(path.path().stem().wstring());

			getCurrentProject().getScenes().emplace(sceneId, std::move(scene));
		}
	}

	this->currentDirectory_ = projectDirectory;
}

void ProjectsService::loadProject(int projectIdx) {
	getCurrentProject().getCurrentScene()->unloadEditorScene();

	//clear all scenes
	getCurrentProject().getScenes().clear();

	const auto &projectDirectory = projectsInfos_.at(projectIdx)->directory;

	this->loadProjectData(projectDirectory);

	this->getCurrentProject().getCurrentSceneIndex() = projectsInfos_.at(projectIdx)->startScene;

	Audio::loadAudioFilesList();

	this->getCurrentProject().getCurrentScene()->loadEditorScene();

	getCurrentProject().onChangedScene()();

	Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectivesWindow>()->selectElementFirstCall(
		this->getCurrentProject().getCurrentScene()->getObjectives().size() - 1);

	this->onUpdate_();

	DiscordService::getInstance().updateActivity(projectsInfos_.at(projectIdx)->name, DiscordActivityType::CREATING);

	ImGui::InsertNotification({ImGuiToastType_Success, 3000, "Loaded!"});
}

void ProjectsService::loadProductionProject(int projectIdx) {
	getCurrentProject().getCurrentScene()->unloadEditorScene();

	//clear all scenes
	getCurrentProject().getScenes().clear();

	const auto &projectDirectory = std::filesystem::temp_directory_path() / productionProjectsInfos_.at(projectIdx)->
		directory.stem();
	if (exists(projectDirectory)) {
		remove_all(projectDirectory);
	}
	if (create_directory(projectDirectory)) {
		const auto extractPath = wstrToUtf8Str(productionProjectsInfos_.at(projectIdx)->directory.wstring());
		const auto extractToDir = wstrToUtf8Str(projectDirectory.wstring());
		auto code = zip_extract(extractPath.c_str(),
		                        extractToDir.c_str(), nullptr, nullptr);
		this->loadProjectData(projectDirectory);
		this->getCurrentProject().getCurrentSceneIndex() = productionProjectsInfos_.at(projectIdx)->startScene;
	}
}

void ProjectsService::deleteProject(int projectIdx) const {
	const auto &projectDirectory = projectsInfos_.at(projectIdx)->directory;
	remove_all(projectDirectory);
}

void ProjectsService::deleteProductionProject(int projectIdx) const {
	const auto &projectDirectory = productionProjectsInfos_.at(projectIdx)->directory;
	remove(projectDirectory);
}

void ProjectsService::makeProjectProduction(int projectIdx) const {
	const auto pathToPack = (PROJECTS_PATH / fmt::format("{}.ldpack", this->projectsInfos_.at(projectIdx)->name)).
		string();
	const auto zip = zip_open(pathToPack.c_str(), 9, 'w');
	if (!zipWalk(zip, this->projectsInfos_.at(projectIdx)->directory))
		return;
	zip_close(zip);

	ImGui::InsertNotification({ImGuiToastType_Success, 3000, "Saved public project!"});
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
