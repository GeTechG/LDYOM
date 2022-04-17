#include "SettingsWindow.h"
#include "imgui.h"
#include "fa.h"
#include <fmt/core.h>

#include "HotKeyService.h"
#include "Settings.h"
#include "utils.h"
#include "Localization/Localization.h"
#include "imHotKey.h"
#include "WindowsRenderService.h"

using namespace std;

namespace Windows
{
	void SettingsWindow::Init()
	{
		settingsUpdate();
		Settings::getInstance().onUpdate().connect([this] { settingsUpdate(); });
	}

	void SettingsWindow::settingsUpdate()
	{
		this->settings_.currentLanguage = ranges::find(Settings::getInstance().listLocalizations(), Settings::getInstance().get<std::string>("main.language")) - Settings::getInstance().listLocalizations().begin();
		this->settings_.currentTheme = ranges::find(Settings::getInstance().listThemes(), Settings::getInstance().get<std::string>("main.theme")) - Settings::getInstance().listThemes().begin();
	}

	void SettingsWindow::draw()
	{
		static auto& local = Localization::getInstance();

		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
		if (ImGui::Begin(fmt::format("{} {}", ICON_FA_COGS, local.get("settings.title")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetNextItemWidth(150.0f);
			if (utils::Combo(local.get("settings.languages").c_str(), &this->settings_.currentLanguage, Settings::getInstance().listLocalizations()))
			{
				Settings::getInstance().set("main.language", Settings::getInstance().listLocalizations().at(this->settings_.currentLanguage));
				Localization::getInstance().Update();
				Settings::getInstance().Save();
			}
			ImGui::SetNextItemWidth(150.0f);
			if (utils::Combo(local.get("settings.themes").c_str(), &this->settings_.currentTheme, Settings::getInstance().listThemes()))
			{
				Settings::getInstance().set("main.theme", Settings::getInstance().listThemes().at(this->settings_.currentTheme));
				WindowsRenderService::getInstance().style();
				Settings::getInstance().Save();
			}

			ImGui::SetNextItemWidth(150.0f);
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_FIRE, local.get("settings.edit_hotkeys").c_str()).c_str()))
			{
				ImGui::OpenPopup(local.get("hotkey_editor.title").c_str());
			}
			ImHotKey::Edit(HotKeyService::getInstance().getHotkeys().data(),
			               HotKeyService::getInstance().getHotkeys().size(), local.get("hotkey_editor.title").c_str(),
			               [](ImHotKey::HotKey& hotKey) { HotKeyService::getInstance().onEditHotKey(hotKey); });

		}
		ImGui::End();
	}
}
