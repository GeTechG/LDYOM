//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void IniFileOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("IniFileOp");
    //ini
	table.set_function("readInt", [](const char* _path, const char* _section, const char* _key) {int value_; auto result = Command<0x0AF0>(_path, _section, _key, &value_); return std::make_tuple(value_, result);});
	table.set_function("writeInt", [](const int& _value, const char* _path, const char* _section, const char* _key) { auto result = Command<0x0AF1>(_value, _path, _section, _key); return std::make_tuple(result);});
	table.set_function("readFloat", [](const char* _path, const char* _section, const char* _key) {float value_; auto result = Command<0x0AF2>(_path, _section, _key, &value_); return std::make_tuple(value_, result);});
	table.set_function("writeFloat", [](const float& _value, const char* _path, const char* _section, const char* _key) { auto result = Command<0x0AF3>(_value, _path, _section, _key); return std::make_tuple(result);});
	table.set_function("readString", [](const char* _path, const char* _section, const char* _key) {const char* value_; auto result = Command<0x0AF4>(_path, _section, _key, (int*)&value_); return std::make_tuple(value_, result);});
	table.set_function("writeString", [](const char* _value, const char* _path, const char* _section, const char* _key) { auto result = Command<0x0AF5>(_value, _path, _section, _key); return std::make_tuple(result);});

}