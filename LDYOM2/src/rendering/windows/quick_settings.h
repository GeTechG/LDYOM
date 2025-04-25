#pragma once
#include "modal_popup_window.h"

class QuickSettings : public ModalPopupWindow {
	static void renderContent(Window* window);

  public:
	QuickSettings();
	~QuickSettings() override = default;
};