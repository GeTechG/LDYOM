#include "project_manager.h"
#include "fa_icons.h"
#include "localization.h"
#include "project_info.h"
#include <fmt/format.h>
#include <format>
#include <imgui_widgets/imgui_widgets.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

void ProjectManager::renderHeaderBar() {
	const ImVec2 availContentSize = ImGui::GetContentRegionAvail();
	const float topBarHeight = ImGui::GetFrameHeight() * 1.5f;
	const float buttonHeight = topBarHeight;

	if (ImGui::BeginChild("HeaderBar", ImVec2(-1.0f, topBarHeight), false)) {
		const auto projectsText = _("project_manager.projects");
		const auto settingsText = _("project_manager.settings");

		ImGui::PushFont(ImGuiConfigurate::getHeaderFont());
		const float projectButtonWidth =
			ImGui::CalcTextSize(projectsText.c_str()).x + ImGui::GetStyle().FramePadding.x * 4;
		ImGui::PopFont();
		const float settingsButtonWidth =
			ImGui::CalcTextSize(settingsText.c_str()).x + ImGui::GetStyle().FramePadding.x * 4;

		const float totalCentralButtonsWidth = projectButtonWidth;
		const float centerPosX = (availContentSize.x - totalCentralButtonsWidth) * 0.5f - settingsButtonWidth;

		ImGui::SetCursorPosX(centerPosX);
		ImGui::PushFont(ImGuiConfigurate::getHeaderFont());
		if (ImGui::TextSelectable(projectsText.c_str(), true, ImVec2(projectButtonWidth, buttonHeight))) {
			// Projects action
		}
		ImGui::PopFont();

		ImGui::SameLine(0, 0);

		const float rightTabPosX = availContentSize.x - settingsButtonWidth;
		ImGui::SetCursorPosX(rightTabPosX);
		if (ImGui::TextButton(settingsText.c_str(), ImVec2(settingsButtonWidth, buttonHeight))) {
			WindowManager::instance().openWindow("quick_settings");
		}
	}
	ImGui::EndChild();
}

void ProjectManager::renderTopButtons(float spacing) {
	const float childHeight = ImGui::GetFrameHeight();
	if (ImGui::BeginChild("TopButtons", ImVec2(-1.0f, childHeight), false)) {
		if (ImGui::Button(_("project_manager.create", ICON_FA_PLUS_LARGE).c_str())) {
			WindowManager::instance().openWindow("create_new_project");
		}

		ImGui::SameLine();

		// Import button
		if (ImGui::Button(_("project_manager.import", ICON_FA_FOLDER).c_str())) {
			// Import action
		}

		ImGui::SameLine();

		// Scan button
		if (ImGui::Button(_("project_manager.scan", ICON_FA_MAGNIFYING_GLASS).c_str())) {
			// Scan action
		}

		ImGui::SameLine();

		const float remainingWidth = ImGui::GetContentRegionAvail().x;
		// const float sortWidth = remainingWidth * 0.25f;
		const float filterWidth = remainingWidth - spacing * 2;

		static char filterBuffer[256] = "";
		ImGui::PushItemWidth(filterWidth);
		ImGui::InputTextWithHint("##filter", _("project_manager.filter_projects").c_str(), filterBuffer,
		                         IM_ARRAYSIZE(filterBuffer));
		ImGui::SameLine();
		const float inputX = ImGui::GetCursorPosX();
		const auto iconSearchSize = ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS);
		ImGui::SetCursorPosX(inputX - iconSearchSize.x - spacing * 2);
		ImGui::TextDisabled(ICON_FA_MAGNIFYING_GLASS);
		// ImGui::SameLine(0, 1);
		// ImGui::SetCursorPosX(inputX);
		// ImGui::AlignTextToFramePadding();
		// ImGui::Text("Sort:");
		// ImGui::SameLine();
		// ImGui::PushItemWidth(sortWidth);
		// if (ImGui::BeginCombo("##sort", sortOptions[currentSortOption])) {
		// 	for (int i = 0; i < IM_ARRAYSIZE(sortOptions); i++) {
		// 		const bool isSelected = (currentSortOption == i);
		// 		if (ImGui::Selectable(sortOptions[i], isSelected)) {
		// 			currentSortOption = i;
		// 		}
		// 		if (isSelected) {
		// 			ImGui::SetItemDefaultFocus();
		// 		}
		// 	}
		// 	ImGui::EndCombo();
		// }
	}
	ImGui::EndChild();
}

void ProjectManager::renderProjectList(ProjectManager* window, float spacing, float width) {
	if (ImGui::BeginChild("ProjectList", ImVec2(width, -1.f), ImGuiChildFlags_FrameStyle)) {
		const float iconSize = ImGui::GetTextLineHeight() * 2.5f;
		const float starSize = ImGui::CalcTextSize(ICON_FA_STAR).x;
		const float itemHeight = iconSize * 1.5f;

		const float availableWidth = ImGui::GetContentRegionAvail().x;

		const auto& projects = ProjectsManager::instance().getProjects();
		for (int i = 0; i < static_cast<int>(projects.size()); i++) {
			const auto& project = projects[i];

			ImGui::PushID(i);

			const bool isSelected = (i == window->m_selectedProjectIndex);
			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			}

			if (i == 0 && window->m_isAppering) {
				ImGui::SetKeyboardFocusHere(0);
			}
			ImGui::BeginChild(std::format("##project_{}", i).c_str(), ImVec2(-1.0f, itemHeight),
			                  ImGuiChildFlags_NavFlattened);

			ImGui::SetCursorPosX(spacing);
			// ImGui::SetCursorPos(ImVec2(spacing, (itemHeight - starSize) * 0.5f));
			// ImVec2 starPos = ImGui::GetCursorScreenPos();
			// ImDrawList* draw_list = ImGui::GetWindowDrawList();
			// ImU32 starColor =
			// 	projects[i].favorite ? ImGui::GetColorU32(ImGuiCol_Text) : ImGui::GetColorU32(ImGuiCol_TextDisabled);
			// draw_list->AddText(starPos, starColor, ICON_FA_STAR);
			// if (ImGui::InvisibleButton("##star", ImVec2(starSize, starSize))) {
			// 	projects[i].favorite = !projects[i].favorite;
			// }

			// ImGui::SameLine();

			const float fullIconSize = itemHeight - spacing * 2;
			const float iconVerticalPadding = (itemHeight - fullIconSize) * 0.5f;

			ImGui::SetCursorPosY(iconVerticalPadding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, fullIconSize / 2);
			if (ImGui::Button("P", ImVec2(fullIconSize, fullIconSize))) {
				window->m_selectedProjectIndex = i;
			}
			ImGui::PopStyleVar();

			ImGui::SameLine();

			const auto timestampText =
				Localization::instance().getI18N().formatDate("short_date_time", std::localtime(&project.timestamp));
			const float timestampWidth =
				ImGui::CalcTextSize(timestampText.c_str()).x + ImGui::GetStyle().FramePadding.x * 4;
			const float versionWidth = ImGui::CalcTextSize("0.0").x;

			float labelAreaWidth =
				availableWidth - fullIconSize - starSize - timestampWidth - versionWidth - spacing * 5;
			if (labelAreaWidth < 100)
				labelAreaWidth = 100;

			// Project details - центрируем вертикально
			const float textHeight = ImGui::GetTextLineHeightWithSpacing() * 2;
			const float textVerticalCenterPos = (itemHeight - textHeight) * 0.5f;
			ImGui::SetCursorPosY(textVerticalCenterPos);

			ImGui::BeginGroup();
			ImGui::Text("%s", projects[i].name.c_str());
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
			ImGui::TextDisabled(ICON_FA_FOLDER " %s", projects[i].path.c_str());
			ImGui::EndGroup();

			// Выравнивание версии и метки времени по правому краю
			const float rightAlignPos = ImGui::GetWindowContentRegionMax().x - timestampWidth;
			ImGui::SameLine(rightAlignPos);
			ImGui::SetCursorPosY(textVerticalCenterPos);

			ImGui::BeginGroup();
			ImGui::Text(projects[i].author.c_str());
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
			ImGui::TextDisabled(timestampText.c_str());
			ImGui::EndGroup();

			// Делаем весь элемент проекта кликабельным
			ImGui::SetCursorPos(ImVec2(0, 0));
			if (ImGui::InvisibleButton("##wholeline", ImVec2(-1.0f, itemHeight), ImGuiButtonFlags_EnableNav)) {
				window->m_selectedProjectIndex = i;
			}

			ImGui::EndChild();

			if (isSelected) {
				ImGui::PopStyleColor(2);
			}

			if (i < static_cast<int>(projects.size()) - 1) {
				ImGui::Separator();
			}

			ImGui::PopID();
		}
	}
	ImGui::EndChild();
}

void ProjectManager::renderSidebar(ProjectManager* window, float sidebarWidth, float* maxButtonWidth) {
	if (ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, -1.0f), ImGuiChildFlags_None)) {
		ImGui::Separator();

		const float buttonHeight = ImGui::GetFrameHeight() * 1.5f;

		const float framePaddingX = ImGui::GetStyle().FramePadding.x;

		const auto editText = _("project_manager.edit", ICON_FA_PEN);
		const auto runText = _("project_manager.run", ICON_FA_PLAY);
		const auto renameText = _("project_manager.rename", ICON_FA_I_CURSOR);
		const auto removeText = _("project_manager.remove", ICON_FA_TRASH);

		// Calculate each text width plus padding
		const float editWidth = ImGui::CalcTextSize(editText.c_str()).x + framePaddingX * 2;
		const float runWidth = ImGui::CalcTextSize(runText.c_str()).x + framePaddingX * 2;
		const float renameWidth = ImGui::CalcTextSize(renameText.c_str()).x + framePaddingX * 2;
		const float removeWidth = ImGui::CalcTextSize(removeText.c_str()).x + framePaddingX * 2;

		const float currentMaxWidth = std::max(std::max(editWidth, runWidth), std::max(renameWidth, removeWidth));

		if (maxButtonWidth != nullptr) {
			*maxButtonWidth = currentMaxWidth;
		}

		if (ImGui::Button(editText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			ProjectsManager::instance().openProject(window->m_selectedProjectIndex);
			ScenesManager::instance().loadScenesInfo();
			if (!ScenesManager::instance().getScenesInfo().empty()) {
				ScenesManager::instance().loadScene(ScenesManager::instance().getScenesInfo()[0].id);
			}
			window->close();
			WindowManager::instance().closeWindow("project_manager");
			WindowManager::instance().openWindow("main_menu");
		}

		if (ImGui::Button(runText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			// Run action
		}

		if (ImGui::Button(renameText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			// Rename action
		}

		if (ImGui::Button(removeText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			// Remove action
		}
	}
	ImGui::EndChild();
}

void ProjectManager::renderContent(ProjectManager* window) {
	const float spacing = ImGui::GetStyle().ItemSpacing.x;

	renderHeaderBar();
	renderTopButtons(spacing);

	static float maxButtonWidth = 0.0f;

	const ImVec2 availContentSize = ImGui::GetContentRegionAvail();
	const float sidebarWidth =
		std::max(availContentSize.x * 0.15f, maxButtonWidth + ImGui::GetStyle().WindowPadding.x * 2);
	const float projectListWidth = availContentSize.x - sidebarWidth - spacing;

	renderProjectList(window, spacing, projectListWidth);
	ImGui::SameLine();
	ImGui::BeginDisabled(window->m_selectedProjectIndex < 0);
	renderSidebar(window, sidebarWidth, &maxButtonWidth);
	ImGui::EndDisabled();
}

ProjectManager::ProjectManager()
	: Window("project_manager.title") {
	setSize(900, 520);
	setSizeMin(540, 320);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_NoCollapse);
	setRenderCallback<ProjectManager>(renderContent);
}

void ProjectManager::open() {
	Window::open();
	m_selectedProjectIndex = -1;
}
