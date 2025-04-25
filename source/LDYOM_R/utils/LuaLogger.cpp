#include "LuaLogger.h"

#include "easylogging/easylogging++.h"


void LuaLogger::print(sol::state_view state, const sol::object& obj, sol::variadic_args args) {
	std::stringstream buf;
	const std::string objString = state["tostring"](obj);
	buf << objString;
	for (auto v : args) {
		std::string vString = state["tostring"](v);
		buf << "\t" << vString;
	}
	auto log = buf.str();
	CLOG(INFO, "lua") << log;
	this->items_.emplace_back(log);
}

void LuaLogger::print(const std::string& text) {
	this->items_.push_back(text);
}

std::list<std::string>& LuaLogger::getItems() {
	return items_;
}
