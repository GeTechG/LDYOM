#pragma once
#include "modal_popup_window.h"

class QuickSettings : public ModalPopupWindow {
	static void renderContent(Window* window);

	bool m_onboardingMode = false;

  public:
	QuickSettings();
	~QuickSettings() override = default;

	void setOnboardingMode(bool value) { m_onboardingMode = value; }
	bool isOnboardingMode() const { return m_onboardingMode; }

	void render() override;
	void close() override;
};