#pragma once
#include <string>

class Logger
{
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(Logger&) = delete;
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}
	void Init();
	void log(const std::string& text);
};

