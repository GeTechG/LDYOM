#include "utilsRender.h"

#include "fa.h"
#include "imgui.h"
#include "Localization/Localization.h"
#include "plugin.h"
#include "CWorld.h"
#include "utils.h"

using namespace plugin;

void InputPosition(float* position, const std::function<void()>& callback) {
	ImGui::PushID("positionGroup");
	ImGui::Text(Localization::getInstance().get("general.position").c_str());
	if (ImGui::Button(ICON_FA_STREET_VIEW, ImVec2(25.0f, 0.0f))) {
		const CVector playerPosition = FindPlayerPed()->GetPosition();
		position[0] = playerPosition.x;
		position[1] = playerPosition.y;
		position[2] = playerPosition.z;
		callback();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(Localization::getInstance().get("general.player_position").c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	if (ImGui::InputFloat3("##inputPos", position, "%.6f"))
		callback();

	ImGui::PopID();
}

void DragAngleRotation(float* angleRotation, const std::function<void()>& callback) {
	ImGui::PushID("headingGroup");

	ImGui::Text(Localization::getInstance().get("general.angle_rotation").c_str());
	if (ImGui::Button(ICON_FA_STREET_VIEW, ImVec2(25.0f, 0.0f))) {
		const float playerAngleRotation = FindPlayerPed()->GetHeading();
		*angleRotation = DEG(playerAngleRotation);
		callback();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(Localization::getInstance().get("general.player_angle_rotation").c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
	if (ImGui::DragFloat("##headingActor", angleRotation, 0.5f, -180.0, 180.0, "%.3f °"))
		callback();

	ImGui::PopID();
}
