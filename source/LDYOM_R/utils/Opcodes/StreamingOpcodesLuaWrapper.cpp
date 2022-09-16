//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void StreamingOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("StreamingOp");
    //default
	table.set_function("loadSpecialCharacter", [](const int& _slotId, const char* _modelName) { auto result = Command<0x023C>(_slotId, _modelName); return std::make_tuple(result);});
	table.set_function("hasSpecialCharacterLoaded", [](const int& _slotId) { auto result = Command<0x023D>(_slotId); return std::make_tuple(result);});
	table.set_function("requestModel", [](const int& _modelId) { auto result = Command<0x0247>(_modelId); return std::make_tuple(result);});
	table.set_function("hasModelLoaded", [](const int& _modelId) { auto result = Command<0x0248>(_modelId); return std::make_tuple(result);});
	table.set_function("markModelAsNoLongerNeeded", [](const int& _modelId) { auto result = Command<0x0249>(_modelId); return std::make_tuple(result);});
	table.set_function("unloadSpecialCharacter", [](const int& _slotId) { auto result = Command<0x0296>(_slotId); return std::make_tuple(result);});
	table.set_function("loadAllModelsNow", []() { auto result = Command<0x038B>(); return std::make_tuple(result);});
	table.set_function("switch", [](const bool& _state) { auto result = Command<0x03AF>(_state); return std::make_tuple(result);});
	table.set_function("loadScene", [](const float& _x, const float& _y, const float& _z) { auto result = Command<0x03CB>(_x, _y, _z); return std::make_tuple(result);});
	table.set_function("isModelAvailable", [](const int& _modelId) { auto result = Command<0x0488>(_modelId); return std::make_tuple(result);});
	table.set_function("setAreaVisible", [](const int& _areaId) { auto result = Command<0x04BB>(_areaId); return std::make_tuple(result);});
	table.set_function("requestCollision", [](const float& _x, const float& _y) { auto result = Command<0x04E4>(_x, _y); return std::make_tuple(result);});
	table.set_function("requestAnimation", [](const char* _animationFile) { auto result = Command<0x04ED>(_animationFile); return std::make_tuple(result);});
	table.set_function("hasAnimationLoaded", [](const char* _animationFile) { auto result = Command<0x04EE>(_animationFile); return std::make_tuple(result);});
	table.set_function("removeAnimation", [](const char* _animationFile) { auto result = Command<0x04EF>(_animationFile); return std::make_tuple(result);});
	table.set_function("getVehicleModType", [](const int& _modelId) {int slotId_; auto result = Command<0x06E6>(_modelId, &slotId_); return std::make_tuple(slotId_, result);});
	table.set_function("requestVehicleMod", [](const int& _modelId) { auto result = Command<0x06E9>(_modelId); return std::make_tuple(result);});
	table.set_function("hasVehicleModLoaded", [](const int& _modelId) { auto result = Command<0x06EA>(_modelId); return std::make_tuple(result);});
	table.set_function("markVehicleModAsNoLongerNeeded", [](const int& _modelId) { auto result = Command<0x06EB>(_modelId); return std::make_tuple(result);});
	table.set_function("requestIpl", [](const char* _iplName) { auto result = Command<0x0776>(_iplName); return std::make_tuple(result);});
	table.set_function("removeIpl", [](const char* _iplName) { auto result = Command<0x0777>(_iplName); return std::make_tuple(result);});
	table.set_function("removeIplDiscreetly", [](const char* _iplName) { auto result = Command<0x0778>(_iplName); return std::make_tuple(result);});
	table.set_function("getAreaVisible", []() {int interiorId_; auto result = Command<0x077E>(&interiorId_); return std::make_tuple(interiorId_, result);});
	table.set_function("requestCarRecording", [](const int& _pathId) { auto result = Command<0x07C0>(_pathId); return std::make_tuple(result);});
	table.set_function("hasCarRecordingBeenLoaded", [](const int& _pathId) { auto result = Command<0x07C1>(_pathId); return std::make_tuple(result);});
	table.set_function("isModelInCdimage", [](const int& _modeId) { auto result = Command<0x07DE>(_modeId); return std::make_tuple(result);});
	table.set_function("getModelDimensions", [](const int& _modelId) {float leftBottomBackX_;float leftBottomBackY_;float leftBottomBackZ_;float rightTopFrontX_;float rightTopFrontY_;float rightTopFrontZ_; auto result = Command<0x07E4>(_modelId, &leftBottomBackX_, &leftBottomBackY_, &leftBottomBackZ_, &rightTopFrontX_, &rightTopFrontY_, &rightTopFrontZ_); return std::make_tuple(leftBottomBackX_, leftBottomBackY_, leftBottomBackZ_, rightTopFrontX_, rightTopFrontY_, rightTopFrontZ_, result);});
	table.set_function("isThisModelABoat", [](const int& _modelId) { auto result = Command<0x081E>(_modelId); return std::make_tuple(result);});
	table.set_function("isThisModelAPlane", [](const int& _modelId) { auto result = Command<0x081F>(_modelId); return std::make_tuple(result);});
	table.set_function("isThisModelAHeli", [](const int& _modelId) { auto result = Command<0x0820>(_modelId); return std::make_tuple(result);});
	table.set_function("removeCarRecording", [](const int& _pathId) { auto result = Command<0x0873>(_pathId); return std::make_tuple(result);});
	table.set_function("getRandomCarModelInMemory", [](const bool& __p1) {int modelId_;int class_; auto result = Command<0x09B2>(__p1, &modelId_, &class_); return std::make_tuple(modelId_, class_, result);});
	table.set_function("isThisModelACar", [](const int& _modelId) { auto result = Command<0x0A01>(_modelId); return std::make_tuple(result);});
	table.set_function("loadSceneInDirection", [](const float& _x, const float& _y, const float& _z, const float& _heading) { auto result = Command<0x0A0B>(_x, _y, _z, _heading); return std::make_tuple(result);});
	
	//CLEO
	table.set_function("getNameOfVehicleModel", [](const int& _modelId) {const char* carName_; auto result = Command<0x0ADB>(_modelId, (int*)&carName_); return std::make_tuple(carName_, result);});
	
	//CLEO+
	table.set_function("getModelByName", [](const char* _name) {int modelId_; auto result = Command<0x0E9C>(_name, &modelId_); return std::make_tuple(modelId_, result);});

}