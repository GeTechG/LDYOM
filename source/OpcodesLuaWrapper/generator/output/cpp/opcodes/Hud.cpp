// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindHud(sol::state &state) {
  auto table = state.create_table("HudOp");
  // default
  table.set_function("displayTimer",
                     [](const int &_timer, const int &_direction) { Command<0x014E>(_timer, _direction); });
  table.set_function("clearTimer", [](const int &_timer) { Command<0x014F>(_timer); });
  table.set_function("clearCounter", [](const int &_counter) { Command<0x0151>(_counter); });
  table.set_function("switchWidescreen", [](const int &_state) { Command<0x02A3>(_state); });
  table.set_function("drawSprite",
                     [](const int &_memorySlot, const float &_offsetLeft, const float &_offsetTop, const float &_width,
                        const float &_height, const int &_r, const int &_g, const int &_b, const int &_a) {
                       Command<0x038D>(_memorySlot, _offsetLeft, _offsetTop, _width, _height, _r, _g, _b, _a);
                     });
  table.set_function("drawRect", [](const float &_x, const float &_y, const float &_width, const float &_height,
                                    const int &_r, const int &_g, const int &_b,
                                    const int &_a) { Command<0x038E>(_x, _y, _width, _height, _r, _g, _b, _a); });
  table.set_function("freezeTimer", [](const int &_state) { Command<0x0396>(_state); });
  table.set_function("displayTimerWithString", [](const int &_timer, const int &_direction, const int &_text) {
    Command<0x03C3>(_timer, _direction, _text);
  });
  table.set_function("displayCounterWithString", [](const int &_counter, const int &_display, const int &_text) {
    Command<0x03C4>(_counter, _display, _text);
  });
  table.set_function("setSpritesDrawBeforeFade", [](const int &_state) { Command<0x03E3>(_state); });
  table.set_function("flashObject", [](const int &_object) { Command<0x03E7>(_object); });
  table.set_function("displayNthCounterWithString",
                     [](const int &_counter, const int &_display, const int &_slot, const int &_text) {
                       Command<0x04F7>(_counter, _display, _slot, _text);
                     });
  table.set_function("displayRadar", [](const int &_state) { Command<0x0581>(_state); });
  table.set_function("setCounterFlashWhenFirstDisplayed",
                     [](const int &_counter, const int &_state) { Command<0x059C>(_counter, _state); });
  table.set_function("drawSpriteWithRotation", [](const int &_memorySlot, const float &_offsetLeft,
                                                  const float &_offsetTop, const float &_width, const float &_height,
                                                  const float &_angle, const int &_red, const int &_green,
                                                  const int &_blue, const int &_alpha) {
    Command<0x074B>(_memorySlot, _offsetLeft, _offsetTop, _width, _height, _angle, _red, _green, _blue, _alpha);
  });
  table.set_function("setRadarZoom", [](const int &_zoom) { Command<0x075B>(_zoom); });
  table.set_function("display", [](const int &_state) { Command<0x0826>(_state); });
  table.set_function("setTimerBeepCountdownTime",
                     [](const int &_timer, const int &_timeInSec) { Command<0x0890>(_timer, _timeInSec); });
  table.set_function("getColor", [](const int &_hudObject) {
    int red_;
    int green_;
    int blue_;
    int alpha_;
    Command<0x0904>(_hudObject, &red_, &green_, &blue_, &alpha_);
    return std::make_tuple(red_, green_, blue_, alpha_);
  });
  table.set_function("drawWindow", [](const float &_leftTopX, const float &_leftTopY, const float &_rightBottomX,
                                      const float &_rightBottomY, const int &_header, const int &_zIndex) {
    Command<0x0937>(_leftTopX, _leftTopY, _rightBottomX, _rightBottomY, _header, _zIndex);
  });
  table.set_function("drawCrosshair", [](const int &_state) { Command<0x09A3>(_state); });
  table.set_function("displayCarNames", [](const int &_state) { Command<0x09B9>(_state); });
  table.set_function("displayZoneNames", [](const int &_state) { Command<0x09BA>(_state); });
  table.set_function("forceBigMessageAndCounter", [](const int &_state) { Command<0x09EE>(_state); });

  // CLEO+
  table.set_function("getFixedXyAspectRatio", [](const float &_x, const float &_y) {
    float x_;
    float y_;
    Command<0x0E0F>(_x, _y, &x_, &y_);
    return std::make_tuple(x_, y_);
  });
  table.set_function("displayTimerLocal",
                     [](const int &_timer, const int &_direction) { Command<0x0E4E>(_timer, _direction); });
  table.set_function("displayTimerWithStringLocal", [](const int &_timer, const int &_direction, const int &_text) {
    Command<0x0E4F>(_timer, _direction, _text);
  });
  table.set_function("displayCounterLocal",
                     [](const int &_timer, const int &_display) { Command<0x0E50>(_timer, _display); });
  table.set_function("displayCounterWithStringLocal", [](const int &_counter, const int &_display, const int &_text) {
    Command<0x0E51>(_counter, _display, _text);
  });
  table.set_function("displayTwoCountersLocal", [](const int &_leftCounter, const int &_rightCounter) {
    Command<0x0E52>(_leftCounter, _rightCounter);
  });
  table.set_function("displayTwoCountersWithStringLocal",
                     [](const int &_leftCounter, const int &_rightCounter, const int &_text) {
                       Command<0x0E53>(_leftCounter, _rightCounter, _text);
                     });
  table.set_function("clearTimerLocal", [](const int &_timer) { Command<0x0E54>(_timer); });
  table.set_function("clearCounterLocal", [](const int &_counter) { Command<0x0E55>(_counter); });
  table.set_function("setCounterFlashWhenFirstDisplayedLocal",
                     [](const int &_counter, const int &_state) { Command<0x0E56>(_counter, _state); });
  table.set_function("setTimerBeepCountdownTimeLocal",
                     [](const int &_timer, const int &_timeInSec) { Command<0x0E57>(_timer, _timeInSec); });
  table.set_function("setCounterColorLocal",
                     [](const int &_counter, const int &_color) { Command<0x0E58>(_counter, _color); });
  table.set_function("isRadarVisible", []() {
    auto result = Command<0x0EB8>();
    return std::make_tuple(result);
  });
  table.set_function("isVisible", []() {
    auto result = Command<0x0EB9>();
    return std::make_tuple(result);
  });
}