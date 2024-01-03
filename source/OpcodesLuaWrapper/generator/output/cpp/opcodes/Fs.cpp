// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindFs(sol::state &state) {
  auto table = state.create_table("FsOp");
  // CLEO
  table.set_function("setCurrentDirectory", [](const int &_path) { Command<0x0A99>(_path); });
  table.set_function("doesFileExist", [](const char *_path) {
    auto result = Command<0x0AAB>(_path);
    return std::make_tuple(result);
  });
  table.set_function("doesDirectoryExist", [](const char *_dirPath) {
    auto result = Command<0x0AE4>(_dirPath);
    return std::make_tuple(result);
  });
  table.set_function("createDirectory", [](const char *_dirName) {
    auto result = Command<0x0AE5>(_dirName);
    return std::make_tuple(result);
  });
  table.set_function("resolvePath", [](const char *_path) {
    const char *resolved_;
    Command<0x2000>(_path, (int *)&resolved_);
    return std::make_tuple(resolved_);
  });
  table.set_function("getScriptFilename", [](const int &_scriptStruct, const int &_fullPath) {
    const char *string_;
    auto result = Command<0x2001>(_scriptStruct, _fullPath, (int *)&string_);
    return std::make_tuple(result, string_);
  });

  // file
  table.set_function("deleteFile", [](const char *_fileName) {
    auto result = Command<0x0B00>(_fileName);
    return std::make_tuple(result);
  });
  table.set_function("deleteDirectory", [](const char *_dirPath, const int &_recursive) {
    auto result = Command<0x0B01>(_dirPath, _recursive);
    return std::make_tuple(result);
  });
  table.set_function("moveFile", [](const char *_fileName, const char *_newFileName) {
    auto result = Command<0x0B02>(_fileName, _newFileName);
    return std::make_tuple(result);
  });
  table.set_function("moveDirectory", [](const char *_dirPath, const char *_newDirPath) {
    auto result = Command<0x0B03>(_dirPath, _newDirPath);
    return std::make_tuple(result);
  });
  table.set_function("copyFile", [](const char *_fileName, const char *_newFileName) {
    auto result = Command<0x0B04>(_fileName, _newFileName);
    return std::make_tuple(result);
  });
  table.set_function("copyDirectory", [](const char *_dirPath, const char *_newDirPath) {
    auto result = Command<0x0B05>(_dirPath, _newDirPath);
    return std::make_tuple(result);
  });
}