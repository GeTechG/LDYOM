#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor_binding.h"
#include "window_manager.h"
#include "windows/node_editor.h"
#include <ImNodeFlow.h>
#include <lua_node.h>
#include <node_registry.h>

void register_node_editor_bindings(sol::state_view lua) {
	// Extend the existing node_editor table (created by NodeRegistry::sol_lua_register)
	sol::table table = lua["node_editor"];

	// node_editor.get_scene_nodes() -> {{ uid=N, type="..." }, ...}
	// Returns all nodes currently in the active node graph.
	table.set_function("get_scene_nodes", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		auto winOpt = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
		if (!winOpt || !*winOpt)
			return l.create_table();

		auto* graph = (*winOpt)->getNodeFlow();
		if (!graph)
			return l.create_table();

		sol::table result = l.create_table();
		int idx = 1;
		for (auto& [uid, nodePtr] : graph->getNodes()) {
			auto* luaNode = dynamic_cast<LuaNode*>(nodePtr.get());
			if (!luaNode)
				continue;
			sol::table entry = l.create_table();
			entry["uid"]  = uid;
			entry["type"] = luaNode->getNodeType();
			result[idx++] = entry;
		}
		return result;
	});

	// node_editor.get_node_handle(uid) -> LuaNodeHandle* or nil
	table.set_function("get_node_handle", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto winOpt = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
		if (!winOpt || !*winOpt)
			return sol::object{};

		auto* graph = (*winOpt)->getNodeFlow();
		if (!graph)
			return sol::object{};

		auto& nodes = graph->getNodes();
		auto it = nodes.find(uid);
		if (it == nodes.end())
			return sol::object{};

		auto* luaNode = dynamic_cast<LuaNode*>(it->second.get());
		if (!luaNode)
			return sol::object{};

		auto handle = luaNode->getHandle();
		if (!handle)
			return sol::object{};

		return sol::make_object(l, handle.get());
	});

	// node_editor.get_node_execute_fn(uid) -> function or nil
	table.set_function("get_node_execute_fn", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto winOpt = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
		if (!winOpt || !*winOpt)
			return sol::object{};

		auto* graph = (*winOpt)->getNodeFlow();
		if (!graph)
			return sol::object{};

		auto& nodes = graph->getNodes();
		auto it = nodes.find(uid);
		if (it == nodes.end())
			return sol::object{};

		auto* luaNode = dynamic_cast<LuaNode*>(it->second.get());
		if (!luaNode)
			return sol::object{};

		const NodeDescriptor* desc = NodeRegistry::instance().find(luaNode->getNodeType());
		if (!desc || !desc->on_execute.valid())
			return sol::object{};

		return sol::make_object(l, desc->on_execute);
	});

	// node_editor.get_next_flow_node(uid, pinIndex) -> NodeUID or nil
	// Finds the node connected to the output pin at pinIndex (0-based index into getOuts()).
	// on_execute should return the integer index of the flow pin to follow (nil => 0).
	table.set_function("get_next_flow_node", [](sol::this_state s, ImFlow::NodeUID uid, int pinIndex) -> sol::object {
		sol::state_view l(s);
		auto winOpt = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
		if (!winOpt || !*winOpt)
			return sol::object{};

		auto* graph = (*winOpt)->getNodeFlow();
		if (!graph)
			return sol::object{};

		auto& nodes = graph->getNodes();
		auto it = nodes.find(uid);
		if (it == nodes.end())
			return sol::object{};

		auto* luaNode = dynamic_cast<LuaNode*>(it->second.get());
		if (!luaNode)
			return sol::object{};

		const auto& outs = luaNode->getOuts();
		if (pinIndex < 0 || pinIndex >= static_cast<int>(outs.size()))
			return sol::object{};

		ImFlow::Pin* targetPin = outs[pinIndex].get();

		// Walk all links; find the one whose left (output) side is our target pin.
		for (auto& weakLink : graph->getLinks()) {
			auto link = weakLink.lock();
			if (!link)
				continue;
			if (link->left() == targetPin) {
				auto* nextNode = dynamic_cast<LuaNode*>(link->right()->getParent());
				if (nextNode)
					return sol::make_object(l, nextNode->getUID());
			}
		}

		return sol::object{};
	});
}
