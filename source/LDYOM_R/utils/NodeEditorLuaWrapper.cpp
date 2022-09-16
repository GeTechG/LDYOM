#include "NodeEditorLuaWrapper.h"

#include "NodeEditorUtils.h"

void NodeEditorLuaWrapper::wrap(sol::state& state) {
	auto table = state.create_table("NodeEditor");
	table.set_function("registerNode", [&state](const char* category, const sol::metatable& nodeClass) {
		auto nodes = state["global_data"]["nodes"].get_or_create<sol::table>();
		nodes.add(nodeClass);
		state["global_data"]["nodes_categories"]
			.get_or_create<sol::table>()[category]
			.get_or_create<sol::table>()
			.add(nodes.size());
		state["bitser"]["registerClass"](nodeClass);
	});
}
