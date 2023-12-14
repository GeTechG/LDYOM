#pragma once
#include "AbstractWindow.h"
#include "Settings.h"

namespace Windows {
	struct SettingsData {
	public:
		int currentLanguage = 0;
		int currentTheme = 0;
		bool showEntitiesName = false;
		float distanceShowNames = 100.0f;
		float scaleUi = 1.0f;
		bool autoBindDeleteAfterLastObjective = true;
		bool autoBindRequireFields = true;
		std::vector<int> userPedsModels{};
		std::vector<std::string> userSpecialPedsModels{};
		std::vector<int> userVehiclesModels{};
		std::vector<int> userWeaponsModels{};
		std::vector<std::pair<std::string, std::string>> userPedAnimations{};
	};

	class SettingsWindow : public AbstractWindow {
	private:
		SettingsData settings_;

	public:
		void Init();
		void draw() override;
		void settingsUpdate();
	};
}
