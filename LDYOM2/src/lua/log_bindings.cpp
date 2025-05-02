#include "log_bindings.h"
#include "utils/logger.h"
#include <fmt/args.h>

void register_log_bindings(sol::state& lua) {
	// Create a "log" table in Lua
	sol::table log = lua.create_table("log");

	// Register logging functions
	log["trace"] = [](const std::string& msg) { LDYOM_TRACE(msg); };
	log["debug"] = [](const std::string& msg) { LDYOM_DEBUG(msg); };
	log["info"] = [](const std::string& msg) { LDYOM_INFO(msg); };
	log["warn"] = [](const std::string& msg) { LDYOM_WARN(msg); };
	log["error"] = [](const std::string& msg) { LDYOM_ERROR(msg); };
	log["critical"] = [](const std::string& msg) { LDYOM_CRITICAL(msg); };

	// Helper function to convert sol::variadic_args to vector of strings
	auto args_to_strings = [](const sol::variadic_args& va, size_t start = 0) {
		fmt::dynamic_format_arg_store<fmt::format_context> formatArgStore;
		for (size_t i = start; i < va.size(); ++i) {
			formatArgStore.push_back(va.get<std::string>(i));
		}
		return formatArgStore;
	};

	// Register formatted versions that accept multiple arguments
	log["tracef"] = [args_to_strings](const sol::variadic_args& va) {
		if (va.size() < 1)
			return;
		try {
			auto args = args_to_strings(va, 1);
			std::string msg = fmt::vformat(va.get<std::string>(0), args);
			LDYOM_TRACE(msg);
		} catch (const fmt::format_error& e) {
			LDYOM_ERROR("Format error in log.tracef: {}", e.what());
		}
	};

	log["debugf"] = [args_to_strings](const sol::variadic_args& va) {
		if (va.size() < 1)
			return;
		try {
			auto args = args_to_strings(va, 1);
			std::string msg = fmt::vformat(va.get<std::string>(0), args);
			LDYOM_DEBUG(msg);
		} catch (const fmt::format_error& e) {
			LDYOM_ERROR("Format error in log.debugf: {}", e.what());
		}
	};

	log["infof"] = [args_to_strings](const sol::variadic_args& va) {
		if (va.size() < 1)
			return;
		try {
			auto args = args_to_strings(va, 1);
			std::string msg = fmt::vformat(va.get<std::string>(0), args);
			LDYOM_INFO(msg);
		} catch (const fmt::format_error& e) {
			LDYOM_ERROR("Format error in log.infof: {}", e.what());
		}
	};

	log["warnf"] = [args_to_strings](const sol::variadic_args& va) {
		if (va.size() < 1)
			return;
		try {
			auto args = args_to_strings(va, 1);
			std::string msg = fmt::vformat(va.get<std::string>(0), args);
			LDYOM_WARN(msg);
		} catch (const fmt::format_error& e) {
			LDYOM_ERROR("Format error in log.warnf: {}", e.what());
		}
	};

	log["errorf"] = [args_to_strings](const sol::variadic_args& va) {
		if (va.size() < 1)
			return;
		try {
			auto args = args_to_strings(va, 1);
			std::string msg = fmt::vformat(va.get<std::string>(0), args);
			LDYOM_ERROR(msg);
		} catch (const fmt::format_error& e) {
			LDYOM_ERROR("Format error in log.errorf: {}", e.what());
		}
	};

	log["criticalf"] = [args_to_strings](const sol::variadic_args& va) {
		if (va.size() < 1)
			return;
		try {
			auto args = args_to_strings(va, 1);
			std::string msg = fmt::vformat(va.get<std::string>(0), args);
			LDYOM_CRITICAL(msg);
		} catch (const fmt::format_error& e) {
			LDYOM_ERROR("Format error in log.criticalf: {}", e.what());
		}
	};
}