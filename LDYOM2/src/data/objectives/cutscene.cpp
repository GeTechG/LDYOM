#include "cutscene.h"
#include "rendering/in_game/cutscene_objective_editing.h"

namespace objectives::cutscene {
void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	ImGui::Text(_("text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputText("##text", &data.text);

	ImGui::Text(_("text_time").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##text_time", &data.textTime, 0.1f, 0.f, 10.f, "%.1f");

	ImGui::Text(tr("camera_position").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat3("##camera_position", data.cameraPosition.data(), 0.1f, -10000.0f, 10000.0f);

	ImGui::Text(tr("behaviour").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##behaviour", tr(fmt::format("behaviours.{}", data.behaviour)).c_str())) {
		for (int i = 0; i < 9; ++i) {
			const bool isSelected = (data.behaviour == i);
			if (ImGui::Selectable(tr(fmt::format("behaviours.{}", i)).c_str(), isSelected)) {
				data.behaviour = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (data.behaviour <= 2) {
		ImGui::Text(tr("target_position").c_str());
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::DragFloat3("##target_position", data.targetPosition.data(), 0.1f, -10000.0f, 10000.0f);
	} else if (data.behaviour <= 5) {
		renderEntityByComponentSelection<components::Actor>(data.actorUuid, ImGui::GetContentRegionAvail().x, 0.45f);
	}

	ImGui::Text(tr("slow_motion").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##slow_motion", &data.slowMotion);

	ImGui::Text(tr("fade_in_out").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##fade_in_out", &data.fadeInOut);

	ImGui::Text(tr("shaking").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##shaking", &data.shaking);

	ImGui::Text(tr("wide_screen").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##wide_screen", &data.wideScreen);

	if (ImGui::Button(_("edit").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		CutsceneObjectiveEditing::openCutsceneEditor(data, [&data](bool saveChanges, Data newData) {
			if (saveChanges) {
				data = newData;
			}
			WindowManager::instance().disableWindowRendering(false);
		});
	}
}
} // namespace objectives::cutscene