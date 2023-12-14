#include "ProjectData.h"

#include "LuaLogger.h"
#include "easylogging/easylogging++.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

ProjectData::ProjectData() {
	newScene(false);
	this->projectInfo_ = std::make_unique<ProjectInfo>();
	this->projectInfo_->name = fmt::format("{} #{}", Localization::getInstance().get("projects.project"),
	                                       time(nullptr));
	this->projectInfo_->startScene = this->currentScene_;
	this->projectInfo_->directory.clear();
}

ProjectData& ProjectData::operator=(ProjectData &&other) noexcept {
	if (this == &other)
		return *this;
	scenes_ = std::move(other.scenes_);
	onChangedScene_ = std::move(other.onChangedScene_);
	projectInfo_ = std::move(other.projectInfo_);
	currentScene_ = other.currentScene_;
	return *this;
}

Scene* ProjectData::getCurrentScene() {
	return this->getScenes().at(this->currentScene_).get();
}

int& ProjectData::getCurrentSceneIndex() {
	return this->currentScene_;
}

boost::signals2::signal<void()>& ProjectData::onChangedScene() {
	return onChangedScene_;
}

void ProjectData::newScene(bool change) {
	auto &local = Localization::getInstance();
	const auto newName = fmt::format("{} #{}", local.get("scenes.scene").c_str(), this->scenes_.size());
	auto scene = std::make_unique<Scene>(newName.c_str());
	int sceneId = reinterpret_cast<int>(scene.get());
	this->scenes_.emplace(sceneId, std::move(scene));
	if (change)
		changeScene(sceneId);
	else {
		this->currentScene_ = sceneId;
		this->getCurrentScene()->loadEditorScene();
		this->onChangedScene_();
	}
	/*for (auto pair : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onNewScene"].get_or_create<sol::table>()) {
		if (auto result = pair.second.as<sol::function>()(sceneId); !result.valid()) {
			sol::error err = result;
			CLOG(ERROR, "lua") << err.what();
			LuaLogger::getInstance().print(err.what());
		}
	}*/
}

void ProjectData::changeScene(int scene) {
	this->getCurrentScene()->unloadEditorScene();
	this->currentScene_ = scene;
	this->getCurrentScene()->loadEditorScene();
	this->onChangedScene_();
	/*for (auto pair : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onChangeScene"].get_or_create<sol::table>()) {
		if (auto result = pair.second.as<sol::function>()(scene); !result.valid()) {
			sol::error err = result;
			CLOG(ERROR, "lua") << err.what();
			LuaLogger::getInstance().print(err.what());
		}
	}*/
}

void ProjectData::deleteScene(int scene) {
	if (scene == currentScene_)
		this->getCurrentScene()->unloadEditorScene();
	this->scenes_.erase(scene);
	if (scene == currentScene_) {
		this->currentScene_ = this->scenes_.begin()->first;
		this->getCurrentScene()->loadEditorScene();
	}
	/*for (auto pair : LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onDeleteScene"].get<sol::table>()) {
		if (auto result = pair.second.as<sol::function>()(scene); !result.valid()) {
			sol::error err = result;
			CLOG(ERROR, "lua") << err.what();
			LuaLogger::getInstance().print(err.what());
		}
	}*/
}

std::unordered_map<int, std::unique_ptr<Scene>>& ProjectData::getScenes() {
	return scenes_;
}

std::unique_ptr<ProjectInfo>& ProjectData::getProjectInfo() {
	return projectInfo_;
}
