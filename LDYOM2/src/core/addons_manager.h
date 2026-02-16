#pragma once
#include <filesystem>
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct AddonMetadata {
	std::string id;
	std::string name;
	std::string description;
	std::string version;
	std::optional<std::string> author;
	sol::protected_function on_load;
	sol::protected_function on_unload;
	int priority = 0;
	std::filesystem::path path;
};

class AddonsManager {
  private:
	static constexpr const char* ADDONS_DIR = "addons";

	std::vector<AddonMetadata> addonsList;
	std::unordered_set<std::string> activeAddons;
	std::unordered_map<std::string, sol::object> sharedData;
	std::filesystem::path m_currentLoadingPath;

	AddonsManager() = default;
	bool loadAddonMetadata(const std::filesystem::path& addonPath);
	void loadAddonLocale(const AddonMetadata& addon);

  public:
	static AddonsManager& instance();

	AddonsManager(const AddonsManager&) = delete;
	AddonsManager& operator=(const AddonsManager&) = delete;

	void initialize();
	void shutdown();

	bool enableAddon(const std::string& addonId);
	bool disableAddon(const std::string& addonId);
	bool isAddonActive(const std::string& addonId) const;

	void setSharedValue(const std::string& key, sol::object value);
	sol::object getSharedValue(const std::string& key) const;
	bool hasSharedValue(const std::string& key) const;
	void removeSharedValue(const std::string& key);

	const std::vector<AddonMetadata>& getAddons() const;
	const std::unordered_set<std::string>& getActiveAddons() const { return activeAddons; }
};
