#pragma once
#include "window.h"
#include <vector>

class ObjectivesWindow : public Window {
  private:
	static void renderContent(ObjectivesWindow* window);
	int m_selectedObjectiveIndex = -1;
	int m_indexToRemove = -1;
	std::string m_renameBuffer = "";

  public:
	ObjectivesWindow();
	~ObjectivesWindow() override = default;

	int getSelectedObjectiveIndex() const { return m_selectedObjectiveIndex; }
};