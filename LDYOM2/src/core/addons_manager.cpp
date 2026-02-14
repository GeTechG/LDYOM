#include "addons_manager.h"
#include "lua_manager.h"
#include "settings.h"
#include <logger.h>
#include <paths.h>

AddonsManager& AddonsManager::instance() {
	static AddonsManager instance;
	return instance;
}

void AddonsManager::initialize() {
	// Register once — available to all addon init scripts
	LuaManager::instance().registerFunction("register_addon", [](sol::table metadata, sol::this_state s) {
		sol::state_view lua(s);
		sol::table addons = lua["__addons"].get_or_create<sol::table>();
		std::string id = metadata["id"].get<std::string>();
		if (addons[id].valid()) {
			LDYOM_ERROR("Addon '{}' is already registered", id);
			return;
		}
		addons[id] = metadata;
		LDYOM_INFO("Addon registered: {} ({})", metadata["name"].get<std::string>(), id);
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

	{
		auto lua = LuaManager::instance().getState();
		sol::object addonsObj = lua.get()["__addons"];
		if (!addonsObj.valid()) return;

		sol::table addons = addonsObj.as<sol::table>();
		for (const auto& addonId : activeAddons) {
			sol::object addonObj = addons[addonId];
			if (!addonObj.valid()) {
				LDYOM_WARN("Active addon '{}' not found, skipping on_load", addonId);
				continue;
			}
			sol::table addonTable = addonObj.as<sol::table>();
			auto hookFn = addonTable["on_load"];
			if (hookFn.valid()) {
				auto result = hookFn();
				if (!result.valid()) {
					sol::error err = result;
					LDYOM_ERROR("Addon '{}' on_load failed: {}", addonId, err.what());
				}
			}
		}
	}
}

void AddonsManager::shutdown() {
	auto lua = LuaManager::instance().getState();
	auto& state = lua.get();

	sol::object addonsObj = state["__addons"];
	if (!addonsObj.valid()) return;

	sol::table addons = addonsObj.as<sol::table>();
	for (const auto& addonId : activeAddons) {
		sol::object addonObj = addons[addonId];
		if (!addonObj.is<sol::table>()) continue;

		sol::table addonTable = addonObj.as<sol::table>();
		auto hookFn = addonTable["on_unload"];
		if (!hookFn.valid()) continue;

		auto result = hookFn();
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
	{
		auto lua = LuaManager::instance().getState();
		auto& state = lua.get();

		sol::object addonsObj = state["__addons"];
		if (!addonsObj.valid()) {
			LDYOM_ERROR("No addons table in Lua state");
			return false;
		}

		sol::table addons = addonsObj.as<sol::table>();
		if (!addons[addonId].valid()) {
			LDYOM_ERROR("Addon '{}' not found", addonId);
			return false;
		}

		sol::table addonTable = addons[addonId];
		auto hookFn = addonTable["on_load"];
		if (hookFn.valid()) {
			auto result = hookFn();
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("Addon '{}' on_load failed: {}", addonId, err.what());
				return false;
			}
		}
	} // release Lua lock before writing settings

	activeAddons.insert(addonId);
	Settings::instance().setSetting("active_addons", activeAddons);
	return true;
}

bool AddonsManager::disableAddon(const std::string& addonId) {
	{
		auto lua = LuaManager::instance().getState();
		auto& state = lua.get();

		sol::object addonsObj = state["__addons"];
		if (!addonsObj.valid()) {
			LDYOM_ERROR("No addons table in Lua state");
			return false;
		}

		sol::table addons = addonsObj.as<sol::table>();
		if (!addons[addonId].valid()) {
			LDYOM_ERROR("Addon '{}' not found", addonId);
			return false;
		}

		sol::table addonTable = addons[addonId];
		auto hookFn = addonTable["on_unload"];
		if (hookFn.valid()) {
			auto result = hookFn();
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("Addon '{}' on_unload failed: {}", addonId, err.what());
				return false;
			}
		}
	} // release Lua lock before writing settings

	activeAddons.erase(addonId);
	Settings::instance().setSetting("active_addons", activeAddons);
	return true;
}

bool AddonsManager::isAddonActive(const std::string& addonId) const {
	return activeAddons.contains(addonId);
}

std::vector<AddonsMetadata> AddonsManager::getAddons() const {
	std::vector<AddonsMetadata> addonsList;
	auto lua = LuaManager::instance().getState();
	auto& state = lua.get();

	sol::object addonsObj = state["__addons"];
	if (!addonsObj.valid()) return addonsList;

	sol::table addons = addonsObj.as<sol::table>();
	for (const auto& [key, val] : addons) {
		if (!val.is<sol::table>()) continue;

		sol::table addonTable = val.as<sol::table>();
		try {
			AddonsMetadata metadata;
			metadata.id = key.as<std::string>();
			metadata.name = addonTable["name"].get<std::string>();
			metadata.description = addonTable["description"].get<std::string>();
			metadata.version = addonTable["version"].get<std::string>();
			if (addonTable["author"].valid()) {
				metadata.author = addonTable["author"].get<std::string>();
			}
			addonsList.emplace_back(std::move(metadata));
		} catch (const std::exception& e) {
			LDYOM_ERROR("Error reading metadata for addon '{}': {}", key.as<std::string>(), e.what());
		}
	}

	return addonsList;
}
