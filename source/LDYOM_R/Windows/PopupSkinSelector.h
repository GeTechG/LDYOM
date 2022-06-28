#pragma once
#include <utility>

#include "functional"
#include "imgui.h"
#include "ModelRenderer.h"
#include "optional"
#include "../Data/Texture.h"


class PopupSkinSelector {
private:
	std::unordered_map<int, std::unique_ptr<Texture>> pedsIcons_;
	std::unordered_map<int, std::unique_ptr<Texture>> specialPedIcons_;
	std::unordered_map<int, RwTexture*> unknownSkins_;
	std::unique_ptr<Texture> unknownIcon_;

	bool isOpen_ = false;

	static std::optional<ModelRenderer> renderer_;

	bool filteringPed(int model) const;

	PopupSkinSelector () = default;
	PopupSkinSelector (const PopupSkinSelector & root) = delete;
	PopupSkinSelector & operator=(const PopupSkinSelector &) = delete;

	void clearUnknownSkins();
	std::pair<IDirect3DTexture9*, ImVec2> getModelIcon(int modelId, bool special);
public:
	static PopupSkinSelector & getInstance() {
		static PopupSkinSelector  instance;
		return instance;
	}

	void Init();

	void showPopup();
	void renderPopup(const std::function<void(int)>& onSelectCallback, bool special = false, int slot = 0);
};

