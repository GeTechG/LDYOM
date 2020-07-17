sNodes = {}
-- Really dumb data structure provided for the example.
-- Note that we storing sNodes.links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
function COLOR_U32(r, g, b, a)
    a = a or 255
    return imgui.ColorConvertFloat4ToU32(imgui.ImVec4(r/255,g/255,b/255,a/255))
end

function PlusVec4(v1,v2)
    return imgui.ImVec4(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z,v1.w+v2.w)
end

-- function multVec2(vec,v)
--     return imgui.ImVec2(vec.x*v,vec.y*v)
-- end

local grid_magnit = 32
local magnit_onC = new.bool()
function magnit(val)
    if magnit_onC[0] then
        return math.floor(val/grid_magnit+0.5)*grid_magnit
    else
        return val
    end
end

--States mouse
STATE_NONE = 0
STATE_INPUT = 1
STATE_OUTPUT = 2

local NODE_SLOT_RADIUS = 5;
local NODE_WINDOW_PADDING = imgui.ImVec2(8, 8);
local NODE_TITLE_PADDING = imgui.ImVec2(0, 20);

node_types = fe.enum([[event,func,fork,loop,var,exit]])

function getTypeNode(type)
    local color
    local text
    if type[0] == node_types.event then
        color = imgui.ImVec4(0.18, 0.55, 0, 1)
        text = faicons.ICON_SIGN_IN_ALT
    elseif type[0] == node_types.fork then
        color = imgui.ImVec4(1.00, 0.26, 0.12, 1)
        text = faicons.ICON_UTENSIL_FORK
    elseif type[0] == node_types.loop then
        color = imgui.ImVec4(0.95, 0.64, 0.05, 1)
        text = faicons.ICON_REPEAT
    elseif type[0] == node_types.var then
        color = imgui.ImVec4(0.00, 0.38, 0.65, 1)
        text = faicons.ICON_ARCHIVE
    elseif type[0] == node_types.func then
        color = imgui.ImVec4(0.37, 0.21, 0.69, 1)
        text = faicons.ICON_FUNCTION
    elseif type[0] == node_types.exit then
        color = imgui.ImVec4(0.85, 0.05, 0.12, 1)
        text = faicons.ICON_SIGN_OUT_ALT
    end
    return color,text
end

function getTypeSocket(type)
    if tostring(type) == 'wire' then
        return imgui.GetStyle().Colors[imgui.Col.Text]
    elseif getTypeValue(tostring(type)) == 0 then
        return imgui.ImVec4(0.09, 0.52, 0.82,1)
    elseif getTypeValue(tostring(type)) == 1 then
        return imgui.ImVec4(0.79, 0.05, 0.06,1)
    elseif getTypeValue(tostring(type)) == 2 then
        return imgui.ImVec4(0.27, 0.75, 0.33,1)
    end
end

function getTypeValue(value)
    if value == 'ctype<int [1]>' then
        return 0
    elseif value == 'ctype<float [1]>' then
        return 0
    elseif value == 'ctype<bool [1]>' then
        return 1
    elseif string.sub(value,7,10) == 'char' then
        return 2
    end
end

sNodes.nodes = {};
sNodes.links = {};
sNodes.vars = {};
sNodes.real_vars = {};
sNodes.names_vars = {}
local inited = false;
local scrolling = imgui.ImVec2(0.0, 0.0);
local node_hovered_in_scene = new.int(-1)
local show_gridC = new.bool(true);
local state_mouse = STATE_NONE
local node_select = nil
local slot_select = nil
local connecting = false
local curr_var = new.int(0)
sNodes.white_list = {"main"}

-- Dummy
local Node = class('Node')
Node.static.Name = new.char[65]('Node')
Node.static.Type = new.ImU16(node_types.event)
function Node:initialize(pos, input, output)
    self.ID = new.int(#sNodes.nodes)
    self.Pos = pos
    self.Size = imgui.ImVec2()
    self.Inputs = input
    self.Outputs = output
    self.OutputMax = new.float(0)
end
function Node:GetInputSlotPos(y)
    return imgui.ImVec2(magnit(self.Pos.x)+NODE_SLOT_RADIUS*2, y)
end
function Node:GetOutputSlotPos(y)
    return imgui.ImVec2(magnit(self.Pos.x) + self.Size.x + NODE_SLOT_RADIUS*4, y)
end
function Node:Overlaps(rMin,rMax)
    local Max = self.Pos + self.Size
    return rMin.y <  Max.y and rMax.y >  self.Pos.y and rMin.x <  Max.x and rMax.x >  self.Pos.x
end
function Node:renderSocket(socket)
    local value
    if self.class.Type[0] == node_types.var and not socket.value then
        value = self.class.static.value
    else
        value = socket.value
    end
    if socket.type == 'wire' or socket.type == 'var' or socket.notEditable[0] or (socket.link or type(socket.link) == 'table' and #socket.link > 0) then
        imgui.Text(langt[ffi.string(socket.name)] or '')
    elseif socket.type == 'InputText' then
        imgui.PushIDStr(langt[ffi.string(socket.name)] or '')
        imgui.Text(langt[ffi.string(socket.name)] or '')
        imgui[socket.type]('',value,ffi.sizeof(value))
        imgui.PopID()
    elseif socket.type == 'Combo' then
        imgui.PushIDStr(langt[ffi.string(socket.name)] or '')
        imgui.Text(langt[ffi.string(socket.name)] or '')
        local ext = loadstring('return '..socket.ext)()
        imgui[socket.type]('',value,new('const char* const [?]',#ext,ext),#ext)
        imgui.PopID()
    elseif socket.type == 'Bool' then
        mimgui_addons.ToggleButton(langt[ffi.string(socket.name)] or '',value)
    else
        imgui.PushIDStr(langt[ffi.string(socket.name)] or '')
        imgui.Text(langt[ffi.string(socket.name)] or '')
        imgui[socket.type]('',value)
        imgui.PopID()
    end
end
function Node:deleteConnections()
    for k,v in pairs(self.Inputs) do
        if v.link then
            local link = sNodes.links[v.link]
            local node2 = sNodes.nodes[link.OutputIdx]
            local socket = node2.Outputs[link.OutputSlot]
            socket.link[link.OutputSlotNum] = nil
            sNodes.links[v.link] = nil
            v.link = nil
        end
    end
    for k,v in pairs(self.Outputs) do
        for k2,v2 in pairs(v.link or {}) do
            if v.link[k2] then
                local link = sNodes.links[v.link[k2]]
                local node2 = sNodes.nodes[link.InputIdx]
                local socket = node2.Inputs[link.InputSlot]
                socket.link = nil
                sNodes.links[v.link[k2]] = nil
                v.link[k2] = nil
            end
        end
    end
end
function Node:getInputValue(idx)
    if self.Inputs[idx].link then
        local link = sNodes.links[self.Inputs[idx].link].OutputIdx
        local node = sNodes.nodes[link.OutputIdx]
        if node.Type == node_types.var then
            return sNodes.real_vars[node.class.name]
        else
            return sNodes.nodes[link.OutputIdx].Outputs[link.OutputSlot].value
        end
    else
        return self.Inputs[idx].value
    end
end
bitser.registerClass(Node)

ffi.cdef[[
    struct NodeLink {
        int InputIdx;
        int InputSlot;
        int OutputIdx;
        int OutputSlot;
        int OutputSlotNum;
    }
]]
bitser.register('NodeLink',ffi.typeof(ffi.new('struct NodeLink')))

local Socket = class('Socket')
function Socket:initialize(name, type, value, ext, notEditable)
    self.name = new.char[65](name)
    self.type = type
    self.link = nil
    self.ext = ext
    if type ~= 'wire' then
        self.value = value
        self.notEditable = new.bool(notEditable or false)
    else
        self.notEditable = new.bool(false) 
        self.value = new('void*')
    end
end
bitser.registerClass(Socket)

local classVar = class('Variable')
function classVar:initialize()
    self.name = new.char[65](langt['variable']..' #'..#sNodes.names_vars+1)
    self.typeValue = new.ImU8(0)
    self.value = new.float(0)
end
bitser.registerClass(classVar)

sNodes.Nodes = {
    main = {},
    storyline = {}
}

local node = Node:subclass('Start')
node.static.Name = new.char[65]('nodeStart')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {},
        {
            Socket('','wire')
        }
    )
end
function node:play()
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            sNodes.nodes[sNodes.links[v].InputIdx]:play()
        end
    end
end
sNodes.Nodes.main[#sNodes.Nodes.main+1] = node
bitser.registerClass(node)

node = Node:subclass('MissionComplete')
node.static.Name = new.char[65]('nodeMissionComplete')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    local pack_idx
    for i,v in ipairs(vr.temp_var.list_name_mission_packs) do
        if ffi.string(vr.storyline.missPack) == ffi.string(v) then
            pack_idx = i
        end
    end 
    Node.initialize(self,
        pos,
        {
            Socket('mission', 'Combo', new.int(0), 'vr.temp_var.list_name_missions['..pack_idx..']'),
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play()
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            sNodes.nodes[sNodes.links[v].InputIdx]:play()
        end
    end
end
sNodes.Nodes.storyline[#sNodes.Nodes.storyline+1] = node
bitser.registerClass(node)

node = Node:subclass('MainCycle')
node.static.Name = new.char[65]('nodeMainCycle')
node.static.Type = new.ImU16(node_types.loop)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {},
        {
            Socket('','wire'),
        }
    )
end
function node:play()
    while mp.storylineOn do
        wait(0)
        if self.Outputs[1].link then
            for k,v in pairs(self.Outputs[1].link) do
                sNodes.nodes[sNodes.links[v].InputIdx]:play()
            end
        end
    end
end
sNodes.Nodes.main[#sNodes.Nodes.main+1] = node
bitser.registerClass(node)

node = Node:subclass('GotoMission')
node.static.Name = new.char[65]('nodeGotoMission')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    local pack_idx
    for i,v in ipairs(vr.temp_var.list_name_mission_packs) do
        if ffi.string(vr.storyline.missPack) == ffi.string(v) then
            pack_idx = i
        end
    end
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('mission', 'Combo', new.int(0), 'vr.temp_var.list_name_missions['..pack_idx..']'),
        },
        {
            Socket('nodeMissionComplete','wire'),
            Socket('mission','',new.int(0),nil,true),
        }
    )
end
function node:play()
    local value = self:getInputValue(2)
    local num_pack
	for i,v in ipairs(vr.temp_var.list_name_mission_packs) do
		if ffi.string(v) == ffi.string(vr.storyline.missPack) then
			num_pack = i
		end
	end
	local missions_list = vr.temp_var.list_name_missions[num_pack]
    mp.playMission(missions_list[value[0]+1])
    mp.last_miss = value[0]+1
    for k,v in pairs(sNodes.nodes) do
        if v.class.name == 'MissionComplete' then
            if v:getInputValue(1)[0] == value[0] then
                mp.thread[#mp.thread+1] = lua_thread.create(function(v)
                v:play()
                end,v)
            end
        end
    end
    self.Outputs[2].value[0] = value[0]
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            sNodes.nodes[sNodes.links[v].InputIdx]:play()
        end
    end
end
sNodes.Nodes.storyline[#sNodes.Nodes.storyline+1] = node
bitser.registerClass(node)

node = Node:subclass('endStoryline')
node.static.Name = new.char[65]('nodeEndStoryline')
node.static.Type = new.ImU16(node_types.exit)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire')
        },
        {}
    )
end
function node:play()
    mp.storylineOn = false
end
sNodes.Nodes.storyline[#sNodes.Nodes.storyline+1] = node
bitser.registerClass(node)

node = Node:subclass('ActivateСheckpoint')
node.static.Name = new.char[65]('nodeActivСheck')
node.static.Type = new.ImU16(node_types.func)
node.static.Ext = 'vr.temp_var.list_name_storylineCheckpoints'
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('checkpoint','Combo', new.int(0)),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play()
    local check = self:getInputValue(2)[0]+1
    local xx,yy,zz = vr.storyline.list_checkpoints[check].data.pos[0],vr.storyline.list_checkpoints[check].data.pos[1],vr.storyline.list_checkpoints[check].data.pos[2]
    mp.storyCheck[check] = addSpriteBlipForCoord(xx,yy,zz,vr.storyline.list_checkpoints[check].data.iconMarker[0])
    changeBlipColour(mp.storyCheck[check],vr.storyline.list_checkpoints[check].data.colorBlip[0])
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            sNodes.nodes[sNodes.links[v].InputIdx]:play()
        end
    end
end
sNodes.Nodes.storyline[#sNodes.Nodes.storyline+1] = node
bitser.registerClass(node)

classNodeVar = Node:subclass('VariableGetSet')
classNodeVar.static.Type = new.ImU16(node_types.var)
function classNodeVar:initialize(var,pos,set)
    self.set = new.bool(set or false)
    self.var = var
    if set then
        local type
        local value
        if sNodes.vars[var].typeValue[0] == 0 then
            type = 'InputFloat'
            value = new.float(0)
        elseif sNodes.vars[var].typeValue[0] == 1 then
            type = 'Bool'
            value = new.bool(false)
        elseif sNodes.vars[var].typeValue[0] == 2 then
            type = 'InputText'
            value = new.char[128]()
        end
        Node.initialize(self,
            pos,
            {
                Socket('','wire'),
                Socket('value',type,value),
            },
            {
                Socket('','wire')
            }
        )
    else
        Node.initialize(self,
            pos,
            {},
            {
                Socket('','var',nil,nil,false)
            }
        )
    end
end
bitser.registerClass(classNodeVar)

imgui.OnFrame(function() return (not isGamePaused()) and vr.nodeEditor[0] end,
function()
    imgui.SetNextWindowSize(imgui.ImVec2(700, 600), imgui.Cond.FirstUseEver);
    if (not imgui.Begin(faicons.ICON_PROJECT_DIAGRAM..' '..langt['nodeEditor'], opened)) then
        imgui.End();
        return;
    end

    local io = imgui.GetIO();
    local node_selected = -1;

    -- if (not inited) then
    --     inited = true;
    -- end
    imgui.BeginChild('Panel',imgui.ImVec2(150,0))
    imgui.Text(langt['variables'])
    imgui.PushIDStr(langt['variables'])
    imgui.SetNextItemWidth(150)
    imgui.ListBoxStr_arr('', curr_var, new('const char* const [?]',#sNodes.names_vars,sNodes.names_vars),#sNodes.names_vars,15)
    imgui.PopID()
    if imgui.Button(langt['add']) then
        local new_node = classVar()
        sNodes.vars[#sNodes.vars+1] = new_node
        sNodes.names_vars[#sNodes.names_vars+1] = new_node.name
    end
    if #sNodes.vars > 0 then
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            local find = false
            for k,v in pairs(sNodes.nodes) do
                if v.var then
                    if v.var == curr_var[0]+1 then
                        v:deleteConnections()
                        sNodes.nodes[k] = nil
                        find = true
                    end
                    if v.var > curr_var[0]+1 then
                        v.var = v.var - 1
                    end
                end
            end
            table.remove(sNodes.vars,curr_var[0]+1)
            table.remove(sNodes.names_vars,curr_var[0]+1)
            if curr_var[0] > 0 then curr_var[0] = curr_var[0]-1 end
        else
            imgui.Text(langt['settings'])
            imgui.BeginChild(langt['settings'],nil,true)
            imgui.InputText(langt['name'], sNodes.names_vars[curr_var[0]+1], 65)
            if imgui.SliderScalar(langt['type'],imgui.DataType.U8,sNodes.vars[curr_var[0]+1].typeValue,new.ImU8(0),new.ImU8(2),vr.temp_var.typesValue[sNodes.vars[curr_var[0]+1].typeValue[0]+1]) then
                for k,v in pairs(sNodes.nodes) do
                    if v.var == curr_var[0]+1 then
                        v:deleteConnections()
                        if v.set[0] then
                            if sNodes.vars[curr_var[0]+1].typeValue[0] == 0 then
                                v.Inputs[2].type = 'InputFloat'
                                v.Inputs[2].value = new.float(0)
                            elseif sNodes.vars[curr_var[0]+1].typeValue[0] == 1 then
                                v.Inputs[2].type = 'Bool'
                                v.Inputs[2].value = new.bool(false)
                            elseif sNodes.vars[curr_var[0]+1].typeValue[0] == 2 then
                                v.Inputs[2].type = 'InputText'
                                v.Inputs[2].value = new.char[128]()
                            end
                        end
                    end
                end
                if sNodes.vars[curr_var[0]+1].typeValue[0] == 0 then
                    sNodes.vars[curr_var[0]+1].value = new.float(0)
                elseif sNodes.vars[curr_var[0]+1].typeValue[0] == 1 then
                    sNodes.vars[curr_var[0]+1].value = new.bool(false)
                elseif sNodes.vars[curr_var[0]+1].typeValue[0] == 2 then
                    sNodes.vars[curr_var[0]+1].value = new.char[128]()
                end
            end
            if sNodes.vars[curr_var[0]+1].typeValue[0] == 0 then
                imgui.InputFloat(langt['value'], sNodes.vars[curr_var[0]+1].value)
            elseif sNodes.vars[curr_var[0]+1].typeValue[0] == 1 then
                mimgui_addons.ToggleButton(langt['value'], sNodes.vars[curr_var[0]+1].value)
            elseif sNodes.vars[curr_var[0]+1].typeValue[0] == 2 then
                imgui.InputText(langt['value'],sNodes.vars[curr_var[0]+1].value,ffi.sizeof(sNodes.vars[curr_var[0]+1].value))
            end
            imgui.EndChild()
        end
    end

    imgui.EndChild()

    imgui.SameLine();
    imgui.BeginGroup();

    -- Create our child canvas
    imgui.Text(langt['holdPlease'].." (%.2f,%.2f)", scrolling.x, scrolling.y);
    imgui.Checkbox(langt['showGrid'], show_gridC);
    imgui.SameLine();
    imgui.Checkbox(langt['snapGrid'], magnit_onC);
    imgui.PushStyleVarVec2(imgui.StyleVar.FramePadding, imgui.ImVec2(1, 1));
    imgui.PushStyleVarVec2(imgui.StyleVar.WindowPadding, imgui.ImVec2(0, 0));
    imgui.PushStyleColor(imgui.Col.ChildBg, PlusVec4(imgui.GetStyle().Colors[imgui.Col.ChildBg] , imgui.ImVec4(0,0,0,0)));
    imgui.BeginChild("scrolling_region", imgui.ImVec2(0, 0), true, imgui.WindowFlags.NoScrollbar + imgui.WindowFlags.NoMove);
    imgui.PopStyleVar()
    imgui.PushItemWidth(120.0);

    local offset = imgui.GetCursorScreenPos() + scrolling;
    local draw_list = imgui.GetWindowDrawList();
    -- Display grid
    local canvas_sz = imgui.GetWindowSize();
    local win_pos = imgui.GetCursorScreenPos();
    if (show_gridC[0]) then
        local GRID_COLOR = imgui.ColorConvertFloat4ToU32(PlusVec4(imgui.GetStyle().Colors[imgui.Col.Text] , imgui.ImVec4(0,0,0,-0.84)));
        local GRID_SZ = 32;
        for x = scrolling.x % GRID_SZ, canvas_sz.x, GRID_SZ do
            draw_list:AddLine(imgui.ImVec2(x, 0.0) + win_pos, imgui.ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
        end
        for y = scrolling.y % GRID_SZ, canvas_sz.y, GRID_SZ do
            draw_list:AddLine(imgui.ImVec2(0.0, y) + win_pos, imgui.ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
        end
    end

    draw_list:ChannelsSplit(2);

    -- Display sNodes.nodes
    for node_idx,node in pairs(sNodes.nodes) do
        if node:Overlaps(-scrolling,-scrolling+canvas_sz) then
            imgui.PushIDInt(node.ID[0]);
            node_rect_min = offset + imgui.ImVec2(magnit(node.Pos.x),magnit(node.Pos.y));

            -- Display node contents first
            draw_list:ChannelsSetCurrent(1); -- Foreground
            old_any_active = imgui.IsAnyItemActive();
            imgui.SetCursorScreenPos(NODE_TITLE_PADDING + node_rect_min + NODE_WINDOW_PADDING + imgui.ImVec2(0,NODE_WINDOW_PADDING.y) + imgui.ImVec2(NODE_SLOT_RADIUS*3,0));
            imgui.BeginGroup()
            imgui.BeginGroup(); -- Lock horizontal position
            for slot_idx = 1,#node.Inputs do
                local y = imgui.GetCursorScreenPos().y
                imgui.BeginGroup()
                node:renderSocket(node.Inputs[slot_idx])
                imgui.EndGroup()
                node.Inputs[slot_idx].socketY = y + (imgui.GetItemRectSize().y / 2) - offset.y
            end
            imgui.EndGroup();
            imgui.SameLine(0,15)
            local out_start = imgui.GetCursorScreenPos()
            imgui.BeginGroup(); -- Lock horizontal position
            for slot_idx = 1,#node.Outputs do
                imgui.SetCursorScreenPos(imgui.ImVec2(out_start.x, imgui.GetCursorScreenPos().y))
                if node.OutputMax[0] > 0 and node.Outputs[slot_idx].socketWidth then
                    imgui.SetCursorScreenPos(imgui.GetCursorScreenPos() + imgui.ImVec2(node.OutputMax[0]-node.Outputs[slot_idx].socketWidth,0))
                end
                local y = imgui.GetCursorScreenPos().y
                imgui.BeginGroup()
                node:renderSocket(node.Outputs[slot_idx])
                imgui.EndGroup()
                node.Outputs[slot_idx].socketY = y + (imgui.GetItemRectSize().y / 2) - offset.y
                node.Outputs[slot_idx].socketWidth = imgui.GetItemRectSize().x
                node.OutputMax[0] = math.max(imgui.GetItemRectSize().x,node.OutputMax[0])
            end
            imgui.EndGroup();
            imgui.EndGroup();
            --local inputs_size = imgui.GetItemRectSize()

            -- Save the size of what we have emitted and whether any of the widgets are being used
            node_widgets_active = not old_any_active and imgui.IsAnyItemActive()
            node.Size = imgui.GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING + NODE_TITLE_PADDING + imgui.ImVec2(0,NODE_WINDOW_PADDING.y);
            if node.class.Type[0] == node_types.var then
                node.Size.x = math.max(node.Size.x,imgui.CalcTextSize('## '..ffi.string(sNodes.vars[node.var].name)).x+NODE_WINDOW_PADDING.x*2)
            else
                node.Size.x = math.max(node.Size.x,imgui.CalcTextSize('## '..(langt[ffi.string(node.class.Name)] or ffi.string(node.class.Name))).x+NODE_WINDOW_PADDING.x*2)
            end
            local node_rect_max = node_rect_min + node.Size + imgui.ImVec2(NODE_SLOT_RADIUS*6,0);
            local title_rect_max = node_rect_min + imgui.ImVec2(node.Size.x,NODE_TITLE_PADDING.y) + imgui.ImVec2(0,NODE_WINDOW_PADDING.y) + imgui.ImVec2(NODE_SLOT_RADIUS*6,0);

            -- Display node box
            draw_list:ChannelsSetCurrent(0); -- Background
            imgui.SetCursorScreenPos(node_rect_min+imgui.ImVec2(NODE_SLOT_RADIUS*3,0));
            imgui.InvisibleButton("node", node.Size);
            if (imgui.IsItemHovered()) then
                node_hovered_in_scene = node.ID;
            end
            local node_moving_active = imgui.IsItemActive();
            if (node_widgets_active or node_moving_active) then
                node_selected = node.ID;
            end
            if (node_moving_active and imgui.IsMouseDragging(0)) then
                node.Pos = node.Pos + io.MouseDelta
            end
            local node_bg_color = fif(node_hovered_in_scene == node.ID, PlusVec4(imgui.GetStyle().Colors[imgui.Col.WindowBg], imgui.ImVec4(0.09,0.09,0.09,0)), PlusVec4(imgui.GetStyle().Colors[imgui.Col.WindowBg], imgui.ImVec4(0.07,0.07,0.07,0)));
            draw_list:AddRectFilled(node_rect_min, node_rect_max, imgui.ColorConvertFloat4ToU32(node_bg_color), imgui.GetStyle().FrameRounding);
            local color,text = getTypeNode(node.class.Type)
            if node.class.Type[0] == node_types.var then
                text = text..' '..ffi.string(sNodes.vars[node.var].name)
            else
                text = text..' '..(langt[ffi.string(node.class.Name)] or ffi.string(node.class.Name))
            end
            draw_list:AddRectFilled(node_rect_min, title_rect_max, imgui.ColorConvertFloat4ToU32(color), imgui.GetStyle().FrameRounding, imgui.DrawCornerFlags.Top);
            draw_list:AddText(node_rect_min+NODE_WINDOW_PADDING, imgui.ColorConvertFloat4ToU32(imgui.ImVec4(1,1,1,1)), text)
            if imgui.GetStyle().WindowBorderSize == 1.0 then
                draw_list:AddRect(node_rect_min, node_rect_max, imgui.ColorConvertFloat4ToU32(imgui.GetStyle().Colors[imgui.Col.Border]), imgui.GetStyle().FrameRounding);
            end
            for slot_idx = 1,#node.Inputs do
                imgui.PushIDInt(slot_idx)
                local size_dot_area = imgui.ImVec2(NODE_SLOT_RADIUS,NODE_SLOT_RADIUS)
                imgui.SetCursorScreenPos(offset + node:GetInputSlotPos(node.Inputs[slot_idx].socketY) - size_dot_area);
                imgui.InvisibleButton(tostring(slot_idx), size_dot_area * 2)
                if imgui.IsItemHovered() and state_mouse == STATE_NONE then
                    state_mouse = STATE_INPUT
                    node_dragging = node_idx
                    slot_dragging = slot_idx
                    if imgui.IsMouseReleased(1) and node.Inputs[slot_idx].link then
                        local link = sNodes.links[node.Inputs[slot_idx].link]
                        local node2 = sNodes.nodes[link.OutputIdx]
                        local socket = node2.Outputs[link.OutputSlot]
                        socket.link[link.OutputSlotNum] = nil
                        sNodes.links[node.Inputs[slot_idx].link] = nil
                        node.Inputs[slot_idx].link = nil
                    end
                end
                local color
                if node.Inputs[slot_idx].type == 'wire' then
                    color = getTypeSocket('wire')
                else
                    if node.class.Type[0] == node_types.var and not node.Inputs[slot_idx].value then
                        color = getTypeSocket(ffi.typeof(sNodes.vars[node.var].value))
                    else
                        color = getTypeSocket(ffi.typeof(node.Inputs[slot_idx].value))
                    end
                end
                if node.Inputs[slot_idx].link then
                    draw_list:AddCircleFilled(offset + node:GetInputSlotPos(node.Inputs[slot_idx].socketY), NODE_SLOT_RADIUS, imgui.ColorConvertFloat4ToU32(color));
                else
                    draw_list:AddCircle(offset + node:GetInputSlotPos(node.Inputs[slot_idx].socketY), NODE_SLOT_RADIUS, imgui.ColorConvertFloat4ToU32(color),12,3);
                end
                imgui.PopID()
            end
            for slot_idx = 1,#node.Outputs do
                imgui.PushIDInt(slot_idx)
                local size_dot_area = imgui.ImVec2(NODE_SLOT_RADIUS,NODE_SLOT_RADIUS)
                imgui.SetCursorScreenPos(offset + node:GetOutputSlotPos(node.Outputs[slot_idx].socketY) - size_dot_area);
                imgui.InvisibleButton(tostring(slot_idx), size_dot_area * 2)
                if imgui.IsItemHovered() and state_mouse == STATE_NONE then
                    state_mouse = STATE_OUTPUT
                    node_dragging = node_idx
                    slot_dragging = slot_idx
                end
                local color
                if node.Outputs[slot_idx].type == 'wire' then
                    color = getTypeSocket('wire')
                else
                    if node.class.Type[0] == node_types.var and not node.Outputs[slot_idx].value  then
                        color = getTypeSocket(ffi.typeof(sNodes.vars[node.var].value))
                    else
                        color = getTypeSocket(ffi.typeof(node.Outputs[slot_idx].value))
                    end
                end
                if node.Outputs[slot_idx].link and #node.Outputs[slot_idx].link > 0 then
                    draw_list:AddCircleFilled(offset + node:GetOutputSlotPos(node.Outputs[slot_idx].socketY), NODE_SLOT_RADIUS, imgui.ColorConvertFloat4ToU32(color));
                else
                    draw_list:AddCircle(offset + node:GetOutputSlotPos(node.Outputs[slot_idx].socketY), NODE_SLOT_RADIUS, imgui.ColorConvertFloat4ToU32(color),12,3);
                end
                imgui.PopID()
            end
            imgui.PopID();
        end
    end

    -- Display sNodes.links
    for k,v in pairs(sNodes.links) do
        local node_inp = sNodes.nodes[v.InputIdx]
        local node_out = sNodes.nodes[v.OutputIdx]
        local color
        if node_out.Outputs[v.OutputSlot].type == 'wire' then
            color = getTypeSocket('wire')
        else
            if node_out.class.Type[0] == node_types.var and not node_out.Outputs[v.OutputSlot].value then
                color = getTypeSocket(ffi.typeof(sNodes.vars[node_out.var].value))
            else
                color = getTypeSocket(ffi.typeof(node_out.Outputs[v.OutputSlot].value))
            end
        end
        local p1 = offset + node_out:GetOutputSlotPos(node_out.Outputs[v.OutputSlot].socketY);
        local p2 = offset + node_inp:GetInputSlotPos(node_inp.Inputs[v.InputSlot].socketY);
        draw_list:AddBezierCurve(p1, p1 + imgui.ImVec2(50, 0), p2 + imgui.ImVec2(-50, 0), p2, imgui.ColorConvertFloat4ToU32(color), 3.0);
    end

    if state_mouse ~= STATE_NONE then
        if imgui.IsMouseDragging(0,0.1) then
            connecting = true
            local color
            if state_mouse == STATE_OUTPUT then
                if sNodes.nodes[node_dragging].Outputs[slot_dragging].type == 'wire' then
                    color = getTypeSocket('wire')
                else
                    if sNodes.nodes[node_dragging].class.Type[0] == node_types.var and not sNodes.nodes[node_dragging].Outputs[slot_dragging].value then
                        color = getTypeSocket(ffi.typeof(sNodes.vars[sNodes.nodes[node_dragging].var].value))
                    else
                        color = getTypeSocket(ffi.typeof(sNodes.nodes[node_dragging].Outputs[slot_dragging].value))
                    end
                end
                local slot_pos = offset + sNodes.nodes[node_dragging]:GetOutputSlotPos(sNodes.nodes[node_dragging].Outputs[slot_dragging].socketY)
                local mouse_pos = imgui.GetMousePos()
                draw_list:AddBezierCurve(slot_pos, slot_pos + imgui.ImVec2(50, 0), mouse_pos + imgui.ImVec2(-50, 0), mouse_pos, imgui.ColorConvertFloat4ToU32(color), 3.0);
            else
                if sNodes.nodes[node_dragging].Inputs[slot_dragging].type == 'wire' then
                    color = getTypeSocket('wire')
                else
                    if sNodes.nodes[node_dragging].class.Type[0] == node_types.var and not sNodes.nodes[node_dragging].Inputs[slot_dragging].value then
                        color = getTypeSocket(ffi.typeof(sNodes.vars[sNodes.nodes[node_dragging].var].value))
                    else
                        color = getTypeSocket(ffi.typeof(sNodes.nodes[node_dragging].Inputs[slot_dragging].value))
                    end
                end
                local slot_pos = offset + sNodes.nodes[node_dragging]:GetInputSlotPos(sNodes.nodes[node_dragging].Inputs[slot_dragging].socketY)
                local mouse_pos = imgui.GetMousePos()
                draw_list:AddBezierCurve(mouse_pos, mouse_pos + imgui.ImVec2(50, 0), slot_pos + imgui.ImVec2(-50, 0), slot_pos, imgui.ColorConvertFloat4ToU32(color), 3.0);
            end
        else
            if state_mouse == STATE_OUTPUT and connecting then
                local find_socket = false
                for i,n in pairs(sNodes.nodes) do
                    for socket_idx = 1,#n.Inputs do
                        local socket_pos = n:GetInputSlotPos(n.Inputs[socket_idx].socketY)
                        local mouse_pos = imgui.GetMousePos() - offset
                        local type1
                        local type2
                        if sNodes.nodes[node_dragging].class.Type[0] == node_types.var and not sNodes.nodes[node_dragging].Outputs[slot_dragging].value then
                            type1 = sNodes.vars[sNodes.nodes[node_dragging].var].value
                        else
                            type1 = sNodes.nodes[node_dragging].Outputs[slot_dragging].value
                        end
                        if n.class.Type[0] == node_types.var and not n.Inputs[socket_idx].value then
                            type2 = sNodes.vars[n.var].value
                        else
                            type2 = n.Inputs[socket_idx].value
                        end
                        if getTypeValue(tostring(ffi.typeof(type1))) == getTypeValue(tostring(ffi.typeof(type2))) then
                            if getDistanceBetweenCoords2d(socket_pos.x,socket_pos.y,mouse_pos.x,mouse_pos.y) < 4 then
                                if n.Inputs[socket_idx].link ~= nil then
                                    local link = sNodes.links[n.Inputs[socket_idx].link]
                                    local node2 = sNodes.nodes[link.OutputIdx]
                                    local socket = node2.Outputs[link.OutputSlot]
                                    socket.link[link.OutputSlotNum] = nil
                                    sNodes.links[n.Inputs[socket_idx].link] = nil
                                end
                                local new_link = #sNodes.links+1
                                sNodes.nodes[node_dragging].Outputs[slot_dragging].link = sNodes.nodes[node_dragging].Outputs[slot_dragging].link or {}
                                local new_out = #sNodes.nodes[node_dragging].Outputs[slot_dragging].link+1
                                sNodes.links[new_link] = ffi.new('struct NodeLink',i,socket_idx,node_dragging,slot_dragging,new_out)
                                n.Inputs[socket_idx].link = new_link
                                sNodes.nodes[node_dragging].Outputs[slot_dragging].link[new_out] = new_link
                                find_socket = true
                                break
                            end
                        end
                    end
                    if find_socket then break end
                end
            elseif state_mouse == STATE_INPUT and connecting then
                local find_socket = false
                for i,n in pairs(sNodes.nodes) do
                    for socket_idx = 1,#n.Outputs do
                        local socket_pos = n:GetOutputSlotPos(n.Outputs[socket_idx].socketY)
                        local mouse_pos = imgui.GetMousePos() - offset
                        local type1
                        local type2
                        if sNodes.nodes[node_dragging].class.Type[0] == node_types.var and not sNodes.nodes[node_dragging].Inputs[slot_dragging].value then
                            type1 = sNodes.vars[sNodes.nodes[node_dragging].var].value
                        else
                            type1 = sNodes.nodes[node_dragging].Inputs[slot_dragging].value
                        end
                        if n.class.Type[0] == node_types.var and not n.Outputs[socket_idx].value then
                            type2 = sNodes.vars[n.var].value
                        else
                            type2 = n.Outputs[socket_idx].value
                        end
                        if getTypeValue(tostring(ffi.typeof(type1))) == getTypeValue(tostring(ffi.typeof(type2))) then
                            if getDistanceBetweenCoords2d(socket_pos.x,socket_pos.y,mouse_pos.x,mouse_pos.y) < 4 then
                                if sNodes.nodes[node_dragging].Inputs[slot_dragging].link ~= nil then
                                    local link = sNodes.links[sNodes.nodes[node_dragging].Inputs[slot_dragging].link]
                                    local node2 = sNodes.nodes[link.OutputIdx]
                                    local socket = node2.Outputs[link.OutputSlot]
                                    socket.link[link.OutputSlotNum] = nil
                                    sNodes.links[sNodes.nodes[node_dragging].Inputs[slot_dragging].link] = nil
                                end
                                local new_link = #sNodes.links+1
                                n.Outputs[socket_idx].link = n.Outputs[socket_idx].link or {}
                                local new_out = #n.Outputs[socket_idx].link+1
                                sNodes.links[new_link] = ffi.new('struct NodeLink',node_dragging,slot_dragging,i,socket_idx,new_out)
                                sNodes.nodes[node_dragging].Inputs[slot_dragging].link = new_link
                                n.Outputs[socket_idx].link[new_out] = new_link
                                find_socket = true
                                break
                            end
                        end
                    end
                    if find_socket then break end
                end
            end
            state_mouse = STATE_NONE
            node_dragging = nil
            slot_dragging = nil
            connecting = false
        end
    end
    draw_list:ChannelsMerge();

    -- Open context menu
    if imgui.IsMouseReleased(1) and imgui.IsMouseHoveringRect(win_pos,win_pos+imgui.GetWindowSize()) and imgui.IsWindowHovered(imgui.HoveredFlags.AllowWhenOverlapped) then
        if not imgui.IsAnyItemHovered() then
            imgui.OpenPopup('createNode')
        else
            imgui.OpenPopup('menuNode')
        end
    end

    -- Draw context menu
    imgui.PushStyleVarVec2(imgui.StyleVar.WindowPadding, imgui.ImVec2(8, 8));
    if imgui.BeginPopup("createNode") then
        local scene_pos = imgui.GetMousePosOnOpeningCurrentPopup() - offset;
        for k,v in pairs(sNodes.Nodes) do
            if ifInTable(k,sNodes.white_list) then
                if imgui.TreeNodeStr(langt[k]) then
                    for i,v in ipairs(sNodes.Nodes[k]) do
                        local _,text = getTypeNode(v.Type)
                        if imgui.MenuItemBool(text..' '..(langt[ffi.string(v.Name)] or '')) then
                            sNodes.nodes[#sNodes.nodes+1] = v(scene_pos)
                        end
                    end
                    imgui.TreePop()
                end
            end
        end
        if imgui.BeginMenu(langt['variables']) then
            if imgui.BeginMenu(langt['get']) then
                for k,v in pairs(sNodes.names_vars) do
                    if imgui.MenuItemBool(v) then
                        sNodes.nodes[#sNodes.nodes+1] = classNodeVar(k,scene_pos)
                    end
                end
                imgui.EndMenu()
            end
            if imgui.BeginMenu(langt['set']) then
                for k,v in pairs(sNodes.names_vars) do
                    if imgui.MenuItemBool(v) then
                        sNodes.nodes[#sNodes.nodes+1] = classNodeVar(k,scene_pos,true)
                    end
                end
                imgui.EndMenu()
            end
            imgui.EndMenu()
        end
        imgui.EndPopup();
    end
    if (imgui.BeginPopup("menuNode")) then
        if imgui.MenuItemBool(langt['delete']) then
            sNodes.nodes[node_hovered_in_scene[0]+1]:deleteConnections()
            sNodes.nodes[node_hovered_in_scene[0]+1] = nil
        end
        imgui.EndPopup()
    end

    imgui.PopStyleVar();

    -- Scrolling
    if (imgui.IsWindowHovered() and not imgui.IsAnyItemActive() and imgui.IsMouseDragging(2, 0.0)) then
        scrolling = scrolling + io.MouseDelta;
    end

    imgui.PopItemWidth();
    imgui.EndChild();

    

    imgui.PopStyleColor();
    imgui.PopStyleVar();
    imgui.EndGroup();

    imgui.End();
end)
return sNodes