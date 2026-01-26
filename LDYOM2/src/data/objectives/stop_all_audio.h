#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <components/audio.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <utils/imgui_configurate.h>

namespace objectives::stop_all_audio {
constexpr const char* TYPE = "core.stop_all_audio";

struct Data {
	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveStopAllAudioData");
	}
	friend void to_json(nlohmann::json& j, const Data& data) { j = nlohmann::json::object(); }

	friend void from_json(const nlohmann::json& j, Data& data) {
		// No properties to deserialize
	}
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, (SCL_PX).y)); // Add some space at the top
}

inline ktwait execute(Data& data) {
	auto entities = ProjectPlayer::instance().getEntities();

	for (auto& entity : entities) {
		auto audio = components::Audio::cast(entity->getComponent(components::Audio::TYPE));
		if (audio && audio->projectAudioHandle.has_value()) {
			int streamState = 0;
			plugin::Command<0x0AB9>(audio->projectAudioHandle.value(), &streamState); // GET_AUDIO_STREAM_STATE
			if (streamState == 1) {                                                   // 1 = playing
				audio->despawn();
			}
		}
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "audio", .builder = make}; }
} // namespace objectives::stop_all_audio
