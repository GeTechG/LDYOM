#pragma once
#include <functional>
#include <memory>
#include <model_renderer.h>
#include <optional>
#include <string>
#include <texture.h>
#include <unordered_map>

class PopupVehicleSelector {
  private:
	static bool isOpen;
	static std::optional<ModelRenderer> m_modelRenderer;

	static bool filteringVehicle(int model);
	static Texture* getModelIcon(int model);

  public:
	static void showPopup();
	static void renderPopup(const std::function<void(int)>& onSelectCallback);
};