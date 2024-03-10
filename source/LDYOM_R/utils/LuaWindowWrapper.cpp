#include <sol.hpp>

#include "LuaEngine.h"
#include "LuaWrapperWindow.h"
#include "MainMenu.h"
#include "WindowsRenderService.h"

void luaWindowWrapper(sol::state &state) {
	auto ldyomTable = state["ld"].get_or_create<sol::table>();
	auto windowTable = ldyomTable["window"].get_or_create<sol::table>();
	windowTable["openLuaWindow"] = [](const sol::function &drawFunction) {
		const auto luaWrapperWindow = Windows::WindowsRenderService::getInstance().getWindow<
			Windows::LuaWrapperWindow>();
		luaWrapperWindow->open();
		luaWrapperWindow->setDrawFunction(drawFunction);
	};
	windowTable["closeLuaWindow"] = []() {
		Windows::WindowsRenderService::getInstance().getWindow<Windows::LuaWrapperWindow>()->close();
	};
	windowTable["closeMainMenu"] = []() {
		Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>()->close();
	};
	windowTable["openMainMenu"] = []() {
		Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>()->open();
	};
	windowTable["replaceWindow"] = [](const std::function<void()> &executeFunc, const std::function<void()> &undoFunc) {
		Windows::WindowsRenderService::getInstance().addCommand(
			std::make_unique<CustomWindowsRenderCommand>(executeFunc, undoFunc));
	};
	windowTable["setMainMenuRender"] = [](const std::string &name, const sol::function &drawFunction) {
		if (Windows::LuaWrapperWindow::mainMenuRender.contains(name)) {
			Windows::LuaWrapperWindow::mainMenuRender[name] = drawFunction;
		} else {
			Windows::LuaWrapperWindow::mainMenuRender.emplace(name, drawFunction);
		}
	};
	windowTable["removeMainMenuRender"] = [](const std::string &name) {
		Windows::LuaWrapperWindow::mainMenuRender.erase(name);
	};
	windowTable["luaWindowState"] = Windows::WindowsRenderService::getInstance().getWindow<
		Windows::LuaWrapperWindow>()->isShow();
	windowTable["setRenderWindows"] = [](const bool windowState) {
		Windows::WindowsRenderService::getInstance().setRenderWindows(windowState);
	};
	windowTable["addRender"] = [](const std::string &name, const sol::function &drawFunction) {
		const auto newFunc = sol::function(LuaEngine::getInstance().getLuaState(), drawFunction);
		Windows::WindowsRenderService::getInstance().addRender(name, [=]() {
			if (const auto result = newFunc(); !result.valid()) {
				LuaEngine::errorHandler(result);
			}
		});
	};
	windowTable["removeRender"] = [](const std::string &name) {
		Windows::WindowsRenderService::getInstance().removeRender(name);
	};
}
