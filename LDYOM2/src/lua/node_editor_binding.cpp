#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor_binding.h"
#include "window_manager.h"
#include "windows/node_editor.h"
#include <ImNodeFlow.h>
#include <lua_node.h>
#include <node_editor_lua.h>
#include <node_registry.h>


// Helper: get the NodeEditorWindow from WindowManager, returns nullptr on failure.
static NodeEditorWindow* getNodeEditorWindow() {
	auto winOpt = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
	if (!winOpt || !*winOpt)
		return nullptr;
	return *winOpt;
}

void register_node_editor_bindings(sol::state_view lua) {
	// ── LuaNode usertype ─────────────────────────────────────────────────────
	// Provides OOP-style access to a node in the graph.
	// NOTE: raw pointer — valid only while the node exists in the graph.
	lua.new_usertype<LuaNode>(
		"LuaNode", sol::no_constructor,
		"uid",  sol::property([](const LuaNode& n) { return n.getUID(); }),
		"type", sol::property(&LuaNode::getNodeType),
		"handle", sol::property([](sol::this_state s, LuaNode& n) -> sol::object {
			sol::state_view l(s);
			auto handle = n.getHandle();
			if (!handle)
				return sol::object{};
			// Pass the shared_ptr so Lua keeps the handle alive even after node removal.
			return sol::make_object(l, handle);
		}));

	// Extend the existing node_editor table (created by NodeRegistry::sol_lua_register)
	sol::table table = lua["node_editor"];

	// node_editor.get_scene_nodes() -> {{ uid=N, type="...", workspace=N }, ...}
	// Returns all nodes from ALL workspaces.
	table.set_function("get_scene_nodes", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win)
			return l.create_table();

		sol::table result = l.create_table();
		int idx = 1;
		int wsIdx = 1;
		for (const auto& ws : win->getWorkspaces()) {
			if (!ws.nodeFlow) {
				++wsIdx;
				continue;
			}
			for (auto& [uid, nodePtr] : ws.nodeFlow->getNodes()) {
				auto* luaNode = dynamic_cast<LuaNode*>(nodePtr.get());
				if (!luaNode)
					continue;
				sol::table entry = l.create_table();
				entry["uid"] = uid;
				entry["type"] = luaNode->getNodeType();
				entry["workspace"] = wsIdx;
				result[idx++] = entry;
			}
			++wsIdx;
		}
		return result;
	});

	// node_editor.get_node(uid) -> LuaNode* or nil
	// Returns the LuaNode usertype for direct OOP access (node.uid, node.type, node.handle).
	// NOTE: do NOT store the returned value past the node's lifetime.
	table.set_function("get_node", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win)
			return sol::object{};

		auto* luaNode = win->findNodeByUID(uid);
		if (!luaNode)
			return sol::object{};
		return sol::make_object(l, luaNode);
	});

	// node_editor.get_node_handle(uid) -> LuaNodeHandle or nil
	// Returns the handle as a shared_ptr so Lua keeps it alive even after node removal.
	table.set_function("get_node_handle", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win)
			return sol::object{};

		auto* luaNode = win->findNodeByUID(uid);
		if (!luaNode)
			return sol::object{};

		auto handle = luaNode->getHandle();
		if (!handle)
			return sol::object{};
		return sol::make_object(l, handle);
	});

	// node_editor.get_node_type(uid) -> string or nil
	// Shortcut — avoids fetching the full handle just to read the type.
	table.set_function("get_node_type", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win)
			return sol::object{};

		auto* luaNode = win->findNodeByUID(uid);
		if (!luaNode)
			return sol::object{};
		return sol::make_object(l, luaNode->getNodeType());
	});

	// node_editor.get_node_execute_fn(uid) -> function or nil
	// Searches all workspaces.
	table.set_function("get_node_execute_fn", [](sol::this_state s, ImFlow::NodeUID uid) -> sol::object {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win)
			return sol::object{};

		auto* luaNode = win->findNodeByUID(uid);
		if (!luaNode)
			return sol::object{};

		const NodeDescriptor* desc = NodeRegistry::instance().find(luaNode->getNodeType());
		if (!desc || !desc->on_execute.valid())
			return sol::object{};
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
		if (!win)
			return sol::object{};

		ImFlow::ImNodeFlow* graph = nullptr;
		auto* luaNode = win->findNodeByUID(uid, &graph);
		if (!luaNode || !graph)
			return sol::object{};

		const auto& outs = luaNode->getOuts();
		if (pinIndex < 0 || pinIndex >= static_cast<int>(outs.size()))
			return sol::object{};

		ImFlow::Pin* targetPin = outs[pinIndex].get();

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

	// ── Workspace management ─────────────────────────────────────────────────

	// node_editor.get_workspace_count() -> int
	table.set_function("get_workspace_count", []() -> int {
		auto* win = getNodeEditorWindow();
		if (!win)
			return 0;
		return static_cast<int>(win->getWorkspaces().size());
	});

	// node_editor.get_workspace_names() -> { "name1", "name2", ... }
	table.set_function("get_workspace_names", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		auto* win = getNodeEditorWindow();
		if (!win)
			return l.create_table();

		sol::table result = l.create_table();
		int idx = 1;
		for (const auto& ws : win->getWorkspaces())
			result[idx++] = ws.name;
		return result;
	});

	// node_editor.save_graph(path)
	table.set_function("save_graph", [](const std::string& path) {
		auto* win = getNodeEditorWindow();
		if (win)
			win->saveGraph(path);
	});

	// node_editor.load_graph(path)
	table.set_function("load_graph", [](const std::string& path) {
		auto* win = getNodeEditorWindow();
		if (win)
			win->loadGraph(path);
	});

	// node_editor.clear_graph()
	// Clears the active workspace's graph.
	table.set_function("clear_graph", []() {
		auto* win = getNodeEditorWindow();
		if (win)
			win->clearGraph();
	});

	// node_editor.run_flow_from(startUID)
	// Executes the node flow chain starting from startUID.
	// Must be called from within a node_tasks coroutine; supports yielding execute functions.
	lua.script(embedded::node_editor_lua);
}
