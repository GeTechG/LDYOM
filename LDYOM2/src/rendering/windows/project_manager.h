#pragma once
#include "window.h"

class ProjectManager : public Window {
  private:
	int m_selectedProjectIndex = -1;

	static void renderContent(ProjectManager* window);
	static void renderHeaderBar();
	static void renderTopButtons(float spacing);
	static void renderProjectList(ProjectManager* window, float spacing, float width);
	static void renderSidebar(ProjectManager* window, float sidebarWidth, float* maxButtonWidth);

  public:
	ProjectManager();
	~ProjectManager() override = default;
	void open() override;
};