#pragma once
#include <functional>

void InputPosition(float* position, const std::function<void()>& callback = nullptr);
void DragAngleRotation(float* angleRotation, const std::function<void()>& callback = nullptr);
