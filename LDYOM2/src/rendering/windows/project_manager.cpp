#include "project_manager.h"
#include "fa_icons.h"
#include "localization.h"
#include "project_info.h"
#include <algorithm>
#include <cctype>
#include <fmt/format.h>
#include <format>
#include <imgui_widgets/imgui_widgets.h>
#include <project_player.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

namespace {
bool containsCaseInsensitive(const std::string& text, const std::string& search) {
	if (search.empty()) {
		return true;
	}

	auto it = std::search(text.begin(), text.end(), search.begin(), search.end(),
	                      [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); });

	return it != text.end();
}
} // namespace

void ProjectManager::renderHeaderBar() {
	const ImVec2 availContentSize = ImGui::GetContentRegionAvail();
	const float topBarHeight = ImGui::GetFrameHeight() * 1.5f;
	const float buttonHeight = topBarHeight;

	if (ImGui::BeginChild("HeaderBar", ImVec2(-1.0f, topBarHeight), false)) {
		const auto projectsText = _("project_manager.projects");
		const auto settingsText = ICON_FA_GEAR;
		const auto infoText = ICON_FA_INFO;

		ImGui::PushFont(ImGuiConfigurate::getHeaderFont());
		const float projectButtonWidth =
			ImGui::CalcTextSize(projectsText.c_str()).x + ImGui::GetStyle().FramePadding.x * 4;
		ImGui::PopFont();
		const float settingsButtonWidth = ImGui::CalcTextSize(settingsText).x + ImGui::GetStyle().FramePadding.x * 4;
		const float infoButtonWidth = ImGui::CalcTextSize(infoText).x + ImGui::GetStyle().FramePadding.x * 4;

		const float totalCentralButtonsWidth = projectButtonWidth;
		const float centerPosX = (availContentSize.x - totalCentralButtonsWidth) * 0.5f - settingsButtonWidth;

		ImGui::SetCursorPosX(centerPosX);
		ImGui::PushFont(ImGuiConfigurate::getHeaderFont());
		if (ImGui::TextSelectable(projectsText.c_str(), true, ImVec2(projectButtonWidth, buttonHeight))) {
			// Projects action
		}
		ImGui::PopFont();

		ImGui::SameLine(0, 0);

		const float rightTabPosX = availContentSize.x - settingsButtonWidth - infoButtonWidth;
		ImGui::SetCursorPosX(rightTabPosX);
		if (ImGui::TextButton(infoText, ImVec2(infoButtonWidth, buttonHeight))) {
			WindowManager::instance().openWindow("info_window");
		}

		ImGui::SameLine(0, 0);
		if (ImGui::TextButton(settingsText, ImVec2(settingsButtonWidth, buttonHeight))) {
			WindowManager::instance().openWindow("quick_settings");
		}
	}
	ImGui::EndChild();
}

void ProjectManager::renderTopButtons(ProjectManager* window, float spacing) {
	const float childHeight = ImGui::GetFrameHeight();
	if (ImGui::BeginChild("TopButtons", ImVec2(-1.0f, childHeight), false)) {
		if (ImGui::Button(_("project_manager.create", ICON_FA_PLUS_LARGE).c_str())) {
			WindowManager::instance().openWindow("create_new_project");
		}

		ImGui::SameLine();

		if (ImGui::Button(_("project_manager.refresh", ICON_FA_MAGNIFYING_GLASS_ARROWS_ROTATE).c_str())) {
			ProjectsManager::instance().refreshProjects();
		}

		ImGui::SameLine();

		const float remainingWidth = ImGui::GetContentRegionAvail().x;
		const float filterWidth = remainingWidth - spacing * 2;

		ImGui::PushItemWidth(filterWidth);
		ImGui::InputTextWithHint("##filter", _("project_manager.filter_projects").c_str(), window->m_filterBuffer,
		                         IM_ARRAYSIZE(window->m_filterBuffer));
		ImGui::SameLine();
		const float inputX = ImGui::GetCursorPosX();
		const auto iconSearchSize = ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS);
		ImGui::SetCursorPosX(inputX - iconSearchSize.x - spacing * 2);
		ImGui::TextDisabled(ICON_FA_MAGNIFYING_GLASS);
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
		const std::string filterText(window->m_filterBuffer);

		for (int i = 0; i < static_cast<int>(projects.size()); i++) {
			const auto& project = projects[i];

			// Фильтрация: проверка вхождения в название или автора
			if (!containsCaseInsensitive(project.name, filterText) &&
			    !containsCaseInsensitive(project.author, filterText)) {
				continue;
			}

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
		const auto renameText = _("item_popup.rename", ICON_FA_I_CURSOR);
		const auto removeText = _("item_popup.remove", ICON_FA_TRASH);

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
			if (ProjectsManager::instance().loadProject(window->m_selectedProjectIndex)) {
				window->close();
				WindowManager::instance().closeWindow("project_manager");
				WindowManager::instance().openWindow("main_menu");
			}
		}

		if (ImGui::Button(runText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			if (ProjectsManager::instance().loadProject(window->m_selectedProjectIndex)) {
				window->close();
				WindowManager::instance().closeWindow("project_manager");
				ProjectPlayer::instance().startCurrentProject();
			}
		}

		if (ImGui::Button(renameText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			const auto& projects = ProjectsManager::instance().getProjects();
			if (window->m_selectedProjectIndex >= 0 && window->m_selectedProjectIndex < static_cast<int>(projects.size())) {
				window->m_renameBuffer = projects[window->m_selectedProjectIndex].name;
				window->m_openRenamePopup = true;
			}
		}

		if (ImGui::Button(removeText.c_str(), ImVec2(-1.0f, buttonHeight))) {
			window->m_indexToRemove = window->m_selectedProjectIndex;
		}
	}
	ImGui::EndChild();
}

void ProjectManager::renderContent(ProjectManager* window) {
	const float spacing = ImGui::GetStyle().ItemSpacing.x;
	const float footerHeight = ImGui::GetFrameHeight();

	renderHeaderBar();
	renderTopButtons(window, spacing);

	static float maxButtonWidth = 0.0f;

	const float separatorHeight = ImGui::GetStyle().ItemSpacing.y * 2 + 1.0f;
	const float contentHeight = ImGui::GetContentRegionAvail().y - footerHeight - separatorHeight;

	if (ImGui::BeginChild("ContentArea", ImVec2(-1.0f, contentHeight), false)) {
		const ImVec2 innerAvail = ImGui::GetContentRegionAvail();
		const float innerSidebarWidth =
			std::max(innerAvail.x * 0.15f, maxButtonWidth + ImGui::GetStyle().WindowPadding.x * 2);
		const float innerListWidth = innerAvail.x - innerSidebarWidth - spacing;

		renderProjectList(window, spacing, innerListWidth);
		ImGui::SameLine();
		ImGui::BeginDisabled(window->m_selectedProjectIndex < 0);
		renderSidebar(window, innerSidebarWidth, &maxButtonWidth);
		ImGui::EndDisabled();
	}
	ImGui::EndChild();

	// Footer: version info
	ImGui::Separator();
	ImGui::TextDisabled("LDYOM v%s", LDYOM_VERSION_STRING);

	// Открываем popup на уровне главного окна
	if (window->m_openRenamePopup) {
		ImGui::OpenPopup("rename_project_popup");
		window->m_openRenamePopup = false;
	}

	// Rename popup (обрабатывается вне disabled блока)
	if (ImGui::RenamePopup("rename_project_popup", &window->m_renameBuffer)) {
		if (!window->m_renameBuffer.empty()) {
			if (ProjectsManager::instance().renameProject(window->m_selectedProjectIndex, window->m_renameBuffer)) {
				ProjectsManager::instance().refreshProjects();
			}
		}
	}

	// Remove confirmation dialog
	if (window->m_indexToRemove != -1) {
		auto state = ImGui::ConfirmDialog(_("project_manager.remove_title").c_str(), _("project_manager.remove_message").c_str());
		if (state == 1) {
			if (ProjectsManager::instance().removeProject(window->m_indexToRemove)) {
				ProjectsManager::instance().refreshProjects();
				window->m_selectedProjectIndex = std::min(window->m_selectedProjectIndex, static_cast<int>(ProjectsManager::instance().getProjects().size()) - 1);
			}
			window->m_indexToRemove = -1;
		} else if (state == 0) {
			window->m_indexToRemove = -1;
		}
	}
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
	m_filterBuffer[0] = '\0';
	m_openRenamePopup = false;
	ProjectsManager::instance().refreshProjects();
}