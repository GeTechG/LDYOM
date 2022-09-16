#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class GlobalVariablesWindow : public AbstractWindow {
	public:
		void draw() override;
	};
}
