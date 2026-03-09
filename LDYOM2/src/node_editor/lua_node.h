#pragma once

#include "node_registry.h"
#include <ImNodeFlow.h>
#include <functional>
#include <ktcoro_wait.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <string>

// Zero-size tag type for execution-flow pins.
// Connects only to other FlowToken pins via SameType() filter.
struct FlowToken {};

// Lua-facing proxy object passed to on_init / on_draw / on_execute callbacks.
// Holds per-instance key/value storage and a back-pointer to the owning node.
struct LuaNodeHandle {
	sol::table nodeData;       // per-instance key/value store (serialized)
	sol::table runtimeData;    // temporary key/value store (not serialized, runtime-only)
	ImFlow::BaseNode* nodePtr; // back-pointer (raw, non-owning)
	std::string typeKey;

	// Read value from an input pin by 1-based index (returns sol::nil if out of range or flow pin)
	sol::object getInput(int index);

	// Get/set the default value of an unconnected input pin by 1-based index
	sol::object getInputDefault(int index);
	void setInputDefault(int index, sol::object value);

	// Check connection status of a pin by 1-based index
	bool isInputConnected(int index);
	bool isOutputConnected(int index);

	// Persistent data accessors (serialized)
	sol::object getData(const std::string& key) const;
	void setData(const std::string& key, sol::object value);

	// Temporary runtime data accessors (not serialized)
	sol::object getRuntimeData(const std::string& key) const;
	void setRuntimeData(const std::string& key, sol::object value);

	static void sol_lua_register(sol::state_view lua);
};

class LuaNode : public ImFlow::BaseNode {
  public:
	explicit LuaNode(const std::string& type);
	~LuaNode() override;

	void draw() override;

	// Drives the Lua on_execute coroutine; outFlowPin receives the name of the
	// output flow pin to follow after execution completes.
	ktwait execute(std::string& outFlowPin);

	nlohmann::json serializeData() const;
	void deserializeData(const nlohmann::json& j);

	// Serialize/deserialize default values of unconnected input pins (array indexed by pin position)
	nlohmann::json serializePinDefaults();
	void deserializePinDefaults(const nlohmann::json& j);

	const std::string& getNodeType() const { return m_typeKey; }
	std::shared_ptr<LuaNodeHandle> getHandle() const { return m_handle; }

  private:
	std::string m_typeKey;
	std::shared_ptr<LuaNodeHandle> m_handle;

	void setupPins(const NodeDescriptor& desc);
	std::function<void(ImFlow::Pin*)> makePinRenderer(sol::protected_function luaRenderer);
};
