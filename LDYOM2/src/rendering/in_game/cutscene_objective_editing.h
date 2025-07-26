#pragma once

#include <CVector.h>
#include <array>
#include <functional>
#include <objectives/cutscene.h>
#include <vector>

class CutsceneObjectiveEditing {
  private:
	static objectives::cutscene::Data m_data;
	static CObject* m_object;
	static std::function<void(bool, objectives::cutscene::Data)> m_onCloseCallback;
	static CPlayerPed* playerPed;

	static void render() noexcept;
	static void updateCutsceneObject() noexcept;

  public:
	static void openCutsceneEditor(objectives::cutscene::Data& data,
	                               std::function<void(bool, objectives::cutscene::Data)> onClose) noexcept;
	static void closeCutsceneEditor(bool saveChanges) noexcept;
};