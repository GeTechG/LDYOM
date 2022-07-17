#include "utilsRender.h"

#include <CCamera.h>
#include <CRadar.h>

#include "fa.h"
#include "imgui.h"
#include "Localization/Localization.h"
#include "plugin.h"
#include "CWorld.h"
#include "ProjectsService.h"
#include "utils.h"
#include "ImGuizmo/ImGuizmo.h"
#include <boost/math/quaternion.hpp>

#include "HotKeyService.h"

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

void DragPosition(float* position, const std::function<void()>& callback) {
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
	if (ImGui::DragFloat3("##inputPos", position, 0.001f, NULL, NULL, "%.6f"))
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

void ObjectiveDependentInput(ObjectiveDependent* objectiveDependent) {
	utils::ToggleButton(Localization::getInstance().get("general.use_objectives").c_str(), &objectiveDependent->isUseObjective());

	ImGui::BeginDisabled(!objectiveDependent->isUseObjective());
	const auto& objectives = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives();

	const int spawnObjectiveIdx = utils::indexByUuid(objectives, objectiveDependent->getSpawnObjectiveUuid());
	IncorrectHighlight(spawnObjectiveIdx == -1 && objectiveDependent->isUseObjective(), [&] {
		utils::Combo(Localization::getInstance().get("general.spawn_before").c_str(), &objectiveDependent->getSpawnObjectiveUuid(), spawnObjectiveIdx, static_cast<int>(objectives.size()), [&objectives](const int i) {
			return objectives.at(i)->getName();
			}, [&objectives](const int i) {
				return objectives.at(i)->getUuid();
			});
	});

	const int deleteObjectiveIdx = utils::indexByUuid(objectives, objectiveDependent->getDeleteObjectiveUuid());
	utils::Combo(Localization::getInstance().get("general.delete_after").c_str(), &objectiveDependent->getDeleteObjectiveUuid(), deleteObjectiveIdx, static_cast<int>(objectives.size()), [&objectives](const int i) {
		return objectives.at(i)->getName();
		}, [&objectives](const int i) {
			return objectives.at(i)->getUuid();
		});
	ImGui::EndDisabled();
}

void DragRotations(float* rotations, const std::function<void()>& callback) {
	ImGui::PushID("rotationsGroup");
	ImGui::Text(Localization::getInstance().get("general.rotate").c_str());
	if (ImGui::DragFloat3("##inputRots", rotations, 0.5f, -180.0f, 180.0f, "%.3f °"))
		callback();

	ImGui::PopID();
}

void InputRotations(float* rotations, const std::function<void()>& callback) {
	ImGui::PushID("rotationsGroup");
	ImGui::Text(Localization::getInstance().get("general.rotate").c_str());
	ImGui::InputFloat3("##inputRots", rotations, "%.3f °");
	ImGui::SameLine();
	if (ImGui::Button(Localization::getInstance().get("general.apply").c_str()))
		callback();

	ImGui::PopID();
}

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16) {
	const float temp = 2.0f * znear;
	const float temp2 = right - left;
	const float temp3 = top - bottom;
	const float temp4 = zfar - znear;
	m16[0] = temp / temp2;
	m16[1] = 0.0;
	m16[2] = 0.0;
	m16[3] = 0.0;
	m16[4] = 0.0;
	m16[5] = temp / temp3;
	m16[6] = 0.0;
	m16[7] = 0.0;
	m16[8] = (right + left) / temp2;
	m16[9] = (top + bottom) / temp3;
	m16[10] = (-zfar - znear) / temp4;
	m16[11] = -1.0f;
	m16[12] = 0.0;
	m16[13] = 0.0;
	m16[14] = (-temp * zfar) / temp4;
	m16[15] = 0.0;
}

void Cross(const float* a, const float* b, float* r)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

float Dot(const float* a, const float* b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void Normalize(const float* a, float* r)
{
	const float il = 1.f / (sqrtf(Dot(a, a)) + FLT_EPSILON);
	r[0] = a[0] * il;
	r[1] = a[1] * il;
	r[2] = a[2] * il;
}

void LookAt(const float* eye, const float* at, const float* up, float* m16)
{
	float X[3], Y[3], Z[3], tmp[3];

	tmp[0] = eye[0] - at[0];
	tmp[1] = eye[1] - at[1];
	tmp[2] = eye[2] - at[2];
	Normalize(tmp, Z);
	Normalize(up, Y);

	Cross(Y, Z, tmp);
	Normalize(tmp, X);

	Cross(Z, X, tmp);
	Normalize(tmp, Y);

	m16[0] = X[0];
	m16[1] = Y[0];
	m16[2] = Z[0];
	m16[3] = 0.0f;
	m16[4] = X[1];
	m16[5] = Y[1];
	m16[6] = Z[1];
	m16[7] = 0.0f;
	m16[8] = X[2];
	m16[9] = Y[2];
	m16[10] = Z[2];
	m16[11] = 0.0f;
	m16[12] = -Dot(X, eye);
	m16[13] = -Dot(Y, eye);
	m16[14] = -Dot(Z, eye);
	m16[15] = 1.0f;
}

void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
{
	const float ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
	const float xmax = ymax * aspectRatio;
	Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}

void EditTransform(const float* cameraView, const float* cameraProjection, float* matrix, const bool editTransformDecomposition) {
	auto& local = Localization::getInstance();

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (editTransformDecomposition)
	{
		const auto hotKey = HotKeyService::getInstance().getHotKey();
		if (hotKey != nullptr) {
			if (std::strcmp(hotKey->functionName, "guizmoTranslate") == 0) // key Z
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (std::strcmp(hotKey->functionName, "guizmoRotate") == 0) //key E
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			if (std::strcmp(hotKey->functionName, "guizmoScale") == 0) // r Key
				mCurrentGizmoOperation = ImGuizmo::SCALE;
		}

		ImGui::PushID("guizmo_modes");
		if (ImGui::RadioButton(local.get("general.position").c_str(), mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton(local.get("general.rotate").c_str(), mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton(local.get("general.scale").c_str(), mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		ImGui::SameLine();
		if (ImGui::RadioButton(local.get("general.universal").c_str(), mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
			mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
		ImGui::PopID();

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton(local.get("general.local").c_str(), mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton(local.get("general.world").c_str(), mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##UseSnap", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3(local.get("general.snap").c_str(), &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat(local.get("general.angle_snap").c_str(), &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat(local.get("general.scale_snap").c_str(), &snap[0]);
			break;
		}
		ImGui::Checkbox(local.get("general.bound_sizing").c_str(), &boundSizing);
		if (boundSizing) {
			ImGui::PushID(3);
			ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3(local.get("general.snap").c_str(), boundsSnap);
			ImGui::PopID();
		}
	}

	const ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, nullptr, useSnap ? &snap[0] : nullptr, boundSizing ? bounds : nullptr, boundSizingSnap ? boundsSnap : nullptr);
}

std::array<float, 3> toEular(std::array<float, 16> m) {
	auto sy = sqrt(static_cast<double>(m[0]) * static_cast<double>(m[0]) + static_cast<double>(m[4]) * static_cast<double>(m[4]));

	bool singular = sy < 1e-6; // If

	float x, y, z;
	if (!singular)
	{
		x = atan2(m[9], m[10]);
		y = atan2(-m[8], sy);
		z = atan2(m[4], m[0]);
	}
	else
	{
		x = atan2(-m[6], m[5]);
		y = atan2(-m[8], sy);
		z = 0;
	}

	return {-x, -y, -z};
}

void makeMatrix(std::array<float, 16>& m, CQuaternion& q, const CVector& position, float* scale) {
	float sqw = q.real * q.real;
	float sqx = q.imag.x * q.imag.x;
	float sqy = q.imag.y * q.imag.y;
	float sqz = q.imag.z * q.imag.z;

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = 1.f / (sqx + sqy + sqz + sqw);
	m[0] = (sqx - sqy - sqz + sqw) * invs * scale[0]; // since sqw + sqx + sqy + sqz =1/invs*invs
	m[5] = (-sqx + sqy - sqz + sqw) * invs * scale[1];
	m[10] = (-sqx - sqy + sqz + sqw) * invs * scale[2];

	float tmp1 = q.imag.x * q.imag.y;
	float tmp2 = q.imag.z * q.real;
	m[4] = 2.f * (tmp1 + tmp2) * invs * scale[1];
	m[1] = 2.f * (tmp1 - tmp2) * invs * scale[0];

	tmp1 = q.imag.x * q.imag.z;
	tmp2 = q.imag.y * q.real;
	m[8] = 2.f * (tmp1 - tmp2) * invs * scale[2];
	m[2] = 2.f * (tmp1 + tmp2) * invs * scale[0];
	tmp1 = q.imag.y * q.imag.z;
	tmp2 = q.imag.x * q.real;
	m[9] = 2.f * (tmp1 + tmp2) * invs * scale[2];
	m[6] = 2.f * (tmp1 - tmp2) * invs * scale[1];

	m[12] = position.x;
	m[13] = position.y;
	m[14] = position.z;
	m[15] = 1.f;
}

bool TransformEditor(float* translate, CQuaternion& rotates, float* scale) {
	float cameraProjection[16];
	float cameraView[16];
	const auto& cameraMatrix = TheCamera.m_mCameraMatrix;

	const float camXAngle = atan2f(cameraMatrix.up.z, cameraMatrix.at.z);
	//const float camYAngle = atan2f(-cameraMatrix.right.z, sqrtf(cameraMatrix.up.z * cameraMatrix.up.z + cameraMatrix.at.z * cameraMatrix.at.z));
	const float camZAngle = atan2f(cameraMatrix.right.y, -cameraMatrix.right.x);

	const auto dist = DistanceBetweenPoints(TheCamera.GetPosition(), CVector(translate[0], translate[1], translate[2]));

	float eye[] = { cosf(camZAngle) * cosf(-camXAngle) * dist, sinf(-camXAngle) * dist, sinf(camZAngle) * cosf(camXAngle) * dist };
	constexpr float at[] = { 0.f, 0.f, 0.f };
	constexpr float up[] = { 0.f, 1.f, 0.f };
	LookAt(eye, at, up, cameraView);

	const CVector pos = { -(translate[1] - (cameraMatrix.pos.y + eye[0])), translate[2] - (cameraMatrix.pos.z + -eye[1]), -(translate[0] - (cameraMatrix.pos.x + eye[2])) };
	//std::array scaleVec = { scale, scale, scale };
	//std::array rot = { -rotates[1] + 360, rotates[2], -rotates[0]};
	std::array<float, 16> matrix{};
	//ImGuizmo::RecomposeMatrixFromComponents(pos.data(), rot.data(), scaleVec.data(), matrix.data());
	makeMatrix(matrix, rotates, pos, scale);

	Perspective(TheCamera.FindCamFOV() / 2.0f, ImGui::GetIO().DisplaySize.x / ImGui::GetIO().DisplaySize.y, 0.1f, 100.f, cameraProjection);

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();

	/*ImGui::Text("imgizmo matrix:");
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix[0], matrix[1], matrix[2], matrix[3]);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix[4], matrix[5], matrix[6], matrix[7]);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix[8], matrix[9], matrix[10], matrix[11]);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix[12], matrix[13], matrix[14], matrix[15]);*/

	EditTransform(cameraView, cameraProjection, matrix.data(), true);

	if (ImGuizmo::IsUsing()) {
		float newScale[3];
		float newRot[3];
		float newPos[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix.data(), newPos, newRot, newScale);

		matrix[0] /= newScale[0];
		matrix[1] /= newScale[0];
		matrix[2] /= newScale[0];
		matrix[4] /= newScale[1];
		matrix[5] /= newScale[1];
		matrix[6] /= newScale[1];
		matrix[8] /= newScale[2];
		matrix[9] /= newScale[2];
		matrix[10] /= newScale[2];

		rotates.real = sqrt(max(0, 1 + matrix[0] + matrix[5] + matrix[10])) / 2;
		rotates.imag.x = sqrt(max(0, 1 + matrix[0] - matrix[5] - matrix[10])) / 2;
		rotates.imag.y = sqrt(max(0, 1 - matrix[0] + matrix[5] - matrix[10])) / 2;
		rotates.imag.z = sqrt(max(0, 1 - matrix[0] - matrix[5] + matrix[10])) / 2;
		rotates.imag.x = _copysignf(rotates.imag.x, matrix[9] - matrix[6]);
		rotates.imag.y = _copysignf(rotates.imag.y, matrix[2] - matrix[8]);
		rotates.imag.z = _copysignf(rotates.imag.z, matrix[4] - matrix[1]);


		/*rotates[0] = -roundf(DEG(newRot2[2]) * 1000.0f) / 1000.0f;
		rotates[1] = -roundf(DEG(newRot2[0]) * 1000.0f) / 1000.0f;
		rotates[2] =  roundf(DEG(newRot2[1]) * 1000.0f) / 1000.0f;*/
		translate[0] = -newPos[2] + (cameraMatrix.pos.x + eye[2]);
		translate[1] = -newPos[0] + (cameraMatrix.pos.y + eye[0]);
		translate[2] = newPos[1] + (cameraMatrix.pos.z + -eye[1]);
		memcpy(scale, newScale, sizeof newScale);
		return true;
	}

	return false;
}

void IncorrectHighlight(const bool cond, const std::function<void()> render) {
	if (cond) {
		const ImU32 originalColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		const ImU32 blendColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 0.f, 0.f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImAlphaBlendColors(originalColor, blendColor));
	}

	render();

	if (cond)
		ImGui::PopStyleColor();
}
