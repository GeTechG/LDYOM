#pragma once
#include <CStreaming.h>
#include <CVector.h>
#include <CVehicle.h>
#include <ranges>
#include <span>
#include <nlohmann/json.hpp>
#include "CVectorSerialization.h"

#include "FixedVector .h"

constexpr UINT32 TOTAL_RRR_MODEL_IDS = 475;

class CVehicleStateEachFrame {
public:
	UINT32 m_nTime;
	FixedVector<INT16, 16383.5f> m_sVelocity;
	FixedVector<INT8, 127.0f> m_bRight;
	FixedVector<INT8, 127.0f> m_bTop;
	FixedFloat<UINT8, 20.0f> m_bSteeringAngle;
	FixedFloat<UINT8, 100.0f> m_bGasPedalPower;
	FixedFloat<UINT8, 100.0f> m_bBreakPedalPower;
	bool m_bHandbrakeUsed;
	CVector m_vecPosn;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(CVehicleStateEachFrame, m_nTime, m_sVelocity, m_bRight, m_bTop, m_bSteeringAngle,
	                               m_bGasPedalPower, m_bBreakPedalPower, m_bHandbrakeUsed, m_vecPosn)
};

VALIDATE_SIZE(CVehicleStateEachFrame, 0x20);

constexpr auto TOTAL_VEHICLE_RECORDS = 16;

class CPath {
public:
	INT32 m_nNumber;
	CVehicleStateEachFrame *m_pData;
	INT32 m_nBytesSize; // Byte size, use ::Size() for getting the element size!
	INT8 m_nRefCount;

	UINT32 GetIndex() const;

	size_t Size() const {
		return m_nBytesSize / sizeof(CVehicleStateEachFrame);
	}

	auto GetFrames() {
		return std::span{m_pData, Size()};
	}

	/*void Remove() {
	    if (m_pData) {
	        CMemoryMgr::Free(m_pData);
	        m_pData = nullptr;
	        CStreaming::RemoveModel(RRRToModelId(GetIndex()));
	    }
	}*/

	//void AddRef() {
	//    CARREC_DEV_LOG("Ref added for path {} (number= {}, size= {}, ptr= {})", GetIndex(), m_nNumber, m_nBytesSize, LOG_PTR(m_pData));
	//    m_nRefCount++;
	//}

	//void RemoveRef() {
	//    CARREC_DEV_LOG("Ref removed for path {} (number= {}, size= {}, ptr= {})", GetIndex(), m_nNumber, m_nBytesSize, LOG_PTR(m_pData));
	//    if (!--m_nRefCount) {
	//        Remove();
	//    }
	//}
};

VALIDATE_SIZE(CPath, 0x10);

struct RwStream;

class CVehicleRecording {
public:
	static inline INT32 &NumPlayBackFiles = *(INT32*)0x97F630;
	static inline std::array<CPath, TOTAL_RRR_MODEL_IDS> &StreamingArray = *(std::array<CPath, TOTAL_RRR_MODEL_IDS>*)
		0x97D880;
	static inline std::array<CVehicle*, TOTAL_VEHICLE_RECORDS> &pVehicleForPlayback = *(std::array<
		CVehicle*, TOTAL_VEHICLE_RECORDS>*)0x97D840;
	static inline std::array<CVehicleStateEachFrame*, TOTAL_VEHICLE_RECORDS> &pPlaybackBuffer = *(std::array<
		CVehicleStateEachFrame*, TOTAL_VEHICLE_RECORDS>*)0x97D800;
	static inline std::array<INT32, TOTAL_VEHICLE_RECORDS> &PlaybackIndex = *(std::array<INT32, TOTAL_VEHICLE_RECORDS>*)
		0x97D7C0;
	static inline std::array<INT32, TOTAL_VEHICLE_RECORDS> &PlaybackBufferSize = *(std::array<
		INT32, TOTAL_VEHICLE_RECORDS>*)0x97D780;
	static inline std::array<float, TOTAL_VEHICLE_RECORDS> &PlaybackRunningTime = *(std::array<
		float, TOTAL_VEHICLE_RECORDS>*)0x97D740;
	static inline std::array<float, TOTAL_VEHICLE_RECORDS> &PlaybackSpeed = *(std::array<float, TOTAL_VEHICLE_RECORDS>*)
		0x97D700;
	static inline std::array<bool, TOTAL_VEHICLE_RECORDS> &bPlaybackGoingOn = *(std::array<bool, TOTAL_VEHICLE_RECORDS>
		*)0x97D6F0;
	static inline std::array<bool, TOTAL_VEHICLE_RECORDS> &bPlaybackLooped = *(std::array<bool, TOTAL_VEHICLE_RECORDS>*)
		0x97D6E0;
	static inline std::array<bool, TOTAL_VEHICLE_RECORDS> &bPlaybackPaused = *(std::array<bool, TOTAL_VEHICLE_RECORDS>*)
		0x97D6D0;
	static inline std::array<bool, TOTAL_VEHICLE_RECORDS> &bUseCarAI = *(std::array<bool, TOTAL_VEHICLE_RECORDS>*)
		0x97D6C0;
	static inline std::array<UINT32, TOTAL_VEHICLE_RECORDS> &PlayBackStreamingIndex = *(std::array<
		UINT32, TOTAL_VEHICLE_RECORDS>*)0x97D670;
	// DisplayMode

public:
	/*static void Init();
	static void InitAtStartOfGame();
	static void ShutDown();
	static void Render();*/

	static void ChangeCarPlaybackToUseAI(CVehicle *vehicle);
	static UINT32 FindIndexWithFileNameNumber(INT32 fileNumber);
	//static void InterpolateInfoForCar(CVehicle *vehicle, const CVehicleStateEachFrame &frame, float interpValue);

	static bool HasRecordingFileBeenLoaded(INT32 fileNumber);
	static void Load(RwStream *stream, INT32 recordId, INT32 totalSize);
	//static void SmoothRecording(INT32 recordId);

	static INT32 RegisterRecordingFile(const char *name);
	static void RemoveRecordingFile(INT32 fileNumber);
	static void RequestRecordingFile(INT32 fileNumber);

	static void SetPlaybackSpeed(CVehicle *vehicle, float speed);
	//static void RenderLineSegment(INT32 &numVertices);

	/*static void RemoveAllRecordingsThatArentUsed();
	static void RestoreInfoForCar(CVehicle *vehicle, const CVehicleStateEachFrame &frame, bool pause);
	static void RestoreInfoForMatrix(CMatrix &matrix, const CVehicleStateEachFrame &frame);*/
	//static void SaveOrRetrieveDataForThisFrame();
	static void SetRecordingToPointClosestToCoors(INT32 playbackId, CVector posn);
	//static void SkipForwardInRecording(CVehicle *vehicle, float distance);
	//static void SkipToEndAndStopPlaybackRecordedCar(CVehicle *vehicle);

	static void StopPlaybackWithIndex(INT32 playbackId);
	static void StartPlaybackRecordedCar(CVehicle *vehicle, INT32 fileNumber, bool useCarAI, bool looped);
	static void StopPlaybackRecordedCar(CVehicle *vehicle);
	static void PausePlaybackRecordedCar(CVehicle *vehicle);
	static void UnpausePlaybackRecordedCar(CVehicle *vehicle);

	static bool IsPlaybackGoingOnForCar(CVehicle *vehicle);
	static bool IsPlaybackPausedForCar(CVehicle *vehicle);

	// @notsa
	static std::span<CPath> GetRecordings() {
		return std::span{StreamingArray.data(), static_cast<size_t>(NumPlayBackFiles)};
	}

	static auto GetActivePlaybackIndices() {
		return std::ranges::views::iota(0, TOTAL_VEHICLE_RECORDS) | std::views::filter([](auto &&i) {
			return bPlaybackGoingOn[i];
		});
	}

	static CPath* FindRecording(INT32 number) {
		if (!NumPlayBackFiles)
			return nullptr;

		const auto idx = FindIndexWithFileNameNumber(number);
		// FindIndexWithFileNameNumber returns zero if not found, but
		// can also return zero if the found path is at the first index.
		if (idx != 0 || StreamingArray[idx].m_nNumber == number) {
			return &StreamingArray[idx];
		}
		return nullptr;
	}

	static INT32 FindVehicleRecordingIndex(CVehicle *vehicle) {
		for (auto i : GetActivePlaybackIndices()) {
			if (pVehicleForPlayback[i] == vehicle) {
				return i;
			}
		}
		return -1;
	}

	static auto GetFramesFromPlaybackBuffer(size_t bufferIdx) {
		return std::span{pPlaybackBuffer[bufferIdx], PlaybackBufferSize[bufferIdx] / sizeof(CVehicleStateEachFrame)};
	}

	static auto GetCurrentFrameIndex(size_t bufferIdx) {
		return PlaybackIndex[bufferIdx] / sizeof(CVehicleStateEachFrame);
	}
};
