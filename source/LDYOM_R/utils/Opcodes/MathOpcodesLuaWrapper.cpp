//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void MathOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("MathOp");
    //default
	table.set_function("abs", []() {int number_; auto result = Command<0x0094>(&number_); return std::make_tuple(number_, result);});
	table.set_function("abs", []() {int number_; auto result = Command<0x0095>(&number_); return std::make_tuple(number_, result);});
	table.set_function("abs", []() {float number_; auto result = Command<0x0096>(&number_); return std::make_tuple(number_, result);});
	table.set_function("abs", []() {float number_; auto result = Command<0x0097>(&number_); return std::make_tuple(number_, result);});
	table.set_function("random", []() {float float_; auto result = Command<0x0098>(&float_); return std::make_tuple(float_, result);});
	table.set_function("random", []() {int int_; auto result = Command<0x0099>(&int_); return std::make_tuple(int_, result);});
	table.set_function("sqrt", [](const float& _num) {float result_; auto result = Command<0x01FB>(_num, &result_); return std::make_tuple(result_, result);});
	table.set_function("randomFloatInRange", [](const float& _min, const float& _max) {float result_; auto result = Command<0x0208>(_min, _max, &result_); return std::make_tuple(result_, result);});
	table.set_function("randomIntInRange", [](const int& _min, const int& _max) {int result_; auto result = Command<0x0209>(_min, _max, &result_); return std::make_tuple(result_, result);});
	table.set_function("sin", [](const float& _angle) {float result_; auto result = Command<0x02F6>(_angle, &result_); return std::make_tuple(result_, result);});
	table.set_function("cos", [](const float& _angle) {float result_; auto result = Command<0x02F7>(_angle, &result_); return std::make_tuple(result_, result);});
	table.set_function("convertMetersToFeet", [](const float& _meters) {float feet_; auto result = Command<0x0425>(_meters, &feet_); return std::make_tuple(feet_, result);});
	table.set_function("convertMetersToFeet", [](const int& _meters) {int feet_; auto result = Command<0x042D>(_meters, &feet_); return std::make_tuple(feet_, result);});
	table.set_function("getDistanceBetweenCoords2D", [](const float& _fromX, const float& _fromY, const float& _toX, const float& _toZ) {float distance_; auto result = Command<0x0509>(_fromX, _fromY, _toX, _toZ, &distance_); return std::make_tuple(distance_, result);});
	table.set_function("getDistanceBetweenCoords3D", [](const float& _fromX, const float& _fromY, const float& _fromZ, const float& _toX, const float& _toY, const float& _toZ) {float distance_; auto result = Command<0x050A>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, &distance_); return std::make_tuple(distance_, result);});
	table.set_function("getAngleBetween2DVectors", [](const float& _x1, const float& _y1, const float& _x2, const float& _y2) {float angle_; auto result = Command<0x05A4>(_x1, _y1, _x2, _y2, &angle_); return std::make_tuple(angle_, result);});
	table.set_function("getHeadingFromVector2D", [](const float& _x, const float& _y) {float heading_; auto result = Command<0x0604>(_x, _y, &heading_); return std::make_tuple(heading_, result);});
	table.set_function("limitAngle", [](const float& _value) {float result_; auto result = Command<0x0656>(_value, &result_); return std::make_tuple(result_, result);});
	
	//CLEO
	table.set_function("pow", [](const float& _number, const float& _power) {float result_; auto result = Command<0x0AEE>(_number, _power, &result_); return std::make_tuple(result_, result);});
	table.set_function("log", [](const float& _number, const float& _base) {float result_; auto result = Command<0x0AEF>(_number, _base, &result_); return std::make_tuple(result_, result);});
	
	//CLEO+
	table.set_function("perlinNoise", [](const float& _x) {float result_; auto result = Command<0x0E03>(_x, &result_); return std::make_tuple(result_, result);});
	table.set_function("ease", [](const float& _k, const int& _mode, const int& _way) {float result_; auto result = Command<0x0E1F>(_k, _mode, _way, &result_); return std::make_tuple(result_, result);});
	table.set_function("getAngleFromTwoCoords", [](const float& _x1, const float& _y1, const float& _x2, const float& _y2) {float angle_; auto result = Command<0x0E27>(_x1, _y1, _x2, _y2, &angle_); return std::make_tuple(angle_, result);});
	table.set_function("perlinNoiseFractal", [](const float& _x, const int& _octaves, const float& _frequency, const float& _amplitude, const float& _lacunarity, const float& _persistence) {float result_; auto result = Command<0x0E29>(_x, _octaves, _frequency, _amplitude, _lacunarity, _persistence, &result_); return std::make_tuple(result_, result);});
	table.set_function("perlinNoiseFractal2D", [](const float& _x, const float& _y, const int& _octaves, const float& _frequency, const float& _amplitude, const float& _lacunarity, const float& _persistence) {float result_; auto result = Command<0x0EF1>(_x, _y, _octaves, _frequency, _amplitude, _lacunarity, _persistence, &result_); return std::make_tuple(result_, result);});
	table.set_function("perlinNoiseFractal3D", [](const float& _x, const float& _y, const float& _z, const int& _octaves, const float& _frequency, const float& _amplitude, const float& _lacunarity, const float& _persistence) {float result_; auto result = Command<0x0EF2>(_x, _y, _z, _octaves, _frequency, _amplitude, _lacunarity, _persistence, &result_); return std::make_tuple(result_, result);});

}