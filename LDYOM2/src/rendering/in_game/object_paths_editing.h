#pragma once

#include <CObject.h>
#include <CQuaternion.h>
#include <array>
#include <components/object/object_move_by_path.h>
#include <functional>
#include <vector>

class CPlayerPed;

class ObjectPathsEditing {
  private:
	static std::vector<components::PathPoint> m_points;
	static std::function<void(bool, const std::vector<components::PathPoint>&)> m_onCloseCallback;
	static size_t m_currentPointIndex;
	static std::array<float, 3> m_tempPosition;
	static CQuaternion m_tempRotation;
	static CObject* m_object;
	static CPlayerPed* playerPed;

	static void render() noexcept;

  public:
	static void openPathEditor(CObject* object, const std::vector<components::PathPoint>& points,
	                           std::function<void(bool, const std::vector<components::PathPoint>&)> onClose) noexcept;
	static void closePathEditor(bool saveChanges) noexcept;
};
