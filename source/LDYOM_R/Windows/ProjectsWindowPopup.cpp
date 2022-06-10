// ReSharper disable CppMemberFunctionMayBeStatic
#include "ProjectsWindowPopup.h"
#include "imgui.h"
#include "fmt/core.h"
#include "fa.h"
#include "PopupWarning.h"
#include "ProjectsService.h"
#include "utils.h"
#include "Localization/Localization.h"

void newProjectInfoPopup() {
	auto &local = Localization::getInstance();
	if (ImGui::BeginPopupModal(local.get("project_info.title").c_str(), nullptr)) {
		const auto &projectInfo = ProjectsService::getInstance().getCurrentProject().getProjectInfo();

		ImGui::InputText(local.get("project_info.name").c_str(), projectInfo->name, sizeof projectInfo->name);
		ImGui::InputText(local.get("project_info.author").c_str(), projectInfo->authorName, sizeof projectInfo->authorName);

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !(ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered())) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

static int selected_project = -1;

Windows::ProjectsWindowPopup::ProjectsWindowPopup(): popupWarningNewProject_("projects.new_project_warning"),
													 popupWarningLoadProject_("projects.load_project_warning"),
													 popupWarningDeleteProject_("projects.delete_project_warning") {
	this->missionIcon_ = utils::LoadTextureRequiredFromFile(L"LDYOM\\Resources\\mission_icon.png").value();
	ProjectsService::getInstance().onUpdate().connect([] {
		selected_project = -1;
	});
}

void projectsList(const ImVec2 window_size) {
	ImGui::BeginChild("##projectsList", ImVec2(window_size.x / 3.0f, window_size.y / 3.0f), true);
	for (int i = 0; i < static_cast<int>(ProjectsService::getInstance().getProjectsInfos().size()); ++i) {
		const auto projectInfo = ProjectsService::getInstance().getProjectsInfos().at(i).get();
		if (ImGui::Selectable(projectInfo->name, selected_project == i)) {
			selected_project = i;
		}
	}

	ImGui::EndChild();
}

void Windows::ProjectsWindowPopup::projectInfo(Localization& local) const {
	static const float BASE_WIDTH = ImGui::GetFontSize() * 18.75f;

	ImGui::BeginChild("##projectInfo", ImVec2(BASE_WIDTH + 40.0f, ImGui::GetItemRectSize().y), true);

	if (selected_project != -1) {
		const auto projectInfo = ProjectsService::getInstance().getProjectsInfos().at(selected_project).get();
		Texture* icon;
		if (projectInfo->icon.has_value())
			icon = projectInfo->icon.value().get();
		else
			icon = this->missionIcon_.get();

		const float scaleX = BASE_WIDTH / static_cast<float>(icon->getWidth());
		const float scaleY = (BASE_WIDTH - 100.0f) / static_cast<float>(icon->getHeight());

		const float scale = min(scaleX, scaleY);

		const auto size = ImVec2(static_cast<float>(icon->getWidth()) * scale, 
		                         static_cast<float>(icon->getHeight()) * scale);

		ImGui::Dummy(ImVec2(5.0f, 0.0f));
		ImGui::SameLine();
		ImGui::Image(icon->getTexture(), size);
		ImGui::PushTextWrapPos(BASE_WIDTH);
		ImGui::Text(fmt::format("{}: {}", local.get("project_info.name"), projectInfo->name).c_str());
		ImGui::Text(fmt::format("{}: {}", local.get("project_info.author"), projectInfo->authorName).c_str());
		ImGui::PopTextWrapPos();
	}

	ImGui::EndChild();
}

void Windows::ProjectsWindowPopup::draw() {
	static auto& local = Localization::getInstance();

	const ImVec2 window_size = ImGui::GetIO().DisplaySize;

	if (ImGui::BeginPopupModal(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str(), &this->show_, ImGuiWindowFlags_AlwaysAutoResize)) {

		projectsList(window_size);
		
		ImGui::SameLine();
		projectInfo(local);

		// / (1/3 window) / (3 buttons) - 3.0
		const auto widthButton = window_size.x / 3.0f / 3.0f - 3.0f;
		if (ImGui::Button(local.get("projects.new_project").c_str(), ImVec2(widthButton, .0f))) {
			ImGui::OpenPopup(popupWarningNewProject_.getName());
		}
		ImGui::SameLine();
		ImGui::BeginDisabled(selected_project == -1);
		if (ImGui::Button(local.get("projects.load_project").c_str(), ImVec2(widthButton, .0f))) {
			ImGui::OpenPopup(popupWarningLoadProject_.getName());
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("projects.delete_project").c_str(), ImVec2(widthButton, .0f))) {
			ImGui::OpenPopup(popupWarningDeleteProject_.getName());
		}
		ImGui::EndDisabled();

		static bool openNewProjectInfo = false;
		popupWarningNewProject_.draw([&] {
			ProjectsService::getInstance().createNewProject();
			openNewProjectInfo = true;
			});
		popupWarningLoadProject_.draw([&] {
			ProjectsService::getInstance().loadProject(selected_project);
			});
		popupWarningDeleteProject_.draw([&] {
			ProjectsService::getInstance().deleteProject(selected_project);
			});

		if (openNewProjectInfo) {
			ImGui::OpenPopup(local.get("project_info.title").c_str());
			openNewProjectInfo = false;
		}

		newProjectInfoPopup();

		ImGui::EndPopup();
	}
}

void Windows::ProjectsWindowPopup::setShow(bool show) {
	show_ = show;
}
