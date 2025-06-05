#include "objective_specific.h"
#include "scenes_manager.h"
#include <objectives_manager.h>
#include <project_player.h>

void components::ObjectiveSpecific::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ObjectiveSpecific>("ObjectiveSpecificComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ObjectiveSpecific, cast, spawnObjective,
	                 despawnObjective, onSpawned, onDespawned);
}

components::ObjectiveSpecific::ObjectiveSpecific()
	: Component(TYPE) {
	if (!ScenesManager::instance().getCurrentScene().objectives.data.empty()) {
		this->spawnObjective = to_string(ScenesManager::instance().getCurrentScene().objectives.data.back().id);
	}
}

nlohmann::json components::ObjectiveSpecific::to_json() const {
	auto j = this->Component::to_json();
	j["spawnObjective"] = spawnObjective;
	j["despawnObjective"] = despawnObjective;
	return j;
}

void components::ObjectiveSpecific::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("spawnObjective").get_to(spawnObjective);
	j.at("despawnObjective").get_to(despawnObjective);
}

void components::ObjectiveSpecific::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("spawn_objective").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	auto* constptr = &ScenesManager::instance().getCurrentScene().objectives;
	auto* ptrmut = &ScenesManager::instance().getUnsafeCurrentScene().objectives;
	auto objectives = ScenesManager::instance().getCurrentScene().objectives.data;
	auto uuid = uuids::uuid::from_string(spawnObjective);
	auto objectiveName =
		std::ranges::find_if(objectives, [this, &uuid](const Objective& obj) { return obj.id == uuid; });
	if (ImGui::BeginCombo("##spawnObjective", objectiveName != objectives.end() ? objectiveName->name.c_str() : "")) {
		for (size_t i = 0; i < objectives.size(); ++i) {
			const auto& objective = objectives[i];
			bool isSelected = (objective.id == uuid);
			if (ImGui::Selectable((objective.name + "##" + std::to_string(i)).c_str(), isSelected)) {
				spawnObjective = to_string(objective.id);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("despawn_objective").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	auto despawnUuid = uuids::uuid::from_string(despawnObjective);
	auto despawnObjectiveName =
		std::ranges::find_if(objectives, [this, &despawnUuid](const Objective& obj) { return obj.id == despawnUuid; });
	if (ImGui::BeginCombo("##despawnObjective", despawnObjectiveName != objectives.end()
	                                                ? despawnObjectiveName->name.c_str()
	                                                : tr("last_objective").c_str())) {
		if (ImGui::Selectable(tr("last_objective").c_str(), despawnObjective.empty())) {
			despawnObjective = "";
		}
		for (size_t i = 0; i < objectives.size(); ++i) {
			const auto& objective = objectives[i];
			bool isSelected = (objective.id == despawnUuid);
			if (ImGui::Selectable((objective.name + "##" + std::to_string(i)).c_str(), isSelected)) {
				despawnObjective = to_string(objective.id);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void components::ObjectiveSpecific::onStart() {
	this->Component::onStart();
	if (IS_PLAYING) {
		auto spawnObjectiveUuid = uuids::uuid::from_string(spawnObjective);
		this->spawnObjectiveConnection = std::make_optional(ProjectPlayer::instance().onObjectiveStarted.connect(
			[this, spawnObjectiveUuid](int index) {
				if (ObjectivesManager::instance().getObjective(index).id == spawnObjectiveUuid) {
					onSpawned();
				}
			},
			rocket::queued_connection));
		if (!despawnObjective.empty()) {
			auto despawnObjectiveUuid = uuids::uuid::from_string(despawnObjective);
			this->despawnObjectiveConnection =
				std::make_optional(ProjectPlayer::instance().onObjectiveCompleted.connect(
					[this, despawnObjectiveUuid](int index) {
						if (ObjectivesManager::instance().getObjective(index).id == despawnObjectiveUuid) {
							onDespawned();
						}
					},
					rocket::queued_connection));
		}
	}
}

void components::ObjectiveSpecific::onUpdate(float deltaTime) { this->Component::onUpdate(deltaTime); }

void components::ObjectiveSpecific::onReset() {
	this->Component::onReset();
	this->spawnObjectiveConnection.reset();
	this->despawnObjectiveConnection.reset();
}