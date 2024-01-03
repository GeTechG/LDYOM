// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindMath(sol::state &state) {
  auto table = state.create_table("MathOp");
  // default
  table.set_function("abs", []() {
    int number_;
    Command<0x0094>(&number_);
    return std::make_tuple(number_);
  });
  table.set_function("abs", []() {
    int number_;
    Command<0x0095>(&number_);
    return std::make_tuple(number_);
  });
  table.set_function("abs", []() {
    float number_;
    Command<0x0096>(&number_);
    return std::make_tuple(number_);
  });
  table.set_function("abs", []() {
    float number_;
    Command<0x0097>(&number_);
    return std::make_tuple(number_);
  });
  table.set_function("random", []() {
    float float_;
    Command<0x0098>(&float_);
    return std::make_tuple(float_);
  });
  table.set_function("random", []() {
    int int_;
    Command<0x0099>(&int_);
    return std::make_tuple(int_);
  });
  table.set_function("sqrt", [](const float &_num) {
    float result_;
    Command<0x01FB>(_num, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("randomFloatInRange", [](const float &_min, const float &_max) {
    float result_;
    Command<0x0208>(_min, _max, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("randomIntInRange", [](const int &_min, const int &_max) {
    int result_;
    Command<0x0209>(_min, _max, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("sin", [](const float &_angle) {
    float result_;
    Command<0x02F6>(_angle, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("cos", [](const float &_angle) {
    float result_;
    Command<0x02F7>(_angle, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("convertMetersToFeet", [](const float &_meters) {
    float feet_;
    Command<0x0425>(_meters, &feet_);
    return std::make_tuple(feet_);
  });
  table.set_function("convertMetersToFeet", [](const int &_meters) {
    int feet_;
    Command<0x042D>(_meters, &feet_);
    return std::make_tuple(feet_);
  });
  table.set_function("getDistanceBetweenCoords2D",
                     [](const float &_fromX, const float &_fromY, const float &_toX, const float &_toZ) {
                       float distance_;
                       Command<0x0509>(_fromX, _fromY, _toX, _toZ, &distance_);
                       return std::make_tuple(distance_);
                     });
  table.set_function("getDistanceBetweenCoords3D", [](const float &_fromX, const float &_fromY, const float &_fromZ,
                                                      const float &_toX, const float &_toY, const float &_toZ) {
    float distance_;
    Command<0x050A>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, &distance_);
    return std::make_tuple(distance_);
  });
  table.set_function("getAngleBetween2DVectors",
                     [](const float &_x1, const float &_y1, const float &_x2, const float &_y2) {
                       float angle_;
                       Command<0x05A4>(_x1, _y1, _x2, _y2, &angle_);
                       return std::make_tuple(angle_);
                     });
  table.set_function("do2DRectanglesCollide", [](const float &_rectangle1PositionX, const float &_rectangle1PositionY,
                                                 const float &_rectangle1SizeX, const float &_rectangle1SizeY,
                                                 const float &_rectangle2PositionX, const float &_rectangle2PositionY,
                                                 const float &_rectangle2SizeX, const float &_rectangle2SizeY) {
    auto result = Command<0x05A5>(_rectangle1PositionX, _rectangle1PositionY, _rectangle1SizeX, _rectangle1SizeY,
                                  _rectangle2PositionX, _rectangle2PositionY, _rectangle2SizeX, _rectangle2SizeY);
    return std::make_tuple(result);
  });
  table.set_function("get2DLinesIntersectPoint", [](const float &_line1StartX, const float &_line1StartY,
                                                    const float &_line1EndX, const float &_line1EndY,
                                                    const float &_line2StartX, const float &_line2StartY,
                                                    const float &_line2EndX, const float &_line2EndY) {
    float intersectPointX_;
    float intersectPointY_;
    auto result = Command<0x05B0>(_line1StartX, _line1StartY, _line1EndX, _line1EndY, _line2StartX, _line2StartY,
                                  _line2EndX, _line2EndY, &intersectPointX_, &intersectPointY_);
    return std::make_tuple(result, intersectPointX_, intersectPointY_);
  });
  table.set_function("getHeadingFromVector2D", [](const float &_x, const float &_y) {
    float heading_;
    Command<0x0604>(_x, _y, &heading_);
    return std::make_tuple(heading_);
  });
  table.set_function("limitAngle", [](const float &_value) {
    float result_;
    Command<0x0656>(_value, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("isBitSet", [](const int &_number, const int &_n) {
    auto result = Command<0x08B4>(_number, _n);
    return std::make_tuple(result);
  });
  table.set_function("isBitSet", [](const int &_number, const int &_n) {
    auto result = Command<0x08B5>(_number, _n);
    return std::make_tuple(result);
  });
  table.set_function("isBitSet", [](const int &_number, const int &_n) {
    auto result = Command<0x08B6>(_number, _n);
    return std::make_tuple(result);
  });
  table.set_function("isBitSet", [](const int &_number, const int &_n) {
    auto result = Command<0x08B7>(_number, _n);
    return std::make_tuple(result);
  });
  table.set_function("isBitSet", [](const int &_number, const int &_n) {
    auto result = Command<0x08B8>(_number, _n);
    return std::make_tuple(result);
  });
  table.set_function("isBitSet", [](const int &_number, const int &_n) {
    auto result = Command<0x08B9>(_number, _n);
    return std::make_tuple(result);
  });
  table.set_function("setBit", [](const int &_number, const int &_n) { Command<0x08BA>(_number, _n); });
  table.set_function("setBit", [](const int &_number, const int &_n) { Command<0x08BB>(_number, _n); });
  table.set_function("setBit", [](const int &_number, const int &_n) { Command<0x08BC>(_number, _n); });
  table.set_function("setBit", [](const int &_number, const int &_n) { Command<0x08BD>(_number, _n); });
  table.set_function("setBit", [](const int &_number, const int &_n) { Command<0x08BE>(_number, _n); });
  table.set_function("setBit", [](const int &_number, const int &_n) { Command<0x08BF>(_number, _n); });
  table.set_function("clearBit", [](const int &_number, const int &_n) { Command<0x08C0>(_number, _n); });
  table.set_function("clearBit", [](const int &_number, const int &_n) { Command<0x08C1>(_number, _n); });
  table.set_function("clearBit", [](const int &_number, const int &_n) { Command<0x08C2>(_number, _n); });
  table.set_function("clearBit", [](const int &_number, const int &_n) { Command<0x08C3>(_number, _n); });
  table.set_function("clearBit", [](const int &_number, const int &_n) { Command<0x08C4>(_number, _n); });
  table.set_function("clearBit", [](const int &_number, const int &_n) { Command<0x08C5>(_number, _n); });

  // CLEO
  table.set_function("pow", [](const float &_number, const float &_power) {
    float result_;
    Command<0x0AEE>(_number, _power, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("log", [](const float &_number, const float &_base) {
    float result_;
    Command<0x0AEF>(_number, _base, &result_);
    return std::make_tuple(result_);
  });

  // CLEO+
  table.set_function("perlinNoise", [](const float &_x) {
    float result_;
    Command<0x0E03>(_x, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("ease", [](const float &_k, const int &_mode, const int &_way) {
    float result_;
    Command<0x0E1F>(_k, _mode, _way, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("getAngleFromTwoCoords",
                     [](const float &_x1, const float &_y1, const float &_x2, const float &_y2) {
                       float angle_;
                       Command<0x0E27>(_x1, _y1, _x2, _y2, &angle_);
                       return std::make_tuple(angle_);
                     });
  table.set_function("perlinNoiseFractal",
                     [](const float &_x, const int &_octaves, const float &_frequency, const float &_amplitude,
                        const float &_lacunarity, const float &_persistence) {
                       float result_;
                       Command<0x0E29>(_x, _octaves, _frequency, _amplitude, _lacunarity, _persistence, &result_);
                       return std::make_tuple(result_);
                     });
  table.set_function("randomPercent", [](const int &_percent) {
    auto result = Command<0x0E4D>(_percent);
    return std::make_tuple(result);
  });
  table.set_function("generateRandomIntInRangeWithSeed", [](const int &_seed, const int &_min, const int &_max) {
    int randomInteger_;
    Command<0x0EBC>(_seed, _min, _max, &randomInteger_);
    return std::make_tuple(randomInteger_);
  });
  table.set_function("generateRandomFloatInRangeWithSeed", [](const int &_seed, const float &_min, const float &_max) {
    float randomFloat_;
    Command<0x0EBD>(_seed, _min, _max, &randomFloat_);
    return std::make_tuple(randomFloat_);
  });
  table.set_function("perlinNoiseFractal2D",
                     [](const float &_x, const float &_y, const int &_octaves, const float &_frequency,
                        const float &_amplitude, const float &_lacunarity, const float &_persistence) {
                       float result_;
                       Command<0x0EF1>(_x, _y, _octaves, _frequency, _amplitude, _lacunarity, _persistence, &result_);
                       return std::make_tuple(result_);
                     });
  table.set_function("perlinNoiseFractal3D", [](const float &_x, const float &_y, const float &_z, const int &_octaves,
                                                const float &_frequency, const float &_amplitude,
                                                const float &_lacunarity, const float &_persistence) {
    float result_;
    Command<0x0EF2>(_x, _y, _z, _octaves, _frequency, _amplitude, _lacunarity, _persistence, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("clampFloat", [](const float &_float, const float &_min, const float &_max) {
    float clamped_;
    Command<0x0EF4>(_float, _min, _max, &clamped_);
    return std::make_tuple(clamped_);
  });
  table.set_function("clampInt", [](const float &_integer, const float &_min, const float &_max) {
    float clamped_;
    Command<0x0EF7>(_integer, _min, _max, &clamped_);
    return std::make_tuple(clamped_);
  });

  // bitwise
  table.set_function("signExtend", [](const int &_value, const int &_fromSize) { Command<0x0B1E>(_value, _fromSize); });
}