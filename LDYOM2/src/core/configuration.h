#pragma once

#include "logger.h"
#include "nlohmann/json.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


class Configuration {
  public:
	static Configuration& instance();

	Configuration(const Configuration&) = delete;
	void operator=(const Configuration&) = delete;

	void initialize();

	bool save();

	template <typename T> T getSetting(const std::string& key, const T& defaultValue) const;

	template <typename T> void setSetting(const std::string& key, const T& value);

  private:
	Configuration() = default;

	static const std::string CONFIG_FILE_PATH;

	// Convert dot notation to JSON pointer
	static nlohmann::json::json_pointer dotToPointer(const std::string& dotKey);

	// Configuration data
	nlohmann::json m_config;
};

template <typename T> T Configuration::getSetting(const std::string& key, const T& defaultValue) const {
	try {
		auto ptr = dotToPointer(key);
		if (m_config.contains(ptr)) {
			return m_config[ptr].get<T>();
		}
	} catch (const nlohmann::json::exception& e) {
		LDYOM_ERROR("Error retrieving setting '{}': {}", key, e.what());
	} catch (const std::exception& e) {
		LDYOM_ERROR("Error retrieving setting '{}': {}", key, e.what());
	} catch (...) {
		LDYOM_ERROR("Unknown error retrieving setting '{}'", key);
	}
	return defaultValue;
}

template <typename T> void Configuration::setSetting(const std::string& key, const T& value) {
	auto ptr = dotToPointer(key);
	bool changed = !m_config.contains(ptr) || m_config[ptr] != value;
	if (changed) {
		m_config[ptr] = value;
		save();
	}
}