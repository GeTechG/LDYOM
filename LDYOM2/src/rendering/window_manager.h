#pragma once

#include "window.h"
#include <algorithm>
#include <functional>
#include <imgui.h>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class WindowManagerException : public std::runtime_error {
  public:
	using std::runtime_error::runtime_error;
};

struct HotkeyInfo {
	ImGuiKey key;
	bool ctrl;
	bool alt;
	bool shift;

	bool operator==(const HotkeyInfo& other) const {
		return key == other.key && ctrl == other.ctrl && alt == other.alt && shift == other.shift;
	}
};

namespace std {
template <> struct hash<HotkeyInfo> {
	std::size_t operator()(const HotkeyInfo& k) const {
		return ((std::hash<int>()(static_cast<int>(k.key)) ^ (std::hash<bool>()(k.ctrl) << 1)) >> 1) ^
		       (std::hash<bool>()(k.alt) << 1) ^ (std::hash<bool>()(k.shift) << 2);
	}
};
} // namespace std

class WindowManager {
  private:
	std::unordered_map<std::string, std::unique_ptr<Window>> m_windows;
	bool m_initialized = false;

	std::unordered_map<HotkeyInfo, std::string> m_hotkeys;

	auto findWindow(std::string_view id) const noexcept;
	auto findWindowMutable(std::string_view id) noexcept;

	void checkInitialized() const;

	WindowManager() = default;

  public:
	static WindowManager& instance() noexcept;

	void initialize();
	void shutdown() noexcept;

	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;

	WindowManager(WindowManager&&) = delete;
	WindowManager& operator=(WindowManager&&) = delete;

	Window& addWindow(std::string_view id, std::unique_ptr<Window> window);
	std::optional<std::reference_wrapper<Window>> getWindow(std::string_view id);
	std::optional<std::reference_wrapper<const Window>> getWindow(std::string_view id) const;
	bool removeWindow(std::string_view id);
	void openWindow(std::string_view id);
	void closeWindow(std::string_view id);
	bool isWindowOpen(std::string_view id) const;

	void registerHotkey(ImGuiKey key, bool ctrl, bool alt, bool shift, std::string_view windowId);
	bool unregisterHotkey(ImGuiKey key, bool ctrl, bool alt, bool shift);
	void processHotkeys();
	bool hasHotkey(ImGuiKey key, bool ctrl, bool alt, bool shift) const;
	std::string getHotkeyWindowId(ImGuiKey key, bool ctrl, bool alt, bool shift) const;

	void render();
};