#include "FastObjectSelector.h"

extern bool openWindowsMenu;

void Windows::FastObjectSelector::openWithCallback(const std::function<void(int)>& callbackSelect) {
	AbstractWindow::open();
	this->objectSelectorPopup_.setCallbackSelect(callbackSelect);
	this->opened = true;
}

void Windows::FastObjectSelector::draw() {
	if (this->opened) {
		this->objectSelectorPopup_.open();
		this->opened = false;
	}

	this->objectSelectorPopup_.draw();
	if (!this->objectSelectorPopup_.isShow()) {
		openWindowsMenu = false;
		this->close();
	} 
}
