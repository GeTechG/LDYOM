#pragma once
#include <sol.hpp>

#include "AbstractWindow.h"

namespace Windows {
	class NodeEditorWindow: public AbstractWindow {
	private:
		void newNodePopup(sol::table globalData, sol::table ed, bool* isCreateNode);
	public:
		void draw() override;
	};
}
