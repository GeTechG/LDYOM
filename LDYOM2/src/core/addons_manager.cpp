#include "addons_manager.h"
#include "localization.h"
#include "lua_manager.h"
#include "settings.h"
#include <algorithm>
#include <logger.h>
#include <paths.h>

AddonsManager& AddonsManager::instance() {
	static AddonsManager instance;
	return instance;
}

void AddonsManager::initialize() {
	LuaManager::instance().registerFunction(
		"addon_global_set", [this](const std::string& key, sol::object value) { sharedData[key] = value; });

	LuaManager::instance().registerFunction("addon_global_get",
	                                        [this](const std::string& key, sol::this_state s) -> sol::object {
												auto it = sharedData.find(key);
												if (it == sharedData.end())
													return sol::make_object(s, sol::lua_nil);
												return it->second;
											});

	LuaManager::instance().registerFunction(
		"addon_global_has", [this](const std::string& key) -> bool { return sharedData.contains(key); });

	LuaManager::instance().registerFunction("addon_global_remove",
	                                        [this](const std::string& key) { sharedData.erase(key); });

	LuaManager::instance().registerFunction("register_addon", [this](sol::table metadata, sol::this_state s) {
		std::string id = metadata["id"].get<std::string>();

		auto it =
			std::find_if(addonsList.begin(), addonsList.end(), [&id](const AddonMetadata& a) { return a.id == id; });
		if (it != addonsList.end()) {
			LDYOM_ERROR("Addon '{}' is already registered", id);
			return;
		}

		AddonMetadata addon;
		addon.id = id;
		addon.name = metadata["name"].get<std::string>();
		addon.description = metadata["description"].get<std::string>();
		addon.version = metadata["version"].get<std::string>();
		if (metadata["author"].valid()) {
			addon.author = metadata["author"].get<std::string>();
		}
		addon.priority = metadata["priority"].get_or(0);
		if (metadata["on_load"].valid()) {
			addon.on_load = metadata["on_load"];
		}
		if (metadata["on_unload"].valid()) {
			addon.on_unload = metadata["on_unload"];
		}

		addon.path = m_currentLoadingPath;
		LDYOM_INFO("Addon registered: {} ({})", addon.name, id);
		addonsList.emplace_back(std::move(addon));
	});

	const std::filesystem::path addonsDir(LDYOM_PATH(ADDONS_DIR));
	if (!std::filesystem::exists(addonsDir)) {
		std::filesystem::create_directories(addonsDir);
		LDYOM_INFO("Created addons directory: {}", addonsDir.string());
	}

	for (const auto& entry : std::filesystem::directory_iterator(addonsDir)) {
		if (entry.is_directory()) {
			loadAddonMetadata(entry.path());
		}
	}

	activeAddons = Settings::instance().getSetting<std::unordered_set<std::string>>("active_addons", {});

	std::vector<AddonMetadata*> toLoad;
	for (auto& addon : addonsList) {
		if (activeAddons.contains(addon.id)) {
			toLoad.push_back(&addon);
		}
	}
	std::sort(toLoad.begin(), toLoad.end(), [](const auto* a, const auto* b) { return a->priority > b->priority; });

	{
		auto lua = LuaManager::instance().getState();
		for (auto* addon : toLoad) {
			if (addon->on_load.valid()) {
				auto result = addon->on_load();
				if (!result.valid()) {
					sol::error err = result;
					LDYOM_ERROR("Addon '{}' on_load failed: {}", addon->id, err.what());
				}
			}
			loadAddonLocale(*addon);
		}
	}
}

void AddonsManager::shutdown() {
	auto lua = LuaManager::instance().getState();
	for (const auto& addonId : activeAddons) {
		auto it = std::find_if(addonsList.begin(), addonsList.end(),
		                       [&addonId](const AddonMetadata& a) { return a.id == addonId; });
		if (it == addonsList.end())
			continue;

		if (!it->on_unload.valid())
			continue;

		auto result = it->on_unload();
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("Addon '{}' on_unload failed: {}", addonId, err.what());
		} else {
			LDYOM_INFO("Addon '{}' unloaded", addonId);
		}
	}
}

bool AddonsManager::loadAddonMetadata(const std::filesystem::path& addonPath) {
	const auto initScript = addonPath / "init.lua";
	if (!std::filesystem::exists(initScript)) {
		return false;
	}

	m_currentLoadingPath = addonPath;

	try {
		auto lua = LuaManager::instance().getState();
		sol::environment env(lua.get(), sol::create, lua.get().globals());
		return LuaManager::instance().executeFileRaw(initScript.string(), env);
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to load addon from {}: {}", initScript.string(), e.what());
		return false;
	}
}

bool AddonsManager::enableAddon(const std::string& addonId) {
	auto it = std::find_if(addonsList.begin(), addonsList.end(),
	                       [&addonId](const AddonMetadata& a) { return a.id == addonId; });
	if (it == addonsList.end()) {
		LDYOM_ERROR("Addon '{}' not found", addonId);
		return false;
	}

	{
		if (it->on_load.valid()) {
			auto result = it->on_load();
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("Addon '{}' on_load failed: {}", addonId, err.what());
				return false;
			}
		}
	}

	loadAddonLocale(*it);
	activeAddons.insert(addonId);
	Settings::instance().setSetting("active_addons", activeAddons);
	return true;
}

bool AddonsManager::disableAddon(const std::string& addonId) {
	auto it = std::find_if(addonsList.begin(), addonsList.end(),
	                       [&addonId](const AddonMetadata& a) { return a.id == addonId; });
	if (it == addonsList.end()) {
		LDYOM_ERROR("Addon '{}' not found", addonId);
		return false;
	}

	{
		if (it->on_unload.valid()) {
			auto result = it->on_unload();
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("Addon '{}' on_unload failed: {}", addonId, err.what());
				return false;
			}
		}
	}

	activeAddons.erase(addonId);
	Settings::instance().setSetting("active_addons", activeAddons);
	return true;
}

bool AddonsManager::isAddonActive(const std::string& addonId) const { return activeAddons.contains(addonId); }

void AddonsManager::setSharedValue(const std::string& key, sol::object value) { sharedData[key] = value; }

sol::object AddonsManager::getSharedValue(const std::string& key) const {
	auto it = sharedData.find(key);
	if (it == sharedData.end())
		return sol::object{};
	return it->second;
}

bool AddonsManager::hasSharedValue(const std::string& key) const { return sharedData.contains(key); }

void AddonsManager::removeSharedValue(const std::string& key) { sharedData.erase(key); }

const std::vector<AddonMetadata>& AddonsManager::getAddons() const { return addonsList; }

void AddonsManager::loadAddonLocale(const AddonMetadata& addon) {
	if (addon.path.empty()) {
		return;
	}

	const auto langDir = addon.path / "languages";
	if (!std::filesystem::exists(langDir) || !std::filesystem::is_directory(langDir)) {
		return;
	}

	auto& i18n = Localization::instance().getI18N();
	const std::string currentLocale = Localization::instance().getCurrentLocale();

	const auto tryLoad = [&](const std::string& locale) {
		const auto filePath = langDir / (locale + ".json");
		if (!std::filesystem::exists(filePath)) {
			return;
		}
		try {
			i18n.mergeLocaleFromFile(filePath.string());
			LDYOM_INFO("Addon '{}': loaded locale file '{}'", addon.id, filePath.string());
		} catch (const std::exception& e) {
			LDYOM_ERROR("Addon '{}': failed to load locale '{}': {}", addon.id, filePath.string(), e.what());
		}
	};

	tryLoad("en");
	if (currentLocale != "en") {
		tryLoad(currentLocale);
	}
}
