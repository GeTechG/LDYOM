﻿#include "ActorsWindow.h"

#include <CWorld.h>

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
#include "CCamera.h"
#include <extensions/ScriptCommands.h>

#include "EditByPlayerService.h"

std::string Windows::ActorsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_RUNNING, Localization::getInstance().get("entities.actors"));
}

std::string Windows::ActorsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_RUNNING, Localization::getInstance().get("entities.actor"));
}

int Windows::ActorsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
		.getCurrentProject()
		.getCurrentScene()
		->getActors()
		.size());
}

void Windows::ActorsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewActor();
}

void Windows::ActorsWindow::createNewElementFrom(int i) {
	const auto & actor = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewActorFrom(*actor);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().back()->spawnEditorPed();
}

char* Windows::ActorsWindow::getElementName(const int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().at(i)->getName();
}

void Windows::ActorsWindow::deleteElement(const int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().at(i)->deleteEditorPed();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().erase(begin + i);
	this->currentElement--;
}

void weaponsSection(Actor* actor, Localization& local) {
	if (ImGui::TreeNode(local.get("general.weapons").c_str())) {
		if (ImGui::BeginTable("##weaponsTable", 4, ImGuiTableFlags_ScrollY, ImVec2(300.0f, 200.0f))) {

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
		if (ImGui::Button(local.get("general.add").c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 1.0f, 0.0f))) {
			actor->getWeapons().emplace_back(1, 0);
			actor->spawnEditorPed();
		}

		ImGui::TreePop();
	}
}

void characteristicsSection(Localization& local, Actor* actor) {
	if (ImGui::TreeNode(local.get("general.characteristics").c_str())) {
		ImGui::SetNextItemWidth(150.0f);
		ImGui::InputInt(local.get("general.health").c_str(), &actor->getHealth(), 0, 0);
		ImGui::SetNextItemWidth(150.0f);
		ImGui::DragInt(local.get("actor.accuracy_shooting").c_str(), &actor->getAccuracy(), 0.5f, 0, 100, "%d %%");
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderInt(local.get("actor.group").c_str(), &actor->getGroup(), 0, 8, 
			actor->getGroup() == 0 ? local.get("general.player").c_str() : 
										   fmt::format("{} {}", local.get("actor.group"), actor->getGroup()).c_str());

		utils::ToggleButton(local.get("actor.random_spawn").c_str(), &actor->isRandomSpawn());
		utils::ToggleButton(local.get("general.must_survive").c_str(), &actor->isShouldNotDie());
		utils::ToggleButton(local.get("actor.stay_in_same_place").c_str(), &actor->isStayInSamePlace());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(local.get("actor.stay_in_same_place_description").c_str());
		utils::ToggleButton(local.get("actor.kinda_stay_in_same_place").c_str(), &actor->isKindaStayInSamePlace());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(local.get("actor.kinda_stay_in_same_place_description").c_str());
		utils::ToggleButton(local.get("actor.headshot").c_str(), &actor->isHeadshot());
		utils::ToggleButton(local.get("actor.drop_weapons").c_str(), &actor->isDropWeapons());

		ImGui::TreePop();
	}
}

void Windows::ActorsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Actor* actor = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors().at(this->currentElement).get();

	//position
	InputPosition(actor->getPosition(), [actor] { actor->updateLocation(); });
	//heading
	DragAngleRotation(&actor->getHeadingAngle(), [actor] { actor->updateLocation(); });

	modelSkinSelection(actor->getModelType(), actor->getModelId(), actor->getSlot(), [actor] {
		actor->spawnEditorPed();
	});

	utils::ToggleButton(local.get("teleport_player_objective.dress_up").c_str(), &actor->isDressUp());

	if (actor->getModelType() == 0 && actor->getModelId() == 0) {
		if (ImGui::Button(local.get("teleport_player_objective.copy_clothes").c_str())) {
			const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
			std::memcpy(actor->getClotherMAnTextureKeys().data(), playerClothes->m_anTextureKeys, sizeof playerClothes->m_anTextureKeys);
			std::memcpy(actor->getClotherMAnModelKeys().data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
			actor->getFatStat() = playerClothes->m_fFatStat;
			actor->getMusculeStat() = playerClothes->m_fMuscleStat;
			actor->spawnEditorPed();
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(local.get("teleport_player_objective.help_copy_clothes").c_str());
	}

	characteristicsSection(local, actor);

	weaponsSection(actor, local);

	ObjectiveDependentInput(actor);

	if (ImGui::Button(local.get("actor.edit_by_player").c_str())) {
		EditByPlayerService::getInstance().editByPlayerActor(*actor);
	}

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("info_overlay.camera_view").c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();

	utils::controlCamera(CVector(actor->getPosition()[0], actor->getPosition()[1], actor->getPosition()[2] + 1.0f));
}

void Windows::ActorsWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::ActorsWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}