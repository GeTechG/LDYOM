#pragma once
#include "window.h"

class MainMenu : public Window {
	static void renderContent(Window* window);

  public:
	MainMenu();
	~MainMenu() override = default;
};