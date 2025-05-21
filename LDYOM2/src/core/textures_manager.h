#pragma once
#include <memory>
#include <optional>
#include <string>
#include <texture.h>
#include <unordered_map>

class TexturesManager {
  private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

	TexturesManager() = default;

  public:
	TexturesManager(const TexturesManager&) = delete;
	void operator=(const TexturesManager&) = delete;

	static TexturesManager& instance();

	void initialize();
	void shutdown();

	bool loadTexture(const std::string_view path, const std::string_view name);
	bool addTexture(const std::string_view name, std::unique_ptr<Texture> texture);
	bool hasTexture(const std::string_view name);
	std::optional<Texture*> getTexture(const std::string_view name);
};
