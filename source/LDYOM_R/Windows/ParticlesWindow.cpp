#include "ParticlesWindow.h"

#include <CCamera.h>
#include <CSprite.h>
#include <FxManager_c.h>
#include <imHotKey.h>
#include <extensions/ScriptCommands.h>

#include "EditByPlayerService.h"
#include "fa.h"
#include "HotKeyService.h"
#include "imgui.h"
#include "ModelsService.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"

std::string Windows::ParticlesWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_SPARKLES, Localization::getInstance().get("entities.particles"));
}

std::string Windows::ParticlesWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_SPARKLES, Localization::getInstance().get("entities.particle"));
}

int Windows::ParticlesWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
		.getCurrentProject()
		.getCurrentScene()
		->getParticles()
		.size());
}

void Windows::ParticlesWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewParticle();
}

char* Windows::ParticlesWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getParticles().at(i)->getName();
}

void Windows::ParticlesWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getParticles().at(i)->deleteEditorParticle();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getParticles().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getParticles().erase(begin + i);
	this->currentElement--;
}


void Windows::ParticlesWindow::drawOptions() {
	auto& local = Localization::getInstance();

	Particle* particle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getParticles().at(this->currentElement).get();

	/*ImGui::Text("RwMatrix matrix:");
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->right.x, matrix->right.y, matrix->right.z, 0.f);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->up.x, matrix->up.y, matrix->up.z, .0f);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->at.x, matrix->at.y, matrix->at.z, 0.f);
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", matrix->pos.x, matrix->pos.y, matrix->pos.z, 1.f);*/

	if (TransformEditor(particle->getPosition(), particle->getRotations(), particle->getScale().data())) {
		particle->updateLocation();
	}

	//position
	DragPosition(particle->getPosition(), [particle] {
		particle->updateLocation();
		});
	//rotations
	static std::array<float, 3> eularRot = { 0, 0, 0 };
	InputRotations(eularRot.data(), [&] {
		particle->getRotations().Set(RAD(eularRot[0]), RAD(eularRot[1]), RAD(eularRot[2]));
		particle->updateLocation();
		});
	///scale
	if (ImGui::DragFloat3(local.get("general.scale").c_str(), particle->getScale().data(), 0.001f, 0.001f, 0, "%.3f")) {
		particle->updateLocation();
	}

	ImGui::Separator();

	if (utils::Combo(local.get("entities.particle").c_str(), &particle->getParticleType(), ModelsService::getInstance().getParticlesNames())) {
		particle->spawnEditorParticle();
	}

	if (ImGui::SliderInt(local.get("particle.attach_to").c_str(), &particle->getAttachType(), 0, 3, local.getArray("particle.attach_type").at(particle->getAttachType()).c_str())) {
		particle->getAttachUuid() = boost::uuids::uuid{};
	}
	switch (particle->getAttachType()) {
	case 1: {
		const auto& actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
		const int index = utils::indexByUuid(actors, particle->getAttachUuid());
		utils::Combo(local.get("entities.actor").c_str(), &particle->getAttachUuid(), index, static_cast<int>(actors.size()), [&actors](const int i) {
			return actors.at(i)->getName();
			}, [&actors](const int i) {
				return actors.at(i)->getUuid();
			});


		const auto &pedBones = ModelsService::getInstance().getPedBones();
		if (ImGui::BeginCombo(local.get("general.bone").c_str(), pedBones.at(particle->getPedBodeId()).c_str())) {
			for (auto pair : pedBones) {
				bool isSelected = pair.first == particle->getPedBodeId();
				if (ImGui::Selectable(pair.second.c_str(), &isSelected)) {
					particle->getPedBodeId() = pair.first;
					particle->updateLocation();
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (index != -1) {
			RwV3d bonePos;
			actors.at(index)->getEditorPed().value()->GetBonePosition(bonePos, particle->getPedBodeId(), false);

			RwV3d screenCoords;
			CSprite::CalcScreenCoors(bonePos, &screenCoords, &ImGui::GetIO().DisplaySize.x, &ImGui::GetIO().DisplaySize.y, false, false);
			ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(screenCoords.x, screenCoords.y), 5.f, ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 0.f, 0.f, 1.f)));
		}

		break;
	}
	case 2: {
		const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
		const int index = utils::indexByUuid(vehicles, particle->getAttachUuid());
		utils::Combo(local.get("entities.vehicle").c_str(), &particle->getAttachUuid(), index, static_cast<int>(vehicles.size()), [&vehicles](const int i) {
			return vehicles.at(i)->getName();
			}, [&vehicles](const int i) {
				return vehicles.at(i)->getUuid();
			});
		break;
	}
	case 3: {
		const auto& objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
		const int index = utils::indexByUuid(objects, particle->getAttachUuid());
		utils::Combo(local.get("entities.object").c_str(), &particle->getAttachUuid(), index, static_cast<int>(objects.size()), [&objects](const int i) {
			return objects.at(i)->getName();
			}, [&objects](const int i) {
				return objects.at(i)->getUuid();
			});
		break;
	}
	default:
		break;
	} 

	if (ImGui::Button(local.get("object.edit_by_camera").c_str())) {
		EditByPlayerService::getInstance().positionalObject(nullptr, [particle](CMatrix& mat) {
			mat.UpdateRW(&particle->getEditorParticle().value()->m_localMatrix);
		}, particle->getPosition(), particle->getRotations(), false);
	}

	ObjectiveDependentInput(particle);

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("info_overlay.camera_view").c_str());
		ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
		ImGui::Text(local.get("info_overlay.move_element").c_str());
		char guizmoTranslate[32];
		ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoTranslate")->functionKeys, guizmoTranslate, sizeof guizmoTranslate);
		ImGui::Text(fmt::format("{} - {}", local.get("hotkey_editor.hk_guizmoTranslate"), guizmoTranslate).c_str());
		char guizmoRotate[32];
		ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoRotate")->functionKeys, guizmoRotate, sizeof guizmoRotate);
		ImGui::Text(fmt::format("{} - {}", local.get("hotkey_editor.hk_guizmoRotate"), guizmoRotate).c_str());
		char guizmoScale[32];
		ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoScale")->functionKeys, guizmoScale, sizeof guizmoScale);
		ImGui::Text(fmt::format("{} - {}", local.get("hotkey_editor.hk_guizmoScale"), guizmoScale).c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();

	if (utils::controlCameraWithMove(particle->getPosition())) {
		particle->updateLocation();
	}
}

void Windows::ParticlesWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
}

void Windows::ParticlesWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}