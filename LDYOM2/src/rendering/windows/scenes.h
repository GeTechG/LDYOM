#pragma once
#include "window.h"
#include <vector>

class ScenesWindow : public Window {
  private:
	static void renderContent(ScenesWindow* window);
	int m_selectedSceneIndex = -1;
	int m_indexToRemove = -1;

  public:
	ScenesWindow();
	~ScenesWindow() override = default;
};