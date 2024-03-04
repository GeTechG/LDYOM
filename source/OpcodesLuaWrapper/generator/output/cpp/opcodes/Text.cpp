// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindText(sol::state &state) {
  auto table = state.create_table("TextOp");
  // default
  table.set_function(
      "printBig", [](const char *_key, const int &_time, const int &_style) { Command<0x00BA>(_key, _time, _style); });
  table.set_function("print",
                     [](const char *_key, const int &_time, const int &_flag) { Command<0x00BB>(_key, _time, _flag); });
  table.set_function("printNow",
                     [](const char *_key, const int &_time, const int &_flag) { Command<0x00BC>(_key, _time, _flag); });
  table.set_function("clearPrints", []() { Command<0x00BE>(); });
  table.set_function("printWithNumberBig", [](const char *_key, const int &_num, const int &_duration,
                                              const int &_style) { Command<0x01E3>(_key, _num, _duration, _style); });
  table.set_function("printWithNumber", [](const char *_key, const int &_num, const int &_duration, const int &_flag) {
    Command<0x01E4>(_key, _num, _duration, _flag);
  });
  table.set_function("printWithNumberNow", [](const char *_key, const int &_num, const int &_duration,
                                              const int &_flag) { Command<0x01E5>(_key, _num, _duration, _flag); });
  table.set_function("printBigQ", [](const char *_key, const int &_duration, const int &_style) {
    Command<0x0217>(_key, _duration, _style);
  });
  table.set_function("printWith2NumbersNow",
                     [](const char *_key, const int &_num1, const int &_num2, const int &_duration, const int &_style) {
                       Command<0x02FD>(_key, _num1, _num2, _duration, _style);
                     });
  table.set_function("printWith3Numbers",
                     [](const char *_key, const int &_num1, const int &_num2, const int &_num3, const int &_duration,
                        const int &_style) { Command<0x02FF>(_key, _num1, _num2, _num3, _duration, _style); });
  table.set_function("printWith4Numbers", [](const char *_key, const int &_num1, const int &_num2, const int &_num3,
                                             const int &_num4, const int &_duration, const int &_style) {
    Command<0x0302>(_key, _num1, _num2, _num3, _num4, _duration, _style);
  });
  table.set_function("printWith4NumbersNow", [](const char *_key, const int &_num1, const int &_num2, const int &_num3,
                                                const int &_num4, const int &_duration, const int &_style) {
    Command<0x0303>(_key, _num1, _num2, _num3, _num4, _duration, _style);
  });
  table.set_function("printWith6Numbers",
                     [](const char *_key, const int &_num1, const int &_num2, const int &_num3, const int &_num4,
                        const int &_num5, const int &_num6, const int &_duration, const int &_style) {
                       Command<0x0308>(_key, _num1, _num2, _num3, _num4, _num5, _num6, _duration, _style);
                     });
  table.set_function("display", [](const float &_offsetLeft, const float &_offsetTop, const char *_key) {
    Command<0x033E>(_offsetLeft, _offsetTop, _key);
  });
  table.set_function("setScale", [](const float &_widthScale, const float &_heightScale) {
    Command<0x033F>(_widthScale, _heightScale);
  });
  table.set_function("setColor", [](const int &_red, const int &_green, const int &_blue, const int &_alpha) {
    Command<0x0340>(_red, _green, _blue, _alpha);
  });
  table.set_function("setJustify", [](const bool &_state) { Command<0x0341>(_state); });
  table.set_function("setCenter", [](const bool &_state) { Command<0x0342>(_state); });
  table.set_function("setWrapX", [](const float &_width) { Command<0x0343>(_width); });
  table.set_function("setCenterSize", [](const float &_width) { Command<0x0344>(_width); });
  table.set_function("setBackground", [](const bool &_state) { Command<0x0345>(_state); });
  table.set_function("setProportional", [](const bool &_state) { Command<0x0348>(_state); });
  table.set_function("setFont", [](const int &_font) { Command<0x0349>(_font); });
  table.set_function("printWith2NumbersBig",
                     [](const char *_key, const int &_num1, const int &_num2, const int &_duration, const int &_style) {
                       Command<0x036D>(_key, _num1, _num2, _duration, _style);
                     });
  table.set_function("printStringInStringNow",
                     [](const char *_templateKey, const char *_replacementKey, const int &_duration,
                        const int &_style) { Command<0x0384>(_templateKey, _replacementKey, _duration, _style); });
  table.set_function("clearThisPrint", [](const char *_key) { Command<0x03D5>(_key); });
  table.set_function("clearThisBigPrint", [](const char *_key) { Command<0x03D6>(_key); });
  table.set_function("setDrawBeforeFade", [](const bool &_state) { Command<0x03E0>(_state); });
  table.set_function("setRightJustify", [](const bool &_state) { Command<0x03E4>(_state); });
  table.set_function("printHelp", [](const char *_key) { Command<0x03E5>(_key); });
  table.set_function("clearHelp", []() { Command<0x03E6>(); });
  table.set_function("clearSmallPrints", []() { Command<0x03EB>(); });
  table.set_function("useCommands", [](const bool &_state) { Command<0x03F0>(_state); });
  table.set_function("displayWithNumber",
                     [](const float &_offsetLeft, const float &_offsetTop, const char *_key, const int &_num) {
                       Command<0x045A>(_offsetLeft, _offsetTop, _key, _num);
                     });
  table.set_function("displayWith2Numbers",
                     [](const float &_offsetLeft, const float &_offsetTop, const char *_key, const int &_num1,
                        const int &_num2) { Command<0x045B>(_offsetLeft, _offsetTop, _key, _num1, _num2); });
  table.set_function("printHelpForever", [](const char *_key) { Command<0x0512>(_key); });
  table.set_function("printHelpForeverWithNumber",
                     [](const char *_gxt, const int &_number) { Command<0x0513>(_gxt, _number); });
  table.set_function("loadMissionText", [](const char *_tableName) { Command<0x054C>(_tableName); });
  table.set_function("setDropshadow",
                     [](const int &_intensity, const int &_red, const int &_green, const int &_blue,
                        const int &_alpha) { Command<0x060D>(_intensity, _red, _green, _blue, _alpha); });
  table.set_function("isMessageBeingDisplayed", []() {
    auto result = Command<0x076F>();
    return std::make_tuple(result);
  });
  table.set_function("displayWithFloat",
                     [](const float &_leftTopX, const float &_leftTopY, const char *_key, const float &_value,
                        const int &_precision) { Command<0x07FC>(_leftTopX, _leftTopY, _key, _value, _precision); });
  table.set_function("setEdge", [](const int &_size, const int &_red, const int &_green, const int &_blue,
                                   const int &_alpha) { Command<0x081C>(_size, _red, _green, _blue, _alpha); });
  table.set_function("isEmpty", [](const char *_text) {
    auto result = Command<0x0844>(_text);
    return std::make_tuple(result);
  });
  table.set_function("isEmpty", [](const char *_text) {
    auto result = Command<0x0845>(_text);
    return std::make_tuple(result);
  });
  table.set_function("isEmpty", [](const char *_text) {
    auto result = Command<0x0846>(_text);
    return std::make_tuple(result);
  });
  table.set_function("isEmpty", [](const char *_text) {
    auto result = Command<0x0847>(_text);
    return std::make_tuple(result);
  });
  table.set_function("isHelpMessageBeingDisplayed", []() {
    auto result = Command<0x08FE>();
    return std::make_tuple(result);
  });
  table.set_function("setMessageFormatting", [](const bool &__p1, const int &_margin, const int &_width) {
    Command<0x0912>(__p1, _margin, _width);
  });
  table.set_function("setHelpMessageBoxSize", [](const int &_size) { Command<0x0989>(_size); });
  table.set_function("getHashKey", [](const char *_text) {
    int hash_;
    Command<0x09A9>(_text, &hash_);
    return std::make_tuple(hash_);
  });
  table.set_function("addNextMessageToPreviousBriefs", [](const bool &_state) { Command<0x09C1>(_state); });
  table.set_function("getStringWidth", [](const char *_entry) {
    int width_;
    Command<0x09FD>(_entry, &width_);
    return std::make_tuple(width_);
  });
  table.set_function("getStringWidthWithNumber", [](const char *_gxtEntry, const int &_number) {
    int width_;
    Command<0x0A08>(_gxtEntry, _number, &width_);
    return std::make_tuple(width_);
  });
  table.set_function("clearThisPrintBigNow", [](const int &_textStyle) { Command<0x0A0E>(_textStyle); });
  table.set_function("setAreaName", [](const char *_name) { Command<0x0A19>(_name); });
  table.set_function("isThisHelpMessageBeingDisplayed", [](const char *_gxt) {
    auto result = Command<0x0A2A>(_gxt);
    return std::make_tuple(result);
  });
  table.set_function("drawSubtitlesBeforeFade", [](const bool &_state) { Command<0x0A2C>(_state); });
  table.set_function("drawOddjobTitleBeforeFade", [](const bool &_state) { Command<0x0A2D>(_state); });
  table.set_function("displayNonMinigameHelpMessages", [](const bool &_state) { Command<0x0A44>(_state); });

  // CLEO
  table.set_function("printHelpString", [](const char *_text) { Command<0x0ACA>(_text); });
  table.set_function("printBigString", [](const char *_text, const int &_time, const int &_style) {
    Command<0x0ACB>(_text, _time, _style);
  });
  table.set_function("printString", [](const char *_text, const int &_time) { Command<0x0ACC>(_text, _time); });
  table.set_function("printStringNow", [](const char *_text, const int &_time) { Command<0x0ACD>(_text, _time); });
  table.set_function("printHelpFormatted", [](const char *_text, const int &_args) { Command<0x0ACE>(_text, _args); });
  table.set_function("printBigFormatted", [](const char *_fmt, const int &_time, const int &_style, const int &_args) {
    Command<0x0ACF>(_fmt, _time, _style, _args);
  });
  table.set_function("printFormatted",
                     [](const char *_fmt, const int &_time, const int &_arg) { Command<0x0AD0>(_fmt, _time, _arg); });
  table.set_function("printFormattedNow",
                     [](const char *_fmt, const int &_time, const int &_arg) { Command<0x0AD1>(_fmt, _time, _arg); });
  table.set_function("stringFormat", [](const int &_buffer, const char *_format, const int &_args) {
    Command<0x0AD3>(_buffer, _format, _args);
  });
  table.set_function("scanString", [](const char *_string, const char *_format) {
    int nValues_;
    int values_;
    auto result = Command<0x0AD4>(_string, _format, &nValues_, &values_);
    return std::make_tuple(result, nValues_, values_);
  });
  table.set_function("getLabelString", [](const char *_key) {
    const char *text_;
    Command<0x0ADE>(_key, (int *)&text_);
    return std::make_tuple(text_);
  });
  table.set_function("addLabel",
                     [](const char *_dynamicKey, const char *_text) { Command<0x0ADF>(_dynamicKey, _text); });
  table.set_function("removeLabel", [](const char *_key) { Command<0x0AE0>(_key); });
  table.set_function("stringFloatFormat", [](const float &_number, const char *_format) {
    const char *text_;
    Command<0x0AED>(_number, _format, (int *)&text_);
    return std::make_tuple(text_);
  });

  // CLEO+
  table.set_function("getStringLength", [](const char *_text) {
    int length_;
    Command<0x0D4C>(_text, &length_);
    return std::make_tuple(length_);
  });
  table.set_function("copyString",
                     [](const char *_string, const int &_address) { Command<0x0D4D>(_string, _address); });
  table.set_function("drawString",
                     [](const char *_string, const int &_drawEvent, const float &_posX, const float &_posY,
                        const float &_sizeX, const float &_sizeY, const bool &_fixAr, const int &_font) {
                       Command<0x0E62>(_string, _drawEvent, _posX, _posY, _sizeX, _sizeY, _fixAr, _font);
                     });
  table.set_function("drawStringExt", [](const char *_string, const int &_drawEvent, const float &_posX,
                                         const float &_posY, const float &_sizeX, const float &_sizeY,
                                         const bool &_fixAr, const int &_font, const bool &_prop, const int &_align,
                                         const float &_wrap, const bool &_justify, const int &_red, const int &_green,
                                         const int &_blue, const int &_alpha, const int &_edge, const int &_shadow,
                                         const int &_dropRed, const int &_dropGreen, const int &_dropBlue,
                                         const int &_dropAlpha, const bool &_background, const int &_backRed,
                                         const int &_backGreen, const int &_backBlue, const int &_backAlpha) {
    Command<0x0E63>(_string, _drawEvent, _posX, _posY, _sizeX, _sizeY, _fixAr, _font, _prop, _align, _wrap, _justify,
                    _red, _green, _blue, _alpha, _edge, _shadow, _dropRed, _dropGreen, _dropBlue, _dropAlpha,
                    _background, _backRed, _backGreen, _backBlue, _backAlpha);
  });
  table.set_function("isStringEqual", [](const char *_string1, const char *_string2, const int &_maxSize,
                                         const bool &_caseSensitive, const char *_ignoreCharacter) {
    auto result = Command<0x0E80>(_string1, _string2, _maxSize, _caseSensitive, _ignoreCharacter);
    return std::make_tuple(result);
  });
  table.set_function("isStringComment", [](const char *_string) {
    auto result = Command<0x0E81>(_string);
    return std::make_tuple(result);
  });
  table.set_function("setStringUpper", [](const int &_stringAddress) { Command<0x0EC2>(_stringAddress); });
  table.set_function("setStringLower", [](const int &_stringAddress) { Command<0x0EC3>(_stringAddress); });
  table.set_function("stringFind", [](const int &_stringFind, const char *_stringOrigin, const char *_strFind) {
    int index_;
    auto result = Command<0x0EC4>(_stringFind, _stringOrigin, _strFind, &index_);
    return std::make_tuple(result, index_);
  });
  table.set_function("cutStringAt",
                     [](const int &_stringAddress, const int &_index) { Command<0x0EC5>(_stringAddress, _index); });
  table.set_function("isStringCharacterAt", [](const char *_string, const char *_characters, const int &_index) {
    auto result = Command<0x0EC6>(_string, _characters, _index);
    return std::make_tuple(result);
  });
}