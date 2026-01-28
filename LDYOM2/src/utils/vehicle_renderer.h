#pragma once
#include <CVehicle.h>
#include <memory>
#include <plugin.h>
#include <unordered_map>

// Vehicle custom color rendering system
// Based on LDYOM Beta 0.8 VehicleRenderer
// Allows setting custom RGB colors and textures for individual vehicle materials

class VehicleRenderer {
  public:
	struct MaterialProperties {
		MaterialProperties()
			: color{0, 0, 0, 0},
			  recolor(false),
			  geometry(nullptr),
			  originalColor{0, 0, 0, 0},
			  originalGeometryFlags(0) {}

		RwRGBA color;
		bool recolor;
		RpGeometry* geometry;
		RwRGBA originalColor;
		RwInt32 originalGeometryFlags;
	};

	struct VehicleData {
		VehicleData() = default;
		std::unordered_map<RpMaterial*, MaterialProperties> materialProperties;
	};

	static VehicleRenderer& instance();

	void setMaterialColor(CVehicle* veh, RpMaterial* material, RpGeometry* geometry, RwRGBA color);
	void resetMaterialColor(CVehicle* veh, RpMaterial* material);
	void processRender(CVehicle* veh);
	void postRender(CVehicle* veh);
	bool isInitialized() const { return m_initialized; }

  private:
	VehicleRenderer() = default;
	~VehicleRenderer() = default;

	VehicleData& getVehicleData(CVehicle* veh);
	bool vehicleExists(CVehicle* veh) const;
	void removeVehicle(CVehicle* veh);

	bool m_initialized = true;
	std::unordered_map<const CVehicle*, std::unique_ptr<VehicleData>> m_vehicleDataPool;

	// Cleanup hook
	void setupCleanupHook();
	inline static bool s_cleanupHookInstalled = false;
};
