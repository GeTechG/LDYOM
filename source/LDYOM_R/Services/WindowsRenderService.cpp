#include "WindowsRenderService.h"

#include <CMenuManager.h>
#include <fmt/core.h>

#include "AbstractWindow.h"
#include "ActorsWindow.h"
#include "AudioWindow.h"
#include "CheckpointsWindow.h"
#include "ConsoleWindow.h"
#include "EntitiesWindow.h"
#include "FAQWindow.h"
#include "FastObjectSelector.h"
#include "ImGuiHook.h"
#include "imguistyleloader.h"
#include "imgui_notify.h"
#include "InfoWindow.h"
#include "Logger.h"
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
#include "Settings.h"
#include "SettingsWindow.h"
#include "TrainsWindow.h"
#include "VehiclesWindow.h"
#include "VisualEffectsWindow.h"

bool reloadTheme = false;

void loadTheme() {
	if (!ImGui::StyleLoader::LoadFile(fmt::format("LDYOM/Themes/{}.toml",
	                                              Settings::getInstance().get<std::string>("main.theme").value()))) {
		Logger::getInstance().log("invalid load theme!");
	}
}

void addWindows() {
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::MainMenu>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::ProjectInfoWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::ObjectivesWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::EntitiesWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::SettingsWindow>());
	Windows::WindowsRenderService::getInstance().getWindow<Windows::SettingsWindow>()->Init();
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::ActorsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::VehiclesWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::ObjectsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::TrainsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::ParticlesWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::FastObjectSelector>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::PickupsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::PyrotechnicsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::AudioWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::VisualEffectsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::CheckpointsWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::ConsoleWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::SaveConfirmPopup>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::FAQWindow>());
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::InfoWindow>());
	Windows::WindowsRenderService::getInstance().addRender("showEntitiesName", [] {
		if (!ProjectPlayerService::getInstance().isProjectRunning()) {
			if (Settings::getInstance().get<bool>("main.showEntitiesName").value_or(false))
				NameEntitiesRender::draw();
		}
	});
	Windows::WindowsRenderService::getInstance().getWindows().emplace_back(
		std::make_unique<Windows::QuickCommandsWindow>());

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

void Windows::WindowsRenderService::Init() const {
	ImGuiHook::windowCallback = [this] {
		reloadTheme = true;
		this->render();
	};
	ImGuiHook::preRenderCallback = [this] {
		ImGuiHook::m_bShowMouse = this->mouseShown_ && renderWindows_ && !FrontEndMenuManager.m_bMenuActive;
	};

	addWindows();
}

void Windows::WindowsRenderService::render() const {
	if (reloadTheme) {
		loadTheme();
		reloadTheme = false;
	}

	if (!FrontEndMenuManager.m_bMenuActive) {
		auto currRenderList = renderList_;
		for (const auto &function : currRenderList | std::views::values)
			function();

		if (renderWindows_) {
			for (const auto &window : this->windows_) {
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
	this->renderList_.clear();
	this->renderWindows_ = true;
	this->windows_.clear();
	setMouseShown(false);

	addWindows();
}

std::list<std::unique_ptr<Windows::AbstractWindow>>& Windows::WindowsRenderService::getWindows() {
	return windows_;
}

bool& Windows::WindowsRenderService::isMouseShown() {
	return mouseShown_;
}

void Windows::WindowsRenderService::setMouseShown(bool mouseShown) {
	mouseShown_ = mouseShown;
}

void Windows::WindowsRenderService::addRender(std::string name, std::function<void()> renderFunc) {
	this->renderList_.emplace(name, renderFunc);
}

void Windows::WindowsRenderService::removeRender(const std::string name) {
	this->renderList_.erase(name);
}

void Windows::WindowsRenderService::closeAllWindows() const {
	for (const auto &window : windows_) {
		if (window->isShow())
			window->close();
	}
}
