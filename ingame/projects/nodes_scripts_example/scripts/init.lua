local sweet_handler = require("sweet_handler")

local SCENE_ID = "1774287674"

local disconnectors = {}

register_project_script({
    on_load = function()
        disconnectors[1] = require("on_objective_handler")
        disconnectors[2] = events.on_scene_started(function(sceneId)
            if project_player.current_scene_id == SCENE_ID then
                TextOp.addLabel("SWINT", "Press 'E' to interact")
                TextOp.addLabel("TESMENU", "Choice")
                TextOp.addLabel("TESTM1", "What happened?")
                TextOp.addLabel("TESTM2", "Not now.")
                TextOp.addLabel("PACKNOT", "Sweet: Did you bring the package?")
            end
        end)
        disconnectors[3] = events.on_scene_ended(function(sceneId)
            if project_player.is_playing then
                sweet_handler.on_unload()
            end
        end)
        disconnectors[4] = events.on_project_stopped(function()
            sweet_handler.on_unload()
        end)
    end,
    on_unload = function()
        for _, disconnect in ipairs(disconnectors) do
            disconnect()
        end
        disconnectors = {}
        sweet_handler.on_unload()
    end,
})
