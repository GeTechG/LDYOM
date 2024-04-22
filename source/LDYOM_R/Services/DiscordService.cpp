#include "DiscordService.h"
#include <cstring>
#include <iostream>
#include <tlhelp32.h>

#include "easylogging/easylogging++.h"
#include "Localization/Localization.h"

bool isDiscordRunning() {
	// Create a snapshot of all running processes
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return false; // Failed to create snapshot
	}

	// Initialize process entry structure
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	// Iterate through processes
	if (!Process32First(snapshot, &entry)) {
		CloseHandle(snapshot);
		return false; // Failed to get first process
	}

	do {
		// Check if the process name is "discord.exe"
		if (_stricmp(entry.szExeFile, "discord.exe") == 0) {
			CloseHandle(snapshot);
			return true; // Discord is running
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return false; // Discord not found
}

void DiscordService::Init() {
	if (isDiscordRunning()) {
		auto result = discord::Core::Create(1172608604138901515, DiscordCreateFlags_NoRequireDiscord, &core);
		CLOG(INFO, "LDYOM") << "Discord core created: " << static_cast<int>(result);
		if (result != discord::Result::Ok) {
			core = nullptr;
			CLOG(ERROR, "LDYOM") << "Failed to create Discord core: " << static_cast<int>(result);
			return;
		}

		core->SetLogHook(
			discord::LogLevel::Debug, [](discord::LogLevel level, const char *message) {
				CLOG(DEBUG, "LDYOM") << message;
			});
	} else {
		CLOG(INFO, "LDYOM") << "Discord is not running!";
	}
}

void DiscordService::runCallbacks() const {
	if (this->core) {
		core->RunCallbacks();
	}
}

void DiscordService::updateActivity(const std::string &projectName, DiscordActivityType type) const {
	if (this->core) {
		auto &local = Localization::getInstance();
		const auto activities = local.getArray("discord.activity_type");

		discord::Activity activity{};
		activity.SetDetails(projectName.c_str());
		activity.GetTimestamps().SetStart(time(nullptr));
		activity.SetState(activities[static_cast<size_t>(type)].c_str());
		activity.GetAssets().SetSmallImage("smallicon");
		activity.GetAssets().SetLargeImage("ldyom");
		activity.GetParty().SetPrivacy(discord::ActivityPartyPrivacy::Public);
		activity.SetType(discord::ActivityType::Playing);
		core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
			CLOG(INFO, "LDYOM") << "UpdateActivity: " << static_cast<int>(result);
		});
	}
}
