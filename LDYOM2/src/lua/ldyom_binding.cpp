#include "ldyom_binding.h"
#include "notifications.h"
#include <algorithm>
#include <string>

void register_ldyom_bindings(sol::state& lua) {
    sol::table ldyom = lua.create_table("ldyom");

    ldyom["notify"] = [](sol::this_state s, const std::string& type, const std::string& title,
                         const std::string& message, sol::optional<int> duration_ms) {
        std::string t = type;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);

        Notifications::NotificationType notifType;
        if (t == "success")      notifType = Notifications::NotificationType::Success;
        else if (t == "info")    notifType = Notifications::NotificationType::Info;
        else if (t == "warning") notifType = Notifications::NotificationType::Warning;
        else if (t == "error")   notifType = Notifications::NotificationType::Error;
        else {
            luaL_error(s, "ldyom.notify: unknown type '%s' (expected success|info|warning|error)", type.c_str());
            return;
        }

        auto dur = duration_ms.has_value()
            ? std::chrono::milliseconds{duration_ms.value()}
            : Notifications::kDefaultDuration;

        Notifications::push(notifType, title, message, dur);
    };
}
