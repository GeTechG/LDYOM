//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void FsOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("FsOp");
    //CLEO
	table.set_function("setCurrentDirectory", [](const int& _path) { auto result = Command<0x0A99>(_path); return std::make_tuple(result);});
	table.set_function("doesFileExist", [](const char* _path) { auto result = Command<0x0AAB>(_path); return std::make_tuple(result);});
	table.set_function("doesDirectoryExist", [](const char* _dirPath) { auto result = Command<0x0AE4>(_dirPath); return std::make_tuple(result);});
	table.set_function("createDirectory", [](const char* _dirName) { auto result = Command<0x0AE5>(_dirName); return std::make_tuple(result);});
	
	//file
	table.set_function("deleteFile", [](const char* _fileName) { auto result = Command<0x0B00>(_fileName); return std::make_tuple(result);});
	table.set_function("deleteDirectory", [](const char* _dirPath, const bool& _recursive) { auto result = Command<0x0B01>(_dirPath, _recursive); return std::make_tuple(result);});
	table.set_function("moveFile", [](const char* _fileName, const char* _newFileName) { auto result = Command<0x0B02>(_fileName, _newFileName); return std::make_tuple(result);});
	table.set_function("moveDirectory", [](const char* _dirPath, const char* _newDirPath) { auto result = Command<0x0B03>(_dirPath, _newDirPath); return std::make_tuple(result);});
	table.set_function("copyFile", [](const char* _fileName, const char* _newFileName) { auto result = Command<0x0B04>(_fileName, _newFileName); return std::make_tuple(result);});
	table.set_function("copyDirectory", [](const char* _dirPath, const char* _newDirPath) { auto result = Command<0x0B05>(_dirPath, _newDirPath); return std::make_tuple(result);});

}