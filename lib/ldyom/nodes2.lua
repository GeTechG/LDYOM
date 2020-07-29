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
    elseif value == 'ctype<unsigned char [1]>' then
        return 0
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
    vr.temp_var.upd_actor = {langt['player']}
    for i = 1,#vr.temp_var.list_name_actors do
        vr.temp_var.upd_actor[#vr.temp_var.upd_actor+1] = vr.temp_var.list_name_actors[i]
    end

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
        local ext = loadstring('return function(self) return '..socket.ext..' end')()(self)
        imgui[socket.type]('',value,new('const char* const [?]',#ext,ext),#ext)
        imgui.PopID()
    elseif socket.type == 'Bool' then
        mimgui_addons.ToggleButton(langt[ffi.string(socket.name)] or '',value)
    elseif socket.type == 'Button' then
        if imgui.Button(langt[ffi.string(socket.name)] or '') then
            loadstring('return function(self) '..socket.ext..' end')()(self)
        end
    elseif socket.type == 'ButtonSelecter' then
        if imgui.ImageButton(loadstring('return '..socket.ext[1])(), socket.ext[2], imgui.ImVec2((socket.value[0]-1)*socket.ext[3],0), imgui.ImVec2(socket.value[0]*socket.ext[3],1)) then
            loadstring('return function(self) '..socket.ext[4]..' end')()(self)
        end
        if imgui.BeginPopup('weapon') then
            imgui.BeginChild('weapon',imgui.ImVec2(200,450))
            for i = 1,#ID_Weapons do
                imgui.PushIDStr(tostring(i))
                if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                    self.Inputs[3].value[0] = i
                end
                if imgui.IsItemHovered() then
                    imgui.SetTooltip(vr.temp_var.weap_names[i])
                end
                imgui.PopID()
                if i % 3 ~= 0 then
                    imgui.SameLine()
                end
            end
    
            imgui.EndChild()
            imgui.EndPopup()
        end
    elseif socket.type == 'SliderScalar' then
        imgui.PushIDStr(langt[ffi.string(socket.name)] or '')
        imgui.Text(langt[ffi.string(socket.name)] or ffi.string(socket.name))
        local ext = loadstring('return function(self) return '..socket.ext[4]..' end')()(self)
        imgui.SliderScalar('', socket.ext[1], socket.value, socket.ext[2], socket.ext[3], ext[socket.value[0]+1])
        imgui.PopID()
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
function Node:getInputValue(idx,data)
    if self.Inputs[idx].link then
        local link = data.links[self.Inputs[idx].link]
        local node = data.nodes[link.OutputIdx]
        if node.class.Type[0] == node_types.var then
            return nodes2.real_vars[node.var]
        else
            return data.nodes[link.OutputIdx].Outputs[link.OutputSlot].value
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

local classVar = class('Variable')
function classVar:initialize()
    self.name = new.char[65](langt['variable']..' #'..#sNodes.names_vars+1)
    self.typeValue = new.ImU8(0)
    self.value = new.float(0)
end
bitser.registerClass(classVar)

sNodes.Nodes = {
    main = {},
    storyline = {},
    mission = {},
    actor = {},
    car = {},
    object = {},
    particle = {},
    pickup = {},
    explosion = {},
    audio = {}
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
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
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
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
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
function node:play(data)
    while true do
        wait(0)
        if self.Outputs[1].link then
            for k,v in pairs(self.Outputs[1].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
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
function node:play(data)
    local value = self:getInputValue(2,data)
    local num_pack
	for i,v in ipairs(vr.temp_var.list_name_mission_packs) do
		if ffi.string(v) == ffi.string(vr.storyline.missPack) then
			num_pack = i
		end
	end
	local missions_list = vr.temp_var.list_name_missions[num_pack]
    mp.playMission(missions_list[value[0]+1])
    mp.last_miss = value[0]+1
    for k,v in pairs(data.nodes) do
        if v.class.name == 'MissionComplete' then
            if v:getInputValue(1)[0] == value[0] then
                mp.thread[#mp.thread+1] = lua_thread.create(function(v)
                v:play(data)
                end,v)
            end
        end
    end
    self.Outputs[2].value[0] = value[0]
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
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
function node:play(data)
    mp.storylineOn = false
end
sNodes.Nodes.storyline[#sNodes.Nodes.storyline+1] = node
bitser.registerClass(node)

node = Node:subclass('ActivateСheckpoint')
node.static.Name = new.char[65]('nodeActivСheck')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('checkpoint','Combo', new.int(0), 'vr.temp_var.list_name_storylineCheckpoints'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local check = self:getInputValue(2,data)[0]+1
    local xx,yy,zz = vr.storyline.list_checkpoints[check].data.pos[0],vr.storyline.list_checkpoints[check].data.pos[1],vr.storyline.list_checkpoints[check].data.pos[2]
    mp.storyCheck[check] = addSpriteBlipForCoord(xx,yy,zz,vr.storyline.list_checkpoints[check].data.iconMarker[0])
    changeBlipColour(mp.storyCheck[check],vr.storyline.list_checkpoints[check].data.colorBlip[0])
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
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
function classNodeVar:play(data)
    if self.set then
        data.real_vars[self.var] = self:getInputValue(2,data)[0]+1
    end
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
bitser.registerClass(classNodeVar)

node = Node:subclass('runSignal')
node.static.Name = new.char[65]('nodeRunSignal')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire')
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)

    mp.signal = true

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.mission[#sNodes.Nodes.mission+1] = node
bitser.registerClass(node)

node = Node:subclass('сheckTarget')
node.static.Name = new.char[65]('nodeCheckTarget')
node.static.Type = new.ImU16(node_types.fork)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('target','Combo', new.int(0), 'vr.temp_var.list_name_targets'),
        },
        {
            Socket('truee','wire'),
            Socket('falsee','wire'),
            Socket('','',new.bool(false),nil,true),
        }
    )
end
function node:play(data)
    local target = self:getInputValue(2,data)[0]+1

    if target == mp.curr_target then
        if self.Outputs[1].link then
            for k,v in pairs(self.Outputs[1].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    else
        if self.Outputs[2].link then
            for k,v in pairs(self.Outputs[1].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    end
    self.Outputs[3].value[0] = target == mp.curr_target
end
sNodes.Nodes.mission[#sNodes.Nodes.mission+1] = node
bitser.registerClass(node)

node = Node:subclass('startTarget')
node.static.Name = new.char[65]('nodeStartTarget')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('target','Combo', new.int(0), 'vr.temp_var.list_name_targets'),
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.mission[#sNodes.Nodes.mission+1] = node
bitser.registerClass(node)

node = Node:subclass('setTarget')
node.static.Name = new.char[65]('nodeSetTarget')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('target','Combo', new.int(0), 'vr.temp_var.list_name_targets'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    mp.curr_target = self:getInputValue(2,data)[0]+1

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end

end
sNodes.Nodes.mission[#sNodes.Nodes.mission+1] = node
bitser.registerClass(node)

node = Node:subclass('showActor')
node.static.Name = new.char[65]('nodeShowActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local actor = self:getInputValue(2,data)[0]+1

    mp.thread[#mp.thread+1] = lua_thread.create(function(actor)
        if (not vr.list_actors[actor]['data']["randomSpawn"][0]) or (math.random(0,100) > 50) then
            local xx,xy,xz = vr.list_actors[actor]['data']['pos'][0], vr.list_actors[actor]['data']['pos'][1], vr.list_actors[actor]['data']['pos'][2]
            local modell
            if vr.list_actors[actor]['data']['modelType'][0] == 0 then
                modell = vr.list_actors[actor]['data']['modelId'][0]
                requestModel(modell)
                while not hasModelLoaded(modell) do
                    wait(0)
                end
            elseif vr.list_actors[actor]['data']['modelType'][0] == 1 then
                local modell_n = ID_Spec_Actors[vr.list_actors[actor]['data']['modelId'][0]+1]
                loadSpecialCharacter(modell_n,vr.list_actors[actor]['data']['slotSkin'][0])
                while not hasSpecialCharacterLoaded(vr.list_actors[actor]['data']['slotSkin'][0]) do
                    wait(0)
                end
                modell = 290 + vr.list_actors[actor]['data']['slotSkin'][0]-1
            end
            if vr.list_actors[actor]['data']['group'][0] == 0 then
                mp.actors[actor] = createChar(23, modell, xx, xy, xz)
                local g = getPlayerGroup(PLAYER_HANDLE)
                setGroupMember(g,mp.actors[actor])
            else
                mp.actors[actor] = createChar(24 + vr.list_actors[actor]['data']['group'][0]-1, modell, xx, xy, xz)
            end
            setCharCollision(mp.actors[actor],false)
            while getDistanceBetweenCoords3d(cx,cy,cz,xx,xy,xz) > 100 do
                wait(0)
                cx,cy,cz = getActiveCameraCoordinates()
            end
            setCharCollision(mp.actors[actor],true)
            setCharHealth(mp.actors[actor],vr.list_actors[actor]['data']['health'][0])
            setCharHeading(mp.actors[actor], vr.list_actors[actor]['data']['angle'][0])
            setCharAccuracy(mp.actors[actor],vr.list_actors[actor]['data'].accuracy[0])
            setCharSuffersCriticalHits(mp.actors[actor],vr.list_actors[actor]['data'].headshot[0])
            requestModel(getWeapontypeModel(ID_Weapons[vr.list_actors[actor]['data']['weapon'][0]]))
            while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_actors[actor]['data']['weapon'][0]])) do
                wait(0)
            end
            giveWeaponToChar(mp.actors[actor], ID_Weapons[vr.list_actors[actor]['data']['weapon'][0]], vr.list_actors[actor]['data']['ammo'][0])
            setCurrentCharWeapon(mp.actors[actor],1)
            markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[vr.list_actors[actor]['data']['weapon'][0]]))
            wait(0)
            if vr.list_actors[actor]['data']['shouldNotDie'][0] == true then
                mp.thread[#mp.thread+1] = lua_thread.create(mp.char_is_not_dead,mp.actors[actor])
            end
        end
    end,actor)
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperActor' and actor == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('apperActor')
node.static.Name = new.char[65]('nodeApperActor')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('hideActor')
node.static.Name = new.char[65]('nodeHideActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local actor = self:getInputValue(2,data)[0]+1

    deleteChar(mp.actors[actor])
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperActor' and actor == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperActor')
node.static.Name = new.char[65]('nodeDisapperActor')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('showCar')
node.static.Name = new.char[65]('nodeShowCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local car = self:getInputValue(2,data)[0]+1

    mp.thread[#mp.thread+1] =lua_thread.create(function(car)
        local md = vr.list_cars[car]['data']['modelId'][0]
        local xx,xy,xz = vr.list_cars[car]['data']['pos'][0], vr.list_cars[car]['data']['pos'][1], vr.list_cars[car]['data']['pos'][2]
        requestModel(md)
        while not hasModelLoaded(md) do
            wait(0)
        end
        mp.cars[car] = createCar(md, xx, xy, xz)
        freezeCarPosition(mp.cars[car],true)
        local cx,cy,cz = getActiveCameraCoordinates()
        while getDistanceBetweenCoords3d(cx,cy,cz,xx,xy,xz) > 100 do
            wait(0)
            cx,cy,cz = getActiveCameraCoordinates()
        end
        freezeCarPosition(mp.cars[car],false)
        setCarCoordinates(mp.cars[car],xx,xy,xz)
        setCarHealth(mp.cars[car],vr.list_cars[car]['data']['health'][0])
        setCarHeading(mp.cars[car], vr.list_cars[car]['data']['angle'][0])
        lockCarDoors(mp.cars[car],fif(vr.list_cars[car]['data'].locked[0],0,1))
        setCarProofs(mp.cars[car],vr.list_cars[car]['data']['bulletproof'][0],vr.list_cars[car]['data']['fireproof'][0],vr.list_cars[car]['data']['explosionproof'][0],vr.list_cars[car]['data']['collisionproof'][0],vr.list_cars[car]['data']['meleeproof'][0])
        setCanBurstCarTires(mp.cars[car], not vr.list_cars[car]['data']['tiresVulnerability'][0])
        for_each_vehicle_material(mp.cars[car],function(i,mat, comp, obj)
            local new_r, new_g, new_b, a = vr.list_cars[car]['data'].colors[i][2][0],vr.list_cars[car]['data'].colors[i][2][1],vr.list_cars[car]['data'].colors[i][2][2],vr.list_cars[car]['data'].colors[i][2][3]
            mat:set_color(new_r*255, new_g*255, new_b*255, a*255)
        end)
        if vr.list_cars[car]['data']['shouldNotDie'][0] == true then
            mp.thread[#mp.thread+1] = lua_thread.create(car_is_not_dead,mp.cars[car])
        end
    end,car)
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperCar' and car == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.car[#sNodes.Nodes.car+1] = node
bitser.registerClass(node)

node = Node:subclass('apperCar')
node.static.Name = new.char[65]('nodeApperCar')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.car[#sNodes.Nodes.car+1] = node
bitser.registerClass(node)

node = Node:subclass('hideCar')
node.static.Name = new.char[65]('nodeHideCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local car = self:getInputValue(2,data)[0]+1

    deleteCar(mp.cars[car])
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperCar' and car == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.car[#sNodes.Nodes.car+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperCar')
node.static.Name = new.char[65]('nodeDisapperCar')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.car[#sNodes.Nodes.car+1] = node
bitser.registerClass(node)

node = Node:subclass('showObject')
node.static.Name = new.char[65]('nodeShowObject')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('object','Combo', new.int(0), 'vr.temp_var.list_name_objects'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local object = self:getInputValue(2,data)[0]+1

    local md = vr.list_objects[object]['data']['modelId'][0]
    local xx,xy,xz = vr.list_objects[object]['data']['pos'][0], vr.list_objects[object]['data']['pos'][1], vr.list_objects[object]['data']['pos'][2]
    local rxx,rxy,rxz = vr.list_objects[object]['data']['rotate'][0], vr.list_objects[object]['data']['rotate'][1], vr.list_objects[object]['data']['rotate'][2]
    requestModel(md)
    while not isModelAvailable(md) do
        wait(0)
    end
    mp.objects[object] = createObject(md, xx, xy, xz)
    setObjectCoordinates(mp.objects[object], xx, xy, xz)
    setObjectRotation(mp.objects[object], rxx, rxy, rxz)
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperObject' and object == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.object[#sNodes.Nodes.object+1] = node
bitser.registerClass(node)

node = Node:subclass('apperObject')
node.static.Name = new.char[65]('nodeApperObject')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('object','Combo', new.int(0), 'vr.temp_var.list_name_objects'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.object[#sNodes.Nodes.object+1] = node
bitser.registerClass(node)

node = Node:subclass('hideObject')
node.static.Name = new.char[65]('nodeHideObject')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('object','Combo', new.int(0), 'vr.temp_var.list_name_objects'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local object = self:getInputValue(2,data)[0]+1

    deleteObject(mp.objects[object])
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperObject' and object == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.object[#sNodes.Nodes.object+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperObject')
node.static.Name = new.char[65]('nodeDisapperObject')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('object','Combo', new.int(0), 'vr.temp_var.list_name_objects'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.object[#sNodes.Nodes.object+1] = node
bitser.registerClass(node)

node = Node:subclass('showParticle')
node.static.Name = new.char[65]('nodeShowParticle')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('particle','Combo', new.int(0), 'vr.temp_var.list_name_particles'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local particle = self:getInputValue(2,data)[0]+1

    local md = Particle_name[vr.list_particles[particle]['data']['modelId'][0]+1]
    local xx,xy,xz = vr.list_particles[particle]['data']['pos'][0], vr.list_particles[particle]['data']['pos'][1], vr.list_particles[particle]['data']['pos'][2]
    local rxx,rxy,rxz = vr.list_particles[particle]['data']['rotate'][0], vr.list_particles[particle]['data']['rotate'][1], vr.list_particles[particle]['data']['rotate'][2]

    mp.particles[particle] = {}
    if vr.list_particles[particle]['data'].tied[0] == 0 then
        if not hasModelLoaded(327) then
            requestModel(327)
            while not hasModelLoaded(327) do
                wait(0)
            end
        end
        mp.particles[particle][2] = createObject(327, xx, xy, xz)
        mp.particles[particle][1] = createFxSystemOnObjectWithDirection(md,mp.particles[particle][2],0,0,0,rxx,rxy,rxz, 1)
        playFxSystem(mp.particles[particle][1])
        wait(0)
        setObjectVisible(mp.particles[particle][2],false)
        setObjectCoordinates(mp.particles[particle][2], xx, xy, xz)
    elseif vr.list_particles[particle]['data'].tied[0] == 1 then
        local xt,yt,zt = getCharCoordinates(mp.actors[vr.list_particles[particle]['data']['tiedId'][0]+1])
        local angleTied = getCharHeading(mp.actors[vr.list_particles[particle]['data']['tiedId'][0]+1])
        xx,xy,xz = xx-xt,xy-yt,xz-zt
        xx,xy = rotateVec2(xx,xy,-angleTied)
        local xt,yt,zt = getCharCoordinates(mp.actors[vr.list_particles[particle]['data']['tiedId'][0]+1])
        xx,xy,xz = xx-xt,xy-yt,xz-zt
        mp.particles[particle][1] = createFxSystemOnCharWithDirection(md,mp.actors[vr.list_particles[particle]['data']['tiedId'][0]+1],xx,xy,xz,rxx,rxy,rxz, 1)
        playFxSystem(mp.particles[particle][1])
    elseif vr.list_particles[particle]['data'].tied[0] == 2 then
        local xt,yt,zt = getCarCoordinates(mp.cars[vr.list_particles[particle]['data']['tiedId'][0]+1])
        local angleTied = getCarHeading(mp.cars[vr.list_particles[particle]['data']['tiedId'][0]+1])
        xx,xy,xz = xx-xt,xy-yt,xz-zt
        xx,xy = rotateVec2(xx,xy,-angleTied)
        mp.particles[particle][1] = createFxSystemOnCarWithDirection(md,mp.cars[vr.list_particles[particle]['data']['tiedId'][0]+1],xx,xy,xz,rxx,angleTied-rxy,rxz, 1)
        playFxSystem(mp.particles[particle][1])
    elseif vr.list_particles[particle]['data'].tied[0] == 3 then
        local xt,yt,zt = getObjectCoordinates(mp.objects[vr.list_particles[particle]['data']['tiedId'][0]+1])
        local angleTied = getObjectHeading(mp.objects[vr.list_particles[particle]['data']['tiedId'][0]+1])
        xx,xy,xz = xx-xt,xy-yt,xz-zt
        xx,xy = rotateVec2(xx,xy,-angleTied)
        mp.particles[particle][1] = createFxSystemOnObjectWithDirection(md,mp.objects[vr.list_particles[particle]['data']['tiedId'][0]+1],xx,xy,xz,rxx,angleTied-rxy,rxz, 1)
        playFxSystem(mp.particles[particle][1])
    end
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperParticle' and particle == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.particle[#sNodes.Nodes.particle+1] = node
bitser.registerClass(node)

node = Node:subclass('apperParticle')
node.static.Name = new.char[65]('nodeApperParticle')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('particle','Combo', new.int(0), 'vr.temp_var.list_name_particles'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.particle[#sNodes.Nodes.particle+1] = node
bitser.registerClass(node)

node = Node:subclass('hideParticle')
node.static.Name = new.char[65]('nodeHideParticle')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('particle','Combo', new.int(0), 'vr.temp_var.list_name_particles'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local particle = self:getInputValue(2,data)[0]+1

    killFxSystemNow(mp.particles[particle][1])
	if mp.particles[particle][2] then deleteObject(mp.particles[particle][2]) end
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperParticle' and particle == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.particle[#sNodes.Nodes.particle+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperParticle')
node.static.Name = new.char[65]('nodeDisapperParticle')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('particle','Combo', new.int(0), 'vr.temp_var.list_name_particles'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.particle[#sNodes.Nodes.particle+1] = node
bitser.registerClass(node)

node = Node:subclass('showPickup')
node.static.Name = new.char[65]('nodeShowPickup')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('pickup','Combo', new.int(0), 'vr.temp_var.list_name_pickups'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local pickup = self:getInputValue(2,data)[0]+1

    local xx,xy,xz = vr.list_pickups[pickup]['data']['pos'][0], vr.list_pickups[pickup]['data']['pos'][1], vr.list_pickups[pickup]['data']['pos'][2]
    local spawn_t = 3
    if vr.list_pickups[pickup]['data']['spawnType'][0] == 1 then
        spawn_t = 2
    elseif vr.list_pickups[pickup]['data']['spawnType'][0] == 2 then
        spawn_t = 15
    end

    if vr.list_pickups[pickup]['data']['type'][0] == 0 then
        local md = getWeapontypeModel(ID_Weapons[vr.list_pickups[pickup]['data']['weapon'][0]])
        if not isModelAvailable(md) then
            requestModel(md)
            while not isModelAvailable(md) do
                wait(0)
            end
        end
        mp.pickups[pickup] = createPickupWithAmmo(md, spawn_t, vr.list_pickups[pickup]['data']['ammo'][0], xx, xy, xz)
    end
    if vr.list_pickups[pickup]['data']['type'][0] >= 1 then
        local md = 1240
        if vr.list_pickups[pickup]['data']['type'][0] == 2 then
            md = 1242
        elseif vr.list_pickups[pickup]['data']['type'][0] == 3 then
            md = 1247
        elseif vr.list_pickups[pickup]['data']['type'][0] == 4 then
            md = 1241
        elseif vr.list_pickups[pickup]['data']['type'][0] == 5 then
            md = vr.list_pickups[pickup]['data']['modelId'][0]
        end

        if not isModelAvailable(md) then
            requestModel(md)
            while not isModelAvailable(md) do
                wait(0)
            end
        end
        mp.pickups[pickup] = select(2,createPickup(md, spawn_t, xx, xy, xz))
    end
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperPickup' and pickup == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.pickup[#sNodes.Nodes.pickup+1] = node
bitser.registerClass(node)

node = Node:subclass('apperPickup')
node.static.Name = new.char[65]('nodeApperPickup')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('pickup','Combo', new.int(0), 'vr.temp_var.list_name_pickups'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.pickup[#sNodes.Nodes.pickup+1] = node
bitser.registerClass(node)

node = Node:subclass('hidePickup')
node.static.Name = new.char[65]('nodeHidePickup')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('pickup','Combo', new.int(0), 'vr.temp_var.list_name_pickups'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local pickup = self:getInputValue(2,data)[0]+1

    removePickup(mp.pickups[pickup])
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperPickup' and pickup == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.pickup[#sNodes.Nodes.pickup+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperPickup')
node.static.Name = new.char[65]('nodeDisapperPickup')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('pickup','Combo', new.int(0), 'vr.temp_var.list_name_pickups'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.pickup[#sNodes.Nodes.pickup+1] = node
bitser.registerClass(node)

node = Node:subclass('showExplosion')
node.static.Name = new.char[65]('nodeShowExplosion')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('explosion','Combo', new.int(0), 'vr.temp_var.list_name_explosions'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local explosion = self:getInputValue(2,data)[0]+1

    local xx,xy,xz = vr.list_explosions[explosion]['data']['pos'][0], vr.list_explosions[explosion]['data']['pos'][1], vr.list_explosions[explosion]['data']['pos'][2]

    if vr.list_explosions[explosion]['data']['type'][0] == 0 then
        mp.explosions[explosion] = startScriptFire(xx,xy,xz,vr.list_explosions[explosion]['data']['propagationFire'][0],vr.list_explosions[explosion]['data']['sizeFire'][0])
    end
    if vr.list_explosions[explosion]['data']['type'][0] == 1 then
        addExplosion(xx,xy,xz,vr.list_explosions[explosion]['data']['typeExplosion'][0])
    end
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperExplosion' and explosion == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.explosion[#sNodes.Nodes.explosion+1] = node
bitser.registerClass(node)

node = Node:subclass('apperExplosion')
node.static.Name = new.char[65]('nodeApperExplosion')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('explosion','Combo', new.int(0), 'vr.temp_var.list_name_explosions'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.explosion[#sNodes.Nodes.explosion+1] = node
bitser.registerClass(node)

node = Node:subclass('hideExplosion')
node.static.Name = new.char[65]('nodeHideExplosion')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('explosion','Combo', new.int(0), 'vr.temp_var.list_name_explosions'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local explosion = self:getInputValue(2,data)[0]+1

    if vr.list_explosions[explosion]['data']['type'][0] == 0 then
        removeScriptFire(mp.explosions[explosion])
    end
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperExplosion' and explosion == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.explosion[#sNodes.Nodes.explosion+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperExplosion')
node.static.Name = new.char[65]('nodeDisapperExplosion')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('explosion','Combo', new.int(0), 'vr.temp_var.list_name_explosions'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.explosion[#sNodes.Nodes.explosion+1] = node
bitser.registerClass(node)

node = Node:subclass('showAudio')
node.static.Name = new.char[65]('nodeShowAudio')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('audio','Combo', new.int(0), 'vr.temp_var.list_name_audios'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local audio = self:getInputValue(2,data)[0]+1

    audio_player(audio,vr.list_audio)
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'apperAudio' and audio == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.audio[#sNodes.Nodes.audio+1] = node
bitser.registerClass(node)

node = Node:subclass('apperAudio')
node.static.Name = new.char[65]('nodeApperAudio')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('audio','Combo', new.int(0), 'vr.temp_var.list_name_audios'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.audio[#sNodes.Nodes.audio+1] = node
bitser.registerClass(node)

node = Node:subclass('hideAudio')
node.static.Name = new.char[65]('nodeHideAudio')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('audio','Combo', new.int(0), 'vr.temp_var.list_name_audios'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local audio = self:getInputValue(2,data)[0]+1

    setAudioStreamState(mp.audio[audio],0)
	releaseAudioStream(mp.audio[audio])
    
    for k,v in pairs(data.nodes) do
        if v.class.name == 'disapperAudio' and audio == v:getInputValue(1,data)[0]+1 then
            mp.thread[#mp.thread+1] = lua_thread.create(function(v,data)
                v:play(data)
            end,v,data)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.audio[#sNodes.Nodes.audio+1] = node
bitser.registerClass(node)

node = Node:subclass('disapperAudio')
node.static.Name = new.char[65]('nodeDisapperAudio')
node.static.Type = new.ImU16(node_types.event)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('audio','Combo', new.int(0), 'vr.temp_var.list_name_audios'),
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.audio[#sNodes.Nodes.audio+1] = node
bitser.registerClass(node)

node = Node:subclass('animActor')
node.static.Name = new.char[65]('nodeAnimActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('pack','Combo', new.int(0), 'Anims.Anim_name'),
            Socket('anim','Combo', new.int(0), "Anims.Anim_list[self.Inputs[3].value[0]+1]"),
            Socket('looped','Bool', new.bool(false)),
            Socket('unbreak','Bool', new.bool(false)),
            Socket('stopEndFrameW','Bool', new.bool(false)),
            Socket('useAnimMove','Bool', new.bool(false)),
            Socket('time','InputFloat', new.float(0)),
            Socket('preview','Button', nil, "playNodePreviewAnimPed(self)"),
            Socket('previewPos','Button', nil, "vr.editmodeNodeAnimActor = true;vr.nodeEditmode = self"),
            Socket('unPreview','Button', nil, "taskPlayAnim(PLAYER_PED, 'WALK_START', 'PED', 1.0, false, false, false, false, -1)"),
        },
        {
            Socket('','wire'),
            Socket('whenItsDone','wire')
        }
    )
end
function node:play(data)
    mp.thread[#mp.thread+1] = lua_thread.create(function(self,data)
        local ped = mp.actors[self:getInputValue(2,data)[0]+1]
        local pack = self:getInputValue(3,data)[0]+1
        local anim = self:getInputValue(4,data)[0]+1
        local looped = self:getInputValue(5,data)[0]
        local unbreak = self:getInputValue(6,data)[0]
        local stopEndFrame = self:getInputValue(7,data)[0]
        local useAnimMove = self:getInputValue(8,data)[0]
        local time = self:getInputValue(9,data)[0]

        if not hasAnimationLoaded(Anims['Anim_name'][pack]) then
            requestAnimation(Anims['Anim_name'][pack])
            while not hasAnimationLoaded(Anims['Anim_name'][pack]) do
                wait(0)
            end
        end
        local animm = Anims['Anim_list'][pack]
        animm = animm[anim]
        if not unbreak then
            taskPlayAnim(ped, animm, Anims['Anim_name'][pack], 1.0, looped, useAnimMove, useAnimMove, stopEndFrame, fif(time > 0,time*1000,-1))
        else
            taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][pack], 1.0, looped, useAnimMove, useAnimMove, stopEndFrame, fif(time > 0,time*1000,-1),false,false)
        end
        if time > 0 then
            wait(time * 1000)
        else
            while not isCharPlayingAnim(ped,animm) do
                wait(0)
            end
            wait(getCharAnimTotalTime(ped,animm))
            if not doesCharExist(ped) then
                return
            end
        end

        if self.Outputs[2].link then
            for k,v in pairs(self.Outputs[2].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    end,self,data)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('pathActor')
node.static.Name = new.char[65]('nodePathActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('type_move_ped','Combo', new.int(0), 'vr.temp_var.move_type_ped'),
            Socket('type_route_ped','Combo', new.int(0), 'vr.temp_var.move_route_ped'),
            Socket('add_enter','Button', nil, "vr.editmodeNodePathActor = true;vr.nodeEditmode = self")
        },
        {
            Socket('','wire'),
            Socket('whenItsDone','wire')
        }
    )
end
function node:play(data)
    mp.thread[#mp.thread+1] = lua_thread.create(function(self,data)
        local ped = mp.actors[self:getInputValue(2,data)[0]+1]
        local type_move_ped = self:getInputValue(3,data)[0]
        local type_route_ped = self:getInputValue(4,data)[0]+1

        local type_walk = 4
        if type_move_ped == 3 then type_walk = 6
        elseif type_move_ped == 4 then type_walk = 7 end
        taskToggleDuck(ped, fif(type_move_ped == 0,true,false))
        repeat
            for i = 1,#self.points do
                if not  doesCharExist(ped) then
                    return
                end
                local x1,y1,z1 = self.points[i][0],self.points[i][1],self.points[i][2]
                taskGoStraightToCoord(ped,x1,y1,z1,type_walk,-1)
                local px,py,pz = getCharCoordinates(ped)
                while getDistanceBetweenCoords3d(x1,y1,z1,px,py,pz) > 1 do
                    wait(0)
                    if doesCharExist(ped) then
                        px,py,pz = getCharCoordinates(ped)
                    else
                        return
                    end
                end
            end
            wait(0)
        until type_route_ped == 1
        taskToggleDuck(ped, false)
            
        while getDistanceBetweenCoords3d(self.points[#self.points][0],self.points[#self.points][1],self.points[#self.points][2],px,py,pz) > 0.1 do
            wait(0)
            if doesCharExist(ped) then
                px,py,pz = getCharCoordinates(ped)
            else
                return
            end
        end

        if self.Outputs[2].link then
            for k,v in pairs(self.Outputs[2].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    end,self,data)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('pathActorCar')
node.static.Name = new.char[65]('nodePathActorCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
            Socket('speed','InputFloat', new.float(60)),
            Socket('type_route_ped','Combo', new.int(0), 'vr.temp_var.move_route_ped'),
            Socket('add_enter','Button', nil, "vr.editmodeNodePathActorCar = true;vr.nodeEditmode = self")
        },
        {
            Socket('','wire'),
            Socket('whenItsDone','wire')
        }
    )
end
function node:play(data)
    mp.thread[#mp.thread+1] = lua_thread.create(function(self,data)
        local ped = mp.actors[self:getInputValue(2,data)[0]+1]
        local car = mp.cars[self:getInputValue(3,data)[0]+1]
        local speed = self:getInputValue(4,data)[0]
        local type_route_ped = self:getInputValue(5,data)[0]+1

        local px,py,pz = getCharCoordinates(ped)
        repeat
            for i = 1,#self.points do
                if not  doesCharExist(ped) then
                    return
                end
                local x1,y1,z1 = self.points[i][0],self.points[i][1],self.points[i][2]
                taskCarDriveToCoord(ped,car,x1,y1,z1,speed,0,0,0)
                px,py,pz = getCharCoordinates(ped)
                while getDistanceBetweenCoords3d(px,py,pz,x1,y1,z1) > 5 do
                    wait(0)
                    if not  doesCharExist(ped) then
                        return
                    end
                    px,py,pz = getCharCoordinates(ped)
                end
            end
            wait(0)
        until type_route_ped == 1

        while getDistanceBetweenCoords3d(self.points[#self.points][0],self.points[#self.points][1],self.points[#self.points][2],px,py,pz) > 1 do
            wait(0)
            if doesCharExist(ped) then
                px,py,pz = getCarCoordinates(car)
            else
                return
            end
        end

        if self.Outputs[2].link then
            for k,v in pairs(self.Outputs[2].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    end,self,data)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('exitActorCar')
node.static.Name = new.char[65]('nodeExitActorCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
        },
        {
            Socket('','wire'),
            Socket('whenItsDone','wire')
        }
    )
end
function node:play(data)
    mp.thread[#mp.thread+1] = lua_thread.create(function(self,data)
        local ped = mp.actors[self:getInputValue(2,data)[0]+1]

        if isCharInAnyCar(ped) then
            taskLeaveAnyCar(ped)
            while isCharInAnyCar(ped) do
                wait(0)
            end
        end

        if self.Outputs[2].link then
            for k,v in pairs(self.Outputs[2].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    end,self,data)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('сhaseCar')
node.static.Name = new.char[65]('nodeChaseCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('car_actr','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
            Socket('max_speed','InputFloat', new.float(60)),
            Socket('beh_transp','InputInt', new.int(1)),
            Socket('beh_driver','Combo', new.int(0), 'vr.temp_var.driver_beh'),
            Socket('car_target','Combo', new.int(0), 'vr.temp_var.list_name_cars')
        },
        {
            Socket('','wire')
        }
    )
end
function node:play(data)
    local ped = mp.actors[self:getInputValue(2,data)[0]+1]
    local car_actr = mp.cars[self:getInputValue(3,data)[0]+1]
    local max_speed = self:getInputValue(4,data)[0]
    local beh_transp = self:getInputValue(5,data)[0]
    local beh_driver = self:getInputValue(6,data)[0]
    local car_target = mp.cars[self:getInputValue(7,data)[0]+1]

    taskCarMission(ped,car_actr,car_target,beh_driver-1,max_speed,beh_transp)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('enterActorCar')
node.static.Name = new.char[65]('nodeEnterActorCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
            Socket('speed_walk','Combo', new.int(0), 'vr.temp_var.speed_walk_to_car'),
            Socket('seat','Combo', new.int(0), 'vr.temp_var.place_in_car'),
            Socket('teleport','Bool', new.bool(false)),
        },
        {
            Socket('','wire'),
            Socket('whenItsDone','wire')
        }
    )
end
function node:play(data)
    mp.thread[#mp.thread+1] = lua_thread.create(function(self,data)
        local ped = mp.actors[self:getInputValue(2,data)[0]+1]
        local car = mp.cars[self:getInputValue(3,data)[0]+1]
        local speed_walk = self:getInputValue(4,data)[0]+1
        local seat = self:getInputValue(5,data)[0]+1
        local teleport = self:getInputValue(6,data)[0]

        if seat == 1 then
            setNextDesiredMoveState(speed_walk+4)
            if teleport then
                taskEnterCarAsDriver(ped,car,0)
            else
                taskEnterCarAsDriver(ped,car,-1)
            end
        else
            setNextDesiredMoveState(speed_walk+4)
            if teleport then
                taskEnterCarAsPassenger(ped,car,0,seat-1)
            else
                taskEnterCarAsPassenger(ped,car,-1,seat-1)
            end
        end

        while not isCharInAnyCar(ped) do
            wait(0)
            if not doesCharExist(ped) then
                return
            end
        end

        if self.Outputs[2].link then
            for k,v in pairs(self.Outputs[2].link) do
                data.nodes[data.links[v].InputIdx]:play(data)
            end
        end
    end,self,data)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('goActorToActor')
node.static.Name = new.char[65]('nodeGoActorToActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('target','Combo', new.int(0), 'vr.temp_var.upd_actor'),
            Socket('radius','InputInt', new.int(1)),
            Socket('executeUntil','', new.bool(false),nil,true),
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)
    mp.thread[#mp.thread+1] = lua_thread.create(function(self,data)
        local ped = mp.actors[self:getInputValue(2,data)[0]+1]
        local ped2 = self:getInputValue(3,data)[0]
        local radius = self:getInputValue(4,data)[0]
        local executeUntil = self:getInputValue(5,data)[0]

        repeat
            wait(0)
            if not doesCharExist(ped) then
                return
            end

            executeUntil = self:getInputValue(5,data)[0]
            local xa,ya,za = getCharCoordinates(ped)
            if ped2 == 0 then
                local xp,yp,zp = getCharCoordinates(PLAYER_PED)
                if getDistanceBetweenCoords2d(xa,ya,xp,yp) > radius then
                    taskGotoChar(ped,PLAYER_PED,-1,radius)
                end
            else
                local xp,yp,zp = getCharCoordinates(mp.actors[ped2])
                if getDistanceBetweenCoords2d(xa,ya,xp,yp) > radius then
                    taskGotoChar(ped,mp.actors[ped2],-1,radius)
                end
            end
        until executeUntil
    end,self,data)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('giveWeapActor')
node.static.Name = new.char[65]('nodeGiveWeapActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
            Socket('weapon','ButtonSelecter', new.int(1), {'weaponsAtlas',imgui.ImVec2(52,52),0.02272727272,'imgui.OpenPopup("weapon")'}),
            Socket('countAmmo','InputInt', new.int(0))
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)
    local ped = mp.actors[self:getInputValue(2,data)[0]+1]
    local weapon = self:getInputValue(3,data)[0]
    local ammo = self:getInputValue(4,data)[0]

    requestModel(getWeapontypeModel(ID_Weapons[weapon]))
    while not hasModelLoaded(getWeapontypeModel(ID_Weapons[weapon])) do
        wait(0)
    end
    giveWeaponToChar(ped, ID_Weapons[weapon], ammo)
    setCurrentCharWeapon(ped,1)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('takeAllWeapActor')
node.static.Name = new.char[65]('nodeTakeAllWeapActor')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('actor','Combo', new.int(0), 'vr.temp_var.list_name_actors'),
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)
    local ped = mp.actors[self:getInputValue(2,data)[0]+1]

    removeAllCharWeapons(ped)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.actor[#sNodes.Nodes.actor+1] = node
bitser.registerClass(node)

node = Node:subclass('openDoorsCar')
node.static.Name = new.char[65]('nodeOpenDoorsCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
            Socket('door_car_1','Bool', new.bool(false)),
            Socket('door_car_2','Bool', new.bool(false)),
            Socket('door_car_3','Bool', new.bool(false)),
            Socket('door_car_4','Bool', new.bool(false)),
            Socket('door_car_5','Bool', new.bool(false)),
            Socket('door_car_6','Bool', new.bool(false)),
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)
    local car = mp.cars[self:getInputValue(2,data)[0]+1]

    closeAllCarDoors(car)
    for g = 1,6 do
        if self:getInputValue(g+2,data)[0] then
            openCarDoor(car,g-1)
        end
    end

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.car[#sNodes.Nodes.car+1] = node
bitser.registerClass(node)

node = Node:subclass('lockDoorsCar')
node.static.Name = new.char[65]('nodeLockDoorsCar')
node.static.Type = new.ImU16(node_types.func)
function node:initialize(pos)
    self.points = {}
    Node.initialize(self,
        pos,
        {
            Socket('','wire'),
            Socket('car','Combo', new.int(0), 'vr.temp_var.list_name_cars'),
            Socket('doors','SliderScalar', new.ImU8(0),{imgui.DataType.U8,new.ImU8(0),new.ImU8(1),'vr.temp_var.open_close'}),
        },
        {
            Socket('','wire'),
        }
    )
end
function node:play(data)
    local car = mp.cars[self:getInputValue(2,data)[0]+1]
    local doors = self:getInputValue(3,data)[0]+1

    lockCarDoors(car,doors)

    if self.Outputs[1].link then
        for k,v in pairs(self.Outputs[1].link) do
            data.nodes[data.links[v].InputIdx]:play(data)
        end
    end
end
sNodes.Nodes.car[#sNodes.Nodes.car+1] = node
bitser.registerClass(node)

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
    imgui.BeginChild("scrolling_region", imgui.ImVec2(0, 0), true, imgui.WindowFlags.NoScrollbar + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoScrollWithMouse);
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
                if node.Inputs[slot_idx].type ~= 'Button' then
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
                        if n.Inputs[socket_idx].type ~= 'Button' then
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
    imgui.SetNextWindowSize(imgui.ImVec2(0,200))
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