//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void GameOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("GameOp");
    //default
	table.set_function("setMaxWantedLevel", [](const int& _wantedLevel) { auto result = Command<0x01F0>(_wantedLevel); return std::make_tuple(result);});
	table.set_function("setCollectableTotal", [](const int& _amount) { auto result = Command<0x02ED>(_amount); return std::make_tuple(result);});
	table.set_function("setFreeResprays", [](const bool& _state) { auto result = Command<0x0335>(_state); return std::make_tuple(result);});
	table.set_function("setEveryoneIgnorePlayer", [](const int& _handle, const bool& _state) { auto result = Command<0x03BF>(_handle, _state); return std::make_tuple(result);});
	table.set_function("setWantedMultiplier", [](const float& _multiplier) { auto result = Command<0x03C7>(_multiplier); return std::make_tuple(result);});
	table.set_function("activateSaveMenu", []() { auto result = Command<0x03D8>(); return std::make_tuple(result);});
	table.set_function("hasSaveGameFinished", []() { auto result = Command<0x03D9>(); return std::make_tuple(result);});
	table.set_function("setAllCarsCanBeDamaged", [](const bool& _state) { auto result = Command<0x03F4>(_state); return std::make_tuple(result);});
	table.set_function("isGerman", []() { auto result = Command<0x040C>(); return std::make_tuple(result);});
	table.set_function("areMeasurementsInMeters", []() { auto result = Command<0x0424>(); return std::make_tuple(result);});
	table.set_function("areAnyCarCheatsActivated", []() { auto result = Command<0x0445>(); return std::make_tuple(result);});
	table.set_function("isPcVersion", []() { auto result = Command<0x0485>(); return std::make_tuple(result);});
	table.set_function("getMaxWantedLevel", []() {int level_; auto result = Command<0x050F>(&level_); return std::make_tuple(level_, result);});
	table.set_function("setAllTaxisHaveNitro", [](const bool& _state) { auto result = Command<0x0572>(_state); return std::make_tuple(result);});
	table.set_function("setIsInStadium", [](const bool& _state) { auto result = Command<0x057E>(_state); return std::make_tuple(result);});
	table.set_function("isAustralian", []() { auto result = Command<0x059A>(); return std::make_tuple(result);});
	table.set_function("switchEmergencyServices", [](const bool& _state) { auto result = Command<0x06D0>(_state); return std::make_tuple(result);});
	table.set_function("limitTwoPlayerDistance", [](const float& _distance) { auto result = Command<0x06F1>(_distance); return std::make_tuple(result);});
	table.set_function("switchCopsOnBikes", [](const bool& _state) { auto result = Command<0x072C>(_state); return std::make_tuple(result);});
	table.set_function("setRelationship", [](const int& _relationshipType, const int& _ofPedType, const int& _toPedType) { auto result = Command<0x0746>(_relationshipType, _ofPedType, _toPedType); return std::make_tuple(result);});
	table.set_function("clearRelationship", [](const int& _relationshipType, const int& _ofPedType, const int& _toPedType) { auto result = Command<0x0747>(_relationshipType, _ofPedType, _toPedType); return std::make_tuple(result);});
	table.set_function("setArea51SamSite", [](const bool& _state) { auto result = Command<0x07A8>(_state); return std::make_tuple(result);});
	table.set_function("isRelationshipSet", [](const int& _ofPedType, const int& _toPedType, const int& _relationshipType) { auto result = Command<0x07E8>(_ofPedType, _toPedType, _relationshipType); return std::make_tuple(result);});
	table.set_function("is2PlayerGameGoingOn", []() { auto result = Command<0x0800>(); return std::make_tuple(result);});
	table.set_function("setMaxFireGenerations", [](const int& _limit) { auto result = Command<0x0828>(_limit); return std::make_tuple(result);});
	table.set_function("activateInteriorPeds", [](const bool& _state) { auto result = Command<0x084D>(_state); return std::make_tuple(result);});
	table.set_function("enableEntryExitPlayerGroupWarping", [](const float& _x, const float& _y, const float& _radius, const bool& _state) { auto result = Command<0x0864>(_x, _y, _radius, _state); return std::make_tuple(result);});
	table.set_function("isProceduralInteriorActive", [](const int& _interiorId) { auto result = Command<0x0867>(_interiorId); return std::make_tuple(result);});
	table.set_function("setGangWarsActive", [](const bool& _state) { auto result = Command<0x0879>(_state); return std::make_tuple(result);});
	table.set_function("isGangWarGoingOn", []() { auto result = Command<0x087A>(); return std::make_tuple(result);});
	table.set_function("canTriggerGangWarWhenOnAMission", [](const bool& _state) { auto result = Command<0x08A3>(_state); return std::make_tuple(result);});
	table.set_function("setGangWarsTrainingMission", [](const bool& _state) { auto result = Command<0x08AC>(_state); return std::make_tuple(result);});
	table.set_function("setNightVision", [](const bool& _state) { auto result = Command<0x08B1>(_state); return std::make_tuple(result);});
	table.set_function("setInfraredVision", [](const bool& _state) { auto result = Command<0x08B2>(_state); return std::make_tuple(result);});
	table.set_function("switchDeathPenalties", [](const bool& _state) { auto result = Command<0x08DD>(_state); return std::make_tuple(result);});
	table.set_function("switchArrestPenalties", [](const bool& _state) { auto result = Command<0x08DE>(_state); return std::make_tuple(result);});
	table.set_function("attachAnimsToModel", [](const int& _pedModelId, const char* _animationFile) { auto result = Command<0x08E8>(_pedModelId, _animationFile); return std::make_tuple(result);});
	table.set_function("setScriptLimitToGangSize", [](const int& _maxSize) { auto result = Command<0x08F4>(_maxSize); return std::make_tuple(result);});
	table.set_function("clearSpecificZonesToTriggerGangWar", []() { auto result = Command<0x090D>(); return std::make_tuple(result);});
	table.set_function("switchAmbientPlanes", [](const bool& _state) { auto result = Command<0x0923>(_state); return std::make_tuple(result);});
	table.set_function("findMaxNumberOfGroupMembers", []() {int maxNum_; auto result = Command<0x0956>(&maxNum_); return std::make_tuple(maxNum_, result);});
	table.set_function("switchPoliceHelis", [](const bool& _state) { auto result = Command<0x096A>(_state); return std::make_tuple(result);});
	table.set_function("forceDeathRestart", []() { auto result = Command<0x0970>(); return std::make_tuple(result);});
	table.set_function("resetStuffUponResurrection", []() { auto result = Command<0x0974>(); return std::make_tuple(result);});
	table.set_function("setGunshotSenseRangeForRiot2", [](const float& _range) { auto result = Command<0x098A>(_range); return std::make_tuple(result);});
	table.set_function("setNamedEntryExitFlag", [](const char* _name, const int& _entryexitsFlag, const bool& _state) { auto result = Command<0x098E>(_name, _entryexitsFlag, _state); return std::make_tuple(result);});
	table.set_function("isNightVisionActive", []() { auto result = Command<0x099D>(); return std::make_tuple(result);});
	table.set_function("showBlipsOnAllLevels", [](const bool& _state) { auto result = Command<0x09A6>(_state); return std::make_tuple(result);});
	table.set_function("hideAllFrontendBlips", [](const bool& _state) { auto result = Command<0x09AC>(_state); return std::make_tuple(result);});
	table.set_function("setMinigameInProgress", [](const bool& _state) { auto result = Command<0x09BD>(_state); return std::make_tuple(result);});
	table.set_function("isMinigameInProgress", []() { auto result = Command<0x09BE>(); return std::make_tuple(result);});
	table.set_function("setForceRandomCarModel", [](const int& _modelId) { auto result = Command<0x09BF>(_modelId); return std::make_tuple(result);});
	table.set_function("areSubtitlesSwitchedOn", []() { auto result = Command<0x09C8>(); return std::make_tuple(result);});
	table.set_function("clearWantedLevelInGarage", []() { auto result = Command<0x09D4>(); return std::make_tuple(result);});
	table.set_function("makeRoomInPlayerGangForMissionPeds", [](const int& __p1) { auto result = Command<0x09DD>(__p1); return std::make_tuple(result);});
	table.set_function("setAircraftCarrierSamSite", [](const bool& _state) { auto result = Command<0x09E4>(_state); return std::make_tuple(result);});
	table.set_function("shutAllCharsUp", [](const bool& _state) { auto result = Command<0x09F5>(_state); return std::make_tuple(result);});
	table.set_function("hasGameJustReturnedFromFrontend", []() { auto result = Command<0x09FA>(); return std::make_tuple(result);});
	table.set_function("getCurrentLanguage", []() {int languageSlot_; auto result = Command<0x09FB>(&languageSlot_); return std::make_tuple(languageSlot_, result);});
	table.set_function("isGangWarFightingGoingOn", []() { auto result = Command<0x0A03>(); return std::make_tuple(result);});
	table.set_function("hasLanguageChanged", []() { auto result = Command<0x0A0F>(); return std::make_tuple(result);});
	table.set_function("manageAllPopulation", []() { auto result = Command<0x0A13>(); return std::make_tuple(result);});
	table.set_function("setNoResprays", [](const bool& _state) { auto result = Command<0x0A14>(_state); return std::make_tuple(result);});
	table.set_function("takePhoto", [](const bool& __p1) { auto result = Command<0x0A1E>(__p1); return std::make_tuple(result);});
	table.set_function("isWidescreenOnInOptions", []() { auto result = Command<0x0A2B>(); return std::make_tuple(result);});
	table.set_function("forceAllVehicleLightsOff", [](const bool& _state) { auto result = Command<0x0A37>(_state); return std::make_tuple(result);});
	table.set_function("activatePimpCheat", [](const bool& _state) { auto result = Command<0x0A3D>(_state); return std::make_tuple(result);});
	table.set_function("setScriptCoopGame", [](const bool& _state) { auto result = Command<0x0A3F>(_state); return std::make_tuple(result);});
	table.set_function("getRidOfPlayerProstitute", []() { auto result = Command<0x0A43>(); return std::make_tuple(result);});
	table.set_function("switchObjectBrains", [](const int& _type, const bool& _state) { auto result = Command<0x0A46>(_type, _state); return std::make_tuple(result);});
	table.set_function("allowPauseInWidescreen", [](const bool& _state) { auto result = Command<0x0A48>(_state); return std::make_tuple(result);});
	table.set_function("isPcUsingJoypad", []() { auto result = Command<0x0A4B>(); return std::make_tuple(result);});
	
	//CLEO
	table.set_function("isVersionOriginal", []() { auto result = Command<0x0AA9>(); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("getCurrentResolution", []() {int width_;int height_; auto result = Command<0x0E0E>(&width_, &height_); return std::make_tuple(width_, height_, result);});
	table.set_function("isSamp", []() { auto result = Command<0x0E20>(); return std::make_tuple(result);});
	table.set_function("getCurrentSaveSlot", []() {int int_; auto result = Command<0x0E2C>(&int_); return std::make_tuple(int_, result);});
	table.set_function("isFirstStart", []() { auto result = Command<0x0E2D>(); return std::make_tuple(result);});
	table.set_function("disableSecondPlayer", [](const bool& _restoreCamera) { auto result = Command<0x0EA1>(_restoreCamera); return std::make_tuple(result);});
	table.set_function("fixTwoPlayersSeparatedCars", []() { auto result = Command<0x0EA2>(); return std::make_tuple(result);});
	
	//imgui
	table.set_function("getFramerate", []() {int fps_; auto result = Command<0x0C17>(&fps_); return std::make_tuple(fps_, result);});

}