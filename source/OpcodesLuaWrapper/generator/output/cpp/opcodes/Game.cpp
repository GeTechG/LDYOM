// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindGame(sol::state &state) {
  auto table = state.create_table("GameOp");
  // default
  table.set_function("setMaxWantedLevel", [](const int &_wantedLevel) { Command<0x01F0>(_wantedLevel); });
  table.set_function("setPoliceIgnorePlayer",
                     [](const int &_player, const int &_state) { Command<0x01F7>(_player, _state); });
  table.set_function("setCollectableTotal", [](const int &_amount) { Command<0x02ED>(_amount); });
  table.set_function("setFreeResprays", [](const int &_state) { Command<0x0335>(_state); });
  table.set_function("setEveryoneIgnorePlayer",
                     [](const int &_player, const int &_state) { Command<0x03BF>(_player, _state); });
  table.set_function("setWantedMultiplier", [](const float &_multiplier) { Command<0x03C7>(_multiplier); });
  table.set_function("activateSaveMenu", []() { Command<0x03D8>(); });
  table.set_function("hasSaveGameFinished", []() {
    auto result = Command<0x03D9>();
    return std::make_tuple(result);
  });
  table.set_function("setAllCarsCanBeDamaged", [](const int &_state) { Command<0x03F4>(_state); });
  table.set_function("isGerman", []() {
    auto result = Command<0x040C>();
    return std::make_tuple(result);
  });
  table.set_function("areMeasurementsInMeters", []() {
    auto result = Command<0x0424>();
    return std::make_tuple(result);
  });
  table.set_function("areAnyCarCheatsActivated", []() {
    auto result = Command<0x0445>();
    return std::make_tuple(result);
  });
  table.set_function("isPcVersion", []() {
    auto result = Command<0x0485>();
    return std::make_tuple(result);
  });
  table.set_function("getMaxWantedLevel", []() {
    int level_;
    Command<0x050F>(&level_);
    return std::make_tuple(level_);
  });
  table.set_function("setAllTaxisHaveNitro", [](const int &_state) { Command<0x0572>(_state); });
  table.set_function("setIsInStadium", [](const int &_state) { Command<0x057E>(_state); });
  table.set_function("isAustralian", []() {
    auto result = Command<0x059A>();
    return std::make_tuple(result);
  });
  table.set_function("setLaRiots", [](const int &_state) { Command<0x06C8>(_state); });
  table.set_function("switchEmergencyServices", [](const int &_state) { Command<0x06D0>(_state); });
  table.set_function("switchRandomTrains", [](const int &_state) { Command<0x06D7>(_state); });
  table.set_function("limitTwoPlayerDistance", [](const float &_distance) { Command<0x06F1>(_distance); });
  table.set_function("releaseTwoPlayerDistance", []() { Command<0x06F2>(); });
  table.set_function("setPlayerPlayerTargeting", [](const int &_state) { Command<0x06F3>(_state); });
  table.set_function("setPlayersCanBeInSeparateCars", [](const int &_state) { Command<0x06FA>(_state); });
  table.set_function("switchCopsOnBikes", [](const int &_state) { Command<0x072C>(_state); });
  table.set_function("setRelationship", [](const int &_relationshipType, const int &_ofPedType, const int &_toPedType) {
    Command<0x0746>(_relationshipType, _ofPedType, _toPedType);
  });
  table.set_function("clearRelationship",
                     [](const int &_relationshipType, const int &_ofPedType, const int &_toPedType) {
                       Command<0x0747>(_relationshipType, _ofPedType, _toPedType);
                     });
  table.set_function("setArea51SamSite", [](const int &_state) { Command<0x07A8>(_state); });
  table.set_function("isRelationshipSet",
                     [](const int &_relationshipType, const int &_ofPedType, const int &_toPedType) {
                       auto result = Command<0x07E8>(_relationshipType, _ofPedType, _toPedType);
                       return std::make_tuple(result);
                     });
  table.set_function("is2PlayerGameGoingOn", []() {
    auto result = Command<0x0800>();
    return std::make_tuple(result);
  });
  table.set_function("setMaxFireGenerations", [](const int &_limit) { Command<0x0828>(_limit); });
  table.set_function("activateInteriorPeds", [](const int &_state) { Command<0x084D>(_state); });
  table.set_function("enableEntryExitPlayerGroupWarping",
                     [](const float &_x, const float &_y, const float &_radius, const int &_state) {
                       Command<0x0864>(_x, _y, _radius, _state);
                     });
  table.set_function("isProceduralInteriorActive", [](const int &_interiorId) {
    auto result = Command<0x0867>(_interiorId);
    return std::make_tuple(result);
  });
  table.set_function("setGangWarsActive", [](const int &_state) { Command<0x0879>(_state); });
  table.set_function("isGangWarGoingOn", []() {
    auto result = Command<0x087A>();
    return std::make_tuple(result);
  });
  table.set_function("canTriggerGangWarWhenOnAMission", [](const int &_state) { Command<0x08A3>(_state); });
  table.set_function("setAlwaysDraw3DMarkers", [](const int &_state) { Command<0x08A8>(_state); });
  table.set_function("setGangWarsTrainingMission", [](const int &_state) { Command<0x08AC>(_state); });
  table.set_function("setNightVision", [](const int &_state) { Command<0x08B1>(_state); });
  table.set_function("setInfraredVision", [](const int &_state) { Command<0x08B2>(_state); });
  table.set_function("switchDeathPenalties", [](const int &_state) { Command<0x08DD>(_state); });
  table.set_function("switchArrestPenalties", [](const int &_state) { Command<0x08DE>(_state); });
  table.set_function("setCreateRandomGangMembers", [](const int &_state) { Command<0x08EA>(_state); });
  table.set_function("setScriptLimitToGangSize", [](const int &_maxSize) { Command<0x08F4>(_maxSize); });
  table.set_function("clearSpecificZonesToTriggerGangWar", []() { Command<0x090D>(); });
  table.set_function("switchAmbientPlanes", [](const int &_state) { Command<0x0923>(_state); });
  table.set_function("findMaxNumberOfGroupMembers", []() {
    int maxNum_;
    Command<0x0956>(&maxNum_);
    return std::make_tuple(maxNum_);
  });
  table.set_function("switchPoliceHelis", [](const int &_state) { Command<0x096A>(_state); });
  table.set_function("forceDeathRestart", []() { Command<0x0970>(); });
  table.set_function("resetStuffUponResurrection", []() { Command<0x0974>(); });
  table.set_function("setOnlyCreateGangMembers", [](const int &_state) { Command<0x0983>(_state); });
  table.set_function("setGunshotSenseRangeForRiot2", [](const float &_range) { Command<0x098A>(_range); });
  table.set_function("setNamedEntryExitFlag", [](const char *_name, const int &_entryexitsFlag, const int &_state) {
    Command<0x098E>(_name, _entryexitsFlag, _state);
  });
  table.set_function("isNightVisionActive", []() {
    auto result = Command<0x099D>();
    return std::make_tuple(result);
  });
  table.set_function("setCreateRandomCops", [](const int &_state) { Command<0x099E>(_state); });
  table.set_function("showBlipsOnAllLevels", [](const int &_state) { Command<0x09A6>(_state); });
  table.set_function("hideAllFrontendBlips", [](const int &_state) { Command<0x09AC>(_state); });
  table.set_function("setMinigameInProgress", [](const int &_state) { Command<0x09BD>(_state); });
  table.set_function("isMinigameInProgress", []() {
    auto result = Command<0x09BE>();
    return std::make_tuple(result);
  });
  table.set_function("setForceRandomCarModel", [](const int &_modelId) { Command<0x09BF>(_modelId); });
  table.set_function("areSubtitlesSwitchedOn", []() {
    auto result = Command<0x09C8>();
    return std::make_tuple(result);
  });
  table.set_function("enableAmbientCrime", [](const int &_state) { Command<0x09D2>(_state); });
  table.set_function("clearWantedLevelInGarage", []() { Command<0x09D4>(); });
  table.set_function("makeRoomInPlayerGangForMissionPeds", [](const int &__p1) { Command<0x09DD>(__p1); });
  table.set_function("setAircraftCarrierSamSite", [](const int &_state) { Command<0x09E4>(_state); });
  table.set_function("enableBurglaryHouses", [](const int &_state) { Command<0x09E6>(_state); });
  table.set_function("shutAllCharsUp", [](const int &_state) { Command<0x09F5>(_state); });
  table.set_function("doWeaponStuffAtStartOf2PGame", []() { Command<0x09F8>(); });
  table.set_function("hasGameJustReturnedFromFrontend", []() {
    auto result = Command<0x09FA>();
    return std::make_tuple(result);
  });
  table.set_function("getCurrentLanguage", []() {
    int languageSlot_;
    Command<0x09FB>(&languageSlot_);
    return std::make_tuple(languageSlot_);
  });
  table.set_function("isGangWarFightingGoingOn", []() {
    auto result = Command<0x0A03>();
    return std::make_tuple(result);
  });
  table.set_function("hasLanguageChanged", []() {
    auto result = Command<0x0A0F>();
    return std::make_tuple(result);
  });
  table.set_function("manageAllPopulation", []() { Command<0x0A13>(); });
  table.set_function("setNoResprays", [](const int &_state) { Command<0x0A14>(_state); });
  table.set_function("isWidescreenOnInOptions", []() {
    auto result = Command<0x0A2B>();
    return std::make_tuple(result);
  });
  table.set_function("forceAllVehicleLightsOff", [](const int &_state) { Command<0x0A37>(_state); });
  table.set_function("activatePimpCheat", [](const int &_state) { Command<0x0A3D>(_state); });
  table.set_function("setScriptCoopGame", [](const int &_state) { Command<0x0A3F>(_state); });
  table.set_function("getRidOfPlayerProstitute", []() { Command<0x0A43>(); });
  table.set_function("switchObjectBrains", [](const int &_type, const int &_state) { Command<0x0A46>(_type, _state); });
  table.set_function("allowPauseInWidescreen", [](const int &_state) { Command<0x0A48>(_state); });
  table.set_function("isPcUsingJoypad", []() {
    auto result = Command<0x0A4B>();
    return std::make_tuple(result);
  });

  // CLEO
  table.set_function("isVersionOriginal", []() {
    auto result = Command<0x0AA9>();
    return std::make_tuple(result);
  });
  table.set_function("getPlatform", []() {
    int platform_;
    Command<0x0DD5>(&platform_);
    return std::make_tuple(platform_);
  });

  // CLEO+
  table.set_function("getCurrentResolution", []() {
    int width_;
    int height_;
    Command<0x0E0E>(&width_, &height_);
    return std::make_tuple(width_, height_);
  });
  table.set_function("isSamp", []() {
    auto result = Command<0x0E20>();
    return std::make_tuple(result);
  });
  table.set_function("getCurrentSaveSlot", []() {
    int int_;
    Command<0x0E2C>(&int_);
    return std::make_tuple(int_);
  });
  table.set_function("isFirstStart", []() {
    auto result = Command<0x0E2D>();
    return std::make_tuple(result);
  });
  table.set_function("frameMod", [](const int &_mod) {
    auto result = Command<0x0E45>(_mod);
    return std::make_tuple(result);
  });
  table.set_function("isCheatActive", [](const int &_cheat) {
    auto result = Command<0x0E5D>(_cheat);
    return std::make_tuple(result);
  });
  table.set_function("isSelectMenuJustPressed", []() {
    auto result = Command<0x0E6E>();
    return std::make_tuple(result);
  });
  table.set_function("disableSecondPlayer", [](const int &_restoreCamera) { Command<0x0EA1>(_restoreCamera); });
  table.set_function("fixTwoPlayersSeparatedCars", []() { Command<0x0EA2>(); });
  table.set_function("setOnMission", [](const int &_status) { Command<0x0F16>(_status); });

  // imgui
  table.set_function("getFramerate", []() {
    int fps_;
    Command<0x2244>(&fps_);
    return std::make_tuple(fps_);
  });
}