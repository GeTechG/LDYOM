// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindList(sol::state &state) {
  auto table = state.create_table("ListOp");
  // CLEO+
  table.set_function("create", [](const int &_type) {
    int handle_;
    Command<0x0E72>(_type, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("delete", [](const int &_self) { Command<0x0E73>(_self); });
  table.set_function("add", [](const int &_self, const int &_value) { Command<0x0E74>(_self, _value); });
  table.set_function("removeValue", [](const int &_self, const int &_value) { Command<0x0E75>(_self, _value); });
  table.set_function("removeIndex", [](const int &_self, const int &_index) { Command<0x0E76>(_self, _index); });
  table.set_function("getSize", [](const int &_self) {
    int entries_;
    Command<0x0E77>(_self, &entries_);
    return std::make_tuple(entries_);
  });
  table.set_function("getValueByIndex", [](const int &_self, const int &_index) {
    int value_;
    Command<0x0E78>(_self, _index, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("reset", [](const int &_self) { Command<0x0E79>(_self); });
  table.set_function("getStringValueByIndex", [](const int &_self, const int &_index) {
    const char *string_;
    Command<0x0E7A>(_self, _index, (int *)&string_);
    return std::make_tuple(string_);
  });
  table.set_function("addString", [](const int &_self, const char *_string) { Command<0x0E7B>(_self, _string); });
  table.set_function("removeStringValue",
                     [](const int &_self, const char *_string) { Command<0x0E7C>(_self, _string); });
  table.set_function("removeIndexRange", [](const int &_self, const int &_start, const int &_stop) {
    Command<0x0E7D>(_self, _start, _stop);
  });
  table.set_function("reverse", [](const int &_self) { Command<0x0E7E>(_self); });
  table.set_function("replaceValueByIndex", [](const int &_self, const int &_index, const int &_value) {
    Command<0x0F06>(_self, _index, _value);
  });
  table.set_function("replaceStringValueByIndex", [](const int &_self, const int &_index, const char *_string) {
    Command<0x0F07>(_self, _index, _string);
  });
  table.set_function("insertValueByIndex", [](const int &_self, const int &_index, const int &_value) {
    Command<0x0F08>(_self, _index, _value);
  });
  table.set_function("insertStringValueByIndex", [](const int &_self, const int &_index, const char *_string) {
    Command<0x0F09>(_self, _index, _string);
  });
}