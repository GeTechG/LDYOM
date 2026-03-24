local sweet_handler = {}

local sweet_actor_id = "cf3cc73e-610b-4707-910d-20bc2f2d8144";

sweet_handler.on_interact = function()
    tasks.cancel("on_interact_sweet");
    tasks.run("on_interact_sweet", function()
        global_vars.set("interact_sweet", false);
        local sweet = ActorComponent.cast(scenes.find_entity(sweet_actor_id):getComponent("actor")):getPedRef()
        while not global_vars.get("interact_sweet") do
            local x, y, z = CharOp.getCoordinates(sweet);
            if CharOp.locateOnFoot2D(PlayerOp.getChar(0), x, y, 1.0, 1.0, false) then
                TextOp.print("SWINT", 100, 0);
                if (PadOp.isKeyJustPressed(69)) then
                    global_vars.set("interact_sweet", true);
                    if project_player.current_objective_index == 1 then
                        project_player:send_signal("default");
                    elseif project_player.current_objective_index == 5 then
                        sweet_handler.not_collect_say();
                    elseif project_player.current_objective_index == 6 then
                        project_player:send_signal("default");
                    end
                end
            end
            tasks.yield();
        end
    end)
end

sweet_handler.say = function()
    local sweet = ActorComponent.cast(scenes.find_entity(sweet_actor_id):getComponent("actor")):getPedRef()
    TaskOp.playAnim(sweet, "IDLE_chat", "PED", 1, true, false, false, false, 3000)
end

sweet_handler.choice = function()
    tasks.cancel("sweet_choice");
    tasks.run("sweet_choice", function()
        local menu = MenuOp.create("TESMENU", 240, 180, 180, 1, true, false, 0);
        MenuOp.setColumn(menu, 0, "DUMMY", "TESTM1", "TESTM2", "DUMMY", "DUMMY", "DUMMY", "DUMMY", "DUMMY", "DUMMY",
            "DUMMY", "DUMMY", "DUMMY", "DUMMY");
        while not (global_vars.get("selected_choice") >= 0) do
            global_vars.set("selected_choice", MenuOp.getItemAccepted(menu));
            tasks.yield();
        end
        MenuOp.delete(menu);
        if global_vars.get("selected_choice") == 0 then
            project_player:send_signal("help");
        elseif global_vars.get("selected_choice") == 1 then
            project_player:send_signal("exit");
            PlayerOp.setControl(0, true);
        end
        global_vars.set("selected_choice", -1);
    end)
end

sweet_handler.not_collect_say = function()
    tasks.run("not_collect_say", function()
        sweet_handler.say();
        TextOp.printNow("PACKNOT", 2000, 0);
        tasks.sleep(2000);
        sweet_handler.on_interact();
    end)
end

sweet_handler.on_unload = function()
    tasks.cancel("on_interact_sweet");
    tasks.cancel("sweet_choice");
    tasks.cancel("not_collect_say");
end

return sweet_handler
