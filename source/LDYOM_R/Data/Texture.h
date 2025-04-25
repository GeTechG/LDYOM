#pragma once
#include <d3d9.h>


class Texture {
private:
	PDIRECT3DTEXTURE9 texture_;
	int width_;
	int height;
public:
	Texture(PDIRECT3DTEXTURE9 texture, int width, int height);
	~Texture();

	PDIRECT3DTEXTURE9& getTexture();
	int& getWidth();
	int& getHeight();
};
