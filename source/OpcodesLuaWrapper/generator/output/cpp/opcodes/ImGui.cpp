// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindImGui(sol::state &state) {
  auto table = state.create_table("ImGuiOp");
  // imgui
  table.set_function("end", []() { Command<0x2203>(); });
  table.set_function("checkbox", [](const char *_label, const bool &_isChecked) {
    bool state_;
    Command<0x2215>(_label, _isChecked, (int *)&state_);
    return std::make_tuple(state_);
  });
  table.set_function("button", [](const char *_buttonName, const float &_width, const float &_height) {
    auto result = Command<0x2218>(_buttonName, _width, _height);
    return std::make_tuple(result);
  });
  table.set_function("buttonColored",
                     [](const char *_buttonName, const float &_red, const float &_green, const float &_blue,
                        const float &_alpha, const float &_width, const float &_height) {
                       auto result = Command<0x221B>(_buttonName, _red, _green, _blue, _alpha, _width, _height);
                       return std::make_tuple(result);
                     });
  table.set_function("buttonInvisible", [](const char *_buttonName, const float &_width, const float &_height) {
    auto result = Command<0x221A>(_buttonName, _width, _height);
    return std::make_tuple(result);
  });
  table.set_function("setWindowPos", [](const float &_x, const float &_y, const int &_imGuiCond) {
    Command<0x220A>(_x, _y, _imGuiCond);
  });
  table.set_function("setWindowSize", [](const float &_width, const float &_height, const int &_imGuiCond) {
    Command<0x220B>(_width, _height, _imGuiCond);
  });
  table.set_function("setNextWindowPos", [](const float &_x, const float &_y, const int &_imGuiCond) {
    Command<0x220C>(_x, _y, _imGuiCond);
  });
  table.set_function("setNextWindowSize", [](const float &_width, const float &_height, const int &_imGuiCond) {
    Command<0x220D>(_width, _height, _imGuiCond);
  });
  table.set_function("dummy", [](const float &_width, const float &_height) { Command<0x2226>(_width, _height); });
  table.set_function("text", [](const char *_text) { Command<0x220E>(_text); });
  table.set_function("textDisabled", [](const char *_text) { Command<0x2210>(_text); });
  table.set_function("textWrapped", [](const char *_text) { Command<0x2211>(_text); });
  table.set_function("textColored", [](const char *_text, const float &_red, const float &_green, const float &_blue,
                                       const float &_alpha) { Command<0x2212>(_text, _red, _green, _blue, _alpha); });
  table.set_function("textWithBullet", [](const char *_text) { Command<0x2213>(_text); });
  table.set_function("setTooltip", [](const char *_text) { Command<0x2217>(_text); });
  table.set_function("sameLine", []() { Command<0x2227>(); });
  table.set_function("newLine", []() { Command<0x2228>(); });
  table.set_function("spacing", []() { Command<0x222B>(); });
  table.set_function("separator", []() { Command<0x222C>(); });
  table.set_function("nextColumn", []() { Command<0x222A>(); });
  table.set_function("columns", [](const int &_count) { Command<0x2229>(_count); });
  table.set_function("getVersion", []() {
    const char *version_;
    Command<0x2245>((int *)&version_);
    return std::make_tuple(version_);
  });
  table.set_function("getPluginVersion", []() {
    float version_;
    Command<0x2246>(&version_);
    return std::make_tuple(version_);
  });
  table.set_function("setCursorVisible", [](const bool &_show) { Command<0x2247>(_show); });
  table.set_function("getDisplaySize", []() {
    float width_;
    float height_;
    Command<0x224E>(&width_, &height_);
    return std::make_tuple(width_, height_);
  });
  table.set_function("getFrameHeight", []() {
    float height_;
    Command<0x2248>(&height_);
    return std::make_tuple(height_);
  });
  table.set_function("getWindowPos", [](const char *_uniqueId) {
    float x_;
    float y_;
    Command<0x2249>(_uniqueId, &x_, &y_);
    return std::make_tuple(x_, y_);
  });
  table.set_function("getWindowSize", [](const char *_uniqueId) {
    float width_;
    float height_;
    Command<0x224A>(_uniqueId, &width_, &height_);
    return std::make_tuple(width_, height_);
  });
  table.set_function("calcTextSize", [](const char *_text) {
    float width_;
    float height_;
    Command<0x224B>(_text, &width_, &height_);
    return std::make_tuple(width_, height_);
  });
  table.set_function("getWindowContentRegionWidth", [](const char *_uniqueId) {
    float width_;
    Command<0x224C>(_uniqueId, &width_);
    return std::make_tuple(width_);
  });
  table.set_function("beginMainMenuBar", [](const char *_uniqueId) { Command<0x2204>(_uniqueId); });
  table.set_function("endMainMenuBar", []() { Command<0x2205>(); });
  table.set_function("menuItem", [](const char *_text, const bool &_selected, const bool &_enabled) {
    auto result = Command<0x2224>(_text, _selected, _enabled);
    return std::make_tuple(result);
  });
  table.set_function("selectable", [](const char *_text, const bool &_selected) {
    auto result = Command<0x2225>(_text, _selected);
    return std::make_tuple(result);
  });
  table.set_function("beginChild", [](const char *_uniqueId) { Command<0x2206>(_uniqueId); });
  table.set_function("endChild", []() { Command<0x2207>(); });
  table.set_function("pushItemWidth", [](const float &_width) { Command<0x222D>(_width); });
  table.set_function("popItemWidth", []() { Command<0x222E>(); });
  table.set_function("collapsingHeader", [](const char *_label) {
    auto result = Command<0x2209>(_label);
    return std::make_tuple(result);
  });
  table.set_function("sliderInt", [](const char *_label, const int &_initValue, const int &_min, const int &_max) {
    int val_;
    Command<0x221D>(_label, _initValue, _min, _max, &val_);
    return std::make_tuple(val_);
  });
  table.set_function("sliderFloat",
                     [](const char *_label, const float &_initValue, const float &_min, const float &_max) {
                       float val_;
                       Command<0x221E>(_label, _initValue, _min, _max, &val_);
                       return std::make_tuple(val_);
                     });
  table.set_function("inputFloat",
                     [](const char *_label, const float &_initValue, const float &_min, const float &_max) {
                       float val_;
                       Command<0x2220>(_label, _initValue, _min, _max, &val_);
                       return std::make_tuple(val_);
                     });
  table.set_function("inputInt", [](const char *_label, const int &_initValue, const int &_min, const int &_max) {
    int val_;
    Command<0x221F>(_label, _initValue, _min, _max, &val_);
    return std::make_tuple(val_);
  });
  table.set_function("inputText", [](const char *_label) {
    const char *text_;
    Command<0x2221>(_label, (int *)&text_);
    return std::make_tuple(text_);
  });
  table.set_function("radioButton", [](const char *_label, const int &_selectedBtn, const int &_btnNo) {
    int val_;
    Command<0x2222>(_label, _selectedBtn, _btnNo, &val_);
    return std::make_tuple(val_);
  });
  table.set_function("colorPicker", [](const char *_label) {
    int red_;
    int green_;
    int blue_;
    int alpha_;
    Command<0x2223>(_label, &red_, &green_, &blue_, &alpha_);
    return std::make_tuple(red_, green_, blue_, alpha_);
  });
  table.set_function("beginFrame", [](const char *_uniqueId) { Command<0x2200>(_uniqueId); });
  table.set_function("endFrame", []() { Command<0x2201>(); });
  table.set_function("isItemActive", [](const char *_uniqueId) {
    auto result = Command<0x222F>(_uniqueId);
    return std::make_tuple(result);
  });
  table.set_function("isItemClicked", [](const char *_uniqueId) {
    auto result = Command<0x2230>(_uniqueId);
    return std::make_tuple(result);
  });
  table.set_function("isItemFocused", [](const char *_uniqueId) {
    auto result = Command<0x2231>(_uniqueId);
    return std::make_tuple(result);
  });
  table.set_function("isItemHovered", [](const char *_uniqueId) {
    auto result = Command<0x2232>(_uniqueId);
    return std::make_tuple(result);
  });
  table.set_function("getScalingSize", [](const char *_uniqueId, const int &_count, const bool &_spacing) {
    float x_;
    float y_;
    Command<0x224D>(_uniqueId, _count, _spacing, &x_, &y_);
    return std::make_tuple(x_, y_);
  });
  table.set_function("setNextWindowTransparency", [](const float &_alpha) { Command<0x224F>(_alpha); });
  table.set_function("setMessage", [](const char *_text) { Command<0x2250>(_text); });
  table.set_function("buttonArrow", [](const char *_name, const int &_imGuiDir) {
    auto result = Command<0x221C>(_name, _imGuiDir);
    return std::make_tuple(result);
  });
  table.set_function("bullet", []() { Command<0x2214>(); });
  table.set_function("comboBox", [](const char *_name, const char *_options, const int &_selection) {
    int selected_;
    Command<0x2216>(_name, _options, _selection, &selected_);
    return std::make_tuple(selected_);
  });
  table.set_function("buttonImage",
                     [](const char *_name, const int &_image, const float &_width, const float &_height) {
                       auto result = Command<0x2219>(_name, _image, _width, _height);
                       return std::make_tuple(result);
                     });
  table.set_function("loadImage", [](const char *_path) {
    int image_;
    Command<0x2238>(_path, &image_);
    return std::make_tuple(image_);
  });
  table.set_function("freeImage", [](const int &_image) { Command<0x2239>(_image); });
  table.set_function("textCentered", [](const char *_text) { Command<0x220F>(_text); });
  table.set_function("pushStyleVar",
                     [](const int &_imGuiStyleVar, const float &_val) { Command<0x223A>(_imGuiStyleVar, _val); });
  table.set_function("pushStyleVar2", [](const int &_imGuiStyleVar, const float &_x, const float &_y) {
    Command<0x223B>(_imGuiStyleVar, _x, _y);
  });
  table.set_function("pushStyleColor", [](const int &_imGuiCol, const int &_r, const int &_g, const int &_b,
                                          const int &_a) { Command<0x223C>(_imGuiCol, _r, _g, _b, _a); });
  table.set_function("popStyleVar", [](const int &_count) { Command<0x223D>(_count); });
  table.set_function("popStyleColor", [](const int &_count) { Command<0x223E>(_count); });
  table.set_function("tabs", [](const char *_name, const char *_tabNames) {
    int index_;
    Command<0x2208>(_name, _tabNames, &index_);
    return std::make_tuple(index_);
  });
  table.set_function("setImageBgColor", [](const float &_r, const float &_g, const float &_b, const float &_a) {
    Command<0x2236>(_r, _g, _b, _a);
  });
  table.set_function("setImageTintColor", [](const float &_r, const float &_g, const float &_b, const float &_a) {
    Command<0x2237>(_r, _g, _b, _a);
  });
  table.set_function("getForegroundDrawList", []() {
    int drawList_;
    Command<0x223F>(&drawList_);
    return std::make_tuple(drawList_);
  });
  table.set_function("getBackgroundDrawList", []() {
    int drawList_;
    Command<0x2240>(&drawList_);
    return std::make_tuple(drawList_);
  });
  table.set_function("getWindowDrawlist", []() {
    int drawList_;
    Command<0x2241>(&drawList_);
    return std::make_tuple(drawList_);
  });
  table.set_function("addText", [](const int &_drawList, const float &_posX, const float &_posY, const int &_r,
                                   const int &_g, const int &_b, const int &_a, const char *_text) {
    Command<0x2242>(_drawList, _posX, _posY, _r, _g, _b, _a, _text);
  });
  table.set_function("addLine", [](const int &_drawList, const float &_p1X, const float &_p1Y, const float &_p2X,
                                   const float &_p2Y, const int &_r, const int &_g, const int &_b, const int &_a,
                                   const float &_thickness) {
    Command<0x2243>(_drawList, _p1X, _p1Y, _p2X, _p2Y, _r, _g, _b, _a, _thickness);
  });
  table.set_function("setItemValueInt", [](const char *_id, const int &_val) { Command<0x2233>(_id, _val); });
  table.set_function("setItemValueFloat", [](const char *_id, const float &_val) { Command<0x2234>(_id, _val); });
  table.set_function("setItemValueText", [](const char *_id, const char *_val) { Command<0x2235>(_id, _val); });
  table.set_function("begin", [](const char *_windowName, const bool &_state, const bool &_noTitleBar,
                                 const bool &_noResize, const bool &_noMove, const bool &_autoResize) {
    bool state_;
    Command<0x2202>(_windowName, _state, _noTitleBar, _noResize, _noMove, _autoResize, (int *)&state_);
    return std::make_tuple(state_);
  });
}