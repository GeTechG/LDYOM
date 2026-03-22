#pragma once

#include <cstdint>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declaration to avoid pulling in ImNodeFlow / imgui headers here
namespace ImFlow {
class NodeStyle;
}

// ─── NodeStyleRegistry ───────────────────────────────────────────────────────
// Maps semantic style keys (e.g. "event", "function") to ImFlow::NodeStyle.
// Addons can register their own styles from Lua via node_editor.register_style().

class NodeStyleRegistry {
  public:
	static NodeStyleRegistry& instance();

	NodeStyleRegistry(const NodeStyleRegistry&) = delete;
	NodeStyleRegistry& operator=(const NodeStyleRegistry&) = delete;

	// Register a named style.
	// icon: UTF-8 FontAwesome glyph string (e.g. ICON_FA_BOLT), may be empty.
	// headerBg / titleColor: RGBA packed as 0xAABBGGRR (IM_COL32 format).
	void registerStyle(const std::string& key, const std::string& icon, uint32_t headerBg, uint32_t titleColor,
	                   float radius = 6.5f);

	// Returns the style for key, or the "function" default if the key is unknown.
	std::shared_ptr<ImFlow::NodeStyle> getStyle(const std::string& key) const;

	// Returns the icon string for key, or "" if none registered.
	const std::string& getIcon(const std::string& key) const;

	static void sol_lua_register(sol::state_view lua);

  private:
	NodeStyleRegistry();
	std::unordered_map<std::string, std::shared_ptr<ImFlow::NodeStyle>> m_styles;
	std::unordered_map<std::string, std::string> m_icons;
};

// Entry for a lazy (not yet loaded) node — type, category and the Lua file to load on first use
struct LazyNodeEntry {
	std::string type;
	std::string category;
	std::string path;
};

struct NodePinDescriptor {
	std::string title;
	std::string type; // "flow", "number", "bool", "string"
	std::string dir;  // "in" or "out"
	sol::object defaultValue;
	sol::protected_function behaviour; // optional: out-pin value producer (handle) -> value
	sol::protected_function on_render; // optional: custom ImGui renderer (handle)
};

struct NodeDescriptor {
	std::string type; // e.g. "my_addon.delay"
	std::string category;
	std::string styleKey; // semantic key registered via NodeStyleRegistry, e.g. "event", "function"
	bool is_pure = false; // pure (value) node: no flow pins, pull-based evaluation with per-step caching
	std::vector<NodePinDescriptor> pins;
	sol::table defaultData; // optional: initial key/value pairs for nodeData
	sol::protected_function on_init;
	sol::protected_function on_load; // called after deserializeData, before links are reconnected
	sol::protected_function on_draw;
	sol::protected_function on_execute;
};

class NodeRegistry {
  public:
	static NodeRegistry& instance();

	NodeRegistry(const NodeRegistry&) = delete;
	NodeRegistry& operator=(const NodeRegistry&) = delete;

	bool registerNode(const sol::table& descriptor);
	bool registerLazy(const sol::table& descriptor);
	// Ensures a node is loaded; if it was registered lazily, executes its Lua file.
	// Returns true if the node is available after the call.
	bool ensureLoaded(const std::string& type);

	const NodeDescriptor* find(const std::string& type) const;
	std::vector<std::string> getCategories() const;
	std::vector<const NodeDescriptor*> getDescriptorsByCategory(const std::string& category) const;
	// Returns all node types for a category (both loaded and lazy), for menu rendering
	std::vector<std::string> getTypesForCategory(const std::string& category) const;

	static void sol_lua_register(sol::state_view lua);

  private:
	NodeRegistry() = default;

	std::unordered_map<std::string, NodeDescriptor> m_descriptors;
	std::unordered_map<std::string, LazyNodeEntry> m_lazyEntries;
	std::vector<std::string> m_categoryOrder;
};
