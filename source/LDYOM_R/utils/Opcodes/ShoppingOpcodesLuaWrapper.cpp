//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ShoppingOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ShoppingOp");
    //default
	table.set_function("loadPrices", [](const char* _sectionName) { auto result = Command<0x075D>(_sectionName); return std::make_tuple(result);});
	table.set_function("load", [](const char* _name) { auto result = Command<0x075E>(_name); return std::make_tuple(result);});
	table.set_function("getNumberOfItems", []() {int numItems_; auto result = Command<0x075F>(&numItems_); return std::make_tuple(numItems_, result);});
	table.set_function("getItem", [](const int& _nth) {int id_; auto result = Command<0x0760>(_nth, &id_); return std::make_tuple(id_, result);});
	table.set_function("getPriceOfItem", [](const int& _itemId) {int price_; auto result = Command<0x0761>(_itemId, &price_); return std::make_tuple(price_, result);});
	table.set_function("getExtraInfo", [](const int& _itemId, const int& _flag) {int value_; auto result = Command<0x0783>(_itemId, _flag, &value_); return std::make_tuple(value_, result);});
	table.set_function("getNameOfItem", [](const int& _itemId) {const char* name_; auto result = Command<0x078C>(_itemId, (int*)&name_); return std::make_tuple(name_, result);});
	table.set_function("buyItem", [](const int& _itemId) { auto result = Command<0x0790>(_itemId); return std::make_tuple(result);});
	table.set_function("getLoaded", []() {const char* name_; auto result = Command<0x07B0>((int*)&name_); return std::make_tuple(name_, result);});
	table.set_function("clearLoaded", []() { auto result = Command<0x087C>(); return std::make_tuple(result);});
	table.set_function("addPriceModifier", [](const int& _itemId, const int& _modifier) { auto result = Command<0x08C8>(_itemId, _modifier); return std::make_tuple(result);});
	table.set_function("removePriceModifier", [](const int& _itemId) { auto result = Command<0x08C9>(_itemId); return std::make_tuple(result);});
	table.set_function("hasPlayerBoughtItem", [](const int& _itemId) { auto result = Command<0x0942>(_itemId); return std::make_tuple(result);});

}