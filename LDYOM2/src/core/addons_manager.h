#pragma once
#include <filesystem>
#include <optional>
#include <sol/sol.hpp>
#include <string>
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
};

class AddonsManager {
  private:
	static constexpr const char* ADDONS_DIR = "addons";

	std::vector<AddonMetadata> addonsList;
	std::unordered_set<std::string> activeAddons;

	AddonsManager() = default;
	bool loadAddonMetadata(const std::filesystem::path& addonPath);

  public:
	static AddonsManager& instance();

	AddonsManager(const AddonsManager&) = delete;
	AddonsManager& operator=(const AddonsManager&) = delete;

	void initialize();
	void shutdown();

	bool enableAddon(const std::string& addonId);
	bool disableAddon(const std::string& addonId);
	bool isAddonActive(const std::string& addonId) const;

	const std::vector<AddonMetadata>& getAddons() const;
	const std::unordered_set<std::string>& getActiveAddons() const { return activeAddons; }
};
