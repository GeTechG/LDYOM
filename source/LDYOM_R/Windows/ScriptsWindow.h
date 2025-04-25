#pragma once
#include <list>
#include <string>

#include "AbstractWindow.h"

namespace Windows {
	class ScriptsWindow : public AbstractWindow {
	private:
		std::list<std::string> unloadedScripts{};
		std::list<std::string> loadedScripts{};

	public:
		void Init();
		void draw() override;
	};
}
