#pragma once
#include <functional>
#include "../Data/INameable.h"

class PopupWarning: public INameable {
	std::string name;
public:
	PopupWarning(std::string name = "warning_popup.title");
	char* getName() override;
	void draw(const std::function<void()>& onDelete);
};
