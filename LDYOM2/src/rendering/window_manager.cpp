#include "window_manager.h"
#include "plugin.h"
#include <CMenuManager.h>
#include <format>
#include <imgui.h>
#include <imgui_hook.h>
#include <logger.h>
#include <ranges>

std::unordered_map<std::string, std::unique_ptr<Window>>::const_iterator
WindowManager::findWindow(const std::string_view id) const noexcept {
	return m_windows.find(std::string(id));
}

std::unordered_map<std::string, std::unique_ptr<Window>>::iterator
WindowManager::findWindowMutable(const std::string_view id) noexcept {
	return m_windows.find(std::string(id));
}

void WindowManager::checkInitialized() const {
	if (!m_initialized) {
		throw WindowManagerException("WindowManager is not initialized");
	}
}

WindowManager& WindowManager::instance() noexcept {
	static WindowManager instance;
	return instance;
}

void WindowManager::initialize() {
	if (m_initialized) {
		throw WindowManagerException("WindowManager is already initialized");
	}
	m_initialized = true;
	LDYOM_INFO("Window Manager initialized");
}

void WindowManager::shutdown() noexcept {
	if (m_initialized) {
		m_windows.clear();
		m_hotkeys.clear();
		m_initialized = false;
	}
}

void WindowManager::addWindow(std::string_view id, std::unique_ptr<Window>&& window) {
	checkInitialized();

	std::string idStr(id);

	if (m_windows.contains(idStr)) {
		throw WindowManagerException(std::format("Window with id '{}' already exists", idStr));
	}

	m_windows.emplace(idStr, std::move(window));
}

std::optional<Window*> WindowManager::getWindow(const std::string_view id) {
	checkInitialized();

	auto it = findWindowMutable(id);
	if (it != m_windows.end()) {
		return &*it->second;
	}
	return std::nullopt;
}

std::optional<const Window*> WindowManager::getWindow(const std::string_view id) const {
	checkInitialized();

	auto it = findWindow(id);
	if (it != m_windows.end()) {
		return &*it->second;
	}
	return std::nullopt;
}

bool WindowManager::removeWindow(std::string_view id) {
	checkInitialized();

	auto it = findWindowMutable(id);
	if (it != m_windows.end()) {
		if (it->second->isOpen())
			it->second->close();
		m_windows.erase(it);
		return true;
	}
	return false;
}

void WindowManager::openWindow(std::string_view id) {
	checkInitialized();

	auto it = findWindowMutable(id);
	if (it != m_windows.end()) {
		it->second->open();
	}
}

void WindowManager::closeWindow(std::string_view id) {
	checkInitialized();

	auto it = findWindowMutable(id);
	if (it != m_windows.end()) {
		it->second->close();
	}
}

bool WindowManager::isWindowOpen(std::string_view id) const {
	checkInitialized();

	auto it = findWindow(id);
	return it != m_windows.end() && it->second->isOpen();
}

void WindowManager::addBackgroundRenderCallback(std::string_view id, std::function<void()> callback) {
	m_backgroundRenderCallbacks[std::string(id)] = std::move(callback);
}

void WindowManager::removeBackgroundRenderCallback(std::string_view id) {
	m_backgroundRenderCallbacks.erase(std::string(id));
}

bool WindowManager::isAnyWindowOpen() const {
	checkInitialized();

	return std::ranges::any_of(m_windows, [](const auto& pair) { return pair.second->isOpen(); });
}

void WindowManager::closeAllWindows() {
	checkInitialized();

	for (auto& [id, window] : m_windows) {
		if (window->isOpen()) {
			window->close();
		}
	}
}

void WindowManager::registerHotkey(ImGuiKey key, bool ctrl, bool alt, bool shift, std::string_view windowId) {
	checkInitialized();

	if (findWindow(windowId) == m_windows.end()) {
		throw WindowManagerException(
			std::format("Cannot register hotkey: window with id '{}' does not exist", std::string(windowId)));
	}

	HotkeyInfo hotkey{.key = key, .ctrl = ctrl, .alt = alt, .shift = shift};

	m_hotkeys[hotkey] = std::string(windowId);
}

bool WindowManager::unregisterHotkey(ImGuiKey key, bool ctrl, bool alt, bool shift) {
	checkInitialized();

	HotkeyInfo hotkey{.key = key, .ctrl = ctrl, .alt = alt, .shift = shift};

	return m_hotkeys.erase(hotkey) > 0;
}

void WindowManager::processHotkeys() {
	checkInitialized();

	ImGuiIO& io = ImGui::GetIO();

	for (const auto& [hotkeyInfo, windowId] : m_hotkeys) {
		bool ctrlPressed = hotkeyInfo.ctrl && io.KeyCtrl;
		bool altPressed = hotkeyInfo.alt && io.KeyAlt;
		bool shiftPressed = hotkeyInfo.shift && io.KeyShift;

		if ((hotkeyInfo.ctrl && !io.KeyCtrl) || (!hotkeyInfo.ctrl && io.KeyCtrl) || (hotkeyInfo.alt && !io.KeyAlt) ||
		    (!hotkeyInfo.alt && io.KeyAlt) || (hotkeyInfo.shift && !io.KeyShift) ||
		    (!hotkeyInfo.shift && io.KeyShift)) {
			continue;
		}

		if (ImGui::IsKeyPressed(hotkeyInfo.key, false) && !io.WantTextInput) {
			const auto window = this->getWindow(windowId);
			if (window) {
				if (window.value()->isOpen() && window.value()->isFocused())
					window.value()->close();
				else
					window.value()->open();
			}
		}
	}
}

bool WindowManager::hasHotkey(ImGuiKey key, bool ctrl, bool alt, bool shift) const {
	checkInitialized();

	HotkeyInfo hotkey{.key = key, .ctrl = ctrl, .alt = alt, .shift = shift};

	return m_hotkeys.contains(hotkey);
}

std::string WindowManager::getHotkeyWindowId(ImGuiKey key, bool ctrl, bool alt, bool shift) const {
	checkInitialized();

	HotkeyInfo hotkey{.key = key, .ctrl = ctrl, .alt = alt, .shift = shift};

	auto it = m_hotkeys.find(hotkey);
	if (it != m_hotkeys.end()) {
		return it->second;
	}

	throw WindowManagerException("Hotkey not found");
}

void WindowManager::render() {
	checkInitialized();

	processHotkeys();

	const bool isLockPlayerControl = std::ranges::any_of(
		m_windows, [](const auto& pair) { return pair.second->isNeedLockPlayerControl() && pair.second->isOpen(); });

	ImguiHook::SetControlEnabled(!isLockPlayerControl && !m_disableWindowRendering);

	static bool mouseShown = false;
	const bool isSomeWindowOpen = this->isAnyWindowOpen();
	const bool isRenderWindows = isSomeWindowOpen && !FrontEndMenuManager.m_bMenuActive && !m_disableWindowRendering;
	if (mouseShown != isRenderWindows) {
		mouseShown = isRenderWindows;
		ImguiHook::SetMouseState(mouseShown);
	}
	if (!FrontEndMenuManager.m_bMenuActive) {
		auto keys = m_backgroundRenderCallbacks | std::views::keys | std::ranges::to<std::vector>();
		for (const auto& id : keys) {
			auto it = m_backgroundRenderCallbacks.find(id);
			if (it != m_backgroundRenderCallbacks.end()) {
				it->second();
			}
		}
	}

	if (isRenderWindows) {
		for (const auto& window : m_windows | std::views::values) {
			try {
				window->render();
			} catch (const std::exception&) {
				window->close();
			}
		}
	}
}