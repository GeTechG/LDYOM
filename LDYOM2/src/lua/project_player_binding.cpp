#include "project_player_binding.h"
#include "core/project_player.h"
#include <entity.h>
#include <logger.h>
#include <rocket.hpp>
#include <string>
#include <unordered_map>

// ── per-event subscriber maps ────────────────────────────────────────────────

struct LuaProjectPlayerHandlers {
	std::unordered_map<int, sol::protected_function> onProjectStarted;
	std::unordered_map<int, sol::protected_function> onProjectStopped;
	std::unordered_map<int, sol::protected_function> onSceneStarted;
	std::unordered_map<int, sol::protected_function> onObjectiveStarted;
	std::unordered_map<int, sol::protected_function> onObjectiveCompleted;
	std::unordered_map<int, sol::protected_function> onSignal;

	std::vector<rocket::scoped_connection> connections;
	int nextId = 0;
};

static LuaProjectPlayerHandlers& getHandlers() {
	static LuaProjectPlayerHandlers h;
	return h;
}

// ── helpers ──────────────────────────────────────────────────────────────────

template <typename... Args>
static void fireHandlers(std::unordered_map<int, sol::protected_function>& handlers, const char* name, Args&&... args) {
	for (auto& [id, fn] : handlers) {
		auto result = fn(std::forward<Args>(args)...);
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("Lua {} callback error: {}", name, err.what());
		}
	}
}

// ── connect to ProjectPlayer signals once ────────────────────────────────────

static void initConnections() {
	static bool connected = false;
	if (connected)
		return;
	connected = true;

	auto& h = getHandlers();

	h.connections.push_back(ProjectPlayer::instance().onProjectStarted.connect(
		[]() { fireHandlers(getHandlers().onProjectStarted, "events.on_project_started"); }));

	h.connections.push_back(ProjectPlayer::instance().onProjectStopped.connect(
		[]() { fireHandlers(getHandlers().onProjectStopped, "events.on_project_stopped"); }));

	h.connections.push_back(ProjectPlayer::instance().onSceneStarted.connect([](const std::string& sceneId) {
		fireHandlers(getHandlers().onSceneStarted, "events.on_scene_started", sceneId);
	}));

	h.connections.push_back(ProjectPlayer::instance().onObjectiveStarted.connect(
		[](int index) { fireHandlers(getHandlers().onObjectiveStarted, "events.on_objective_started", index); }));

	h.connections.push_back(ProjectPlayer::instance().onObjectiveCompleted.connect(
		[](int index) { fireHandlers(getHandlers().onObjectiveCompleted, "events.on_objective_completed", index); }));

	h.connections.push_back(ProjectPlayer::instance().signals.connect(
		[](const std::string& signal) { fireHandlers(getHandlers().onSignal, "events.on_signal", signal); }));
}

// ── public API ────────────────────────────────────────────────────────────────

void clear_project_player_lua_callbacks() {
	auto& h = getHandlers();
	h.onProjectStarted.clear();
	h.onProjectStopped.clear();
	h.onSceneStarted.clear();
	h.onObjectiveStarted.clear();
	h.onObjectiveCompleted.clear();
	h.onSignal.clear();
}

void register_project_player_bindings(sol::state_view lua) {
	initConnections();

	// ── ProjectPlayer usertype ────────────────────────────────────────────────

	lua.new_usertype<ProjectPlayer>(
		"ProjectPlayer", sol::no_constructor,

		// Control
		"start",
		[](ProjectPlayer& pp, sol::optional<std::string> sceneId) { pp.startCurrentProject(sceneId.value_or("")); },
		"stop", &ProjectPlayer::stopCurrentProject, "fail", &ProjectPlayer::failCurrentProject,
		"request_scene_transition",
		[](ProjectPlayer& pp, const std::string& sceneId) { pp.requestSceneTransition(sceneId); },

		// State (read-only properties, except is_faded which is read-write)
		"is_playing", sol::property(&ProjectPlayer::isPlaying), "current_scene_id",
		sol::property(&ProjectPlayer::getCurrentSceneId), "current_objective_index",
		sol::property(&ProjectPlayer::getCurrentObjectiveIndex), "is_faded",
		sol::property(&ProjectPlayer::isFaded, &ProjectPlayer::setFaded),

		// Signals
		"send_signal",
		[](ProjectPlayer& pp, const std::string& signal) { pp.signals(signal); },

		// Entities
		"get_entities",
		[](ProjectPlayer& pp, sol::this_state s) {
			sol::state_view l(s);
			auto entities = pp.getEntities();
			return sol::as_table(entities);
		});

	// Expose singleton as `project_player`
	lua["project_player"] = &ProjectPlayer::instance();

	// ── Events ────────────────────────────────────────────────────────────────

	sol::table raw = lua.create_table("_ppe_raw");

	raw["_subscribe"] = [](const std::string& event, sol::protected_function fn) -> int {
		auto& h = getHandlers();
		int id = h.nextId++;
		if (event == "project_started")
			h.onProjectStarted[id] = std::move(fn);
		else if (event == "project_stopped")
			h.onProjectStopped[id] = std::move(fn);
		else if (event == "scene_started")
			h.onSceneStarted[id] = std::move(fn);
		else if (event == "objective_started")
			h.onObjectiveStarted[id] = std::move(fn);
		else if (event == "objective_completed")
			h.onObjectiveCompleted[id] = std::move(fn);
		else if (event == "signal")
			h.onSignal[id] = std::move(fn);
		else
			LDYOM_WARN("events: unknown event '{}'", event);
		return id;
	};

	raw["_unsubscribe"] = [](const std::string& event, int id) {
		auto& h = getHandlers();
		if (event == "project_started")
			h.onProjectStarted.erase(id);
		else if (event == "project_stopped")
			h.onProjectStopped.erase(id);
		else if (event == "scene_started")
			h.onSceneStarted.erase(id);
		else if (event == "objective_started")
			h.onObjectiveStarted.erase(id);
		else if (event == "objective_completed")
			h.onObjectiveCompleted.erase(id);
		else if (event == "signal")
			h.onSignal.erase(id);
	};

	// Public `events` table – each subscriber returns a disconnect function
	lua.script(R"(
		local _r = _ppe_raw
		events = events or {}

		local function make_subscriber(event_name)
			return function(callback)
				local id = _r._subscribe(event_name, callback)
				return function()
					_r._unsubscribe(event_name, id)
				end
			end
		end

		events.on_project_started     = make_subscriber("project_started")
		events.on_project_stopped     = make_subscriber("project_stopped")
		events.on_scene_started       = make_subscriber("scene_started")
		events.on_objective_started   = make_subscriber("objective_started")
		events.on_objective_completed = make_subscriber("objective_completed")
		events.on_signal              = make_subscriber("signal")

		_ppe_raw = nil
	)");
}
