#pragma once
#include "window.h"
#include <vector>

class ScenesSettings : public Window {
  private:
	static void renderContent(ScenesSettings* window);
	int m_selectedSceneIndex = -1;

  public:
	ScenesSettings();
	~ScenesSettings() override = default;
};