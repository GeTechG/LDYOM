#pragma once
#include <CPed.h>
#include <CRadar.h>
#include <extensions/ScriptCommands.h>
#include <memory>

struct BlipDeleter {
	void operator()(int* handle) const noexcept {
		plugin::Command<plugin::Commands::REMOVE_BLIP>(*handle);
		delete handle;
	}
};

using BlipHandle = std::shared_ptr<int>;

inline BlipHandle addBlipToPed(int ped, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_CHAR>(ped, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return BlipHandle(new int(handle), BlipDeleter{});
}

inline BlipHandle addBlipToVehicle(int vehicle, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_CAR>(vehicle, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return BlipHandle(new int(handle), BlipDeleter{});
}

inline BlipHandle addBlipToObject(int object, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_OBJECT>(object, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return BlipHandle(new int(handle), BlipDeleter{});
}

inline BlipHandle addBlipToPickup(int pickup, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_PICKUP>(pickup, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return BlipHandle(new int(handle), BlipDeleter{});
}

inline BlipHandle addBlipToCoord(float x, float y, float z, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_COORD>(x, y, z, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return BlipHandle(new int(handle), BlipDeleter{});
}
