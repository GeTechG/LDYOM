// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindMenu(sol::state &state) {
  auto table = state.create_table("MenuOp");
  // default
  table.set_function("create", [](const char *_header, const float &_topLeftX, const float &_topLeftY,
                                  const float &_width, const int &_numColumns, const bool &_interactive,
                                  const bool &_background, const int &_alignment) {
    int handle_;
    Command<0x08D4>(_header, _topLeftX, _topLeftY, _width, _numColumns, _interactive, _background, _alignment,
                    &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setColumnOrientation", [](const int &_self, const int &_column, const int &_alignment) {
    Command<0x08D6>(_self, _column, _alignment);
  });
  table.set_function("getItemSelected", [](const int &_self) {
    int row_;
    Command<0x08D7>(_self, &row_);
    return std::make_tuple(row_);
  });
  table.set_function("getItemAccepted", [](const int &_self) {
    int row_;
    Command<0x08D8>(_self, &row_);
    return std::make_tuple(row_);
  });
  table.set_function("activateItem", [](const int &_self, const int &_row, const bool &_state) {
    Command<0x08D9>(_self, _row, _state);
  });
  table.set_function("delete", [](const int &_self) { Command<0x08DA>(_self); });
  table.set_function("setColumn", [](const int &_self, const int &_column, const char *_title, const char *_row0,
                                     const char *_row1, const char *_row2, const char *_row3, const char *_row4,
                                     const char *_row5, const char *_row6, const char *_row7, const char *_row8,
                                     const char *_row9, const char *_row10, const char *_row11) {
    Command<0x08DB>(_self, _column, _title, _row0, _row1, _row2, _row3, _row4, _row5, _row6, _row7, _row8, _row9,
                    _row10, _row11);
  });
  table.set_function("setItemWithNumber",
                     [](const int &_self, const int &_column, const int &_row, const char *_gxt, const int &_number) {
                       Command<0x08EE>(_self, _column, _row, _gxt, _number);
                     });
  table.set_function("setItemWith2Numbers",
                     [](const int &_self, const int &_column, const int &_row, const char *_gxt, const int &_number1,
                        const int &_number2) { Command<0x08EF>(_self, _column, _row, _gxt, _number1, _number2); });
  table.set_function("setActiveItem", [](const int &_self, const int &_row) { Command<0x090E>(_self, _row); });
  table.set_function("setColumnWidth", [](const int &_self, const int &_column, const int &_width) {
    Command<0x09DB>(_self, _column, _width);
  });
  table.set_function("changeCarColor", [](const int &_self, const int &_vehicle, const int &_colorSlot,
                                          const int &_row) { Command<0x0A22>(_self, _vehicle, _colorSlot, _row); });
  table.set_function("highlightItem", [](const int &_self, const int &_row, const bool &_state) {
    Command<0x0A23>(_self, _row, _state);
  });
}