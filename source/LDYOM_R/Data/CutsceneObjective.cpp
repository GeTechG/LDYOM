#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS
#include "CutsceneObjective.h"

#include <CCamera.h>
#include <CGame.h>
#include <CHud.h>
#include <CMenuManager.h>
#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>

#include "CutsceneMutex.h"
#include "EditByPlayerService.h"
#include "imgui.h"
#include "MathUtils.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "tweeny.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

void CutsceneObjective::updateLocation() {
	const CVector pos = {this->position_[0], this->position_[1], this->position_[2]};
	CVector up = {0.f, sinf(RAD(this->xAngle_)), cosf(RAD(this->xAngle_))};
	TheCamera.SetCamPositionForFixedMode(&pos, &up);

	/*CVector aimPos = {
		2.f * (rotation_.imag.x * rotation_.imag.y - rotation_.real * rotation_.imag.z),
		1.f - 2.f * (rotation_.imag.x * rotation_.imag.x + rotation_.imag.z * rotation_.imag.z),
		2.f * (rotation_.imag.y * rotation_.imag.z + rotation_.real * rotation_.imag.x)
	};*/

	const CMatrix matrix = MathUtils::quatToMatrix(this->rotation_);
	CVector aimPos = {matrix.right.z, matrix.up.z, matrix.at.z};
	aimPos += pos;
	TheCamera.TakeControlNoEntity(&aimPos, 2, 1);
}

CutsceneObjective::CutsceneObjective(const CVector &position, const CQuaternion &rotation): BaseObjective(nullptr),
	rotation_(rotation) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.cutscene"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);

	this->position_ = {position.x, position.y, position.z};
}

void CutsceneObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	//position
	DragPosition(this->position_.data(), [&] {
		this->updateLocation();
	});

	//rotations
	static std::array<float, 3> eularRot = {0, 0, 0};
	InputRotations(eularRot.data(), [&] {
		this->rotation_.Set(0.f, RAD(eularRot[1]), RAD(eularRot[2]));
		this->xAngle_ = eularRot[0];
		this->updateLocation();
	});

	if (ImGui::Button(local.get("general.edit_manually").c_str())) {
		EditByPlayerService::getInstance().editByPlayerCamera(this->position_.data(), &this->rotation_,
		                                                      this->wideScreen_, [this] {
			                                                      this->xAngle_ = 0.f;
			                                                      this->updateLocation();
		                                                      });
	}

	ImGui::Separator();

	auto entityCombo = [&]<typename T>(const char *name, boost::uuids::uuid *uuid,
	                                   std::vector<std::unique_ptr<T>> &entities) {
		const int index = utils::indexByUuid(entities, *uuid);

		IncorrectHighlight(index == -1, [&] {
			utils::Combo(local.get(name).c_str(), uuid, index, static_cast<int>(entities.size()),
			             [&entities](const int i) {
				             return entities.at(i)->getName();
			             }, [&entities](const int i) {
				             return entities.at(i)->getUuid();
			             });
		});
	};

	ImGui::PushID("attachGroup");
	if (ImGui::SliderInt(local.get("cutscene_objective.attach").c_str(), &this->attachType_, 0, 3,
	                     local.getArray("cutscene_objective.attach_types")[this->attachType_].c_str()))
		this->attachUuid_ = boost::uuids::uuid{};

	switch (this->attachType_) {
	case 1:
		entityCombo("entities.actor", &this->attachUuid_,
		            ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors());
		break;
	case 2:
		entityCombo("entities.vehicle", &this->attachUuid_,
		            ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles());
		break;
	case 3:
		entityCombo("entities.object", &this->attachUuid_,
		            ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects());
		break;
	default: break;
	}
	ImGui::PopID();

	ImGui::PushID("followGroup");
	if (ImGui::SliderInt(local.get("cutscene_objective.follow").c_str(), &this->followType_, 0, 3,
	                     local.getArray("cutscene_objective.follow_types")[this->followType_].c_str()))
		this->followUuid_ = boost::uuids::uuid{};

	switch (this->followType_) {
	case 1:
		entityCombo("entities.actor", &this->followUuid_,
		            ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors());
		break;
	case 2:
		entityCombo("entities.vehicle", &this->followUuid_,
		            ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles());
		break;
	case 3:
		entityCombo("entities.object", &this->followUuid_,
		            ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects());
		break;
	default: break;
	}
	ImGui::PopID();

	ImGui::BeginDisabled(this->attachType_ != 0);

	ImGui::Separator();

	utils::ToggleButton(local.get("cutscene_objective.interpolation").c_str(), &this->interpolation_);

	ImGui::BeginDisabled(!this->interpolation_);
	ImGui::Text(local.get("cutscene_objective.position_interpolation").c_str());

	ImGui::PushItemWidth(ImGui::GetFontSize() * 5.f);
	EasingCombo("X", &this->positionXInterpolationType_);
	ImGui::SameLine();
	EasingCombo("Y", &this->positionYInterpolationType_);
	ImGui::SameLine();
	EasingCombo("Z", &this->positionZInterpolationType_);
	ImGui::PopItemWidth();

	EasingCombo(local.get("cutscene_objective.rotation_interpolation").c_str(), &this->rotationInterpolationType_);
	ImGui::EndDisabled();

	ImGui::EndDisabled();

	ImGui::Separator();

	utils::ToggleButton(local.get("cutscene_objective.start_fade_out").c_str(), &this->startFadeOut);
	ImGui::BeginDisabled(!this->startFadeOut);
	ImGui::DragFloat(local.get("cutscene_objective.fade_out_time").c_str(), &this->startFadeOutTime, 0.01f);
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("cutscene_objective.end_fade_in").c_str(), &this->endFadeIn);
	ImGui::BeginDisabled(!this->endFadeIn);
	ImGui::DragFloat(local.get("cutscene_objective.fade_in_time").c_str(), &this->endFadeInTime, 0.01f);
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("cutscene_objective.widescreen").c_str(), &this->wideScreen_);
	utils::ToggleButton(local.get("cutscene_objective.final_scene").c_str(), &this->endCutscene_);
	utils::ToggleButton(local.get("cutscene_objective.lock_player_control").c_str(), &this->lockPlayerControl_);
	utils::ToggleButton(local.get("cutscene_objective.asynchronous").c_str(), &this->async_);

	listOverlay.emplace_back(local.get("info_overlay.rotate_with_shift"));

	if (plugin::KeyPressed(VK_SHIFT)) {
		if (plugin::KeyPressed(VK_UP)) {
			CQuaternion temp;
			temp.Set(0.f, 0.f, RAD(1));
			CQuaternion res;
			res.Multiply(this->rotation_, temp);
			this->rotation_ = res;
			this->updateLocation();
		} else if (plugin::KeyPressed(VK_DOWN)) {
			CQuaternion temp;
			temp.Set(0.f, 0.f, RAD(-1));
			CQuaternion res;
			res.Multiply(this->rotation_, temp);
			this->rotation_ = res;
			this->updateLocation();
		}

		if (plugin::KeyPressed(VK_LEFT)) {
			CQuaternion temp;
			temp.Set(0.f, RAD(-1), 0.f);
			CQuaternion res;
			res.Multiply(this->rotation_, temp);
			this->rotation_ = res;
			this->updateLocation();
		} else if (plugin::KeyPressed(VK_RIGHT)) {
			CQuaternion temp;
			temp.Set(0.f, RAD(1), 0.f);
			CQuaternion res;
			res.Multiply(this->rotation_, temp);
			this->rotation_ = res;
			this->updateLocation();
		}

		if (plugin::KeyPressed('Q')) {
			this->xAngle_ += 1.f;
			this->updateLocation();
		} else if (plugin::KeyPressed('E')) {
			this->xAngle_ += -1.f;
			this->updateLocation();
		}
	}
}

ktwait CutsceneObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	while (CutsceneMutex::isLocked()) {
		co_await 1;
	}

	bool lockPlayerControl = this->lockPlayerControl_ && !Command<Commands::IS_PLAYER_CONTROL_ON>(0);

	if (lockPlayerControl)
		Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	CEntity *targetAttach = nullptr;
	int indexAttach = -1;
	CVector targetPositionOffset = {0.f, 0.f, 0.f};
	CQuaternion targetOffsetRotation;

	if (this->attachType_ > 0) {
		switch (this->attachType_) {
		case 1:
			indexAttach = utils::indexByUuid(scene->getActors(), this->attachUuid_);
			if (indexAttach != -1) {
				if (const auto ped = scene->getActors().at(indexAttach)->getProjectPed(); ped.has_value()) {
					targetAttach = ped.value();
					targetPositionOffset = {
						this->position_[0] - scene->getActors().at(indexAttach)->getPosition()[0],
						this->position_[1] - scene->getActors().at(indexAttach)->getPosition()[1],
						this->position_[2] - scene->getActors().at(indexAttach)->getPosition()[2]
					};
					auto m = MathUtils::matrixFromEular(
						RAD(-scene->getActors().at(indexAttach)->getHeadingAngle()), 0.f, 0.f);
					rotateVec2(targetPositionOffset.x, targetPositionOffset.y,
					           -scene->getActors().at(indexAttach)->getHeadingAngle());
					targetOffsetRotation = MathUtils::multiply(this->rotation_, MathUtils::matrixToQuat(m));
				}
			}
			break;
		case 2:
			indexAttach = utils::indexByUuid(scene->getVehicles(), this->attachUuid_);
			if (indexAttach != -1) {
				if (const auto vehicle = scene->getVehicles().at(indexAttach)->getProjectVehicle(); vehicle.
					has_value()) {
					targetAttach = vehicle.value();
					targetPositionOffset = {
						this->position_[0] - scene->getVehicles().at(indexAttach)->getPosition()[0],
						this->position_[1] - scene->getVehicles().at(indexAttach)->getPosition()[1],
						this->position_[2] - scene->getVehicles().at(indexAttach)->getPosition()[2]
					};
					auto m = MathUtils::matrixFromEular(
						0.f, RAD(-scene->getVehicles().at(indexAttach)->getHeadingAngle()), 0.f);
					rotateVec2(targetPositionOffset.x, targetPositionOffset.y,
					           -scene->getVehicles().at(indexAttach)->getHeadingAngle());
					targetOffsetRotation = MathUtils::multiply(this->rotation_, MathUtils::matrixToQuat(m));
				}
			}
			break;
		case 3:
			indexAttach = utils::indexByUuid(scene->getObjects(), this->attachUuid_);
			if (indexAttach != -1) {
				if (const auto object = scene->getObjects().at(indexAttach)->getProjectObject(); object.has_value()) {
					targetAttach = object.value();
					targetPositionOffset = {
						this->position_[0] - scene->getObjects().at(indexAttach)->getPosition()[0],
						this->position_[1] - scene->getObjects().at(indexAttach)->getPosition()[1],
						this->position_[2] - scene->getObjects().at(indexAttach)->getPosition()[2]
					};
					auto q = scene->getObjects().at(indexAttach)->getRotations();
					q.Conjugate();
					MathUtils::multiply(MathUtils::quatToMatrix(q), targetPositionOffset);
					targetOffsetRotation = MathUtils::multiply(this->rotation_, q);
				}
			}
			break;
		default:
			break;
		}

		if (indexAttach == -1) {
			setObjectiveError(result, *this, NotSelected, "The attach entity for the objective is not selected.");
			co_return;
		}

		if (targetAttach == nullptr) {
			setObjectiveError(result, *this, NotExists, "The attach entity does not exist.");
			co_return;
		}
	}

	CEntity *targetFollow = nullptr;
	int indexFollow = -1;

	if (this->followType_ > 0) {
		switch (this->followType_) {
		case 1:
			indexFollow = utils::indexByUuid(scene->getActors(), this->followUuid_);
			if (indexFollow != -1) {
				if (const auto ped = scene->getActors().at(indexFollow)->getProjectPed(); ped.has_value()) {
					targetFollow = ped.value();
				}
			}
			break;
		case 2:
			indexFollow = utils::indexByUuid(scene->getVehicles(), this->followUuid_);
			if (indexFollow != -1) {
				if (const auto vehicle = scene->getVehicles().at(indexFollow)->getProjectVehicle(); vehicle.has_value())
					targetFollow = vehicle.value();
			}
			break;
		case 3:
			indexFollow = utils::indexByUuid(scene->getObjects(), this->followUuid_);
			if (indexFollow != -1) {
				if (const auto object = scene->getObjects().at(indexFollow)->getProjectObject(); object.has_value())
					targetFollow = object.value();
			}
			break;
		default:
			break;
		}

		if (indexFollow == -1) {
			setObjectiveError(result, *this, NotSelected, "The follow entity for the objective is not selected.");
			co_return;
		}

		if (targetFollow == nullptr) {
			setObjectiveError(result, *this, NotExists, "The follow entity does not exist.");
			co_return;
		}
	}

	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	CTheScripts::bDisplayHud = false;
	CHud::bScriptDontDisplayRadar = true;

	static auto task = [](CutsceneObjective *cutscene, CVector targetPositionOffset, CQuaternion targetOffsetRotation,
	                      CEntity *targetFollow, CEntity *targetAttach) -> ktwait {
		CutsceneMutexGuard guard;
		CVector startPos = TheCamera.m_mCameraMatrix.pos;
		float startUp = atan2f(TheCamera.m_vecFixedModeUpOffSet.x, TheCamera.m_vecFixedModeUpOffSet.z);

		CQuaternion startRotation = MathUtils::lookRotationQuat({
			                                                        TheCamera.m_mMatInverse.right.y,
			                                                        TheCamera.m_mMatInverse.up.y,
			                                                        TheCamera.m_mMatInverse.at.y
		                                                        }, {0.f, 0.f, 1.f});

		auto tweenPositionX = tweeny::from(0.f).to(1.f).during(static_cast<unsigned>(cutscene->getTextTime() * 1000.f)).
		                                        via(static_cast<tweeny::easing::enumerated>(cutscene->
			                                        getPositionXInterpolationType()));
		auto tweenPositionY = tweeny::from(0.f).to(1.f).during(static_cast<unsigned>(cutscene->getTextTime() * 1000.f)).
		                                        via(static_cast<tweeny::easing::enumerated>(cutscene->
			                                        getPositionYInterpolationType()));
		auto tweenPositionZ = tweeny::from(0.f).to(1.f).during(static_cast<unsigned>(cutscene->getTextTime() * 1000.f)).
		                                        via(static_cast<tweeny::easing::enumerated>(cutscene->
			                                        getPositionZInterpolationType()));
		auto tweenRotation = tweeny::from(0.f).to(1.f).during(static_cast<unsigned>(cutscene->getTextTime() * 1000.f)).
		                                       via(static_cast<tweeny::easing::enumerated>(cutscene->
			                                       getRotationInterpolationType()));

		auto lastTime = std::chrono::high_resolution_clock::now();

		if (cutscene->isStartFadeOut())
			plugin::Command<Commands::DO_FADE>(static_cast<int>(cutscene->getStartFadeOutTime() * 1000.f), 1);

		bool useEndFadeIn = false;

		CVector aimPos;
		CVector up;
		CVector endPos;

		while (true) {
			if (tweenPositionX.progress() >= 1.f) {
				break;
			}

			const unsigned delta = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - lastTime).count());
			lastTime = std::chrono::high_resolution_clock::now();
			CVector stepPosition = {tweenPositionX.peek(), tweenPositionY.peek(), tweenPositionZ.peek()};
			auto stepRotation = tweenRotation.peek();
			if (!FrontEndMenuManager.m_bMenuActive) {
				stepPosition = {tweenPositionX.step(delta), tweenPositionY.step(delta), tweenPositionZ.step(delta)};
				stepRotation = tweenRotation.step(delta);
			}
			if (static_cast<tweeny::easing::enumerated>(cutscene->getPositionXInterpolationType()) ==
				tweeny::easing::enumerated::stepped)
				stepPosition.x = 1.f;
			if (static_cast<tweeny::easing::enumerated>(cutscene->getPositionYInterpolationType()) ==
				tweeny::easing::enumerated::stepped)
				stepPosition.y = 1.f;
			if (static_cast<tweeny::easing::enumerated>(cutscene->getPositionZInterpolationType()) ==
				tweeny::easing::enumerated::stepped)
				stepPosition.z = 1.f;
			if (static_cast<tweeny::easing::enumerated>(cutscene->getRotationInterpolationType()) ==
				tweeny::easing::enumerated::stepped)
				stepRotation = 1.f;


			float endUp;
			CQuaternion endRotation;

			if (cutscene->getAttachType() > 0) {
				endPos = targetPositionOffset;
				endRotation = targetOffsetRotation;
			} else {
				endPos = {cutscene->getPosition()[0], cutscene->getPosition()[1], cutscene->getPosition()[2]};
				endRotation = cutscene->getRotation();
			}

			endUp = RAD(cutscene->getXAngle());

			if (cutscene->isInterpolation() && cutscene->getAttachType() == 0) {
				auto currentPos = MathUtils::multiply(startPos, CVector(1.f, 1.f, 1.f) - stepPosition) +
					MathUtils::multiply(endPos, stepPosition);

				auto currentUpAngle = startUp * (1.f - stepRotation) + endUp * stepRotation;
				CVector currentUp = {sinf(currentUpAngle), 0.f, cosf(currentUpAngle)};

				if (targetFollow != nullptr) {
					auto lookAt = targetFollow->GetPosition() - currentPos;
					lookAt.Normalise();
					endRotation = MathUtils::lookRotationQuat(lookAt, {0.f, 0.f, 1.f});
				}

				auto currentRotation = MathUtils::slerp(startRotation, endRotation, stepRotation, 0);

				TheCamera.SetCamPositionForFixedMode(&currentPos, &currentUp);

				const CMatrix matrix = MathUtils::quatToMatrix(currentRotation);
				aimPos = {matrix.right.z, matrix.up.z, matrix.at.z};
				aimPos += currentPos;
				TheCamera.TakeControlNoEntity(&aimPos, 2, 1);
			} else {
				const CMatrix matrix = MathUtils::quatToMatrix(endRotation);
				aimPos = {matrix.right.z, matrix.up.z, matrix.at.z};

				if (cutscene->getAttachType() > 0) {
					TheCamera.TakeControlAttachToEntity(targetFollow, targetAttach, &endPos, &aimPos, endUp, 2, 1);
				} else {
					up = {sinf(endUp), 0.f, cosf(endUp)};
					TheCamera.SetCamPositionForFixedMode(&endPos, &up);

					aimPos += endPos;

					if (targetFollow != nullptr)
						TheCamera.TakeControl(targetFollow, MODE_FIXED, 2, 1);
					else
						TheCamera.TakeControlNoEntity(&aimPos, 2, 1);
				}
			}

			if (tweenPositionX.progress() * cutscene->getTextTime() >= cutscene->getTextTime() - cutscene->
				getEndFadeInTime() && !useEndFadeIn && cutscene->isEndFadeIn()) {
				plugin::Command<Commands::DO_FADE>(static_cast<int>(cutscene->getEndFadeInTime() * 1000.f), 0);
				useEndFadeIn = true;
			}

			co_await 1;
		}

		if (cutscene->isEndCutscene()) {
			CTheScripts::bDisplayHud = true;
			CHud::bScriptDontDisplayRadar = false;
			TheCamera.RestoreWithJumpCut();
		}
	};

	if (lockPlayerControl)
		Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	if (this->async_)
		tasklist.add_task(task, this, targetPositionOffset, targetOffsetRotation, targetFollow, targetAttach);
	else
		co_await task(this, targetPositionOffset, targetOffsetRotation, targetFollow, targetAttach);
}

void CutsceneObjective::open() {
	WorldObjective::open();
	updateLocation();
}

int& CutsceneObjective::getAttachType() {
	return attachType_;
}

boost::uuids::uuid& CutsceneObjective::getAttachUuid() {
	return attachUuid_;
}

int& CutsceneObjective::getFollowType() {
	return followType_;
}

boost::uuids::uuid& CutsceneObjective::getFollowUuid() {
	return followUuid_;
}

std::array<float, 3>& CutsceneObjective::getPosition() {
	return position_;
}

CQuaternion& CutsceneObjective::getRotation() {
	return rotation_;
}

float& CutsceneObjective::getXAngle() {
	return xAngle_;
}

std::array<char, TEXT_SIZE>& CutsceneObjective::getText() {
	return text_;
}

float& CutsceneObjective::getTextTime() {
	return textTime_;
}

bool& CutsceneObjective::isInterpolation() {
	return interpolation_;
}

int& CutsceneObjective::getPositionXInterpolationType() {
	return positionXInterpolationType_;
}

int& CutsceneObjective::getPositionYInterpolationType() {
	return positionYInterpolationType_;
}

int& CutsceneObjective::getPositionZInterpolationType() {
	return positionZInterpolationType_;
}

int& CutsceneObjective::getRotationInterpolationType() {
	return rotationInterpolationType_;
}

bool& CutsceneObjective::isWideScreen() {
	return wideScreen_;
}

bool& CutsceneObjective::isAsync() {
	return async_;
}

bool& CutsceneObjective::isStartFadeOut() {
	return startFadeOut;
}

float& CutsceneObjective::getStartFadeOutTime() {
	return startFadeOutTime;
}

bool& CutsceneObjective::isEndFadeIn() {
	return endFadeIn;
}

float& CutsceneObjective::getEndFadeInTime() {
	return endFadeInTime;
}

bool& CutsceneObjective::isEndCutscene() {
	return endCutscene_;
}

std::array<char, TEXT_SIZE>& CutsceneObjective::getGameText() {
	return gameText_;
}
