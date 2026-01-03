#include "object_move_by_path.h"
#include "components/object.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <CVector.h>
#include <angles.h>
#include <cmath>
#include <common.h>
#include <components/object.h>
#include <cquat_json.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <in_game/object_paths_editing.h>
#include <matrix_utils.h>
#include <project_player.h>
#include <window_manager.h>

void components::ObjectMoveByPath::sol_lua_register(sol::state_view lua_state) {
	auto pathPointType = lua_state.new_usertype<PathPoint>("PathPoint", sol::constructors<PathPoint()>());
	pathPointType["position"] = &PathPoint::position;
	pathPointType["rotation"] = &PathPoint::rotation;

	auto ut = lua_state.new_usertype<ObjectMoveByPath>("ObjectMoveByPathComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ObjectMoveByPath, cast, points, speedType, loop,
	                 autoStart, start, stop, reset);
}

components::ObjectMoveByPath::ObjectMoveByPath()
	: Component(TYPE) {}

void components::ObjectMoveByPath::start() {
	this->currentPointIndex = 0;
	this->currentProgress = 0.f;
	this->isMoving = !points.empty(); // Only move if we have points

	// Save initial object position as point 0
	initialPoint.position = this->entity->position;
	initialPoint.rotation = this->entity->rotation;

	// Set start position
	this->currentPosition = initialPoint.position;
	this->currentRotation = initialPoint.rotation;

	// Update segment points
	if (!points.empty()) {
		startPoint = initialPoint;
		endPoint = points[0];
	}
}

void components::ObjectMoveByPath::stop() {
	this->isMoving = false;
}

void components::ObjectMoveByPath::reset() {
	this->currentPointIndex = 0;
	this->currentProgress = 0.f;
	this->isMoving = false;

	// Reset to initial position
	this->currentPosition = initialPoint.position;
	this->currentRotation = initialPoint.rotation;

	// Update object position
	auto object = Object::cast(this->entity->getComponent(Object::TYPE));
	if (object && object->handle) {
		object->handle->SetPosn(currentPosition[0], currentPosition[1], currentPosition[2]);
		object->handle->m_matrix->SetRotate(currentRotation);
		scaleMatrix(*object->handle->m_matrix, this->entity->scale);
		object->handle->m_matrix->UpdateRW();
		object->handle->UpdateRwMatrix();
		object->handle->UpdateRwFrame();
	}
}

nlohmann::json components::ObjectMoveByPath::to_json() const {
	auto j = this->Component::to_json();
	j["points"] = points;
	j["speedType"] = speedType;
	j["loop"] = loop;
	j["autoStart"] = autoStart;
	return j;
}

void components::ObjectMoveByPath::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("points").get_to(points);
	j.at("speedType").get_to(speedType);
	j.at("loop").get_to(loop);
	if (j.contains("autoStart")) {
		j.at("autoStart").get_to(autoStart);
	}
}

void components::ObjectMoveByPath::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Speed selection with three preset options like in DYOM
	ImGui::Text(tr("speed").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);

	if (ImGui::BeginCombo("##speed", tr(fmt::format("speed_types.{}", speedType).c_str()).c_str())) {
		for (int i = 0; i < 3; i++) {
			const bool isSelected = (speedType == i);
			if (ImGui::Selectable(tr(fmt::format("speed_types.{}", i).c_str()).c_str(), isSelected)) {
				speedType = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("loop").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##loop", &loop);

	ImGui::Text(tr("auto_start").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##autoStart", &autoStart);

	ImGui::Text(tr("points_count").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	// Total points = initial position + user-defined points
	ImGui::Text("%zu", points.empty() ? 0 : points.size() + 1);

	if (ImGui::Button(tr("edit_path").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		auto object = Object::cast(this->entity->getComponent(Object::TYPE));
		CObject* objectHandle = object && object->handle ? object->handle.get() : nullptr;
		ObjectPathsEditing::openPathEditor(
			objectHandle, points, [this, object](bool saveChanges, const std::vector<PathPoint>& newPoints) {
				if (saveChanges) {
					this->points = newPoints;
				}
				object->dirty = Object::DirtyFlags::Position | Object::DirtyFlags::Rotation;
				WindowManager::instance().disableWindowRendering(false);
			});
	}
}

void components::ObjectMoveByPath::onStart() {
	Component::onStart();
	if (IS_PLAYING && autoStart) {
		start();
	}
}

const float speedValues[] = {0.1f, 1.0f, 5.0f}; // m/s for Slow, Normal, Fast

void components::ObjectMoveByPath::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);

	if (!IS_PLAYING || !isMoving) {
		return;
	}

	auto object = Object::cast(this->entity->getComponent(Object::TYPE));
	if (!object || !object->handle) {
		return;
	}

	// Update current segment points based on currentPointIndex
	// Point 0 is initialPoint (spawn position), points 1+ are from the points array
	if (currentPointIndex == 0) {
		startPoint = initialPoint;
		endPoint = points[0];
	} else if (currentPointIndex <= static_cast<int>(points.size()) - 1) {
		startPoint = points[currentPointIndex - 1];
		endPoint = points[currentPointIndex];
	} else if (loop && currentPointIndex == static_cast<int>(points.size())) {
		// Closing segment: from last point back to initial point
		startPoint = points[points.size() - 1];
		endPoint = initialPoint;
	}

	// Calculate distance between current and next point
	const CVector startPos(startPoint.position[0], startPoint.position[1], startPoint.position[2]);
	const CVector endPos(endPoint.position[0], endPoint.position[1], endPoint.position[2]);
	const float distance = DistanceBetweenPoints(startPos, endPos);

	if (distance < 0.001f) {
		// Points are too close, skip to next
		currentPointIndex++;
		if (loop) {
			// For loop mode: after closing segment (points.size()), go back to 0
			if (currentPointIndex > static_cast<int>(points.size())) {
				currentPointIndex = 0;
			}
		} else {
			// For non-loop mode: stop at the end
			if (currentPointIndex >= static_cast<int>(points.size())) {
				isMoving = false;
				return;
			}
		}
		currentProgress = 0.f;
		return;
	}

	const float speed = speedValues[speedType];

	// Calculate movement time for this segment
	const float segmentTime = distance / speed;

	// Update progress
	currentProgress += deltaTime / segmentTime;

	if (currentProgress >= 1.0f) {
		// Move to next point
		currentPointIndex++;
		currentProgress = 0.f;

		// Check if we reached the end
		if (loop) {
			// For loop mode: after closing segment (points.size()), go back to 0
			if (currentPointIndex > static_cast<int>(points.size())) {
				currentPointIndex = 0;
			}
		} else {
			// For non-loop mode: stop at the end
			if (currentPointIndex >= static_cast<int>(points.size())) {
				// Stay at last point and stop moving
				currentPosition = endPoint.position;
				currentRotation = endPoint.rotation;
				isMoving = false; // Stop movement

				// Apply final position and return to stop movement
				object->handle->SetPosn(currentPosition[0], currentPosition[1], currentPosition[2]);
				object->handle->m_matrix->SetRotate(currentRotation);
				scaleMatrix(*object->handle->m_matrix, this->entity->scale);
				object->handle->m_matrix->UpdateRW();
				object->handle->UpdateRwMatrix();
				object->handle->UpdateRwFrame();
				return;
			}
		}
	} else {
		// Interpolate position
		const float t = currentProgress;
		currentPosition[0] = startPoint.position[0] + (endPoint.position[0] - startPoint.position[0]) * t;
		currentPosition[1] = startPoint.position[1] + (endPoint.position[1] - startPoint.position[1]) * t;
		currentPosition[2] = startPoint.position[2] + (endPoint.position[2] - startPoint.position[2]) * t;

		// Interpolate rotation using slerp
		currentRotation.Slerp(startPoint.rotation, endPoint.rotation, t);
	}

	// Apply position and rotation to object
	object->handle->SetPosn(currentPosition[0], currentPosition[1], currentPosition[2]);
	object->handle->m_matrix->SetRotate(currentRotation);
	scaleMatrix(*object->handle->m_matrix, this->entity->scale);
	object->handle->m_matrix->UpdateRW();
	object->handle->UpdateRwMatrix();
	object->handle->UpdateRwFrame();
}

void components::ObjectMoveByPath::onReset() {
	Component::onReset();
	this->currentPointIndex = 0;
	this->currentProgress = 0.f;
	this->isMoving = false;
}

Dependencies components::ObjectMoveByPath::getDependencies() { return Dependencies{{Object::TYPE}, true}; }
