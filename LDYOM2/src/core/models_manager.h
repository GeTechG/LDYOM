#pragma once
#include <string>
#include <vector>

class ModelsManager {
	static std::vector<int> m_pedModels;
	static std::vector<std::string> m_pedSpecialModels;
	static std::vector<int> m_weaponIds;

  public:
	static const std::vector<int>& GetPedModels() { return m_pedModels; }
	static const std::vector<std::string>& GetPedSpecialModels() { return m_pedSpecialModels; }
	static const std::vector<int>& GetWeaponIds() { return m_weaponIds; }

	static void addPedModel(int modelId) { m_pedModels.push_back(modelId); }
	static void addPedSpecialModel(std::string modelName) { m_pedSpecialModels.push_back(modelName); }
	static void addWeaponId(int weaponId) { m_weaponIds.push_back(weaponId); }

	static bool validatePedModel(int modelId);
	static bool validatePedSpecialModel(std::string modelName);
	static bool validateWeaponId(int weaponId);
};