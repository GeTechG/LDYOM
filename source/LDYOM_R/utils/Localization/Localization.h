﻿#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "toml++/toml.h"

class Localization
{
private:
	std::unordered_map<std::string, std::string> textLocalizations_;
	std::unordered_map<std::string, std::vector<std::string>> arrayLocalization_;
	std::vector<std::string> scriptsLocalizationDirectories;

	Localization() = default;
	Localization(const Localization&) = delete;
	Localization& operator=(Localization&) = delete;

	void unpackLocalization(std::string path, toml::table& table);
public:
	static Localization& getInstance() {
		static Localization instance;
		return instance;
	}
	void Init();

	void Update() {
		Init();
	}

	void loadLocalization(const std::string& localizationName);
	void loadLocalization(const std::string& localizationDirectory, const std::string& localizationName);
	void addScriptsLocalization(const std::string& localizationDirectory);
	void clearScriptsLocalization();
	const std::string& get(const std::string& path);
	std::vector<std::string>& getArray(const std::string& path);
};
