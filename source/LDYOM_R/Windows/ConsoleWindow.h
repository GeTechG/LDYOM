#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class ConsoleWindow final : public AbstractWindow {
	public:
		void draw() override;
	};
}
