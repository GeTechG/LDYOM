// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindWorld(sol::state &state) {
  auto table = state.create_table("WorldOp");
  // default
  table.set_function("setCarDensityMultiplier", [](const float &_multiplier) { Command<0x01EB>(_multiplier); });
  table.set_function("getGroundZFor3DCoord", [](const float &_x, const float &_y, const float &_z) {
    float groundZ_;
    Command<0x02CE>(_x, _y, _z, &groundZ_);
    return std::make_tuple(groundZ_);
  });
  table.set_function("isProjectileInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                              const float &_leftBottomZ, const float &_rightTopX,
                                              const float &_rightTopY, const float &_rightTopZ) {
    auto result = Command<0x02EE>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
    return std::make_tuple(result);
  });
  table.set_function("removeAllScriptFires", []() { Command<0x031A>(); });
  table.set_function("getRandomCarOfTypeInArea",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const int &_modelId) {
                       int handle_;
                       Command<0x0327>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _modelId, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("isAreaOccupied",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ, const bool &_solid,
                        const bool &_car, const bool &_char, const bool &_object, const bool &_particle) {
                       auto result = Command<0x0339>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY,
                                                     _rightTopZ, _solid, _car, _char, _object, _particle);
                       return std::make_tuple(result);
                     });
  table.set_function("isExplosionInArea", [](const int &_explosionType, const float &_leftBottomX,
                                             const float &_leftBottomY, const float &_leftBottomZ,
                                             const float &_rightTopX, const float &_rightTopY,
                                             const float &_rightTopZ) {
    auto result =
        Command<0x0356>(_explosionType, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
    return std::make_tuple(result);
  });
  table.set_function("setVisibilityOfClosestObjectOfType",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_modelId,
                        const bool &_state) { Command<0x0363>(_x, _y, _z, _radius, _modelId, _state); });
  table.set_function("isPointObscuredByAMissionEntity",
                     [](const float &_x, const float &_y, const float &_z, const float &_radiusX, const float &_radiusY,
                        const float &_radiusZ) {
                       auto result = Command<0x038A>(_x, _y, _z, _radiusX, _radiusY, _radiusZ);
                       return std::make_tuple(result);
                     });
  table.set_function("clearArea",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius,
                        const bool &_clearParticles) { Command<0x0395>(_x, _y, _z, _radius, _clearParticles); });
  table.set_function("swapNearestBuildingModel",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius,
                        const int &_fromModelId,
                        const int &_toModelId) { Command<0x03B6>(_x, _y, _z, _radius, _fromModelId, _toModelId); });
  table.set_function("switchProcessing", [](const bool &_state) { Command<0x03B7>(_state); });
  table.set_function("clearAreaOfCars",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x03BA>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("createRandomCarForCarPark", [](const float &_x, const float &_y, const float &_z,
                                                     const float &_heading) { Command<0x03C5>(_x, _y, _z, _heading); });
  table.set_function("setPedDensityMultiplier", [](const float &_multiplier) { Command<0x03DE>(_multiplier); });
  table.set_function("clearAreaOfChars",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x042B>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("isAnyPickupAtCoords", [](const float &_x, const float &_y, const float &_z) {
    auto result = Command<0x048C>(_x, _y, _z);
    return std::make_tuple(result);
  });
  table.set_function("getDeadCharPickupCoords", [](const int &_char) {
    float x_;
    float y_;
    float z_;
    Command<0x04A5>(_char, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("createScriptRoadblock", [](const float &_leftBottomX, const float &_leftBottomY,
                                                 const float &_leftBottomZ, const float &_rightTopX,
                                                 const float &_rightTopY, const float &_rightTopZ, const int &_type) {
    Command<0x04C0>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ, _type);
  });
  table.set_function("clearAllScriptRoadblocks", []() { Command<0x04C1>(); });
  table.set_function("addSetPiece", [](const int &_type, const float &_fromX, const float &_fromY, const float &_toX,
                                       const float &_toY, const float &_spawnPoliceAAtX, const float &_spawnPoliceAAtY,
                                       const float &_headedTowardsAAtX, const float &_headedTowardsAAtY,
                                       const float &_spawnPoliceBAtX, const float &_spawnPoliceBAtY,
                                       const float &_headedTowardsBAtX, const float &_headedTowardsBAtY) {
    Command<0x04F8>(_type, _fromX, _fromY, _toX, _toY, _spawnPoliceAAtX, _spawnPoliceAAtY, _headedTowardsAAtX,
                    _headedTowardsAAtY, _spawnPoliceBAtX, _spawnPoliceBAtY, _headedTowardsBAtX, _headedTowardsBAtY);
  });
  table.set_function("setExtraColors", [](const int &_color, const bool &_fade) { Command<0x04F9>(_color, _fade); });
  table.set_function("clearExtraColors", [](const bool &_withFade) { Command<0x04FA>(_withFade); });
  table.set_function("getRandomCarOfTypeInAreaNoSave",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const int &_modelId) {
                       int handle_;
                       Command<0x053E>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _modelId, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("fireSingleBullet",
                     [](const float &_fromX, const float &_fromY, const float &_fromZ, const float &_toX,
                        const float &_toY, const float &_toZ,
                        const int &_energy) { Command<0x06BC>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _energy); });
  table.set_function("isLineOfSightClear", [](const float &_fromX, const float &_fromY, const float &_fromZ,
                                              const float &_toX, const float &_toY, const float &_toZ,
                                              const bool &_buildings, const bool &_cars, const bool &_chars,
                                              const bool &_objects, const bool &_particles) {
    auto result =
        Command<0x06BD>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _buildings, _cars, _chars, _objects, _particles);
    return std::make_tuple(result);
  });
  table.set_function("getNumberOfFiresInRange",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius) {
                       int numFires_;
                       Command<0x06C3>(_x, _y, _z, _radius, &numFires_);
                       return std::make_tuple(numFires_);
                     });
  table.set_function("deleteMissionTrains", []() { Command<0x06D9>(); });
  table.set_function("deleteAllTrains", []() { Command<0x06DB>(); });
  table.set_function("getPercentageTaggedInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                                     const float &_rightTopX, const float &_rightTopY) {
    int percent_;
    Command<0x0702>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY, &percent_);
    return std::make_tuple(percent_);
  });
  table.set_function("setTagStatusInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                              const float &_rightTopX, const float &_rightTopY, const int &_percent) {
    Command<0x0703>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _percent);
  });
  table.set_function("isClosestObjectOfTypeSmashedOrDamaged",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_modelId,
                        const bool &_smashed, const bool &_damaged) {
                       auto result = Command<0x0716>(_x, _y, _z, _radius, _modelId, _smashed, _damaged);
                       return std::make_tuple(result);
                     });
  table.set_function("isFlameInAngledArea2D", [](const float &_leftBottomX, const float &_leftBottomY,
                                                 const float &_rightTopX, const float &_rightTopY, const float &_angle,
                                                 const bool &_drawSphere) {
    auto result = Command<0x072D>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isFlameInAngledArea3D",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ, const float &_angle,
                        const bool &_drawSphere) {
                       auto result = Command<0x072E>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY,
                                                     _rightTopZ, _angle, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("getRandomCarInSphereNoSave",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_model) {
                       int handle_;
                       Command<0x073E>(_x, _y, _z, _radius, _model, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("getRandomCharInSphere",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const bool &_civilian,
                        const bool &_gang, const bool &_criminal) {
                       int handle_;
                       Command<0x073F>(_x, _y, _z, _radius, _civilian, _gang, _criminal, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("getNumberOfFiresInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                                  const float &_leftBottomZ, const float &_rightTopX,
                                                  const float &_rightTopY, const float &_rightTopZ) {
    int numFires_;
    Command<0x0786>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ, &numFires_);
    return std::make_tuple(numFires_);
  });
  table.set_function("getNearestTagPosition", [](const float &_xCoord, const float &_yCoord, const float &_zCoord) {
    float x_;
    float y_;
    float z_;
    Command<0x07A6>(_xCoord, _yCoord, _zCoord, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("removeOilPuddlesInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                                  const float &_rightTopX, const float &_rightTopY) {
    Command<0x07DF>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY);
  });
  table.set_function("getCityFromCoords", [](const float &_x, const float &_y, const float &_z) {
    int townId_;
    Command<0x07EF>(_x, _y, _z, &townId_);
    return std::make_tuple(townId_);
  });
  table.set_function("hasObjectOfTypeBeenSmashed",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_modelId) {
                       auto result = Command<0x07F0>(_x, _y, _z, _radius, _modelId);
                       return std::make_tuple(result);
                     });
  table.set_function("switchEntryExit",
                     [](const char *_interiorName, const bool &_state) { Command<0x07FB>(_interiorName, _state); });
  table.set_function("getParkingNodeInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                                const float &_leftBottomZ, const float &_rightTopX,
                                                const float &_rightTopY, const float &_rightTopZ) {
    float x_;
    float y_;
    float z_;
    Command<0x0810>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("addStuntJump", [](const float &_startX, const float &_startY, const float &_startZ,
                                        const float &_startRadiusX, const float &_startRadiusY,
                                        const float &_startRadiusZ, const float &_finishX, const float &_finishY,
                                        const float &_finishZ, const float &_finishRadiusX, const float &_finishRadiusY,
                                        const float &_finishRadiusZ, const float &_cameraX, const float &_cameraY,
                                        const float &_cameraZ, const int &_reward) {
    Command<0x0814>(_startX, _startY, _startZ, _startRadiusX, _startRadiusY, _startRadiusZ, _finishX, _finishY,
                    _finishZ, _finishRadiusX, _finishRadiusY, _finishRadiusZ, _cameraX, _cameraY, _cameraZ, _reward);
  });
  table.set_function("setPoolTableCoords",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x0830>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("getSoundLevelAtCoords",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z) {
                       float level_;
                       Command<0x0855>(_handle, _x, _y, _z, &level_);
                       return std::make_tuple(level_);
                     });
  table.set_function("createEmergencyServicesCar", [](const int &_model, const float &_x, const float &_y,
                                                      const float &_z) { Command<0x085A>(_model, _x, _y, _z); });
  table.set_function("getClosestStealableObject",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius) {
                       int handle_;
                       Command<0x0866>(_x, _y, _z, _radius, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("createBirds", [](const float &_xFrom, const float &_yFrom, const float &_zFrom, const float &_xTo,
                                       const float &_yTo, const float &_zTo, const int &_quantity, const int &_type) {
    Command<0x0876>(_xFrom, _yFrom, _zFrom, _xTo, _yTo, _zTo, _quantity, _type);
  });
  table.set_function("setUsesCollisionOfClosestObjectOfType",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_modelId,
                        const bool &_state) { Command<0x088D>(_x, _y, _z, _radius, _modelId, _state); });
  table.set_function("getRandomCharInSphereOnlyDrugsBuyers",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius) {
                       int handle_;
                       Command<0x089E>(_x, _y, _z, _radius, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("getRandomCharInSphereNoBrain",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius) {
                       int handle_;
                       Command<0x08E5>(_x, _y, _z, _radius, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("disableAllEntryExits", [](const bool &_state) { Command<0x08E7>(_state); });
  table.set_function("getUserOfClosestMapAttractor",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_modelId,
                        const char *_attractorName) {
                       int handle_;
                       Command<0x091C>(_x, _y, _z, _radius, _modelId, _attractorName, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("getWaterHeightAtCoords", [](const float &_x, const float &_y, const bool &_ignoreWaves) {
    float height_;
    Command<0x092E>(_x, _y, _ignoreWaves, &height_);
    return std::make_tuple(height_);
  });
  table.set_function("syncWater", []() { Command<0x0971>(); });
  table.set_function("extinguishFireAtPoint", [](const float &_x, const float &_y, const float &_z,
                                                 const float &_radius) { Command<0x0980>(_x, _y, _z, _radius); });
  table.set_function("setCharUsesCollisionClosestObjectOfType",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const int &_modelId,
                        const bool &_state,
                        const int &_target) { Command<0x0985>(_x, _y, _z, _radius, _modelId, _state, _target); });
  table.set_function("clearAllScriptFireFlags", []() { Command<0x0986>(); });
  table.set_function("setClosestEntryExitFlag",
                     [](const float &_x, const float &_y, const float &_radius, const int &_entryexitsFlag,
                        const bool &_state) { Command<0x09B4>(_x, _y, _radius, _entryexitsFlag, _state); });
  table.set_function("getRandomCarOfTypeInAngledAreaNoSave",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const float &_angle, const int &_modelId) {
                       int vehicle_;
                       Command<0x09C0>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle, _modelId, &vehicle_);
                       return std::make_tuple(vehicle_);
                     });
  table.set_function("isCopVehicleInArea3DNoSave", [](const float &_leftBottomX, const float &_leftBottomY,
                                                      const float &_leftBottomZ, const float &_rightTopX,
                                                      const float &_rightTopY, const float &_rightTopZ) {
    auto result = Command<0x09C3>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
    return std::make_tuple(result);
  });
  table.set_function("isMoneyPickupAtCoords", [](const float &_x, const float &_y, const float &_z) {
    auto result = Command<0x09DA>(_x, _y, _z);
    return std::make_tuple(result);
  });
  table.set_function("getRandomCharInAreaOffsetNoSave",
                     [](const float &_x, const float &_y, const float &_z, const float &_radiusX, const float &_radiusY,
                        const float &_radiusZ) {
                       int handle_;
                       Command<0x0A3E>(_x, _y, _z, _radiusX, _radiusY, _radiusZ, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("setRailtrackResistanceMult", [](const float &_mult) { Command<0x0A45>(_mult); });

  // CLEO
  table.set_function("getTargetCoords", []() {
    float x_;
    float y_;
    float z_;
    auto result = Command<0x0AB6>(&x_, &y_, &z_);
    return std::make_tuple(result, x_, y_, z_);
  });
  table.set_function("spawnVehicleByCheating", [](const int &_modelId) { Command<0x0ADD>(_modelId); });
  table.set_function("getRandomCharInSphereNoSaveRecursive",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const bool &_findNext,
                        const bool &_skipDead) {
                       int handle_;
                       auto result = Command<0x0AE1>(_x, _y, _z, _radius, _findNext, _skipDead, &handle_);
                       return std::make_tuple(result, handle_);
                     });
  table.set_function("getRandomCarInSphereNoSaveRecursive",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius, const bool &_findNext,
                        const bool &_skipWrecked) {
                       int handle_;
                       auto result = Command<0x0AE2>(_x, _y, _z, _radius, _findNext, _skipWrecked, &handle_);
                       return std::make_tuple(result, handle_);
                     });
  table.set_function("getRandomObjectInSphereNoSaveRecursive", [](const float &_x, const float &_y, const float &_z,
                                                                  const float &_radius, const bool &_findNext) {
    int handle_;
    auto result = Command<0x0AE3>(_x, _y, _z, _radius, _findNext, &handle_);
    return std::make_tuple(result, handle_);
  });

  // CLEO+
  table.set_function("getPickupThisCoord",
                     [](const float &_x, const float &_y, const float &_z, const bool &_onlyValid) {
                       int handle_;
                       Command<0x0E33>(_x, _y, _z, _onlyValid, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("convert3DToScreen2D",
                     [](const float &_x, const float &_y, const float &_z, const int &_nearClip, const int &_farClip) {
                       float x_;
                       float y_;
                       float sizeX_;
                       float sizeY_;
                       auto result = Command<0x0E3F>(_x, _y, _z, _nearClip, _farClip, &x_, &y_, &sizeX_, &sizeY_);
                       return std::make_tuple(result, x_, y_, sizeX_, sizeY_);
                     });
  table.set_function("getClosestCopNearPos", [](const float &_x, const float &_y, const float &_z, const float &_radius,
                                                const bool &_alive, const bool &_inCar, const bool &_onFoot) {
    int closestCop_;
    auto result = Command<0x0EA6>(_x, _y, _z, _radius, _alive, _inCar, _onFoot, &closestCop_);
    return std::make_tuple(result, closestCop_);
  });
  table.set_function("getAnyCharNoSaveRecursive", [](const int &_progress) {
    int progress_;
    int anyChar_;
    auto result = Command<0x0EA7>(_progress, &progress_, &anyChar_);
    return std::make_tuple(result, progress_, anyChar_);
  });
  table.set_function("getAnyCarNoSaveRecursive", [](const int &_progress) {
    int progress_;
    int anyCar_;
    auto result = Command<0x0EA8>(_progress, &progress_, &anyCar_);
    return std::make_tuple(result, progress_, anyCar_);
  });
  table.set_function("getAnyObjectNoSaveRecursive", [](const int &_progress) {
    int progress_;
    int anyObject_;
    auto result = Command<0x0EA9>(_progress, &progress_, &anyObject_);
    return std::make_tuple(result, progress_, anyObject_);
  });
  table.set_function("getCoordFromAngledDistance",
                     [](const float &_x, const float &_y, const float &_angle, const float &_distance) {
                       float x_;
                       float y_;
                       Command<0x0EF0>(_x, _y, _angle, _distance, &x_, &y_);
                       return std::make_tuple(x_, y_);
                     });
}