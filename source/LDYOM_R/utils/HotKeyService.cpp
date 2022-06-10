// ReSharper disable CppMemberFunctionMayBeStatic
#include "HotKeyService.h"
#include <sstream>

#include "Settings.h"
#include "utils.h"

void HotKeyService::loadHotKeys() {
	const auto hotkeys = Settings::getInstance().getSettings()["hotkeys"].get<std::unordered_map<std::string, std::string>>();
	for (const auto & hotKeySettings : hotkeys) {
		for (auto &hotkey : hotkeys_) {
			if (hotKeySettings.first._Equal(hotkey.functionName)) {
				hotkey.functionKeys = this->stringToKeyCombo(hotKeySettings.second);
			}
		} 
	}
}

void HotKeyService::Init() {
	hotkeys_.emplace_back("openMenu", 0xFFFFFF17);
	hotkeys_.emplace_back("openQuickCommandsMenu", 0xFFFFFF38);
	hotkeys_.emplace_back("accept", 0xFFFFFF15);
	hotkeys_.emplace_back("cancel", 0xFFFFFF31);
	hotkeys_.emplace_back("guizmoTranslate", 0xFFFFFF14);
	hotkeys_.emplace_back("guizmoRotate", 0xFFFFFF13);
	hotkeys_.emplace_back("guizmoScale", 0xFFFFFF1F);
	try {
		loadHotKeys();
	} catch(std::exception exception) {
		Logger::getInstance().log(fmt::format("invalid read hotkeys, error {}", exception.what()));
	}
}

void HotKeyService::onEditHotKey(ImHotKey::HotKey& hotKey) {
	auto& settings = Settings::getInstance().getSettings();
	char hotkey_str[128];
	ImHotKey::GetHotKeyLib(hotKey.functionKeys, hotkey_str, IM_ARRAYSIZE(hotkey_str));
	settings["hotkeys"][hotKey.functionName] = hotkey_str;
	Settings::getInstance().Save();
}

std::vector<ImHotKey::HotKey>& HotKeyService::getHotkeys() {
	return hotkeys_;
}

unsigned HotKeyService::stringToKeyCombo(const std::string& combo) {
	std::vector<std::string> keys = split(combo, "+");

	unsigned char scanCodes[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	unsigned char order[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

	int scanCodeCount = 0;
	for (auto &key : keys) {
		trim(key);
		for (const auto & rowKeys : ImHotKey::Keys) {
			for (const auto &iKey : rowKeys) {
				if (iKey.lib && key._Equal(iKey.lib)) {
					if (scanCodeCount == 4)
						break;
					scanCodes[scanCodeCount] = static_cast<unsigned char>(iKey.scanCodePage1);
					order[scanCodeCount] = static_cast<unsigned char>(iKey.order);

					scanCodeCount++;
					break;
				}
			}
		}
	}

	return ImHotKey::GetOrderedScanCodes(scanCodes, order);
}

const ImHotKey::HotKey* HotKeyService::getHotKeyByName(const std::string& name) const {
	for (const ImHotKey::HotKey& hotkey : hotkeys_) {
		if (name._Equal(hotkey.functionName))
			return &hotkey;
	}
	return nullptr;
}

void HotKeyService::appendHotKey(ImHotKey::HotKey& hotKey) {
	hotkeys_.emplace_back(hotKey);
}

ImHotKey::HotKey* HotKeyService::getHotKey(bool repeat) {
	return GetHotKey(hotkeys_.data(), hotkeys_.size(), repeat);
}
