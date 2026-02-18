#include "project_player_binding.h"
#include "core/project_player.h"
#include <logger.h>
#include <map>
#include <rocket.hpp>
#include <string>

// ── per-event subscriber maps ────────────────────────────────────────────────

struct LuaProjectPlayerHandlers {
	std::map<int, sol::protected_function> onSceneStarted;
	std::map<int, sol::protected_function> onObjectiveStarted;
	std::map<int, sol::protected_function> onObjectiveCompleted;

	std::vector<rocket::scoped_connection> connections;
	int nextId = 0;
};

static LuaProjectPlayerHandlers& getHandlers() {
	static LuaProjectPlayerHandlers h;
	return h;
}

// ── helpers ──────────────────────────────────────────────────────────────────

template <typename... Args>
static void fireHandlers(std::map<int, sol::protected_function>& handlers, const char* name, Args&&... args) {
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

	h.connections.push_back(ProjectPlayer::instance().onSceneStarted.connect([](const std::string& sceneId) {
		fireHandlers(getHandlers().onSceneStarted, "events.on_scene_started", sceneId);
	}));

	h.connections.push_back(ProjectPlayer::instance().onObjectiveStarted.connect([](int index) {
		fireHandlers(getHandlers().onObjectiveStarted, "events.on_objective_started", index);
	}));

	h.connections.push_back(ProjectPlayer::instance().onObjectiveCompleted.connect([](int index) {
		fireHandlers(getHandlers().onObjectiveCompleted, "events.on_objective_completed", index);
	}));
}

// ── public API ────────────────────────────────────────────────────────────────

void clear_project_player_lua_callbacks() {
	auto& h = getHandlers();
	h.onSceneStarted.clear();
	h.onObjectiveStarted.clear();
	h.onObjectiveCompleted.clear();
}

void register_project_player_bindings(sol::state_view lua) {
	initConnections();

	// Raw C++ functions (subscribe returns int id, unsubscribe takes event+id)
	sol::table raw = lua.create_table("_ppe_raw");

	raw["_subscribe"] = [](const std::string& event, sol::protected_function fn) -> int {
		auto& h = getHandlers();
		int id = h.nextId++;
		if (event == "scene_started")
			h.onSceneStarted[id] = std::move(fn);
		else if (event == "objective_started")
			h.onObjectiveStarted[id] = std::move(fn);
		else if (event == "objective_completed")
			h.onObjectiveCompleted[id] = std::move(fn);
		else
			LDYOM_WARN("events: unknown event '{}'", event);
		return id;
	};

	raw["_unsubscribe"] = [](const std::string& event, int id) {
		auto& h = getHandlers();
		if (event == "scene_started")
			h.onSceneStarted.erase(id);
		else if (event == "objective_started")
			h.onObjectiveStarted.erase(id);
		else if (event == "objective_completed")
			h.onObjectiveCompleted.erase(id);
	};

	// Public `events` table with convenient wrappers that return a disconnect fn
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

		events.on_scene_started       = make_subscriber("scene_started")
		events.on_objective_started   = make_subscriber("objective_started")
		events.on_objective_completed = make_subscriber("objective_completed")

		_ppe_raw = nil
	)");
}
