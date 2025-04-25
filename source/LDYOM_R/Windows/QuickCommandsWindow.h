#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class QuickCommandsWindow : public AbstractWindow {
	public:
		void draw() override;
	};
}
