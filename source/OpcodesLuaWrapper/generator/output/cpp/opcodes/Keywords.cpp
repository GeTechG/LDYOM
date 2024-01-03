// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindKeywords(sol::state &state) {
  auto table = state.create_table("KeywordsOp");
  // default
  table.set_function("nop", []() { Command<0x0000>(); });
  table.set_function("wait", [](const int &_time) { Command<0x0001>(_time); });
  table.set_function("goto", [](const int &_label) { Command<0x0002>(_label); });
  table.set_function("gotoIfFalse", [](const int &_label) { Command<0x004D>(_label); });
  table.set_function("terminateThisScript", []() { Command<0x004E>(); });
  table.set_function("startNewScript",
                     [](const int &_label, const int &_arguments) { Command<0x004F>(_label, _arguments); });
  table.set_function("gosub", [](const int &_label) { Command<0x0050>(_label); });
  table.set_function("return", []() { Command<0x0051>(); });
  table.set_function("if", [](const int &_int) { Command<0x00D6>(_int); });
  table.set_function("launchMission", [](const int &_label) { Command<0x00D7>(_label); });
  table.set_function("setDeatharrestState", [](const int &_state) { Command<0x0111>(_state); });
  table.set_function("hasDeatharrestBeenExecuted", []() {
    auto result = Command<0x0112>();
    return std::make_tuple(result);
  });
  table.set_function("declareMissionFlag", [](const int &_flag) { Command<0x0180>(_flag); });
  table.set_function("scriptName", [](const char *_name) { Command<0x03A4>(_name); });
  table.set_function("terminateAllScriptsWithThisName", [](const char *_name) { Command<0x0459>(_name); });
  table.set_function("skipCutsceneEnd", []() { Command<0x0701>(); });
  table.set_function("skipCutsceneStartInternal", [](const int &_label) { Command<0x0707>(_label); });

  // CLEO
  table.set_function("streamCustomScript", [](const char *_scriptFileName, const int &_arguments) {
    Command<0x0A92>(_scriptFileName, _arguments);
  });
  table.set_function("terminateThisCustomScript", []() { Command<0x0A93>(); });
  table.set_function("loadAndLaunchCustomMission", [](const char *_scriptFileName, const int &_arguments) {
    Command<0x0A94>(_scriptFileName, _arguments);
  });
  table.set_function("saveThisCustomScript", []() { Command<0x0A95>(); });
  table.set_function("gosubIfFalse", [](const int &_label) { Command<0x0AA0>(_label); });
  table.set_function("returnIfFalse", []() { Command<0x0AA1>(); });
  table.set_function("cleoCall", [](const int &_label, const int &_numParams, const int &_params) {
    Command<0x0AB1>(_label, _numParams, _params);
  });
  table.set_function("cleoReturn",
                     [](const int &_numRet, const int &_retParams) { Command<0x0AB2>(_numRet, _retParams); });
  table.set_function("setCleoSharedVar", [](const int &_index, const int &_value) { Command<0x0AB3>(_index, _value); });
  table.set_function("getCleoSharedVar", [](const int &_index) {
    int result_;
    Command<0x0AB4>(_index, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("terminateAllCustomScriptsWithThisName", [](const char *_name) { Command<0x0ABA>(_name); });
  table.set_function("cleoReturnWith", [](const int &_conditionResult, const int &_retArgs) {
    auto result = Command<0x2002>(_conditionResult, _retArgs);
    return std::make_tuple(result);
  });
  table.set_function("cleoReturnFail", []() {
    auto result = Command<0x2003>();
    return std::make_tuple(result);
  });

  // CLEO+
  table.set_function("rotateMatrixOnAxis",
                     [](const int &_matrix, const float &_x, const float &_y, const float &_z, const float &_angle,
                        const int &_rwCombine) { Command<0x0D01>(_matrix, _x, _y, _z, _angle, _rwCombine); });
  table.set_function("getMatrixXAngle", [](const int &_matrix) {
    float angle_;
    Command<0x0D02>(_matrix, &angle_);
    return std::make_tuple(angle_);
  });
  table.set_function("getMatrixYAngle", [](const int &_matrix) {
    float angle_;
    Command<0x0D03>(_matrix, &angle_);
    return std::make_tuple(angle_);
  });
  table.set_function("getMatrixZAngle", [](const int &_matrix) {
    float angle_;
    Command<0x0D04>(_matrix, &angle_);
    return std::make_tuple(angle_);
  });
  table.set_function("getOffsetFromMatrixInWorldCoords",
                     [](const int &_matrix, const float &_x, const float &_y, const float &_z) {
                       float x_;
                       float y_;
                       float z_;
                       Command<0x0D0A>(_matrix, _x, _y, _z, &x_, &y_, &z_);
                       return std::make_tuple(x_, y_, z_);
                     });
  table.set_function("setMatrixRotationFromQuat",
                     [](const int &_matrix, const int &_quat) { Command<0x0D16>(_matrix, _quat); });
  table.set_function("setQuatFromMatrix",
                     [](const int &_quat, const int &_matrix) { Command<0x0D17>(_quat, _matrix); });
  table.set_function("rotateQuatOnAxis",
                     [](const int &_quat, const float &_x, const float &_y, const float &_z, const float &_angle,
                        const int &_rwCombine) { Command<0x0D18>(_quat, _x, _y, _z, _angle, _rwCombine); });
  table.set_function("getNormalisedQuat", [](const int &_quat) {
    int quat_;
    Command<0x0D19>(_quat, &quat_);
    return std::make_tuple(quat_);
  });
  table.set_function("multiplyQuats", [](const int &_quat1, const int &_quat2) {
    int quatResult_;
    Command<0x0D1A>(_quat1, _quat2, &quatResult_);
    return std::make_tuple(quatResult_);
  });
  table.set_function("quatSlerp", [](const int &_from, const int &_to, const float &_lambda) {
    int result_;
    Command<0x0D1E>(_from, _to, _lambda, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("initialiseQuat", [](const int &_quat, const float &_x, const float &_y, const float &_z,
                                          const float &_real) { Command<0x0D24>(_quat, _x, _y, _z, _real); });
  table.set_function("getQuatElements", [](const int &_quat) {
    float x_;
    float y_;
    float z_;
    float real_;
    Command<0x0D29>(_quat, &x_, &y_, &z_, &real_);
    return std::make_tuple(x_, y_, z_, real_);
  });
  table.set_function("setScriptVar", [](const int &_scriptPointer, const int &_varIndex, const int &_value) {
    Command<0x0D2E>(_scriptPointer, _varIndex, _value);
  });
  table.set_function("getScriptVar", [](const int &_scriptPointer, const int &_varIndex) {
    int result_;
    Command<0x0D2F>(_scriptPointer, _varIndex, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("streamCustomScriptFromLabel", [](const int &_label) { Command<0x0E6F>(_label); });
  table.set_function("loadSpecialCharacterForId",
                     [](const int &_id, const char *_name) { Command<0x0E9A>(_id, _name); });
  table.set_function("unloadSpecialCharacterFromId", [](const int &_id) { Command<0x0E9B>(_id); });
  table.set_function("getModelDoesntExistInRange", [](const int &_start, const int &_end) {
    int modelId_;
    Command<0x0E9E>(_start, _end, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("removeAllUnusedModels", []() { Command<0x0E9F>(); });
  table.set_function("convertDirectionToQuat", [](const int &_quat, const float &_x, const float &_y, const float &_z) {
    Command<0x0EB3>(_quat, _x, _y, _z);
  });
  table.set_function("returnScriptEvent", []() { Command<0x0ED0>(); });
  table.set_function("setScriptEventSaveConfirmation", [](const int &_add, const int &_label, const int &_varSaveSlot) {
    Command<0x0ED1>(_add, _label, _varSaveSlot);
  });
  table.set_function("setScriptEventCharDelete", [](const int &_add, const int &_label, const int &_varChar) {
    Command<0x0ED2>(_add, _label, _varChar);
  });
  table.set_function("setScriptEventCharCreate", [](const int &_add, const int &_label, const int &_varChar) {
    Command<0x0ED3>(_add, _label, _varChar);
  });
  table.set_function("setScriptEventCarDelete", [](const int &_add, const int &_label, const int &_varCar) {
    Command<0x0ED4>(_add, _label, _varCar);
  });
  table.set_function("setScriptEventCarCreate", [](const int &_add, const int &_label, const int &_varCar) {
    Command<0x0ED5>(_add, _label, _varCar);
  });
  table.set_function("setScriptEventObjectDelete", [](const int &_add, const int &_label, const int &_varObject) {
    Command<0x0ED6>(_add, _label, _varObject);
  });
  table.set_function("setScriptEventObjectCreate", [](const int &_add, const int &_label, const int &_varObject) {
    Command<0x0ED7>(_add, _label, _varObject);
  });
  table.set_function("setScriptEventOnMenu", [](const int &_add, const int &_label, const int &_varJustPaused) {
    Command<0x0ED8>(_add, _label, _varJustPaused);
  });
  table.set_function("setScriptEventCharProcess", [](const int &_add, const int &_label, const int &_varChar) {
    Command<0x0EDA>(_add, _label, _varChar);
  });
  table.set_function("setScriptEventCarProcess", [](const int &_add, const int &_label, const int &_varCar) {
    Command<0x0EDB>(_add, _label, _varCar);
  });
  table.set_function("setScriptEventObjectProcess", [](const int &_add, const int &_label, const int &_varObject) {
    Command<0x0EDC>(_add, _label, _varObject);
  });
  table.set_function("setScriptEventBuildingProcess", [](const int &_add, const int &_label, const int &_entity) {
    Command<0x0EDD>(_add, _label, _entity);
  });
  table.set_function("setScriptEventCharDamage", [](const int &_add, const int &_label, const int &_varChar) {
    Command<0x0EDE>(_add, _label, _varChar);
  });
  table.set_function("setScriptEventCarWeaponDamage", [](const int &_add, const int &_label, const int &_varCar) {
    Command<0x0EDF>(_add, _label, _varCar);
  });
  table.set_function("setScriptEventBulletImpact",
                     [](const int &_add, const int &_label, const int &_varCharOwner, const int &_varEntityVictim,
                        const int &_varWeaponType, const int &_varColPoint) {
                       Command<0x0EE0>(_add, _label, _varCharOwner, _varEntityVictim, _varWeaponType, _varColPoint);
                     });
  table.set_function("lerp", [](const float &_a, const float &_b, const float &_t) {
    float result_;
    Command<0x0EF3>(_a, _b, _t, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("returnTimes", [](const int &_numReturns) { Command<0x0F0A>(_numReturns); });
  table.set_function("setScriptEventBeforeGameProcess",
                     [](const int &_add, const int &_label) { Command<0x0F0B>(_add, _label); });
  table.set_function("setScriptEventAfterGameProcess",
                     [](const int &_add, const int &_label) { Command<0x0F0C>(_add, _label); });
  table.set_function("setMatrixLookDirection",
                     [](const int &_matrix, const float &_originX, const float &_originY, const float &_originZ,
                        const float &_dirX, const float &_dirY, const float &_dirZ) {
                       Command<0x0F0D>(_matrix, _originX, _originY, _originZ, _dirX, _dirY, _dirZ);
                     });
  table.set_function("getThirdPersonCameraTarget", [](const float &_range, const float &_sourceX, const float &_sourceY,
                                                      const float &_sourceZ) {
    float startX_;
    float startY_;
    float startZ_;
    float endX_;
    float endY_;
    float endZ_;
    Command<0x0F0E>(_range, _sourceX, _sourceY, _sourceZ, &startX_, &startY_, &startZ_, &endX_, &endY_, &endZ_);
    return std::make_tuple(startX_, startY_, startZ_, endX_, endY_, endZ_);
  });
  table.set_function("getDistanceMultiplier", []() {
    float drawing_;
    float generating_;
    Command<0x0F0F>(&drawing_, &generating_);
    return std::make_tuple(drawing_, generating_);
  });
  table.set_function("getActiveCameraRotation", []() {
    float x_;
    float y_;
    float z_;
    Command<0x0F10>(&x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("getClosestWaterDistance", []() {
    float distance_;
    float closestZ_;
    Command<0x0F11>(&distance_, &closestZ_);
    return std::make_tuple(distance_, closestZ_);
  });
  table.set_function("getModelNamePointer", [](const int &_modelId) {
    int address_;
    Command<0x0F17>(_modelId, &address_);
    return std::make_tuple(address_);
  });
}