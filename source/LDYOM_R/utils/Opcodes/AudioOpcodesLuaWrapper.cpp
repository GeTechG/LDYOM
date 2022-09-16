//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void AudioOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("AudioOp");
    //default
	table.set_function("playMissionPassedTune", [](const int& _soundId) { auto result = Command<0x0394>(_soundId); return std::make_tuple(result);});
	table.set_function("loadMissionAudio", [](const int& _slotId, const int& _audioId) { auto result = Command<0x03CF>(_slotId, _audioId); return std::make_tuple(result);});
	table.set_function("hasMissionAudioLoaded", [](const int& _slotId) { auto result = Command<0x03D0>(_slotId); return std::make_tuple(result);});
	table.set_function("playMissionAudio", [](const int& _slotId) { auto result = Command<0x03D1>(_slotId); return std::make_tuple(result);});
	table.set_function("hasMissionAudioFinished", [](const int& _slotId) { auto result = Command<0x03D2>(_slotId); return std::make_tuple(result);});
	table.set_function("setMissionAudioPosition", [](const int& _slotId, const float& _x, const float& _y, const float& _z) { auto result = Command<0x03D7>(_slotId, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("clearMissionAudio", [](const int& _slotId) { auto result = Command<0x040D>(_slotId); return std::make_tuple(result);});
	table.set_function("setRadioChannel", [](const int& _channel) { auto result = Command<0x041E>(_channel); return std::make_tuple(result);});
	table.set_function("setMusicDoesFade", [](const bool& _state) { auto result = Command<0x043C>(_state); return std::make_tuple(result);});
	table.set_function("getRadioChannel", []() {int channel_; auto result = Command<0x051E>(&channel_); return std::make_tuple(channel_, result);});
	table.set_function("getBeatProximity", [](const int& __p1) {int _p2_;int _p3_;int _p4_; auto result = Command<0x07B1>(__p1, &_p2_, &_p3_, &_p4_); return std::make_tuple(_p2_, _p3_, _p4_, result);});
	table.set_function("attachMissionAudioToChar", [](const int& _slotId, const int& _handle) { auto result = Command<0x0949>(_slotId, _handle); return std::make_tuple(result);});
	table.set_function("preloadBeatTrack", [](const int& _trackId) { auto result = Command<0x0952>(_trackId); return std::make_tuple(result);});
	table.set_function("getBeatTrackStatus", []() {int status_; auto result = Command<0x0953>(&status_); return std::make_tuple(status_, result);});
	table.set_function("playBeatTrack", []() { auto result = Command<0x0954>(); return std::make_tuple(result);});
	table.set_function("stopBeatTrack", []() { auto result = Command<0x0955>(); return std::make_tuple(result);});
	table.set_function("reportMissionAudioEventAtPosition", [](const float& _x, const float& _y, const float& _z, const int& _soundId) { auto result = Command<0x097A>(_x, _y, _z, _soundId); return std::make_tuple(result);});
	table.set_function("reportMissionAudioEventAtObject", [](const int& _handle, const int& _soundId) { auto result = Command<0x097B>(_handle, _soundId); return std::make_tuple(result);});
	table.set_function("attachMissionAudioToObject", [](const int& _slotId, const int& _handle) { auto result = Command<0x097C>(_slotId, _handle); return std::make_tuple(result);});
	table.set_function("pauseCurrentBeatTrack", [](const bool& _state) { auto result = Command<0x0991>(_state); return std::make_tuple(result);});
	table.set_function("reportMissionAudioEventAtChar", [](const int& _handle, const int& _soundId) { auto result = Command<0x09F1>(_handle, _soundId); return std::make_tuple(result);});
	table.set_function("reportMissionAudioEventAtCar", [](const int& _handle, const int& _soundId) { auto result = Command<0x09F7>(_handle, _soundId); return std::make_tuple(result);});
	table.set_function("attachMissionAudioToCar", [](const int& _slotId, const int& _handle) { auto result = Command<0x0A16>(_slotId, _handle); return std::make_tuple(result);});
	table.set_function("setRadioToPlayersFavouriteStation", []() { auto result = Command<0x0A26>(); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("getSfxVolume", []() {float volume_; auto result = Command<0x0E21>(&volume_); return std::make_tuple(volume_, result);});
	table.set_function("getRadioVolume", []() {float volume_; auto result = Command<0x0E22>(&volume_); return std::make_tuple(volume_, result);});

}