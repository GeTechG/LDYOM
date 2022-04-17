#include <CMenuManager.h>

#include "HotKeyService.h"
#include "plugin.h"
#include "KeyCodes.h"
#include "Logger.h"
#include "LuaEngine.h"
#include "Settings.h"
#include "Tasker.h"
#include "Localization/Localization.h"
#include "Windows/EntitiesWindow.h"

#include "Windows/MainMenu.h"
#include "Windows/SettingsWindow.h"
#include "ProjectsService.h"
#include "Windows/ActorsWindow.h"

#include "CGame.h"
#include "WindowsRenderService.h"
#include "HotkeysExecutor.h"
#include "ModelsService.h"
#include "Windows/PopupSkinSelector.h"
#include "Windows/PopupWeaponSelector.h"

using namespace plugin;

bool init = false;

extern bool isInitImgui;

class LDYOMR
{
public:
	LDYOMR()
	{
		auto gameProcces = [] {
			Tasker::getInstance().list().process();
			if (isInitImgui)
				hotkeysExecute();
		};

		auto initFunc = [this, &gameProcces] {
			//init services
			Logger::getInstance().Init();
			Settings::getInstance().Init();
			Localization::getInstance().Init();
			HotKeyService::getInstance().Init();
			ProjectsService::getInstance().Init();
			ModelsService::getInstance().Init();
			PopupSkinSelector::getInstance().Init();
			PopupWeaponSelector::getInstance().Init();

			Windows::WindowsRenderService::getInstance().Init();

			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::MainMenu>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::EntitiesWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::SettingsWindow>());
			Windows::WindowsRenderService::getInstance().getWindow<Windows::SettingsWindow>()->Init();
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ActorsWindow>());

			Logger::getInstance().log("LDYOMR initialized.");

			LuaEngine::getInstance().Init();

			defaultWindow = Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>();

			////init is complete
			init = true;

			Events::gameProcessEvent.AddAtId(777, gameProcces);
		};

		Events::initGameEvent += [=]{
			//MPACK7
			if (CGame::bMissionPackGame != 7)
				return;

			initFunc();
		};

		Events::reInitGameEvent += [&] {
			//MPACK7
			if (CGame::bMissionPackGame != 7) {
				Windows::WindowsRenderService::getInstance().closeAllWindows();
				Windows::WindowsRenderService::getInstance().setMouseShown(false);
				Events::gameProcessEvent.RemoveById(777);
				return;
			}

			if (!init) {
				initFunc();
			} else {
				Events::gameProcessEvent += gameProcces;
			}
		};
		Events::initRwEvent += [] {
			
		};
	}
} ldyomr;