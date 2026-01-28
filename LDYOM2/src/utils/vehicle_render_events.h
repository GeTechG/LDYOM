#pragma once
#include <CVehicle.h>
#include <plugin.h>

// Vehicle rendering events
// Based on LDYOM Beta 0.8 additional_events.h
// These hooks are called during vehicle rendering to allow color modifications

namespace VehicleRenderEvents {
// Event called after vehicle rendering to reset modifications
// Address: 0x55332A - CVehicle::ResetAfterRender
extern plugin::ThiscallEvent<plugin::AddressList<0x55332A, plugin::H_CALL>, plugin::PRIORITY_BEFORE,
                              plugin::ArgPickN<CVehicle*, 0>, void(CVehicle*)>
	vehicleResetAfterRender;

// Note: vehicleRenderEvent from old version was a standard plugin-sdk event
// We'll use the existing plugin::Events::vehicleRenderEvent if available,
// or create a MinHook-based solution if not

void initializeEvents();
} // namespace VehicleRenderEvents
