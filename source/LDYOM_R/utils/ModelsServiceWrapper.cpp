#include <sol.hpp>

#include "ModelsService.h"

void modelsServiceWrapper(sol::state &state) {
	auto ldyomTable = state["ld"].get_or_create<sol::table>();
	auto modelsTable = ldyomTable.create_named("models");
	modelsTable["getPacksNames"] = []() {
		return &ModelsService::getInstance().getPacksNames();
	};
	modelsTable["getAnimations"] = []() {
		return &ModelsService::getInstance().getAnimations();
	};
	modelsTable["getTextStyles"] = []() {
		return &ModelsService::getInstance().getTextStyles();
	};
	modelsTable["getPedBones"] = []() {
		return &ModelsService::getInstance().getPedBones();
	};
	modelsTable["getParticlesNames"] = []() {
		return &ModelsService::getInstance().getParticlesNames();
	};
	modelsTable["getVehicleModels"] = []() {
		return &ModelsService::getInstance().getVehicleModels();
	};
	modelsTable["getSpecialsPed"] = []() {
		return &ModelsService::getInstance().getSpecialsPed();
	};
	modelsTable["getWeaponIds"] = []() {
		return &ModelsService::getInstance().getWeaponIds();
	};
	modelsTable["getPedModels"] = []() {
		return &ModelsService::getInstance().getPedModels();
	};
}
