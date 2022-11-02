local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class CharFollowPathNode
---@field static table

---@type BaseNode | CharFollowPathNode
local CharFollowPathNode = class("CharFollowPathNode", ed.BaseNode)
CharFollowPathNode.static.name_ = "core.nodes.char_follow_path";
CharFollowPathNode.static.type = ed.NodeType.Function;

---@param id integer
function CharFollowPathNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.path = {};
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [2] = ed.Pin:new(id + 2, "core.pins.self", ed.PinType.In, ed.ValueType.Number),
        [3] = ed.Pin:new(id + 3, "follow_path_actor_objective.move_type", ed.PinType.In, ed.ValueType.Number),
        [4] = ed.Pin:new(id + 4, "follow_path_actor_objective.path_type", ed.PinType.In, ed.ValueType.Number),
        [5] = ed.Pin:new(id + 5, "follow_path_actor_objective.execute_time", ed.PinType.In, ed.ValueType.Number),
        [7] = ed.Pin:new(id + 7, "", ed.PinType.Out, ed.ValueType.Void),
        [8] = ed.Pin:new(id + 8, "core.pins.wait_end", ed.PinType.Out, ed.ValueType.Void)
    }
end

function CharFollowPathNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    ImGui.text(loc.get(self.pins[2].name));
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[3]);
    if not self.pins[3].connected then
        ImGui.setNextItemWidth(120);
        ImGui.sliderInt(loc.get(self.pins[3].name), self.pins[3].value, 0, 2, loc.getArray("follow_path_actor_objective.move_states")[self.pins[3].value[0]+1]);
    else
        ImGui.text(loc.get(self.pins[3].name));
    end
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[4]);
    if not self.pins[4].connected then
        ImGui.setNextItemWidth(120);
        ImGui.sliderInt(loc.get(self.pins[4].name), self.pins[4].value, 0, 2, loc.getArray("follow_path_actor_objective.path_types")[self.pins[4].value[0]+1]);
    else
        ImGui.text(loc.get(self.pins[4].name));
    end
    ImNodes.endInputAttribute();

    if self.pins[4].value[0] > 0 or self.pins[4].connected then
        ImNodes.beginInputAttribute(self.pins[5]);
        if not self.pins[5].connected then
            ImGui.setNextItemWidth(120);
            ImGui.dragFloat(loc.get(self.pins[5].name), self.pins[5].value, 0.1, 0, 99999999999999999);
        else
            ImGui.text(loc.get(self.pins[5].name));
        end
        ImNodes.endInputAttribute();
    end

    ImNodes.beginStaticAttribute(6);
    if (ImGui.button(loc.get("general.record_path"), 100, 20)) then
        editByPlayerActorLuaPath(self.path);
    end
    ImNodes.endStaticAttribute();

    ImGui.endGroup();

    ImGui.sameLine();
    local inp_size = ImGui.getItemRectSize();

	ImGui.beginGroup();

    ImNodes.beginOutputAttribute(self.pins[7]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[7].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[7].name));
    ImNodes.endOutputAttribute();

    ImNodes.beginOutputAttribute(self.pins[8]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[8].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[8].name));
    ImNodes.endOutputAttribute();

    ImGui.endGroup();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function CharFollowPathNode:execute(scene, tasklist, context)
    local move_states = {4, 6, 7};
    
    local queryId = math.random(1, 100000);

    assert(self.pins[2].connected, "Char required connect value");
    local char = context:getValue(scene, tasklist, self.id + 2, queryId)[0];
    local move_type = context:getValue(scene, tasklist, self.id + 3, queryId)[0];
    local path_type = context:getValue(scene, tasklist, self.id + 4, queryId)[0];
    local execute_time = context:getValue(scene, tasklist, self.id + 5, queryId)[0];

    Tasker.addTask(tasklist, function()
        local execute = true;
		local step = 1;
		local index = 1;
		local startTime = CTimer_snTimeInMilliseconds();

        while (execute) do

			local x = self.path[index][0];
			local y = self.path[index][1];
			local z = self.path[index][2];
			TaskOp.goStraightToCoord(char, x, y, z, move_states[move_type+1], -1);

			local timeCondition = path_type == 0 or CTimer_snTimeInMilliseconds() - startTime < execute_time * 1000;
			local reachDistance = 1;
            if (move_type == 0) then
                reachDistance = 0.5;
            end
            local charX, charY, charZ = CharOp.getCoordinates(char);
			while (DistanceBetweenPoints(x, y, z, charX, charY, charZ) > reachDistance and timeCondition) do
				charX, charY, charZ = CharOp.getCoordinates(char);
                timeCondition = path_type == 0 or CTimer_snTimeInMilliseconds() - startTime < execute_time * 1000;
				coroutine.yield(1);
            end

			index = index + step;

			if (not timeCondition) then
				execute = false;
            end

            if (path_type == 0) then
                if (index == #self.path+1) then
					execute = false;
				end
            elseif (path_type == 1) then
                if (index == #self.path+1) then
					index = 1;
				end
            elseif (path_type == 2) then
                if (index == #self.path+1) then
					step = -1;
					index = index - 2;
                elseif (index <= 0) then
                    step = 1;
					index = index + 2;
				end
            end

		end

        context:callNodes(scene, tasklist, self.id + 8);
    end)
    
    context:callNodes(scene, tasklist, self.id + 7);
end

NodeEditor.registerNode("core.categories.char", CharFollowPathNode);