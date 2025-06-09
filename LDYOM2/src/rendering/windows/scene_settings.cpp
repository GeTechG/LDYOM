#include "scene_settings.h"
#include <CClock.h>
#include <CWeather.h>
#include <fa_icons.h>
#include <functional>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <localization.h>
#include <scenes_manager.h>
#include <utils/string_utils.h>

void SceneSettingsPopup::renderContent(Window* window) {
	auto& scene = ScenesManager::instance().getUnsafeCurrentScene();
	const float contentWidth = ImGui::GetContentRegionAvail().x;
	const float labelWidth = 120.0f; // Standard label width for consistency

	// Scene Name Section - Two column layout
	renderTwoColumnRow(
		_("name").c_str(),
		[&]() {
			if (ImGui::InputText("##sceneName", &scene.info.name)) {
				const auto& sceneInfo =
					std::ranges::find_if(ScenesManager::instance().getScenesInfo(), [&window](const SceneInfo& info) {
						return info.id == ScenesManager::instance().getCurrentScene().info.id;
					});
				sceneInfo->name = ScenesManager::instance().getUnsafeCurrentScene().info.name;
			}
		},
		labelWidth);

	renderTwoColumnRow(
		_("scene_settings.print_scene_name").c_str(),
		[&]() { ImGui::Checkbox("##printSceneName", &scene.settings.isPrintSceneName); }, labelWidth);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Main Settings Toggle - Two column layout
	renderTwoColumnRow(
		_("scene_settings.is_scene_settings_enabled").c_str(),
		[&]() { ImGui::Checkbox("##sceneSettingsEnabled", &scene.settings.isSceneSettingsEnabled); }, labelWidth);

	if (!scene.settings.isSceneSettingsEnabled) {
		ImGui::Spacing();
		ImGui::TextWrapped(
			_("scene_settings.disabled_hint", "Enable scene settings to configure advanced options").c_str());
		return;
	}

	ImGui::Spacing();
	ImGui::Separator();

	// Scrollable content area
	ImGui::BeginChild("SettingsContent", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	// Render each section using helper functions
	renderGroupRelationsSection(scene, contentWidth);
	ImGui::Spacing();

	renderTimeSection(scene, contentWidth);
	ImGui::Spacing();

	renderTrafficSection(scene, contentWidth);
	ImGui::Spacing();

	renderWantedLevelSection(scene, contentWidth);
	ImGui::Spacing();

	renderEnvironmentSection(scene, contentWidth);
	ImGui::EndChild();
}

void SceneSettingsPopup::renderTwoColumnRow(const char* label, const std::function<void()>& renderControl,
                                            float labelWidth) {
	// Left column - Label
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label);

	// Calculate actual text width and use the larger value between labelWidth and actual text width
	const float actualTextWidth = ImGui::CalcTextSize(label).x;
	const float dynamicLabelWidth = std::max(labelWidth, actualTextWidth + ImGui::GetStyle().ItemSpacing.x * 2);

	// Right column - Control
	ImGui::SameLine(dynamicLabelWidth);
	const float controlWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SetNextItemWidth(controlWidth);
	renderControl();
}

void SceneSettingsPopup::renderGroupRelationsSection(Scene& scene, float contentWidth) {
	if (ImGui::CollapsingHeader(fmt::format("{} {}", ICON_FA_USERS, _("scene_settings.group_relations")).c_str(),
	                            ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent(10.0f);
		const float childHeight = std::min((ImGui::GetFontSize() + 6.f) * 8.f, ImGui::GetContentRegionAvail().y * 0.4f);
		ImGui::BeginChild("##relations", ImVec2(0.f, childHeight), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		auto& relations = scene.settings.groupRelations;

		// Add new relationship button
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_PLUS, _("scene_settings.add_relation")).c_str())) {
			relations.push_back({PED_TYPE_MISSION1, PED_TYPE_PLAYER1,
			                     eRelationshipType::TYPE_IGNORE}); // Default: Mission1 to Player1, Neutral
		}

		ImGui::Spacing();
		// Render existing relationships
		for (size_t i = 0; i < relations.size(); i++) {
			ImGui::PushID(static_cast<int>(i));
			auto& relation = relations[i];

			const float availableWidth = ImGui::GetContentRegionAvail().x;
			const float spacing = ImGui::GetStyle().ItemSpacing.x;
			const float deleteButtonWidth = ImGui::CalcTextSize(ICON_FA_TRASH).x + ImGui::GetStyle().FramePadding.x * 2;
			const float totalSpacing = spacing * 3; // 3 spaces between 4 elements
			const float remainingWidth = availableWidth - deleteButtonWidth - totalSpacing;

			// Distribute remaining width: 30% for source, 30% for target, 40% for relationship type
			const float sourceWidth = remainingWidth * 0.3f;
			const float targetWidth = remainingWidth * 0.3f;
			const float relationshipWidth = remainingWidth * 0.4f;

			// Source group (ofPedType)
			ImGui::SetNextItemWidth(sourceWidth);
			if (ImGui::BeginCombo("##fromGroup", to_string(relation.ofPedType))) {
				for (int pedType = PED_TYPE_PLAYER1; pedType <= PED_TYPE_MISSION8; pedType++) {
					bool isSelected = (relation.ofPedType == static_cast<ePedType>(pedType));
					if (ImGui::Selectable(to_string(static_cast<ePedType>(pedType)), isSelected)) {
						relation.ofPedType = static_cast<ePedType>(pedType);
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();

			// Target group (toPedType)
			ImGui::SetNextItemWidth(targetWidth);
			if (ImGui::BeginCombo("##toGroup", to_string(relation.toPedType))) {
				for (int pedType = PED_TYPE_PLAYER1; pedType <= PED_TYPE_MISSION8; pedType++) {
					bool isSelected = (relation.toPedType == static_cast<ePedType>(pedType));
					if (ImGui::Selectable(to_string(static_cast<ePedType>(pedType)), isSelected)) {
						relation.toPedType = static_cast<ePedType>(pedType);
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();

			// Relationship type slider
			const auto currentRelation =
				_(fmt::format("scene_settings.relationship_types.{}", (int)relation.relationType));
			ImGui::SetNextItemWidth(relationshipWidth);
			char sliderId[32];
			std::snprintf(sliderId, sizeof(sliderId), "##relation_%zu", i);
			int relationTypeInt = static_cast<int>(relation.relationType);
			if (ImGui::SliderInt(sliderId, &relationTypeInt, 0, 4, currentRelation.c_str())) {
				relation.relationType = static_cast<eRelationshipType>(relationTypeInt);
			}

			// Delete button
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_TRASH, ImVec2(deleteButtonWidth, 0))) {
				relations.erase(relations.begin() + static_cast<long>(i));
				ImGui::PopID();
				break;
			}

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::EndChild();
		ImGui::Unindent(10.0f);
	}
}

void SceneSettingsPopup::renderTimeSection(Scene& scene, float contentWidth) {
	if (ImGui::CollapsingHeader(fmt::format("{} {}", ICON_FA_CLOCK, _("game_time")).c_str(),
	                            ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent(10.0f);
		bool timeChanged = false;
		const float labelWidth = 100.0f;

		// Hours
		renderTwoColumnRow(
			_("hours").c_str(),
			[&]() {
				ImGui::SetNextItemWidth(-1);
				timeChanged |= ImGui::SliderInt("##hours", &scene.settings.time[0], 0, 23, "%02d");
			},
			labelWidth);

		// Minutes
		renderTwoColumnRow(
			_("minutes").c_str(),
			[&]() {
				ImGui::SetNextItemWidth(-1);
				timeChanged |= ImGui::SliderInt("##minutes", &scene.settings.time[1], 0, 59, "%02d");
			},
			labelWidth);

		if (timeChanged) {
			CClock::SetGameClock(scene.settings.time[0], scene.settings.time[1], 0);
		}

		ImGui::Unindent(10.0f);
	}
}

void SceneSettingsPopup::renderTrafficSection(Scene& scene, float contentWidth) {
	if (ImGui::CollapsingHeader(fmt::format("{} {}", ICON_FA_TRAFFIC_CONE, _("traffic_settings")).c_str(),
	                            ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent(10.0f);
		const float labelWidth = 140.0f;

		// Ped Traffic
		renderTwoColumnRow(
			fmt::format("{} {}", ICON_FA_PERSON, _("ped_traffic")).c_str(),
			[&]() { ImGui::DragFloat("##pedTraffic", &scene.settings.trafficPed, 0.01f, 0.f, 2.f, "%.2f"); },
			labelWidth);

		// Car Traffic
		renderTwoColumnRow(
			fmt::format("{} {}", ICON_FA_CAR, _("car_traffic")).c_str(),
			[&]() { ImGui::DragFloat("##carTraffic", &scene.settings.trafficCar, 0.01f, 0.f, 2.f, "%.2f"); },
			labelWidth);

		ImGui::Unindent(10.0f);
	}
}

void SceneSettingsPopup::renderWantedLevelSection(Scene& scene, float contentWidth) {
	if (ImGui::CollapsingHeader(fmt::format("{} {}", ICON_FA_STAR, _("wanted_level")).c_str(),
	                            ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent(10.0f);
		const float labelWidth = 140.0f;

		// Min Wanted Level
		renderTwoColumnRow(
			fmt::format("{} {}", ICON_FA_STAR, _("min_wanted")).c_str(),
			[&]() { ImGui::SliderInt("##minWanted", &scene.settings.wantedMin, 0, 6); }, labelWidth);

		// Max Wanted Level
		renderTwoColumnRow(
			fmt::format("{} {}", ICON_FA_STAR, _("max_wanted")).c_str(),
			[&]() { ImGui::SliderInt("##maxWanted", &scene.settings.wantedMax, 0, 6); }, labelWidth);

		// Ensure min <= max with visual feedback
		if (scene.settings.wantedMin > scene.settings.wantedMax) {
			scene.settings.wantedMax = scene.settings.wantedMin;
			ImGui::TextWrapped(_("validation.min_max_corrected",
			                     "Minimum value cannot be higher than maximum. Adjusted automatically.")
			                       .c_str());
		}

		ImGui::Unindent(10.0f);
	}
}

void SceneSettingsPopup::renderEnvironmentSection(Scene& scene, float contentWidth) {
	if (ImGui::CollapsingHeader(fmt::format("{} {}", ICON_FA_SUN_CLOUD, _("environment")).c_str(),
	                            ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent(10.0f);
		const float labelWidth = 140.0f;

		// Weather
		renderTwoColumnRow(
			fmt::format("{} {}", ICON_FA_CLOUD_SUN, _("weather")).c_str(),
			[&]() {
				if (ImGui::DragInt("##weather", &scene.settings.weather, 0.1f, 0, 255, "Weather ID: %d")) {
					CWeather::ForceWeatherNow(static_cast<short>(scene.settings.weather));
				}
			},
			labelWidth);

		ImGui::Spacing();

		// Riot Mode
		renderTwoColumnRow(
			fmt::format("{} {}", ICON_FA_SKULL_CROSSBONES, _("riot_mode")).c_str(),
			[&]() {
				ImGui::Checkbox("##riotMode", &scene.settings.riot);
				if (scene.settings.riot) {
					ImGui::SameLine();
					ImGui::Text("(!)");
					if (ImGui::IsItemHovered()) {
						ImGui::SetTooltip(
							_("riot_mode_warning", "Warning: This will make the game world chaotic!").c_str());
					}
				}
			},
			labelWidth);

		ImGui::Unindent(10.0f);
	}
}

SceneSettingsPopup::SceneSettingsPopup()
	: ModalPopupWindow(_("scene_settings.title", ICON_FA_SLIDERS)) {
	setSize(400, 550);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setRenderCallback(SceneSettingsPopup::renderContent);
}