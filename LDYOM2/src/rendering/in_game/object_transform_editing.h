#pragma once

#include <CObject.h>
#include <CQuaternion.h>
#include <array>
#include <functional>
#include <vector>

class CPlayerPed;

struct ObjectTransformEditingCallbackData {
	bool saveChanges;
	std::array<float, 3> position;
	CQuaternion rotation;
	std::array<float, 3> scale;
};

class ObjectTransformEditing {
  private:
	static std::array<float, 3> m_position;
	static CQuaternion m_rotation;
	static std::array<float, 3> m_scale;
	static CObject* m_object;
	static std::function<void(ObjectTransformEditingCallbackData)> m_onCloseCallback;
	static CPlayerPed* playerPed;

	static void render() noexcept;

  public:
	static void openTransformEditing(CObject* object, std::array<float, 3> position, CQuaternion rotation,
	                                 std::array<float, 3> scale,
	                                 std::function<void(ObjectTransformEditingCallbackData)> onClose) noexcept;
	static void closeTransformEditing(bool saveChanges) noexcept;
};