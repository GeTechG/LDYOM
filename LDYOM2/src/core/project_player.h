#pragma once
#include "ktcoro_wait.hpp"
#include "scene.h"
#include "scene_info.h"
#include <functional>
#include <memory>
#include <optional>
#include <rocket.hpp>
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
	~ProjectPlayer();

	static ktwait run();
	static ktwait processStopCheat();

  public:
	std::unique_ptr<ktcoro_tasklist> projectTasklist = std::make_unique<ktcoro_tasklist>();

	rocket::thread_safe_signal<void(std::string)> signals;
	rocket::thread_safe_signal<void()> onProjectStarted;
	rocket::thread_safe_signal<void()> onProjectStopped;
	rocket::thread_safe_signal<void(int)> onObjectiveStarted;
	rocket::thread_safe_signal<void(int)> onObjectiveCompleted;

	static ProjectPlayer& instance();

	ProjectPlayer(const ProjectPlayer&) = delete;
	void operator=(const ProjectPlayer&) = delete;

	void startCurrentProject(std::string_view sceneId = "");
	void stopCurrentProject();
	void failCurrentProject();
	void transitionPlayingState(bool toPlayMode);

	bool isPlaying() const { return m_state.isPlaying; }
	std::string getCurrentSceneId() const { return m_state.currentSceneId; }
	int getCurrentObjectiveIndex() const { return m_state.currentObjectiveIndex; }
};

#define IS_PLAYING ProjectPlayer::instance().isPlaying()