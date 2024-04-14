#include "PlayCameraPath.h"

#include <CHud.h>
#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>
#include "imgui.h"
#include "imgui_stdlib.h"

#include "CCameraExtend.h"
#include "CutsceneMutex.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "TimeUtils.h"
#include "utilsRender.h"

PlayCameraPathObjective::PlayCameraPathObjective(void *_new) : BaseObjective(nullptr) {}

void PlayCameraPathObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &cameraPaths = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCameraPaths();
	const int indexCameraPath = utils::indexByUuid(cameraPaths, this->cameraPathUuid_);

	IncorrectHighlight(indexCameraPath == -1, [&] {
		utils::Combo(local.get("entities.camera_path").c_str(), &this->cameraPathUuid_, indexCameraPath,
		             cameraPaths.size(),
		             [&cameraPaths](const int i) {
			             return std::ref(cameraPaths.at(i)->getName());
		             }, [&cameraPaths](const int i) {
			             return cameraPaths.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (indexCameraPath == -1) {
			if (!cameraPaths.empty()) {
				this->cameraPathUuid_ = cameraPaths.back()->getUuid();
			}
		}
	}

	ImGui::Separator();

	ImGui::InputText(local.get("general.text").c_str(), &this->text);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	utils::ToggleButton(local.get("cutscene_objective.start_fade_out").c_str(), &this->startFadeOut);
	ImGui::BeginDisabled(!this->startFadeOut);
	ImGui::DragFloat(local.get("cutscene_objective.fade_out_time").c_str(), &this->startFadeOutTime, 0.01f);
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("cutscene_objective.end_fade_in").c_str(), &this->endFadeIn);
	ImGui::BeginDisabled(!this->endFadeIn);
	ImGui::DragFloat(local.get("cutscene_objective.fade_in_time").c_str(), &this->endFadeInTime, 0.01f);
	ImGui::EndDisabled();

	ImGui::Separator();

	utils::ToggleButton(local.get("cutscene_objective.lock_player_control").c_str(), &this->lockPlayerControl_);
	utils::ToggleButton(local.get("cutscene_objective.asynchronous").c_str(), &this->async_);
}

ktwait PlayCameraPathObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	const auto &cameraPaths = scene->getCameraPaths();
	const int indexCameraPath = utils::indexByUuid(cameraPaths, this->cameraPathUuid_);

	if (indexCameraPath == -1) {
		setObjectiveError(result, *this, NotSelected, "The camera path for the objective is not selected.");
		co_return;
	}

	const auto &cameraPath = cameraPaths.at(indexCameraPath).get();

	while (CutsceneMutex::isLocked()) {
		co_await 1;
	}


	bool lockPlayerControl = this->lockPlayerControl_ && Command<Commands::IS_PLAYER_CONTROL_ON>(0);

	if (lockPlayerControl)
		Command<Commands::SET_PLAYER_CONTROL>(0, false);

	auto cp1251Text = utf8ToCp1251(this->text);
	gxtEncode(cp1251Text);
	this->gameText = cp1251Text;

	CMessages::AddMessage(this->gameText.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	CTheScripts::bDisplayHud = false;
	CHud::bScriptDontDisplayRadar = true;

	static auto task = [](PlayCameraPathObjective *playCameraPathObjective, CameraPath *cameraPath,
	                      bool lockPlayerControl) -> ktwait {
		CutsceneMutexGuard guard;

		if (playCameraPathObjective->isStartFadeOut())
			plugin::Command<Commands::DO_FADE>(
				static_cast<int>(playCameraPathObjective->getStartFadeOutTime() * 1000.f), 1);

		TheCameraExtend.setExtendMode(true);

		TheCameraExtend.playCameraPath(cameraPath);

		const std::chrono::milliseconds
			duration(static_cast<unsigned>(cameraPath->getTime() * 1000.f));
		if (playCameraPathObjective->isEndFadeIn()) {
			const std::chrono::milliseconds fadeInDuration(
				static_cast<int>(playCameraPathObjective->getEndFadeInTime() * 1000.f));
			co_await waitInGame((duration - fadeInDuration).count());
			plugin::Command<Commands::DO_FADE>(static_cast<int>(fadeInDuration.count()), 0);
			co_await waitInGame(fadeInDuration.count());
		} else {
			co_await waitInGame(duration.count());
		}

		TheCameraExtend.stopCameraPath();
		TheCameraExtend.setExtendMode(false);

		if (lockPlayerControl)
			Command<Commands::SET_PLAYER_CONTROL>(0, true);
	};

	if (this->async_)
		tasklist.add_task(task, this, cameraPath, lockPlayerControl);
	else
		co_await task(this, cameraPath, lockPlayerControl);
}

boost::uuids::uuid& PlayCameraPathObjective::getCameraPathUuid() { return cameraPathUuid_; }
std::string& PlayCameraPathObjective::getText() { return text; }
float& PlayCameraPathObjective::getTextTime() { return textTime_; }
bool& PlayCameraPathObjective::isAsync() { return async_; }
bool& PlayCameraPathObjective::isStartFadeOut() { return startFadeOut; }
float& PlayCameraPathObjective::getStartFadeOutTime() { return startFadeOutTime; }
bool& PlayCameraPathObjective::isEndFadeIn() { return endFadeIn; }
float& PlayCameraPathObjective::getEndFadeInTime() { return endFadeInTime; }
bool& PlayCameraPathObjective::isLockPlayerControl() { return lockPlayerControl_; }
std::string& PlayCameraPathObjective::getGameText() { return gameText; }
