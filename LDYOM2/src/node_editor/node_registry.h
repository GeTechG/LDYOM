#pragma once

#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct NodePinDescriptor {
    std::string title;
    std::string type;    // "flow", "float", "int", "bool", "string"
    std::string dir;     // "in" or "out"
    sol::object defaultValue;
    sol::protected_function behaviour; // optional: out-pin value producer (handle) -> value
    sol::protected_function on_render; // optional: custom ImGui renderer (handle)
};

struct NodeDescriptor {
    std::string type;       // e.g. "my_addon.delay"
    std::string title;
    std::string category;
    std::string styleKey;   // "cyan", "green", "red", "brown"
    std::vector<NodePinDescriptor> pins;
    sol::protected_function on_init;
    sol::protected_function on_draw;
    sol::protected_function on_execute;
};

class NodeRegistry {
  public:
    static NodeRegistry& instance();

    NodeRegistry(const NodeRegistry&) = delete;
    NodeRegistry& operator=(const NodeRegistry&) = delete;

    bool registerNode(const sol::table& descriptor);
    const NodeDescriptor* find(const std::string& type) const;
    std::vector<std::string> getCategories() const;
    std::vector<const NodeDescriptor*> getDescriptorsByCategory(const std::string& category) const;

    static void sol_lua_register(sol::state_view lua);

  private:
    NodeRegistry() = default;

    std::unordered_map<std::string, NodeDescriptor> m_descriptors;
    std::vector<std::string> m_categoryOrder;
};
