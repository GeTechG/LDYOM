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