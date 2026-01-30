#include "CVehicleRecording.h"

void CVehicleRecording::ChangeCarPlaybackToUseAI(CVehicle *vehicle) {
	plugin::Call<0x45A360, CVehicle*>(vehicle);
}

UINT32 CVehicleRecording::FindIndexWithFileNameNumber(INT32 fileNumber) {
	return plugin::CallAndReturn<UINT32, 0x459FF0, INT32>(fileNumber);
}

bool CVehicleRecording::HasRecordingFileBeenLoaded(INT32 fileNumber) {
	return plugin::CallAndReturn<bool, 0x45A060, INT32>(fileNumber);
}

void CVehicleRecording::Load(RwStream *stream, INT32 recordId, INT32 totalSize) {
	plugin::Call<0x45A8F0, RwStream*, INT32, INT32>(stream, recordId, totalSize);
}

INT32 CVehicleRecording::RegisterRecordingFile(const char *name) {
	return plugin::CallAndReturn<INT32, 0x459F80, const char*>(name);
}

void CVehicleRecording::RemoveRecordingFile(INT32 fileNumber) {
	plugin::Call<0x45A0A0, INT32>(fileNumber);
}

void CVehicleRecording::RequestRecordingFile(INT32 fileNumber) {
	plugin::Call<0x45A020, INT32>(fileNumber);
}

void CVehicleRecording::SetPlaybackSpeed(CVehicle *vehicle, float speed) {
	plugin::Call<0x459660, CVehicle*, float>(vehicle, speed);
}

void CVehicleRecording::SetRecordingToPointClosestToCoors(INT32 playbackId, CVector posn) {
	plugin::Call<0x45A1E0, INT32, CVector>(playbackId, posn);
}

void CVehicleRecording::StopPlaybackWithIndex(INT32 playbackId) {
	plugin::Call<0x459440, INT32>(playbackId);
}

void CVehicleRecording::StartPlaybackRecordedCar(CVehicle *vehicle, INT32 fileNumber, bool useCarAI, bool looped) {
	plugin::Call<0x45A980, CVehicle*, INT32, bool, bool>(vehicle, fileNumber, useCarAI, looped);
}

void CVehicleRecording::StopPlaybackRecordedCar(CVehicle *vehicle) {
	plugin::Call<0x45A280, CVehicle*>(vehicle);
}

void CVehicleRecording::PausePlaybackRecordedCar(CVehicle *vehicle) {
	plugin::Call<0x459740, CVehicle*>(vehicle);
}

void CVehicleRecording::UnpausePlaybackRecordedCar(CVehicle *vehicle) {
	plugin::Call<0x459850, CVehicle*>(vehicle);
}

bool CVehicleRecording::IsPlaybackGoingOnForCar(CVehicle *vehicle) {
	return plugin::CallAndReturn<bool, 0x4594C0, CVehicle*>(vehicle);
}

bool CVehicleRecording::IsPlaybackPausedForCar(CVehicle *vehicle) {
	return plugin::CallAndReturn<bool, 0x4595A0, CVehicle*>(vehicle);
}
