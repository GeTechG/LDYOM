#include "logger.h"
#include "paths.h"
#include <csignal>
#include <iostream>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

#include <DbgHelp.h>
#include <windows.h>
#pragma comment(lib, "Dbghelp.lib")

std::shared_ptr<spdlog::logger> Logger::s_Logger;

void Logger::Initialize() {
	try {
		std::vector<spdlog::sink_ptr> logSinks;

		auto msvcSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		msvcSink->set_level(spdlog::level::trace);
		logSinks.push_back(msvcSink);

		// Добавляем вывод в файл
		auto fileSink =
			std::make_shared<spdlog::sinks::rotating_file_sink_mt>(LDYOM_PATH("ldyom.log"), 1024 * 1024 * 5, 3);
		fileSink->set_level(spdlog::level::info);
		logSinks.push_back(fileSink);

		// Создаем логгер с указанными приемниками
		s_Logger = std::make_shared<spdlog::logger>("LDYOM", begin(logSinks), end(logSinks));
		s_Logger->set_level(spdlog::level::trace);
		s_Logger->flush_on(spdlog::level::trace);

		// Настройка форматирования
		spdlog::set_pattern("%^[%T] %n: %v%$");
		spdlog::flush_every(std::chrono::seconds(1)); // Сократим время до сброса логов на диск

		// Включаем механизм автоматической регистрации функций для стектрейса в spdlog
		// spdlog подходит для сохранения программных маркеров (логические точки стека вызовов),
		// но не для системного стектрейса
		spdlog::enable_backtrace(64);

		// Установка обработчиков сигналов для перехвата падений
		SetupCrashHandlers();

		LDYOM_INFO("Logger initialized");
	} catch (const spdlog::spdlog_ex& ex) {
		std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
	}
}

void Logger::Shutdown() {
	if (s_Logger) {
		LDYOM_INFO("Logger shutdown");
		s_Logger->flush();
		spdlog::shutdown();
	}
}

// Получение реального стектрейса операционной системы
std::string GetStackTraceString(int skip = 1) {
	std::string result;

	const int MAX_FRAMES = 32;
	void* frames[MAX_FRAMES];
	HANDLE process = GetCurrentProcess();

	// Расширенные опции для лучшей работы с символами и PDB
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_DEBUG);

	// Построение путей поиска символов
	std::string symbolPath;

	// 1. Получаем путь к текущему ASI/DLL-модулю
	HMODULE hModule = NULL;
	BOOL result_module =
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
	                      (LPCTSTR)&GetStackTraceString, // Используем адрес нашей функции для получения модуля
	                      &hModule);

	if (result_module) {
		char modulePath[MAX_PATH];
		if (GetModuleFileNameA(hModule, modulePath, MAX_PATH)) {
			// Получаем директорию текущего модуля
			std::string moduleDir = modulePath;
			size_t lastSlash = moduleDir.find_last_of("\\/");
			if (lastSlash != std::string::npos) {
				moduleDir = moduleDir.substr(0, lastSlash);
				symbolPath += moduleDir + ";";
			}
		}
	}

	// 2. Получаем путь к текущему исполняемому файлу
	char exePath[MAX_PATH];
	if (GetModuleFileNameA(NULL, exePath, MAX_PATH)) {
		// Получаем директорию текущего исполняемого файла
		std::string exeDir = exePath;
		size_t lastSlash = exeDir.find_last_of("\\/");
		if (lastSlash != std::string::npos) {
			exeDir = exeDir.substr(0, lastSlash);
			symbolPath += exeDir + ";";
		}
	}

	// Логируем пути поиска в файл для диагностики
	if (Logger::GetLogger()) {
		LDYOM_DEBUG("Symbol search paths: {}", symbolPath);
	}

	// Инициализируем с нашими путями к символам
	if (!SymInitialize(process, symbolPath.c_str(), TRUE)) {
		DWORD error = GetLastError();
		result += "Failed to initialize symbol handler: " + std::to_string(error) + "\n";
		return result;
	}

	WORD frames_count = CaptureStackBackTrace(skip, MAX_FRAMES, frames, NULL);

	// Логируем количество захваченных фреймов
	result += "Captured frames: " + std::to_string(frames_count) + "\n";

	for (WORD i = 0; i < frames_count; i++) {
		DWORD64 address = (DWORD64)(frames[i]);

		// Получаем имя модуля для этого адреса
		IMAGEHLP_MODULE64 moduleInfo;
		moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
		bool hasModuleInfo = SymGetModuleInfo64(process, address, &moduleInfo);

		// Буфер для информации о символе
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;

		DWORD64 displacement = 0;

		// Получаем информацию о символе по адресу
		if (SymFromAddr(process, address, &displacement, symbol)) {
			// Информация о строке и файле
			IMAGEHLP_LINE64 line;
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD displacement_line = 0;

			if (SymGetLineFromAddr64(process, address, &displacement_line, &line)) {
				// Форматируем информацию включая модуль
				char frame_desc[2048];
				int written = _snprintf_s(frame_desc, sizeof(frame_desc), _TRUNCATE, "#%d 0x%p [%s] %s in %s:%d\n", i,
				                          (void*)address, hasModuleInfo ? moduleInfo.ModuleName : "unknown",
				                          symbol->Name, line.FileName, line.LineNumber);

				if (written >= 0) {
					result += frame_desc;
				} else {
					result += "Frame info too large to display\n";
				}
			} else {
				DWORD error = GetLastError();
				char frame_desc[1024];
				int written = _snprintf_s(frame_desc, sizeof(frame_desc), _TRUNCATE,
				                          "#%d 0x%p [%s] %s (no line info, error: %d)\n", i, (void*)address,
				                          hasModuleInfo ? moduleInfo.ModuleName : "unknown", symbol->Name, error);

				if (written >= 0) {
					result += frame_desc;
				} else {
					result += "Frame info too large to display\n";
				}
			}
		} else {
			// Не удалось получить символ - выводим причину
			DWORD error = GetLastError();
			char frame_desc[512];
			_snprintf_s(frame_desc, sizeof(frame_desc), _TRUNCATE, "#%d 0x%p [%s] (no symbol info, error: %d)\n", i,
			            (void*)address, hasModuleInfo ? moduleInfo.ModuleName : "unknown", error);
			result += frame_desc;
		}
	}

	SymCleanup(process);

	return result;
}

void Logger::DumpBacktrace() {
	if (s_Logger) {
		LDYOM_CRITICAL("===== STACKTRACE DUMP =====");

		// Записываем программные точки spdlog (если они были)
		s_Logger->dump_backtrace();

		// Записываем реальный системный стектрейс
		std::string stacktrace =
			GetStackTraceString(2); // Пропускаем первые 2 кадра (DumpBacktrace и GetStackTraceString)
		LDYOM_CRITICAL("System stacktrace:\n{}", stacktrace);

		s_Logger->flush();
	}
}

void Logger::SetupCrashHandlers() {
	// Установка обработчиков сигналов
	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGTERM, SignalHandler);

#ifdef _WIN32
	// Дополнительный обработчик для Windows через SEH
	SetUnhandledExceptionFilter([](EXCEPTION_POINTERS* pExp) -> LONG {
		const char* exception_name = "Unknown SEH Exception";

		// Определяем тип исключения
		if (pExp->ExceptionRecord->ExceptionCode == 0xE06D7363) { // C++ Exception (MS C++ Exception code)
			exception_name = "C++ Exception";
		}

		switch (pExp->ExceptionRecord->ExceptionCode) {
		case EXCEPTION_ACCESS_VIOLATION:
			exception_name = "EXCEPTION_ACCESS_VIOLATION";
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			exception_name = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
			break;
		case EXCEPTION_BREAKPOINT:
			exception_name = "EXCEPTION_BREAKPOINT";
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			exception_name = "EXCEPTION_DATATYPE_MISALIGNMENT";
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			exception_name = "EXCEPTION_FLT_DENORMAL_OPERAND";
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			exception_name = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			exception_name = "EXCEPTION_FLT_INEXACT_RESULT";
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			exception_name = "EXCEPTION_FLT_INVALID_OPERATION";
			break;
		case EXCEPTION_FLT_OVERFLOW:
			exception_name = "EXCEPTION_FLT_OVERFLOW";
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			exception_name = "EXCEPTION_FLT_STACK_CHECK";
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			exception_name = "EXCEPTION_FLT_UNDERFLOW";
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			exception_name = "EXCEPTION_ILLEGAL_INSTRUCTION";
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			exception_name = "EXCEPTION_IN_PAGE_ERROR";
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			exception_name = "EXCEPTION_INT_DIVIDE_BY_ZERO";
			break;
		case EXCEPTION_INT_OVERFLOW:
			exception_name = "EXCEPTION_INT_OVERFLOW";
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			exception_name = "EXCEPTION_INVALID_DISPOSITION";
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			exception_name = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			exception_name = "EXCEPTION_PRIV_INSTRUCTION";
			break;
		case EXCEPTION_SINGLE_STEP:
			exception_name = "EXCEPTION_SINGLE_STEP";
			break;
		case EXCEPTION_STACK_OVERFLOW:
			exception_name = "EXCEPTION_STACK_OVERFLOW";
			break;
		}

		if (Logger::GetLogger()) {
			LDYOM_CRITICAL("Unhandled SEH Exception: {} (0x{:08X}) at address 0x{:p}", exception_name,
			               pExp->ExceptionRecord->ExceptionCode, pExp->ExceptionRecord->ExceptionAddress);

			// Для C++ исключений можно извлечь информацию о стеке непосредственно из контекста исключения
			// Создаем контекст стека для StackWalk64
			CONTEXT context = *pExp->ContextRecord;

			// Получаем информацию о стеке в точке исключения вместо точки обработки
			HANDLE process = GetCurrentProcess();
			HANDLE thread = GetCurrentThread();

			// Настраиваем символы
			SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_DEBUG);

			// Строим пути для поиска символов
			std::string symbolPath;

			// Добавляем путь к текущему модулю
			char exePath[MAX_PATH];
			if (GetModuleFileNameA(NULL, exePath, MAX_PATH)) {
				std::string exeDir = exePath;
				size_t lastSlash = exeDir.find_last_of("\\/");
				if (lastSlash != std::string::npos) {
					exeDir = exeDir.substr(0, lastSlash);
					symbolPath += exeDir + ";";
				}
			}

			if (!SymInitialize(process, symbolPath.c_str(), TRUE)) {
				LDYOM_ERROR("Failed to initialize symbols for exception context: {}", GetLastError());
				Logger::DumpBacktrace();
				Logger::GetLogger()->flush();
				return EXCEPTION_CONTINUE_SEARCH;
			}

			STACKFRAME64 stack;
			memset(&stack, 0, sizeof(STACKFRAME64));

			DWORD machine;

			machine = IMAGE_FILE_MACHINE_I386;
			stack.AddrPC.Offset = context.Eip;
			stack.AddrPC.Mode = AddrModeFlat;
			stack.AddrFrame.Offset = context.Ebp;
			stack.AddrFrame.Mode = AddrModeFlat;
			stack.AddrStack.Offset = context.Esp;
			stack.AddrStack.Mode = AddrModeFlat;

			LDYOM_CRITICAL("Exception stack trace from context (most accurate):");

			// Ходим по стеку исключения
			for (int frameNum = 0; frameNum < 32; frameNum++) {
				if (!StackWalk64(machine, process, thread, &stack, &context, NULL, SymFunctionTableAccess64,
				                 SymGetModuleBase64, NULL)) {
					break;
				}

				DWORD64 address = stack.AddrPC.Offset;
				if (address == 0) {
					break;
				}

				// Получаем имя модуля
				IMAGEHLP_MODULE64 moduleInfo;
				moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
				bool hasModuleInfo = SymGetModuleInfo64(process, address, &moduleInfo);

				// Получаем символ
				char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
				PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuffer;
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
				symbol->MaxNameLen = MAX_SYM_NAME;

				DWORD64 displacement = 0;
				if (SymFromAddr(process, address, &displacement, symbol)) {
					IMAGEHLP_LINE64 line;
					line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
					DWORD displacement_line = 0;

					if (SymGetLineFromAddr64(process, address, &displacement_line, &line)) {
						LDYOM_CRITICAL("#{} 0x{:p} [{}] {} in {}:{}", frameNum, (void*)address,
						               hasModuleInfo ? moduleInfo.ModuleName : "unknown", symbol->Name, line.FileName,
						               line.LineNumber);
					} else {
						LDYOM_CRITICAL("#{} 0x{:p} [{}] {} (no line info, error: {})", frameNum, (void*)address,
						               hasModuleInfo ? moduleInfo.ModuleName : "unknown", symbol->Name, GetLastError());
					}
				} else {
					LDYOM_CRITICAL("#{} 0x{:p} [{}] (no symbol info, error: {})", frameNum, (void*)address,
					               hasModuleInfo ? moduleInfo.ModuleName : "unknown", GetLastError());
				}
			}

			SymCleanup(process);

			Logger::GetLogger()->flush();
		}

		return EXCEPTION_CONTINUE_SEARCH; // Продолжаем поиск обработчиков
	});
#endif

	// Установка обработчика необработанных исключений C++
	std::set_terminate([]() {
		LDYOM_CRITICAL("Unhandled C++ exception caught!");
		DumpBacktrace();

		if (s_Logger) {
			s_Logger->flush();
		}

		abort(); // Завершаем программу
	});

	LDYOM_INFO("Crash handlers initialized");
}

void Logger::SignalHandler(int signum) {
	const char* signal_name = "Unknown signal";

	switch (signum) {
	case SIGABRT:
		signal_name = "SIGABRT";
		break;
	case SIGFPE:
		signal_name = "SIGFPE";
		break;
	case SIGILL:
		signal_name = "SIGILL";
		break;
	case SIGSEGV:
		signal_name = "SIGSEGV";
		break;
	case SIGTERM:
		signal_name = "SIGTERM";
		break;
	}

	if (s_Logger) {
		LDYOM_CRITICAL("Signal received: {} ({})", signal_name, signum);
		DumpBacktrace();
		s_Logger->flush();
	}

	// Восстановление стандартного обработчика и повторная отправка сигнала
	signal(signum, SIG_DFL);
	raise(signum);
}