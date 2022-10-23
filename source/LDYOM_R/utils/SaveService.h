#pragma once

#include <unordered_map>
#include "nlohmann/json.hpp"

struct SaveData {
	int sceneId = 0;
	std::string luaData;
	std::string objectiveUuid;
	int nodePinId;
	int playerModel = 0;
	std::array<float, 3> playerPosition;
	std::array<unsigned, 10> clotherMAnModelKeys_;
	std::array<unsigned, 18> clotherMAnTextureKeys_;
	float fatStat = 1.f;
	float musculeStat = 1.f;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveData, sceneId, luaData, objectiveUuid, nodePinId, playerModel, playerPosition, clotherMAnModelKeys_, clotherMAnTextureKeys_, fatStat, musculeStat);
};

class SaveService {
private:
	std::unordered_map<std::string, SaveData> saves_;

	SaveService() = default;
	SaveService(const SaveService & root) = delete;
	SaveService& operator=(const SaveService &) = delete;

	void saveToFile();
public:

	static SaveService& getInstance() {
		static SaveService instance;
		return instance;
	}
	void Init();

	void addSave(const std::string& projectName, const SaveData& data);
	std::optional<SaveData*> getSave(const std::string& projectName);
};
