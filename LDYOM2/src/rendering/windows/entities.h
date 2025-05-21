#pragma once
#include "entity.h"
#include "window.h"
#include <vector>

enum EntitiesWindowType { EntitiesWindowType_Actor, EntitiesWindowType_Car, EntitiesWindowType_Object };

class EntitiesWindow : public Window {
  private:
	int m_selectedEntityIndex = -1;
	int m_indexToRemove = -1;
	std::string m_renameBuffer = "";
	EntitiesWindowType m_windowType = EntitiesWindowType_Actor;

	static void renderContent(EntitiesWindow* window);
	static void renderEntity(EntitiesWindow* window, const Entity& entity, int i);

  public:
	EntitiesWindow();
	~EntitiesWindow() override = default;

	int getSelectedEntityIndex() const { return m_selectedEntityIndex; }
	void setSelectedEntityIndex(int index) { m_selectedEntityIndex = index; }
	void setWindowType(EntitiesWindowType type) { m_windowType = type; }
};