#pragma once

#include "imHotKey.h"

class HotKeyService {
private:
	std::vector<ImHotKey::HotKey> hotkeys_;

	void loadHotKeys();

	HotKeyService() = default;
	HotKeyService(const HotKeyService&) = delete;
	HotKeyService& operator=(HotKeyService&) = delete;
public:
	static HotKeyService& getInstance() {
		static HotKeyService instance;
		return instance;
	}

	void Init();

	void onEditHotKey(ImHotKey::HotKey& hotKey);

	std::vector<ImHotKey::HotKey>& getHotkeys();

	unsigned int stringToKeyCombo(const std::string &combo);
	const ImHotKey::HotKey* getHotKeyByName(const std::string& name) const;

	void appendHotKey(ImHotKey::HotKey& hotKey);
	ImHotKey::HotKey* getHotKey(bool repeat = false);
};
