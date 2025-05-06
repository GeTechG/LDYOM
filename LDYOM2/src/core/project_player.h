#pragma once
#include "ktcoro_wait.hpp"
#include "scene.h"
#include "scene_info.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>

struct ProjectPlayerState {
	bool isPlaying = false;
	int currentObjectiveIndex = 0;
	std::string currentSceneId;
};

class ProjectPlayer {
  private:
	ProjectPlayerState m_state;

	ProjectPlayer() = default;

	static ktwait run();

  public:
	static ProjectPlayer& instance();

	ProjectPlayer(const ProjectPlayer&) = delete;
	void operator=(const ProjectPlayer&) = delete;

	void startCurrentProject(std::string_view sceneId = "");

	bool isPlaying() const { return m_state.isPlaying; }
	std::string getCurrentSceneId() const { return m_state.currentSceneId; }
	int getCurrentObjectiveIndex() const { return m_state.currentObjectiveIndex; }
};