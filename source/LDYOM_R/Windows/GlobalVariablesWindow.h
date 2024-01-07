#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class GlobalVariablesWindow final : public AbstractWindow {
	public:
		void draw() override;
	};
}
