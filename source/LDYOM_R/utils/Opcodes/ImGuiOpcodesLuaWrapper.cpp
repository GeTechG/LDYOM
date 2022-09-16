//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ImGuiOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ImGuiOp");
    //imgui
	table.set_function("begin", [](const char* _windowName, const bool& _state, const bool& _noTitleBar, const bool& _noResize, const bool& _noMove, const bool& _autoResize) {bool state_; auto result = Command<0x0C00>(_windowName, _state, _noTitleBar, _noResize, _noMove, _autoResize, (int*)&state_); return std::make_tuple(state_, result);});
	table.set_function("end", []() { auto result = Command<0x0C01>(); return std::make_tuple(result);});
	table.set_function("checkbox", [](const char* _label, const bool& _isChecked) {bool isChecked_; auto result = Command<0x0C02>(_label, _isChecked, (int*)&isChecked_); return std::make_tuple(isChecked_, result);});
	table.set_function("button", [](const char* _buttonName, const float& _width, const float& _height) { auto result = Command<0x0C03>(_buttonName, _width, _height); return std::make_tuple(result);});
	table.set_function("buttonColored", [](const char* _buttonName, const float& _red, const float& _green, const float& _blue, const float& _alpha, const float& _width, const float& _height) { auto result = Command<0x0C04>(_buttonName, _red, _green, _blue, _alpha, _width, _height); return std::make_tuple(result);});
	table.set_function("buttonInvisible", [](const char* _buttonName, const float& _width, const float& _height) { auto result = Command<0x0C05>(_buttonName, _width, _height); return std::make_tuple(result);});
	table.set_function("setWindowPos", [](const float& _x, const float& _y, const int& _imGuiCond) { auto result = Command<0x0C06>(_x, _y, _imGuiCond); return std::make_tuple(result);});
	table.set_function("setWindowSize", [](const float& _width, const float& _height, const int& _imGuiCond) { auto result = Command<0x0C07>(_width, _height, _imGuiCond); return std::make_tuple(result);});
	table.set_function("setNextWindowPos", [](const float& _x, const float& _y, const int& _imGuiCond) { auto result = Command<0x0C08>(_x, _y, _imGuiCond); return std::make_tuple(result);});
	table.set_function("setNextWindowSize", [](const float& _width, const float& _height, const int& _imGuiCond) { auto result = Command<0x0C09>(_width, _height, _imGuiCond); return std::make_tuple(result);});
	table.set_function("dummy", [](const float& _width, const float& _height) { auto result = Command<0x0C0A>(_width, _height); return std::make_tuple(result);});
	table.set_function("text", [](const char* _text) { auto result = Command<0x0C0B>(_text); return std::make_tuple(result);});
	table.set_function("textDisabled", [](const char* _text) { auto result = Command<0x0C0C>(_text); return std::make_tuple(result);});
	table.set_function("textWrapped", [](const char* _text) { auto result = Command<0x0C0D>(_text); return std::make_tuple(result);});
	table.set_function("textColored", [](const char* _text, const float& _red, const float& _green, const float& _blue, const float& _alpha) { auto result = Command<0x0C0E>(_text, _red, _green, _blue, _alpha); return std::make_tuple(result);});
	table.set_function("textWithBullet", [](const char* _text) { auto result = Command<0x0C0F>(_text); return std::make_tuple(result);});
	table.set_function("setTooltip", [](const char* _text) { auto result = Command<0x0C10>(_text); return std::make_tuple(result);});
	table.set_function("sameLine", []() { auto result = Command<0x0C11>(); return std::make_tuple(result);});
	table.set_function("newLine", []() { auto result = Command<0x0C12>(); return std::make_tuple(result);});
	table.set_function("spacing", []() { auto result = Command<0x0C13>(); return std::make_tuple(result);});
	table.set_function("separator", []() { auto result = Command<0x0C14>(); return std::make_tuple(result);});
	table.set_function("nextColumn", []() { auto result = Command<0x0C15>(); return std::make_tuple(result);});
	table.set_function("columns", [](const int& _count) { auto result = Command<0x0C16>(_count); return std::make_tuple(result);});
	table.set_function("getVersion", []() {const char* version_; auto result = Command<0x0C18>((int*)&version_); return std::make_tuple(version_, result);});
	table.set_function("getPluginVersion", []() {float version_; auto result = Command<0x0C19>(&version_); return std::make_tuple(version_, result);});
	table.set_function("setCursorVisible", [](const bool& _show) { auto result = Command<0x0C1A>(_show); return std::make_tuple(result);});
	table.set_function("getDisplaySize", []() {float width_;float height_; auto result = Command<0x0C1B>(&width_, &height_); return std::make_tuple(width_, height_, result);});
	table.set_function("getFrameHeight", []() {float height_; auto result = Command<0x0C1C>(&height_); return std::make_tuple(height_, result);});
	table.set_function("getWindowPos", [](const char* _uniqueId) {float x_;float y_; auto result = Command<0x0C1D>(_uniqueId, &x_, &y_); return std::make_tuple(x_, y_, result);});
	table.set_function("getWindowSize", [](const char* _uniqueId) {float width_;float height_; auto result = Command<0x0C1E>(_uniqueId, &width_, &height_); return std::make_tuple(width_, height_, result);});
	table.set_function("calcTextSize", [](const char* _text) {float width_;float height_; auto result = Command<0x0C1F>(_text, &width_, &height_); return std::make_tuple(width_, height_, result);});
	table.set_function("getWindowContentRegionWidth", [](const char* _uniqueId) {float width_; auto result = Command<0x0C20>(_uniqueId, &width_); return std::make_tuple(width_, result);});
	table.set_function("beginMainMenuBar", [](const char* _uniqueId) {bool active_; auto result = Command<0x0C21>(_uniqueId, (int*)&active_); return std::make_tuple(active_, result);});
	table.set_function("endMainMenuBar", []() { auto result = Command<0x0C22>(); return std::make_tuple(result);});
	table.set_function("menuItem", [](const char* _text, const bool& _selected, const bool& _enabled) {bool clicked_; auto result = Command<0x0C23>(_text, _selected, _enabled, (int*)&clicked_); return std::make_tuple(clicked_, result);});
	table.set_function("selectable", [](const char* _text, const bool& _selected) {bool clicked_; auto result = Command<0x0C24>(_text, _selected, (int*)&clicked_); return std::make_tuple(clicked_, result);});
	table.set_function("beginChild", [](const char* _uniqueId) { auto result = Command<0x0C25>(_uniqueId); return std::make_tuple(result);});
	table.set_function("endChild", []() { auto result = Command<0x0C26>(); return std::make_tuple(result);});
	table.set_function("pushItemWidth", [](const float& _width) { auto result = Command<0x0C27>(_width); return std::make_tuple(result);});
	table.set_function("popItemWidth", []() { auto result = Command<0x0C28>(); return std::make_tuple(result);});
	table.set_function("collapsingHeader", [](const char* _label) {bool open_; auto result = Command<0x0C29>(_label, (int*)&open_); return std::make_tuple(open_, result);});
	table.set_function("sliderInt", [](const char* _label, const int& _initValue, const int& _min, const int& _max) {int val_; auto result = Command<0x0C2A>(_label, _initValue, _min, _max, &val_); return std::make_tuple(val_, result);});
	table.set_function("sliderFloat", [](const char* _label, const float& _initValue, const float& _min, const float& _max) {float val_; auto result = Command<0x0C2B>(_label, _initValue, _min, _max, &val_); return std::make_tuple(val_, result);});
	table.set_function("inputFloat", [](const char* _label, const float& _initValue, const float& _min, const float& _max) {float val_; auto result = Command<0x0C2C>(_label, _initValue, _min, _max, &val_); return std::make_tuple(val_, result);});
	table.set_function("inputInt", [](const char* _label, const int& _initValue, const int& _min, const int& _max) {int val_; auto result = Command<0x0C2D>(_label, _initValue, _min, _max, &val_); return std::make_tuple(val_, result);});
	table.set_function("inputText", [](const char* _label) {const char* text_; auto result = Command<0x0C2E>(_label, (int*)&text_); return std::make_tuple(text_, result);});
	table.set_function("radioButton", [](const char* _label, const int& _selectedBtn, const int& _btnNo) {int val_; auto result = Command<0x0C2F>(_label, _selectedBtn, _btnNo, &val_); return std::make_tuple(val_, result);});
	table.set_function("colorPicker", [](const char* _label) {float red_;float green_;float blue_;float alpha_; auto result = Command<0x0C30>(_label, &red_, &green_, &blue_, &alpha_); return std::make_tuple(red_, green_, blue_, alpha_, result);});
	table.set_function("beginFrame", [](const char* _uniqueId) { auto result = Command<0x0C31>(_uniqueId); return std::make_tuple(result);});
	table.set_function("endFrame", []() { auto result = Command<0x0C32>(); return std::make_tuple(result);});
	table.set_function("isItemActive", [](const char* _uniqueId) {bool state_; auto result = Command<0x0C33>(_uniqueId, (int*)&state_); return std::make_tuple(state_, result);});
	table.set_function("isItemClicked", [](const char* _uniqueId) {bool state_; auto result = Command<0x0C34>(_uniqueId, (int*)&state_); return std::make_tuple(state_, result);});
	table.set_function("isItemFocused", [](const char* _uniqueId) {bool state_; auto result = Command<0x0C35>(_uniqueId, (int*)&state_); return std::make_tuple(state_, result);});
	table.set_function("isItemHovered", [](const char* _uniqueId) {bool state_; auto result = Command<0x0C36>(_uniqueId, (int*)&state_); return std::make_tuple(state_, result);});
	table.set_function("getScalingSize", [](const char* _uniqueId, const int& _count, const bool& _spacing) {float x_;float y_; auto result = Command<0x0C37>(_uniqueId, _count, _spacing, &x_, &y_); return std::make_tuple(x_, y_, result);});
	table.set_function("setNextWindowTransparency", [](const float& _alpha) { auto result = Command<0x0C38>(_alpha); return std::make_tuple(result);});
	table.set_function("setMessage", [](const char* _text) { auto result = Command<0x0C39>(_text); return std::make_tuple(result);});
	table.set_function("buttonArrow", [](const char* _name, const int& _imGuiDir) { auto result = Command<0x0C3A>(_name, _imGuiDir); return std::make_tuple(result);});
	table.set_function("bullet", []() { auto result = Command<0x0C3B>(); return std::make_tuple(result);});
	table.set_function("comboBox", [](const char* _name, const char* _options, const int& _selection) {int selected_; auto result = Command<0x0C3C>(_name, _options, _selection, &selected_); return std::make_tuple(selected_, result);});
	table.set_function("buttonImage", [](const char* _name, const int& _image, const float& _width, const float& _height) { auto result = Command<0x0C3D>(_name, _image, _width, _height); return std::make_tuple(result);});
	table.set_function("loadImage", [](const char* _path) {int image_; auto result = Command<0x0C3E>(_path, &image_); return std::make_tuple(image_, result);});
	table.set_function("freeImage", [](const int& _image) { auto result = Command<0x0C3F>(_image); return std::make_tuple(result);});
	table.set_function("textCentered", [](const char* _text) { auto result = Command<0x0C40>(_text); return std::make_tuple(result);});
	table.set_function("pushStyleVar", [](const int& _imGuiStyleVar, const float& _val) { auto result = Command<0x0C41>(_imGuiStyleVar, _val); return std::make_tuple(result);});
	table.set_function("pushStyleVar2", [](const int& _imGuiStyleVar, const float& _x, const float& _y) { auto result = Command<0x0C42>(_imGuiStyleVar, _x, _y); return std::make_tuple(result);});
	table.set_function("pushStyleColor", [](const int& _imGuiCol, const int& _r, const int& _g, const int& _b, const int& _a) { auto result = Command<0x0C43>(_imGuiCol, _r, _g, _b, _a); return std::make_tuple(result);});
	table.set_function("popStyleVar", [](const int& _count) { auto result = Command<0x0C44>(_count); return std::make_tuple(result);});
	table.set_function("popStyleColor", [](const int& _count) { auto result = Command<0x0C45>(_count); return std::make_tuple(result);});
	table.set_function("tabs", [](const char* _name, const char* _tabNames) {int index_; auto result = Command<0x0C46>(_name, _tabNames, &index_); return std::make_tuple(index_, result);});

}