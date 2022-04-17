#pragma once
#include "IWindow.h"
#include "Settings.h"

namespace Windows
{
	class SettingsData
	{
	public:
		int currentLanguage = 0;
		int currentTheme = 0;
		SettingsData() = default;
	};

	class SettingsWindow : public IWindow
	{
	private:
		SettingsData settings_;
	public:
		void Init();
		void draw() override;
		void settingsUpdate();
	};
}