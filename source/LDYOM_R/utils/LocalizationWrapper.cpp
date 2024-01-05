#include <sol.hpp>

#include "strUtils.h"
#include "Localization/Localization.h"

void localizationWrapper(sol::state &state) {
	auto ldyomTable = state["ld"].get_or_create<sol::table>();

	state.set_function("gxtEncode", [](const std::string &text) {
		auto cp1251Text = utf8ToCp1251(text);
		gxtEncode(cp1251Text);
		return cp1251Text;
	});

	auto loc = ldyomTable["loc"].get_or_create<sol::table>();
	loc.set_function("addLocalizationDirectory", [](const std::string &path) {
		Localization::getInstance().addScriptsLocalization(path);
	});
	loc.set_function("get", [](const std::string &key) {
		return Localization::getInstance().get(key);
	});
	loc.set_function("getArray", [](const std::string &key) {
		return &Localization::getInstance().getArray(key);
	});
}
