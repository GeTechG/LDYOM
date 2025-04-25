#define IMGUI_DEFINE_MATH_OPERATORS
#include "create_new_project.h"
#include "projects_manager.h"
#include "string_utils.h"
#include <fmt/format.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <utils/theme_loader.h>
#include <window_manager.h>

void CreateNewProject::open() {
	ModalPopupWindow::open();
	const std::string projectName = _("create_new_project.default_project_name");
	const size_t copiedCount = projectName.copy(projectNameBuffer.data(), projectNameBuffer.size() - 1);
	projectNameBuffer[copiedCount] = '\0';
	authorNameBuffer.fill(0);
}

void CreateNewProject::renderContent(CreateNewProject* window) {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;
	ImGui::Text(_("create_new_project.project_name").c_str());
	ImGui::SetNextItemWidth(availableWidth);
	if (window->m_isAppering)
		ImGui::SetKeyboardFocusHere();
	ImGui::InputText("##ProjectName", window->projectNameBuffer.data(), window->projectNameBuffer.size());
	const auto previewProjectFolderName = to_snake_case(window->projectNameBuffer.data());
	ImGui::TextDisabled(_("create_new_project.project_folder_name_preview", previewProjectFolderName.c_str()).c_str());
	ImGui::Text(_("create_new_project.author_name").c_str());
	ImGui::SetNextItemWidth(availableWidth);
	ImGui::InputText("##AuthorName", window->authorNameBuffer.data(), window->authorNameBuffer.size());

	auto availableHeight = ImGui::GetContentRegionAvail().y;
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + availableHeight - ImGui::GetFrameHeight());

	const auto createText = _("create_new_project.create");
	const auto cancelText = _("create_new_project.cancel");
	const auto center = availableWidth / 2.f;
	const auto buttonsWidth = ImGui::CalcTextSize(createText.c_str()).x + ImGui::CalcTextSize(cancelText.c_str()).x +
	                          ImGui::GetStyle().FramePadding.x * 4;

	ImGui::SetCursorPosX(center - ImGui::CalcTextSize(createText.c_str()).x - ImGui::GetStyle().FramePadding.x * 2 -
	                     (availableWidth - buttonsWidth) * 0.1f);
	if (ImGui::Button(createText.c_str())) {
		ProjectsManager::instance().createNewProject(window->projectNameBuffer.data(), window->authorNameBuffer.data());
		window->close();
		WindowManager::instance().closeWindow("project_manager");
		WindowManager::instance().openWindow("main_menu");
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(center + (availableWidth - buttonsWidth) * 0.1f);
	if (ImGui::Button(cancelText.c_str())) {
		window->close();
	}
}

CreateNewProject::CreateNewProject()
	: ModalPopupWindow("create_new_project.title") {
	authorNameBuffer.fill(0);
	projectNameBuffer.fill(0);
	setSize(450, 250);
	setSizeMin(300, 210);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setRenderCallback<CreateNewProject>(renderContent);
}