// ReSharper disable CppMemberFunctionMayBeStatic
#include "Logger.h"
#include <filesystem>
#include <fstream>

using namespace std;

const filesystem::path logPath = filesystem::path("LDYOM/LDYOM.log");


void Logger::Init()
{
	ofstream logFile(logPath, std::ios::out);
	logFile << "Logger initialized.\n";
	logFile.close();
}

void Logger::log(const std::string& text)
{
	ofstream logFile(logPath, std::ios::app);
	logFile << text << '\n';
	logFile.close();
}
