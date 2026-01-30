#pragma once
#include "window.h"
#include <set>
#include <string>
#include <uuid_wrap.h>

class Entity;

/**
 * Window for managing CarRec vehicle recording paths
 * Provides UI for viewing, creating, editing, and deleting recorded paths
 */
class CarrecPathsWindow : public Window {
  private:
	static void renderContent(CarrecPathsWindow* window);
	void selectVehiclePopup(const char* namePopup, bool* open, const std::function<void(int)>& callback);

	int m_selectedPathIndex = -1;
	int m_indexToRemove = -1;
	std::string m_renameBuffer = "";
	bool m_showSelectVehiclePopup = false;
	bool m_showExistingSelectVehiclePopup = false;
	int m_editPathIndex = -1;
	int m_selectedVehicleModel = -1;
	std::set<int> m_useInPreview;
	std::set<int> m_useAIRerecord;

  public:
	CarrecPathsWindow();
	~CarrecPathsWindow() override = default;
};
