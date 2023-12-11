#define NOMINMAX
#include "TeleportPlayerObjective.h"

#include <CCamera.h>
#include <CGame.h>
#include <common.h>
#include <CPools.h>
#include <CStreaming.h>
#include <CVector.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>
#include "CClothes.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "fa.h"
#include "imgui.h"
#include "ModelsService.h"
#include "PopupWeaponSelector.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

CPed* TeleportPlayerObjective::spawnPed() {
	using namespace plugin;

	std::array<unsigned, 18> textures{};
	std::array<unsigned, 10> models{};
	float muscular;
	float fats;
	auto playerModel = CWorld::Players[0].m_pPed->m_nModelIndex;
	{
		Command<Commands::SET_PLAYER_MODEL>(0, 0);
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(textures.data(), playerClothes->m_anTextureKeys, sizeof playerClothes->m_anTextureKeys);
		std::memcpy(models.data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
		muscular = playerClothes->m_fFatStat;
		fats = playerClothes->m_fMuscleStat;
	}

	if (this->modelType_ == 0 && this->modelId_ == 0) {
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, this->clotherMAnTextureKeys_.data(),
		            sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, this->clotherMAnModelKeys_.data(),
		            sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = this->fatStat;
		playerClothes->m_fMuscleStat = this->musculeStat;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	int model;
	if (this->modelType_ == 0) {
		model = ModelsService::validPedModel(this->modelId_);
		CStreaming::RequestModel(model, GAME_REQUIRED);
	} else {
		model = 290 + this->slot_;
		const auto &specialsPed = ModelsService::getInstance().getSpecialsPed().at(this->modelId_);
		CStreaming::RequestSpecialChar(this->slot_, specialsPed.c_str(), GAME_REQUIRED);
	}
	CStreaming::LoadAllRequestedModels(false);

	int newPed;
	Command<Commands::CREATE_CHAR>(23, model, this->pos_[0], this->pos_[1], this->pos_[2], &newPed);
	CStreaming::SetMissionDoesntRequireModel(model);

	CPed *ped = CPools::GetPed(newPed);

	{
		Command<Commands::SET_PLAYER_MODEL>(0, playerModel);
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, textures.data(), sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, models.data(), sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = fats;
		playerClothes->m_fMuscleStat = muscular;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	for (const auto &[weaponRaw, ammo] : this->weapons) {
		int weapon = ModelsService::validWeaponModel(weaponRaw);
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

		CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		ped->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);

		CStreaming::SetMissionDoesntRequireModel(weaponModel);
	}
	if (!this->weapons.empty()) {
		int currentWeapon = this->weapons.at(this->defaultWeapon).weapon;
		ped->SetCurrentWeapon(static_cast<eWeaponType>(currentWeapon));
	}

	ped->DisablePedSpeech(1);

	return ped;
}

void TeleportPlayerObjective::characteristicsSection(Localization &local) {
	if (ImGui::TreeNode(local.get("general.characteristics").c_str())) {
		ImGui::SetNextItemWidth(150.0f);
		ImGui::InputInt(local.get("general.health").c_str(), &this->health_, 0, 0);

		ImGui::TreePop();
	}
}

TeleportPlayerObjective::TeleportPlayerObjective(CVector &pos, const float heading): BaseObjective(nullptr),
	pos_{pos.x, pos.y, pos.z},
	headingAngle_(heading),
	interiorID_(CGame::currArea) {
	const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
	std::memcpy(this->clotherMAnTextureKeys_.data(), playerClothes->m_anTextureKeys,
	            sizeof playerClothes->m_anTextureKeys);
	std::memcpy(this->clotherMAnModelKeys_.data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
	this->fatStat = playerClothes->m_fFatStat;
	this->musculeStat = playerClothes->m_fMuscleStat;

	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.teleport_player"));
	this->name += suffix;
	this->spawnEditorPed();
}

TeleportPlayerObjective::~TeleportPlayerObjective() {
	this->deleteEditorPed();
}

std::array<float, 3>& TeleportPlayerObjective::getPos() { return pos_; }
float& TeleportPlayerObjective::getHeadingAngle() { return headingAngle_; }
unsigned char& TeleportPlayerObjective::getModelType() { return modelType_; }
int& TeleportPlayerObjective::getSlot() { return slot_; }
int& TeleportPlayerObjective::getModelId() { return modelId_; }
int& TeleportPlayerObjective::getHealth() { return health_; }
int& TeleportPlayerObjective::getInteriorId() { return interiorID_; }
bool& TeleportPlayerObjective::isDressUp() { return dressUp_; }
std::array<unsigned, 10>& TeleportPlayerObjective::getClotherMAnModelKeys() { return clotherMAnModelKeys_; }
std::array<unsigned, 18>& TeleportPlayerObjective::getClotherMAnTextureKeys() { return clotherMAnTextureKeys_; }
float& TeleportPlayerObjective::getFatStat() { return fatStat; }
float& TeleportPlayerObjective::getMusculeStat() { return musculeStat; }
std::vector<Weapon>& TeleportPlayerObjective::getWeapons() { return weapons; }
int& TeleportPlayerObjective::getDefaultWeapon() { return defaultWeapon; }
std::optional<CPed*>& TeleportPlayerObjective::getEditorPed() { return editorPed_; }


void weaponsSection(TeleportPlayerObjective *objective, Localization &local) {
	if (ImGui::TreeNode(local.get("general.weapons").c_str())) {
		const auto fontSize = ImGui::GetFontSize();
		if (ImGui::BeginTable("##weaponsTable", 4, ImGuiTableFlags_ScrollY,
		                      ImVec2(0, fontSize * 7.5f))) {
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, fontSize * 3.75f);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, fontSize * 8.f);
			ImGui::TableSetupColumn(local.get("general.initial").c_str());
			ImGui::TableSetupColumn("");
			ImGui::TableSetupScrollFreeze(4, 1);
			ImGui::TableHeadersRow();

			for (int i = 0; i < static_cast<int>(objective->getWeapons().size()); ++i) {
				auto &weapon = objective->getWeapons().at(i);

				ImGui::PushID(i);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				PopupWeaponSelector::getInstance().weaponButton(&weapon.weapon);

				PopupWeaponSelector::getInstance().renderPopup([&weapon, &objective](const int weaponId) {
					weapon.weapon = weaponId;
					objective->spawnEditorPed();
				});


				ImGui::TableNextColumn();
				ImGui::BeginGroup();

				ImGui::SetNextItemWidth(fontSize * 3);
				if (ImGui::InputInt(local.get("general.model").c_str(), &weapon.weapon, 0, 0)) {
					objective->spawnEditorPed();
				}
				ImGui::SetNextItemWidth(fontSize * 3);
				ImGui::InputInt(local.get("weapons.ammo").c_str(), &weapon.ammo, 0, 0);
				ImGui::EndGroup();

				ImGui::TableNextColumn();
				if (ImGui::RadioButton(fmt::format("##{}", i).c_str(), &objective->getDefaultWeapon(), i)) {
					if (objective->getEditorPed().has_value())
						objective->getEditorPed().value()->SetCurrentWeapon(static_cast<eWeaponType>(weapon.weapon));
				}

				ImGui::TableNextColumn();
				if (ImGui::Button(ICON_FA_TRASH_ALT, ImVec2(25.0f, 0.0f))) {
					objective->getWeapons().erase(objective->getWeapons().begin() + i);
					i = static_cast<int>(objective->getWeapons().size());
					objective->getDefaultWeapon() = std::max(
						std::min(static_cast<int>(objective->getWeapons().size() - 1), objective->getDefaultWeapon()),
						0);
					objective->spawnEditorPed();
				}

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		if (ImGui::Button(local.get("general.add").c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 1.0f, 0.0f))) {
			objective->getWeapons().emplace_back(1, 0);
			objective->spawnEditorPed();
		}

		ImGui::TreePop();
	}
}

void TeleportPlayerObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	//position
	InputPosition(this->pos_.data(), [this] {
		this->interiorID_ = CGame::currArea;
		this->spawnEditorPed();
	});
	//heading
	DragAngleRotation(&this->headingAngle_, [this] { this->spawnEditorPed(); });

	modelSkinSelection(this->modelType_, this->modelId_, this->slot_, [this] {
		this->spawnEditorPed();
	});

	utils::ToggleButton(local.get("teleport_player_objective.dress_up").c_str(), &this->dressUp_);

	if (this->modelType_ == 0 && this->modelId_ == 0) {
		if (ImGui::Button(local.get("teleport_player_objective.copy_clothes").c_str())) {
			const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
			std::memcpy(this->clotherMAnTextureKeys_.data(), playerClothes->m_anTextureKeys,
			            sizeof playerClothes->m_anTextureKeys);
			std::memcpy(this->clotherMAnModelKeys_.data(), playerClothes->m_anModelKeys,
			            sizeof playerClothes->m_anModelKeys);
			this->fatStat = playerClothes->m_fFatStat;
			this->musculeStat = playerClothes->m_fMuscleStat;
			spawnEditorPed();
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(local.get("teleport_player_objective.help_copy_clothes").c_str());
	}

	characteristicsSection(local);

	weaponsSection(this, local);

	listOverlay.emplace_back(local.get("info_overlay.camera_view"));
	listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

	auto &position = this->pos_;
	utils::controlCamera({position[0], position[1], position[2]});
}

ktwait TeleportPlayerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;
	using namespace std::chrono;

	CPed *playerPed = FindPlayerPed();
	auto playerInVehicle = Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) ||
		Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
		Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) ||
		Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
		Command<Commands::IS_CHAR_IN_ANY_TRAIN>(playerPed);
	if (playerInVehicle) {
		Command<Commands::TASK_LEAVE_ANY_CAR>(playerPed);
		while (playerInVehicle) {
			co_await 1;
			playerInVehicle = Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_TRAIN>(playerPed);
		}
	}

	Command<Commands::SET_CHAR_AREA_VISIBLE>(playerPed, this->interiorID_);
	Command<Commands::SET_AREA_VISIBLE>(this->interiorID_);

	int model;
	if (this->modelType_ == 0) {
		model = ModelsService::validPedModel(this->modelId_);
		CStreaming::RequestModel(model, GAME_REQUIRED);
	} else {
		model = 290 + this->slot_;
		const auto &specialsPed = ModelsService::getInstance().getSpecialsPed().at(this->modelId_);
		CStreaming::RequestSpecialChar(this->slot_, specialsPed.c_str(), GAME_REQUIRED);
	}

	CStreaming::LoadAllRequestedModels(false);

	playerPed->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
	Command<Commands::SET_PLAYER_MODEL>(0, model);
	Command<Commands::SET_CHAR_HEADING>(playerPed, this->headingAngle_);
	Command<Commands::REQUEST_COLLISION>(this->pos_[0], this->pos_[1]);

	if (this->modelType_ == 0 && this->modelId_ == 0 && this->dressUp_) {
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, this->clotherMAnTextureKeys_.data(),
		            sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, this->clotherMAnModelKeys_.data(),
		            sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = this->fatStat;
		playerClothes->m_fMuscleStat = this->musculeStat;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	for (const auto &[weaponRaw, ammo] : this->weapons) {
		int weapon = ModelsService::validWeaponModel(weaponRaw);
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

		CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		playerPed->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);

		CStreaming::SetMissionDoesntRequireModel(weaponModel);
	}
	if (!this->weapons.empty()) {
		int currentWeapon = this->weapons.at(this->defaultWeapon).weapon;
		playerPed->SetCurrentWeapon(static_cast<eWeaponType>(currentWeapon));
	}

	co_return;
}

void TeleportPlayerObjective::spawnEditorPed() {
	using namespace plugin;

	if (editorPed_.has_value())
		this->deleteEditorPed();

	this->editorPed_ = spawnPed();
	this->editorPed_.value()->m_bUsesCollision = 0;
	this->editorPed_.value()->m_nPhysicalFlags.bBulletProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bCollisionProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bExplosionProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bFireProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bMeleeProof = 1;
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetPedRef(this->editorPed_.value()), 1);
	this->editorPed_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
	this->editorPed_.value()->SetHeading(RAD(this->headingAngle_));
}

extern bool restart;

void TeleportPlayerObjective::deleteEditorPed() {
	using namespace plugin;

	if (this->editorPed_.has_value() && !restart) {
		int pedRef = CPools::GetPedRef(this->editorPed_.value());
		Command<Commands::DELETE_CHAR>(pedRef);
		this->editorPed_ = std::nullopt;
	}
}
