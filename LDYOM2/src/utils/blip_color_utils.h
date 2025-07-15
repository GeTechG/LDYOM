#pragma once
#include <CPed.h>
#include <CRadar.h>
#include <extensions/ScriptCommands.h>

inline int addBlipToPed(int ped, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_CHAR>(ped, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return handle;
}

inline int addBlipToVehicle(int vehicle, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_CAR>(vehicle, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return handle;
}

inline int addBlipToCoord(float x, float y, float z, int colorBlip) {
	int handle;
	plugin::Command<plugin::Commands::ADD_BLIP_FOR_COORD>(x, y, z, &handle);
	if (colorBlip != 6) {
		CRadar::ChangeBlipColour(handle, colorBlip - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return handle;
}