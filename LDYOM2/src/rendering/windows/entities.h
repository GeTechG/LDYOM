#pragma once
#include "entity.h"
#include "window.h"
#include <vector>

enum EntitiesWindowType {
	EntitiesWindowType_Actor,
	EntitiesWindowType_Vehicle,
	EntitiesWindowType_Train,
	EntitiesWindowType_Object,
	EntitiesWindowType_Pickup,
	EntitiesWindowType_Firework,
	EntitiesWindowType_Particle,
	EntitiesWindowType_Checkpoint,
	EntitiesWindowType_Audio
};

class EntitiesWindow : public Window {
  private:
	int m_selectedEntityIndex = -1;
	int m_indexToRemove = -1;
	std::string m_renameBuffer = "";
	EntitiesWindowType m_windowType = EntitiesWindowType_Actor;

	// Отложенные операции с компонентами
	struct PendingComponentOperation {
		enum Type { Remove, Duplicate, MoveUp, MoveDown };
		Type type;
		size_t componentIndex;
	};
	std::vector<PendingComponentOperation> m_pendingComponentOperations;

	bool filterByType(Entity& entity);

	static void renderContent(EntitiesWindow* window);
	static void renderEntity(EntitiesWindow* window, const Entity& entity, int i);

  public:
	EntitiesWindow();
	~EntitiesWindow() override = default;

	void open() override {
		Window::open();
		// Restore previously selected entity (if any). close() tore down EntityOrbitCamera and EntityInfoPanel;
		// re-calling setSelectedEntityIndex with the saved index re-activates them. Mirrors ObjectivesWindow's
		// implicit persistence (it has no open() override and never resets m_selectedObjectiveIndex).
		if (m_selectedEntityIndex >= 0) {
			setSelectedEntityIndex(m_selectedEntityIndex);
		}
	}

	void close() override;

	int getSelectedEntityIndex() const { return m_selectedEntityIndex; }
	void setSelectedEntityIndex(int index);
	void setWindowType(EntitiesWindowType type) {
		if (m_windowType == type) {
			return;
		}
		m_windowType = type;
		setSelectedEntityIndex(-1);
	}
};