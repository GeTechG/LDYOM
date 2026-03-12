#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor_binding.h"
#include "window_manager.h"
#include "windows/node_editor.h"
#include <ImNodeFlow.h>
#include <lua_node.h>
#include <node_registry.h>

// Helper: get the NodeEditorWindow from WindowManager, returns nullptr on failure.
static NodeEditorWindow* getNodeEditorWindow() {
	auto winOpt = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
	if (!winOpt || !*winOpt) return nullptr;
	return *winOpt;
}

void register_node_editor_bindings(sol::state_view lua) {
	// Extend the existing node_editor table (created by NodeRegistry::sol_lua_register)
	sol::table table = lua["node_editor"];

	// node_editor.get_scene_nodes() -> {{ uid=N, type="...", workspace=N }, ...}
	// Returns all nodes from ALL workspaces.
	table.set_function("get_scene_nodes", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win) return l.create_table();

		sol::table result = l.create_table();
		int idx = 1;
		int wsIdx = 1;
		for (const auto& ws : win->getWorkspaces()) {
			if (!ws.nodeFlow) { ++wsIdx; continue; }
			for (auto& [uid, nodePtr] : ws.nodeFlow->getNodes()) {
				auto* luaNode = dynamic_cast<LuaNode*>(nodePtr.get());
				if (!luaNode) continue;
				sol::table entry = l.create_table();
				entry["uid"]       = uid;
				entry["type"]      = luaNode->getNodeType();
				entry["workspace"] = wsIdx;
				result[idx++] = entry;
			}
			++wsIdx;
		}
		return result;
	});

	// node_editor.get_node_handle(uid) -> LuaNodeHandle* or nil
	// Searches all workspaces.
	table.set_function("get_node_handle", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win) return sol::object{};

		auto* luaNode = win->findNodeByUID(uid);
		if (!luaNode) return sol::object{};

		auto handle = luaNode->getHandle();
		if (!handle) return sol::object{};
		return sol::make_object(l, handle.get());
	});

	// node_editor.get_node_execute_fn(uid) -> function or nil
	// Searches all workspaces.
	table.set_function("get_node_execute_fn", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win) return sol::object{};

		auto* luaNode = win->findNodeByUID(uid);
		if (!luaNode) return sol::object{};

		const NodeDescriptor* desc = NodeRegistry::instance().find(luaNode->getNodeType());
		if (!desc || !desc->on_execute.valid()) return sol::object{};
		return sol::make_object(l, desc->on_execute);
	});

	// node_editor.bump_pure_generation()
	// Invalidates the cached results of all pure nodes.
	// Call this at the start of each flow-node execution step so pure nodes
	// are re-evaluated at most once per step even if multiple value pins feed from them.
	table.set_function("bump_pure_generation", []() { LuaNode::bumpEvalGeneration(); });

	// node_editor.get_next_flow_node(uid, pinIndex) -> NodeUID or nil
	// Follows the output flow pin at pinIndex on the node identified by uid.
	// Searches all workspaces.
	table.set_function("get_next_flow_node", [](sol::this_state s, ImFlow::NodeUID uid, int pinIndex) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win) return sol::object{};

		ImFlow::ImNodeFlow* graph = nullptr;
		auto* luaNode = win->findNodeByUID(uid, &graph);
		if (!luaNode || !graph) return sol::object{};

		const auto& outs = luaNode->getOuts();
		if (pinIndex < 0 || pinIndex >= static_cast<int>(outs.size())) return sol::object{};

		ImFlow::Pin* targetPin = outs[pinIndex].get();

		for (auto& weakLink : graph->getLinks()) {
			auto link = weakLink.lock();
			if (!link) continue;
			if (link->left() == targetPin) {
				auto* nextNode = dynamic_cast<LuaNode*>(link->right()->getParent());
				if (nextNode) return sol::make_object(l, nextNode->getUID());
			}
		}
		return sol::object{};
	});
}
