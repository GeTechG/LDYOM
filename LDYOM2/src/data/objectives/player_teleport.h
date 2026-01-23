#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CPlayerPed.h>
#include <CPools.h>
#include <CStreaming.h>
#include <CWeaponInfo.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <models_manager.h>
#include <objectives_manager.h>
#include <popups/skin_selector.h>
#include <popups/weapon_selector.h>
#include <project_player.h>
#include <random>
#include <scenes_manager.h>
#include <textures_manager.h>
#include <utils/imgui_configurate.h>

namespace objectives::player_teleport {
constexpr const char* TYPE = "core.player_teleport";

struct Data {
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	float angle = 0.0f;
	int interior = 0; // Hidden, auto-captured with GET_AREA_VISIBLE (0x077E)

	// Player model settings
	bool changeModel = false;
	bool isSpecialModel = false;
	int modelId = 0;               // Regular model ID
	std::string specialModel = ""; // Special model name (e.g., "CJ")

	// Weapon settings
	struct Weapon {
		int weapon;
		int ammo;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Weapon, weapon, ammo)
	};

	bool giveWeapon = false;
	bool isRandomWeapon = false;
	std::vector<Weapon> weapons;
	int defaultWeapon = 0;

	// Health
	int health = 100;

	// Editor visualization
	std::shared_ptr<CPed> editorPed;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerTeleportData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, position, angle, interior, changeModel, isSpecialModel,
		                 modelId, specialModel, giveWeapon, isRandomWeapon, weapons, defaultWeapon, health);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, position, angle, interior, changeModel, isSpecialModel, modelId, specialModel,
	                               giveWeapon, isRandomWeapon, weapons, defaultWeapon, health);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void recreateEditorPed(Data& data) {
	// First despawn the existing ped before creating a new one
	if (data.editorPed) {
		data.editorPed = nullptr;
	}

	// Only create in editor mode
	if (IS_PLAYING) {
		return;
	}

	int modelToLoad = 0; // Default CJ model

	if (data.changeModel) {
		if (data.isSpecialModel) {
			// Special model (e.g., "CJ", "TRUTH", etc.) - special models use ID 290
			if (ModelsManager::validatePedSpecialModel(data.specialModel)) {
				modelToLoad = 290;
				CStreaming::RequestSpecialChar(0, data.specialModel.c_str(), MISSION_REQUIRED);
			} else {
				return; // Invalid special model
			}
		} else {
			// Regular model ID
			if (ModelsManager::validatePedModel(data.modelId)) {
				modelToLoad = data.modelId;
				CStreaming::RequestModel(modelToLoad, MISSION_REQUIRED);
			} else {
				return; // Invalid model
			}
		}
	} else {
		// Default player model (0 = CJ)
		modelToLoad = 0;
		CStreaming::RequestModel(modelToLoad, MISSION_REQUIRED);
	}

	CStreaming::LoadAllRequestedModels(false);

	// Create ped at teleport position
	int newPed;
	plugin::Command<plugin::Commands::CREATE_CHAR>(PED_TYPE_CIVMALE, modelToLoad, data.position[0], data.position[1],
	                                               data.position[2], &newPed);
	CStreaming::SetMissionDoesntRequireModel(modelToLoad);

	CPed* ped = CPools::GetPed(newPed);
	if (!ped) {
		return; // Failed to create ped
	}

	// Use the same deleter pattern as Actor component - check for restart game
	data.editorPed = std::shared_ptr<CPed>(ped, [](CPed* ped) {
		if (!ScenesManager::instance().isRestartGame()) {
			auto ref = CPools::GetPedRef(ped);
			plugin::Command<plugin::Commands::DELETE_CHAR>(ref);
		}
	});

	// Set heading
	plugin::Command<plugin::Commands::SET_CHAR_HEADING>(newPed, data.angle);

	auto adjustedZ = data.position[2] + 1.f;
	// Set position again to be sure
	plugin::Command<plugin::Commands::SET_CHAR_COORDINATES_DONT_WARP_GANG_NO_OFFSET>(newPed, data.position[0],
	                                                                                 data.position[1], adjustedZ);

	// Set area/interior
	plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(newPed, data.interior);

	// Make the ped invulnerable and frozen in the editor
	ped->m_bUsesCollision = 0;
	ped->m_nPhysicalFlags.bBulletProof = 1;
	ped->m_nPhysicalFlags.bCollisionProof = 1;
	ped->m_nPhysicalFlags.bExplosionProof = 1;
	ped->m_nPhysicalFlags.bFireProof = 1;
	ped->m_nPhysicalFlags.bMeleeProof = 1;
	plugin::Command<plugin::Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(newPed, 1);

	// Set health
	ped->m_fMaxHealth = std::max(ped->m_fMaxHealth, static_cast<float>(data.health));
	ped->m_fHealth = static_cast<float>(data.health);

	// Give weapons to editor ped for visualization
	if (data.giveWeapon && !data.weapons.empty()) {
		if (data.isRandomWeapon) {
			// Give a random weapon for preview
			const auto weaponIds = ModelsManager::getWeaponIds();
			if (!weaponIds.empty()) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<size_t> dist(0, weaponIds.size() - 1);
				int randomWeaponId = weaponIds[dist(gen)];

				if (ModelsManager::validateWeaponId(randomWeaponId)) {
					const int weaponModel =
						CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(randomWeaponId), 1)->m_nModelId;
					CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
					CStreaming::LoadAllRequestedModels(false);
					plugin::Command<plugin::Commands::GIVE_WEAPON_TO_CHAR>(newPed, randomWeaponId,
					                                                       data.weapons[0].ammo);
					CStreaming::SetMissionDoesntRequireModel(weaponModel);
				}
			}
		} else {
			// Give all weapons from the list
			for (const auto& [weapon, ammo] : data.weapons) {
				if (!ModelsManager::validateWeaponId(weapon)) {
					continue;
				}
				const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId;

				CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
				CStreaming::LoadAllRequestedModels(false);
				plugin::Command<plugin::Commands::GIVE_WEAPON_TO_CHAR>(newPed, weapon, ammo);
				CStreaming::SetMissionDoesntRequireModel(weaponModel);
			}

			// Set default weapon as current
			if (data.defaultWeapon >= 0 && data.defaultWeapon < static_cast<int>(data.weapons.size())) {
				plugin::Command<plugin::Commands::SET_CURRENT_CHAR_WEAPON>(
					newPed, static_cast<eWeaponType>(data.weapons[data.defaultWeapon].weapon));
			}
		}
	}
}

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Position
	ImGui::Text(tr("position").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragFloat3("##position", data.position.data(), 0.1f, -10000.0f, 10000.0f)) {
		// Update editor ped position
		if (!IS_PLAYING && data.editorPed) {
			data.editorPed->SetPosn(data.position[0], data.position[1], data.position[2]);
		}
	}

	ImGui::SetCursorPosX(availableWidth * 0.45f);
	if (ImGui::Button(fmt::format("{} {}", ICON_FA_LOCATION_CROSSHAIRS, tr("set_current_position")).c_str())) {
		auto player = FindPlayerPed();
		if (player) {
			CVector pos = player->GetPosition();
			data.position[0] = pos.x;
			data.position[1] = pos.y;
			data.position[2] = pos.z;
			// Get current interior (opcode 077E: GET_AREA_VISIBLE)
			plugin::Command<plugin::Commands::GET_AREA_VISIBLE>(&data.interior);
			// Update editor ped position
			if (!IS_PLAYING && data.editorPed) {
				data.editorPed->SetPosn(data.position[0], data.position[1], data.position[2]);
				plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(CPools::GetPedRef(data.editorPed.get()),
				                                                         data.interior);
			}
		}
	}

	// Angle
	ImGui::Text(tr("angle").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragFloat("##angle", &data.angle, 0.1f, -180.0f, 180.0f, "%.1f")) {
		// Update editor ped angle
		if (!IS_PLAYING && data.editorPed) {
			plugin::Command<plugin::Commands::SET_CHAR_HEADING>(CPools::GetPedRef(data.editorPed.get()), data.angle);
		}
	}

	ImGui::SetCursorPosX(availableWidth * 0.45f);
	if (ImGui::Button(fmt::format("{} {}", ICON_FA_COMPASS, tr("set_current_angle")).c_str())) {
		auto player = FindPlayerPed();
		if (player) {
			data.angle = player->m_fCurrentRotation * (180.0f / 3.14159265f);
			// Update editor ped angle
			if (!IS_PLAYING && data.editorPed) {
				plugin::Command<plugin::Commands::SET_CHAR_HEADING>(CPools::GetPedRef(data.editorPed.get()),
				                                                    data.angle);
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Player Model Section
	ImGui::Text(tr("change_model").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##change_model", &data.changeModel)) {
		// Recreate editor ped when changing model setting
		if (!IS_PLAYING) {
			recreateEditorPed(data);
		}
	}

	if (data.changeModel) {
		ImGui::Text(tr("is_special").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		if (ImGui::Checkbox("##is_special", &data.isSpecialModel)) {
			// Recreate editor ped when changing special/regular model
			if (!IS_PLAYING) {
				recreateEditorPed(data);
			}
		}

		ImGui::Text(tr("model").c_str());
		ImGui::SameLine(availableWidth * 0.45f);

		ImGui::BeginGroup();
		const float currentAvailableWidthInGroup = ImGui::GetContentRegionAvail().x;
		const float buttonWidth = ImGui::GetFrameHeight();
		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		float comboWidth = std::max((SCL_PX).x * 100.0f, currentAvailableWidthInGroup - buttonWidth - spacing);

		ImGui::SetNextItemWidth(comboWidth);

		if (data.isSpecialModel) {
			auto& models = ModelsManager::getPedSpecialModels();
			if (ImGui::BeginCombo("##specialModelCombo", data.specialModel.c_str())) {
				for (auto& item : models) {
					if (ImGui::Selectable(item.c_str(), item == data.specialModel)) {
						data.specialModel = item;
						// Recreate editor ped with new model
						if (!IS_PLAYING) {
							recreateEditorPed(data);
						}
					}
				}
				ImGui::EndCombo();
			}
		} else {
			auto& pedModels = ModelsManager::getPedModels();
			if (ImGui::BeginCombo("##regularModelCombo", std::to_string(data.modelId).c_str())) {
				for (auto& item : pedModels) {
					if (ImGui::Selectable(std::to_string(item).c_str(), item == data.modelId)) {
						data.modelId = item;
						// Recreate editor ped with new model
						if (!IS_PLAYING) {
							recreateEditorPed(data);
						}
					}
				}
				ImGui::EndCombo();
			}
		}

		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button(ICON_FA_SHIRT, ImVec2(buttonWidth, 0))) {
			PopupSkinSelector::showPopup();
		}

		static std::function<void(Skin)> skinSelectorCallback = [&data](Skin selected) {
			if (selected.model != -1) {
				data.modelId = selected.model;
				data.isSpecialModel = false;
			} else {
				data.specialModel = selected.special;
				data.isSpecialModel = true;
			}
			// Recreate editor ped with new model
			if (!IS_PLAYING) {
				recreateEditorPed(data);
			}
		};
		PopupSkinSelector::renderPopup(skinSelectorCallback, data.isSpecialModel);

		ImGui::EndGroup();
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Weapon Section
	ImGui::Text(tr("give_weapon").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##give_weapon", &data.giveWeapon)) {
		if (data.giveWeapon && data.weapons.empty()) {
			// Initialize with one weapon
			data.weapons.push_back({static_cast<int>(WEAPON_UNARMED), 30});
		}
		// Recreate editor ped to show/hide weapons
		if (!IS_PLAYING) {
			recreateEditorPed(data);
		}
	}

	if (data.giveWeapon) {
		ImGui::Text(tr("is_random").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		if (ImGui::Checkbox("##is_random", &data.isRandomWeapon)) {
			data.weapons.clear();
			if (data.isRandomWeapon) {
				data.defaultWeapon = 0;
				data.weapons.push_back({static_cast<int>(WEAPON_UNARMED), 30});
			}
			// Recreate editor ped to update weapon visualization
			if (!IS_PLAYING) {
				recreateEditorPed(data);
			}
		}

		if (data.isRandomWeapon) {
			ImGui::Text(_("ammo").c_str());
			ImGui::SameLine(availableWidth * 0.45f);
			ImGui::SetNextItemWidth(-1.f);
			ImGui::InputInt("##ammo", &data.weapons[0].ammo, 1, 10);
		} else {
			ImGui::Spacing();
			// Weapons list with tree nodes
			std::string weaponsListText = std::string(ICON_FA_GUN) + " " + tr("weapons_list");
			if (ImGui::TreeNodeEx(weaponsListText.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

				// Add weapon button
				static bool addWeaponPopup = false;
				std::string addWeaponText = std::string(ICON_FA_PLUS) + " " + tr("add_weapon");
				if (ImGui::Button(addWeaponText.c_str(), ImVec2(-1, 0))) {
					PopupWeaponSelector::showPopup("add_weapon");
					addWeaponPopup = true;
				}

				// Weapon selector popup
				PopupWeaponSelector::renderPopup("add_weapon", &addWeaponPopup, [&data](int weaponId) {
					// Check if weapon already exists
					bool weaponExists = false;
					for (const auto& weapon : data.weapons) {
						if (weapon.weapon == weaponId) {
							weaponExists = true;
							break;
						}
					}

					if (!weaponExists) {
						data.weapons.push_back({weaponId, 30}); // Default 30 ammo
						if (data.weapons.size() - 1 == data.defaultWeapon) {
							if (!IS_PLAYING) {
								recreateEditorPed(data);
							}
						}
					}
				});

				ImGui::Spacing();

				static int weaponEditIndex = -1;
				bool openEditPopup = false;
				int deleteWeaponIndex = -1;

				// Display weapons list
				for (size_t i = 0; i < data.weapons.size(); ++i) {
					auto& weapon = data.weapons[i];

					ImGui::PushID(static_cast<int>(i));

					// Create tree node for each weapon
					auto weaponTitle = fmt::format("{} {} ({})", ICON_FA_CROSSHAIRS,
					                               CWeaponInfo::ms_aWeaponNames[weapon.weapon], weapon.ammo);
					bool nodeOpen = ImGui::TreeNodeEx(weaponTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

					// Add default marker and delete button on the same line
					ImGui::SameLine();
					if (data.defaultWeapon == static_cast<int>(i)) {
						ImGui::Text(ICON_FA_STAR);
						ImGui::SameLine();
					}

					// Position delete button to the right
					float buttonWidth = ImGui::CalcTextSize(ICON_FA_TRASH).x + ImGui::GetStyle().FramePadding.x * 2;
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().ScrollbarSize -
					                     ImGui::GetStyle().WindowPadding.x);

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive));
					if (ImGui::SmallButton(ICON_FA_TRASH)) {
						deleteWeaponIndex = static_cast<int>(i);
					}
					ImGui::PopStyleColor(3);

					if (nodeOpen) {
						// Radio button for default weapon selection
						bool isDefault = (data.defaultWeapon == static_cast<int>(i));
						if (ImGui::RadioButton(tr("is_default").c_str(), isDefault)) {
							data.defaultWeapon = static_cast<int>(i);
						}

						ImGui::Separator();

						ImGui::Text(_("weapon").c_str());
						ImGui::SameLine(availableWidth * 0.35f);
						{
							auto icon = ModelsManager::validateWeaponId(weapon.weapon)
							                ? PopupWeaponSelector::getWeaponIcon(weapon.weapon)
							                : TexturesManager::instance().getTexture("50px-Weapon-unk-hd").value();
							if (icon->getTexture() == nullptr) {
								icon = TexturesManager::instance().getTexture("50px-Weapon-unk-hd").value();
							}
							auto size =
								ImVec2(static_cast<float>(icon->getWidth()), static_cast<float>(icon->getHeight())) *
								SCL_PX;
							if (ImGui::ImageButton(std::to_string(i).c_str(), (ImTextureID)icon->getTexture(), size,
							                       ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0))) {
								openEditPopup = true;
								weaponEditIndex = static_cast<int>(i);
							}
						}

						ImGui::Text(_("ammo").c_str());
						ImGui::SameLine(availableWidth * 0.35f);
						ImGui::SetNextItemWidth(-1.f);
						ImGui::InputInt("##ammo", &weapon.ammo, 1, 10);

						// Ensure ammo is not negative
						if (weapon.ammo < 0)
							weapon.ammo = 0;

						ImGui::TreePop();
					}

					ImGui::PopID();
				}

				static bool selectWeaponPopup = false;
				if (openEditPopup) {
					PopupWeaponSelector::showPopup("select_weapon");
					selectWeaponPopup = true;
				}

				PopupWeaponSelector::renderPopup("select_weapon", &selectWeaponPopup, [&data](int weaponId) {
					// Check if weapon already exists
					bool weaponExists = false;
					for (const auto& weapon : data.weapons) {
						if (weapon.weapon == weaponId) {
							weaponExists = true;
							break;
						}
					}

					if (!weaponExists && weaponEditIndex >= 0 &&
					    weaponEditIndex < static_cast<int>(data.weapons.size())) {
						data.weapons[weaponEditIndex].weapon = weaponId;
						if (weaponEditIndex == data.defaultWeapon) {
							if (!IS_PLAYING) {
								recreateEditorPed(data);
							}
						}
					}
					weaponEditIndex = -1;
				});

				// Handle weapon deletion
				if (deleteWeaponIndex != -1) {
					data.weapons.erase(data.weapons.begin() + deleteWeaponIndex);
					if (data.defaultWeapon >= deleteWeaponIndex) {
						data.defaultWeapon = std::max(0, data.defaultWeapon - 1);
					}
					deleteWeaponIndex = -1;
					// Recreate editor ped to update weapon visualization
					if (!IS_PLAYING) {
						recreateEditorPed(data);
					}
				}

				ImGui::TreePop();
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Health
	ImGui::Text(tr("health").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragInt("##health", &data.health, 1.0f, 0, 500)) {
		// Update editor ped health
		if (!IS_PLAYING && data.editorPed) {
			data.editorPed->m_fMaxHealth = std::max(data.editorPed->m_fMaxHealth, static_cast<float>(data.health));
			data.editorPed->m_fHealth = static_cast<float>(data.health);
		}
	}
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);

	if (!ProjectPlayer::instance().isFaded()) {
		// First: Fade OUT to black
		plugin::Command<plugin::Commands::DO_FADE>(500, 0);
		co_await 600; // Wait 600ms for fade to complete
		ProjectPlayer::instance().setFaded(true);
	}

	// Based on DYOM teleport objective logic (lines 21532-21597)

	// Add 1.0 to Z coordinate (DYOM does this to avoid falling through floor)
	float adjustedZ = data.position[2] + 1.0f;

	// If player is in car, remove from car and place at position (opcode 0362: WARP_CHAR_FROM_CAR_TO_COORD)
	if (plugin::Command<plugin::Commands::IS_CHAR_IN_ANY_CAR>(player)) {
		plugin::Command<plugin::Commands::WARP_CHAR_FROM_CAR_TO_COORD>(player, data.position[0], data.position[1],
		                                                               adjustedZ);
	} else {
		// Otherwise just set coordinates (opcode 09BC: SET_CHAR_COORDINATES_DONT_WARP_GANG_NO_OFFSET)
		plugin::Command<plugin::Commands::SET_CHAR_COORDINATES_DONT_WARP_GANG_NO_OFFSET>(player, data.position[0],
		                                                                                 data.position[1], adjustedZ);
	}

	// Set player angle (opcode 0173: SET_CHAR_HEADING)
	plugin::Command<plugin::Commands::SET_CHAR_HEADING>(player, data.angle);

	// Set interior (opcode 04BB: SET_AREA_VISIBLE and 0860: SET_CHAR_AREA_VISIBLE)
	plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(data.interior);
	plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(player, data.interior);

	// Reset camera (opcodes 0373: SET_CAMERA_BEHIND_PLAYER and 02EB: RESTORE_CAMERA_JUMPCUT)
	plugin::Command<plugin::Commands::SET_CAMERA_BEHIND_PLAYER>();
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();

	// Refresh renderer at position (opcode 04E4: REQUEST_COLLISION and 03CB: LOAD_SCENE)
	plugin::Command<plugin::Commands::REQUEST_COLLISION>(data.position[0], data.position[1]);
	plugin::Command<plugin::Commands::LOAD_SCENE>(data.position[0], data.position[1], adjustedZ);

	// Change player model if specified (opcode 09C7: SET_PLAYER_MODEL)
	if (data.changeModel) {
		int modelToLoad = -1;

		if (data.isSpecialModel) {
			// Special model (e.g., "CJ", "TRUTH", etc.) - special models use ID 290
			if (ModelsManager::validatePedSpecialModel(data.specialModel)) {
				modelToLoad = 290;
				CStreaming::RequestSpecialChar(0, data.specialModel.c_str(), MISSION_REQUIRED);
			}
		} else {
			// Regular model ID
			if (ModelsManager::validatePedModel(data.modelId)) {
				modelToLoad = data.modelId;
				CStreaming::RequestModel(modelToLoad, MISSION_REQUIRED);
			}
		}

		if (modelToLoad != -1) {
			CStreaming::LoadAllRequestedModels(false);
			while (!plugin::Command<plugin::Commands::HAS_MODEL_LOADED>(modelToLoad)) {
				co_await 1;
			}
			plugin::Command<plugin::Commands::SET_PLAYER_MODEL>(0, modelToLoad);
		}
	}

	// Give weapons with ammo (opcode 01B2: GIVE_WEAPON_TO_CHAR)
	if (data.giveWeapon && !data.weapons.empty()) {
		// Clear all weapons first
		plugin::Command<plugin::Commands::REMOVE_ALL_CHAR_WEAPONS>(player);

		if (data.isRandomWeapon) {
			// Random weapon
			const auto weaponIds = ModelsManager::getWeaponIds();
			if (!weaponIds.empty()) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<size_t> dist(0, weaponIds.size() - 1);
				int randomWeaponId = weaponIds[dist(gen)];

				if (ModelsManager::validateWeaponId(randomWeaponId)) {
					const int weaponModel =
						CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(randomWeaponId), 1)->m_nModelId;
					CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
					CStreaming::LoadAllRequestedModels(false);
					while (!plugin::Command<plugin::Commands::HAS_MODEL_LOADED>(weaponModel)) {
						co_await 1;
					}
					plugin::Command<plugin::Commands::GIVE_WEAPON_TO_CHAR>(player, randomWeaponId,
					                                                       data.weapons[0].ammo);
					CStreaming::SetMissionDoesntRequireModel(weaponModel);
				}
			}
		} else {
			// Give all weapons in the list
			for (const auto& [weapon, ammo] : data.weapons) {
				if (!ModelsManager::validateWeaponId(weapon)) {
					continue;
				}
				const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId;

				CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
				CStreaming::LoadAllRequestedModels(false);
				while (!plugin::Command<plugin::Commands::HAS_MODEL_LOADED>(weaponModel)) {
					co_await 1;
				}
				plugin::Command<plugin::Commands::GIVE_WEAPON_TO_CHAR>(player, weapon, ammo);
				CStreaming::SetMissionDoesntRequireModel(weaponModel);
			}

			// Set default weapon as current
			if (data.defaultWeapon >= 0 && data.defaultWeapon < static_cast<int>(data.weapons.size())) {
				plugin::Command<plugin::Commands::SET_CURRENT_CHAR_WEAPON>(
					player, static_cast<eWeaponType>(data.weapons[data.defaultWeapon].weapon));
			}
		}
	}

	// Set player health (opcode 0223 and stat 24 for max health)
	if (data.health > 100) {
		// Set max health stat to 1138.0 for health > 100
		plugin::Command<plugin::Commands::SET_FLOAT_STAT>(24, 1138.0f);
	} else {
		// Set max health stat to 569.0 for health <= 100
		plugin::Command<plugin::Commands::SET_FLOAT_STAT>(24, 569.0f);
	}
	plugin::Command<plugin::Commands::SET_CHAR_HEALTH>(player, data.health);

	// Enable player control (opcode player.CanMove)
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);

	// Then immediately: Fade IN from black (so cutscene is visible!)
	plugin::Command<plugin::Commands::DO_FADE>(500, 1);
	ProjectPlayer::instance().setFaded(false);

	co_return;
}

void onStart(Data& data) {
	// Create visual representation for the editor
	recreateEditorPed(data);
}

void onReset(Data& data) {
	// Remove visual representation
	if (data.editorPed) {
		data.editorPed = nullptr;
	}
}

Objective make() {
	Data initialData{};
	plugin::Command<plugin::Commands::GET_AREA_VISIBLE>(&initialData.interior);
	auto playerCoord = FindPlayerPed()->GetPosition();
	plugin::Command<plugin::Commands::GET_CHAR_COORDINATES>(FindPlayerPed(), &initialData.position[0],
	                                                        &initialData.position[1], &initialData.position[2]);
	initialData.position[2] -= 1.f;
	initialData.angle = FindPlayerPed()->m_fCurrentRotation * (180.0f / 3.14159265f);

	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), initialData,
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute),
	                 std::function<void(Data&)>(onStart), std::function<void(Data&)>(onReset));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_teleport
