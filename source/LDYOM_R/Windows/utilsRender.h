#pragma once
#include <functional>

class CQuaternion;
class ObjectiveDependent;
void InputPosition(float* position, const std::function<void()>& callback = nullptr);
void DragPosition(float* position, const std::function<void()>& callback = nullptr);
void DragAngleRotation(float* angleRotation, const std::function<void()>& callback = nullptr);
void ObjectiveDependentInput(ObjectiveDependent* objectiveDependent);
void DragRotations(float* rotations, const std::function<void()>& callback);
void InputRotations(float* rotations, const std::function<void()>& callback);
bool TransformEditor(float* translate, CQuaternion& rotates, float* scale);
