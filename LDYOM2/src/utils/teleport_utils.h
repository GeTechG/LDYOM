#pragma once
#include <CCamera.h>
#include <CMenuManager.h>
#include <CPed.h>
#include <CRadar.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>
#include <optional>
#include <utils/ktcoro_wait.hpp>

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

inline ktwait teleportPlayerToMarker() {
	auto player = FindPlayerPed();
	if (!player)
		co_return;

	int playerId = 0;
	if (!plugin::Command<plugin::Commands::IS_PLAYER_PLAYING>(playerId))
		co_return;

	auto targetCoords = getTargetBlipCoordinates();
	if (!targetCoords.has_value())
		co_return;

	float heading = player->m_fCurrentRotation * (180.0f / 3.14159265f);
	int playerRef = CPools::GetPedRef(player);

	// Fade out to black
	plugin::Command<plugin::Commands::SET_FADING_COLOUR>(0, 0, 0);
	plugin::Command<plugin::Commands::DO_FADE>(500, 0);
	co_await 500;

	// Disable player control and prepare world state
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(playerId, false);
	plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(0);
	plugin::Command<plugin::Commands::CLEAR_EXTRA_COLOURS>(false);
	plugin::Command<plugin::Commands::SET_PLAYER_IS_IN_STADIUM>(false);

	// Load scene at target location
	plugin::Command<plugin::Commands::REQUEST_COLLISION>(targetCoords->x, targetCoords->y);
	plugin::Command<plugin::Commands::LOAD_SCENE>(targetCoords->x, targetCoords->y, targetCoords->z);

	// Teleport player (Z=-100.0 lets the game find the actual ground Z)
	if (plugin::Command<plugin::Commands::IS_PLAYER_PLAYING>(playerId)) {
		plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(playerRef, 0);
		plugin::Command<plugin::Commands::SET_CHAR_COORDINATES>(playerRef, targetCoords->x, targetCoords->y, -100.0f);
		plugin::Command<plugin::Commands::SET_CHAR_HEADING>(playerRef, heading);
	}

	// Wait 1 frame, then until player is stable
	co_await 1;
	while (!plugin::Command<plugin::Commands::IS_PLAYER_PLAYING>(playerId))
		co_await 1;
	co_await 1500;

	// Fade in
	plugin::Command<plugin::Commands::DO_FADE>(1000, 1);
	while (plugin::Command<plugin::Commands::GET_FADING_STATUS>())
		co_await 1;

	co_await 1;
	while (!plugin::Command<plugin::Commands::IS_PLAYER_PLAYING>(playerId))
		co_await 1;

	// Re-enable player control and reset camera
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(playerId, true);
	plugin::Command<plugin::Commands::SET_CAMERA_BEHIND_PLAYER>();
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();
}

inline void unlockPlayer() {
	auto player = FindPlayerPed();
	if (!player)
		return;

	// Check if player is playing
	int playerId = 0;
	if (!plugin::Command<plugin::Commands::IS_PLAYER_PLAYING>(playerId))
		return;

	// Get player reference
	int playerRef = CPools::GetPedRef(player);

	// Enable player control if disabled
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(playerId, true);

	// Get current position
	CVector currentPos = player->GetPosition();

	// Lift player up slightly (0.5 units)
	currentPos.z += 0.5f;

	// Set new position
	plugin::Command<plugin::Commands::SET_CHAR_COORDINATES>(playerRef, currentPos.x, currentPos.y, currentPos.z);

	// Ensure player is not frozen
	plugin::Command<plugin::Commands::FREEZE_CHAR_POSITION>(playerRef, false);
}
