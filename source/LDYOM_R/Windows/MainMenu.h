#pragma once
#include "IWindow.h"
#include "ProjectsWindowPopup.h"

namespace Windows
{
	class MainMenu : public IWindow
	{
	private:
		ProjectsWindowPopup projectsWindowPopup_;
	public:
		void draw() override;
	};
}

