#pragma once
#include "data/texture.h"
#include "plugin.h"
#include "virtual_scene.h"
#include <array>

class ModelRenderer {
  private:
	VirtualScene m_scene;

	std::array<float, 3> pos{};
	std::vector<std::pair<int, RenderwareTexture*>> listReRender_;
	std::vector<std::pair<std::string, RenderwareTexture*>> listSpecialPedRender_;

	CVector m_rotateModel;
	float zoomKoef = 0.f;
	std::array<float, 4> backgroundColor_ = {0.4f, 0.4f, 0.4f, 1};

  public:
	explicit ModelRenderer(const CVector& rotateModel, int sceneSize = 128);
	explicit ModelRenderer(const CVector& rotateModel, int sceneWidth, int sceneHeight);
	void init();

	std::array<float, 3>& getPos();
	CVector& getRotateModel();
	float& getZoomKoef();

	void render(const int model, RenderwareTexture* texture);
	void renderSpecialPed(const std::string& modelName, RenderwareTexture* texture);
};