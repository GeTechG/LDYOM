// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindPath(sol::state &state) {
  auto table = state.create_table("PathOp");
  // default
  table.set_function("switchRoadsOn",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x01E7>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("switchRoadsOff",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x01E8>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("switchPedRoadsOn",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x022A>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("switchPedRoadsOff",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x022B>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("getClosestCharNode", [](const float &_x, const float &_y, const float &_z) {
    float nodeX_;
    float nodeY_;
    float nodeZ_;
    Command<0x02C0>(_x, _y, _z, &nodeX_, &nodeY_, &nodeZ_);
    return std::make_tuple(nodeX_, nodeY_, nodeZ_);
  });
  table.set_function("getClosestCarNode", [](const float &_x, const float &_y, const float &_z) {
    float nodeX_;
    float nodeY_;
    float nodeZ_;
    Command<0x02C1>(_x, _y, _z, &nodeX_, &nodeY_, &nodeZ_);
    return std::make_tuple(nodeX_, nodeY_, nodeZ_);
  });
  table.set_function("getClosestCarNodeWithHeading", [](const float &_x, const float &_y, const float &_z) {
    float nodeX_;
    float nodeY_;
    float nodeZ_;
    float angle_;
    Command<0x03D3>(_x, _y, _z, &nodeX_, &nodeY_, &nodeZ_, &angle_);
    return std::make_tuple(nodeX_, nodeY_, nodeZ_, angle_);
  });
  table.set_function("getClosestStraightRoad", [](const float &_x, const float &_y, const float &_z,
                                                  const float &_minDist, const float &_maxDist) {
    float node1X_;
    float node1Y_;
    float node1Z_;
    float node2X_;
    float node2Y_;
    float node2Z_;
    float angle_;
    Command<0x04B9>(_x, _y, _z, _minDist, _maxDist, &node1X_, &node1Y_, &node1Z_, &node2X_, &node2Y_, &node2Z_,
                    &angle_);
    return std::make_tuple(node1X_, node1Y_, node1Z_, node2X_, node2Y_, node2Z_, angle_);
  });
  table.set_function("getNthClosestCarNode",
                     [](const float &_fromX, const float &_fromY, const float &_fromZ, const int &_n) {
                       float x_;
                       float y_;
                       float z_;
                       Command<0x04D3>(_fromX, _fromY, _fromZ, _n, &x_, &y_, &z_);
                       return std::make_tuple(x_, y_, z_);
                     });
  table.set_function("flushRoute", []() { Command<0x05D6>(); });
  table.set_function("extendRoute",
                     [](const float &_x, const float &_y, const float &_z) { Command<0x05D7>(_x, _y, _z); });
  table.set_function("loadPathNodesInArea", [](const float &_leftBottomX, const float &_leftBottomY,
                                               const float &_rightTopX, const float &_rightTopY) {
    Command<0x0606>(_leftBottomX, _leftBottomY, _rightTopX, _rightTopY);
  });
  table.set_function("releaseNodes", []() { Command<0x0607>(); });
  table.set_function("getNthClosestCarNodeWithHeading",
                     [](const float &_xCoord, const float &_yCoord, const float &_zCoord, const int &_nth) {
                       float x_;
                       float y_;
                       float z_;
                       float heading_;
                       Command<0x06F8>(_xCoord, _yCoord, _zCoord, _nth, &x_, &y_, &z_, &heading_);
                       return std::make_tuple(x_, y_, z_, heading_);
                     });
  table.set_function("flushPatrolRoute", []() { Command<0x0754>(); });
  table.set_function("extendPatrolRoute",
                     [](const float &_x, const float &_y, const float &_z, const char *_animationName,
                        const char *_animationFile) { Command<0x0755>(_x, _y, _z, _animationName, _animationFile); });
  table.set_function("switchRoadsBackToOriginal",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x091D>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("switchPedRoadsBackToOriginal",
                     [](const float &_leftBottomX, const float &_leftBottomY, const float &_leftBottomZ,
                        const float &_rightTopX, const float &_rightTopY, const float &_rightTopZ) {
                       Command<0x091E>(_leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ);
                     });
  table.set_function("markRoadNodeAsDontWander",
                     [](const float &_x, const float &_y, const float &_z) { Command<0x0994>(_x, _y, _z); });
  table.set_function("unmarkAllRoadNodesAsDontWander", []() { Command<0x0995>(); });
}