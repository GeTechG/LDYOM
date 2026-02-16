#include "lua_node.h"
#include "pin_semantic_registry.h"
#include <functional>
#include <imgui.h>
#include <localization.h>
#include <logger.h>
#include <lua_manager.h>

// ─── helpers ─────────────────────────────────────────────────────────────────

namespace {

std::shared_ptr<ImFlow::PinStyle> pinStyleForType(const std::string& type) {
	if (type == "flow")
		return ImFlow::PinStyle::white();
	if (type == "float")
		return ImFlow::PinStyle::cyan();
	if (type == "int")
		return ImFlow::PinStyle::green();
	if (type == "bool")
		return ImFlow::PinStyle::red();
	if (type == "string")
		return ImFlow::PinStyle::brown();
	return ImFlow::PinStyle::cyan();
}

std::shared_ptr<ImFlow::NodeStyle> nodeStyleForKey(const std::string& key) {
	return NodeStyleRegistry::instance().getStyle(key);
}

// A nil sol::object (default-constructed)
static sol::object nilObject() { return sol::object{}; }

} // namespace

// ─── LuaNodeHandle ───────────────────────────────────────────────────────────

sol::object LuaNodeHandle::getInput(int index) {
	if (!nodePtr)
		return nilObject();

	const auto& ins = nodePtr->getIns();
	int i = index - 1; // convert from 1-based Lua index to 0-based
	if (i < 0 || i >= static_cast<int>(ins.size()))
		return nilObject();

	const auto& pin = ins[i];
	// Skip flow pins — they carry no data value
	if (pin->getDataType() == typeid(FlowToken))
		return nilObject();
	// Safe to cast: all non-flow input pins are InPin<sol::object>
	auto* typedPin = static_cast<ImFlow::InPin<sol::object>*>(pin.get());
	return typedPin->val();
}

sol::object LuaNodeHandle::getData(const std::string& key) const {
	if (!nodeData.valid())
		return nilObject();
	return nodeData.get<sol::object>(key);
}

void LuaNodeHandle::setData(const std::string& key, sol::object value) {
	if (nodeData.valid()) {
		nodeData[key] = value;
	}
}

sol::object LuaNodeHandle::getRuntimeData(const std::string& key) const {
	if (!runtimeData.valid())
		return nilObject();
	return runtimeData.get<sol::object>(key);
}

void LuaNodeHandle::setRuntimeData(const std::string& key, sol::object value) {
	if (runtimeData.valid()) {
		runtimeData[key] = value;
	}
}

void LuaNodeHandle::sol_lua_register(sol::state_view lua) {
	lua.new_usertype<LuaNodeHandle>("LuaNodeHandle", sol::no_constructor, "getInput", &LuaNodeHandle::getInput,
	                                "getData", &LuaNodeHandle::getData, "setData", &LuaNodeHandle::setData,
	                                "getRuntimeData", &LuaNodeHandle::getRuntimeData, "setRuntimeData",
	                                &LuaNodeHandle::setRuntimeData);
}

// ─── LuaNode ─────────────────────────────────────────────────────────────────

LuaNode::LuaNode(const std::string& type)
	: m_typeKey(type) {
	const NodeDescriptor* desc = NodeRegistry::instance().find(type);
	if (!desc) {
		LDYOM_ERROR("LuaNode: unknown node type '{}'", type);
		setTitle(type);
		return;
	}

	const std::string& icon = NodeStyleRegistry::instance().getIcon(desc->styleKey);
	auto label = icon.empty() ? _(desc->type) : icon + " " + _(desc->type);
	setTitle(label);
	setStyle(nodeStyleForKey(desc->styleKey));

	// Create handle
	m_handle = std::make_shared<LuaNodeHandle>();
	m_handle->nodePtr = this;
	{
		auto guard = LuaManager::instance().getState();
		m_handle->nodeData = guard.get().create_table();
		m_handle->runtimeData = guard.get().create_table();

		if (desc->defaultData.valid()) {
			for (auto& kv : desc->defaultData) {
				m_handle->nodeData[kv.first] = kv.second;
			}
		}
	}
	m_handle->typeKey = type;

	setupPins(*desc);

	// Call on_init if present
	if (desc->on_init.valid()) {
		try {
			auto guard = LuaManager::instance().getState();
			auto result = desc->on_init(m_handle);
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("LuaNode on_init error for '{}': {}", type, err.what());
			}
		} catch (const std::exception& e) {
			LDYOM_ERROR("LuaNode on_init exception for '{}': {}", type, e.what());
		}
	}
}

LuaNode::~LuaNode() {
	// Unregister all pin UIDs from the semantic registry
	for (auto& pin : getIns()) {
		PinSemanticRegistry::instance().unregisterPin(pin->getUid());
	}
	for (auto& pin : getOuts()) {
		PinSemanticRegistry::instance().unregisterPin(pin->getUid());
	}
}

// Returns a C++ renderer lambda that calls the Lua on_render function with the handle.
// Used by setupPins for both IN and OUT pins.
auto LuaNode::makePinRenderer(sol::protected_function luaRenderer) -> std::function<void(ImFlow::Pin*)> {
	return [this, luaRenderer = std::move(luaRenderer)](ImFlow::Pin*) {
		if (!m_handle)
			return;
		auto guard = LuaManager::instance().getState();
		auto result = luaRenderer(m_handle);
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("LuaNode pin on_render error for '{}': {}", m_typeKey, err.what());
		}
	};
}

void LuaNode::setupPins(const NodeDescriptor& desc) {
	for (const auto& pin : desc.pins) {
		if (pin.dir == "in") {
			if (pin.type == "flow") {
				auto p = addIN<FlowToken>(pin.title, FlowToken{}, ImFlow::ConnectionFilter::SameType(),
				                          ImFlow::PinStyle::white());
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p->getUid(), "flow");
			} else {
				sol::object defVal = pin.defaultValue;
				auto p = addIN<sol::object>(
					pin.title, defVal,
					[sType = pin.type](ImFlow::Pin* out, ImFlow::Pin* in) {
						return PinSemanticRegistry::sameType(out, in);
					},
					pinStyleForType(pin.type));
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p->getUid(), pin.type);
			}
		} else { // "out"
			if (pin.type == "flow") {
				auto p = addOUT<FlowToken>(pin.title, ImFlow::PinStyle::white());
				p->behaviour([] { return FlowToken{}; });
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p->getUid(), "flow");
			} else {
				auto p = addOUT<sol::object>(pin.title, pinStyleForType(pin.type));
				if (pin.behaviour.valid()) {
					p->behaviour([this, luaBehaviour = pin.behaviour]() -> sol::object {
						if (!m_handle)
							return nilObject();
						auto guard = LuaManager::instance().getState();
						auto result = luaBehaviour(m_handle);
						if (!result.valid()) {
							sol::error err = result;
							LDYOM_ERROR("LuaNode pin behaviour error for '{}': {}", m_typeKey, err.what());
							return nilObject();
						}
						return result.get<sol::object>();
					});
				} else {
					// Fallback: read from nodeData by pin title (set via handle:setData)
					p->behaviour([this, pinTitle = pin.title]() -> sol::object {
						if (!m_handle)
							return nilObject();
						return m_handle->getData(pinTitle);
					});
				}
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p->getUid(), pin.type);
			}
		}
	}
}

void LuaNode::draw() {
	const NodeDescriptor* desc = NodeRegistry::instance().find(m_typeKey);
	if (!desc || !desc->on_draw.valid())
		return;

	try {
		auto guard = LuaManager::instance().getState();
		auto result = desc->on_draw(m_handle);
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("LuaNode on_draw error for '{}': {}", m_typeKey, err.what());
		}
	} catch (const std::exception& e) {
		LDYOM_ERROR("LuaNode on_draw exception for '{}': {}", m_typeKey, e.what());
	}
}

ktwait LuaNode::execute(std::string& outFlowPin) {
	const NodeDescriptor* desc = NodeRegistry::instance().find(m_typeKey);
	if (!desc || !desc->on_execute.valid())
		co_return;

	// sol::coroutine can be constructed directly from a protected_function
	sol::coroutine luaCoro;
	{
		auto guard = LuaManager::instance().getState();
		luaCoro = sol::coroutine(desc->on_execute);
	}

	// First resume — pass the handle as argument
	sol::protected_function_result result;
	{
		auto guard = LuaManager::instance().getState();
		result = luaCoro(m_handle);
	}

	// Keep resuming while the coroutine has yielded and can continue
	while (luaCoro.runnable()) {
		co_await 1; // yield one ms (allows current frame to complete)
		auto guard = LuaManager::instance().getState();
		result = luaCoro();
	}

	if (result.valid()) {
		if (auto pin = result.get<sol::optional<std::string>>()) {
			outFlowPin = *pin;
		}
	} else {
		sol::error err = result;
		LDYOM_ERROR("LuaNode on_execute error for '{}': {}", m_typeKey, err.what());
	}
}

nlohmann::json LuaNode::serializeData() const {
	nlohmann::json j;
	if (!m_handle || !m_handle->nodeData.valid())
		return j;

	auto guard = LuaManager::instance().getState();
	for (auto& kv : m_handle->nodeData) {
		std::string key;
		if (kv.first.is<std::string>()) {
			key = kv.first.as<std::string>();
		} else {
			continue;
		}

		sol::object val = kv.second;
		if (val.is<bool>())
			j[key] = val.as<bool>();
		else if (val.is<int>())
			j[key] = val.as<int>();
		else if (val.is<double>())
			j[key] = val.as<double>();
		else if (val.is<std::string>())
			j[key] = val.as<std::string>();
	}
	return j;
}

void LuaNode::deserializeData(const nlohmann::json& j) {
	if (!m_handle)
		return;
	auto guard = LuaManager::instance().getState();
	auto& lua = guard.get();

	for (auto it = j.begin(); it != j.end(); ++it) {
		const std::string& key = it.key();
		const auto& val = it.value();
		if (val.is_boolean())
			m_handle->nodeData[key] = sol::make_object(lua, val.get<bool>());
		else if (val.is_number_integer())
			m_handle->nodeData[key] = sol::make_object(lua, val.get<int>());
		else if (val.is_number_float())
			m_handle->nodeData[key] = sol::make_object(lua, val.get<double>());
		else if (val.is_string())
			m_handle->nodeData[key] = sol::make_object(lua, val.get<std::string>());
	}
}
