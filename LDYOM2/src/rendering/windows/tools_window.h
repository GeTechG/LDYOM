#pragma once
#include "modal_popup_window.h"

class ToolsWindow : public ModalPopupWindow {
	static void renderContent(Window* window);

  public:
	ToolsWindow();
	~ToolsWindow() override = default;
};
