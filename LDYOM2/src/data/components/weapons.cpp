#define IMGUI_DEFINE_MATH_OPERATORS
#include "weapons.h"
#include "actor.h"

#include <CStreaming.h>
#include <CWeaponInfo.h>
#include <entity.h>
#include <fmt/format.h>
#include <models_manager.h>
#include <rendering/popups/weapon_selector.h>
#include <textures_manager.h>
#include <utils/imgui_configurate.h>

void components::Weapons::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Weapons>("WeaponsComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Weapons, cast, dirty, weapons, defaultWeapon, accuracy,
	                 isRandom, giveWeapons, updateDefaultWeapon);
}

components::Weapons::Weapons()
	: Component(TYPE) {}

void components::Weapons::giveWeapons() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		actor->ped->ClearWeapons();
		if (this->isRandom) {
			const auto weaponIds = ModelsManager::getWeaponIds();
			const int i = (std::rand() % (weaponIds.size() + 1)) - 1;
			if (i >= 0) {
				const auto weaponId = static_cast<eWeaponType>(weaponIds[i]);
				const int weaponModel = CWeaponInfo::GetWeaponInfo(weaponId, 1)->m_nModelId;
				CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
				CStreaming::LoadAllRequestedModels(false);
				actor->ped->GiveWeapon(weaponId, this->weapons[0].ammo, false);
				CStreaming::SetMissionDoesntRequireModel(weaponModel);
				actor->ped->SetCurrentWeapon(weaponId);
			}
		} else {
			for (const auto& [weapon, ammo] : weapons) {
				if (!ModelsManager::validateWeaponId(weapon)) {
					continue;
				}
				const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId;

				CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
				CStreaming::LoadAllRequestedModels(false);
				actor->ped->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);
				CStreaming::SetMissionDoesntRequireModel(weaponModel);
			}
			if (!weapons.empty()) {
				actor->ped->SetCurrentWeapon(static_cast<eWeaponType>(weapons[defaultWeapon].weapon));
				actor->ped->m_nWeaponAccuracy = static_cast<unsigned char>(accuracy);
			}
		}
	}
}

void components::Weapons::updateDefaultWeapon() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		actor->ped->SetCurrentWeapon(static_cast<eWeaponType>(weapons[defaultWeapon].weapon));
	}
}

nlohmann::json components::Weapons::to_json() const {
	auto j = this->Component::to_json();
	j["weapons"] = weapons;
	j["defaultWeapon"] = defaultWeapon;
	j["accuracy"] = accuracy;
	j["isRandomWeapons"] = isRandom;
	return j;
}

void components::Weapons::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("weapons").get_to(weapons);
	j.at("defaultWeapon").get_to(defaultWeapon);
	j.at("accuracy").get_to(accuracy);
	j.at("isRandomWeapons").get_to(isRandom);
}

void components::Weapons::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("accuracy").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##accuracy", &accuracy, 0, 100, "%d%%");

	ImGui::Text(tr("is_random").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##is_random", &isRandom)) {
		weapons.clear();
		if (isRandom) {
			defaultWeapon = 0;
			weapons.push_back({static_cast<int>(WEAPON_UNARMED), 30});
		}
		dirty |= GiveWeapons;
	}

	if (this->isRandom) {
		ImGui::Text(_("ammo").c_str());
		ImGui::SameLine(availableWidth * 0.35f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputInt("##ammo", &weapons[0].ammo, 1, 10);
	} else {
		ImGui::Spacing();
		ImGui::Separator();
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
			PopupWeaponSelector::renderPopup("add_weapon", &addWeaponPopup, [this](int weaponId) {
				// Check if weapon already exists
				bool weaponExists = false;
				for (const auto& weapon : weapons) {
					if (weapon.weapon == weaponId) {
						weaponExists = true;
						break;
					}
				}

				if (!weaponExists) {
					weapons.push_back({weaponId, 30}); // Default 30 ammo
					dirty |= GiveWeapons;
				}
			});

			ImGui::Spacing();

			static int weaponEditIndex = -1;
			bool openEditPopup = false;
			int deleteWeaponIndex = -1;

			// Display weapons list
			for (size_t i = 0; i < weapons.size(); ++i) {
				auto& weapon = weapons[i];

				ImGui::PushID(static_cast<int>(i));

				// Create tree node for each weapon
				auto weaponTitle = fmt::format("{} {} ({})", ICON_FA_CROSSHAIRS,
				                               CWeaponInfo::ms_aWeaponNames[weapon.weapon], weapon.ammo);
				bool nodeOpen = ImGui::TreeNodeEx(weaponTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

				// Add delete button on the same line (right side)
				ImGui::SameLine();
				if (defaultWeapon == static_cast<int>(i)) {
					ImGui::Text(ICON_FA_STAR);
					ImGui::SameLine();
				}

				// Position delete button to the right
				float buttonWidth = ImGui::CalcTextSize(ICON_FA_TRASH).x + ImGui::GetStyle().FramePadding.x * 2;
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().ScrollbarSize -
				                     ImGui::GetStyle().WindowPadding.x);

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent background
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive));
				if (ImGui::SmallButton(ICON_FA_TRASH)) {
					deleteWeaponIndex = static_cast<int>(i);
				}
				ImGui::PopStyleColor(3);

				if (nodeOpen) {
					// Radio button for default weapon selection
					bool isDefault = (defaultWeapon == static_cast<int>(i));
					if (ImGui::RadioButton(tr("is_default").c_str(), isDefault)) {
						defaultWeapon = static_cast<int>(i);
						dirty |= UpdateDefaultWeapon;
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

			PopupWeaponSelector::renderPopup("select_weapon", &selectWeaponPopup, [this](int weaponId) {
				// Check if weapon already exists
				bool weaponExists = false;
				for (const auto& weapon : weapons) {
					if (weapon.weapon == weaponId) {
						weaponExists = true;
						break;
					}
				}

				if (!weaponExists) {
					weapons[weaponEditIndex].weapon = weaponId;
					dirty |= GiveWeapons;
				}
				weaponEditIndex = -1;
			});

			// Handle weapon deletion
			if (deleteWeaponIndex != -1) {
				weapons.erase(weapons.begin() + deleteWeaponIndex);
				if (defaultWeapon >= deleteWeaponIndex) {
					defaultWeapon = std::max(0, defaultWeapon - 1);
				}
				deleteWeaponIndex = -1;
				dirty |= GiveWeapons;
			}

			ImGui::TreePop();
		}
	}
}

void components::Weapons::onStart() {
	Component::onStart();
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor) {
		if (actor->ped) {
			giveWeapons();
		}
		this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { giveWeapons(); }));
	}
}

void components::Weapons::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & GiveWeapons) {
		giveWeapons();
	}
	if (this->dirty & UpdateDefaultWeapon) {
		updateDefaultWeapon();
	}
	this->dirty = None;
}

void components::Weapons::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::Weapons::getDependencies() { return {{Actor::TYPE}, true}; }