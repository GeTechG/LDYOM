#include "hotkeys.h"
#include "settings.h"
#include <array>
#include <string_utils.h>

Hotkeys& Hotkeys::instance() {
	static Hotkeys instance;
	return instance;
}

void Hotkeys::initialize() {
	this->m_hotkeys.emplace_back(ImHotKey::HotKey{
		.functionName = "openEditor",
		.functionKeys = 0xFFFFFF17, // I
	});

	this->m_hotkeys.emplace_back(ImHotKey::HotKey{
		.functionName = "toggleDebugInfo",
		.functionKeys = 0xFFFFFF3E, // F4
	});

	this->m_hotkeys.emplace_back(ImHotKey::HotKey{
		.functionName = "teleportToMarker",
		.functionKeys = 0xFFFFFF3F, // F5
	});

	this->m_hotkeys.emplace_back(ImHotKey::HotKey{
		.functionName = "unlockPlayer",
		.functionKeys = 0xFFFFFF40, // F6
	});

	this->m_hotkeys.emplace_back(ImHotKey::HotKey{
		.functionName = "saveScene",
		.functionKeys = 0xFFFF1F1D, // Ctrl+S
	});

	const auto hotkeysConfig =
		Settings::instance().getSetting<std::unordered_map<std::string, std::string>>("hotkeys", {});

	for (size_t i = 0; i < m_hotkeys.size(); i++) {
		const auto it = hotkeysConfig.find(m_hotkeys[i].functionName);
		if (it != hotkeysConfig.end()) {
			m_hotkeys[i].functionKeys = stringToKeyCombo(it->second);
		}
		this->saveHotkey(i);
	}
}

void Hotkeys::shutdown() {
	this->m_hotkeys.clear();
	this->m_hotkeyCallbacks.clear();
}

void Hotkeys::saveHotkey(int index) {
	std::array<char, 128> hotkey_str;
	ImHotKey::GetHotKeyLib(this->m_hotkeys[index].functionKeys, hotkey_str.data(), hotkey_str.size());
	Settings::instance().setSetting(std::format("hotkeys.{}", this->m_hotkeys[index].functionName), hotkey_str.data());
}

void Hotkeys::addHotkey(const ImHotKey::HotKey& hotkey) {
	this->m_hotkeys.push_back(hotkey);
	Settings::instance().setSetting(std::format("hotkeys.{}", hotkey.functionName), hotkey.functionKeys);
}

std::vector<ImHotKey::HotKey>& Hotkeys::getHotkeys() { return this->m_hotkeys; }

std::optional<std::reference_wrapper<ImHotKey::HotKey>> Hotkeys::getHotKeyByName(std::string_view name) {
	for (auto& hotkey : this->m_hotkeys) {
		if (hotkey.functionName == name) {
			return std::ref(hotkey);
		}
	}
	return std::nullopt;
}

void Hotkeys::addHotkeyCallback(std::string_view name, std::function<void()> callback) {
	const auto key = std::string(name);
	if (!this->m_hotkeyCallbacks.contains(key)) {
		auto signal = std::make_unique<rocket::thread_safe_signal<void()>>();
		this->m_hotkeyCallbacks.emplace(key, std::move(signal));
	}
	this->m_hotkeyCallbacks[key]->connect(std::move(callback), rocket::queued_connection);
}

void Hotkeys::update() {
	if (ImGui::GetIO().WantTextInput)
		return;

	auto hotkey = this->getHotKey(false);
	if (hotkey != nullptr) {
		// Only process if this is a new key combination (not the same as last frame)
		if (hotkey->functionKeys != m_lastProcessedKeys) {
			m_lastProcessedKeys = hotkey->functionKeys;
			const auto& hotkeyName = hotkey->functionName;
			auto it = this->m_hotkeyCallbacks.find(hotkeyName);
			if (it != this->m_hotkeyCallbacks.end()) {
				(*it->second)();
			}
		}
	} else {
		// No keys pressed, reset state
		m_lastProcessedKeys = 0;
	}
}

unsigned int Hotkeys::stringToKeyCombo(std::string_view combo) {
	constexpr size_t MaxKeys = 4;
	std::array<unsigned char, MaxKeys> scanCodes{0xFF, 0xFF, 0xFF, 0xFF};
	std::array<unsigned char, MaxKeys> order{0xFF, 0xFF, 0xFF, 0xFF};

	size_t scanCodeCount = 0;
	const auto keys = split(combo, "+");

	for (const auto& key : keys) {
		if (scanCodeCount >= MaxKeys)
			break;

		const auto trimmedKey = trim_copy(key);
		const auto found = std::ranges::find_if(ImHotKey::Keys, [&trimmedKey](const auto& rowKeys) {
			return std::ranges::any_of(rowKeys,
			                           [&trimmedKey](const auto& iKey) { return iKey.lib && trimmedKey == iKey.lib; });
		});

		if (found != std::end(ImHotKey::Keys)) {
			const auto& matchingKey = std::ranges::find_if(
				*found, [&trimmedKey](const auto& iKey) { return iKey.lib && trimmedKey == iKey.lib; });

			if (matchingKey != std::end(*found)) {
				scanCodes[scanCodeCount] = static_cast<unsigned char>(matchingKey->scanCodePage1);
				order[scanCodeCount] = static_cast<unsigned char>(matchingKey->order);
				++scanCodeCount;
			}
		}
	}

	return ImHotKey::GetOrderedScanCodes(scanCodes.data(), order.data());
}

ImHotKey::HotKey* Hotkeys::getHotKey(bool repeat) {
	return ImHotKey::GetHotKey(this->m_hotkeys.data(), this->m_hotkeys.size(), repeat);
}
