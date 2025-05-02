#include "addons_manager.h"
#include "configuration.h"
#include "lua_manager.h"
#include <logger.h>
#include <paths.h>
#include <unordered_set>

const std::string AddonsManager::ADDONS_DIR_PATH = "addons";

AddonsManager& AddonsManager::instance() {
	static AddonsManager instance;
	return instance;
}

void AddonsManager::initialize() {
	const std::filesystem::path addonsDir(LDYOM_PATH(ADDONS_DIR_PATH));
	if (!std::filesystem::exists(addonsDir)) {
		std::filesystem::create_directories(addonsDir);
		LDYOM_INFO("Created addons directory: {}", addonsDir.string());
	}

	for (const auto& entry : std::filesystem::directory_iterator(addonsDir)) {
		if (entry.is_directory()) {
			loadAddonMetadata(entry.path());
		}
	}

	this->activeAddons = Configuration::instance().getSetting<std::unordered_set<std::string>>("active_addons", {});
}

void AddonsManager::shutdown() {
	try {
		auto lua = LuaManager::instance().getState();

		// Check if __addons exists in the Lua state
		if (!lua.get()["__addons"].valid()) {
			LDYOM_WARN("No addons table found during shutdown");
			return;
		}

		auto addonsTable = lua.get()["__addons"].get<sol::table>();
		if (addonsTable.empty()) {
			LDYOM_INFO("No addons to deinitialize");
			return;
		}

		for (auto [id, addon] : addonsTable) {
			if (!addon.is<sol::table>()) {
				LDYOM_WARN("Invalid addon entry (not a table)");
				continue;
			}

			sol::table addonTable = addon.as<sol::table>();
			std::string addonId;

			try {
				addonId = id.as<std::string>();
			} catch (const std::exception&) {
				LDYOM_WARN("Addon with invalid ID encountered");
				continue;
			}

			if (addonTable["deinit"].valid()) {
				try {
					addonTable["deinit"]();
					LDYOM_INFO("Addon {} deinitialized successfully", addonId);
				} catch (const std::exception& e) {
					LDYOM_ERROR("Failed to deinitialize addon {}: {}", addonId, e.what());
				}
			} else {
				LDYOM_WARN("Addon {} does not have a deinit function", addonId);
			}
		}
	} catch (const std::exception& e) {
		LDYOM_ERROR("Error during addons shutdown: {}", e.what());
	}
}

bool AddonsManager::loadAddonMetadata(const std::filesystem::path& addonPath) {
	auto initScript = addonPath / "init.lua";
	if (!std::filesystem::exists(initScript)) {
		return false;
	}

	try {
		auto lua = LuaManager::instance().getState();

		sol::environment env(lua.get(), sol::create, lua.get().globals());

		// Предоставляем функцию регистрации аддона
		lua.get()["register_addon"] = [](const sol::table metadata, sol::this_state s) {
			sol::state_view lua(s);
			auto addons = lua["__addons"].get_or_create<sol::table>();
			std::string id = metadata["id"].get<std::string>();
			if (addons[id].valid()) {
				LDYOM_ERROR("Addon with ID '{}' is already registered", id);
				return;
			}
			addons[id] = metadata;
			LDYOM_INFO("Addon registered: {} ({})", metadata["name"].get<std::string>(), id);
		};

		return LuaManager::instance().executeFileRaw(initScript.string(), env);
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to load addon metadata from {}: {}", initScript.string(), e.what());
		return false;
	}
}

bool AddonsManager::enableAddon(const std::string& addonId) {
	auto lua = LuaManager::instance().getState();
	auto addonsTable = lua.get()["__addons"].get<sol::table>();

	try {
		if (!addonsTable[addonId].valid()) {
			LDYOM_ERROR("Addon {} not found in Lua environment", addonId);
			return false;
		}

		// Вызываем функцию инициализации аддона если она есть
		if (addonsTable[addonId]["on_load"].valid()) {
			addonsTable[addonId]["on_load"]();
		} else {
			LDYOM_WARN("Addon {} does not have an 'on_load' function", addonId);
		}

		this->activeAddons.insert(addonId);
		Configuration::instance().setSetting("active_addons", this->activeAddons);

		return true;
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to enable addon {}: {}", addonId, e.what());
		return false;
	}
}

bool AddonsManager::disableAddon(const std::string& addonId) {
	try {
		auto lua = LuaManager::instance().getState();
		auto addonsTable = lua.get()["__addons"].get<sol::table>();
		if (!addonsTable[addonId].valid()) {
			LDYOM_ERROR("Addon {} not found in Lua environment", addonId);
			return false;
		}

		// Вызываем функцию деинициализации аддона если она есть
		if (addonsTable[addonId]["on_unload"].valid()) {
			addonsTable[addonId]["on_unload"]();
		} else {
			LDYOM_WARN("Addon {} does not have a 'on_unload' function", addonId);
		}

		this->activeAddons.erase(addonId);
		Configuration::instance().setSetting("active_addons", this->activeAddons);

		return true;
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to disable addon {}: {}", addonId, e.what());
		return false;
	}
}

bool AddonsManager::isAddonActive(const std::string& addonId) const {
	return activeAddons.find(addonId) != activeAddons.end();
}

std::vector<AddonsMetadata> AddonsManager::getAddons() const {
	std::vector<AddonsMetadata> addonsList;
	auto lua = LuaManager::instance().getState();
	auto addonsTable = lua.get()["__addons"].get<sol::table>();

	for (const auto& [id, addon] : addonsTable) {
		if (!addon.is<sol::table>()) {
			LDYOM_WARN("Invalid addon entry (not a table) for ID: {}", id.as<std::string>());
			continue;
		}

		sol::table addonTable = addon.as<sol::table>();
		AddonsMetadata metadata;
		try {
			metadata.id = id.as<std::string>();
			metadata.name = addonTable["name"].get<std::string>();
			metadata.description = addonTable["description"].get<std::string>();
			metadata.version = addonTable["version"].get<std::string>();
			if (addonTable["author"].valid()) {
				metadata.author = addonTable["author"].get<std::string>();
			}
			addonsList.emplace_back(std::move(metadata));
		} catch (const std::exception& e) {
			LDYOM_ERROR("Error reading metadata for addon {}: {}", id.as<std::string>(), e.what());
		}
	}

	return addonsList;
}
