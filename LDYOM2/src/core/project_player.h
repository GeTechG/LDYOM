#pragma once
#include "ktcoro_wait.hpp"
#include "scene.h"
#include "scene_info.h"
#include <functional>
#include <memory>
#include <optional>
#include <rocket.hpp>
#include <string>
#include <variant>
#include <nlohmann/json.hpp>

struct PendingTransition {
	std::string sceneId;
	bool instant;
};

namespace mission_fail_actions {
	struct EndProject {
		static constexpr const char* TYPE = "end_project";
		bool fadeOut = true;
	};

	inline void to_json(nlohmann::json& j, const EndProject& e) {
		j = nlohmann::json::object();
		j["fadeOut"] = e.fadeOut;
	}
	inline void from_json(const nlohmann::json& j, EndProject& e) {
		e.fadeOut = j.value("fadeOut", true);
	}

	struct RestartScene {
		static constexpr const char* TYPE = "restart_scene";
		bool fadeOut = true;
	};

	inline void to_json(nlohmann::json& j, const RestartScene& r) {
		j = nlohmann::json::object();
		j["fadeOut"] = r.fadeOut;
	}
	inline void from_json(const nlohmann::json& j, RestartScene& r) {
		r.fadeOut = j.value("fadeOut", true);
	}

	struct GotoScene {
		static constexpr const char* TYPE = "goto_scene";
		std::string sceneId;
		std::optional<std::string> objectiveId;
		bool fadeOut = true;
	};

	inline void to_json(nlohmann::json& j, const GotoScene& g) {
		j = nlohmann::json::object();
		j["sceneId"] = g.sceneId;
		if (g.objectiveId.has_value()) {
			j["objectiveId"] = g.objectiveId.value();
		}
		j["fadeOut"] = g.fadeOut;
	}
	inline void from_json(const nlohmann::json& j, GotoScene& g) {
		g.sceneId = j.at("sceneId").get<std::string>();
		if (j.contains("objectiveId") && !j.at("objectiveId").is_null()) {
			g.objectiveId = j.at("objectiveId").get<std::string>();
		} else {
			g.objectiveId = std::nullopt;
		}
		g.fadeOut = j.value("fadeOut", true);
	}
} // namespace mission_fail_actions

using MissionFailAction = std::variant<mission_fail_actions::EndProject, mission_fail_actions::RestartScene, mission_fail_actions::GotoScene>;

namespace nlohmann {
	template <>
	struct adl_serializer<MissionFailAction> {
		static void to_json(json& j, const MissionFailAction& action) {
			std::visit([&j](const auto& a) {
				using T = std::decay_t<decltype(a)>;
				json data = a;
				j = {{"type", T::TYPE}, {"data", data}};
			}, action);
		}

		static void from_json(const json& j, MissionFailAction& action) {
			const std::string type = j.at("type").get<std::string>();
			const auto& data = j.at("data");
			if (type == mission_fail_actions::EndProject::TYPE) {
				action = data.get<mission_fail_actions::EndProject>();
			} else if (type == mission_fail_actions::RestartScene::TYPE) {
				action = data.get<mission_fail_actions::RestartScene>();
			} else if (type == mission_fail_actions::GotoScene::TYPE) {
				action = data.get<mission_fail_actions::GotoScene>();
			} else {
				throw std::runtime_error("Unknown MissionFailAction type: " + type);
			}
		}
	};
} // namespace nlohmann

struct MissionModeState {
	bool active = false;
	int failTextMode = 0; // 0 = built-in M_FAIL GXT, 1 = author-supplied custom text
	std::string failTextGxt; // cp1251 + gxt-encoded custom fail text (only used when failTextMode == 1)
	MissionFailAction failAction = mission_fail_actions::EndProject{};
};

struct ProjectPlayerState {
	bool isPlaying = false;
	int currentObjectiveIndex = 0;
	std::string currentSceneId;
	bool isFaded = false; // Tracks fade state: true = screen is black, false = screen is visible (analog of $DYOM_faded)
	std::optional<PendingTransition> pendingSceneTransition;
	std::optional<std::string> pendingObjectiveJump; // UUID of objective to jump to
	MissionModeState missionMode;
};

class ProjectPlayer {
  private:
	ProjectPlayerState m_state;

	ProjectPlayer() = default;
	~ProjectPlayer();

	static ktwait run();
	static ktwait runScenesLoop();
	static ktwait processStopCheat();
	static ktwait playerLeaveAnyVehicle();
	static ktwait missionFailSequence();
	static void navigationalArmContinue(const std::string& sceneId, const std::optional<std::string>& objectiveId);

  public:
	static constexpr int MISSION_FAIL_TEXT_TIME_MS = 1000;
	static constexpr int MISSION_FADE_TIME_MS = 500;

	std::unique_ptr<ktcoro_tasklist> projectTasklist = std::make_unique<ktcoro_tasklist>();

	rocket::thread_safe_signal<void(std::string)> signals;
	rocket::thread_safe_signal<void()> onProjectStarted;
	rocket::thread_safe_signal<void()> onProjectStopped;
	rocket::thread_safe_signal<void(std::string)> onSceneStarted;
	rocket::thread_safe_signal<void(std::string)> onSceneEnded;
	rocket::thread_safe_signal<void(int)> onObjectiveStarted;
	rocket::thread_safe_signal<void(int)> onObjectiveCompleted;

	static ProjectPlayer& instance();

	ProjectPlayer(const ProjectPlayer&) = delete;
	void operator=(const ProjectPlayer&) = delete;

	void startCurrentProject(std::string_view sceneId = "");
	void stopCurrentProject();
	void failCurrentProject();
	void transitionPlayingState(bool toPlayMode);
	void requestSceneTransition(std::string_view sceneId, bool instant = false);
	void requestObjectiveJump(std::string_view objectiveId);

	bool isPlaying() const { return m_state.isPlaying; }
	std::string getCurrentSceneId() const { return m_state.currentSceneId; }
	int getCurrentObjectiveIndex() const { return m_state.currentObjectiveIndex; }
	bool isFaded() const { return m_state.isFaded; }
	void setFaded(bool faded) { m_state.isFaded = faded; }
	bool isMissionActive() const { return m_state.missionMode.active; }
	void setMissionMode(int failTextMode, std::string failTextGxt, MissionFailAction action) {
		m_state.missionMode.active = true;
		m_state.missionMode.failTextMode = failTextMode;
		m_state.missionMode.failTextGxt = std::move(failTextGxt);
		m_state.missionMode.failAction = std::move(action);
	}
	void clearMissionMode() { m_state.missionMode = {}; }
	std::vector<Entity*> getEntities();
};

#define IS_PLAYING ProjectPlayer::instance().isPlaying()