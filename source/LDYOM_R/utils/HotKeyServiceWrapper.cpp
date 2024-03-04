#include <sol.hpp>

#include "HotKeyService.h"


void hotKeyServiceWrapper(sol::state &state) {
	auto ldTable = state["ld"].get<sol::table>();
	ldTable["hotkeyService"] = &HotKeyService::getInstance();
	state.new_usertype<ImHotKey::HotKey>("LDHotKey", sol::no_constructor,
	                                     "functionName", &ImHotKey::HotKey::functionName,
	                                     "functionKeys", &ImHotKey::HotKey::functionKeys
	);
	state.new_usertype<HotKeyService>("LDHotKeyService", sol::no_constructor,
	                                  "getHotKeyByName", &HotKeyService::getHotKeyByName,
	                                  "stringToKeyCombo", &HotKeyService::stringToKeyCombo,
	                                  "getHotKey", &HotKeyService::getHotKey
	);
	ldTable["GetHotKeyLib"] = [](const unsigned int functionKeys) {
		char buffer[32];
		ImHotKey::GetHotKeyLib(functionKeys, buffer, sizeof buffer);
		std::string str = buffer;
		return str;
	};
}
