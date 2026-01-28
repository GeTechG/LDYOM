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
	this->initialPointPassed = false;

	// Save initial object position as point 0
	initialPoint.position = this->entity->position;
	initialPoint.rotation = this->entity->rotation;

	// Set start position
	this->currentPosition = initialPoint.position;
	this->currentRotation = initialPoint.rotation;

	// Update segment points - start from initial point to first path point
	if (!points.empty()) {
		startPoint = initialPoint;
		endPoint = points[0];
	}
}

void components::ObjectMoveByPath::stop() { this->isMoving = false; }

void components::ObjectMoveByPath::reset() {
	this->currentPointIndex = 0;
	this->currentProgress = 0.f;
	this->isMoving = false;
	this->initialPointPassed = false;

	// Reset to initial position
	this->currentPosition = initialPoint.position;
	this->currentRotation = initialPoint.rotation;

	// Update object position following CObject::Teleport pattern
	auto object = Object::cast(this->entity->getComponent(Object::TYPE));
	if (object && object->handle) {
		object->handle->Remove();

		CMatrixLink* matrix = object->handle->GetMatrix();
		if (matrix) {
			matrix->GetPosition().x = currentPosition[0];
			matrix->GetPosition().y = currentPosition[1];
			matrix->GetPosition().z = currentPosition[2];
			matrix->SetRotate(currentRotation);
			scaleMatrix(*matrix, this->entity->scale);
			matrix->UpdateRW();
		} else {
			object->handle->m_placement.m_vPosn.x = currentPosition[0];
			object->handle->m_placement.m_vPosn.y = currentPosition[1];
			object->handle->m_placement.m_vPosn.z = currentPosition[2];
		}

		if (object->handle->m_pRwObject) {
			object->handle->UpdateRwMatrix();
		}
		object->handle->UpdateRwFrame();
		object->handle->Add();
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
	ImGui::Text("%zu", points.empty() ? 0 : points.size());

	if (ImGui::Button(tr("edit_path").c_str())) {
		auto object = Object::cast(this->entity->getComponent(Object::TYPE));
		CObject* objectHandle = object && object->handle ? object->handle.get() : nullptr;
		ObjectPathsEditing::openPathEditor(
			this->entity, objectHandle, points, [this, object](bool saveChanges, const std::vector<PathPoint>& newPoints) {
				if (saveChanges) {
					this->points = newPoints;
				}
				object->dirty = Object::DirtyFlags::Position | Object::DirtyFlags::Rotation;
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

	// Update current segment points based on initialPointPassed flag
	if (!initialPointPassed) {
		// First segment: from initial position to first path point
		startPoint = initialPoint;
		endPoint = points[0];
	} else {
		// Moving through path points
		startPoint = points[currentPointIndex];

		// Determine next point based on loop mode
		if (loop) {
			const size_t nextIndex = (currentPointIndex + 1) % points.size();
			endPoint = points[nextIndex];
		} else {
			// Non-loop: check if there's a next point
			if (currentPointIndex + 1 < points.size()) {
				endPoint = points[currentPointIndex + 1];
			} else {
				// We're at the last point, stay there
				endPoint = points[currentPointIndex];
			}
		}
	}

	// Calculate distance between current and next point
	const CVector startPos(startPoint.position[0], startPoint.position[1], startPoint.position[2]);
	const CVector endPos(endPoint.position[0], endPoint.position[1], endPoint.position[2]);
	const float distance = DistanceBetweenPoints(startPos, endPos);

	if (distance < 0.001f) {
		// Points are too close, skip to next
		if (!initialPointPassed) {
			// Finished initial segment, now loop only through path points
			initialPointPassed = true;
			currentPointIndex = 0;
			// If only 1 point and no loop, stop
			if (points.size() == 1 && !loop) {
				isMoving = false;
				return;
			}
		} else {
			// Moving through path points
			if (points.size() == 1) {
				// Only 1 point - stay in place if looping
				if (!loop) {
					isMoving = false;
				}
				return;
			}

			// Check if we're at the last point
			if (!loop && currentPointIndex == static_cast<int>(points.size()) - 1) {
				// We've reached the last point, stop
				isMoving = false;
				return;
			}

			// Move to next point
			currentPointIndex++;
			if (currentPointIndex >= static_cast<int>(points.size())) {
				// Loop back to start
				currentPointIndex = 0;
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
		currentProgress = 0.f;

		if (!initialPointPassed) {
			// Finished initial segment, now loop only through path points
			initialPointPassed = true;
			currentPointIndex = 0;
			// If only 1 point and no loop, stop at that point
			if (points.size() == 1 && !loop) {
				currentPosition = endPoint.position;
				currentRotation = endPoint.rotation;
				isMoving = false;

				// Proper matrix update following CObject::Teleport pattern
				object->handle->Remove();

				CMatrixLink* matrix = object->handle->GetMatrix();
				if (matrix) {
					matrix->GetPosition().x = currentPosition[0];
					matrix->GetPosition().y = currentPosition[1];
					matrix->GetPosition().z = currentPosition[2];
					matrix->SetRotate(currentRotation);
					scaleMatrix(*matrix, this->entity->scale);
					matrix->UpdateRW();
				} else {
					object->handle->m_placement.m_vPosn.x = currentPosition[0];
					object->handle->m_placement.m_vPosn.y = currentPosition[1];
					object->handle->m_placement.m_vPosn.z = currentPosition[2];
				}

				if (object->handle->m_pRwObject) {
					object->handle->UpdateRwMatrix();
				}
				object->handle->UpdateRwFrame();
				object->handle->Add();
				return;
			}
		} else {
			// Moving through path points
			if (points.size() == 1) {
				// Only 1 point - stay in place if looping
				if (!loop) {
					isMoving = false;
				}
				return;
			}

			// Check if we're at the last point
			if (!loop && currentPointIndex == static_cast<int>(points.size()) - 1) {
				// We just finished the last segment, stop here
				currentPosition = endPoint.position;
				currentRotation = endPoint.rotation;
				isMoving = false;

				// Proper matrix update following CObject::Teleport pattern
				object->handle->Remove();

				CMatrixLink* matrix = object->handle->GetMatrix();
				if (matrix) {
					matrix->GetPosition().x = currentPosition[0];
					matrix->GetPosition().y = currentPosition[1];
					matrix->GetPosition().z = currentPosition[2];
					matrix->SetRotate(currentRotation);
					scaleMatrix(*matrix, this->entity->scale);
					matrix->UpdateRW();
				} else {
					object->handle->m_placement.m_vPosn.x = currentPosition[0];
					object->handle->m_placement.m_vPosn.y = currentPosition[1];
					object->handle->m_placement.m_vPosn.z = currentPosition[2];
				}

				if (object->handle->m_pRwObject) {
					object->handle->UpdateRwMatrix();
				}
				object->handle->UpdateRwFrame();
				object->handle->Add();
				return;
			}

			// Move to next point
			currentPointIndex++;
			if (currentPointIndex >= static_cast<int>(points.size())) {
				// Loop back to start
				currentPointIndex = 0;
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

	// Apply position and rotation to object following CObject::Teleport pattern
	object->handle->Remove();

	CMatrixLink* matrix = object->handle->GetMatrix();
	if (matrix) {
		matrix->GetPosition().x = currentPosition[0];
		matrix->GetPosition().y = currentPosition[1];
		matrix->GetPosition().z = currentPosition[2];
		matrix->SetRotate(currentRotation);
		scaleMatrix(*matrix, this->entity->scale);
		matrix->UpdateRW();
	} else {
		object->handle->m_placement.m_vPosn.x = currentPosition[0];
		object->handle->m_placement.m_vPosn.y = currentPosition[1];
		object->handle->m_placement.m_vPosn.z = currentPosition[2];
	}

	if (object->handle->m_pRwObject) {
		object->handle->UpdateRwMatrix();
	}
	object->handle->UpdateRwFrame();
	object->handle->Add();
}

void components::ObjectMoveByPath::onReset() {
	Component::onReset();
	this->currentPointIndex = 0;
	this->currentProgress = 0.f;
	this->isMoving = false;
	this->initialPointPassed = false;
}

Dependencies components::ObjectMoveByPath::getDependencies() { return Dependencies{{Object::TYPE}, true}; }
