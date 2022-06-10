#pragma once

//#include "pch.h"
//#include "lua_component.h"
//#include "lua_texture.h"
#include "vehicle_renderer.h"
#include "CPools.h"
#include "CVehicle.h"
#include "CClumpModelInfo.h"
#include "CVisibilityPlugins.h"
#include <vector>

using namespace std;

class components {
public:
	template <typename T>
	struct DataStruct
	{
		vector<T>* _table;
		CVehicle* _vehicle;
	};

	class VehicleMaterial
	{
	public:
		CVehicle* _vehicle;
		RpGeometry* _geometry;
		RpMaterial* _material;

		VehicleMaterial(CVehicle* veh, RpGeometry* geometry, RpMaterial* mat) :
			_vehicle(veh),
			_geometry(geometry),
			_material(mat)
		{
		}

		void setColor(CRGBA color)
		{
			VehicleRenderer::get()->setMaterialColor(_vehicle, _material, _geometry, { color.r, color.g, color.b, color.a });
		}

		CRGBA getColor() const
		{
			RwRGBA clr = _material->color;
			return CRGBA{ clr.red, clr.green, clr.blue, clr.alpha };
		}

		void resetColor()
		{
			VehicleRenderer::get()->resetMaterialColor(_vehicle, _material);
		}

		void setTexture(std::shared_ptr<lua_texture::Texture> texture)
		{
			VehicleRenderer::get()->setMaterialTexture(_vehicle, _material, texture);
		}

		void resetTexture()
		{
			VehicleRenderer::get()->resetMaterialTexture(_vehicle, _material);
		}

		uintptr_t getRawPointer() const
		{
			return reinterpret_cast<uintptr_t>(_material);
		}

		static VehicleMaterial fromMemory(uintptr_t vehicle, uintptr_t mat, uintptr_t geometry)
		{
			return VehicleMaterial{ reinterpret_cast<CVehicle*>(vehicle), reinterpret_cast<RpGeometry*>(geometry), reinterpret_cast<RpMaterial*>(mat) };
		}
	}; // VehicleMaterial

	class VehicleAtomic
	{
	private:
		CVehicle* _vehicle;
		RpAtomic* _atomic;
	public:
		VehicleAtomic(CVehicle* veh, RpAtomic* atomic) :
			_vehicle(veh),
			_atomic(atomic)
		{
		}

		void hide(bool hide)
		{
			if (hide)
				_atomic->object.object.flags &= ~rpATOMICRENDER;
			else
				_atomic->object.object.flags |= rpATOMICRENDER;
		}

		bool getAtomicFlag(unsigned int flag) const
		{
			return (RpAtomicGetVisibilityPlugin(_atomic)->m_wFlags & flag) != 0;
		}

		void setAtomicFlag(unsigned short flag, bool value)
		{
			if (value)
				CVisibilityPlugins::SetAtomicFlag(_atomic, flag);
			else
				CVisibilityPlugins::ClearAtomicFlag(_atomic, flag);
		}

		vector<VehicleMaterial> getMaterials() const
		{
			vector<VehicleMaterial> materials;
			// yup
			std::pair<DataStruct<VehicleMaterial>, RpGeometry*> data{ {&materials, _vehicle}, _atomic->geometry };
			RpGeometryForAllMaterials(_atomic->geometry, [](RpMaterial* mat, void* data) {
				auto* ds = static_cast<std::pair<DataStruct<VehicleMaterial>, RpGeometry*>*>(data);
				ds->first._table->push_back(VehicleMaterial{ ds->first._vehicle, ds->second, mat });
				return mat;
			}, &data);
			return materials;
		}

		uintptr_t getRawPointer() const
		{
			return reinterpret_cast<uintptr_t>(_atomic);
		}

		static VehicleAtomic fromMemory(uintptr_t vehicle, uintptr_t atomic)
		{
			return VehicleAtomic{ reinterpret_cast<CVehicle*>(vehicle), reinterpret_cast<RpAtomic*>(atomic) };
		}
	}; // VehicleAtomic

	class VehicleComponent
	{
	private:
		CVehicle* _vehicle;
		RwFrame* _frame;
	public:
		VehicleComponent(CVehicle* vehicle, RwFrame* component) :
			_vehicle(vehicle),
			_frame(component)
		{
		}

		std::string getName() const
		{
			// #pluginsdk
			return reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(_frame) + 0xAC);
		}

		CMatrix* getMatrix() const
		{
			return reinterpret_cast<CMatrix*>(RwFrameGetLTM(_frame));
		}

		CMatrix* getModelingMatrix() const
		{
			return reinterpret_cast<CMatrix*>(&_frame->modelling);
		}

		void setAlpha(unsigned char alpha)
		{
			RwFrameForAllObjects(_frame, reinterpret_cast<RwObjectCallBack>(CVehicle::SetComponentAtomicAlpha), reinterpret_cast<void*>(alpha));
		}

		/*sol::optional<VehicleComponent> getChild() const
		{
			if (_frame->child)
				return VehicleComponent{ _vehicle, _frame->child };
			return sol::nullopt;
		}

		sol::optional<VehicleComponent> getNextComponent() const
		{
			if (_frame->next)
				return VehicleComponent{ _vehicle, _frame->next };
			return sol::nullopt;
		}

		sol::optional<VehicleComponent> getParent() const
		{
			if (_frame->object.parent)
				return VehicleComponent{ _vehicle, static_cast<RwFrame*>(_frame->object.parent) };
			return sol::nullopt;
		}*/

		vector<VehicleAtomic> getObjects() const
		{
			vector<VehicleAtomic> objects;
			DataStruct<VehicleAtomic> data{ &objects, _vehicle };
			RwFrameForAllObjects(_frame, [](RwObject* object, void* data) -> RwObject* {
				if (object->type == rpATOMIC)
				{
					auto* ds = static_cast<DataStruct<VehicleAtomic>*>(data);
					ds->_table->push_back(VehicleAtomic{ ds->_vehicle, reinterpret_cast<RpAtomic*>(object) });
				}
				return object;
			}, &data);
			return objects;
		}

		/*sol::table getChildComponents(sol::this_state ts) const
		{
			sol::table components = sol::state_view{ ts }.create_table();
			detail::DataStruct data{ &components, _vehicle };
			RwFrameForAllChildren(_frame, &detail::storeVehicleComponent, &data);
			return components;
		}*/

		uintptr_t getRawPointer() const
		{
			return reinterpret_cast<uintptr_t>(_frame);
		}
	}; // VehicleComponent

	static RwFrame* storeVehicleComponent(RwFrame* frame, void* data)
	{
		auto ds = static_cast<DataStruct<VehicleComponent>*>(data);
		ds->_table->push_back(VehicleComponent{ ds->_vehicle, frame });
		return frame;
	}

	static RwFrame* storeAllVehicleComponents(RwFrame* frame, void* data)
	{
		storeVehicleComponent(frame, data);
		return RwFrameForAllChildren(frame, &storeAllVehicleComponents, data);
	}

	template<typename T, T cb>
	static vector<VehicleComponent> getVehicleComponents(CVehicle* vehicle)
	{
		vector<VehicleComponent> components;
		if (vehicle && vehicle->m_pRwClump)
		{
			DataStruct<VehicleComponent> data{ &components, vehicle };
			RwFrameForAllChildren(static_cast<RwFrame*>(vehicle->m_pRwClump->object.parent), cb, &data);
		}
		return components;
	}

	static inline vector<VehicleComponent> get_all_vehicle_components(CVehicle* vehicle) {
		return getVehicleComponents<decltype(&storeAllVehicleComponents), &storeAllVehicleComponents>(vehicle);
	}

	static void extractComObjMatVehicle(CVehicle* vehicle, std::function<void(int, VehicleComponent, VehicleAtomic, VehicleMaterial)> callback) {
		int i = 0;
		for (auto comp : get_all_vehicle_components(vehicle)) {
			for (auto obj : comp.getObjects()) {
				for (auto mat : obj.getMaterials()) {
					callback(i, comp, obj, mat);
					i++;
				}
			}
		}
	}

};