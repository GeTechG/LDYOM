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
#include "FileWatcher.h"
#include "WindowsRenderService.h"
#include "HotkeysExecutor.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "vehicle_renderer.h"
#include "Windows/QuickCommandsWindow.h"
#include "Windows/NameEntitiesRender.h"
#include "Windows/PopupSkinSelector.h"
#include "Windows/PopupWeaponSelector.h"
#include "Windows/VehiclesWindow.h"
#include "Windows/ActorsWindow.h"
#include "Windows/ObjectivesWindow.h"
#include "Data/CheckpointObjective.h"
#include "Windows/FastObjectSelector.h"
#include "Windows/ObjectsWindow.h"
#include "Windows/ProjectInfoWindow.h"

using namespace plugin;

bool init = false;

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
		};

		auto scriptProcess = [] {
			Tasker::getInstance().process();
			if (!ProjectPlayerService::getInstance().isProjectRunning()) {
				for (const auto& objective : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives()) {
					//Checkpoint
					if (auto* checkpoint = dynamic_cast<CheckpointObjective*>(objective.get())) {
						checkpoint->render();
					}
				}
			}
		};

		auto initFunc = [this, &gameProcces, &scriptProcess] {
			//init services
			Logger::getInstance().Init();
			Settings::getInstance().Init();
			Localization::getInstance().Init();
			HotKeyService::getInstance().Init();
			ProjectsService::getInstance().Init();
			ModelsService::getInstance().Init();
			PopupSkinSelector::getInstance().Init();
			PopupWeaponSelector::getInstance().Init();
			PopupSpriteBlipSelector::Init();

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
			Windows::WindowsRenderService::getInstance().getWindows().emplace_back(std::make_unique<Windows::FastObjectSelector>());
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

			LuaEngine::getInstance().Init();

			defaultWindow = Windows::WindowsRenderService::getInstance().getWindow<Windows::MainMenu>();

			////init is complete
			init = true;

			Events::gameProcessEvent.AddAtId(777, gameProcces);
			Events::processScriptsEvent.AddAtId(778, scriptProcess);
		};

		Events::initGameEvent += [=]{
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

			if (!init) {
				initFunc();
			} else {
				Events::gameProcessEvent.AddAtId(777, gameProcces);
				Events::processScriptsEvent.AddAtId(778, scriptProcess);
				FileWatcher::start();
			}

			restart = true;
			ProjectsService::getInstance().getCurrentProject() = ProjectData();
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
	}
} ldyomr;