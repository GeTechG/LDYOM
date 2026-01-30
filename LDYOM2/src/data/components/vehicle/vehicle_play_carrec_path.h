#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <optional>
#include <rocket.hpp>
#include <string>

namespace components {

class Vehicle;

/**
 * Vehicle component for playing back recorded CarRec paths
 * Allows vehicles to follow pre-recorded paths with AI or ghost mode
 */
class VehiclePlayCarrecPath : public Component {
private:
    std::optional<rocket::scoped_connection> m_vehicleSpawnedConnection;
    int m_playbackSlot = -1; // Current playback slot (-1 = not playing)

    void renderPathSelection();
    std::shared_ptr<components::Vehicle> getVehicleComponent();

public:
    static constexpr auto TYPE = "vehicle_play_carrec_path";
    static constexpr auto CATEGORY = "vehicle";

    static std::shared_ptr<VehiclePlayCarrecPath> cast(std::shared_ptr<Component> component) {
        return std::dynamic_pointer_cast<VehiclePlayCarrecPath>(component);
    }

    static Dependencies getDependencies();

    // Properties
    std::string pathUuid = "";              // UUID of CarrecPath to play
    bool useAI = false;                     // AI following vs ghost mode
    bool looped = false;                    // Loop playback
    float playbackSpeed = 1.0f;             // Speed multiplier (not yet implemented)
    bool autoStart = true;                  // Start playback automatically on scene start

    VehiclePlayCarrecPath();

    /**
     * Start playback of the assigned path
     * @return true if playback started successfully
     */
    bool startPlayback();

    /**
     * Stop playback
     */
    void stopPlayback();

    /**
     * Check if playback is currently active
     */
    bool isPlaying() const { return m_playbackSlot != -1; }

    [[nodiscard]] nlohmann::json to_json() const override;
    void from_json(const nlohmann::json& j) override;
    void editorRender() override;

    void onStart() override;
    void onUpdate(float deltaTime) override;
    void onReset() override;

    static void sol_lua_register(sol::state_view lua_state);
    static std::shared_ptr<Component> make() { return std::make_shared<VehiclePlayCarrecPath>(); }
};

} // namespace components
