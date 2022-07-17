#pragma once
#include "AbstractWindow.h"
#include "PopupScenes.h"
#include "ProjectsWindowPopup.h"
#include "SceneSettingsPopup.h"

namespace Windows
{
	class MainMenu : public AbstractWindow
	{
	private:
		ProjectsWindowPopup projectsWindowPopup_;
		PopupScenes popupScenes_;
		SceneSettingsWindowPopup sceneSettings_;
	public:
		void draw() override;
	};
}

