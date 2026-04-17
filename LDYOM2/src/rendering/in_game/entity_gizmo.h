#pragma once

#include <CVector.h>

class Entity;

enum class GizmoOperation {
	TRANSLATE,
	ROTATE,
};

enum class GizmoMode {
	LOCAL,
	GLOBAL,
};

class EntityGizmo {
  private:
	static Entity* m_entity;
	static GizmoOperation m_currentOperation;
	static GizmoMode m_currentMode;

  public:
	static void activate(Entity* entity) noexcept;
	static void deactivate() noexcept;
	static bool isActive() noexcept { return m_entity != nullptr; }
	static GizmoOperation getCurrentOperation() noexcept { return m_currentOperation; }
	static GizmoMode getCurrentMode() noexcept { return m_currentMode; }
	static void render(CVector eye, CVector center) noexcept;
};
