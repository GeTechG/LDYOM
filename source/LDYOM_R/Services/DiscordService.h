#pragma once
#include <memory>
#include <string>

#include "discord.h"

enum class DiscordActivityType {
	CREATING,
	PLAYING
};

class DiscordService {
public:
	DiscordService(const DiscordService &) = delete;
	DiscordService& operator=(const DiscordService &) = delete;
	~DiscordService() = default;

	void Init();
	void runCallbacks() const;
	void updateActivity(const std::string &projectName, DiscordActivityType type) const;

	static DiscordService& getInstance() {
		static DiscordService instance{};
		return instance;
	}

private:
	explicit DiscordService() = default;

	discord::Core *core = nullptr;
};
