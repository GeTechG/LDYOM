#pragma once
#include <vector>
#include <string>

class ModelsService {
private:
	std::vector<int> pedModels_;
	std::vector<int> weaponIds_;
	std::vector<std::string> specialsPed_;

	ModelsService() = default;
	ModelsService(const ModelsService& root) = delete;
	ModelsService& operator=(const ModelsService&) = delete;
public:
	static ModelsService& getInstance() {
		static ModelsService instance;
		return instance;
	}

	void Init();

	std::vector<int>& getPedModels();
	std::vector<int>& getWeaponIds();
	std::vector<std::string>& getSpecialsPed();
};

