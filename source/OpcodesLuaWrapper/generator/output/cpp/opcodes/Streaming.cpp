// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindStreaming(sol::state &state) {
  auto table = state.create_table("StreamingOp");
  // default
  table.set_function("loadSpecialCharacter",
                     [](const int &_slotId, const char *_modelName) { Command<0x023C>(_slotId, _modelName); });
  table.set_function("hasSpecialCharacterLoaded", [](const int &_slotId) {
    auto result = Command<0x023D>(_slotId);
    return std::make_tuple(result);
  });
  table.set_function("requestModel", [](const int &_modelId) { Command<0x0247>(_modelId); });
  table.set_function("hasModelLoaded", [](const int &_modelId) {
    auto result = Command<0x0248>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("markModelAsNoLongerNeeded", [](const int &_modelId) { Command<0x0249>(_modelId); });
  table.set_function("unloadSpecialCharacter", [](const int &_slotId) { Command<0x0296>(_slotId); });
  table.set_function("loadAllModelsNow", []() { Command<0x038B>(); });
  table.set_function("switch", [](const bool &_state) { Command<0x03AF>(_state); });
  table.set_function("loadScene",
                     [](const float &_x, const float &_y, const float &_z) { Command<0x03CB>(_x, _y, _z); });
  table.set_function("isModelAvailable", [](const int &_modelId) {
    auto result = Command<0x0488>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("setAreaVisible", [](const int &_areaId) { Command<0x04BB>(_areaId); });
  table.set_function("requestCollision", [](const float &_x, const float &_y) { Command<0x04E4>(_x, _y); });
  table.set_function("requestAnimation", [](const char *_animationFile) { Command<0x04ED>(_animationFile); });
  table.set_function("hasAnimationLoaded", [](const char *_animationFile) {
    auto result = Command<0x04EE>(_animationFile);
    return std::make_tuple(result);
  });
  table.set_function("removeAnimation", [](const char *_animationFile) { Command<0x04EF>(_animationFile); });
  table.set_function("markMissionTrainsAsNoLongerNeeded", []() { Command<0x06DA>(); });
  table.set_function("getVehicleModType", [](const int &_modelId) {
    int slotId_;
    Command<0x06E6>(_modelId, &slotId_);
    return std::make_tuple(slotId_);
  });
  table.set_function("requestVehicleMod", [](const int &_modelId) { Command<0x06E9>(_modelId); });
  table.set_function("hasVehicleModLoaded", [](const int &_modelId) {
    auto result = Command<0x06EA>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("markVehicleModAsNoLongerNeeded", [](const int &_modelId) { Command<0x06EB>(_modelId); });
  table.set_function("customPlateDesignForNextCar",
                     [](const int &_modelId, const int &_townId) { Command<0x0771>(_modelId, _townId); });
  table.set_function("requestIpl", [](const char *_iplName) { Command<0x0776>(_iplName); });
  table.set_function("removeIpl", [](const char *_iplName) { Command<0x0777>(_iplName); });
  table.set_function("removeIplDiscreetly", [](const char *_iplName) { Command<0x0778>(_iplName); });
  table.set_function("getAreaVisible", []() {
    int interiorId_;
    Command<0x077E>(&interiorId_);
    return std::make_tuple(interiorId_);
  });
  table.set_function("requestCarRecording", [](const int &_pathId) { Command<0x07C0>(_pathId); });
  table.set_function("hasCarRecordingBeenLoaded", [](const int &_pathId) {
    auto result = Command<0x07C1>(_pathId);
    return std::make_tuple(result);
  });
  table.set_function("isModelInCdimage", [](const int &_modeId) {
    auto result = Command<0x07DE>(_modeId);
    return std::make_tuple(result);
  });
  table.set_function("getModelDimensions", [](const int &_modelId) {
    float leftBottomBackX_;
    float leftBottomBackY_;
    float leftBottomBackZ_;
    float rightTopFrontX_;
    float rightTopFrontY_;
    float rightTopFrontZ_;
    Command<0x07E4>(_modelId, &leftBottomBackX_, &leftBottomBackY_, &leftBottomBackZ_, &rightTopFrontX_,
                    &rightTopFrontY_, &rightTopFrontZ_);
    return std::make_tuple(leftBottomBackX_, leftBottomBackY_, leftBottomBackZ_, rightTopFrontX_, rightTopFrontY_,
                           rightTopFrontZ_);
  });
  table.set_function("isThisModelABoat", [](const int &_modelId) {
    auto result = Command<0x081E>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("isThisModelAPlane", [](const int &_modelId) {
    auto result = Command<0x081F>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("isThisModelAHeli", [](const int &_modelId) {
    auto result = Command<0x0820>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("removeCarRecording", [](const int &_pathId) { Command<0x0873>(_pathId); });
  table.set_function("attachAnimsToModel", [](const int &_pedModelId, const char *_animationFile) {
    Command<0x08E8>(_pedModelId, _animationFile);
  });
  table.set_function("getRandomCarModelInMemory", [](const bool &__p1) {
    int modelId_;
    int class_;
    Command<0x09B2>(__p1, &modelId_, &class_);
    return std::make_tuple(modelId_, class_);
  });
  table.set_function("isThisModelACar", [](const int &_modelId) {
    auto result = Command<0x0A01>(_modelId);
    return std::make_tuple(result);
  });
  table.set_function("loadSceneInDirection", [](const float &_x, const float &_y, const float &_z,
                                                const float &_heading) { Command<0x0A0B>(_x, _y, _z, _heading); });

  // CLEO
  table.set_function("getNameOfVehicleModel", [](const int &_modelId) {
    const char *carName_;
    Command<0x0ADB>(_modelId, (int *)&carName_);
    return std::make_tuple(carName_);
  });

  // CLEO+
  table.set_function("getModelType", [](const int &_model) {
    int type_;
    Command<0x0E7F>(_model, &type_);
    return std::make_tuple(type_);
  });
  table.set_function("requestPriorityModel", [](const int &_modelId) { Command<0x0E98>(_modelId); });
  table.set_function("loadAllPriorityModelsNow", []() { Command<0x0E99>(); });
  table.set_function("getModelByName", [](const char *_name) {
    int modelId_;
    Command<0x0E9C>(_name, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("isModelAvailableByName", [](const char *_name) {
    auto result = Command<0x0E9D>(_name);
    return std::make_tuple(result);
  });
  table.set_function("removeModelIfUnused", [](const int &_modelId) { Command<0x0EA3>(_modelId); });
  table.set_function("getModelInfo", [](const int &_modelId) {
    int modelInfo_;
    auto result = Command<0x0EF8>(_modelId, &modelInfo_);
    return std::make_tuple(result, modelInfo_);
  });
  table.set_function("loadSpecialModel", [](const char *_dff, const char *_txd) {
    int specialModel_;
    auto result = Command<0x0F00>(_dff, _txd, &specialModel_);
    return std::make_tuple(result, specialModel_);
  });
  table.set_function("removeSpecialModel", [](const int &_specialModel) { Command<0x0F01>(_specialModel); });
  table.set_function("getSpecialModelData", [](const int &_specialModel) {
    int clump_;
    int atomic_;
    int txdIndex_;
    Command<0x0F05>(_specialModel, &clump_, &atomic_, &txdIndex_);
    return std::make_tuple(clump_, atomic_, txdIndex_);
  });
}