#pragma once
#include "HotKeyService.h"
#include "ProjectPlayerService.h"
#include "WindowsRenderService.h"
#include "../Windows/QuickCommandsWindow.h"

namespace Windows {
	class QuickCommandsWindow;
	class MainMenu;
}

inline std::optional<Windows::AbstractWindow*> defaultWindow;

inline bool openWindowsMenu = false;
void inline hotkeysExecute() {

	static bool openPress = false;
	if (const auto hotkey = HotKeyService::getInstance().getHotKey(true)) {
		if (std::strcmp(hotkey->functionName,"openMenu") == 0 && !openPress) {
			openWindowsMenu ^= TRUE;
			if (openWindowsMenu && defaultWindow.has_value())
				defaultWindow.value()->open();
			else
				Windows::WindowsRenderService::getInstance().closeAllWindows();
			openPress = true;
		}
	} else {
		openPress = false;
	}
	static bool openFastMenu = false;
	openFastMenu = false;
	if (const auto hotkey = HotKeyService::getInstance().getHotKey(true)) {
		if (std::strcmp(hotkey->functionName, "openQuickCommandsMenu") == 0 && !openWindowsMenu) {
			openFastMenu = true;
		}
	}
	if (openFastMenu)
		Windows::WindowsRenderService::getInstance().getWindow<Windows::QuickCommandsWindow>()->open();
	else
		Windows::WindowsRenderService::getInstance().getWindow<Windows::QuickCommandsWindow>()->close();

	const bool editorMouseState = (openWindowsMenu || openFastMenu) && !ProjectPlayerService::getInstance().isProjectRunning();

	Windows::WindowsRenderService::getInstance().setMouseShown(editorMouseState);
}
