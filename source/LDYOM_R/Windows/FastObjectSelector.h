#pragma once
#include "AbstractWindow.h"
#include "ObjectSelectorPopup.h"

namespace Windows {
	class FastObjectSelector: public AbstractWindow {
	private:
		ObjectSelectorPopup objectSelectorPopup_;
		bool opened = false;
	public:
		void openWithCallback(const std::function<void(int)>& callbackSelect);
		void draw() override;
	};
}
