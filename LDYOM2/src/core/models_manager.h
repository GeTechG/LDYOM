#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class ModelsManager {
	static std::vector<int> m_pedModels;
	static std::vector<int> m_vehicleModels;
	static std::vector<std::string> m_pedSpecialModels;
	static std::vector<int> m_weaponIds;
	static std::unordered_map<std::string, std::vector<std::string>> m_pedAnimations;

  public:
	static void loadData();
	static const std::vector<int>& getPedModels() { return m_pedModels; }
	static const std::vector<int>& getVehicleModels() { return m_vehicleModels; }
	static const std::vector<std::string>& getPedSpecialModels() { return m_pedSpecialModels; }
	static const std::vector<int>& getWeaponIds() { return m_weaponIds; }

	static const std::unordered_map<std::string, std::vector<std::string>>& getPedAnimations();

	static void addPedModel(const int modelId) { m_pedModels.push_back(modelId); }
	static void addVehicleModel(const int modelId) { m_vehicleModels.push_back(modelId); }
	static void addPedSpecialModel(const std::string& modelName) { m_pedSpecialModels.push_back(modelName); }
	static void addWeaponId(const int weaponId) { m_weaponIds.push_back(weaponId); }

	static void addPedAnimation(const std::string& packName, const std::string& animName);

	static bool validatePedModel(int modelId);
	static bool validateVehicleModel(int modelId);
	static bool validatePedSpecialModel(std::string modelName);
	static bool validateWeaponId(int weaponId);
	static bool validObjectModel(int modelId);

	static bool validatePedAnimation(const std::string& packName, const std::string& animName);
};