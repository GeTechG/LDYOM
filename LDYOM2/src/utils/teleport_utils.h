#pragma once
#include <CCamera.h>
#include <CMenuManager.h>
#include <CPed.h>
#include <CRadar.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>
#include <optional>

inline std::optional<CVector> getTargetBlipCoordinates() {
	// Get target blip index from menu manager (same as CLEO opcode 0AB6)
	DWORD hMarker = FrontEndMenuManager.m_nTargetBlipIndex;

	if (!hMarker)
		return std::nullopt;

	// Get the marker from RadarBlips array using low word as index
	tRadarTrace* pMarker = &CRadar::ms_RadarTrace[LOWORD(hMarker)];

	// Check if marker is valid and displayed
	if (!pMarker || !pMarker->m_nBlipDisplay)
		return std::nullopt;

	// Get marker position and find ground Z
	CVector coords(pMarker->m_vecPos);
	coords.z = CWorld::FindGroundZForCoord(coords.x, coords.y);

	return coords;
}

inline void teleportPlayerToMarker() {
	auto player = FindPlayerPed();
	if (!player)
		return;

	// Check if player is playing
	int playerId = 0;
	if (!plugin::Command<plugin::Commands::IS_PLAYER_PLAYING>(playerId))
		return;

	// Get target blip coordinates
	auto targetCoords = getTargetBlipCoordinates();
	if (!targetCoords.has_value())
		return;

	// Get player heading to preserve it
	float heading = player->m_fCurrentRotation * (180.0f / 3.14159265f);

	// Get player reference
	int playerRef = CPools::GetPedRef(player);

	// Request collision and teleport directly
	plugin::Command<plugin::Commands::REQUEST_COLLISION>(targetCoords->x, targetCoords->y);
	plugin::Command<plugin::Commands::LOAD_SCENE>(targetCoords->x, targetCoords->y, targetCoords->z);

	// Teleport player
	plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(playerRef, 0);
	plugin::Command<plugin::Commands::SET_CHAR_COORDINATES>(playerRef, targetCoords->x, targetCoords->y, targetCoords->z);
	plugin::Command<plugin::Commands::SET_CHAR_HEADING>(playerRef, heading);

	// Reset camera
	plugin::Command<plugin::Commands::SET_CAMERA_BEHIND_PLAYER>();
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();
}
