//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void KeywordsOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("KeywordsOp");
    //default
	table.set_function("nop", []() { auto result = Command<0x0000>(); return std::make_tuple(result);});
	table.set_function("wait", [](const int& _time) { auto result = Command<0x0001>(_time); return std::make_tuple(result);});
	table.set_function("goto", [](const int& _label) { auto result = Command<0x0002>(_label); return std::make_tuple(result);});
	table.set_function("gotoIfFalse", [](const int& _label) { auto result = Command<0x004D>(_label); return std::make_tuple(result);});
	table.set_function("terminateThisScript", []() { auto result = Command<0x004E>(); return std::make_tuple(result);});
	table.set_function("startNewScript", [](const int& _label, const int& _arguments) { auto result = Command<0x004F>(_label, _arguments); return std::make_tuple(result);});
	table.set_function("gosub", [](const int& _label) { auto result = Command<0x0050>(_label); return std::make_tuple(result);});
	table.set_function("return", []() { auto result = Command<0x0051>(); return std::make_tuple(result);});
	table.set_function("if", [](const int& _int) { auto result = Command<0x00D6>(_int); return std::make_tuple(result);});
	table.set_function("launchMission", [](const int& _label) { auto result = Command<0x00D7>(_label); return std::make_tuple(result);});
	table.set_function("setDeatharrestState", [](const bool& _state) { auto result = Command<0x0111>(_state); return std::make_tuple(result);});
	table.set_function("hasDeatharrestBeenExecuted", []() { auto result = Command<0x0112>(); return std::make_tuple(result);});
	table.set_function("declareMissionFlag", [](const int& _flag) { auto result = Command<0x0180>(_flag); return std::make_tuple(result);});
	table.set_function("scriptName", [](const char* _name) { auto result = Command<0x03A4>(_name); return std::make_tuple(result);});
	table.set_function("terminateAllScriptsWithThisName", [](const char* _name) { auto result = Command<0x0459>(_name); return std::make_tuple(result);});
	table.set_function("skipCutsceneEnd", []() { auto result = Command<0x0701>(); return std::make_tuple(result);});
	table.set_function("skipCutsceneStartInternal", [](const int& _label) { auto result = Command<0x0707>(_label); return std::make_tuple(result);});
	
	//CLEO
	table.set_function("streamCustomScript", [](const char* _scriptFileName, const int& _arguments) { auto result = Command<0x0A92>(_scriptFileName, _arguments); return std::make_tuple(result);});
	table.set_function("terminateThisCustomScript", []() { auto result = Command<0x0A93>(); return std::make_tuple(result);});
	table.set_function("loadAndLaunchCustomMission", [](const char* _scriptFileName, const int& _arguments) { auto result = Command<0x0A94>(_scriptFileName, _arguments); return std::make_tuple(result);});
	table.set_function("saveThisCustomScript", []() { auto result = Command<0x0A95>(); return std::make_tuple(result);});
	table.set_function("gosubIfFalse", [](const int& _label) { auto result = Command<0x0AA0>(_label); return std::make_tuple(result);});
	table.set_function("returnIfFalse", []() { auto result = Command<0x0AA1>(); return std::make_tuple(result);});
	table.set_function("cleoCall", [](const int& _label, const int& _numParams, const int& _params) { auto result = Command<0x0AB1>(_label, _numParams, _params); return std::make_tuple(result);});
	table.set_function("cleoReturn", [](const int& _numRet, const int& _retParams) { auto result = Command<0x0AB2>(_numRet, _retParams); return std::make_tuple(result);});
	table.set_function("setCleoSharedVar", [](const int& _varId, const int& _value) { auto result = Command<0x0AB3>(_varId, _value); return std::make_tuple(result);});
	table.set_function("getCleoSharedVar", [](const int& _varId) {int result_; auto result = Command<0x0AB4>(_varId, &result_); return std::make_tuple(result_, result);});
	table.set_function("terminateAllCustomScriptsWithThisName", [](const char* _scriptFileName) { auto result = Command<0x0ABA>(_scriptFileName); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("setScriptVar", [](const int& _scriptPointer, const int& _varIndex, const int& _value) { auto result = Command<0x0D2E>(_scriptPointer, _varIndex, _value); return std::make_tuple(result);});
	table.set_function("getScriptVar", [](const int& _scriptPointer, const int& _varIndex) {int result_; auto result = Command<0x0D2F>(_scriptPointer, _varIndex, &result_); return std::make_tuple(result_, result);});
	table.set_function("returnScriptEvent", []() { auto result = Command<0x0ED0>(); return std::make_tuple(result);});
	table.set_function("setScriptEventSaveConfirmation", [](const bool& _add, const int& _label, const int& _varSaveSlot) { auto result = Command<0x0ED1>(_add, _label, _varSaveSlot); return std::make_tuple(result);});
	table.set_function("setScriptEventCharCreate", [](const bool& _add, const int& _label, const int& _varChar) { auto result = Command<0x0ED2>(_add, _label, _varChar); return std::make_tuple(result);});
	table.set_function("setScriptEventCarDelete", [](const bool& _add, const int& _label, const int& _varCar) { auto result = Command<0x0ED4>(_add, _label, _varCar); return std::make_tuple(result);});
	table.set_function("setScriptEventCarCreate", [](const bool& _add, const int& _label, const int& _varCar) { auto result = Command<0x0ED5>(_add, _label, _varCar); return std::make_tuple(result);});
	table.set_function("setScriptEventObjectDelete", [](const bool& _add, const int& _label, const int& _varObject) { auto result = Command<0x0ED6>(_add, _label, _varObject); return std::make_tuple(result);});
	table.set_function("setScriptEventObjectCreate", [](const bool& _add, const int& _label, const int& _varObject) { auto result = Command<0x0ED7>(_add, _label, _varObject); return std::make_tuple(result);});
	table.set_function("setScriptEventOnMenu", [](const bool& _add, const int& _label, const bool& _varJustPaused) { auto result = Command<0x0ED8>(_add, _label, _varJustPaused); return std::make_tuple(result);});
	table.set_function("setScriptEventCharProcess", [](const bool& _add, const int& _label, const int& _varChar) { auto result = Command<0x0EDA>(_add, _label, _varChar); return std::make_tuple(result);});
	table.set_function("setScriptEventCarProcess", [](const bool& _add, const int& _label, const int& _varCar) { auto result = Command<0x0EDB>(_add, _label, _varCar); return std::make_tuple(result);});
	table.set_function("setScriptEventObjectProcess", [](const bool& _add, const int& _label, const int& _varObject) { auto result = Command<0x0EDC>(_add, _label, _varObject); return std::make_tuple(result);});
	table.set_function("setScriptEventBuildingProcess", [](const bool& _add, const int& _label, const int& _entity) { auto result = Command<0x0EDD>(_add, _label, _entity); return std::make_tuple(result);});
	table.set_function("setScriptEventCharDamage", [](const bool& _add, const int& _label, const int& _varChar) { auto result = Command<0x0EDE>(_add, _label, _varChar); return std::make_tuple(result);});
	table.set_function("setScriptEventCarWeaponDamage", [](const bool& _add, const int& _label, const int& _varCar) { auto result = Command<0x0EDF>(_add, _label, _varCar); return std::make_tuple(result);});
	table.set_function("setScriptEventBulletImpact", [](const bool& _add, const int& _label, const int& _varCharOwner, const int& _varEntityVictim, const int& _varWeaponType, const int& _varColPoint) { auto result = Command<0x0EE0>(_add, _label, _varCharOwner, _varEntityVictim, _varWeaponType, _varColPoint); return std::make_tuple(result);});

}