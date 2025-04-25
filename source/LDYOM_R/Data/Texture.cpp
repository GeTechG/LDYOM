#include "Texture.h"

Texture::Texture(PDIRECT3DTEXTURE9 texture, int width, int height): texture_(texture),
                                                                    width_(width),
                                                                    height(height) {}

Texture::~Texture() {
	this->texture_->Release();
}

PDIRECT3DTEXTURE9& Texture::getTexture() {
	return texture_;
}

int& Texture::getWidth() {
	return width_;
}

int& Texture::getHeight() {
	return height;
}
