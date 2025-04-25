#pragma once
#define NOMINMAX
#include "i18ncpp.h"
#include "wtr/watcher.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class Localization {
  public:
	/**
	 * @brief Get the singleton instance of the Localization class
	 * @return Reference to the Localization instance
	 */
	static Localization& instance();

	/**
	 * @brief Initialize the localization system
	 * @param localesDir Directory containing locale files
	 * @param defaultLocale Default locale to use (e.g., "en-US")
	 * @return true if initialization was successful, false otherwise
	 */
	void initialize();

	void shutdown();

	/**
	 * @brief Set the active locale
	 * @param locale Locale code (e.g., "en-US", "ru-RU")
	 */
	void setLocale(const std::string& locale);

	/**
	 * @brief Get the current active locale
	 * @return Current locale code
	 */
	std::string getCurrentLocale() const;

	/**
	 * @brief Get list of available locales
	 * @return Vector of available locale codes
	 */
	const std::vector<std::string>& getAvailableLocales() const;

	/**
	 * @brief Check if a locale is available
	 * @param locale Locale code to check
	 * @return true if locale is available, false otherwise
	 */
	bool isLocaleAvailable(std::string_view locale) const;

	i18n::I18N& getI18N() { return *m_i18n; }

  private:
	std::unique_ptr<wtr::watch> m_watcher;

	static const std::string LOCALES_DIR_PATH;

	// Private constructor to enforce singleton pattern
	Localization();
	~Localization();

	// Prevent copying and moving
	Localization(const Localization&) = delete;
	Localization& operator=(const Localization&) = delete;
	Localization(Localization&&) = delete;
	Localization& operator=(Localization&&) = delete;

	void loadAvailableLocales();
	void loadLocale(std::string_view locale);
	void handleFileChange(const wtr::event& event);

	std::unique_ptr<i18n::I18N> m_i18n;
	std::vector<std::string> m_availableLocales;
};

// Convenience macro for translation
#define _(key, ...) ::Localization::instance().getI18N().trv(key, ##__VA_ARGS__)

// Convenience macro for plural translation
#define _p(key, count, ...) ::Localization::instance().getI18N().trPluralv(key, count, ##__VA_ARGS__)