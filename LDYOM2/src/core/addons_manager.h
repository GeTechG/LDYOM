#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_set>

struct AddonsMetadata {
	std::string id;
	std::string name;
	std::string description;
	std::string version;
	std::optional<std::string> author;
};

class AddonsManager {
  private:
	static const std::string ADDONS_DIR_PATH;

	std::unordered_set<std::string> activeAddons;

	AddonsManager() = default;
	bool loadAddonMetadata(const std::filesystem::path& addonPath);

  public:
	static AddonsManager& instance();

	AddonsManager(const AddonsManager&) = delete;
	void operator=(const AddonsManager&) = delete;

	void initialize();
	void shutdown();

	bool enableAddon(const std::string& addonId);
	bool disableAddon(const std::string& addonId);
	bool isAddonActive(const std::string& addonId) const;

	std::vector<AddonsMetadata> getAddons() const;
	std::unordered_set<std::string>& getActiveAddons() { return activeAddons; }
};