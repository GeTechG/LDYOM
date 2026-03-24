local sweet_handler = require("sweet_handler")

local SCENE_ID = "1774287674"

return events.on_objective_started(function(index)
    if not project_player.is_playing or project_player.current_scene_id ~= SCENE_ID then return end

    if index == 1 or index == 5 then
        sweet_handler.on_interact()
    end
    if index == 2 or index == 4 or index == 7 then
        sweet_handler.say()
    end
    if index == 3 then
        sweet_handler.choice()
    end
    if index == 5 or index == 2 or index == 7 or index == 8 then
        PlayerOp.setControl(0, index == 5 or index == 8)
    end
end)
