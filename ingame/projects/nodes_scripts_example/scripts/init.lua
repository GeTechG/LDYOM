local sweet_handler = require("sweet_handler")

---@type fun() | nil
local on_objective_started_disconnect = nil

---@type fun() | nil
local on_scene_started_disconnect = nil

---@type fun() | nil
local on_scene_ended_disconnect = nil

---@type fun() | nil
local on_project_stopped_disconnect = nil

register_project_script({
    on_load = function()
        on_objective_started_disconnect = require("on_objective_handler")
        on_scene_started_disconnect = events.on_scene_started(function(sceneId)
            if project_player.current_scene_id == "1774287674" then
                TextOp.addLabel("SWINT", "Press 'E' to interact");
                TextOp.addLabel("TESMENU", "Choice");
                TextOp.addLabel("TESTM1", "What happened?");
                TextOp.addLabel("TESTM2", "Not now.");
                TextOp.addLabel("PACKNOT", "Sweet: Did you bring the package?");
            end
        end)
        on_scene_ended_disconnect = events.on_scene_ended(function(sceneId)
            if project_player.is_playing then
                sweet_handler.on_unload();
            end
        end)
        on_project_stopped_disconnect = events.on_project_stopped(function()
            sweet_handler.on_unload();
        end)
    end,
    on_unload = function()
        if on_objective_started_disconnect then
            on_objective_started_disconnect()
        end
        if on_scene_started_disconnect then
            on_scene_started_disconnect()
        end
        if on_scene_ended_disconnect then
            on_scene_ended_disconnect()
        end
        if on_project_stopped_disconnect then
            on_project_stopped_disconnect()
        end
        sweet_handler.on_unload();
    end,
})
