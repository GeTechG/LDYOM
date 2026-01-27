#pragma once
#include "window.h"

class ProjectManager : public Window {
  private:
	int m_selectedProjectIndex = -1;
	char m_filterBuffer[256] = "";
	int m_indexToRemove = -1;
	std::string m_renameBuffer = "";
	bool m_openRenamePopup = false;

	static void renderContent(ProjectManager* window);
	static void renderHeaderBar();
	static void renderTopButtons(ProjectManager* window, float spacing);
	static void renderProjectList(ProjectManager* window, float spacing, float width);
	static void renderSidebar(ProjectManager* window, float sidebarWidth, float* maxButtonWidth);

  public:
	ProjectManager();
	~ProjectManager() override = default;
	void open() override;
};