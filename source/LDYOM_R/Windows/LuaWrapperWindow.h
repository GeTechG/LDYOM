#pragma once
#include <map>
#include <sol.hpp>

#include "AbstractWindow.h"

namespace Windows {
	class LuaWrapperWindow : public AbstractWindow {
	private:
		std::optional<sol::function> drawFunction;

	public:
		LuaWrapperWindow();

		static std::map<std::string, sol::function> mainMenuRender;

		void draw() override;

		std::optional<sol::function>& getDrawFunction();
		void setDrawFunction(const sol::function &drawFunction);

		void reset();
	};
}
