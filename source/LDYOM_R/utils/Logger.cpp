// ReSharper disable CppMemberFunctionMayBeStatic
#include "Logger.h"
#include <filesystem>
#include "../easylogging/easylogging++.h"

using namespace std;

const filesystem::path logPath = filesystem::path("LDYOM/LDYOM.log");

#define ELPP_FEATURE_CRASH_LOG
INITIALIZE_EASYLOGGINGPP

void Logger::Init()
{
	std::filesystem::remove("LDYOM/LDYOM.log");
	el::Configurations defaultConf;
	defaultConf.setToDefault();
	defaultConf.setGlobally(
		el::ConfigurationType::ToFile, "true");
	defaultConf.setGlobally(
		el::ConfigurationType::Filename, "LDYOM/LDYOM.log");
	defaultConf.setGlobally(
		el::ConfigurationType::Format, "%datetime [%logger] %fbase:%line | %msg");
	defaultConf.set(el::Level::Error, el::ConfigurationType::Format, "%datetime [%logger] %fbase:%func:%line %thread | %msg");

	el::Loggers::getLogger("LDYOM");
	el::Loggers::reconfigureAllLoggers(defaultConf);

	CLOG(INFO, "LDYOM") << "Logger initialized.\n";
}

void Logger::log(const std::string& text)
{
	CLOG(INFO, "LDYOM") << text;
}
