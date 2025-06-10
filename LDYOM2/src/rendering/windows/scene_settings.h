#pragma once
#include "modal_popup_window.h"

struct Scene;

class SceneSettingsPopup : public ModalPopupWindow {
	static void renderContent(Window* window); // Helper functions for rendering different sections
	static void renderGroupRelationsSection(Scene& scene, float contentWidth);
	static void renderTimeSection(Scene& scene, float contentWidth);
	static void renderMissionTimeSection(Scene& scene, float contentWidth);
	static void renderTrafficSection(Scene& scene, float contentWidth);
	static void renderWantedLevelSection(Scene& scene, float contentWidth);
	static void renderEnvironmentSection(Scene& scene, float contentWidth);

	// Helper function for two-column layout
	static void renderTwoColumnRow(const char* label, const std::function<void()>& renderControl,
	                               float labelWidth = 120.0f);

  public:
	SceneSettingsPopup();
	~SceneSettingsPopup() override = default;
};