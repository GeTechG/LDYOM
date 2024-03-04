// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindMemory(sol::state &state) {
  auto table = state.create_table("MemoryOp");
  // CLEO
  table.set_function("write", [](const int &_address, const int &_size, const int &_value, const bool &_vp) {
    Command<0x0A8C>(_address, _size, _value, _vp);
  });
  table.set_function("read", [](const int &_address, const int &_size, const bool &_vp) {
    int result_;
    Command<0x0A8D>(_address, _size, _vp, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("getPedPointer", [](const int &_char) {
    int address_;
    Command<0x0A96>(_char, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("getVehiclePointer", [](const int &_vehicle) {
    int address_;
    Command<0x0A97>(_vehicle, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("getObjectPointer", [](const int &_object) {
    int address_;
    Command<0x0A98>(_object, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("getThisScriptStruct", []() {
    int address_;
    Command<0x0A9F>(&address_);
    return std::make_tuple(address_);
  });
  table.set_function("callFunction",
                     [](const int &_address, const int &_numParams, const int &_pop, const int &_funcParams) {
                       Command<0x0AA5>(_address, _numParams, _pop, _funcParams);
                     });
  table.set_function("callMethod",
                     [](const int &_address, const int &_struct, const int &_numParams, const int &_pop,
                        const int &_funcParams) { Command<0x0AA6>(_address, _struct, _numParams, _pop, _funcParams); });
  table.set_function("callFunctionReturn",
                     [](const int &_address, const int &_numParams, const int &_pop, const int &_funcParams) {
                       int funcRet_;
                       Command<0x0AA7>(_address, _numParams, _pop, _funcParams, &funcRet_);
                       return std::make_tuple(funcRet_);
                     });
  table.set_function("callMethodReturn", [](const int &_address, const int &_struct, const int &_numParams,
                                            const int &_pop, const int &_funcParams) {
    int funcRet_;
    Command<0x0AA8>(_address, _struct, _numParams, _pop, _funcParams, &funcRet_);
    return std::make_tuple(funcRet_);
  });
  table.set_function("getScriptStructNamed", [](const char *_scriptName) {
    int address_;
    Command<0x0AAA>(_scriptName, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("getLabelPointer", [](const char *_label) {
    int address_;
    Command<0x0AC6>(_label, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("getVarPointer", [](const int &_any) {
    int address_;
    Command<0x0AC7>(_any, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("allocate", [](const int &_size) {
    int address_;
    Command<0x0AC8>(_size, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("free", [](const int &_address) { Command<0x0AC9>(_address); });
  table.set_function("popFloat", []() {
    float number_;
    Command<0x0AE9>(&number_);
    return std::make_tuple(number_);
  });
  table.set_function("getPedRef", [](const int &_address) {
    int handle_;
    Command<0x0AEA>(_address, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("getVehicleRef", [](const int &_address) {
    int handle_;
    Command<0x0AEB>(_address, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("getObjectRef", [](const int &_address) {
    int handle_;
    Command<0x0AEC>(_address, &handle_);
    return std::make_tuple(handle_);
  });

  // CLEO+
  table.set_function("copy",
                     [](const int &_src, const int &_dest, const int &_size) { Command<0x0D27>(_src, _dest, _size); });
  table.set_function("writeStructParam", [](const int &_address, const int &_index, const int &_value) {
    Command<0x0D37>(_address, _index, _value);
  });
  table.set_function("readStructParam", [](const int &_address, const int &_index) {
    int value_;
    Command<0x0D38>(_address, _index, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("readStructOffset", [](const int &_address, const int &_offset, const int &_size) {
    int result_;
    Command<0x0D4E>(_address, _offset, _size, &result_);
    return std::make_tuple(result_);
  });
  table.set_function("writeStructOffset", [](const int &_address, const int &_offset, const int &_size,
                                             const int &_value) { Command<0x0E28>(_address, _offset, _size, _value); });
  table.set_function("makeNop", [](const int &_address, const int &_size) { Command<0x0E6A>(_address, _size); });
  table.set_function("getLastCreatedCustomScript", []() {
    int address_;
    auto result = Command<0x0E70>(&address_);
    return std::make_tuple(result, address_);
  });
  table.set_function("readStructOffsetMulti",
                     [](const int &_address, const int &_offset, const int &_count, const int &_size) {
                       int results_;
                       Command<0x0EE2>(_address, _offset, _count, _size, &results_);
                       return std::make_tuple(results_);
                     });
  table.set_function("writeStructOffsetMulti",
                     [](const int &_address, const int &_offset, const int &_count, const int &_size,
                        const int &_params) { Command<0x0EE3>(_address, _offset, _count, _size, _params); });
}