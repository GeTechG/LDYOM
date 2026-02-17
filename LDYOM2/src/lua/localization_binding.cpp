#include "localization_binding.h"
#include "core/localization.h"

void register_localization_bindings(sol::state& lua) {
	sol::table loc = lua.create_table("loc");

	loc["trv"] = [](const std::string& key, const sol::variadic_args& va) {
		std::vector<std::string> args;
		args.reserve(va.size());
		for (const auto& v : va) {
			args.push_back(v.get<std::string>());
		}
		return Localization::instance().getI18N().tr(key, args);
	};

	loc["trPluralv"] = [](const std::string& key, int count, const sol::variadic_args& va) {
		std::vector<std::string> args;
		args.reserve(va.size());
		for (const auto& v : va) {
			args.push_back(v.get<std::string>());
		}
		return Localization::instance().getI18N().trPlural(key, count, args);
	};
}
