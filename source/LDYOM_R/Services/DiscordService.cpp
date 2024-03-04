#include "DiscordService.h"
#include <cstring>
#include <iostream>

#include "easylogging/easylogging++.h"
#include "Localization/Localization.h"

void DiscordService::Init() {
	try {
		auto result = discord::Core::Create(1172608604138901515, DiscordCreateFlags_Default, &core);
		CLOG(INFO, "LDYOM") << "Discord core created: " << static_cast<int>(result);
	} catch (std::exception) {
		CLOG(ERROR, "LDYOM") << "Failed to instantiate discord core!";
		return;
	}

	core->SetLogHook(
		discord::LogLevel::Debug, [](discord::LogLevel level, const char *message) {
			CLOG(DEBUG, "LDYOM") << message;
		});
}

void DiscordService::runCallbacks() const {
	core->RunCallbacks();
}

void DiscordService::updateActivity(const std::string &projectName, DiscordActivityType type) const {
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
