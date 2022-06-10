#pragma once

#include <RenderWare.h>

class CEntity;

class VirtualScene {
private:
	int frameSize;

	RwCamera* camera_ = nullptr;
	RwFrame* frame_ = nullptr;
	RpLight* light_ = nullptr;
	RwTexture* texture_ = nullptr;
	RwRaster* raster_ = nullptr;
	RwRaster* zraster_ = nullptr;

	bool create_texture(RwRaster*& buffer, RwTexture*& texture) const;
public:
	explicit VirtualScene(const int frameSize);

	bool initialize();

	void begin_camera(RwRaster * buffer, RwRGBA bgr) const;
	void end_camera() const;

	RwTexture* renderEntity(CEntity * entity, RwRGBA bgr) const;

	RwTexture* renderVehicle(int model, RwRGBA bgr, const RwV3d & pos, const RwV3d & rotate, int primary = -1, int secondary = -1);
	RwTexture* renderPed(int model, RwRGBA bgr, const RwV3d & pos, const RwV3d & rotate);
	RwTexture* renderObject(int model, RwRGBA bgr, const RwV3d & pos, const RwV3d & rotate);
};
