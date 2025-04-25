#include "WindowsRenderService.h"

#include <CMenuManager.h>
#include <fmt/core.h>

#include "AbstractWindow.h"
#include "ActorsWindow.h"
#include "AudioWindow.h"
#include "CameraPathsWindow.h"
#include "CarrecPathsWindow.h"
#include "CheckpointsWindow.h"
#include "ConsoleWindow.h"
#include "DeveloperWindow.h"
#include "EntitiesWindow.h"
#include "FAQWindow.h"
#include "FastObjectSelector.h"
#include "GlobalVariablesWindow.h"
#include "ImGuiHook.h"
#include "imguistyleloader.h"
#include "imgui_notify.h"
#include "InfoWindow.h"
#include "Logger.h"
#include "LuaEngine.h"
#include "LuaWrapperWindow.h"
#include "MainMenu.h"
#include "NameEntitiesRender.h"
#include "ObjectivesWindow.h"
#include "ObjectsWindow.h"
#include "ParticlesWindow.h"
#include "PickupsWindow.h"
#include "PopupSkinSelector.h"
#include "PopupWeaponSelector.h"
#include "ProjectInfoWindow.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "PyrotechnicsWindow.h"
#include "QuickCommandsWindow.h"
#include "SaveConfirmPopup.h"
#include "ScriptsWindow.h"
#include "Settings.h"
#include "SettingsWindow.h"
#include "ToolsWindow.h"
#include "TrainsWindow.h"
#include "VehiclesWindow.h"
#include "VisualEffectsWindow.h"

bool reloadTheme = false;

extern std::optional<Windows::AbstractWindow*> defaultWindow;

void loadTheme() {
	if (!ImGui::StyleLoader::LoadFile(fmt::format("LDYOM/Themes/{}.toml",
	                                              Settings::getInstance().get<std::string>("main.theme").value()))) {
		Logger::getInstance().log("invalid load theme!");
	}
}

void addWindows() {
	auto &windows = Windows::WindowsRenderService::getInstance().getWindows();
	windows.emplace_back(
		std::make_unique<Windows::MainMenu>());
	windows.emplace_back(
		std::make_unique<Windows::ProjectInfoWindow>());
	windows.emplace_back(
		std::make_unique<Windows::ObjectivesWindow>());
	windows.emplace_back(
		std::make_unique<Windows::EntitiesWindow>());
	auto settingsWindow = std::make_unique<Windows::SettingsWindow>();
	settingsWindow->Init();
	windows.emplace_back(std::move(settingsWindow));
	windows.emplace_back(
		std::make_unique<Windows::ActorsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::VehiclesWindow>());
	windows.emplace_back(
		std::make_unique<Windows::ObjectsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::TrainsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::ParticlesWindow>());
	windows.emplace_back(
		std::make_unique<Windows::FastObjectSelector>());
	windows.emplace_back(
		std::make_unique<Windows::PickupsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::PyrotechnicsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::AudioWindow>());
	windows.emplace_back(
		std::make_unique<Windows::VisualEffectsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::CheckpointsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::ConsoleWindow>());
	windows.emplace_back(
		std::make_unique<Windows::SaveConfirmPopup>());
	windows.emplace_back(
		std::make_unique<Windows::FAQWindow>());
	windows.emplace_back(
		std::make_unique<Windows::InfoWindow>());
	windows.emplace_back(
		std::make_unique<Windows::ToolsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::GlobalVariablesWindow>());
	windows.emplace_back(
		std::make_unique<Windows::DeveloperWindow>());
	windows.emplace_back(
		std::make_unique<Windows::LuaWrapperWindow>());
	windows.emplace_back(
		std::make_unique<Windows::CarrecPathsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::CameraPathsWindow>());
	Windows::WindowsRenderService::getInstance().addRender("showEntitiesName", [] {
		if (!ProjectPlayerService::getInstance().isProjectRunning()) {
			if (Settings::getInstance().get<bool>("main.showEntitiesName").value_or(false))
				NameEntitiesRender::draw();
		}
	});
	windows.emplace_back(
		std::make_unique<Windows::QuickCommandsWindow>());
	windows.emplace_back(
		std::make_unique<Windows::ScriptsWindow>());

	Windows::WindowsRenderService::getInstance().addRender("renderNotifications", [] {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg,
		                      ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		// Background color
		ImGui::RenderNotifications(); // <-- Here we render all notifications
		ImGui::PopStyleVar(1); // Don't forget to Pop()
		ImGui::PopStyleColor(1);
	});
}

void Windows::WindowsRenderService::Init() {
	ImGuiHook::windowCallback = [this] {
		reloadTheme = true;
		this->render();
	};
	ImGuiHook::preRenderCallback = [this] {
		ImGuiHook::m_bShowMouse = this->mouseShown_ && renderWindows_ && !FrontEndMenuManager.m_bMenuActive;
	};

	addWindows();

	defaultWindow = getWindow<MainMenu>();

	LuaEngine::getInstance().getOnReset().connect([this] {
		this->Reset();
	});
}

void Windows::WindowsRenderService::render() {
	if (reloadTheme) {
		loadTheme();
		reloadTheme = false;
	}

	if (!FrontEndMenuManager.m_bMenuActive) {
		auto currRenderList = renderList;
		for (const auto &function : currRenderList | std::views::values)
			function();

		if (renderWindows_) {
			for (const auto &window : this->windows) {
				if (window->isShow()) {
					window->draw();
				}
			}
		}
	}
}

void Windows::WindowsRenderService::style() {
	reloadTheme = true;
}

bool& Windows::WindowsRenderService::isRenderWindows() {
	return renderWindows_;
}

void Windows::WindowsRenderService::setRenderWindows(const bool renderWindows) {
	renderWindows_ = renderWindows;
}

void Windows::WindowsRenderService::Reset() {
	this->closeAllWindows();
	this->renderList.clear();
	this->renderWindows_ = true;
	this->windows.clear();
	this->commands = std::stack<std::unique_ptr<WindowsRenderCommand>>();
	setMouseShown(false);

	addWindows();

	defaultWindow = getWindow<MainMenu>();
}

std::list<std::unique_ptr<Windows::AbstractWindow>>& Windows::WindowsRenderService::getWindows() {
	return windows;
}

bool& Windows::WindowsRenderService::isMouseShown() {
	return mouseShown_;
}

void Windows::WindowsRenderService::setMouseShown(bool mouseShown) {
	mouseShown_ = mouseShown;
}

void Windows::WindowsRenderService::addCommand(std::unique_ptr<WindowsRenderCommand> command) {
	commands.push(std::move(command));
	commands.top()->execute();
}

void Windows::WindowsRenderService::undoLastCommand() {
	if (!commands.empty()) {
		commands.top()->undo();
		commands.pop();
	}
}

void Windows::WindowsRenderService::addRender(std::string name, std::function<void()> renderFunc) {
	this->renderList.emplace(name, renderFunc);
}

void Windows::WindowsRenderService::removeRender(const std::string name) {
	this->renderList.erase(name);
}

void Windows::WindowsRenderService::closeAllWindows() {
	for (const auto &window : windows) {
		if (window->isShow())
			window->close();
	}
	this->commands = std::stack<std::unique_ptr<WindowsRenderCommand>>();
}
