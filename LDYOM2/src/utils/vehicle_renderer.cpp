#include "vehicle_renderer.h"
#include <CPools.h>

VehicleRenderer& VehicleRenderer::instance() {
	static VehicleRenderer s_instance;
	if (!s_cleanupHookInstalled) {
		s_instance.setupCleanupHook();
		s_cleanupHookInstalled = true;
	}
	return s_instance;
}

VehicleRenderer::VehicleData& VehicleRenderer::getVehicleData(CVehicle* veh) {
	auto it = m_vehicleDataPool.find(veh);
	if (it != m_vehicleDataPool.end()) {
		return *it->second;
	}
	return *(m_vehicleDataPool[veh] = std::make_unique<VehicleData>());
}

bool VehicleRenderer::vehicleExists(CVehicle* veh) const {
	return m_vehicleDataPool.find(veh) != m_vehicleDataPool.end();
}

void VehicleRenderer::removeVehicle(CVehicle* veh) { m_vehicleDataPool.erase(veh); }

void VehicleRenderer::setMaterialColor(CVehicle* veh, RpMaterial* material, RpGeometry* geometry, RwRGBA color) {
	auto& vehicleData = getVehicleData(veh);
	auto& matProps = vehicleData.materialProperties[material];
	matProps.recolor = true;
	matProps.color = color;
	matProps.geometry = geometry;
}

void VehicleRenderer::resetMaterialColor(CVehicle* veh, RpMaterial* material) {
	if (isInitialized() && vehicleExists(veh)) {
		auto& vehicleData = getVehicleData(veh);
		auto& matProps = vehicleData.materialProperties[material];
		matProps.recolor = false;
		matProps.color = {0, 0, 0, 0};
	}
}

void VehicleRenderer::processRender(CVehicle* veh) {
	if (!isInitialized() || !vehicleExists(veh)) {
		return;
	}

	auto& vehicleData = getVehicleData(veh);
	for (auto& [material, props] : vehicleData.materialProperties) {
		if (props.recolor) {
			// Save original color and flags
			props.originalColor = material->color;
			props.originalGeometryFlags = props.geometry->flags;

			// Apply custom color
			material->color = props.color;
			props.geometry->flags |= rpGEOMETRYMODULATEMATERIALCOLOR;
		}
	}
}

void VehicleRenderer::postRender(CVehicle* veh) {
	if (!isInitialized() || !vehicleExists(veh)) {
		return;
	}

	auto& vehicleData = getVehicleData(veh);
	for (auto& [material, props] : vehicleData.materialProperties) {
		if (props.recolor) {
			// Restore original color and flags
			material->color = props.originalColor;
			props.geometry->flags = props.originalGeometryFlags;
		}
	}
}

void VehicleRenderer::setupCleanupHook() {
	// Hook into vehicle destructor to clean up data
	plugin::Events::vehicleDtorEvent.after += [this](CVehicle* veh) { this->removeVehicle(veh); };
}
