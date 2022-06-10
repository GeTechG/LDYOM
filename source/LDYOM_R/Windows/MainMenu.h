#pragma once
#include "AbstractWindow.h"
#include "PopupScenes.h"
#include "ProjectsWindowPopup.h"

namespace Windows
{
	class MainMenu : public AbstractWindow
	{
	private:
		ProjectsWindowPopup projectsWindowPopup_;
		PopupScenes popupScenes_;
	public:
		void draw() override;
	};
}

