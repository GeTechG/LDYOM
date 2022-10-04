#include <sol.hpp>
#include "../Data/Audio.h"

void AudioLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Audio>("Audio");
	clazz["getUuid"] = &Audio::getUuid;
	clazz["getEditorAudio"] = &Audio::getEditorAudio;
	clazz["getProjectAudio"] = &Audio::getProjectAudio;
	clazz["getAudioName"] = &Audio::getAudioName;
	clazz["isAudio3D"] = &Audio::isAudio3D;
	clazz["getAttachType3d"] = &Audio::getAttachType3d;
	clazz["getAttachUuid"] = &Audio::getAttachUuid;
	clazz["isIsLooped"] = &Audio::isIsLooped;
	clazz["getVolume"] = &Audio::getVolume;
	clazz["updateLocation"] = &Audio::updateLocation;
	clazz["getName"] = &Audio::getName;
	clazz["getPosition"] = &Audio::getPosition;
	clazz["spawnEditorAudio"] = &Audio::spawnEditorAudio;
	clazz["deleteEditorAudio"] = &Audio::deleteEditorAudio;
	clazz["spawnProjectEntity"] = &Audio::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Audio::deleteProjectEntity;
	clazz["preloadProjectAudio"] = &Audio::preloadProjectAudio;
	clazz["getAudioFilesList"] = &Audio::getAudioFilesList;
	clazz["loadAudioFilesList"] = &Audio::loadAudioFilesList;
}
