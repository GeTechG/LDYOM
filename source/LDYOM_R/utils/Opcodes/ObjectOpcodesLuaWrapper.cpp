//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ObjectOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ObjectOp");
    //default
	table.set_function("create", [](const int& _modelId, const float& _x, const float& _y, const float& _z) {int handle_; auto result = Command<0x0107>(_modelId, _x, _y, _z, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("delete", [](const int& _self) { auto result = Command<0x0108>(_self); return std::make_tuple(result);});
	table.set_function("getHeading", [](const int& _self) {float heading_; auto result = Command<0x0176>(_self, &heading_); return std::make_tuple(heading_, result);});
	table.set_function("setHeading", [](const int& _self, const float& _heading) { auto result = Command<0x0177>(_self, _heading); return std::make_tuple(result);});
	table.set_function("getCoordinates", [](const int& _self) {float x_;float y_;float z_; auto result = Command<0x01BB>(_self, &x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("setCoordinates", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x01BC>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("markAsNoLongerNeeded", [](const int& _self) { auto result = Command<0x01C4>(_self); return std::make_tuple(result);});
	table.set_function("dontRemove", [](const int& _self) { auto result = Command<0x01C7>(_self); return std::make_tuple(result);});
	table.set_function("createNoOffset", [](const int& _modelId, const float& _x, const float& _y, const float& _z) {int handle_; auto result = Command<0x029B>(_modelId, _x, _y, _z, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("isOnScreen", [](const int& _self) { auto result = Command<0x02CC>(_self); return std::make_tuple(result);});
	table.set_function("rotate", [](const int& _self, const float& _fromAngle, const float& _toAngle, const bool& _collisionCheck) { auto result = Command<0x034D>(_self, _fromAngle, _toAngle, _collisionCheck); return std::make_tuple(result);});
	table.set_function("slide", [](const int& _self, const float& _fromX, const float& _fromY, const float& _fromZ, const float& _xSpeed, const float& _ySpeed, const float& _zSpeed, const bool& _collisionCheck) { auto result = Command<0x034E>(_self, _fromX, _fromY, _fromZ, _xSpeed, _ySpeed, _zSpeed, _collisionCheck); return std::make_tuple(result);});
	table.set_function("placeRelativeToCar", [](const int& _self, const int& _vehicle, const float& _xOffset, const float& _yOffset, const float& _zOffset) { auto result = Command<0x035C>(_self, _vehicle, _xOffset, _yOffset, _zOffset); return std::make_tuple(result);});
	table.set_function("makeTargetable", [](const int& _self, const bool& _state) { auto result = Command<0x035D>(_self, _state); return std::make_tuple(result);});
	table.set_function("hasBeenDamaged", [](const int& _self) { auto result = Command<0x0366>(_self); return std::make_tuple(result);});
	table.set_function("setVelocity", [](const int& _self, const float& _xSpeed, const float& _ySpeed, const float& _zSpeed) { auto result = Command<0x0381>(_self, _xSpeed, _ySpeed, _zSpeed); return std::make_tuple(result);});
	table.set_function("setCollision", [](const int& _self, const bool& _state) { auto result = Command<0x0382>(_self, _state); return std::make_tuple(result);});
	table.set_function("addToVelocity", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x038C>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("setDynamic", [](const int& _self, const bool& _state) { auto result = Command<0x0392>(_self, _state); return std::make_tuple(result);});
	table.set_function("doesExist", [](const int& _handle) { auto result = Command<0x03CA>(_handle); return std::make_tuple(result);});
	table.set_function("getOffsetInWorldCoords", [](const int& _self, const float& _xOffset, const float& _yOffset, const float& _zOffset) {float x_;float y_;float z_; auto result = Command<0x0400>(_self, _xOffset, _yOffset, _zOffset, &x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("setDrawLast", [](const int& _self, const bool& _state) { auto result = Command<0x0418>(_self, _state); return std::make_tuple(result);});
	table.set_function("setRotation", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x0453>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("setRecordsCollisions", [](const int& _self, const bool& _state) { auto result = Command<0x04D9>(_self, _state); return std::make_tuple(result);});
	table.set_function("hasCollidedWithAnything", [](const int& _self) { auto result = Command<0x04DA>(_self); return std::make_tuple(result);});
	table.set_function("locate2D", [](const int& _self, const float& _x, const float& _y, const float& _xRadius, const float& _yRadius, const bool& _drawSphere) { auto result = Command<0x04E5>(_self, _x, _y, _xRadius, _yRadius, _drawSphere); return std::make_tuple(result);});
	table.set_function("locate3D", [](const int& _self, const float& _x, const float& _y, const float& _z, const float& _xRadius, const float& _yRadius, const float& _zRadius, const bool& _drawSphere) { auto result = Command<0x04E6>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere); return std::make_tuple(result);});
	table.set_function("isInWater", [](const int& _self) { auto result = Command<0x04E7>(_self); return std::make_tuple(result);});
	table.set_function("isInArea2D", [](const int& _self, const float& _leftBottomX, const float& _leftBottomY, const float& _rightTopX, const float& _rightTopY, const bool& _drawSphere) { auto result = Command<0x04E9>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere); return std::make_tuple(result);});
	table.set_function("isInArea3D", [](const int& _self, const float& _leftBottomX, const float& _leftBottomY, const float& _leftBottomZ, const float& _rightTopX, const float& _rightTopY, const float& _rightTopZ, const bool& _drawSphere) { auto result = Command<0x04EA>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ, _drawSphere); return std::make_tuple(result);});
	table.set_function("sortOutCollisionWithCar", [](const int& _self, const int& _handle) { auto result = Command<0x050E>(_self, _handle); return std::make_tuple(result);});
	table.set_function("freezePosition", [](const int& _self, const bool& _state) { auto result = Command<0x0550>(_self, _state); return std::make_tuple(result);});
	table.set_function("setAreaVisible", [](const int& _self, const int& _areaId) { auto result = Command<0x0566>(_self, _areaId); return std::make_tuple(result);});
	table.set_function("getVelocity", [](const int& _self) {float x_;float y_;float z_; auto result = Command<0x059F>(_self, &x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("addToRotationVelocity", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x05A1>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("setRotationVelocity", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x05A2>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("isStatic", [](const int& _self) { auto result = Command<0x05A3>(_self); return std::make_tuple(result);});
	table.set_function("getRotationVelocity", [](const int& _self) {float x_;float y_;float z_; auto result = Command<0x05A6>(_self, &x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("addVelocityRelative", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x05A7>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("getSpeed", [](const int& _self, const float& _speed) { auto result = Command<0x05A8>(_self, _speed); return std::make_tuple(result);});
	table.set_function("setRenderScorched", [](const int& _self, const bool& _state) { auto result = Command<0x0654>(_self, _state); return std::make_tuple(result);});
	table.set_function("attachToCar", [](const int& _self, const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xRotation, const float& _yRotation, const float& _zRotation) { auto result = Command<0x0681>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation); return std::make_tuple(result);});
	table.set_function("detach", [](const int& _self, const float& _x, const float& _y, const float& _z, const bool& _collisionDetection) { auto result = Command<0x0682>(_self, _x, _y, _z, _collisionDetection); return std::make_tuple(result);});
	table.set_function("isAttached", [](const int& _self) { auto result = Command<0x0685>(_self); return std::make_tuple(result);});
	table.set_function("attachToObject", [](const int& _self, const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xRotation, const float& _yRotation, const float& _zRotation) { auto result = Command<0x069A>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation); return std::make_tuple(result);});
	table.set_function("attachToChar", [](const int& _self, const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xRotation, const float& _yRotation, const float& _zRotation) { auto result = Command<0x069B>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation); return std::make_tuple(result);});
	table.set_function("getHealth", [](const int& _self, const int& _health) { auto result = Command<0x071E>(_self, _health); return std::make_tuple(result);});
	table.set_function("setHealth", [](const int& _self, const int& _health) { auto result = Command<0x071F>(_self, _health); return std::make_tuple(result);});
	table.set_function("break", [](const int& _self, const int& _intensity) { auto result = Command<0x0723>(_self, _intensity); return std::make_tuple(result);});
	table.set_function("setVisible", [](const int& _self, const bool& _state) { auto result = Command<0x0750>(_self, _state); return std::make_tuple(result);});
	table.set_function("playAnim", [](const int& _self, const char* _animationName, const char* _animationFile, const float& _framedelta, const bool& _lockF, const bool& _loop) { auto result = Command<0x075A>(_self, _animationName, _animationFile, _framedelta, _lockF, _loop); return std::make_tuple(result);});
	table.set_function("getRopeHeight", [](const int& _self) {float height_; auto result = Command<0x0796>(_self, &height_); return std::make_tuple(height_, result);});
	table.set_function("setRopeHeight", [](const int& _self, const float& _height) { auto result = Command<0x0797>(_self, _height); return std::make_tuple(result);});
	table.set_function("grabEntityOnRope", [](const int& _self) {int vehicle_;int char_;int object_; auto result = Command<0x0798>(_self, &vehicle_, &char_, &object_); return std::make_tuple(vehicle_, char_, object_, result);});
	table.set_function("releaseEntityFromRope", [](const int& _self) { auto result = Command<0x0799>(_self); return std::make_tuple(result);});
	table.set_function("getQuaternion", [](const int& _self, const float& _x, const float& _y, const float& _z, const float& _w) { auto result = Command<0x07C3>(_self, _x, _y, _z, _w); return std::make_tuple(result);});
	table.set_function("setQuaternion", [](const int& _self, const float& _x, const float& _y, const float& _z, const float& _w) { auto result = Command<0x07C4>(_self, _x, _y, _z, _w); return std::make_tuple(result);});
	table.set_function("setCollisionDamageEffect", [](const int& _self, const bool& _state) { auto result = Command<0x07F7>(_self, _state); return std::make_tuple(result);});
	table.set_function("getLevelDesignCoords", [](const int& _self, const int& _nth) {float x_;float y_;float z_; auto result = Command<0x080A>(_self, _nth, &x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("setCoordinatesAndVelocity", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x0815>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("connectLods", [](const int& _self, const int& _lodObject) { auto result = Command<0x0827>(_self, _lodObject); return std::make_tuple(result);});
	table.set_function("hasBeenPhotographed", [](const int& _self) { auto result = Command<0x0833>(_self); return std::make_tuple(result);});
	table.set_function("setAnimSpeed", [](const int& _self, const char* _animationName, const float& _speed) { auto result = Command<0x0836>(_self, _animationName, _speed); return std::make_tuple(result);});
	table.set_function("isPlayingAnim", [](const int& _self, const char* _animationName) { auto result = Command<0x0837>(_self, _animationName); return std::make_tuple(result);});
	table.set_function("getAnimCurrentTime", [](const int& _self, const char* _animationName) {float time_; auto result = Command<0x0839>(_self, _animationName, &time_); return std::make_tuple(time_, result);});
	table.set_function("setAnimCurrentTime", [](const int& _self, const char* _animationName, const float& _time) { auto result = Command<0x083A>(_self, _animationName, _time); return std::make_tuple(result);});
	table.set_function("setOnlyDamagedByPlayer", [](const int& _self, const bool& _state) { auto result = Command<0x0875>(_self, _state); return std::make_tuple(result);});
	table.set_function("setScale", [](const int& _self, const float& _scale) { auto result = Command<0x08D2>(_self, _scale); return std::make_tuple(result);});
	table.set_function("isInAngledArea2D", [](const int& _self, const float& _leftBottomX, const float& _leftBottomY, const float& _rightTopX, const float& _rightTopY, const float& _angle, const bool& _drawSphere) { auto result = Command<0x08E3>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle, _drawSphere); return std::make_tuple(result);});
	table.set_function("isInAngledArea3D", [](const int& _self, const float& _leftBottomX, const float& _leftBottomY, const float& _leftBottomZ, const float& _rightTopX, const float& _rightTopY, const float& _rightTopZ, const float& _angle, const bool& _drawSphere) { auto result = Command<0x08E4>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ, _angle, _drawSphere); return std::make_tuple(result);});
	table.set_function("setAsStealable", [](const int& _self, const bool& _state) { auto result = Command<0x08E9>(_self, _state); return std::make_tuple(result);});
	table.set_function("hasBeenDamagedByWeapon", [](const int& _self, const int& _weaponType) { auto result = Command<0x08FF>(_self, _weaponType); return std::make_tuple(result);});
	table.set_function("clearLastWeaponDamage", [](const int& _self) { auto result = Command<0x0900>(_self); return std::make_tuple(result);});
	table.set_function("lockDoor", [](const int& _self, const bool& _state) { auto result = Command<0x0905>(_self, _state); return std::make_tuple(result);});
	table.set_function("setMass", [](const int& _self, const float& _mass) { auto result = Command<0x0906>(_self, _mass); return std::make_tuple(result);});
	table.set_function("getMass", [](const int& _self, const float& _mass) { auto result = Command<0x0907>(_self, _mass); return std::make_tuple(result);});
	table.set_function("setTurnMass", [](const int& _self, const float& _turnMass) { auto result = Command<0x0908>(_self, _turnMass); return std::make_tuple(result);});
	table.set_function("getTurnMass", [](const int& _self, const float& _turnMass) { auto result = Command<0x0909>(_self, _turnMass); return std::make_tuple(result);});
	table.set_function("winchCanPickUp", [](const int& _self, const bool& _state) { auto result = Command<0x0916>(_self, _state); return std::make_tuple(result);});
	table.set_function("hasBeenUprooted", [](const int& _self) { auto result = Command<0x095B>(_self); return std::make_tuple(result);});
	table.set_function("isWithinBrainActivationRange", [](const int& _self) { auto result = Command<0x0977>(_self); return std::make_tuple(result);});
	table.set_function("getModel", [](const int& _self, const int& _model) { auto result = Command<0x0984>(_self, _model); return std::make_tuple(result);});
	table.set_function("removeElegantly", [](const int& _self) { auto result = Command<0x09A2>(_self); return std::make_tuple(result);});
	table.set_function("setProofs", [](const int& _self, const bool& _bulletProof, const bool& _fireProof, const bool& _explosionProof, const bool& _collisionProof, const bool& _meleeProof) { auto result = Command<0x09CA>(_self, _bulletProof, _fireProof, _explosionProof, _collisionProof, _meleeProof); return std::make_tuple(result);});
	table.set_function("doesHaveThisModel", [](const int& _self, const int& _modelId) { auto result = Command<0x09CC>(_self, _modelId); return std::make_tuple(result);});
	table.set_function("isIntersectingWorld", [](const int& _self) { auto result = Command<0x09FC>(_self); return std::make_tuple(result);});
	table.set_function("enableDisabledAttractors", [](const int& _self, const bool& _state) { auto result = Command<0x0A0A>(_self, _state); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("createNoSave", [](const int& _modelId, const float& _x, const float& _y, const float& _z, const bool& _useOffset, const bool& _useGround) {int handle_; auto result = Command<0x0E01>(_modelId, _x, _y, _z, _useOffset, _useGround, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("isScriptControlled", [](const int& _self) { auto result = Command<0x0E0C>(_self); return std::make_tuple(result);});
	table.set_function("markAsNeeded", [](const int& _self) { auto result = Command<0x0E0D>(_self); return std::make_tuple(result);});
	table.set_function("initExtendedVars", [](const int& _self, const char* _identifier, const int& _totalVars) { auto result = Command<0x0E1A>(_self, _identifier, _totalVars); return std::make_tuple(result);});
	table.set_function("setExtendedVar", [](const int& _self, const char* _identifier, const int& _varNumber, const int& _value) { auto result = Command<0x0E1B>(_self, _identifier, _varNumber, _value); return std::make_tuple(result);});
	table.set_function("getExtendedVar", [](const int& _self, const char* _identifier, const int& _varNumber) {int value_; auto result = Command<0x0E1C>(_self, _identifier, _varNumber, &value_); return std::make_tuple(value_, result);});

}