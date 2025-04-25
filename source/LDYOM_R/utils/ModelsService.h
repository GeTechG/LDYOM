#pragma once
#include <optional>
#include <vector>
#include <string>

#define SQLITECPP_ENABLE_ASSERT_HANDLER

#include <array>
#include <set>
#include <SQLiteCpp/Database.h>
#include "../Data/Texture.h"

struct ObjectInfo {
	int modelId;
	std::string titleName;
	std::string modelName;
	bool hasCollision;
	bool isDestroyByHit;
	bool seeByTime;
	std::string iconUrl;
};

struct ObjectModelCategory {
	int id;
	std::string name;
	int parentId = -1;
};

struct ObjectModelTag {
	int id;
	std::string name;
};

class ModelsService {
private:
	std::vector<int> pedModels_;
	std::vector<int> vehicleModels_;
	std::vector<int> weaponIds_;
	std::vector<std::string> specialsPed_;
	std::optional<std::vector<ObjectModelCategory>> modelCategories_;
	std::optional<std::vector<ObjectModelTag>> modelTags_;
	std::vector<std::string> particlesNames_;
	std::map<int, std::string> pedBones_;
	std::vector<std::string> packsNames_;
	std::map<std::string, std::vector<std::string>> animations_;
	std::array<int, 6> textStyles_;

	SQLite::Database db_;
	std::optional<SQLite::Database> iconDb_;

	void loadModelCategories();
	void loadModelTags();

	ModelsService();
	ModelsService(const ModelsService& root) = delete;
	ModelsService& operator=(const ModelsService&) = delete;
public:
	static ModelsService& getInstance() {
		static ModelsService instance;
		return instance;
	}

	void Init();
	void Update() {
		Init();
	}

	static int validPedModel(int model);
	static int validWeaponModel(int model);
	static int validVehiceModel(int model);
	bool validObjectModel(int model) const;

	std::optional<int> addNewTag(const std::string& tagName);
	bool deleteTag(int tagId);
	bool hasObjectInfo(int objectModel);
	bool setModelToTag(int modelId, int tagId);
	bool clearModelToTag(int modelId, int tagId);
	bool newTitleNameModel(int modelId, const std::string& newTitleName);

	std::set<int> getModelTags(int model);

	std::optional<std::unique_ptr<Texture>> getModelIcon(int modelId);

	std::vector<ObjectModelCategory>& getObjectModelCategories();
	std::vector<ObjectModelTag>& getObjectModelTags();

	std::map<int, ObjectInfo> getObjectModelsBySearch(const std::string& searchText, std::set<int>& categories, int objectBehaviourHasCollision, int
	                                                  objectBehaviourDestroyByHit, int objectBehaviourSeeByTime, int countOnPage, int page, int& countModels);
	std::map<int, ObjectInfo> getObjectModelsByTag(int tagId, int page, int
	                                               countOnPage, int& countModels);

	std::vector<int>& getPedModels();
	std::vector<int>& getWeaponIds();
	std::vector<std::string>& getSpecialsPed();
	std::vector<int>& getVehicleModels();
	std::vector<std::string>& getParticlesNames();
	std::map<int, std::string>& getPedBones();
	std::vector<std::string>& getPacksNames();
	std::map<std::string, std::vector<std::string>>& getAnimations();
	std::array<int, 6>& getTextStyles();
};

