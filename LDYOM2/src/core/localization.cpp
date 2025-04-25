#include "localization.h"
#include "configuration.h"
#include <filesystem>
#include <fstream>
#include <logger.h>
#include <paths.h>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

const std::string Localization::LOCALES_DIR_PATH = "languages";

Localization& Localization::instance() {
	static Localization instance;
	return instance;
}

Localization::Localization()
	: m_i18n(std::make_unique<i18n::I18N>()) {}

Localization::~Localization() { this->shutdown(); }

void Localization::handleFileChange(const wtr::event& event) {
	switch (event.effect_type) {
	case wtr::event::effect_type::modify: {
		if (event.path_name.extension() == ".json") {
			std::string localeCode = event.path_name.stem().string();
			if (this->m_i18n.get()->getLocale() == localeCode) {
				LDYOM_INFO("Locale file modified: {}", event.path_name.string());
				this->loadLocale(localeCode);
			}
		}
		break;
	}
	case wtr::event::effect_type::create: {
		if (event.path_name.extension() == ".json") {
			std::string localeCode = event.path_name.stem().string();
			if (!isLocaleAvailable(localeCode)) {
				m_availableLocales.push_back(localeCode);
				LDYOM_INFO("New locale added: {}", localeCode);
			}
		}
		break;
	}
	case wtr::event::effect_type::destroy: {
		if (event.path_name.extension() == ".json") {
			std::string localeCode = event.path_name.stem().string();
			auto it = std::find(m_availableLocales.begin(), m_availableLocales.end(), localeCode);
			if (it != m_availableLocales.end()) {
				m_availableLocales.erase(it);
				LDYOM_INFO("Locale removed: {}", localeCode);
			}
		}
		break;
	}
	default:
		break;
	}
}

void Localization::initialize() {
	std::filesystem::path localesDir = LDYOM_PATH(LOCALES_DIR_PATH);
	if (!exists(localesDir)) {
		create_directories(localesDir);
		LDYOM_INFO("Created locales directory: {}", localesDir.string());
	}

	loadAvailableLocales();

	this->loadLocale("en");
	this->m_i18n.get()->setFallbackLocale("en");

	const auto initialLocale = Configuration::instance().getSetting<std::string>("lang", "en");
	this->loadLocale(initialLocale);
	this->m_i18n.get()->setLocale(initialLocale);

	m_watcher = std::make_unique<wtr::watch>(localesDir, [this](const wtr::event event) { handleFileChange(event); });

	LDYOM_INFO("Localization initialized with locale: {}", initialLocale);
}

void Localization::shutdown() {
	if (m_watcher) {
		m_watcher->close();
		m_watcher.reset();
	}
	LDYOM_INFO("Localization shutdown");
}

void Localization::setLocale(const std::string& locale) {
	Configuration::instance().setSetting<std::string>("lang", locale);
	this->loadLocale(locale);
	this->m_i18n->setLocale(locale);
}

std::string Localization::getCurrentLocale() const { return m_i18n->getLocale(); }

const std::vector<std::string>& Localization::getAvailableLocales() const { return m_availableLocales; }

bool Localization::isLocaleAvailable(std::string_view locale) const {
	return std::find(m_availableLocales.begin(), m_availableLocales.end(), std::string(locale)) !=
	       m_availableLocales.end();
}

void Localization::loadAvailableLocales() {
	m_availableLocales.clear();

	try {
		// Iterate through JSON files in the locales directory
		fs::path localesDir = LDYOM_PATH(LOCALES_DIR_PATH);
		for (const auto& entry : fs::directory_iterator(localesDir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				std::string localeCode = entry.path().stem().string();
				m_availableLocales.push_back(localeCode);
			}
		}
	} catch (const fs::filesystem_error& e) {
		LDYOM_ERROR("Error loading available locales: {}", e.what());
	} catch (const std::exception& e) {
		LDYOM_ERROR("Unexpected error loading available locales: {}", e.what());
	} catch (...) {
		LDYOM_ERROR("Unknown error occurred while loading available locales");
	}

	LDYOM_INFO("Available locales loaded: {}", m_availableLocales.size());
}

void Localization::loadLocale(std::string_view locale) {
	if (isLocaleAvailable(locale)) {
		fs::path localeFilePath(LDYOM_PATH(LOCALES_DIR_PATH) + "/" + std::string(locale) + ".json");
		try {
			m_i18n->loadLocaleFromFile(localeFilePath.string());
			LDYOM_INFO("Locale '{}' loaded successfully", localeFilePath.string());
		} catch (const std::exception& e) {
			LDYOM_ERROR("Failed to load locale '{}': {}", localeFilePath.string(), e.what());
		}
	}
}