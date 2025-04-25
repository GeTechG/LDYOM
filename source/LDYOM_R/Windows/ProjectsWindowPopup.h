#pragma once
#include <memory>

#include "PopupWarning.h"
#include "../Data/Texture.h"
#include "Localization/Localization.h"

struct ProjectInfo;
using ProjectsInfos = std::vector<std::unique_ptr<ProjectInfo>>;

namespace Windows {
	class ProjectsWindowPopup {
	private:
		bool show_ = false;
		std::unique_ptr<Texture> missionIcon_;
		PopupWarning popupWarningLoadProject_;
		PopupWarning popupWarningDeleteProject_;

		void projectInfo(Localization &local, const ProjectsInfos &projectInfos) const;

	public:
		ProjectsWindowPopup();
		void draw();
		void setShow(bool show);
	};
}
