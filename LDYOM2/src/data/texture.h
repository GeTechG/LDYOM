#pragma once
#include <d3d9.h>
#include <memory>
#include <rwcore.h>

class Texture {
  public:
	virtual void* getTexture() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
};

class DirectX9Texture : public Texture {
  private:
	std::shared_ptr<IDirect3DTexture9> texture_;
	int width_;
	int height_;

  public:
	void setTexture(PDIRECT3DTEXTURE9 texture, int width, int height);
	virtual void* getTexture() override;
	virtual int getWidth() override;
	virtual int getHeight() override;
};

class RenderwareTexture : public Texture {
  private:
	std::shared_ptr<RwTexture> texture_;
	int width_;
	int height_;

  public:
	void setTexture(RwTexture* texture, int width, int height);
	virtual void* getTexture() override;
	virtual int getWidth() override;
	virtual int getHeight() override;
};