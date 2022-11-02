local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class CharAnimationNode
---@field static table

---@type BaseNode | CharAnimationNode
local CharAnimationNode = class("CharAnimationNode", ed.BaseNode)
CharAnimationNode.static.name_ = "core.nodes.char_animation";
CharAnimationNode.static.type = ed.NodeType.Function;

---@param id integer
function CharAnimationNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pack = ffi.new("float[1]", {0});
    self.animation = ffi.new("float[1]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [2] = ed.Pin:new(id + 2, "core.pins.self", ed.PinType.In, ed.ValueType.Number),
        [5] = ed.Pin:new(id + 5, "animation.looped", ed.PinType.In, ed.ValueType.Boolean),
        [6] = ed.Pin:new(id + 6, "animation.smoothness", ed.PinType.In, ed.ValueType.Number),
        [7] = ed.Pin:new(id + 7, "general.time", ed.PinType.In, ed.ValueType.Number),
        [9] = ed.Pin:new(id + 9, "", ed.PinType.Out, ed.ValueType.Void),
        [10] = ed.Pin:new(id + 10, "core.pins.wait_end", ed.PinType.Out, ed.ValueType.Void)
    }
end

function CharAnimationNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    ImGui.text(loc.get(self.pins[2].name));
    ImNodes.endInputAttribute();

    local packsNames = ModelsService_getPacksNames();

    local chooseAnim = false;

    ImNodes.beginStaticAttribute(3);
    ImGui.setNextItemWidth(120);
    if (ImGui.combo(loc.get("animation.pack"), self.pack, packsNames)) then
        self.animation[0] = 0;
        chooseAnim = true;
    end
    ImNodes.endStaticAttribute();

    ImNodes.beginStaticAttribute(4);
    ImGui.setNextItemWidth(120);
    chooseAnim = chooseAnim or ImGui.combo(loc.get("animation.title"), self.animation, ModelsService_getAnimations()[packsNames[self.pack[0]+1]]);
    ImNodes.endStaticAttribute();

    if (chooseAnim) then
        if (not StreamingOp.hasAnimationLoaded(packsNames[self.pack[0]+1])) then
            StreamingOp.requestAnimation(packsNames[self.pack[0]+1]);
        end
        local anims = ModelsService_getAnimations()[packsNames[self.pack[0]+1]];
        TaskOp.playAnimNonInterruptable(PlayerOp.getChar(0), anims[self.animation[0]+1], packsNames[self.pack[0]+1], 10 * (1 - (self.pins[6].value[0] - 1.192092896e-07)), self.pins[5].value[0], false, false, false, 0);
    end

    ImNodes.beginInputAttribute(self.pins[5]);
    if not self.pins[5].connected then
        ImGui.setNextItemWidth(120);
        ImGui.checkbox(loc.get(self.pins[5].name), self.pins[5].value);
    else
        ImGui.text(loc.get(self.pins[5].name));
    end
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[6]);
    if not self.pins[6].connected then
        ImGui.setNextItemWidth(120);
        ImGui.dragFloat(loc.get(self.pins[6].name), self.pins[6].value, 0.01, 1.192092896e-07, 1);
    else
        ImGui.text(loc.get(self.pins[6].name));
    end
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[7]);
    if not self.pins[7].connected then
        ImGui.setNextItemWidth(120);
        ImGui.dragFloat(loc.get(self.pins[7].name), self.pins[7].value, 1, -1, 9999999999999999);
    else
        ImGui.text(loc.get(self.pins[7].name));
    end
    ImNodes.endInputAttribute();

    ImGui.endGroup();

    ImGui.sameLine();
    local inp_size = ImGui.getItemRectSize();

	ImGui.beginGroup();

    ImNodes.beginOutputAttribute(self.pins[9]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[9].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[9].name));
    ImNodes.endOutputAttribute();

    ImNodes.beginOutputAttribute(self.pins[10]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[10].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[10].name));
    ImNodes.endOutputAttribute();

    ImGui.endGroup();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function CharAnimationNode:execute(scene, tasklist, context)
    
    local queryId = math.random(1, 100000);

    assert(self.pins[2].connected, "Char required connect value");
    local char = context:getValue(scene, tasklist, self.id + 2, queryId)[0];
    local looped = context:getValue(scene, tasklist, self.id + 5, queryId)[0];
    local smoothness = context:getValue(scene, tasklist, self.id + 6, queryId)[0];
    local time = context:getValue(scene, tasklist, self.id + 7, queryId)[0];

    Tasker.addTask(tasklist, function()
        local packsNames = ModelsService_getPacksNames();

        if (not StreamingOp.hasAnimationLoaded(packsNames[self.pack[0]+1])) then
            StreamingOp.requestAnimation(packsNames[self.pack[0]+1]);
        end
        local anims = ModelsService_getAnimations()[packsNames[self.pack[0]+1]];
        TaskOp.playAnimNonInterruptable(char, anims[self.animation[0]+1], packsNames[self.pack[0]+1], 10 * (1 - (smoothness - 1.192092896e-07)), looped, false, false, false, time * 1000);

        coroutine.yield(1);

        while(CharOp.isPlayingAnim(char, anims[self.animation[0]+1])) do
            coroutine.yield(1);
        end

        context:callNodes(scene, tasklist, self.id + 10);
    end)
    
    context:callNodes(scene, tasklist, self.id + 9);
end

NodeEditor.registerNode("core.categories.char", CharAnimationNode);