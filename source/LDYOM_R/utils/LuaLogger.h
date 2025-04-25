#pragma once
#include <list>
#include <string>
#include <sol.hpp>

class LuaLogger {
private:
	std::list<std::string> items_;

	LuaLogger() = default;
	LuaLogger(const LuaLogger& root) = delete;
	LuaLogger& operator=(const LuaLogger&) = delete;
public:
	static LuaLogger& getInstance() {
		static LuaLogger instance;
		return instance;
	}

	void print(sol::state_view state, const sol::object& obj, sol::variadic_args args);
	void print(const std::string& text);
	template<class ...T>
	void print(std::string text, T... args);
	std::list<std::string>& getItems();
};

template <class ... T>
void LuaLogger::print(const std::string text, T... args) {
	this->items_.push_back(text);
	this->print(args...);
}

