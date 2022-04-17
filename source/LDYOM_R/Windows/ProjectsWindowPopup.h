#pragma once

namespace Windows {
	class ProjectsWindowPopup {
	private:
		bool show_ = false;
	public:
		void draw();
		void setShow(bool show);
	};
}
