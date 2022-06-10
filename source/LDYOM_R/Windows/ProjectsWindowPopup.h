#pragma once
#include <memory>

#include "PopupWarning.h"
#include "../Data/Texture.h"
#include "Localization/Localization.h"

namespace Windows {
	class ProjectsWindowPopup {
	private:
		bool show_ = false;
		std::unique_ptr<Texture> missionIcon_;
		PopupWarning popupWarningNewProject_;
		PopupWarning popupWarningLoadProject_;
		PopupWarning popupWarningDeleteProject_;

		void projectInfo(Localization& local) const;
	public:
		ProjectsWindowPopup();
		void draw();
		void setShow(bool show);
	};
}
