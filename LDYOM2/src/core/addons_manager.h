#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

struct AddonsMetadata {
	std::string id;
	std::string name;
	std::string description;
	std::string version;
	std::optional<std::string> author;
};

class AddonsManager {
  private:
	static constexpr const char* ADDONS_DIR = "addons";

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

	std::vector<AddonsMetadata> getAddons() const;
	const std::unordered_set<std::string>& getActiveAddons() const { return activeAddons; }
};
