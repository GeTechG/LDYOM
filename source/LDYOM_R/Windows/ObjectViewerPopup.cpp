#include "ObjectViewerPopup.h"

#include "imgui.h"
#include "utils.h"

std::optional<ModelRenderer> ObjectViewerPopup::renderer_{};

void ObjectViewerPopup::makePreview() {
	if (!renderer_.has_value()) {
		renderer_ = ModelRenderer(CVector(-10.0f, 0, -135.0f), static_cast<int>(ImGui::GetFontSize() * 16.f));
		renderer_.value().init();
	}
	renderer_.value().render(this->model_, &this->preview_);
}

ObjectViewerPopup::~ObjectViewerPopup() {
	if (this->preview_) {
		RwTextureDestroy(this->preview_);
		this->preview_ = nullptr;
	}
}

void ObjectViewerPopup::open(int model) {
	this->model_ = model;
	makePreview();
	this->isShow() = true;
	ImGui::OpenPopup("objectView");
}

void ObjectViewerPopup::draw() {
	if (ImGui::BeginPopupModal("objectView", &this->isShow())) {
		if (!this->preview_) {
			this->makePreview();
		}

		if (ImGui::DragFloat3("pos", renderer_.value().getPos().data(), 0.1f, -100.f, 100.f))
			makePreview();

		if (ImGui::DragFloat("rotx", &renderer_.value().getRotateModel().x, 0.1f, -100.f, 100.f))
			makePreview();
		if (ImGui::DragFloat("roty", &renderer_.value().getRotateModel().y, 0.1f, -100.f, 100.f))
			makePreview();
		if (ImGui::DragFloat("rotz", &renderer_.value().getRotateModel().z, 0.1f, -100.f, 100.f))
			makePreview();

		if (this->preview_) {
			IDirect3DTexture9* texture = *reinterpret_cast<IDirect3DTexture9**>(this->preview_->raster + 1);
			const float size = ImGui::GetFontSize() * 16.f;
			ImGui::Image(texture, ImVec2(size, size));
		}

		ImGui::EndPopup();
	}
}
