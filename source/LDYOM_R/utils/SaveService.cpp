#include "SaveService.h"

#include <fstream>

#include "easylogging/easylogging++.h"

#define SAVE_FILE_PATH "LDYOM\\Saves.json"

void SaveService::saveToFile() {
	const nlohmann::json jsonData = this->saves_;
	std::ofstream o(SAVE_FILE_PATH);
	o << std::setw(4) << jsonData << std::endl;
	o.close();
}

void SaveService::Init() {
	if (std::filesystem::exists(SAVE_FILE_PATH)) {
		nlohmann::json jsonData;
		std::ifstream i("LDYOM\\Saves.json");
		i >> jsonData;
		i.close();
		try {
			this->saves_ = jsonData;
		} catch (std::exception& e) {
			CLOG(ERROR, "LDYOM") << "Invalid load saves, error: " << e.what();
		}
	}
}

void SaveService::addSave(const std::string& projectName, const SaveData& data) {
	this->saves_[projectName] = data;
	this->saveToFile();
}

std::optional<SaveData*> SaveService::getSave(const std::string& projectName) {
	if (this->saves_.contains(projectName)) {
		return &this->saves_[projectName];
	}

	return std::nullopt;
}
