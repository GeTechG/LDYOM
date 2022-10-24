#include <CWorld.h>
#include <CHud.h>
#include <CMenuManager.h>
#include <CTheScripts.h>

#include "HotKeyService.h"
#include "plugin.h"
#include "Logger.h"
#include "LuaEngine.h"
#include "Settings.h"
#include "Tasker.h"
#include "Localization/Localization.h"
#include "Windows/EntitiesWindow.h"
#include "imgui_notify.h"
#include "additional_events.h"

#include "Windows/MainMenu.h"
#include "Windows/SettingsWindow.h"
#include "ProjectsService.h"

#include "CGame.h"
#include <extensions/ScriptCommands.h>
#include "FileWatcher.h"
#include "WindowsRenderService.h"
#include "HotkeysExecutor.h"
#include "LuaLogger.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "SaveService.h"
#include "vehicle_renderer.h"
#include "Windows/QuickCommandsWindow.h"
#include "Windows/NameEntitiesRender.h"
#include "Windows/PopupSkinSelector.h"
#include "Windows/PopupWeaponSelector.h"
#include "Windows/VehiclesWindow.h"
#include "Windows/ActorsWindow.h"
#include "Windows/ObjectivesWindow.h"
#include "Windows/FastObjectSelector.h"
#include "Windows/ObjectsWindow.h"
#include "Windows/ParticlesWindow.h"
#include "Windows/PickupsWindow.h"
#include "Windows/ProjectInfoWindow.h"
#include "Windows/PyrotechnicsWindow.h"
#include "Windows/TrainsWindow.h"
#include "Data/Audio.h"
#include "easylogging/easylogging++.h"
#include "Windows/AudioWindow.h"
#include "Windows/CheckpointsWindow.h"
#include "Windows/ConsoleWindow.h"
#include "Windows/FAQWindow.h"
#include "Windows/GlobalVariablesWindow.h"
#include "Windows/InfoWindow.h"
#include "Windows/NodeEditorWindow.h"
#include "Windows/SaveConfirmPopup.h"
#include "Windows/VisualEffectsWindow.h"

using namespace plugin;

bool initServices = false;

extern bool isInitImgui;

bool restart = false;

class LDYOMR
{

public:
	LDYOMR()
	{
		auto gameProcces = [] {
			if (isInitImgui && !ImGui::GetIO().WantTextInput && Windows::WindowsRenderService::getInstance().isRenderWindows())
				hotkeysExecute();

			FileWatcher::runTasks();

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
			LuaEngine::getInstance().Init();
			HotKeyService::getInstance().Init();
			ProjectsService::getInstance().Init();
			ModelsService::getInstance().Init();
			PopupSkinSelector::getInstance().Init();
			PopupWeaponSelector::getInstance().Init();
			PopupSpriteBlipSelector::Init();
			SaveService::getInstance().Init();

			Windows::WindowsRenderService::getInstance().Init();

			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::MainMenu>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ProjectInfoWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ObjectivesWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::EntitiesWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::SettingsWindow>());
			Windows::WindowsRenderService::getInstance().getWindow<Windows::SettingsWindow>()->Init();
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ActorsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::VehiclesWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ObjectsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::TrainsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ParticlesWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::FastObjectSelector>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::PickupsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::PyrotechnicsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::AudioWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::VisualEffectsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::CheckpointsWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::ConsoleWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::NodeEditorWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::GlobalVariablesWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::SaveConfirmPopup>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::FAQWindow>());
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::InfoWindow>());
			Windows::WindowsRenderService::getInstance().addRender("showEntitiesName", [] {
				if (!ProjectPlayerService::getInstance().isProjectRunning()) {
					if (Settings::getInstance().get<bool>("main.showEntitiesName").value_or(false))
						NameEntitiesRender::draw();
				}
			});
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::QuickCommandsWindow>());

			Windows::WindowsRenderService::getInstance().addRender("renderNotifications", [] {
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
				ImGui::RenderNotifications(); // <-- Here we render all notifications
				ImGui::PopStyleVar(1); // Don't forget to Pop()
				ImGui::PopStyleColor(1);
			});

			Logger::getInstance().log("LDYOMR initialized.");

			defaultWindow = Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>();
			
			ProjectsService::getInstance().onUpdate().connect(Audio::loadAudioFilesList);

			////init is complete
			initServices = true;

			const auto initFuncs = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["init"].get_or_create<sol::table>();
			for (auto pair : initFuncs) {
				if (auto result = pair.second.as<sol::function>()(); !result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
					LuaLogger::getInstance().print(err.what());
				}
			}

			Tasker::getInstance().getKtcoroTaskList().add_task([]() -> ktwait {
				co_await 5s;
				Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);

				//Command<Commands::MAKE_PLAYER_FIRE_PROOF>(0, true);
			});

			Tasker::getInstance().getKtcoroTaskList().add_task([]() -> ktwait {
				co_await 4s;

				static std::array<char, TEXT_SIZE> welcomeS;

				char openMenuHotKey[32];
				ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("openMenu")->functionKeys, openMenuHotKey, sizeof openMenuHotKey);

				const auto welcomeTemplate = Localization::getInstance().get("welcome_box.text");
				const std::string welcome = utils::stringFormat(welcomeTemplate, LDYOM_VERSION_STRING, openMenuHotKey);

				auto cp1251Text = utf8ToCp1251(welcome);
				gxtEncode(cp1251Text);
				strlcpy(welcomeS.data(), cp1251Text.c_str(), sizeof welcomeS);

				CHud::SetHelpMessage(welcome.c_str(), false, false, false);
				CHud::DrawHelpText();
			});

			Events::gameProcessEvent.AddAtId(777, gameProcces);
			Events::processScriptsEvent.AddAtId(778, scriptProcess);
		};

		Events::initGameEvent += [=]{
			CTheScripts::ScriptSpace = *reinterpret_cast<char**>(0x44CA42 + 2);

			//MPACK7
			if (CGame::bMissionPackGame != 7)
				return;

			initFunc();
			FileWatcher::start();
		};

		Events::reInitGameEvent += [&] {
			//MPACK7
			if (CGame::bMissionPackGame != 7) {
				Windows::WindowsRenderService::getInstance().closeAllWindows();
				Windows::WindowsRenderService::getInstance().setMouseShown(false);
				Events::gameProcessEvent.RemoveById(777);
				Events::gameProcessEvent.RemoveById(778);
				FileWatcher::stop();
				Tasker::getInstance();
				return;
			}

			if (!initServices) {
				initFunc();
			} else {
				Events::gameProcessEvent.AddAtId(777, gameProcces);
				Events::processScriptsEvent.AddAtId(778, scriptProcess);
				FileWatcher::start();
			}

			restart = true;
			ProjectsService::getInstance().getCurrentProject() = ProjectData();
			ProjectsService::getInstance().Reset();
			ProjectPlayerService::getInstance().reset();
			defaultWindow = Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>();
			restart = false;
		};
		Events::vehicleRenderEvent.before += [](CVehicle* veh)
		{
			if (veh)
				VehicleRenderer::get()->processRender(veh);
		};
		Events::vehicleResetAfterRender += [](CVehicle* veh)
		{
			if (veh)
				VehicleRenderer::get()->postRender(veh);
		};
		Events::initRwEvent += [] {
			
		};
		Events::shutdownPoolsEvent.before += [] {
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadProjectScene();
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadEditorScene();
		};
	}
} ldyomr;