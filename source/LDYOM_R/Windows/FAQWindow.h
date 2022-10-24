#pragma once
#include "AbstractWindow.h"

namespace Windows {
	class FAQWindow: public AbstractWindow {
	public:
		void draw() override;
	};
}
