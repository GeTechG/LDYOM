#include <sol.hpp>
#include "../Data/Audio.h"

void audioWrapper(sol::state &state) {
	state.new_usertype<Audio>("LDAudio",
	                          sol::no_constructor,
	                          "copy", &Audio::copy,
	                          "getUuid", &Audio::getUuid,
	                          "getEditorAudio", &Audio::getEditorAudio,
	                          "getProjectAudio", &Audio::getProjectAudio,
	                          "getAudioName", &Audio::getAudioName,
	                          "isAudio3D", &Audio::isAudio3D,
	                          "getAttachType3d", &Audio::getAttachType3d,
	                          "getAttachUuid", &Audio::getAttachUuid,
	                          "isIsLooped", &Audio::isIsLooped,
	                          "getVolume", &Audio::getVolume,
	                          "updateLocation", &Audio::updateLocation,
	                          "getName", &Audio::getName,
	                          "getPosition", [](Audio &audio) {
		                          return std::make_tuple(audio.getPosition()[0], audio.getPosition()[1],
		                                                 audio.getPosition()[2]);
	                          },
	                          "setPosition", [](Audio &audio, const float x, const float y, const float z) {
		                          audio.getPosition()[0] = x;
		                          audio.getPosition()[1] = y;
		                          audio.getPosition()[2] = z;
	                          },
	                          "spawnEditorAudio", &Audio::spawnEditorAudio,
	                          "deleteEditorAudio", &Audio::deleteEditorAudio,
	                          "spawnProjectEntity", &Audio::spawnProjectEntity,
	                          "deleteProjectEntity", &Audio::deleteProjectEntity,
	                          "getAudioFilesList", &Audio::getAudioFilesList,
	                          "loadAudioFilesList", &Audio::loadAudioFilesList
	);
}
