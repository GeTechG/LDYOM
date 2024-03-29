#pragma once
#include <map>
#include <sol.hpp>

#include "AbstractWindow.h"

namespace Windows {
	class LuaWrapperWindow : public AbstractWindow {
	private:
		std::optional<sol::protected_function> drawFunction;

	public:
		LuaWrapperWindow();

		static std::map<std::string, sol::protected_function> mainMenuRender;

		void draw() override;

		std::optional<sol::protected_function>& getDrawFunction();
		void setDrawFunction(const sol::protected_function &drawFunction);

		void reset();
	};
}
