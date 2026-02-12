#include "node_registry.h"
#include <logger.h>

NodeRegistry& NodeRegistry::instance() {
    static NodeRegistry inst;
    return inst;
}

bool NodeRegistry::registerNode(const sol::table& descriptor) {
    NodeDescriptor desc;

    auto typeOpt = descriptor.get<sol::optional<std::string>>("type");
    if (!typeOpt) {
        LDYOM_ERROR("NodeRegistry::registerNode: missing 'type' field");
        return false;
    }
    desc.type = *typeOpt;

    auto titleOpt = descriptor.get<sol::optional<std::string>>("title");
    desc.title = titleOpt.value_or(desc.type);

    auto categoryOpt = descriptor.get<sol::optional<std::string>>("category");
    desc.category = categoryOpt.value_or("Uncategorized");

    auto styleOpt = descriptor.get<sol::optional<std::string>>("style");
    desc.styleKey = styleOpt.value_or("cyan");

    // Parse pins
    auto pinsTable = descriptor.get<sol::optional<sol::table>>("pins");
    if (pinsTable) {
        for (auto& kv : *pinsTable) {
            if (!kv.second.is<sol::table>()) continue;
            sol::table pinTable = kv.second.as<sol::table>();

            NodePinDescriptor pin;
            auto pinTitleOpt = pinTable.get<sol::optional<std::string>>("title");
            if (!pinTitleOpt) continue;
            pin.title = *pinTitleOpt;

            auto pinTypeOpt = pinTable.get<sol::optional<std::string>>("type");
            pin.type = pinTypeOpt.value_or("float");

            auto pinDirOpt = pinTable.get<sol::optional<std::string>>("dir");
            pin.dir = pinDirOpt.value_or("in");

            pin.defaultValue = pinTable.get<sol::object>("default");

            auto pinBehaviour = pinTable.get<sol::optional<sol::protected_function>>("behaviour");
            if (pinBehaviour) pin.behaviour = *pinBehaviour;

            auto pinRenderer = pinTable.get<sol::optional<sol::protected_function>>("on_render");
            if (pinRenderer) pin.on_render = *pinRenderer;

            desc.pins.push_back(std::move(pin));
        }
    }

    // Parse default node data
    auto defaultDataOpt = descriptor.get<sol::optional<sol::table>>("default_data");
    if (defaultDataOpt) desc.defaultData = *defaultDataOpt;

    // Parse callbacks
    auto onInit = descriptor.get<sol::optional<sol::protected_function>>("on_init");
    if (onInit) desc.on_init = *onInit;

    auto onDraw = descriptor.get<sol::optional<sol::protected_function>>("on_draw");
    if (onDraw) desc.on_draw = *onDraw;

    auto onExecute = descriptor.get<sol::optional<sol::protected_function>>("on_execute");
    if (onExecute) desc.on_execute = *onExecute;

    // Track category insertion order
    if (std::find(m_categoryOrder.begin(), m_categoryOrder.end(), desc.category) == m_categoryOrder.end()) {
        m_categoryOrder.push_back(desc.category);
    }

    LDYOM_INFO("NodeRegistry: registered node type '{}'", desc.type);
    m_descriptors[desc.type] = std::move(desc);
    return true;
}

const NodeDescriptor* NodeRegistry::find(const std::string& type) const {
    auto it = m_descriptors.find(type);
    if (it == m_descriptors.end()) return nullptr;
    return &it->second;
}

std::vector<std::string> NodeRegistry::getCategories() const {
    return m_categoryOrder;
}

std::vector<const NodeDescriptor*> NodeRegistry::getDescriptorsByCategory(const std::string& category) const {
    std::vector<const NodeDescriptor*> result;
    for (auto& kv : m_descriptors) {
        if (kv.second.category == category) {
            result.push_back(&kv.second);
        }
    }
    return result;
}

void NodeRegistry::sol_lua_register(sol::state_view lua) {
    auto table = lua.create_named_table("node_editor");
    table.set_function("register", [](const sol::table& desc) {
        return NodeRegistry::instance().registerNode(desc);
    });
}
