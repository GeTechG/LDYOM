#pragma once

#include "fixed_float.h"
#include <array>
#include <nlohmann/json.hpp>
#include <plugin.h>
#include <utils/cvector_json_parser.h>

/**
 * Vehicle state captured for each frame during recording
 * Compressed to exactly 32 bytes using fixed-point arithmetic
 *
 * Memory layout:
 * - 4 bytes: timestamp (UINT32)
 * - 6 bytes: velocity (INT16 x3)
 * - 3 bytes: right vector (INT8 x3)
 * - 3 bytes: top vector (INT8 x3)
 * - 1 byte:  steering angle (UINT8)
 * - 1 byte:  gas pedal (UINT8)
 * - 1 byte:  brake pedal (UINT8)
 * - 1 byte:  handbrake flag (bool)
 * - 12 bytes: position (float x3)
 * ----------------------------------------
 * Total: 32 bytes (0x20)
 */
struct CVehicleStateEachFrame {
	UINT32 m_nTime;
	FixedVector<INT16, 16383.5f> m_sVelocity;
	FixedVector<INT8, 127.0f> m_bRight;
	FixedVector<INT8, 127.0f> m_bTop;
	FixedFloat<UINT8, 20.0f> m_bSteeringAngle;
	FixedFloat<UINT8, 100.0f> m_bGasPedalPower;
	FixedFloat<UINT8, 100.0f> m_bBreakPedalPower;
	bool m_bHandbrakeUsed;
	CVector m_vecPosn;

	CVehicleStateEachFrame()
		: m_nTime(0),
		  m_sVelocity(0.0f, 0.0f, 0.0f),
		  m_bRight(1.0f, 0.0f, 0.0f),
		  m_bTop(0.0f, 1.0f, 0.0f),
		  m_bSteeringAngle(0.0f),
		  m_bGasPedalPower(0.0f),
		  m_bBreakPedalPower(0.0f),
		  m_bHandbrakeUsed(false),
		  m_vecPosn({0.0f, 0.0f, 0.0f}) {}

	CVehicleStateEachFrame(UINT32 time, CVector velocity, CVector right, CVector top, float steeringAngle,
	                       float gasPedal, float breakPedal, bool handbrake, CVector position)
		: m_nTime(time),
		  m_sVelocity(velocity.x, velocity.y, velocity.z),
		  m_bRight(right.x, right.y, right.z),
		  m_bTop(top.x, top.y, top.z),
		  m_bSteeringAngle(steeringAngle),
		  m_bGasPedalPower(gasPedal),
		  m_bBreakPedalPower(breakPedal),
		  m_bHandbrakeUsed(handbrake),
		  m_vecPosn(position) {}

	// JSON serialization
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(CVehicleStateEachFrame, m_nTime, m_sVelocity, m_bRight, m_bTop, m_bSteeringAngle,
	                               m_bGasPedalPower, m_bBreakPedalPower, m_bHandbrakeUsed, m_vecPosn)
};

VALIDATE_SIZE(CVehicleStateEachFrame, 0x20);
