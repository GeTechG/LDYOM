local sweet_handler = {}

local SWEET_ACTOR_ID = "cf3cc73e-610b-4707-910d-20bc2f2d8144"

local function get_sweet_ped()
    return ActorComponent.cast(scenes.find_entity(SWEET_ACTOR_ID):getComponent("actor")):getPedRef()
end

sweet_handler.on_interact = function()
    tasks.cancel("on_interact_sweet")
    tasks.run("on_interact_sweet", function()
        global_vars.set("interact_sweet", false)
        local sweet = get_sweet_ped()
        while not global_vars.get("interact_sweet") do
            local x, y, z = CharOp.getCoordinates(sweet)
            if CharOp.locateOnFoot2D(PlayerOp.getChar(0), x, y, 1.0, 1.0, false) then
                TextOp.print("SWINT", 100, 0)
                if PadOp.isKeyJustPressed(69) then
                    global_vars.set("interact_sweet", true)
                    local obj_index = project_player.current_objective_index
                    if obj_index == 1 or obj_index == 6 then
                        project_player:send_signal("default")
                    elseif obj_index == 5 then
                        sweet_handler.not_collect_say()
                    end
                end
            end
            tasks.yield()
        end
    end)
end

sweet_handler.say = function()
    TaskOp.playAnim(get_sweet_ped(), "IDLE_chat", "PED", 1, true, false, false, false, 3000)
end

sweet_handler.choice = function()
    tasks.cancel("sweet_choice")
    tasks.run("sweet_choice", function()
        local menu = MenuOp.create("TESMENU", 240, 180, 180, 1, true, false, 0)
        MenuOp.setColumn(menu, 0, "DUMMY", "TESTM1", "TESTM2", "DUMMY", "DUMMY", "DUMMY", "DUMMY", "DUMMY", "DUMMY",
            "DUMMY", "DUMMY", "DUMMY", "DUMMY")
        local choice
        repeat
            choice = MenuOp.getItemAccepted(menu)
            tasks.yield()
        until choice >= 0
        MenuOp.delete(menu)
        if choice == 0 then
            project_player:send_signal("help")
        elseif choice == 1 then
            project_player:send_signal("exit")
            PlayerOp.setControl(0, true)
        end
    end)
end

sweet_handler.not_collect_say = function()
    tasks.run("not_collect_say", function()
        sweet_handler.say()
        TextOp.printNow("PACKNOT", 2000, 0)
        tasks.sleep(2000)
        sweet_handler.on_interact()
    end)
end

sweet_handler.on_unload = function()
    tasks.cancel("on_interact_sweet")
    tasks.cancel("sweet_choice")
    tasks.cancel("not_collect_say")
end

return sweet_handler
