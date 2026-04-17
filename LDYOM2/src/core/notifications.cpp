#include "notifications.h"
#define NOTIFY_RENDER_OUTSIDE_MAIN_WINDOW false
#define NOTIFY_USE_DISMISS_BUTTON false
#include "ImGuiNotify.hpp"
#include <tracy/Tracy.hpp>

namespace Notifications {

static ImGuiToastType toBackendType(NotificationType type) {
    switch (type) {
        case NotificationType::Success: return ImGuiToastType::Success;
        case NotificationType::Info:    return ImGuiToastType::Info;
        case NotificationType::Warning: return ImGuiToastType::Warning;
        case NotificationType::Error:   return ImGuiToastType::Error;
    }
    return ImGuiToastType::Info;
}

void push(NotificationType type, const std::string& title, const std::string& message,
          std::chrono::milliseconds duration) {
    ImGuiToast toast(toBackendType(type), static_cast<int>(duration.count()));
    toast.setTitle("%s", title.c_str());
    toast.setContent("%s", message.c_str());
    // NoSavedSettings prevents ImGui from restoring a stale window size from
    // imgui.ini on the first frame, which would cause a size-settling flicker.
    ImGui::InsertNotification(toast);
}

void success(const std::string& title, const std::string& message) { push(NotificationType::Success, title, message); }
void info(const std::string& title, const std::string& message)    { push(NotificationType::Info,    title, message); }
void warning(const std::string& title, const std::string& message) { push(NotificationType::Warning, title, message); }
void error(const std::string& title, const std::string& message)   { push(NotificationType::Error,   title, message); }

void renderAll() {
    ZoneScoped;
    ImGui::RenderNotifications();
}

} // namespace Notifications
