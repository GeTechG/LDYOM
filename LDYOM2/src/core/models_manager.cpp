#include "models_manager.h"

#include "nlohmann/json.hpp"
#include "paths.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

std::vector<int> ModelsManager::m_pedModels = {
	0,   7,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
	30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,
	54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  66,  67,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,
	79,  80,  81,  82,  83,  84,  85,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102,
	103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126,
	127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150,
	151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
	174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
	197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
	221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
	244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275,
	276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288};

std::vector<int> ModelsManager::m_vehicleModels = {
	400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421,
	422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443,
	444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465,
	466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487,
	488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509,
	510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531,
	532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553,
	554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575,
	576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597,
	598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611};

std::vector<std::string> ModelsManager::m_pedSpecialModels = {
	"andre",   "bbthin",  "bb",      "cat",     "cesar",   "claude",  "dwayne",  "emmet",   "forelli",
	"janitor", "jethro",  "jizzy",   "hern",    "kendl",   "maccer",  "maddogg", "ogloc",   "paul",
	"pulaski", "rose",    "ryder",   "ryder3",  "sindaco", "smoke",   "smokev",  "suzie",   "sweet",
	"tbone",   "tenpen",  "torino",  "truth",   "wuzimu",  "zero",    "gangrl2", "copgrl1", "copgrl2",
	"crogrl1", "crogrl2", "gungrl1", "gungrl2", "mecgrl2", "nurgrl2", "ryder2",  "cdeput",  "sfpdm1",
	"lvpdm1",  "csbmydj", "psycho",  "csmech",  "csomost", "wmycd2"};

std::vector<int> ModelsManager::m_weaponIds = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                                               16, 17, 18, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                                               34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46};

std::unordered_map<std::string, std::vector<std::string>> ModelsManager::m_pedAnimations;

std::vector<std::string> ModelsManager::m_particlesNames = {"blood_heli",
                                                            "boat_prop",
                                                            "camflash",
                                                            "carwashspray",
                                                            "cement",
                                                            "cloudfast",
                                                            "coke_puff",
                                                            "coke_trail",
                                                            "cigarette_smoke",
                                                            "explosion_barrel",
                                                            "explosion_crate",
                                                            "explosion_door",
                                                            "exhale",
                                                            "explosion_fuel_car",
                                                            "explosion_large",
                                                            "explosion_medium",
                                                            "explosion_molotov",
                                                            "explosion_small",
                                                            "explosion_tiny",
                                                            "extinguisher",
                                                            "flame",
                                                            "fire",
                                                            "fire_med",
                                                            "fire_large",
                                                            "flamethrower",
                                                            "fire_bike",
                                                            "fire_car",
                                                            "gunflash",
                                                            "gunsmoke",
                                                            "insects",
                                                            "heli_dust",
                                                            "jetpack",
                                                            "jetthrust",
                                                            "nitro",
                                                            "molotov_flame",
                                                            "overheat_car",
                                                            "overheat_car_electric",
                                                            "prt_boatsplash",
                                                            "prt_cardebris",
                                                            "prt_collisionsmoke",
                                                            "prt_glass",
                                                            "prt_gunshell",
                                                            "prt_sand",
                                                            "prt_sand2",
                                                            "prt_smokeII_3_expand",
                                                            "prt_smoke_huge",
                                                            "prt_spark",
                                                            "prt_spark_2",
                                                            "prt_splash",
                                                            "prt_wake",
                                                            "prt_watersplash",
                                                            "prt_wheeldirt",
                                                            "petrolcan",
                                                            "puke",
                                                            "riot_smoke",
                                                            "spraycan",
                                                            "smoke30lit",
                                                            "smoke30m",
                                                            "smoke50lit",
                                                            "shootlight",
                                                            "smoke_flare",
                                                            "tank_fire",
                                                            "teargas",
                                                            "teargasAD",
                                                            "tree_hit_fir",
                                                            "tree_hit_palm",
                                                            "vent",
                                                            "vent2",
                                                            "water_hydrant",
                                                            "water_ripples",
                                                            "water_speed",
                                                            "water_splash",
                                                            "water_splash_big",
                                                            "water_splsh_sml",
                                                            "water_swim",
                                                            "waterfall_end",
                                                            "water_fnt_tme",
                                                            "water_fountain",
                                                            "wallbust",
                                                            "WS_factorysmoke"};

const auto ANIMATION_FILE_PATH = "animations.json";

void ModelsManager::loadData() {
	if (const std::filesystem::path animationsPath(LDYOM_PATH(ANIMATION_FILE_PATH)); exists(animationsPath)) {
		std::ifstream file(animationsPath);
		const auto json = nlohmann::json::parse(file);
		json.get_to(m_pedAnimations);
		file.close();
	}
}

const std::unordered_map<std::string, std::vector<std::string>>& ModelsManager::getPedAnimations() {
	return m_pedAnimations;
}

void ModelsManager::addPedAnimation(const std::string& packName, const std::string& animName) {
	if (!m_pedAnimations.contains(packName)) {
		m_pedAnimations[packName] = std::vector<std::string>();
	}
	m_pedAnimations[packName].push_back(animName);
}

bool ModelsManager::validatePedModel(int modelId) {
	return std::ranges::find(m_pedModels, modelId) != m_pedModels.end();
}

bool ModelsManager::validateVehicleModel(int modelId) {
	return std::ranges::find(m_vehicleModels, modelId) != m_vehicleModels.end();
}

bool ModelsManager::validatePedSpecialModel(std::string modelName) {
	return std::ranges::find(m_pedSpecialModels, modelName) != m_pedSpecialModels.end();
}

bool ModelsManager::validateWeaponId(int weaponId) {
	return std::ranges::find(m_weaponIds, weaponId) != m_weaponIds.end();
}

bool ModelsManager::validObjectModel(int modelId) { return CModelInfo::GetModelInfo(modelId) != nullptr; }

bool ModelsManager::validatePedAnimation(const std::string& packName, const std::string& animName) {
	if (const auto it = m_pedAnimations.find(packName); it != m_pedAnimations.end()) {
		const auto& animations = it->second;

		std::string upperAnimName = animName;
		std::ranges::transform(upperAnimName, upperAnimName.begin(),
		                       [](const unsigned char c) { return std::toupper(c); });

		const auto found = std::ranges::find_if(animations, [&upperAnimName](const std::string& animation) {
			std::string upperAnimation = animation;
			std::ranges::transform(upperAnimation, upperAnimation.begin(),
			                       [](const unsigned char c) { return std::toupper(c); });
			return upperAnimation == upperAnimName;
		});

		return found != animations.end();
	}
	return false;
}

bool ModelsManager::validateParticleName(const std::string& particleName) {
	return std::find(m_particlesNames.begin(), m_particlesNames.end(), particleName) != m_particlesNames.end();
}
