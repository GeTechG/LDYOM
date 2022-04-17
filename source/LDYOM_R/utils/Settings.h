#pragma once
#include "nlohmann/json.hpp"
#include "utils.h"
#include "boost/signals2.hpp"
#include "imHotKey.h"

class Settings
{
private:

	nlohmann::json settings_;
	std::vector<std::string> listLocalizations_;
	std::vector<std::string> listThemes_;
	boost::signals2::signal<void()> onUpdate_;

	Settings() = default;
	Settings(const Settings&) = delete;
	Settings& operator=(Settings&) = delete;

public:
	static Settings& getInstance() {
		static Settings instance;
		return instance;
	}

	nlohmann::json& getSettings();

	boost::signals2::signal<void()>& onUpdate();

	template<typename T>
	std::optional<T> get(std::string path);
	template<typename T>
	void             set(std::string path, T& data);

	std::vector<std::string>& listLocalizations();
	std::vector<std::string>& listThemes();

	void Init();
	void Save() const;
};

template <typename T>
std::optional<T> Settings::get(std::string path)
{
	const std::vector<std::string> partPath = utils::split(path, ".");
	nlohmann::json* temp = &settings_;
	for (std::string part : partPath)
	{
		temp = &(*temp)[part];
	}
	if (temp->is_null())
		return std::nullopt;
	return temp->get<T>();
}

template <typename T>
void Settings::set(std::string path, T& data)
{
	std::vector<std::string> partPath = utils::split(path, ".");
	std::string last = partPath.back();
	partPath.pop_back();
	nlohmann::json* temp = &settings_;
	for (std::string part : partPath)
	{
		temp = &(*temp)[part];
	}
	(*temp)[last] = data;
}

