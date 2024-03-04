---@meta

-- auto signalVoid = state.new_usertype<boost::signals2::signal<void()>>("SignalVoid", sol::no_constructor);
-- signalVoid["connect"] = [](boost::signals2::signal<void()> &signal, const sol::function &func) {
-- 	return signal.connect([func]() {
-- 		if (const auto result = func(); !result.valid()) {
-- 			LuaEngine::errorHandler(result);
-- 		}
-- 	});
-- };

-- auto uuidClass = state.new_usertype<boost::uuids::uuid>("Uuid", sol::no_constructor);
-- uuidClass.set(sol::meta_function::to_string, [](const boost::uuids::uuid &uuid) {
-- 	return to_string(uuid);
-- });

---@class SignalVoid
---@field connect fun(func: function)

---@class Uuid