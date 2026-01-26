#pragma once
#include "component.h"
#include <CObject.h>
#include <CPools.h>
#include <components_manager.h>
#include <fa_icons.h>
#include <filesystem>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <models_manager.h>
#include <optional>
#include <rocket.hpp>
#include <string>
#include <vector>

namespace components {

class Audio : public Component {
  private:
	void updatePosition();
	void loadAudio();
	Entity* findEntityByUuid(const std::string& uuid) const;

	std::optional<rocket::scoped_connection> onSpawnedConnection;
	std::optional<rocket::scoped_connection> onDespawnedConnection;

	std::shared_ptr<CObject> m_visualMarker;

  public:
	enum DirtyFlags {
		None = 0,
		Position = 1 << 0,
		AudioFile = 1 << 1,
		Audio3D = 1 << 2,
		AttachType = 1 << 3,
		Loop = 1 << 4,
		Volume = 1 << 5,
	};
	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "AudioComponentDirtyFlags", None, Position, AudioFile, Audio3D, AttachType, Loop, Volume);

	static constexpr auto TYPE = "audio";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Audio> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Audio>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	// Audio properties
	std::string audioFileName;
	bool audio3D = false;
	int attachType = 0; // 0=coords, 1=Actor, 2=Vehicle, 3=Object
	std::string attachEntityUuid;
	bool isLooped = false;
	float volume = 1.0f;

	// Runtime state
	int dirty = DirtyFlags::None;
	std::optional<int> editorAudioHandle;
	std::optional<int> projectAudioHandle;

	// Static audio files list
	static std::vector<std::string> s_audioFilesList;
	static bool s_audioFilesLoaded;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Audio();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	void spawn();
	void despawn();

	static void loadAudioFilesList();
	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);

	static std::shared_ptr<Component> make() { return std::make_shared<Audio>(); }
};
} // namespace components
