#include "LuaWrapperWindow.h"

#include "LuaEngine.h"

std::map<std::string, sol::function> Windows::LuaWrapperWindow::mainMenuRender;

bool connectToLua = false;

Windows::LuaWrapperWindow::LuaWrapperWindow() {
	if (!connectToLua) {
		connectToLua = true;
		LuaEngine::getInstance().getOnReset().connect([this]() {
			reset();
		});
	}
}

void Windows::LuaWrapperWindow::draw() {
	if (drawFunction) {
		if (const auto result = drawFunction.value()(); !result.valid()) {
			LuaEngine::errorHandler(result);
		}
	}
}

std::optional<sol::function>& Windows::LuaWrapperWindow::getDrawFunction() { return drawFunction; }

void Windows::LuaWrapperWindow::setDrawFunction(const sol::function &drawFunction) {
	this->drawFunction = drawFunction;
}

void Windows::LuaWrapperWindow::reset() {
	this->drawFunction = std::nullopt;

	// static
	mainMenuRender.clear();
}
