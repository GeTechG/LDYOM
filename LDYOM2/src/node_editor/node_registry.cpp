#include "node_registry.h"
#include "fa_icons.h"
#include "lua_manager.h"
#include <ImNodeFlow.h>
#include <logger.h>

namespace {
uint32_t tableToU32(const sol::optional<sol::table>& t, uint32_t def) {
	if (!t)
		return def;
	int r = t->get_or(1, 255);
	int g = t->get_or(2, 255);
	int b = t->get_or(3, 255);
	int a = t->get_or(4, 255);
	return IM_COL32(r, g, b, a);
}
} // namespace

// ─── NodeStyleRegistry ───────────────────────────────────────────────────────

NodeStyleRegistry& NodeStyleRegistry::instance() {
	static NodeStyleRegistry inst;
	return inst;
}

NodeStyleRegistry::NodeStyleRegistry() {
	// Built-in semantic styles
	registerStyle("event", ICON_FA_BOLT, IM_COL32(191, 120, 40, 255), IM_COL32(255, 255, 255, 255), 6.5f);
	registerStyle("function", ICON_FA_CUBE, IM_COL32(71, 142, 173, 255), IM_COL32(233, 241, 244, 255), 6.5f);
	registerStyle("loop", ICON_FA_ARROWS_ROTATE, IM_COL32(130, 80, 191, 255), IM_COL32(233, 241, 244, 255), 6.5f);
	registerStyle("variable", ICON_FA_BOX, IM_COL32(90, 191, 93, 255), IM_COL32(233, 241, 244, 255), 3.5f);
	registerStyle("branch", ICON_FA_CODE_BRANCH, IM_COL32(191, 90, 90, 255), IM_COL32(233, 241, 244, 255), 11.f);

	// Legacy color-name aliases for backwards compatibility
	registerStyle("cyan", ICON_FA_CUBE, IM_COL32(71, 142, 173, 255), IM_COL32(233, 241, 244, 255), 6.5f);
	registerStyle("green", ICON_FA_BOX, IM_COL32(90, 191, 93, 255), IM_COL32(233, 241, 244, 255), 3.5f);
	registerStyle("red", ICON_FA_CODE_BRANCH, IM_COL32(191, 90, 90, 255), IM_COL32(233, 241, 244, 255), 11.f);
	registerStyle("brown", ICON_FA_CUBE, IM_COL32(191, 134, 90, 255), IM_COL32(233, 241, 244, 255), 6.5f);
}

void NodeStyleRegistry::registerStyle(const std::string& key, const std::string& icon, uint32_t headerBg,
                                      uint32_t titleColor, float radius) {
	m_styles[key] = std::make_shared<ImFlow::NodeStyle>(headerBg, ImColor(titleColor), radius);
	m_icons[key] = icon;
}

const std::string& NodeStyleRegistry::getIcon(const std::string& key) const {
	static const std::string empty;
	auto it = m_icons.find(key);
	return it != m_icons.end() ? it->second : empty;
}

std::shared_ptr<ImFlow::NodeStyle> NodeStyleRegistry::getStyle(const std::string& key) const {
	auto it = m_styles.find(key);
	if (it != m_styles.end())
		return it->second;
	// Fall back to "function" (cyan) as the default
	auto def = m_styles.find("function");
	if (def != m_styles.end())
		return def->second;
	return ImFlow::NodeStyle::cyan();
}

void NodeStyleRegistry::sol_lua_register(sol::state_view lua) {
	// node_editor.register_style({ key="event", header_bg={r,g,b,a}, title_color={r,g,b,a}, radius=6.5 })
	sol::table table = lua["node_editor"];
	table.set_function("register_style", [](const sol::table& desc) {
		auto keyOpt = desc.get<sol::optional<std::string>>("key");
		if (!keyOpt) {
			LDYOM_ERROR("node_editor.register_style: missing 'key' field");
			return;
		}

		auto bgTable = desc.get<sol::optional<sol::table>>("header_bg");
		auto titleTable = desc.get<sol::optional<sol::table>>("title_color");
		float radius = desc.get_or("radius", 6.5f);
		std::string icon = desc.get_or<std::string>("icon", "");

		if (!bgTable) {
			LDYOM_ERROR("node_editor.register_style: missing 'header_bg' for key '{}'", *keyOpt);
			return;
		}

		uint32_t headerBg = tableToU32(bgTable, IM_COL32(71, 142, 173, 255));
		uint32_t titleColor = tableToU32(titleTable, IM_COL32(233, 241, 244, 255));

		NodeStyleRegistry::instance().registerStyle(*keyOpt, icon, headerBg, titleColor, radius);
		LDYOM_INFO("NodeStyleRegistry: registered style '{}'", *keyOpt);
	});
}

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

	auto categoryOpt = descriptor.get<sol::optional<std::string>>("category");
	desc.category = categoryOpt.value_or("Uncategorized");

	auto styleOpt = descriptor.get<sol::optional<std::string>>("style");
	desc.styleKey = styleOpt.value_or("cyan");

	// Parse pins
	auto pinsTable = descriptor.get<sol::optional<sol::table>>("pins");
	if (pinsTable) {
		for (auto& kv : *pinsTable) {
			if (!kv.second.is<sol::table>())
				continue;
			sol::table pinTable = kv.second.as<sol::table>();

			NodePinDescriptor pin;
			auto pinTitleOpt = pinTable.get<sol::optional<std::string>>("title");
			if (!pinTitleOpt)
				continue;
			pin.title = *pinTitleOpt;

			auto pinTypeOpt = pinTable.get<sol::optional<std::string>>("type");
			pin.type = pinTypeOpt.value_or("float");

			auto pinDirOpt = pinTable.get<sol::optional<std::string>>("dir");
			pin.dir = pinDirOpt.value_or("in");

			pin.defaultValue = pinTable.get<sol::object>("default");

			auto pinBehaviour = pinTable.get<sol::optional<sol::protected_function>>("behaviour");
			if (pinBehaviour)
				pin.behaviour = *pinBehaviour;

			auto pinRenderer = pinTable.get<sol::optional<sol::protected_function>>("on_render");
			if (pinRenderer)
				pin.on_render = *pinRenderer;

			desc.pins.push_back(std::move(pin));
		}
	}

	// Parse default node data
	auto defaultDataOpt = descriptor.get<sol::optional<sol::table>>("default_data");
	if (defaultDataOpt)
		desc.defaultData = *defaultDataOpt;

	// Parse callbacks
	auto onInit = descriptor.get<sol::optional<sol::protected_function>>("on_init");
	if (onInit)
		desc.on_init = *onInit;

	auto onDraw = descriptor.get<sol::optional<sol::protected_function>>("on_draw");
	if (onDraw)
		desc.on_draw = *onDraw;

	auto onExecute = descriptor.get<sol::optional<sol::protected_function>>("on_execute");
	if (onExecute)
		desc.on_execute = *onExecute;

	// Track category insertion order
	if (std::find(m_categoryOrder.begin(), m_categoryOrder.end(), desc.category) == m_categoryOrder.end()) {
		m_categoryOrder.push_back(desc.category);
	}

	LDYOM_INFO("NodeRegistry: registered node type '{}'", desc.type);
	m_descriptors[desc.type] = std::move(desc);
	return true;
}

bool NodeRegistry::registerLazy(const sol::table& descriptor) {
	auto typeOpt = descriptor.get<sol::optional<std::string>>("type");
	if (!typeOpt) {
		LDYOM_ERROR("NodeRegistry::registerLazy: missing 'type' field");
		return false;
	}
	auto pathOpt = descriptor.get<sol::optional<std::string>>("path");
	if (!pathOpt) {
		LDYOM_ERROR("NodeRegistry::registerLazy: missing 'path' field");
		return false;
	}
	auto categoryOpt = descriptor.get<sol::optional<std::string>>("category");

	LazyNodeEntry entry;
	entry.type = *typeOpt;
	entry.path = *pathOpt;
	entry.category = categoryOpt.value_or("Uncategorized");

	if (m_descriptors.count(entry.type)) {
		LDYOM_WARN("NodeRegistry::registerLazy: '{}' is already fully registered, skipping", entry.type);
		return false;
	}
	if (m_lazyEntries.count(entry.type)) {
		LDYOM_WARN("NodeRegistry::registerLazy: '{}' is already lazily registered, skipping", entry.type);
		return false;
	}

	if (std::find(m_categoryOrder.begin(), m_categoryOrder.end(), entry.category) == m_categoryOrder.end()) {
		m_categoryOrder.push_back(entry.category);
	}

	m_lazyEntries[entry.type] = std::move(entry);
	return true;
}

bool NodeRegistry::ensureLoaded(const std::string& type) {
	if (m_descriptors.count(type))
		return true;

	auto it = m_lazyEntries.find(type);
	if (it == m_lazyEntries.end())
		return false;

	LDYOM_INFO("NodeRegistry: lazy-loading '{}' from '{}'", type, it->second.path);
	bool ok = LuaManager::instance().executeFile(it->second.path, std::nullopt);
	if (!ok) {
		LDYOM_ERROR("NodeRegistry: failed to execute '{}' for node '{}'", it->second.path, type);
		return false;
	}
	if (!m_descriptors.count(type)) {
		LDYOM_ERROR("NodeRegistry: file '{}' did not register node type '{}'", it->second.path, type);
		return false;
	}
	return true;
}

const NodeDescriptor* NodeRegistry::find(const std::string& type) const {
	auto it = m_descriptors.find(type);
	if (it == m_descriptors.end())
		return nullptr;
	return &it->second;
}

std::vector<std::string> NodeRegistry::getCategories() const { return m_categoryOrder; }

std::vector<const NodeDescriptor*> NodeRegistry::getDescriptorsByCategory(const std::string& category) const {
	std::vector<const NodeDescriptor*> result;
	for (auto& kv : m_descriptors) {
		if (kv.second.category == category) {
			result.push_back(&kv.second);
		}
	}
	return result;
}

std::vector<std::string> NodeRegistry::getTypesForCategory(const std::string& category) const {
	std::vector<std::string> result;
	for (auto& kv : m_descriptors) {
		if (kv.second.category == category)
			result.push_back(kv.first);
	}
	for (auto& kv : m_lazyEntries) {
		if (kv.second.category == category && !m_descriptors.count(kv.first))
			result.push_back(kv.first);
	}
	return result;
}

void NodeRegistry::sol_lua_register(sol::state_view lua) {
	auto table = lua.create_named_table("node_editor");
	table.set_function("register", [](const sol::table& desc) { return NodeRegistry::instance().registerNode(desc); });
	table.set_function("register_lazy",
	                   [](const sol::table& desc) { return NodeRegistry::instance().registerLazy(desc); });

	NodeStyleRegistry::sol_lua_register(lua);
}
