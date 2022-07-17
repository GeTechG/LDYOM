#include "AudioWindow.h"

#include <CCamera.h>
#include <extensions/ScriptCommands.h>


#include "fa.h"
#include "imgui-knobs.h"
#include "imgui.h"
#include "utils.h"
#include "utilsRender.h"
#include "../Data/CSoundSystem.h"
#include "fmt/core.h"

namespace CLEO {
	class CAudioStream;
}

std::string Windows::AudioWindow::getNameList() {
	return fmt::format("{} {}##list", ICON_FA_VOLUME, Localization::getInstance().get("entities.audio"));
}

std::string Windows::AudioWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_VOLUME, Localization::getInstance().get("entities.audio"));
}

int Windows::AudioWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
		.getCurrentProject()
		.getCurrentScene()
		->getAudio()
		.size());
}

void Windows::AudioWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewAudio();
}

void Windows::AudioWindow::createNewElementFrom(int i) {
	const auto& audio = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewAudioFrom(*audio);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().back()->spawnEditorAudio();
}

char* Windows::AudioWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().at(i)->getName();
}

void Windows::AudioWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().at(i)->deleteEditorAudio();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().erase(begin + i);
	this->currentElement--;
}

bool AudioCombo(Localization& local, Audio* audio) {
	bool activate = false;

	const auto & audioFilesList = Audio::getAudioFilesList();
	const auto & iterator = std::ranges::find(audioFilesList, audio->getAudioName());
	if (ImGui::BeginCombo("##audioFilesList", iterator == audioFilesList.end()? "" : iterator->c_str())) {

		for (int i = 0; i < static_cast<int>(audioFilesList.size()); ++i) {
			if (ImGui::Selectable(audioFilesList.at(i).c_str(), i == iterator - audioFilesList.begin())) {
				audio->getAudioName() = audioFilesList.at(i);
				activate = true;
			}
		}

		ImGui::EndCombo();
	}
	return activate;
}

void Windows::AudioWindow::drawOptions() {
	auto& local = Localization::getInstance();

	Audio* audio = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getAudio().at(this->currentElement).get();


	const bool isProjectDontHaveDirectory = ProjectsService::getInstance().getCurrentProject().getProjectInfo()->directory.empty();

	if (isProjectDontHaveDirectory) {
		ImGui::SetNextItemWidth(12.5f * ImGui::GetFontSize());
		ImGui::TextWrapped(local.get("audio.warning").c_str());
	}

	ImGui::BeginDisabled(isProjectDontHaveDirectory);

	if (AudioCombo(local, audio)) {
		audio->spawnEditorAudio();
	}
	auto audioFilesList = Audio::getAudioFilesList();
	const auto& iterator = std::ranges::find(audioFilesList, audio->getAudioName());
	if (iterator != audioFilesList.end()) {
		if (audio->getEditorAudio().has_value()) {
			ImGui::SameLine();
			const auto audioStream = reinterpret_cast<CLEO::CAudioStream*>(audio->getEditorAudio().value());
			switch (audioStream->GetState()) {
			case 1:
				if (ImGui::Button(ICON_FA_STOP, ImVec2(20.f, 20.f)))
					audioStream->Stop();
				break;
			default:
				if (ImGui::Button(ICON_FA_PLAY, ImVec2(20.f, 20.f)))
					audioStream->Play();
				break;
			}
		}
	}

	ImGui::SetNextItemWidth(20.f);
	if (ImGuiKnobs::Knob(local.get("audio.volume").c_str(), &audio->getVolume(), 0.0f, 2.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper)) {
		if (audio->getEditorAudio().has_value()) {
			const auto audioStream = reinterpret_cast<CLEO::CAudioStream*>(audio->getEditorAudio().value());
			audioStream->SetVolume(audio->getVolume());
		}
	}

	ImGui::Separator();

	if (utils::ToggleButton(local.get("audio.audio3D").c_str(), &audio->isAudio3D()))
		audio->spawnEditorAudio();
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), ICON_FA_EXCLAMATION_TRIANGLE);
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(local.get("audio.desclaimer_3d_audio").c_str());
	}

	float* attachPos = nullptr;
	if (audio->isAudio3D()) {
		//position
		DragPosition(audio->getPosition(), [audio] {
			audio->updateLocation();
			});
		if (ImGui::SliderInt(local.get("general.type").c_str(), &audio->getAttachType3d(), 0, 3, local.getArray("audio.attach_type_3d").at(audio->getAttachType3d()).c_str())) {
			audio->getAttachUuid() = boost::uuids::uuid{};
			audio->spawnEditorAudio();
		}

		switch (audio->getAttachType3d()) {
			case 1: {
				const auto& actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
				const int index = utils::indexByUuid(actors, audio->getAttachUuid());
				if (utils::Combo(local.get("entities.actor").c_str(), &audio->getAttachUuid(), index, static_cast<int>(actors.size()), [&actors](const int i) {
	                                           return actors.at(i)->getName();
                                           }, [&actors](const int i) {
	                                           return actors.at(i)->getUuid();
                                           })) {
					audio->spawnEditorAudio();
				}
				if (index != -1)
					attachPos = actors.at(index)->getPosition();
				break;
			}
			case 2: {
				const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
				const int index = utils::indexByUuid(vehicles, audio->getAttachUuid());
				if (utils::Combo(local.get("entities.vehicle").c_str(), &audio->getAttachUuid(), index, static_cast<int>(vehicles.size()), [&vehicles](const int i) {
	                                           return vehicles.at(i)->getName();
                                           }, [&vehicles](const int i) {
	                                           return vehicles.at(i)->getUuid();
                                           })) {
					audio->spawnEditorAudio();
				}
				if (index != -1)
					attachPos = vehicles.at(index)->getPosition();
				break;
			}
			case 3: {
				const auto& objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
				const int index = utils::indexByUuid(objects, audio->getAttachUuid());
				if (utils::Combo(local.get("entities.object").c_str(), &audio->getAttachUuid(), index, static_cast<int>(objects.size()), [&objects](const int i) {
	                                           return objects.at(i)->getName();
                                           }, [&objects](const int i) {
	                                           return objects.at(i)->getUuid();
                                           })) {
					audio->spawnEditorAudio();
				}
				if (index != -1)
					attachPos = objects.at(index)->getPosition();
				break;
			}
			default:
				break;
		}
	}

	utils::ToggleButton(local.get("audio.looped").c_str(), &audio->isIsLooped());

	ImGui::EndDisabled();

	ObjectiveDependentInput(audio);

	if (audio->isAudio3D()) {
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.camera_view").c_str());
			ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
			if (audio->getAttachType3d() == 0)
				ImGui::Text(local.get("info_overlay.move_element").c_str());
		}
		ImGui::End();

		if (audio->getAttachType3d() == 0) {
			if (utils::controlCameraWithMove(audio->getPosition())) {
				audio->updateLocation();
			}
		} else if (attachPos != nullptr) {
			utils::controlCamera({ attachPos[0], attachPos[1], attachPos[2] });
		}
	}
}

void Windows::AudioWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::AudioWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}