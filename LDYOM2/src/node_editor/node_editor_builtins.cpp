#include "node_editor_builtins.h"
#include "lua_node.h"
#include "node_registry.h"
#include <lua_manager.h>
#include <sol/sol.hpp>
#include <utils/imgui_configurate.h>

// Registers built-in nodes through the NodeRegistry.
// Callbacks are plain C++ lambdas wrapped as sol::protected_function.
void registerBuiltinNodes() {
	auto guard = LuaManager::instance().getState();
	auto& lua = guard.get();

	// Helper: load a Lua chunk that returns a function
	auto loadFn = [&](const char* code) -> sol::protected_function {
		sol::load_result lr = lua.load(code);
		if (!lr.valid())
			return sol::protected_function{};
		// The chunk returns the actual callback when executed
		sol::protected_function wrapper = lr;
		auto res = wrapper();
		if (!res.valid())
			return sol::protected_function{};
		return res.get<sol::protected_function>();
	};

	// ── Number ────────────────────────────────────────────────────────────
	{
		sol::table desc = lua.create_table();
		desc["type"] = "builtin.number";
		desc["title"] = "Number";
		desc["category"] = "Math";
		desc["style"] = "cyan";

		sol::table pins = lua.create_table();
		sol::table outPin = lua.create_table();
		outPin["title"] = "Value";
		outPin["type"] = "float";
		outPin["dir"] = "out";
		outPin["default"] = 0.0;
		outPin["behaviour"] = [](std::shared_ptr<LuaNodeHandle> handle) -> sol::object {
			if (!handle)
				return sol::lua_nil;
			auto valObj = handle->getData("value");
			return valObj;
		};
		pins[1] = outPin;
		desc["pins"] = pins;
		desc["on_draw"] = [](std::shared_ptr<LuaNodeHandle> handle, sol::this_state s) {
			if (!handle)
				return;
			sol::state_view lua(s);
			sol::object valObj = handle->getData("value");
			double val = 0.0;
			if (valObj.is<double>()) {
				val = valObj.as<double>();
			}
			ImGui::PushItemWidth(80.f * (SCL_PX).x);
			if (ImGui::InputDouble("##Value", &val)) {
				handle->setData("value", sol::make_object(lua, val));
			}
		};
		NodeRegistry::instance().registerNode(desc);
	}
}
