// ReSharper disable CppMemberFunctionMayBeStatic
#include "Settings.h"
#include <filesystem>
#include <fstream>
#include <thread>
#include "Localization/Localization.h"

#include <Windows.h>

#include "FileWatcher.h"
#include "HotKeyService.h"
#include "WindowsRenderService.h"

const std::filesystem::path settingsPath(L"LDYOM/settings.json");
const std::filesystem::path localizationsDirectory(L"LDYOM/Languages");
const std::filesystem::path themesDirectory(L"LDYOM/Themes");

const nlohmann::json defaultSettings = R"(
  {
    "main": {
        "language": "English",
        "theme": "default"
    }
}
)"_json;

nlohmann::json& Settings::getSettings()
{
	return this->settings_;
}

boost::signals2::signal<void()>& Settings::onUpdate() {
	return onUpdate_;
}

std::vector<std::string> getListLocalizations()
{
	std::vector<std::string> localizations{};
	for (const auto& file : std::filesystem::directory_iterator(localizationsDirectory))
	{
		if (!file.is_directory() && file.path().extension() == ".toml")
		{
			localizations.emplace_back(reinterpret_cast<const char*>(file.path().stem().generic_u8string().c_str()));
		}
	}
	return localizations;
}

std::vector<std::string>& Settings::listLocalizations()
{
	return listLocalizations_;
}

std::vector<std::string> getListThemes()
{
	std::vector<std::string> themes{};
	for (const auto& file : std::filesystem::directory_iterator(themesDirectory))
	{
		if (!file.is_directory() && file.path().extension() == ".toml")
		{
			themes.emplace_back(reinterpret_cast<const char*>(file.path().stem().generic_u8string().c_str()));
		}
	}
	return themes;
}

std::vector<std::string>& Settings::listThemes()
{
	return listThemes_;
}

void settingsWatcher(boost::signals2::signal<void()> *onUpdate) {
	HANDLE dwChangeHandles[2];

	dwChangeHandles[0] = FindFirstChangeNotification(
		localizationsDirectory.string().c_str(),
		FALSE,
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE);
	dwChangeHandles[1] = FindFirstChangeNotification(
		themesDirectory.string().c_str(),
		FALSE,
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE);
	while (true) {
		const DWORD dwWaitStatus = WaitForMultipleObjects(2,
		                                                  dwChangeHandles,
		                                                  FALSE,
		                                                  INFINITE);

		auto update = false;

		switch (dwWaitStatus) {
		case WAIT_OBJECT_0:
			Settings::getInstance().listLocalizations() = getListLocalizations();
			Localization::getInstance().Update();
			FindNextChangeNotification(dwChangeHandles[0]);
			update = true;
			break;

		case WAIT_OBJECT_0 + 1:
			Settings::getInstance().listThemes() = getListThemes();
			Windows::WindowsRenderService::getInstance().style();
			FindNextChangeNotification(dwChangeHandles[1]);
			update = true;
			break;
		default:
			break;
		}

		if (update) {
			(*onUpdate)();
		}

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

void Settings::Init()
{
	if (exists(settingsPath))
	{
		std::ifstream settingsFile(settingsPath);
		try {
			settingsFile >> settings_;
		} catch(std::exception e) {
			const std::string error = fmt::format("error read settings, error: {}", e.what());
			Logger::getInstance().log(error);
			throw std::exception(error.c_str());
		}
	} else
	{
		settings_ = defaultSettings;
		Save();
	}
	this->listLocalizations_ = getListLocalizations();
	this->listThemes_ = getListThemes();
	//auto settingsUpdateThead = std::thread(settingsWatcher, &this->onUpdate_);
	//settingsUpdateThead.detach();
	FileWatcher::addWatch(
		localizationsDirectory, 
		false, 
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, 
		[&] {
			this->listLocalizations() = getListLocalizations();
			Localization::getInstance().Update();
			this->onUpdate();
		});
	FileWatcher::addWatch(
		themesDirectory,
		false,
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
		[&] {
			this->listThemes() = getListThemes();
			Windows::WindowsRenderService::getInstance().style();
			this->onUpdate();
		});
}

void Settings::Save() const
{
	std::ofstream settingsFile(settingsPath);
	settingsFile << settings_.dump(4);
	settingsFile.close();
}
