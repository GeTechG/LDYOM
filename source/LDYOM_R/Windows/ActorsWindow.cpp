#include "ActorsWindow.h"

#include "ProjectsService.h"
#include "Localization/Localization.h"
#include "fmt/core.h"
#include "fa.h"
#include "imgui.h"
#include "ModelsService.h"
#include "PopupSkinSelector.h"
#include "PopupWeaponSelector.h"
#include "utilsRender.h"
#include "utils.h"

std::string Windows::ActorsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_RUNNING, Localization::getInstance().get("entities.actors"));
}

std::string Windows::ActorsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_RUNNING, Localization::getInstance().get("entities.actor"));
}

int Windows::ActorsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance().getCurrentProject().getActors().size());
}

void Windows::ActorsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().createNewActor();
}

char* Windows::ActorsWindow::getElementName(const int i) {
	return ProjectsService::getInstance().getCurrentProject().getActors().at(i).getName();
}

void Windows::ActorsWindow::deleteElement(const int i) {
	ProjectsService::getInstance().getCurrentProject().getActors().at(i).deleteEditorPed();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getActors().begin();
	ProjectsService::getInstance().getCurrentProject().getActors().erase(begin + i);
	this->currentElement--;
}

void Windows::ActorsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Actor* actor = &ProjectsService::getInstance().getCurrentProject().getActors().at(this->currentElement);

	//position
	InputPosition(actor->getPosition(), [actor] { actor->updateLocation(); });
	//heading
	DragAngleRotation(&actor->getHeadingAngle(), [actor] { actor->updateLocation(); });

	static char unsigned minActorModelType = 0;
	static char unsigned maxActorModelType = 1;
	if (ImGui::SliderScalar("##actorModelType", ImGuiDataType_U8, &actor->getModelType(), &minActorModelType, &maxActorModelType,
		(actor->getModelType() == 0) ? local.get("actor_model_type.ped").c_str() : local.get("actor_model_type.special").c_str())) {
		actor->getModelId() = 0;
		actor->spawnEditorPed();
	}

	if (actor->getModelType() == 1) {
		ImGui::SliderInt(local.get("general.slot").c_str(), &actor->getSlot(), 0, 8);
	}

	if (ImGui::Button(ICON_FA_TSHIRT, ImVec2(25.0f, 0.0f))) {
		PopupSkinSelector::getInstance().showPopup();
	}
	ImGui::SameLine();
	if (actor->getModelType() == 0) {
		if (ImGui::InputInt("##model", &actor->getModelId(), 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
			actor->spawnEditorPed();
		}
	} else {
		if (utils::Combo("##model", &actor->getModelId(), ModelsService::getInstance().getSpecialsPed())) {
			actor->spawnEditorPed();
		}
	}

	ImGui::Text(local.get("general.weapons").c_str());
	if (ImGui::BeginTable("##weaponsTable", 4, ImGuiTableFlags_ScrollY, ImVec2(0.0f, 200.0f))) {

		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 60.0f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 120.0f);
		ImGui::TableSetupColumn(local.get("general.initial").c_str());
		ImGui::TableSetupColumn("");
		ImGui::TableSetupScrollFreeze(4, 1);
		ImGui::TableHeadersRow();

		for (int i = 0; i < static_cast<int>(actor->getWeapons().size()); ++i) {
			auto& weapon = actor->getWeapons().at(i);

			ImGui::PushID(i);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			PopupWeaponSelector::getInstance().weaponButton(&weapon.weapon);

			PopupWeaponSelector::getInstance().renderPopup([&weapon, &actor](const int weaponId) {
				weapon.weapon = weaponId;
				actor->spawnEditorPed();
				});

			
			ImGui::TableNextColumn();
			ImGui::BeginGroup();

			if (ImGui::InputInt(local.get("general.model").c_str(), &weapon.weapon, 0, 0)) {
				actor->spawnEditorPed();
			}
			ImGui::InputInt(local.get("weapons.ammo").c_str(), &weapon.ammo, 0, 0);
			ImGui::EndGroup();

			ImGui::TableNextColumn();
			if (ImGui::RadioButton(fmt::format("##{}", i).c_str(), &actor->getDefaultWeapon(), i)) {
				if (actor->getEditorPed().has_value())
					actor->getEditorPed().value()->SetCurrentWeapon(static_cast<eWeaponType>(weapon.weapon));
			}

			ImGui::TableNextColumn();
			if (ImGui::Button(ICON_FA_TRASH_ALT, ImVec2(25.0f, 0.0f))) {
				actor->getWeapons().erase(actor->getWeapons().begin() + i);
				i = static_cast<int>(actor->getWeapons().size());
				actor->getDefaultWeapon() = min(actor->getDefaultWeapon(), static_cast<int>(actor->getWeapons().size()) - 1);
				actor->spawnEditorPed();
			}

			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	if (ImGui::Button(local.get("general.add").c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() - 5.0f, 0.0f))) {
		actor->getWeapons().emplace_back(1, 0);
		actor->spawnEditorPed();
	}

	PopupSkinSelector::getInstance().renderPopup([actor](int model) {
			actor->getModelId() = model;
			actor->spawnEditorPed();
		}, actor->getModelType() == 1, actor->getSlot());
}