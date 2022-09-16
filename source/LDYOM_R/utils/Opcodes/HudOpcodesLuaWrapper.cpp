//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void HudOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("HudOp");
    //default
	table.set_function("displayTimer", [](const int& _timer, const int& _direction) { auto result = Command<0x014E>(_timer, _direction); return std::make_tuple(result);});
	table.set_function("clearTimer", [](const int& _timer) { auto result = Command<0x014F>(_timer); return std::make_tuple(result);});
	table.set_function("clearCounter", [](const int& _counter) { auto result = Command<0x0151>(_counter); return std::make_tuple(result);});
	table.set_function("switchWidescreen", [](const bool& _state) { auto result = Command<0x02A3>(_state); return std::make_tuple(result);});
	table.set_function("drawSprite", [](const int& _memorySlot, const float& _offsetLeft, const float& _offsetTop, const float& _width, const float& _height, const int& _r, const int& _g, const int& _b, const int& _a) { auto result = Command<0x038D>(_memorySlot, _offsetLeft, _offsetTop, _width, _height, _r, _g, _b, _a); return std::make_tuple(result);});
	table.set_function("drawRect", [](const float& _x, const float& _y, const float& _width, const float& _height, const int& _r, const int& _g, const int& _b, const int& _a) { auto result = Command<0x038E>(_x, _y, _width, _height, _r, _g, _b, _a); return std::make_tuple(result);});
	table.set_function("freezeTimer", [](const bool& _state) { auto result = Command<0x0396>(_state); return std::make_tuple(result);});
	table.set_function("displayTimerWithString", [](const int& _timer, const int& _direction, const char* _text) { auto result = Command<0x03C3>(_timer, _direction, _text); return std::make_tuple(result);});
	table.set_function("displayCounterWithString", [](const int& _counter, const int& _display, const char* _text) { auto result = Command<0x03C4>(_counter, _display, _text); return std::make_tuple(result);});
	table.set_function("setSpritesDrawBeforeFade", [](const bool& _state) { auto result = Command<0x03E3>(_state); return std::make_tuple(result);});
	table.set_function("flashObject", [](const int& _object) { auto result = Command<0x03E7>(_object); return std::make_tuple(result);});
	table.set_function("displayNthCounterWithString", [](const int& _counter, const int& _display, const int& _slot, const char* _text) { auto result = Command<0x04F7>(_counter, _display, _slot, _text); return std::make_tuple(result);});
	table.set_function("displayRadar", [](const bool& _state) { auto result = Command<0x0581>(_state); return std::make_tuple(result);});
	table.set_function("setOnscreenCounterFlashWhenFirstDisplayed", [](const int& _counter, const bool& _state) { auto result = Command<0x059C>(_counter, _state); return std::make_tuple(result);});
	table.set_function("drawSpriteWithRotation", [](const int& _memorySlot, const float& _offsetLeft, const float& _offsetTop, const float& _height, const float& _width, const float& _angle, const int& _red, const int& _green, const int& _blue, const int& _alpha) { auto result = Command<0x074B>(_memorySlot, _offsetLeft, _offsetTop, _height, _width, _angle, _red, _green, _blue, _alpha); return std::make_tuple(result);});
	table.set_function("setRadarZoom", [](const int& _zoom) { auto result = Command<0x075B>(_zoom); return std::make_tuple(result);});
	table.set_function("display", [](const bool& _state) { auto result = Command<0x0826>(_state); return std::make_tuple(result);});
	table.set_function("setTimerBeepCountdownTime", [](const int& _timer, const int& _timeInSec) { auto result = Command<0x0890>(_timer, _timeInSec); return std::make_tuple(result);});
	table.set_function("getColor", [](const int& _hudObject) {int red_;int green_;int blue_;int alpha_; auto result = Command<0x0904>(_hudObject, &red_, &green_, &blue_, &alpha_); return std::make_tuple(red_, green_, blue_, alpha_, result);});
	table.set_function("drawWindow", [](const float& _leftTopX, const float& _leftTopY, const float& _rightBottomX, const float& _rightBottomY, const char* _header, const int& _zIndex) { auto result = Command<0x0937>(_leftTopX, _leftTopY, _rightBottomX, _rightBottomY, _header, _zIndex); return std::make_tuple(result);});
	table.set_function("drawCrosshair", [](const bool& _state) { auto result = Command<0x09A3>(_state); return std::make_tuple(result);});
	table.set_function("displayCarNames", [](const bool& _state) { auto result = Command<0x09B9>(_state); return std::make_tuple(result);});
	table.set_function("displayZoneNames", [](const bool& _state) { auto result = Command<0x09BA>(_state); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("getFixedXyAspectRatio", [](const float& _x, const float& _y) {float x_;float y_; auto result = Command<0x0E0F>(_x, _y, &x_, &y_); return std::make_tuple(x_, y_, result);});
	table.set_function("isRadarVisible", []() { auto result = Command<0x0EB8>(); return std::make_tuple(result);});
	table.set_function("isVisible", []() { auto result = Command<0x0EB9>(); return std::make_tuple(result);});

}