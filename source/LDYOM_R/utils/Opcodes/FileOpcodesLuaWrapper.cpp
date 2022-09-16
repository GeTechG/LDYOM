//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void FileOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("FileOp");
    //CLEO
	table.set_function("open", [](const char* _filePathName, const char* _mode) {int handle_; auto result = Command<0x0A9A>(_filePathName, _mode, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("close", [](const int& _self) { auto result = Command<0x0A9B>(_self); return std::make_tuple(result);});
	table.set_function("getSize", [](const int& _self) {int size_; auto result = Command<0x0A9C>(_self, &size_); return std::make_tuple(size_, result);});
	table.set_function("read", [](const int& _self, const int& _size) {int destination_; auto result = Command<0x0A9D>(_self, _size, &destination_); return std::make_tuple(destination_, result);});
	table.set_function("write", [](const int& _self, const int& _size, const int& _source) { auto result = Command<0x0A9E>(_self, _size, _source); return std::make_tuple(result);});
	table.set_function("seek", [](const int& _self, const int& _offset, const int& _origin) { auto result = Command<0x0AD5>(_self, _offset, _origin); return std::make_tuple(result);});
	table.set_function("isEndReached", [](const int& _self) { auto result = Command<0x0AD6>(_self); return std::make_tuple(result);});
	table.set_function("readString", [](const int& _self, const char* _buffer, const int& _size) { auto result = Command<0x0AD7>(_self, _buffer, _size); return std::make_tuple(result);});
	table.set_function("writeString", [](const int& _self, const char* _source) { auto result = Command<0x0AD8>(_self, _source); return std::make_tuple(result);});
	table.set_function("writeFormattedString", [](const int& _self, const char* _format, const int& _args) { auto result = Command<0x0AD9>(_self, _format, _args); return std::make_tuple(result);});
	table.set_function("scan", [](const int& _self, const char* _format, const int& _args) {const char* text_; auto result = Command<0x0ADA>(_self, _format, _args, (int*)&text_); return std::make_tuple(text_, result);});

}