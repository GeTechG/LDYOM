#include "project_player.h"
#include "objectives_manager.h"
#include "projects_manager.h"
#include "scenes_manager.h"
#include <logger.h>
#include <task_manager.h>

ktwait ProjectPlayer::run() {
	ProjectPlayer::instance().m_state.isPlaying = true;
	LDYOM_INFO("Project player started");

	// code

	ProjectPlayer::instance().m_state.isPlaying = false;
	LDYOM_INFO("Project player finished");
	co_return;
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
	if (!TaskManager::instance().addTask("run_project_player", ProjectPlayer::run)) {
		LDYOM_ERROR("Failed to add task for project player");
	}
}
