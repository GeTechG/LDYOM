#pragma once
#include "modal_popup_window.h"

class InfoWindow : public ModalPopupWindow {
	static void renderContent(Window* window);

  public:
	InfoWindow();
	~InfoWindow() override = default;
};
