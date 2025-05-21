#pragma once
#include <functional>
#include <memory>
#include <model_renderer.h>
#include <optional>
#include <string>
#include <texture.h>
#include <unordered_map>

struct Skin {
	int model;
	std::string special;
};

class PopupSkinSelector {
  private:
	static bool isOpen;
	static std::optional<ModelRenderer> m_modelRenderer;

	static bool filteringPed(int model);
	static Texture* getModelIcon(Skin skin);

  public:
	static void showPopup();
	static void renderPopup(const std::function<void(Skin)>& onSelectCallback, bool special = false);
};