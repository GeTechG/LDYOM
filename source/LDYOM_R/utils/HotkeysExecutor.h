#pragma once
#include "HotKeyService.h"
#include "WindowsRenderService.h"

namespace Windows {
	class MainMenu;
}

inline Windows::IWindow *defaultWindow;

void inline hotkeysExecute() {
	if (const auto hotkey = HotKeyService::getInstance().getHotKey()) {
		if (std::strcmp(hotkey->functionName,"openMenu") == 0) {
			static bool openMenu = false;
			openMenu ^= TRUE;
			if (openMenu)
				defaultWindow->open();
			else
				Windows::WindowsRenderService::getInstance().closeAllWindows();

			Windows::WindowsRenderService::getInstance().setMouseShown(openMenu);
		}
	}
}
