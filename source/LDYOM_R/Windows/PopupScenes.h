#pragma once
#include "AbstractWindow.h"
#include "imgui.h"
#include "PopupWarning.h"

namespace Windows {
	class PopupScenes: AbstractWindow {
	private:
		PopupWarning popupDelete_;
	public:
		void open() override;
		void draw() override;
	};
}
