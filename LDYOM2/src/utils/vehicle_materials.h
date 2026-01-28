#pragma once
#include "vehicle_renderer.h"
#include <CVehicle.h>
#include <CVisibilityPlugins.h>
#include <vector>

// Helper classes for working with vehicle materials
// Based on LDYOM Beta 0.8 components.h

class VehicleMaterial {
  public:
	CVehicle* vehicle;
	RpGeometry* geometry;
	RpMaterial* material;

	VehicleMaterial(CVehicle* veh, RpGeometry* geom, RpMaterial* mat)
		: vehicle(veh),
		  geometry(geom),
		  material(mat) {}

	// Set custom RGB color for this material
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
		RwRGBA color = {r, g, b, a};
		VehicleRenderer::instance().setMaterialColor(vehicle, material, geometry, color);
	}

	// Get current material color
	RwRGBA getColor() const { return material->color; }

	// Reset to original color
	void resetColor() { VehicleRenderer::instance().resetMaterialColor(vehicle, material); }

	// Get raw pointer for advanced usage
	uintptr_t getRawPointer() const { return reinterpret_cast<uintptr_t>(material); }
};

class VehicleAtomic {
  private:
	CVehicle* vehicle;
	RpAtomic* atomic;

  public:
	VehicleAtomic(CVehicle* veh, RpAtomic* atom)
		: vehicle(veh),
		  atomic(atom) {}

	// Hide/show this atomic
	void hide(bool shouldHide) {
		if (shouldHide) {
			atomic->object.object.flags &= ~rpATOMICRENDER;
		} else {
			atomic->object.object.flags |= rpATOMICRENDER;
		}
	}

	// Get atomic flag
	bool getAtomicFlag(unsigned int flag) const {
		return (RpAtomicGetVisibilityPlugin(atomic)->m_wFlags & flag) != 0;
	}

	// Set/clear atomic flag
	void setAtomicFlag(unsigned short flag, bool value) {
		if (value) {
			CVisibilityPlugins::SetAtomicFlag(atomic, flag);
		} else {
			CVisibilityPlugins::ClearAtomicFlag(atomic, flag);
		}
	}

	// Get all materials in this atomic
	std::vector<VehicleMaterial> getMaterials() const {
		std::vector<VehicleMaterial> materials;

		struct CallbackData {
			std::vector<VehicleMaterial>* materials;
			CVehicle* vehicle;
			RpGeometry* geometry;
		};

		CallbackData data{&materials, vehicle, atomic->geometry};

		RpGeometryForAllMaterials(
			atomic->geometry,
			[](RpMaterial* mat, void* data) -> RpMaterial* {
				auto* cbData = static_cast<CallbackData*>(data);
				cbData->materials->push_back(VehicleMaterial{cbData->vehicle, cbData->geometry, mat});
				return mat;
			},
			&data);

		return materials;
	}

	// Get raw pointer for advanced usage
	uintptr_t getRawPointer() const { return reinterpret_cast<uintptr_t>(atomic); }
};

class VehicleModel {
  public:
	CVehicle* vehicle;

	explicit VehicleModel(CVehicle* veh)
		: vehicle(veh) {}

	// Get all atomics in vehicle model
	std::vector<VehicleAtomic> getAtomics() const {
		std::vector<VehicleAtomic> atomics;

		struct CallbackData {
			std::vector<VehicleAtomic>* atomics;
			CVehicle* vehicle;
		};

		CallbackData data{&atomics, vehicle};

		RpClumpForAllAtomics(
			reinterpret_cast<RpClump*>(vehicle->m_pRwObject),
			[](RpAtomic* atomic, void* data) -> RpAtomic* {
				auto* cbData = static_cast<CallbackData*>(data);
				cbData->atomics->push_back(VehicleAtomic{cbData->vehicle, atomic});
				return atomic;
			},
			&data);

		return atomics;
	}

	// Get all materials in vehicle
	std::vector<VehicleMaterial> getAllMaterials() const {
		std::vector<VehicleMaterial> allMaterials;
		auto atomics = getAtomics();
		for (const auto& atomic : atomics) {
			auto materials = atomic.getMaterials();
			allMaterials.insert(allMaterials.end(), materials.begin(), materials.end());
		}
		return allMaterials;
	}
};
