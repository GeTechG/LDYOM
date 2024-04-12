#include "GoToSceneObjective.h"

#include <CCamera.h>
#include <CPickups.h>
#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

using namespace plugin;

GoToSceneObjective::GoToSceneObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.go_to_scene"));
	this->name += suffix;
}

void GoToSceneObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	if (ImGui::SliderInt(local.get("general.type").c_str(), &this->type_, 0, 2,
	                     local.getArray("go_to_scene_objective.types").at(this->type_).c_str())) {
		this->triggerUuid_ = boost::uuids::uuid{};
	}

	float *position = nullptr;

	switch (this->type_) {
		case 0: {
			utils::ToggleButton(local.get("general.condition").c_str(), &this->condition);

			ImGui::BeginDisabled(!this->condition);
			std::string previewVarName;
			if (!this->varUuid.empty()) {
				if (auto [result, var] = GlobalVariablesService::getInstance().get(this->varUuid); result == 0) {
					previewVarName = var.name;
				} else {
					this->varUuid.clear();
				}
			}

			IncorrectHighlight(this->varUuid.empty(), [&] {
				if (ImGui::BeginCombo(local.get("global_variables.global_variable").c_str(), previewVarName.c_str())) {
					auto &globalVariablesService = GlobalVariablesService::getInstance();
					for (int i = 0; i < globalVariablesService.getSize(); ++i) {
						auto [result, var] = globalVariablesService.get(i);
						const bool isSelected = var.uuid == this->varUuid;
						if (ImGui::Selectable(var.name.c_str(), isSelected)) {
							this->varUuid = var.uuid;
						}
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
			});

			if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
				if (this->varUuid.empty()) {
					if (auto [result, var] = GlobalVariablesService::getInstance().get(
						GlobalVariablesService::getInstance().getSize() - 1); result == 0) {
						this->varUuid = var.uuid;
					}
				}
			}


			if (this->varValue.getType() == GlobalVariableType::String ||
				this->varValue.getType() == GlobalVariableType::Bool) {
				ImGui::Dummy(ImVec2(
					ImGui::CalcItemWidth() / 2 - ImGui::CalcTextSize("==").x / 2 + ImGui::GetStyle().ItemSpacing.x / 2,
					0));
				ImGui::SameLine();
				ImGui::Text("==");
			} else {
				utils::Combo(local.get("##condition").c_str(), &this->conditionType,
				             local.getArray("general.compare_types"));
			}

			if (!this->varUuid.empty()) {
				if (auto [result, currentVar] = GlobalVariablesService::getInstance().get(this->varUuid); result == 0) {
					if (this->varValue.getType() != currentVar.value.getType()) {
						this->varValue.setType(currentVar.value.getType());
					}
					switch (this->varValue.getType()) {
						case GlobalVariableType::Float: {
							ImGui::InputFloat("##value", static_cast<float*>(this->varValue.value));
							break;
						}
						case GlobalVariableType::Int: {
							ImGui::InputInt("##value", static_cast<int*>(this->varValue.value), 0, 0);
							break;
						}
						case GlobalVariableType::String: {
							ImGui::InputText("##value", static_cast<std::string*>(this->varValue.value));
							break;
						}
						case GlobalVariableType::Bool: {
							const auto v = static_cast<bool*>(this->varValue.value);
							const auto booleanStrings = local.getArray("global_variables.bool_values");
							const std::string &label = *v ? booleanStrings[1] : booleanStrings[0];
							ImGui::Checkbox(label.c_str(), v);
							break;
						}
						default:
							break;
					}
				}
			}

			ImGui::EndDisabled();
			break;
		}
		case 1: {
			const auto &checkpoints = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
			                                                         getCheckpoints();
			const int indexCheckpoint = utils::indexByUuid(checkpoints, this->triggerUuid_);

			IncorrectHighlight(indexCheckpoint == -1, [&] {
				utils::Combo(local.get("entities.checkpoint").c_str(), &this->triggerUuid_, indexCheckpoint,
				             checkpoints.size(), [&checkpoints](const int i) {
					             return std::ref(checkpoints.at(i)->getName());
				             }, [&checkpoints](const int i) {
					             return checkpoints.at(i)->getUuid();
				             });
			});

			if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
				if (indexCheckpoint == -1) {
					if (!checkpoints.empty()) {
						this->triggerUuid_ = checkpoints.back()->getUuid();
					}
				}
			}

			if (indexCheckpoint != -1)
				position = checkpoints.at(indexCheckpoint)->getPosition();

			break;
		}
		case 2: {
			const auto &pickups = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups();
			const int indexPickup = utils::indexByUuid(pickups, this->triggerUuid_);

			IncorrectHighlight(indexPickup == -1, [&] {
				utils::Combo(local.get("entities.pickup").c_str(), &this->triggerUuid_, indexPickup, pickups.size(),
				             [&pickups](const int i) {
					             return std::ref(pickups.at(i)->getName());
				             }, [&pickups](const int i) {
					             return pickups.at(i)->getUuid();
				             });
			});

			if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
				if (indexPickup == -1) {
					if (!pickups.empty()) {
						this->triggerUuid_ = pickups.back()->getUuid();
					}
				}
			}

			if (ImGui::SliderInt(local.get("general.type_marker").c_str(), &this->blipType_, 0, 1,
			                     local.getArray("general.type_marker_enum").at(this->blipType_).c_str()))
				this->spawnEditorBlip();

			if (this->blipType_ == 0) {
				if (utils::Combo(local.get("general.color_marker").c_str(), &this->blipColor_,
				                 local.getArray("general.color_marker_enum"), 6)) {
					this->spawnEditorBlip();
				}
			} else {
				popupSpriteBlipSelector_.draw(&this->blipSprite_, [this](int i) {
					this->spawnEditorBlip();
				});
			}

			if (indexPickup != -1)
				position = pickups.at(indexPickup)->getPosition();

			break;
		}
		default:
			break;
	}

	ImGui::Separator();

	utils::ToggleButton(local.get("general.fade").c_str(), &this->fade_);
	ImGui::BeginDisabled(!this->fade_);

	ImGui::DragFloat(local.get("general.fade_in_time").c_str(), &this->fadeInTime_, 0.1f, 0.f, FLT_MAX);
	ImGui::DragFloat(local.get("general.fade_out_time").c_str(), &this->fadeOutTime_, 0.1f, 0.f, FLT_MAX);

	ImGui::EndDisabled();

	ImGui::Separator();

	const auto &scenes = ProjectsService::getInstance().getCurrentProject().getScenes();

	IncorrectHighlight(this->sceneId_ == 0, [&] {
		const auto previewValue = this->sceneId_ == 0 ? "" : scenes.at(this->sceneId_)->getName();
		if (ImGui::BeginCombo(local.get("scenes.scene").c_str(), previewValue.c_str())) {
			for (auto &pair : ProjectsService::getInstance().getCurrentProject().getScenes()) {
				if (ImGui::Selectable(pair.second->getName().c_str(), this->sceneId_ == pair.first)) {
					this->sceneId_ = pair.first;
					this->startObjective_ = 0;
				}
			}
			ImGui::EndCombo();
		}
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (this->sceneId_ == 0) {
			if (!scenes.empty()) {
				this->sceneId_ = (--scenes.end())->first;
			}
		}
	}

	if (scenes.contains(this->sceneId_)) {
		const auto &objectives = ProjectsService::getInstance().getCurrentProject().getScenes().at(this->sceneId_)->
		                                                        getObjectives();
		this->startObjective_ = std::max(std::min(this->startObjective_, static_cast<int>(objectives.size() - 1)), 0);
		const auto previewValue = objectives.empty() ? "" : objectives.at(this->startObjective_)->getName();
		if (ImGui::BeginCombo(local.get("objective.title").c_str(), previewValue.c_str())) {
			for (int i = 0; i < objectives.size(); ++i) {
				if (ImGui::Selectable(objectives.at(i)->getName().c_str(), this->startObjective_ == i)) {
					this->startObjective_ = i;
				}
			}
			ImGui::EndCombo();
		}
	}

	if (position) {
		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

		utils::controlCamera({position[0], position[1], position[2]});
	}
}

ktwait GoToSceneObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	if (this->sceneId_ == 0) {
		setObjectiveError(result, *this, NotSelected, "The scene for the objective is not selected.");
		co_return;
	}

	std::function triggerWait = []() -> ktwait { co_return; };

	this->deleteProjectBlip();
	switch (this->type_) {
		case 0: {
			if (this->condition) {
				if (this->varUuid.empty()) {
					setObjectiveError(result, *this, NotSelected, "The global variable is not selected.");
					co_return;
				}

				auto isTrue = false;
				if (auto [r, var] = GlobalVariablesService::getInstance().get(this->varUuid); r == 0) {
					const auto variable = ProjectPlayerService::getInstance().getGlobalVariablesManager().
					                                                          getGlobalVariable(varUuid);
					switch (this->varValue.getType()) {
						case GlobalVariableType::Float: {
							isTrue = utils::compare<float>(variable, this->varValue,
							                               static_cast<MathCondition>(this->conditionType));
							break;
						}
						case GlobalVariableType::Int: {
							isTrue = utils::compare<int>(variable, this->varValue,
							                             static_cast<MathCondition>(this->conditionType));
							break;
						}
						case GlobalVariableType::String: {
							isTrue = utils::compare<std::string>(variable, this->varValue, Equal);
							break;
						}
						case GlobalVariableType::Bool: {
							isTrue = utils::compare<bool>(variable, this->varValue, Equal);
							break;
						}
					}
				}

				if (!isTrue) {
					co_return;
				}
			}

			break;
		}
		case 1: {
			const auto &checkpoints = scene->getCheckpoints();
			const int indexCheckpoint = utils::indexByUuid(checkpoints, this->triggerUuid_);

			if (indexCheckpoint == -1) {
				setObjectiveError(result, *this, NotSelected, "The checkpoint for the objective is not selected.");
				co_return;
			}

			const auto &checkpoint = checkpoints.at(indexCheckpoint);

			if (this->blipColor_ != 0 || this->blipType_ == 1)
				this->projectBlip = utils::createBlip(checkpoint->getPosition(), this->blipType_, this->blipColor_,
				                                      this->blipSprite_);

			auto playerPed = static_cast<CPed*>(FindPlayerPed());

			if (!checkpoint->existProjectEntity()) {
				setObjectiveError(result, *this, NotExists, "The entity of the checkpoint does not exist.");
				co_return;
			}

			triggerWait = [&checkpoint, playerPed]() -> ktwait {
				bool isLocate = false;
				while (!isLocate) {
					auto position = checkpoint->getProjectEntityPosition();
					float radius = checkpoint->getRadius();
					bool is2d = false;
					if (checkpoint->getType() == 1 && checkpoint->getCheckpointType() <= 2) {
						is2d = true;
					}

					if (is2d)
						isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_2D>(
							playerPed, position.x, position.y, radius, radius, false);
					else
						isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(
							playerPed, position.x, position.y, position.z, radius, radius, radius, false);

					co_await 1;
				}
			};

			break;
		}
		case 2: {
			const auto &pickups = scene->getPickups();
			const int indexPickup = utils::indexByUuid(pickups, this->triggerUuid_);

			if (indexPickup == -1) {
				setObjectiveError(result, *this, NotSelected, "The pickup for the objective is not selected.");
				co_return;
			}

			const auto &pickup = pickups.at(indexPickup);

			if (!pickup->getProjectPickup().has_value()) {
				setObjectiveError(result, *this, NotExists, "The entity of the pickup does not exist.");
				co_return;
			}

			if (this->blipColor_ != 0 || this->blipType_ == 1)
				this->projectBlip = utils::createBlip(pickup->getPosition(), this->blipType_, this->blipColor_,
				                                      this->blipSprite_);

			triggerWait = [&pickup]() -> ktwait {
				while (!CPickups::IsPickUpPickedUp(pickup->getProjectPickupIndex().value())) {
					co_await 1;
				}
			};

			break;
		}
		default:
			break;
	}

	tasklist.add_task([](GoToSceneObjective *_this, const std::function<ktwait()> triggerWait) -> ktwait {
		co_await triggerWait();

		_this->deleteProjectBlip();

		if (_this->isFade()) {
			TheCamera.Fade(_this->getFadeInTime(), 0);
			co_await std::chrono::duration<float>(_this->getFadeInTime());
		}

		const auto &scenes = ProjectsService::getInstance().getCurrentProject().getScenes();
		ProjectPlayerService::getInstance().setNextScene(scenes.at(_this->getSceneId()).get());
		ProjectPlayerService::getInstance().setNextObjective(_this->getStartObjective());

		if (_this->isFade())
			TheCamera.Fade(_this->fadeOutTime_, 1);

		ProjectPlayerService::getInstance().stopCurrentScene();
	}, this, triggerWait);

	co_await std::chrono::milliseconds(100);
}

void GoToSceneObjective::spawnEditorBlip() {
	deleteEditorBlip();

	if (this->type_ == 2) {
		const auto &pickups = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups();
		const int indexPickup = utils::indexByUuid(pickups, this->triggerUuid_);
		if (indexPickup != -1) {
			if (this->blipColor_ != 0 || this->blipType_ == 1)
				this->editorBlip = utils::createBlip(pickups.at(indexPickup)->getPosition(), this->blipType_,
				                                     this->blipColor_, this->blipSprite_);
		}
	}
}

void GoToSceneObjective::deleteEditorBlip() {
	if (this->editorBlip.has_value()) {
		if (this->editorBlip.has_value())
			Command<Commands::REMOVE_BLIP>(this->editorBlip.value());
		this->editorBlip = std::nullopt;
	}
}

void GoToSceneObjective::deleteProjectBlip() {
	if (this->projectBlip.has_value()) {
		if (this->projectBlip.has_value())
			Command<Commands::REMOVE_BLIP>(this->projectBlip.value());
		this->projectBlip = std::nullopt;
	}
}

bool& GoToSceneObjective::isFade() {
	return fade_;
}

float& GoToSceneObjective::getFadeInTime() {
	return fadeInTime_;
}

float& GoToSceneObjective::getFadeOutTime() {
	return fadeOutTime_;
}

int& GoToSceneObjective::getSceneId() {
	return sceneId_;
}

int& GoToSceneObjective::getStartObjective() {
	return startObjective_;
}

int& GoToSceneObjective::getType() { return type_; }
boost::uuids::uuid& GoToSceneObjective::getTriggerUuid() { return triggerUuid_; }
int& GoToSceneObjective::getBlipColor() { return blipColor_; }
int& GoToSceneObjective::getBlipType() { return blipType_; }
int& GoToSceneObjective::getBlipSprite() { return blipSprite_; }
bool& GoToSceneObjective::isCondition() { return condition; }
int& GoToSceneObjective::getConditionType() { return conditionType; }
std::string& GoToSceneObjective::getVarUuid() { return varUuid; }
GlobalVariableView::Value& GoToSceneObjective::getVarValue() { return varValue; }
