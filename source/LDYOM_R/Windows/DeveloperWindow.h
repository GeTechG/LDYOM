#pragma once

#include "AbstractWindow.h"

namespace Windows {
	class DeveloperWindow final : public AbstractWindow {
	public:
		void draw() override;
	};
}
