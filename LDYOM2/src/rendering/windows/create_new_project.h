#pragma once
#include "modal_popup_window.h"
#include <array>
#include <string>

class CreateNewProject : public ModalPopupWindow {
  private:
	std::array<char, 256> authorNameBuffer = {};
	std::array<char, 256> projectNameBuffer = {};

	static void renderContent(CreateNewProject* window);

  public:
	CreateNewProject();
	~CreateNewProject() override = default;
	void open() override;
};