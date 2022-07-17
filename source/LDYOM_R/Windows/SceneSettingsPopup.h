#pragma once

namespace Windows {
	class SceneSettingsWindowPopup {
	private:
		bool show_ = false;
	public:
		//SceneSettingsWindowPopup();
		void draw();
		void setShow(bool show);
	};
}