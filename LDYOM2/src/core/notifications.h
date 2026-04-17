#pragma once
#include <chrono>
#include <string>

namespace Notifications {

enum class NotificationType { Success, Info, Warning, Error };

inline constexpr auto kDefaultDuration = std::chrono::milliseconds{3000};

void push(NotificationType type, const std::string& title, const std::string& message,
          std::chrono::milliseconds duration = kDefaultDuration);
void success(const std::string& title, const std::string& message);
void info(const std::string& title, const std::string& message);
void warning(const std::string& title, const std::string& message);
void error(const std::string& title, const std::string& message);

void renderAll();

} // namespace Notifications
