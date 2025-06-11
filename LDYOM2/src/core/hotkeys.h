#pragma once

#include "nlohmann/json.hpp"
#include <functional>
#include <imHotKey.h>
#include <iostream>
#include <memory>
#include <rocket.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class Hotkeys {
  public:
	static Hotkeys& instance();

	Hotkeys(const Hotkeys&) = delete;
	void operator=(const Hotkeys&) = delete;

	void initialize();
	void shutdown();
	void saveHotkey(int index);
	void addHotkey(const ImHotKey::HotKey& hotkey);
	std::vector<ImHotKey::HotKey>& getHotkeys();
	std::optional<std::reference_wrapper<ImHotKey::HotKey>> getHotKeyByName(std::string_view name);
	void addHotkeyCallback(std::string_view name, std::function<void()> callback);

	void update();

  private:
	std::vector<ImHotKey::HotKey> m_hotkeys;
	std::unordered_map<std::string, std::unique_ptr<rocket::thread_safe_signal<void()>>> m_hotkeyCallbacks;

	Hotkeys() = default;

	unsigned int stringToKeyCombo(std::string_view combo);
	ImHotKey::HotKey* getHotKey(bool repeat = false);
};