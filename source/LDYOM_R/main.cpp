#include <CHud.h>
#include <CMenuManager.h>
#include <CTheScripts.h>
#include <CWorld.h>

#include <extensions/ScriptCommands.h>
#include "additional_events.h"
#include "CGame.h"
#include "chrono"
#include "constants.h"
#include "DiscordService.h"
#include "FileWatcher.h"
#include "HotKeyService.h"
#include "HotkeysExecutor.h"
#include "ImGuiHook.h"
#include "ktcoro_wait.hpp"
#include "Logger.h"
#include "MainMenu.h"
#include "ModelsService.h"
#include "plugin.h"
#include "PopupSkinSelector.h"
#include "PopupSpriteBlipSelector.h"
#include "PopupWeaponSelector.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "SaveService.h"
#include "Settings.h"
#include "Tasker.h"
#include "vehicle_renderer.h"
#include "WindowsRenderService.h"
#include "Data/Audio.h"
#include "Localization/Localization.h"

using namespace plugin;
using namespace std::chrono_literals;

bool initServices = false;

extern bool isInitImgui;

bool restart = false;

class LDYOMR {
public:
	LDYOMR() {
		auto gameProcces = [] {
			if (isInitImgui && !ImGui::GetIO().WantTextInput && Windows::WindowsRenderService::getInstance().
				isRenderWindows())
				hotkeysExecute();

			FileWatcher::runTasks();
			DiscordService::getInstance().runCallbacks();

			static bool lastFrameMenuActive = false;
			if (lastFrameMenuActive && !FrontEndMenuManager.m_bMenuActive) {
				ImGui::GetIO().ClearInputKeys();
			}
			lastFrameMenuActive = FrontEndMenuManager.m_bMenuActive;
		};

		auto scriptProcess = [] {
			Tasker::getInstance().process();
			if (!ProjectPlayerService::getInstance().isProjectRunning()) {
				//for (const auto& objective : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives()) {
				//	//Checkpoint
				//	if (auto* checkpoint = dynamic_cast<CheckpointObjective*>(objective.get())) {
				//		checkpoint->render();
				//	}
				//}
			}

			if (Command<0x0ADC>("TOP2009")) {
				CHud::SetHelpMessage("~r~I LOVE TWENTY ONE PILOTS", false, false, false);
				CHud::DrawHelpText();
			}

			if (Command<0x0ADC>("MISSIOMAFIA")) {
				CHud::SetHelpMessage("~w~I MISSIO MAFIA", false, false, false);
				CHud::DrawHelpText();
			}
		};

		auto initFunc = [this, &gameProcces, &scriptProcess] {
			//init services
			Logger::getInstance().Init();
			Settings::getInstance().Init();
			Localization::getInstance().Init();
			//LuaEngine::getInstance().Init();
			HotKeyService::getInstance().Init();
			ProjectsService::getInstance().Init();
			ModelsService::getInstance().Init();
			PopupSkinSelector::getInstance().Init();
			PopupWeaponSelector::getInstance().Init();
			PopupSpriteBlipSelector::Init();
			SaveService::getInstance().Init();
			DiscordService::getInstance().Init();

			const auto projectName = ProjectsService::getInstance().getCurrentProject().getProjectInfo()->name;
			DiscordService::getInstance().updateActivity(projectName, DiscordActivityType::CREATING);

			ImGuiHook::scaleUi = Settings::getInstance().get<float>("main.scaleUi").value_or(1.0f);
			ImGuiHook::Hook();
			Windows::WindowsRenderService::getInstance().Init();

			Logger::getInstance().log("LDYOMR initialized.");

			defaultWindow = Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>();

			ProjectsService::getInstance().onUpdate().connect(Audio::loadAudioFilesList);

			////init is complete
			initServices = true;

			/*const auto initFuncs = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["init"].
				get_or_create<sol::table>();
			for (auto pair : initFuncs) {
				if (auto result = pair.second.as<sol::function>()(); !result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
					LuaLogger::getInstance().print(err.what());
				}
			}*/

			Tasker::getInstance().getKtcoroTaskList().add_task([]() -> ktwait {
				co_await 5s;
				Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
				Command<Commands::SET_CHAR_DROWNS_IN_WATER>(static_cast<CPed*>(FindPlayerPed()), false);
				FindPlayerPed()->GetPlayerInfoForThisPlayerPed()->m_bDoesNotGetTired = true;

				//Command<Commands::MAKE_PLAYER_FIRE_PROOF>(0, true);
			});

			Tasker::getInstance().getKtcoroTaskList().add_task([]() -> ktwait {
				co_await 4s;

				char openMenuHotKey[32];
				ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("openMenu")->functionKeys,
				                       openMenuHotKey, sizeof openMenuHotKey);

				const auto welcomeTemplate = Localization::getInstance().get("welcome_box.text");
				std::string welcome = utils::stringFormat(welcomeTemplate, LDYOM_VERSION_STRING, openMenuHotKey);

				auto cp1251Text = utf8ToCp1251(welcome);
				gxtEncode(cp1251Text);
				welcome = cp1251Text;

				CHud::SetHelpMessage(welcome.c_str(), false, false, false);
				CHud::DrawHelpText();
			});


			Events::gameProcessEvent.AddAtId(777, gameProcces);
			Events::processScriptsEvent.AddAtId(778, scriptProcess);
		};

		Events::initGameEvent += [=] {
			CTheScripts::ScriptSpace = *reinterpret_cast<char**>(0x44CA42 + 2);

			//MPACK7
			if (CGame::bMissionPackGame != 7)
				return;

			initFunc();
			FileWatcher::start();
		};

		Events::reInitGameEvent += [&] {
			Events::gameProcessEvent.RemoveById(777);
			Events::gameProcessEvent.RemoveById(778);
			FileWatcher::stop();

			//MPACK7
			if (CGame::bMissionPackGame != 7) {
				return;
			}

			if (!initServices) {
				initFunc();
			} else {
				Tasker::getInstance().Reset();
				ProjectsService::getInstance().getCurrentProject() = ProjectData();
				ProjectsService::getInstance().Reset();
				ProjectPlayerService::getInstance().reset();
				Windows::WindowsRenderService::getInstance().Reset();
				Events::gameProcessEvent.AddAtId(777, gameProcces);
				Events::processScriptsEvent.AddAtId(778, scriptProcess);
				FileWatcher::start();
			}

			restart = true;
			defaultWindow = Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>();
			restart = false;
		};
		Events::vehicleRenderEvent.before += [](CVehicle *veh) {
			if (veh)
				VehicleRenderer::get()->processRender(veh);
		};
		Events::vehicleResetAfterRender += [](CVehicle *veh) {
			if (veh)
				VehicleRenderer::get()->postRender(veh);
		};
		Events::initRwEvent += [] {};
		Events::shutdownPoolsEvent.before += [] {
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadProjectScene();
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadEditorScene();
		};
	}
} ldyomr;
