#include "textures_manager.h"
#include <common.h>
#include <dxsdk/d3dx9tex.h>
#include <filesystem>
#include <fmt/format.h>
#include <paths.h>

TexturesManager& TexturesManager::instance() {
	static TexturesManager instance;
	return instance;
}

void TexturesManager::initialize() {
	const std::string pedModelsIconsDirectory = LDYOM_PATH("icons/ped_models");
	const std::string specialModelsIconsDirectory = LDYOM_PATH("icons/special_models");
	const std::string weaponIconsDirectory = LDYOM_PATH("icons/weapons");

	for (const auto& entry : std::filesystem::directory_iterator(pedModelsIconsDirectory)) {
		if (entry.is_regular_file() && entry.path().extension() == ".jpg") {
			const std::string fileName = entry.path().filename().string();
			const std::string textureName = fileName.substr(0, fileName.find_last_of('.'));
			loadTexture(entry.path().string(), textureName);
		}
	}

	for (const auto& entry : std::filesystem::directory_iterator(specialModelsIconsDirectory)) {
		if (entry.is_regular_file() && entry.path().extension() == ".jpg") {
			const std::string fileName = entry.path().filename().string();
			const std::string textureName = fileName.substr(0, fileName.find_last_of('.'));
			loadTexture(entry.path().string(), fmt::format("special_{}", textureName));
		}
	}

	for (const auto& entry : std::filesystem::directory_iterator(weaponIconsDirectory)) {
		if (entry.is_regular_file() && entry.path().extension() == ".png") {
			const std::string fileName = entry.path().filename().string();
			const std::string textureName = fileName.substr(0, fileName.find_last_of('.'));
			loadTexture(entry.path().string(), textureName);
		}
	}
}

void TexturesManager::shutdown() { m_textures.clear(); }

bool TexturesManager::loadTexture(const std::string_view path, const std::string_view name) {
	PDIRECT3DTEXTURE9 texture;
	std::string path_str = std::string(path);
	const HRESULT hr = D3DXCreateTextureFromFileEx(
		(IDirect3DDevice9*)GetD3DDevice(), path_str.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &texture);
	if (hr != S_OK)
		return false;

	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	auto dx9texture = std::make_unique<DirectX9Texture>();
	dx9texture->setTexture(texture, my_image_desc.Width, my_image_desc.Height);
	m_textures.emplace(name, std::move(dx9texture));
	return true;
}

bool TexturesManager::addTexture(const std::string_view name, std::unique_ptr<Texture> texture) {
	if (!texture)
		return false;
	m_textures.emplace(name, std::move(texture));
	return true;
}

bool TexturesManager::hasTexture(const std::string_view name) {
	return m_textures.find(std::string(name)) != m_textures.end();
}

std::optional<Texture*> TexturesManager::getTexture(const std::string_view name) {
	auto it = m_textures.find(std::string(name));
	if (it != m_textures.end()) {
		return it->second.get();
	}
	return std::nullopt;
}
