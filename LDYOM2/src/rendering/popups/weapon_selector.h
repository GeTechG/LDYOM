#pragma once
#include <functional>
#include <memory>
#include <model_renderer.h>
#include <optional>

class PopupWeaponSelector {
  private:
	static std::optional<ModelRenderer> m_modelRenderer;

  public:
	static void showPopup(std::string_view title);
	static void renderPopup(std::string_view title, bool* open, const std::function<void(int)>& onSelectCallback);
	static Texture* getWeaponIcon(int weaponId);
};