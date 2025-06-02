#include "settings.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <logger.h>
#include <paths.h>

const std::string Settings::CONFIG_FILE_PATH = "settings.json";

Settings& Settings::instance() {
	static Settings instance;
	return instance;
}

void Settings::initialize() {
	this->m_config = {{"lang", "en"},
	                  {"hotkeys", std::unordered_map<std::string, std::string>()},
	                  {"display_scale", 1.f},
	                  {"active_addons", {"skic.core"}}};

	std::filesystem::path configPath(LDYOM_PATH(CONFIG_FILE_PATH));
	if (exists(configPath)) {
		nlohmann::json tempData;
		try {
			std::ifstream file(configPath);
			if (file.is_open()) {
				file >> tempData;
				file.close();
			}
		} catch (const std::exception& e) {
			LDYOM_ERROR("Error reading configuration: {}", e.what());
		}
		this->m_config.merge_patch(tempData);
	}

	save();
}

bool Settings::save() {
	try {
		std::filesystem::path configPath(LDYOM_PATH(CONFIG_FILE_PATH));
		create_directories(configPath.parent_path());

		std::ofstream file(configPath);
		if (file.is_open()) {
			file << m_config.dump(4);
			file.close();
			return true;
		}
	} catch (const std::exception& e) {
		LDYOM_ERROR("Error saving configuration: {}", e.what());
	}

	return false;
}

nlohmann::json::json_pointer Settings::dotToPointer(const std::string& dotKey) {
	if (dotKey.empty()) {
		return nlohmann::json::json_pointer();
	}

	std::string result;
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	while ((end = dotKey.find('.', start)) != std::string::npos) {
		result += "/" + dotKey.substr(start, end - start);
		start = end + 1;
	}
	result += "/" + dotKey.substr(start);

	return nlohmann::json::json_pointer(result);
}