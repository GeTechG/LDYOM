#include "project_player.h"
#include "objectives_manager.h"
#include "projects_manager.h"
#include "scenes_manager.h"
#include <logger.h>
#include <task_manager.h>
#include <extensions/ScriptCommands.h>

ProjectPlayer::~ProjectPlayer() { this->projectTasklist->clear_all_tasks(); }

ktwait ProjectPlayer::run() {
	instance().m_state.isPlaying = true;
	instance().transitionPlayingState(true);
	LDYOM_INFO("Project player started");

	auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;

	for (int i = 0; i < static_cast<int>(objectives.size()); i++) {
		instance().m_state.currentObjectiveIndex = i;
		instance().onObjectiveStarted(i);
		auto& objective = ObjectivesManager::instance().getUnsafeObjective(i);
		co_await objective.execute();
		instance().onObjectiveCompleted(i);
	}

	instance().m_state.isPlaying = false;
	instance().transitionPlayingState(false);
	LDYOM_INFO("Project player finished");
}

ktwait ProjectPlayer::processStopCheat() {
	while (!plugin::Command<0x0ADC>("LDSTOP")) {
		co_await 1;
	}
	instance().stopCurrentProject();
}

ProjectPlayer& ProjectPlayer::instance() {
	static ProjectPlayer instance;
	return instance;
}

void ProjectPlayer::startCurrentProject(std::string_view sceneId) {
	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value()) {
		LDYOM_ERROR("No current project set, cannot start project player");
		return;
	}
	this->m_state.currentSceneId = sceneId.empty() ? currentProject.value()->startSceneId : std::string(sceneId);

	if (!TaskManager::instance().addTask("run_project_player", run)) {
		LDYOM_ERROR("Failed to add task for project player");
	}
}

void ProjectPlayer::stopCurrentProject() {
	if (!this->m_state.isPlaying) {
		LDYOM_ERROR("Project player is not running, cannot stop it");
		return;
	}
	TaskManager::instance().removeTask("run_project_player");

	this->m_state.isPlaying = false;
	transitionPlayingState(false);
	LDYOM_INFO("Project player stopped");
}

void ProjectPlayer::failCurrentProject() { this->stopCurrentProject(); }

void ProjectPlayer::transitionPlayingState(bool toPlayMode) {
	if (toPlayMode) {
		TaskManager::instance().addTask("stop_cheat", processStopCheat);
		instance().projectTasklist->clear_all_tasks();
		TaskManager::instance().addTask("project_tasks", []() -> ktwait {
			while (instance().m_state.isPlaying) {
				instance().projectTasklist->process();
				co_await 1;
			}
		});
	} else {
		TaskManager::instance().removeTask("stop_cheat");
		TaskManager::instance().removeTask("project_tasks");
	}
	ScenesManager::instance().resetCurrentScene();
}