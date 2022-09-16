//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void TextOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("TextOp");
    //default
	table.set_function("printBig", [](const char* _key, const int& _time, const int& _style) { auto result = Command<0x00BA>(_key, _time, _style); return std::make_tuple(result);});
	table.set_function("print", [](const char* _key, const int& _time, const int& _flag) { auto result = Command<0x00BB>(_key, _time, _flag); return std::make_tuple(result);});
	table.set_function("printNow", [](const char* _key, const int& _time, const int& _flag) { auto result = Command<0x00BC>(_key, _time, _flag); return std::make_tuple(result);});
	table.set_function("clearPrints", []() { auto result = Command<0x00BE>(); return std::make_tuple(result);});
	table.set_function("printWithNumberBig", [](const char* _key, const int& _num, const int& _duration, const int& _style) { auto result = Command<0x01E3>(_key, _num, _duration, _style); return std::make_tuple(result);});
	table.set_function("printWithNumber", [](const char* _key, const int& _num, const int& _duration, const int& _flag) { auto result = Command<0x01E4>(_key, _num, _duration, _flag); return std::make_tuple(result);});
	table.set_function("printWithNumberNow", [](const char* _key, const int& _num, const int& _duration, const int& _flag) { auto result = Command<0x01E5>(_key, _num, _duration, _flag); return std::make_tuple(result);});
	table.set_function("printBigQ", [](const char* _key, const int& _duration, const int& _style) { auto result = Command<0x0217>(_key, _duration, _style); return std::make_tuple(result);});
	table.set_function("printWith2NumbersNow", [](const char* _key, const int& _num1, const int& _num2, const int& _duration, const int& _style) { auto result = Command<0x02FD>(_key, _num1, _num2, _duration, _style); return std::make_tuple(result);});
	table.set_function("printWith3Numbers", [](const char* _key, const int& _num1, const int& _num2, const int& _num3, const int& _duration, const int& _style) { auto result = Command<0x02FF>(_key, _num1, _num2, _num3, _duration, _style); return std::make_tuple(result);});
	table.set_function("printWith4Numbers", [](const char* _key, const int& _num1, const int& _num2, const int& _num3, const int& _num4, const int& _duration, const int& _style) { auto result = Command<0x0302>(_key, _num1, _num2, _num3, _num4, _duration, _style); return std::make_tuple(result);});
	table.set_function("printWith4NumbersNow", [](const char* _key, const int& _num1, const int& _num2, const int& _num3, const int& _num4, const int& _duration, const int& _style) { auto result = Command<0x0303>(_key, _num1, _num2, _num3, _num4, _duration, _style); return std::make_tuple(result);});
	table.set_function("printWith6Numbers", [](const char* _key, const int& _num1, const int& _num2, const int& _num3, const int& _num4, const int& _num5, const int& _num6, const int& _duration, const int& _style) { auto result = Command<0x0308>(_key, _num1, _num2, _num3, _num4, _num5, _num6, _duration, _style); return std::make_tuple(result);});
	table.set_function("display", [](const float& _offsetLeft, const float& _offsetTop, const char* _key) { auto result = Command<0x033E>(_offsetLeft, _offsetTop, _key); return std::make_tuple(result);});
	table.set_function("setScale", [](const float& _widthScale, const float& _heightScale) { auto result = Command<0x033F>(_widthScale, _heightScale); return std::make_tuple(result);});
	table.set_function("setColor", [](const int& _red, const int& _green, const int& _blue, const int& _alpha) { auto result = Command<0x0340>(_red, _green, _blue, _alpha); return std::make_tuple(result);});
	table.set_function("setJustify", [](const bool& _state) { auto result = Command<0x0341>(_state); return std::make_tuple(result);});
	table.set_function("setCenter", [](const bool& _state) { auto result = Command<0x0342>(_state); return std::make_tuple(result);});
	table.set_function("setWrapX", [](const float& _width) { auto result = Command<0x0343>(_width); return std::make_tuple(result);});
	table.set_function("setCenterSize", [](const float& _width) { auto result = Command<0x0344>(_width); return std::make_tuple(result);});
	table.set_function("setBackground", [](const bool& _state) { auto result = Command<0x0345>(_state); return std::make_tuple(result);});
	table.set_function("setProportional", [](const bool& _state) { auto result = Command<0x0348>(_state); return std::make_tuple(result);});
	table.set_function("setFont", [](const int& _font) { auto result = Command<0x0349>(_font); return std::make_tuple(result);});
	table.set_function("printWith2NumbersBig", [](const char* _key, const int& _num1, const int& _num2, const int& _duration, const int& _style) { auto result = Command<0x036D>(_key, _num1, _num2, _duration, _style); return std::make_tuple(result);});
	table.set_function("printStringInStringNow", [](const char* _templateKey, const char* _replacementKey, const int& _duration, const int& _style) { auto result = Command<0x0384>(_templateKey, _replacementKey, _duration, _style); return std::make_tuple(result);});
	table.set_function("clearThisPrint", [](const char* _key) { auto result = Command<0x03D5>(_key); return std::make_tuple(result);});
	table.set_function("clearThisBigPrint", [](const char* _key) { auto result = Command<0x03D6>(_key); return std::make_tuple(result);});
	table.set_function("setDrawBeforeFade", [](const bool& _state) { auto result = Command<0x03E0>(_state); return std::make_tuple(result);});
	table.set_function("setRightJustify", [](const bool& _state) { auto result = Command<0x03E4>(_state); return std::make_tuple(result);});
	table.set_function("printHelp", [](const char* _key) { auto result = Command<0x03E5>(_key); return std::make_tuple(result);});
	table.set_function("clearHelp", []() { auto result = Command<0x03E6>(); return std::make_tuple(result);});
	table.set_function("clearSmallPrints", []() { auto result = Command<0x03EB>(); return std::make_tuple(result);});
	table.set_function("useCommands", [](const bool& _state) { auto result = Command<0x03F0>(_state); return std::make_tuple(result);});
	table.set_function("displayWithNumber", [](const float& _offsetLeft, const float& _offsetTop, const char* _key, const int& _num) { auto result = Command<0x045A>(_offsetLeft, _offsetTop, _key, _num); return std::make_tuple(result);});
	table.set_function("displayWith2Numbers", [](const float& _offsetLeft, const float& _offsetTop, const char* _key, const int& _num1, const int& _num2) { auto result = Command<0x045B>(_offsetLeft, _offsetTop, _key, _num1, _num2); return std::make_tuple(result);});
	table.set_function("printHelpForever", [](const char* _key) { auto result = Command<0x0512>(_key); return std::make_tuple(result);});
	table.set_function("printHelpForeverWithNumber", [](const char* _gxt, const int& _number) { auto result = Command<0x0513>(_gxt, _number); return std::make_tuple(result);});
	table.set_function("loadMissionText", [](const char* _tableName) { auto result = Command<0x054C>(_tableName); return std::make_tuple(result);});
	table.set_function("setDropshadow", [](const int& _intensity, const int& _red, const int& _green, const int& _blue, const int& _alpha) { auto result = Command<0x060D>(_intensity, _red, _green, _blue, _alpha); return std::make_tuple(result);});
	table.set_function("isMessageBeingDisplayed", []() { auto result = Command<0x076F>(); return std::make_tuple(result);});
	table.set_function("displayWithFloat", [](const float& _leftTopX, const float& _leftTopY, const char* _key, const float& _value, const int& _precision) { auto result = Command<0x07FC>(_leftTopX, _leftTopY, _key, _value, _precision); return std::make_tuple(result);});
	table.set_function("setEdge", [](const int& _size, const int& _red, const int& _green, const int& _blue, const int& _alpha) { auto result = Command<0x081C>(_size, _red, _green, _blue, _alpha); return std::make_tuple(result);});
	table.set_function("isHelpMessageBeingDisplayed", []() { auto result = Command<0x08FE>(); return std::make_tuple(result);});
	table.set_function("setMessageFormatting", [](const bool& __p1, const int& _margin, const int& _width) { auto result = Command<0x0912>(__p1, _margin, _width); return std::make_tuple(result);});
	table.set_function("setHelpMessageBoxSize", [](const int& _size) { auto result = Command<0x0989>(_size); return std::make_tuple(result);});
	table.set_function("addNextMessageToPreviousBriefs", [](const bool& _state) { auto result = Command<0x09C1>(_state); return std::make_tuple(result);});
	table.set_function("forceBigMessageAndCounter", [](const bool& _state) { auto result = Command<0x09EE>(_state); return std::make_tuple(result);});
	table.set_function("getStringWidthWithNumber", [](const char* _gxtEntry, const int& _number) {int width_; auto result = Command<0x0A08>(_gxtEntry, _number, &width_); return std::make_tuple(width_, result);});
	table.set_function("clearThisPrintBigNow", [](const int& _textStyle) { auto result = Command<0x0A0E>(_textStyle); return std::make_tuple(result);});
	table.set_function("setAreaName", [](const char* _name) { auto result = Command<0x0A19>(_name); return std::make_tuple(result);});
	table.set_function("isThisHelpMessageBeingDisplayed", [](const char* _gxt) { auto result = Command<0x0A2A>(_gxt); return std::make_tuple(result);});
	table.set_function("drawSubtitlesBeforeFade", [](const bool& _state) { auto result = Command<0x0A2C>(_state); return std::make_tuple(result);});
	table.set_function("drawOddjobTitleBeforeFade", [](const bool& _state) { auto result = Command<0x0A2D>(_state); return std::make_tuple(result);});
	table.set_function("displayNonMinigameHelpMessages", [](const bool& _state) { auto result = Command<0x0A44>(_state); return std::make_tuple(result);});
	
	//CLEO
	table.set_function("printHelpString", [](const char* _text) { auto result = Command<0x0ACA>(_text); return std::make_tuple(result);});
	table.set_function("printBigString", [](const char* _text, const int& _time, const int& _style) { auto result = Command<0x0ACB>(_text, _time, _style); return std::make_tuple(result);});
	table.set_function("printString", [](const char* _text, const int& _time) { auto result = Command<0x0ACC>(_text, _time); return std::make_tuple(result);});
	table.set_function("printStringNow", [](const char* _text, const int& _time) { auto result = Command<0x0ACD>(_text, _time); return std::make_tuple(result);});
	table.set_function("printHelpFormatted", [](const char* _text, const int& _args) { auto result = Command<0x0ACE>(_text, _args); return std::make_tuple(result);});
	table.set_function("printBigFormatted", [](const char* _text, const int& _time, const int& _style, const int& _args) { auto result = Command<0x0ACF>(_text, _time, _style, _args); return std::make_tuple(result);});
	table.set_function("printFormatted", [](const char* _text, const int& _time, const int& _arg) { auto result = Command<0x0AD0>(_text, _time, _arg); return std::make_tuple(result);});
	table.set_function("printFormattedNow", [](const char* _text, const int& _time, const int& _arg) { auto result = Command<0x0AD1>(_text, _time, _arg); return std::make_tuple(result);});
	table.set_function("stringFormat", [](const char* _format, const int& _args) {const char* text_; auto result = Command<0x0AD3>(_format, _args, (int*)&text_); return std::make_tuple(text_, result);});
	table.set_function("scanString", [](const char* _str, const char* _format, const int& _args) {const char* text_; auto result = Command<0x0AD4>(_str, _format, _args, (int*)&text_); return std::make_tuple(text_, result);});
	table.set_function("getLabelString", [](const char* _key) {const char* text_; auto result = Command<0x0ADE>(_key, (int*)&text_); return std::make_tuple(text_, result);});
	table.set_function("addLabel", [](const char* _dynamicKey, const char* _text) { auto result = Command<0x0ADF>(_dynamicKey, _text); return std::make_tuple(result);});
	table.set_function("removeLabel", [](const char* _dynamicKey) { auto result = Command<0x0AE0>(_dynamicKey); return std::make_tuple(result);});
	table.set_function("stringFloatFormat", [](const float& _number, const char* _format) {const char* text_; auto result = Command<0x0AED>(_number, _format, (int*)&text_); return std::make_tuple(text_, result);});
	
	//CLEO+
	table.set_function("getStringLength", [](const char* _text) {int length_; auto result = Command<0x0D4C>(_text, &length_); return std::make_tuple(length_, result);});

}