#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class InfoWindow: public AbstractWindow {
	public:
		void draw() override;
	};
}
