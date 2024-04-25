#include "ModelsService.h"

#include <fstream>

#include "CModelInfo.h"
#include "Logger.h"
#include "Settings.h"
#include "easylogging/easylogging++.h"
#include "fmt/core.h"

namespace SQLite {
	/// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
	void assertion_failed(const char *apFile, const long apLine, const char *apFunc, const char *apExpr,
	                      const char *apMsg) {
		// Print a message to the standard error output stream, and abort the program.
		std::stringstream error;
		error << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc <<
			"() with message \"" << apMsg << "\"\n";
		Logger::getInstance().log(error.str());
		std::abort();
	}
}

const auto LDYOM_RESOURCES_PATH = std::filesystem::path("LDYOM/Resources");

ModelsService::ModelsService():
	db_("LDYOM/models.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
	SQLite::Statement query(this->db_, "PRAGMA foreign_keys = ON;");
	query.exec();
}

void ModelsService::Init() {
	pedModels_ = {
		0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
		36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
		66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95,
		96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
		120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
		142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
		165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
		187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209,
		210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
		232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
		254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284,
		285, 286, 287, 288
	};
	auto pedModels = Settings::getInstance().get<std::vector<int>>("data.pedModels");
	if (pedModels.has_value())
		pedModels_.insert(pedModels_.end(), pedModels->begin(), pedModels->end());

	weaponIds_ = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
		34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46
	};
	auto weaponIds = Settings::getInstance().get<std::vector<int>>("data.weaponModels");
	if (weaponIds.has_value())
		weaponIds_.insert(weaponIds_.end(), weaponIds->begin(), weaponIds->end());


	specialsPed_ = {
		"andre", "bbthin", "bb", "cat", "cesar", "claude", "dwayne", "emmet", "forelli", "janitor", "jethro", "jizzy",
		"hern", "kendl", "maccer", "maddogg", "ogloc", "paul", "pulaski", "rose", "ryder", "ryder3", "sindaco", "smoke",
		"smokev", "suzie", "sweet", "tbone", "tenpen", "torino", "truth", "wuzimu", "zero", "gangrl2", "copgrl1",
		"copgrl2", "crogrl1", "crogrl2", "gungrl1", "gungrl2", "mecgrl2", "nurgrl2", "ryder2", "cdeput", "sfpdm1",
		"lvpdm1", "csbmydj", "psycho", "csmech", "csomost", "wmycd2"
	};
	auto specialsPed = Settings::getInstance().get<std::vector<std::string>>("data.specialPedModels");
	if (specialsPed.has_value())
		specialsPed_.insert(specialsPed_.end(), specialsPed->begin(), specialsPed->end());

	vehicleModels_ = {
		400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421,
		422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443,
		444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465,
		466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487,
		488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509,
		510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531,
		532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553,
		554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575,
		576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597,
		598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611
	};
	auto vehicleModels = Settings::getInstance().get<std::vector<int>>("data.vehicleModels");
	if (vehicleModels.has_value())
		vehicleModels_.insert(vehicleModels_.end(), vehicleModels->begin(), vehicleModels->end());

	if (exists(LDYOM_RESOURCES_PATH / "animations.json")) {
		std::ifstream file(LDYOM_RESOURCES_PATH / "animations.json");
		animations_ = nlohmann::json::parse(file);
	} else {
		CLOG(ERROR, "LDYOM") << "File animations.json not found";
	}

	if (auto userAnimations = Settings::getInstance().get<std::map<
		std::string, std::vector<std::string>>>("data.pedAnimations"); userAnimations.has_value()) {
		for (auto [packName, newAnims] : userAnimations.value()) {
			if (animations_.contains(packName)) {
				auto &anims = animations_[packName];
				anims.reserve(anims.size() + newAnims.size());
				anims.insert(anims.end(), newAnims.begin(), newAnims.end());
			} else {
				animations_[packName] = newAnims;
			}
		}
	}
	packsNames_.reserve(animations_.size());
	for (const auto packName : animations_ | std::views::keys)
		packsNames_.emplace_back(packName);

	this->particlesNames_ = {
		"blood_heli", "boat_prop", "camflash", "carwashspray", "cement", "cloudfast", "coke_puff", "coke_trail",
		"cigarette_smoke", "explosion_barrel", "explosion_crate", "explosion_door", "exhale", "explosion_fuel_car",
		"explosion_large", "explosion_medium", "explosion_molotov", "explosion_small", "explosion_tiny", "extinguisher",
		"flame", "fire", "fire_med", "fire_large", "flamethrower", "fire_bike", "fire_car", "gunflash", "gunsmoke",
		"insects", "heli_dust", "jetpack", "jetthrust", "nitro", "molotov_flame", "overheat_car",
		"overheat_car_electric", "prt_boatsplash", "prt_cardebris", "prt_collisionsmoke", "prt_glass", "prt_gunshell",
		"prt_sand", "prt_sand2", "prt_smokeII_3_expand", "prt_smoke_huge", "prt_spark", "prt_spark_2", "prt_splash",
		"prt_wake", "prt_watersplash", "prt_wheeldirt", "petrolcan", "puke", "riot_smoke", "spraycan", "smoke30lit",
		"smoke30m", "smoke50lit", "shootlight", "smoke_flare", "tank_fire", "teargas", "teargasAD", "tree_hit_fir",
		"tree_hit_palm", "vent", "vent2", "water_hydrant", "water_ripples", "water_speed", "water_splash",
		"water_splash_big", "water_splsh_sml", "water_swim", "waterfall_end", "water_fnt_tme", "water_fountain",
		"wallbust", "WS_factorysmoke"
	};
	this->pedBones_ = {
		{1, "BONE_PELVIS1"}, {2, "BONE_PELVIS"}, {3, "BONE_SPINE1"}, {4, "BONE_UPPERTORSO"}, {5, "BONE_NECK"},
		{6, "BONE_HEAD2"}, {7, "BONE_HEAD1"}, {8, "BONE_HEAD"}, {21, "BONE_RIGHTUPPERTORSO"},
		{22, "BONE_RIGHTSHOULDER"}, {23, "BONE_RIGHTELBOW"}, {24, "BONE_RIGHTWRIST"}, {25, "BONE_RIGHTHAND"},
		{26, "BONE_RIGHTTHUMB"}, {31, "BONE_LEFTUPPERTORSO"}, {32, "BONE_LEFTSHOULDER"}, {33, "BONE_LEFTELBOW"},
		{34, "BONE_LEFTWRIST"}, {35, "BONE_LEFTHAND"}, {36, "BONE_LEFTTHUMB"}, {41, "BONE_LEFTHIP"},
		{42, "BONE_LEFTKNEE"}, {43, "BONE_LEFTANKLE"}, {44, "BONE_LEFTFOOT"}, {51, "BONE_RIGHTHIP"},
		{52, "BONE_RIGHTKNEE"}, {53, "BONE_RIGHTANKLE"}, {54, "BONE_RIGHTFOOT"}
	};
	this->textStyles_ = {
		STYLE_MIDDLE, STYLE_BOTTOM_RIGHT, STYLE_MIDDLE_SMALLER, STYLE_MIDDLE_SMALLER_HIGHER, STYLE_WHITE_MIDDLE_SMALLER,
		STYLE_LIGHTBLUE_TOP
	};
}

int ModelsService::validPedModel(const int model) {
	int returnModel = 0;
	for (const int pedModel : getInstance().getPedModels()) {
		if (pedModel <= model)
			returnModel = pedModel;
	}
	return returnModel;
}

int ModelsService::validWeaponModel(const int model) {
	int returnModel = 0;
	for (const int weaponModel : getInstance().getWeaponIds()) {
		if (weaponModel <= model)
			returnModel = weaponModel;
	}
	return returnModel;
}

int ModelsService::validVehiceModel(int model) {
	int returnModel = 0;
	for (const int pedModel : getInstance().getVehicleModels()) {
		if (pedModel <= model)
			returnModel = pedModel;
	}
	return returnModel;
}

bool ModelsService::validObjectModel(const int model) const {
	return CModelInfo::GetModelInfo(model) != nullptr;
}

std::optional<int> ModelsService::addNewTag(const std::string &tagName) {
	try {
		SQLite::Statement query(this->db_, "INSERT INTO tags (name) VALUES(?) RETURNING id;");
		query.bind(1, tagName);
		if (query.executeStep()) {
			loadModelTags();
			return query.getColumn(0).getInt();
		}
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
	}
	return std::nullopt;
}

bool ModelsService::deleteTag(int tagId) {
	try {
		SQLite::Statement query(this->db_, "DELETE FROM tags WHERE id = ?;");
		query.bind(1, tagId);
		query.exec();
		loadModelTags();
		return true;
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

bool ModelsService::hasObjectInfo(int objectModel) {
	return true;
}

bool ModelsService::setModelToTag(int modelId, int tagId) {
	try {
		SQLite::Statement query(this->db_, "INSERT INTO tag_to_models(tag_id, model_id) VALUES(?, ?);");
		query.bind(1, tagId);
		query.bind(2, modelId);
		query.exec();
		return true;
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

bool ModelsService::clearModelToTag(int modelId, int tagId) {
	try {
		SQLite::Statement query(this->db_, "DELETE FROM tag_to_models WHERE tag_id = ? AND model_id = ?");
		query.bind(1, tagId);
		query.bind(2, modelId);
		query.exec();
		return true;
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

bool ModelsService::newTitleNameModel(int modelId, const std::string &newTitleName) {
	try {
		SQLite::Statement query(this->db_, "UPDATE models SET title_name= ? WHERE model_id= ?;");
		query.bind(1, newTitleName);
		query.bind(2, modelId);
		query.exec();
		return true;
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

std::set<int> ModelsService::getModelTags(int model) {
	std::set<int> tags;
	try {
		SQLite::Statement query(this->db_, "SELECT tag_id FROM tag_to_models WHERE model_id = ?");
		query.bind(1, model);
		while (query.executeStep()) {
			tags.emplace(query.getColumn(0));
		}
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
	return tags;
}

const auto ICONS_MODELS_PATH = "LDYOM/icons_models.sqlite3";

std::optional<std::unique_ptr<Texture>> ModelsService::getModelIcon(const int modelId) {
	if (!std::filesystem::exists(ICONS_MODELS_PATH))
		return std::nullopt;
	if (!this->iconDb_.has_value()) {
		this->iconDb_ = SQLite::Database(ICONS_MODELS_PATH);
	}
	try {
		SQLite::Statement query(this->iconDb_.value(), "SELECT icon FROM icons WHERE model_id = ?;");
		query.bind(1, modelId);
		if (query.executeStep()) {
			const auto iconData = query.getColumn(0).getBlob();
			const int iconDataSize = query.getColumn(0).getBytes();
			return utils::LoadTextureRequiredFromMemory(iconData, iconDataSize);
		}
		return std::nullopt;
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
}

void ModelsService::loadModelCategories() {
	this->modelCategories_ = std::vector<ObjectModelCategory>();
	try {
		SQLite::Statement query(this->db_, "SELECT * FROM categories;");
		while (query.executeStep()) {
			this->modelCategories_->emplace_back(
				query.getColumn("id"),
				query.getColumn("category_name"),
				query.getColumn("parent_id")
			);
		}
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
}

std::vector<ObjectModelCategory>& ModelsService::getObjectModelCategories() {
	if (!this->modelCategories_.has_value()) {
		loadModelCategories();
	}
	return this->modelCategories_.value();
}

void ModelsService::loadModelTags() {
	this->modelTags_ = std::vector<ObjectModelTag>();
	try {
		SQLite::Statement query(this->db_, "SELECT * FROM tags;");
		while (query.executeStep()) {
			this->modelTags_->emplace_back(
				query.getColumn("id"),
				query.getColumn("name")
			);
		}
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
}

std::vector<ObjectModelTag>& ModelsService::getObjectModelTags() {
	if (!this->modelTags_.has_value()) {
		loadModelTags();
	}
	return this->modelTags_.value();
}

std::map<int, ObjectInfo> ModelsService::getObjectModelsBySearch(const std::string &searchText,
                                                                 std::set<int> &categories,
                                                                 int objectBehaviourHasCollision,
                                                                 int objectBehaviourDestroyByHit,
                                                                 int objectBehaviourSeeByTime, int countOnPage,
                                                                 int page, int &countModels) {
	std::map<int, ObjectInfo> models{};

	try {
		auto baseQuery = fmt::format(
			"FROM category_to_models JOIN models ON category_to_models.model_id = models.model_id WHERE (CAST(models.model_id as TEXT) LIKE '%{}%' OR models.title_name LIKE '%{}%' OR models.model_name LIKE '%{}%') ",
			searchText, searchText, searchText);

		if (objectBehaviourHasCollision != -1) {
			baseQuery += fmt::format(" AND models.has_collision = {}", objectBehaviourHasCollision);
		}
		if (objectBehaviourDestroyByHit != -1) {
			baseQuery += fmt::format(" AND models.is_destroy_by_hit = {}", objectBehaviourDestroyByHit);
		}
		if (objectBehaviourSeeByTime != -1) {
			baseQuery += fmt::format(" AND models.see_by_time = {}", objectBehaviourSeeByTime);
		}

		if (!categories.empty()) {
			std::set categoriesRaw(categories);
			const auto &objectModelCategories = getObjectModelCategories();
			int i = 0;
			while (i < static_cast<int>(objectModelCategories.size())) {
				if (categoriesRaw.contains(objectModelCategories[i].parentId)) {
					categoriesRaw.emplace(objectModelCategories[i].id);
				}
				i++;
			}

			std::string strCategories;
			for (int category : categoriesRaw)
				strCategories += fmt::format("{},", category);
			strCategories.pop_back();
			baseQuery += fmt::format(" AND category_id IN({})", strCategories);
		}


		std::string getQuery = fmt::format("SELECT models.* {} LIMIT {}, {}", baseQuery, (page - 1) * countOnPage,
		                                   countOnPage);
		std::string countQuery = "SELECT COUNT(*) " + baseQuery;

		SQLite::Statement query(this->db_, getQuery.c_str());
		while (query.executeStep()) {
			models.emplace(query.getColumn("model_id").getInt(),
			               ObjectInfo{
				               query.getColumn("model_id"),
				               query.getColumn("title_name"),
				               query.getColumn("model_name"),
				               query.getColumn("has_collision").getInt() == 1,
				               query.getColumn("is_destroy_by_hit").getInt() == 1,
				               query.getColumn("see_by_time").getInt() == 1,
				               query.getColumn("icon_url")
			               });
		}
		SQLite::Statement queryCount(this->db_, countQuery.c_str());
		if (queryCount.executeStep()) {
			countModels = queryCount.getColumn(0);
		}
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}

	return models;
}

std::map<int, ObjectInfo> ModelsService::getObjectModelsByTag(int tagId, int page, int countOnPage, int &countModels) {
	std::map<int, ObjectInfo> models{};

	try {
		auto baseQuery = "FROM models JOIN tag_to_models ON models.model_id = tag_to_models.model_id WHERE tag_id = ?";

		std::string getQuery = fmt::format("SELECT models.* {} LIMIT {}, {}", baseQuery, (page - 1) * countOnPage,
		                                   countOnPage);
		std::string countQuery = fmt::format("SELECT COUNT(*) {}", baseQuery);

		SQLite::Statement query(this->db_, getQuery.c_str());
		query.bind(1, tagId);
		while (query.executeStep()) {
			models.emplace(query.getColumn("model_id").getInt(),
			               ObjectInfo{
				               query.getColumn("model_id"),
				               query.getColumn("title_name"),
				               query.getColumn("model_name"),
				               query.getColumn("has_collision").getInt() == 1,
				               query.getColumn("is_destroy_by_hit").getInt() == 1,
				               query.getColumn("see_by_time").getInt() == 1,
				               query.getColumn("icon_url")
			               });
		}
		SQLite::Statement queryCount(this->db_, countQuery.c_str());
		queryCount.bind(1, tagId);
		if (queryCount.executeStep()) {
			countModels = queryCount.getColumn(0);
		}
	} catch (std::exception &e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}

	return models;
}

std::vector<int>& ModelsService::getPedModels() {
	return pedModels_;
}

std::vector<int>& ModelsService::getWeaponIds() {
	return weaponIds_;
}

std::vector<std::string>& ModelsService::getSpecialsPed() {
	return specialsPed_;
}

std::vector<int>& ModelsService::getVehicleModels() {
	return vehicleModels_;
}

std::vector<std::string>& ModelsService::getParticlesNames() {
	return particlesNames_;
}

std::map<int, std::string>& ModelsService::getPedBones() {
	return pedBones_;
}

std::vector<std::string>& ModelsService::getPacksNames() {
	return packsNames_;
}

std::map<std::string, std::vector<std::string>>& ModelsService::getAnimations() {
	return animations_;
}

std::array<int, 6>& ModelsService::getTextStyles() {
	return textStyles_;
}
