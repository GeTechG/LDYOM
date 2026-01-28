#pragma once

#include <CVehicle.h>
#include <functional>
#include <memory>

namespace components {
class Vehicle;
}

class Entity;
class ManualEditingSession;

class VehicleManualEditing {
  private:
	static components::Vehicle* m_vehicle;
	static CVehicle* m_vehicleHandle;
	static std::function<void()> m_onCompleteCallback;
	static std::unique_ptr<ManualEditingSession> m_session;

	static void render() noexcept;

  public:
	static void openVehicleEditor(Entity* entity, components::Vehicle* vehicle,
	                              std::function<void()> onComplete) noexcept;
	static void closeVehicleEditor(bool saveChanges) noexcept;
};
