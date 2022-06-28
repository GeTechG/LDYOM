#pragma once
#include <array>

#include "plugin.h"
#include "../utils/VirtualScene.h"

class ModelRenderer {
private:
	VirtualScene scene_;

	std::array<float, 3> pos{};
	std::vector<std::pair<int, RwTexture**>> listReRender_;
	std::vector<std::pair<std::string, RwTexture**>> listSpecialPedRender_;


	CVector rotateModel_;
	float zoomKoef = 0.f;
	std::array<float, 4> backgroundColor_ = {0.4f, 0.4f, 0.4f, 1};
public:
	explicit ModelRenderer(const CVector& rotateModel, int sceneSize = 128);
	explicit ModelRenderer(const CVector& rotateModel, int sceneWidth, int sceneHeight);
	void init();

	std::array<float, 3>& getPos();
	CVector& getRotateModel();
	float& getZoomKoef();


	void render(const int model, RwTexture** texture);
	void renderSpecialPed(const std::string& modelName, RwTexture** texture);
};
