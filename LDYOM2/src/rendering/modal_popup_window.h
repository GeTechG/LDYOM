#pragma once
#include "window.h"

class ModalPopupWindow : public Window {
  private:
	bool m_needOpen = false;
	bool m_needClose = false;

  public:
	ModalPopupWindow(const std::string& title);
	~ModalPopupWindow() override = default;
	void render() override;
	void open() override;
	void close() override;
};
