#include <sol.hpp>
#include <boost/signals2.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "LuaEngine.h"

void boostWrapper(sol::state &state) {
	auto signalVoid = state.new_usertype<boost::signals2::signal<void()>>("SignalVoid", sol::no_constructor);
	signalVoid["connect"] = [](boost::signals2::signal<void()> &signal, const sol::function &func) {
		return signal.connect([func]() {
			if (const auto result = func(); !result.valid()) {
				LuaEngine::errorHandler(result);
			}
		});
	};

	auto uuidClass = state.new_usertype<boost::uuids::uuid>("Uuid", sol::no_constructor);
	uuidClass.set(sol::meta_function::to_string, [](const boost::uuids::uuid &uuid) {
		return to_string(uuid);
	});
}
