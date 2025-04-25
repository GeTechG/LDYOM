#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class ProjectInfoWindow: public AbstractWindow {
	public:
		void draw() override;
	};
}
