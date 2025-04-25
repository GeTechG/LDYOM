#pragma once

#include <csignal>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

// Получение системного стектрейса
std::string GetStackTraceString(int skip);

class Logger {
  public:
	static void Initialize();
	static void Shutdown();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	// Вывод стека вызовов в лог
	static void DumpBacktrace();

	// Установка обработчиков сигналов для вывода стека вызовов при падении
	static void SetupCrashHandlers();

  private:
	static std::shared_ptr<spdlog::logger> s_Logger;
	// Обработчик сигналов для вывода стека вызовов
	static void SignalHandler(int signum);
};

#define LDYOM_TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__)
#define LDYOM_DEBUG(...) ::Logger::GetLogger()->debug(__VA_ARGS__)
#define LDYOM_INFO(...) ::Logger::GetLogger()->info(__VA_ARGS__)
#define LDYOM_WARN(...) ::Logger::GetLogger()->warn(__VA_ARGS__)
#define LDYOM_ERROR(...) ::Logger::GetLogger()->error(__VA_ARGS__)
#define LDYOM_CRITICAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)

#define LDYOM_DUMP_BACKTRACE() ::Logger::DumpBacktrace()