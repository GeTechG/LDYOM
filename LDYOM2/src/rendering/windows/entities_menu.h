#pragma once
#include "window.h"

class EntitiesMenu : public Window {
	static void renderContent(Window* window);

  public:
	EntitiesMenu();
	~EntitiesMenu() override = default;
};