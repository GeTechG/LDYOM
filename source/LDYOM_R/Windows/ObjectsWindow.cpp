#include "ObjectsWindow.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <CCamera.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

#include <glm/gtc/quaternion.hpp>
#include "EditByPlayerService.h"
#include "fa.h"
#include "HotKeyService.h"
#include "imgui.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"
#include "glm/glm.hpp"
#include "glm/detail/type_quat.hpp"
#include "Localization/Localization.h"

std::string Windows::ObjectsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_CUBES, Localization::getInstance().get("entities.objects"));
}

std::string Windows::ObjectsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_CUBE, Localization::getInstance().get("entities.object"));
}

int Windows::ObjectsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getObjects()
	                        .size());
}

void Windows::ObjectsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObject();
}

void Windows::ObjectsWindow::createNewElementFrom(int i) {
	const auto &object = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectFrom(*object);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().back()->spawnEditorObject();
}

char* Windows::ObjectsWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().at(i)->getName();
}

void Windows::ObjectsWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().at(i)->deleteEditorObject();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().erase(begin + i);
	this->currentElement--;
}

void Windows::ObjectsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Object *object = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().at(
		this->currentElement).get();

	const auto matrix = object->getEditorObject().value()->GetMatrix();

	/*ImGui::Text("RwMatrix matrix:");
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->right.x, matrix->right.y, matrix->right.z, 0.f);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->up.x, matrix->up.y, matrix->up.z, .0f);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->at.x, matrix->at.y, matrix->at.z, 0.f);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->pos.x, matrix->pos.y, matrix->pos.z, 1.f);*/

	if (TransformEditor(object->getPosition(), object->getRotations(), object->getScale().data())) {
		object->updateLocation();
	}

	//position
	DragPosition(object->getPosition(), [object] {
		object->updateLocation();
	});

	static CQuaternion lastQ;

	const auto q = object->getRotations();
	const glm::quat quaternion(q.real, q.imag.x, q.imag.y, q.imag.z);
	const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);

	//rotations
	static std::array<float, 3> eularRot = {0, 0, 0};
	if (abs(q.real - lastQ.real) > FLT_EPSILON || abs(q.imag.x - lastQ.imag.x) > FLT_EPSILON ||
		abs(q.imag.y - lastQ.imag.y) > FLT_EPSILON || abs(q.imag.z - lastQ.imag.z) > FLT_EPSILON) {
		eularRot[0] = glm::degrees(eulerAngles.x);
		eularRot[1] = glm::degrees(eulerAngles.y);
		eularRot[2] = glm::degrees(eulerAngles.z);
		lastQ = q;
	}
	InputRotations(eularRot.data(), [&] {
		object->getRotations().Set(RAD(eularRot[1]), RAD(eularRot[2]), RAD(eularRot[0]));
		object->updateLocation();
	});
	///scale
	if (ImGui::DragFloat3(local.get("general.scale").c_str(), object->getScale().data(), 0.001f, 0.001f, 0, "%.3f")) {
		object->updateLocation();
	}

	ImGui::Separator();

	if (ImGui::Button(ICON_FA_CUBES, ImVec2(ImGui::GetFontSize() * 1.56f, ImGui::GetFontSize() * 1.56f))) {
		objectSelectorPopup_.open();
		objectSelectorPopup_.setCallbackSelect([&object](int modelId) {
			object->getModelId() = modelId;
			object->spawnEditorObject();
		});
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(local.get("object_selector.title").c_str());
	}
	ImGui::SameLine();
	ImGui::InputInt(local.get("general.model").c_str(), &object->getModelId(), 0, 0);
	ImGui::SameLine();
	if (ImGui::Button(local.get("general.apply").c_str())) {
		object->spawnEditorObject();
	}
	if (ImGui::Button(local.get("object.edit_by_camera").c_str())) {
		EditByPlayerService::getInstance().positionalObject(object->getEditorObject().value(), [object](CMatrix &mat) {
			const auto obj = object->getEditorObject().value();
			CWorld::Remove(obj);
			obj->SetMatrix(mat);
			obj->UpdateRwFrame();
			CWorld::Add(obj);
		}, object->getPosition(), object->getRotations(), false);
	}

	this->objectViewerPopup_.draw();
	this->objectSelectorPopup_.draw();

	ObjectiveDependentInput(object);

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("info_overlay.camera_view").c_str());
		ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
		ImGui::Text(local.get("info_overlay.move_element").c_str());
		char guizmoTranslate[32];
		ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoTranslate")->functionKeys,
		                       guizmoTranslate, sizeof guizmoTranslate);
		ImGui::Text(fmt::format("{} - {}", local.get("hotkey_editor.hk_guizmoTranslate"), guizmoTranslate).c_str());
		char guizmoRotate[32];
		ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoRotate")->functionKeys, guizmoRotate,
		                       sizeof guizmoRotate);
		ImGui::Text(fmt::format("{} - {}", local.get("hotkey_editor.hk_guizmoRotate"), guizmoRotate).c_str());
		char guizmoScale[32];
		ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoScale")->functionKeys, guizmoScale,
		                       sizeof guizmoScale);
		ImGui::Text(fmt::format("{} - {}", local.get("hotkey_editor.hk_guizmoScale"), guizmoScale).c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();

	if (utils::controlCameraWithMove(object->getPosition())) {
		object->updateLocation();
	}
}

void Windows::ObjectsWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::ObjectsWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}
