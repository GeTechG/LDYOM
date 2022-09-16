//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void PlayerOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("PlayerOp");
    //default
	table.set_function("create", [](const int& _modelId, const float& _x, const float& _y, const float& _z) {int handle_; auto result = Command<0x0053>(_modelId, _x, _y, _z, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addScore", [](const int& _self, const int& _money) { auto result = Command<0x0109>(_self, _money); return std::make_tuple(result);});
	table.set_function("isScoreGreater", [](const int& _self, const int& _money) { auto result = Command<0x010A>(_self, _money); return std::make_tuple(result);});
	table.set_function("storeScore", [](const int& _self) {int money_; auto result = Command<0x010B>(_self, &money_); return std::make_tuple(money_, result);});
	table.set_function("alterWantedLevel", [](const int& _self, const int& _wantedLevel) { auto result = Command<0x010D>(_self, _wantedLevel); return std::make_tuple(result);});
	table.set_function("alterWantedLevelNoDrop", [](const int& _self, const int& _wantedLevel) { auto result = Command<0x010E>(_self, _wantedLevel); return std::make_tuple(result);});
	table.set_function("isWantedLevelGreater", [](const int& _self, const int& _wantedLevel) { auto result = Command<0x010F>(_self, _wantedLevel); return std::make_tuple(result);});
	table.set_function("clearWantedLevel", [](const int& _self) { auto result = Command<0x0110>(_self); return std::make_tuple(result);});
	table.set_function("isDead", [](const int& _handle) { auto result = Command<0x0117>(_handle); return std::make_tuple(result);});
	table.set_function("isPressingHorn", [](const int& _self) { auto result = Command<0x0122>(_self); return std::make_tuple(result);});
	table.set_function("setControl", [](const int& _self, const bool& _state) { auto result = Command<0x01B4>(_self, _state); return std::make_tuple(result);});
	table.set_function("storeWantedLevel", [](const int& _self) {int wantedLevel_; auto result = Command<0x01C0>(_self, &wantedLevel_); return std::make_tuple(wantedLevel_, result);});
	table.set_function("getChar", [](const int& _self) {int handle_; auto result = Command<0x01F5>(_self, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setIgnorePolice", [](const int& _self, const bool& _state) { auto result = Command<0x01F7>(_self, _state); return std::make_tuple(result);});
	table.set_function("applyBrakesToCar", [](const int& _self, const bool& _state) { auto result = Command<0x0221>(_self, _state); return std::make_tuple(result);});
	table.set_function("isInRemoteMode", [](const int& _self) { auto result = Command<0x0241>(_self); return std::make_tuple(result);});
	table.set_function("isPlaying", [](const int& _self) { auto result = Command<0x0256>(_self); return std::make_tuple(result);});
	table.set_function("resetNumOfModelsKilled", [](const int& _self) { auto result = Command<0x0297>(_self); return std::make_tuple(result);});
	table.set_function("getNumOfModelsKilled", [](const int& _self, const int& _modelId) {int amount_; auto result = Command<0x0298>(_self, _modelId, &amount_); return std::make_tuple(amount_, result);});
	table.set_function("setNeverGetsTired", [](const int& _self, const bool& _state) { auto result = Command<0x0330>(_self, _state); return std::make_tuple(result);});
	table.set_function("setFastReload", [](const int& _self, const bool& _state) { auto result = Command<0x0331>(_self, _state); return std::make_tuple(result);});
	table.set_function("canStartMission", [](const int& _self) { auto result = Command<0x03EE>(_self); return std::make_tuple(result);});
	table.set_function("makeSafeForCutscene", [](const int& _self) { auto result = Command<0x03EF>(_self); return std::make_tuple(result);});
	table.set_function("setFreeHealthCare", [](const int& _self, const bool& _state) { auto result = Command<0x0414>(_self, _state); return std::make_tuple(result);});
	table.set_function("isTargettingChar", [](const int& _self, const int& _handle) { auto result = Command<0x0457>(_self, _handle); return std::make_tuple(result);});
	table.set_function("isTargettingObject", [](const int& _self, const int& _handle) { auto result = Command<0x0458>(_self, _handle); return std::make_tuple(result);});
	table.set_function("setMood", [](const int& _self, const int& _mood, const int& _time) { auto result = Command<0x04E3>(_self, _mood, _time); return std::make_tuple(result);});
	table.set_function("getWheelieStats", [](const int& _self) {int twoWheelsTime_;float twoWheelsDistance_;int wheelieTime_;float wheelieDistance_;int stoppieTime_;float stoppieDistance_; auto result = Command<0x04FC>(_self, &twoWheelsTime_, &twoWheelsDistance_, &wheelieTime_, &wheelieDistance_, &stoppieTime_, &stoppieDistance_); return std::make_tuple(twoWheelsTime_, twoWheelsDistance_, wheelieTime_, wheelieDistance_, stoppieTime_, stoppieDistance_, result);});
	table.set_function("isWearing", [](const int& _self, const char* _modelName, const int& _bodyPart) { auto result = Command<0x0500>(_self, _modelName, _bodyPart); return std::make_tuple(result);});
	table.set_function("setCanDoDriveBy", [](const int& _self, const bool& _state) { auto result = Command<0x0501>(_self, _state); return std::make_tuple(result);});
	table.set_function("setDrunkenness", [](const int& _self, const int& _intensity) { auto result = Command<0x052C>(_self, _intensity); return std::make_tuple(result);});
	table.set_function("makeFireProof", [](const int& _self, const bool& _state) { auto result = Command<0x055D>(_self, _state); return std::make_tuple(result);});
	table.set_function("increaseMaxHealth", [](const int& _self, const int& _value) { auto result = Command<0x055E>(_self, _value); return std::make_tuple(result);});
	table.set_function("increaseMaxArmor", [](const int& _self, const int& _value) { auto result = Command<0x055F>(_self, _value); return std::make_tuple(result);});
	table.set_function("ensureHasDriveByWeapon", [](const int& _self, const int& _ammo) { auto result = Command<0x0563>(_self, _ammo); return std::make_tuple(result);});
	table.set_function("isInInfoZone", [](const int& _self, const char* _infoZone) { auto result = Command<0x0583>(_self, _infoZone); return std::make_tuple(result);});
	table.set_function("isTargettingAnything", [](const int& _self) { auto result = Command<0x068C>(_self); return std::make_tuple(result);});
	table.set_function("disableSprint", [](const int& _self, const bool& _state) { auto result = Command<0x06AF>(_self, _state); return std::make_tuple(result);});
	table.set_function("delete", [](const int& _self) { auto result = Command<0x06DF>(_self); return std::make_tuple(result);});
	table.set_function("buildModel", [](const int& _self) { auto result = Command<0x070D>(_self); return std::make_tuple(result);});
	table.set_function("giveClothes", [](const int& _self, const int& _textureHash, const int& _modelHash, const int& _bodyPart) { auto result = Command<0x0784>(_self, _textureHash, _modelHash, _bodyPart); return std::make_tuple(result);});
	table.set_function("storeClothesState", []() { auto result = Command<0x0793>(); return std::make_tuple(result);});
	table.set_function("restoreClothesState", []() { auto result = Command<0x0794>(); return std::make_tuple(result);});
	table.set_function("getGroup", [](const int& _self) {int handle_; auto result = Command<0x07AF>(_self, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setGroupRecruitment", [](const int& _self, const bool& _state) { auto result = Command<0x07B4>(_self, _state); return std::make_tuple(result);});
	table.set_function("isPerformingWheelie", [](const int& _self) { auto result = Command<0x07F1>(_self); return std::make_tuple(result);});
	table.set_function("isPerformingStoppie", [](const int& _self) { auto result = Command<0x07F2>(_self); return std::make_tuple(result);});
	table.set_function("getTotalNumberOfPedsKilled", [](const int& _self) {int numPeds_; auto result = Command<0x0806>(_self, &numPeds_); return std::make_tuple(numPeds_, result);});
	table.set_function("getCityIsIn", [](const int& _self) {int cityId_; auto result = Command<0x0842>(_self, &cityId_); return std::make_tuple(cityId_, result);});
	table.set_function("setHeadingForAttached", [](const int& _self, const float& _heading, const float& _rotationSpeed) { auto result = Command<0x0858>(_self, _heading, _rotationSpeed); return std::make_tuple(result);});
	table.set_function("isAttachedHeadingAchieved", [](const int& _self) { auto result = Command<0x0861>(_self); return std::make_tuple(result);});
	table.set_function("giveClothesOutsideShop", [](const int& _self, const char* _textureName, const char* _modelName, const int& _bodyPart) { auto result = Command<0x087B>(_self, _textureName, _modelName, _bodyPart); return std::make_tuple(result);});
	table.set_function("makeGangDisappear", []() { auto result = Command<0x08F5>(); return std::make_tuple(result);});
	table.set_function("makeGangReappear", []() { auto result = Command<0x08F6>(); return std::make_tuple(result);});
	table.set_function("getClothesItem", [](const int& _self, const int& _bodyPart) {int textureHash_;int modelHash_; auto result = Command<0x08F7>(_self, _bodyPart, &textureHash_, &modelHash_); return std::make_tuple(textureHash_, modelHash_, result);});
	table.set_function("getMaxArmor", [](const int& _self) {int maxArmour_; auto result = Command<0x0945>(_self, &maxArmour_); return std::make_tuple(maxArmour_, result);});
	table.set_function("setModel", [](const int& _self, const int& _modelId) { auto result = Command<0x09C7>(_self, _modelId); return std::make_tuple(result);});
	table.set_function("forceInteriorLighting", [](const int& _self, const bool& _state) { auto result = Command<0x09D7>(_self, _state); return std::make_tuple(result);});
	table.set_function("useDetonator", []() { auto result = Command<0x09D9>(); return std::make_tuple(result);});
	table.set_function("isControlOn", [](const int& _self) { auto result = Command<0x09E7>(_self); return std::make_tuple(result);});
	table.set_function("takeOffGoggles", [](const int& _self, const bool& _animate) { auto result = Command<0x09EB>(_self, _animate); return std::make_tuple(result);});
	table.set_function("isUsingJetpack", [](const int& _self) { auto result = Command<0x0A0C>(_self); return std::make_tuple(result);});
	table.set_function("setGroupToFollowAlways", [](const int& _self, const bool& _state) { auto result = Command<0x0A20>(_self, _state); return std::make_tuple(result);});
	table.set_function("isClimbing", [](const int& _self) { auto result = Command<0x0A29>(_self); return std::make_tuple(result);});
	table.set_function("setGroupToFollowNever", [](const int& _self, const bool& _state) { auto result = Command<0x0A31>(_self, _state); return std::make_tuple(result);});
	table.set_function("isLastBuildingModelShot", [](const int& _self, const int& _modelId) { auto result = Command<0x0A3A>(_self, _modelId); return std::make_tuple(result);});
	table.set_function("clearLastBuildingModelShot", [](const int& _self) { auto result = Command<0x0A3B>(_self); return std::make_tuple(result);});
	table.set_function("changeMoney", [](const int& _self, const int& _mode, const int& _value) { auto result = Command<0x0E5E>(_self, _mode, _value); return std::make_tuple(result);});
	
	//CLEO
	table.set_function("getCharIsTargeting", [](const int& _self) {int handle_; auto result = Command<0x0AD2>(_self, &handle_); return std::make_tuple(handle_, result);});

}