#include "ProjectsService.h"

#include "Logger.h"
#include <filesystem>

#include "FileWatcher.h"
#include "utils.h"
#include "fmt/core.h"
#include "imgui_notify.h"
#include "WindowsRenderService.h"
#include "../Windows/ObjectivesWindow.h"

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/unique_ptr.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/array.hpp"
#include "boost/serialization/utility.hpp"
#include "boost/uuid/uuid_serialize.hpp"


const std::filesystem::path PROJECTS_PATH = L"LDYOM\\Projects\\";

std::mutex ProjectsService::mutex_;

void ProjectsService::update() {
	this->projectsInfos_.clear();
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
		}
	}
}

void ProjectsService::createNewProject() {
	this->getCurrentProject().onChangedScene()();
	this->getCurrentProject().getCurrentScene()->unloadEditorScene();
	this->currentProject_ = ProjectData();
	this->currentDirectory_ = std::nullopt;
}

void ProjectsService::saveCurrentProject() {
	const auto projectDirectory = PROJECTS_PATH / std::filesystem::path(utils::stringToPathString(this->getCurrentProject().getProjectInfo()->name));
	auto scenesDirectory = projectDirectory / "scenes";

	if (this->currentDirectory_.has_value() && exists(this->currentDirectory_.value())) {
		std::filesystem::rename(this->currentDirectory_.value(), projectDirectory);
	}

	if (!exists(projectDirectory)) {
		//create directories
		std::error_code error;
		if (!create_directory(projectDirectory, error)) {
			Logger::getInstance().log(fmt::format("Failed to create the project directory, error: {}", error.message()));
			throw std::exception(error.message().c_str());
		}

		if (!create_directory(scenesDirectory, error)) {
			Logger::getInstance().log(fmt::format("Failed to create scenes directory, error: {}", error.message()));
			throw std::exception(error.message().c_str());
		}
	}

	{
		//save index file
		std::ofstream file(projectDirectory / "index.dat", std::ios_base::binary);
		boost::archive::binary_oarchive oa(file);
		oa << this->getCurrentProject().getProjectInfo().get();
		file.close();
	}

	//save scenes
	for (const auto& pair: this->getCurrentProject().getScenes()) {
		std::ofstream file(scenesDirectory / fmt::format("{}.dat", pair.first), std::ios_base::binary);
		boost::archive::binary_oarchive oa(file);
		oa << pair.second;
		file.close();
	}

	this->currentDirectory_ = projectDirectory;

	ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Saved!" });
}

void ProjectsService::loadProject(int projectIdx) {
	getCurrentProject().getCurrentScene()->unloadEditorScene();

	//clear all scenes
	getCurrentProject().getScenes().clear();

	const auto & projectDirectory = projectsInfos_.at(projectIdx)->directory;
	auto scenesDirectory = projectDirectory / "scenes";

	{
		//load index file
		std::ifstream file(projectDirectory / "index.dat", std::ios_base::binary);
		boost::archive::binary_iarchive ia(file);
		ProjectInfo* projectInfo;
		ia >> projectInfo;
		this->getCurrentProject().getProjectInfo() = std::unique_ptr<ProjectInfo>(projectInfo);
		file.close();
	}

	//load scenes
	for (auto &path : std::filesystem::directory_iterator(scenesDirectory)) {
		if (path.is_regular_file() && path.path().extension() == ".dat") {
			std::ifstream file(path.path(), std::ios_base::binary);
			boost::archive::binary_iarchive ia(file);
			std::unique_ptr<Scene> scene;
			ia >> scene;

			getCurrentProject().getScenes().emplace(std::stoi(path.path().stem().wstring()), std::move(scene));
			file.close();
		}
	}

	this->currentDirectory_ = projectDirectory;
	this->getCurrentProject().getCurrentSceneIndex() = projectsInfos_.at(projectIdx)->startScene;

	this->getCurrentProject().getCurrentScene()->loadEditorScene();

	getCurrentProject().onChangedScene()();

	Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectivesWindow>()->selectElement(this->getCurrentProject().getCurrentScene()->getObjectives().size() - 1);

	ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Loaded!" });
}

void ProjectsService::deleteProject(int projectIdx) const {
	const auto& projectDirectory = projectsInfos_.at(projectIdx)->directory;
	remove_all(projectDirectory);
}

std::vector<std::unique_ptr<ProjectInfo>>& ProjectsService::getProjectsInfos() {
	return projectsInfos_;
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

	}
}