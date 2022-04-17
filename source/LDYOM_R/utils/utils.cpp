#include "utils.h"
#include <vector>
#include "imgui.h"
#include "plugin.h"
#include "d3dx9tex.h"

bool utils::Combo(const char* label, int* currentItem, const std::vector<std::string>& items)
{
    bool action = false;
    const unsigned currentItemClamped = std::max(0u, std::min(static_cast<unsigned>(*currentItem), items.size()));
    const std::string previewText = items.empty()? "" : items.at(currentItemClamped);
    if (ImGui::BeginCombo(label, previewText.c_str()))
    {
        for (unsigned i = 0; i < items.size(); ++i)
        {
            bool isSelected = i == currentItemClamped;
            if (ImGui::Selectable(items.at(i).c_str(), &isSelected))
            {
                *currentItem = static_cast<int>(i);
                action = true;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return action;
}

bool utils::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height) {
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    const HRESULT hr = D3DXCreateTextureFromFileEx(GetD3DDevice(), filename, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &texture);
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = static_cast<int>(my_image_desc.Width);
    *out_height = static_cast<int>(my_image_desc.Height);
    return true;
}

std::optional<Texture> utils::LoadTextureRequiredFromFile(const char* filename) {
    int imageWidth = 0;
    int imageHeight = 0;
    PDIRECT3DTEXTURE9 texture = nullptr;
    if (LoadTextureFromFile(filename, &texture, &imageWidth, &imageHeight)) {
        return Texture(texture, imageWidth, imageHeight);
    }
    return std::nullopt;
}
