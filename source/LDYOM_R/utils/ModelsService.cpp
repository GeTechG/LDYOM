#include "ModelsService.h"

#include "Settings.h"

void ModelsService::Init() {
	pedModels_ = Settings::getInstance().get<std::vector<int>>("data.pedModels").value();
	weaponIds_ = Settings::getInstance().get<std::vector<int>>("data.weaponModels").value();
	specialsPed_ = Settings::getInstance().get<std::vector<std::string>>("data.specialPedModels").value();
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
