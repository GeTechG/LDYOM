#include "texture.h"

void DirectX9Texture::setTexture(PDIRECT3DTEXTURE9 texture, int width, int height) {
	texture_ = std::shared_ptr<IDirect3DTexture9>(texture, [](IDirect3DTexture9* tex) {
		if (tex) {
			tex->Release();
		}
	});
	width_ = width;
	height_ = height;
}

void* DirectX9Texture::getTexture() { return texture_.get(); }

int DirectX9Texture::getWidth() { return width_; }

int DirectX9Texture::getHeight() { return height_; }

void RenderwareTexture::setTexture(RwTexture* texture, int width, int height) {
	texture_ = std::shared_ptr<RwTexture>(texture, [](RwTexture* tex) {
		if (tex) {
			RwTextureDestroy(tex);
		}
	});
	width_ = width;
	height_ = height;
}

void* RenderwareTexture::getTexture() {
	if (texture_) {
		return *reinterpret_cast<IDirect3DTexture9**>(texture_->raster + 1);
	}
	return nullptr;
}

int RenderwareTexture::getWidth() { return width_; }

int RenderwareTexture::getHeight() { return height_; }