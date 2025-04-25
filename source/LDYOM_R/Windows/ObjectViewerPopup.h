#pragma once
#include <optional>

#include "AbstractWindow.h"
#include "ModelRenderer.h"

class ObjectViewerPopup: public Windows::AbstractWindow {
private:
	RwTexture* preview_ = nullptr;

	int model_ = 0;

	static std::optional<ModelRenderer> renderer_;

	void makePreview();
public:
	~ObjectViewerPopup() override;
	void open(int model);
	void draw() override;
};
