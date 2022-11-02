#pragma once
#include <set>
#include <sol.hpp>

#include "AbstractWindow.h"

namespace Windows {
	class NodeEditorWindow: public AbstractWindow {
	private:
		std::set<int> favoritesNodes_;

		void newNodePopup(sol::table globalData, sol::table ed, bool* isCreateNode);
		void saveFavoritesNodes();
	public:
		NodeEditorWindow();
		void draw() override;
		void close() override;
	};
}
