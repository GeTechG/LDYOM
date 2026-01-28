#pragma once
#include <functional>

namespace components {
class Vehicle;
}

namespace manual_editing {

void editVehicleManually(components::Vehicle* vehicle, std::function<void()> onComplete);

} // namespace manual_editing
