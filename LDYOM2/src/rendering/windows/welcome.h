#pragma once
#include "window.h"

class Welcome : public Window {
  private:
	int m_selectedPreset = -1;

	static void renderContent(Welcome* window);
	void applyPreset(int preset);

  public:
	Welcome();
	~Welcome() override = default;
	void open() override;
};
