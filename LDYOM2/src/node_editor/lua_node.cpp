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
	if (type == "number")
		return ImFlow::PinStyle::cyan();
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

sol::object LuaNodeHandle::getInputDefault(int index) {
	if (!nodePtr)
		return nilObject();

	const auto& ins = nodePtr->getIns();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(ins.size()))
		return nilObject();

	const auto& pin = ins[i];
	if (pin->getDataType() == typeid(FlowToken))
		return nilObject();
	auto* typedPin = static_cast<ImFlow::InPin<sol::object>*>(pin.get());
	return typedPin->getDefault();
}

void LuaNodeHandle::setInputDefault(int index, sol::object value) {
	if (!nodePtr)
		return;

	const auto& ins = nodePtr->getIns();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(ins.size()))
		return;

	const auto& pin = ins[i];
	if (pin->getDataType() == typeid(FlowToken))
		return;
	auto* typedPin = static_cast<ImFlow::InPin<sol::object>*>(pin.get());
	typedPin->setDefault(value);
}

bool LuaNodeHandle::isInputConnected(int index) {
	if (!nodePtr)
		return false;
	const auto& ins = nodePtr->getIns();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(ins.size()))
		return false;
	return ins[i]->isConnected();
}

bool LuaNodeHandle::isOutputConnected(int index) {
	if (!nodePtr)
		return false;
	const auto& outs = nodePtr->getOuts();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(outs.size()))
		return false;
	return outs[i]->isConnected();
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

sol::object LuaNodeHandle::getOutput(int index) {
	if (!outputData.valid())
		return nilObject();

	return outputData.get<sol::object>(index);
}

void LuaNodeHandle::setOutput(int index, sol::object value) {
	if (outputData.valid()) {
		outputData[index] = value;
	}
}

std::string LuaNodeHandle::getInputPinType(int index) const {
	if (!nodePtr)
		return {};
	const auto& ins = nodePtr->getIns();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(ins.size()))
		return {};
	ImFlow::Pin* pin = ins[i].get();
	if (pin->getDataType() == typeid(FlowToken))
		return {};
	return PinSemanticRegistry::instance().getType(pin);
}

std::string LuaNodeHandle::getOutputPinType(int index) const {
	if (!nodePtr)
		return {};
	const auto& outs = nodePtr->getOuts();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(outs.size()))
		return {};
	ImFlow::Pin* pin = outs[i].get();
	if (pin->getDataType() == typeid(FlowToken))
		return {};
	return PinSemanticRegistry::instance().getType(pin);
}

void LuaNodeHandle::setInputPinType(int index, const std::string& type) {
	if (!nodePtr)
		return;
	const auto& ins = nodePtr->getIns();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(ins.size()))
		return;
	ImFlow::Pin* pin = ins[i].get();
	if (pin->getDataType() == typeid(FlowToken))
		return;
	const std::string existing = PinSemanticRegistry::instance().getType(pin);
	if (!existing.empty() && existing == type)
		return;
	// Disconnect if type changed
	if (pin->isConnected())
		pin->deleteLink();
	PinSemanticRegistry::instance().registerPin(pin, type);
	pin->getStyle() = pinStyleForType(type);
}

void LuaNodeHandle::setOutputPinType(int index, const std::string& type) {
	if (!nodePtr)
		return;
	const auto& outs = nodePtr->getOuts();
	int i = index - 1;
	if (i < 0 || i >= static_cast<int>(outs.size()))
		return;
	ImFlow::Pin* pin = outs[i].get();
	if (pin->getDataType() == typeid(FlowToken))
		return;
	const std::string existing = PinSemanticRegistry::instance().getType(pin);
	if (!existing.empty() && existing == type)
		return;
	// Disconnect all connected input pins
	auto* graph = nodePtr->getHandler();
	if (graph) {
		std::vector<ImFlow::Pin*> toDisconnect;
		for (auto& weakLink : graph->getLinks()) {
			auto link = weakLink.lock();
			if (link && link->left() == pin)
				toDisconnect.push_back(link->right());
		}
		for (auto* inPin : toDisconnect)
			inPin->deleteLink();
	}
	PinSemanticRegistry::instance().registerPin(pin, type);
	pin->getStyle() = pinStyleForType(type);
}

void LuaNodeHandle::sol_lua_register(sol::state_view lua) {
	lua.new_usertype<LuaNodeHandle>(
		"LuaNodeHandle", sol::no_constructor, "getInput", &LuaNodeHandle::getInput, "getInputDefault",
		&LuaNodeHandle::getInputDefault, "setInputDefault", &LuaNodeHandle::setInputDefault, "isInputConnected",
		&LuaNodeHandle::isInputConnected, "isOutputConnected", &LuaNodeHandle::isOutputConnected, "getData",
		&LuaNodeHandle::getData, "setData", &LuaNodeHandle::setData, "getOutput", &LuaNodeHandle::getOutput,
		"setOutput", &LuaNodeHandle::setOutput, "getInputPinType", &LuaNodeHandle::getInputPinType,
		"getOutputPinType", &LuaNodeHandle::getOutputPinType, "setInputPinType", &LuaNodeHandle::setInputPinType,
		"setOutputPinType", &LuaNodeHandle::setOutputPinType, "typeKey", sol::readonly(&LuaNodeHandle::typeKey),
		"uid", sol::property([](const LuaNodeHandle& h) -> ImFlow::NodeUID {
			return h.nodePtr ? h.nodePtr->getUID() : 0;
		}));
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
	auto title = _(fmt::format("nodes_titles.{}", desc->type));
	auto label = icon.empty() ? title : icon + " " + title;
	setTitle(label);
	setStyle(nodeStyleForKey(desc->styleKey));

	// Create handle
	m_handle = std::make_shared<LuaNodeHandle>();
	m_handle->nodePtr = this;
	{
		auto guard = LuaManager::instance().getState();
		m_handle->nodeData = guard.get().create_table();
		m_handle->outputData = guard.get().create_table();

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
		PinSemanticRegistry::instance().unregisterPin(pin.get());
	}
	for (auto& pin : getOuts()) {
		PinSemanticRegistry::instance().unregisterPin(pin.get());
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
	for (size_t i = 0; i < desc.pins.size(); ++i) {
		const auto& pin = desc.pins[i];
		if (pin.dir == "in") {
			if (pin.type == "flow") {
				auto p = addIN<FlowToken>(_(pin.title), FlowToken{}, ImFlow::ConnectionFilter::SameType(),
				                          ImFlow::PinStyle::white());
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p.get(), "flow");
			} else {
				sol::object defVal = pin.defaultValue;
				auto p = addIN<sol::object>(
					_(pin.title), defVal,
					[sType = pin.type](ImFlow::Pin* out, ImFlow::Pin* in) {
						return PinSemanticRegistry::sameType(out, in);
					},
					pinStyleForType(pin.type));
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p.get(), pin.type);
			}
		} else { // "out"
			if (pin.type == "flow") {
				auto p = addOUT<FlowToken>(_(pin.title.starts_with("##") ? "" : pin.title), ImFlow::PinStyle::white());
				p->behaviour([] { return FlowToken{}; });
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p.get(), "flow");
			} else {
				auto p =
					addOUT<sol::object>(_(pin.title.starts_with("##") ? "" : pin.title), pinStyleForType(pin.type));
				if (desc.is_pure) {
					// Pure node: evaluate on_execute once per generation, then return cached data.
					p->behaviour([this, pinTitle = pin.title, i]() -> sol::object {
						if (!m_handle)
							return nilObject();
						evaluatePure();
						return m_handle->getOutput(i + 1);
					});
				} else if (pin.behaviour.valid()) {
					p->behaviour([this, luaBehaviour = pin.behaviour]() -> sol::object {
						if (!m_handle)
							return nilObject();
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
					p->behaviour([this, pinTitle = pin.title, i]() -> sol::object {
						if (!m_handle)
							return nilObject();
						return m_handle->getOutput(i + 1);
					});
				}
				if (pin.on_render.valid())
					p->renderer(makePinRenderer(pin.on_render));
				PinSemanticRegistry::instance().registerPin(p.get(), pin.type);
			}
		}
	}
}

void LuaNode::evaluatePure() {
	const uint64_t current = s_evalGeneration.load(std::memory_order_relaxed);
	if (m_pureGeneration == current)
		return; // already computed this generation
	m_pureGeneration = current;

	const NodeDescriptor* desc = NodeRegistry::instance().find(m_typeKey);
	if (!desc || !desc->on_execute.valid())
		return;

	auto result = desc->on_execute(m_handle);
	if (!result.valid()) {
		sol::error err = result;
		LDYOM_ERROR("LuaNode pure on_execute error for '{}': {}", m_typeKey, err.what());
	}
}

void LuaNode::draw() {
	const NodeDescriptor* desc = NodeRegistry::instance().find(m_typeKey);
	if (desc && desc->on_draw.valid()) {
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
	// NOTE: do NOT hold the LuaManager lock while resuming the coroutine.
	// Lua code may call back into C++ (e.g. pin behaviours, evaluatePure) which
	// also acquire the lock, causing a same-thread reentrant deadlock.
	sol::protected_function_result result = luaCoro(m_handle);

	// Keep resuming while the coroutine has yielded and can continue
	while (luaCoro.runnable()) {
		co_await 1; // yield one ms (allows current frame to complete)
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

nlohmann::json LuaNode::serializePinDefaults() {
	nlohmann::json arr = nlohmann::json::array();
	auto guard = LuaManager::instance().getState();

	for (const auto& pin : getIns()) {
		if (pin->getDataType() == typeid(FlowToken)) {
			arr.push_back(nullptr);
			continue;
		}
		auto* typedPin = static_cast<ImFlow::InPin<sol::object>*>(pin.get());
		sol::object val = typedPin->getDefault();
		if (!val.valid() || val.get_type() == sol::type::lua_nil) {
			arr.push_back(nullptr);
		} else if (val.is<bool>()) {
			arr.push_back(val.as<bool>());
		} else if (val.is<int>()) {
			arr.push_back(val.as<int>());
		} else if (val.is<double>()) {
			arr.push_back(val.as<double>());
		} else if (val.is<std::string>()) {
			arr.push_back(val.as<std::string>());
		} else {
			arr.push_back(nullptr);
		}
	}
	return arr;
}

void LuaNode::deserializePinDefaults(const nlohmann::json& j) {
	if (!j.is_array())
		return;
	auto guard = LuaManager::instance().getState();
	auto& lua = guard.get();

	const auto& ins = getIns();
	const int count = std::min(static_cast<int>(j.size()), static_cast<int>(ins.size()));
	for (int i = 0; i < count; ++i) {
		if (ins[i]->getDataType() == typeid(FlowToken))
			continue;
		const auto& val = j[i];
		if (val.is_null())
			continue;
		auto* typedPin = static_cast<ImFlow::InPin<sol::object>*>(ins[i].get());
		if (val.is_boolean())
			typedPin->setDefault(sol::make_object(lua, val.get<bool>()));
		else if (val.is_number_integer())
			typedPin->setDefault(sol::make_object(lua, val.get<int>()));
		else if (val.is_number_float())
			typedPin->setDefault(sol::make_object(lua, val.get<double>()));
		else if (val.is_string())
			typedPin->setDefault(sol::make_object(lua, val.get<std::string>()));
	}
}

void LuaNode::onLoad() {
	const NodeDescriptor* desc = NodeRegistry::instance().find(m_typeKey);
	if (!desc || !desc->on_load.valid())
		return;
	try {
		auto guard = LuaManager::instance().getState();
		auto result = desc->on_load(m_handle);
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("LuaNode on_load error for '{}': {}", m_typeKey, err.what());
		}
	} catch (const std::exception& e) {
		LDYOM_ERROR("LuaNode on_load exception for '{}': {}", m_typeKey, e.what());
	}
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
