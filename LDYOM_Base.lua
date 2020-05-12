require 'libstd.deps' {
    'fyp:mimgui',
    'kikito:middleclass',
 }

imgui = require 'mimgui'
new = imgui.new
bitser = require 'ldyom.bitser'
class = require 'ldyom.middleclass'
nodes = require 'ldyom.nodes'
ffi = require 'ffi'
inicfg = require 'inicfg'
vkeys = require 'vkeys'
mimgui_addon = require "ldyom.mimgui_addons"
lfs = require 'ldyom.lfs_ffi'
faicons = require 'ldyom.fAwesome5'
carSelector = require 'ldyom.carSelector'
mad = require 'MoonAdditions'
local memory = require 'memory'
encoding = require 'encoding'
cyr = encoding.CP1251
encoding.default = 'UTF-8'

local pedsSkinAtlas
local weaponsAtlas

ID_Actors = {0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288}

ID_Spec_Actors = {'andre','bbthin','bb','cat','cesar','claude',"dwayne",'emmet','forelli','janitor','jethro','jizzy','hern','kendl','maccer','maddogg','ogloc','paul','pulaski','rose','ryder','ryder3','sindaco','smoke','smokev','suzie','sweet','tbone','tenpen','torino','truthy','wuzimu','zero','gangrl2','copgrl1', 'copgrl2','crogrl1', 'crogrl2','gungrl1','gungrl2','mecgrl2','nurgrl2','ryder2','cdeput','sfpdm1','lvpdm1','csbmydj','psycho','csmech','csomost','wmycd2'}

ID_Cars = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611}


ID_Weapons = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46}

vr = {
    mainMenu = new.bool(),
    settings = new.bool(),
    targets = new.bool(),
    actors = new.bool(),
    cars = new.bool(),
    temp_var = {
        theme_curr = new.int(0),
        curr_lang = new.int(0),
        list_name_targets = {},
        list_name_actors = {},
        list_name_cars = {},
        selTarget = new.int(0),
        selTypeTarget = new.int(0),
        updateSphere = false,
        moveTarget = -1,
    },
    current_target = new.int(0),
    list_targets = {},
    current_actor = new.int(0),
    current_car = new.int(0),
    list_actors = {},
    list_cars = {},
    camera_zoom = 5,
    camera_angle = {45,0}
}
LanguageList = {}
langt = {}

function imgui.TextColoredRGB(text)
    local style = imgui.GetStyle()
    local colors = style.Colors
    local ImVec4 = imgui.ImVec4
    
    local explode_argb = function(argb)
        local a = bit.band(bit.rshift(argb, 24), 0xFF)
        local r = bit.band(bit.rshift(argb, 16), 0xFF)
        local g = bit.band(bit.rshift(argb, 8), 0xFF)
        local b = bit.band(argb, 0xFF)
        return a, r, g, b
    end

    local getcolor = function(color)
        if color:sub(1, 6):upper() == 'SSSSSS' then
            local r, g, b = colors[1].x, colors[1].y, colors[1].z
            local a = tonumber(color:sub(7, 8), 16) or colors[1].w * 255
            return ImVec4(r, g, b, a / 255)
        end
        local color = type(color) == 'string' and tonumber(color, 16) or color
        if type(color) ~= 'number' then return end
        local r, g, b, a = explode_argb(color)
        return imgui.ImVec4(r, g, b, a)
    end

    local render_text = function(text_)
        for w in text_:gmatch('[^\r\n]+') do
            local text, colors_, m = {}, {}, 1
            w = w:gsub('{(......)}', '{%1FF}')
            while w:find('{........}') do
                local n, k = w:find('{........}')
                local color = getcolor(w:sub(n + 1, k - 1))
                if color then
                    text[#text], text[#text + 1] = w:sub(m, n - 1), w:sub(k + 1, #w)
                    colors_[#colors_ + 1] = color
                    m = n
                end
                w = w:sub(1, n - 1) .. w:sub(k + 1, #w)
            end
            if text[0] then
                for i = 0, #text do
                    imgui.TextColored(colors_[i] or colors[1], text[i])
                    imgui.SameLine(nil, 0)
                end
                imgui.NewLine()
            else imgui.Text(w) end
        end
    end
    render_text(text)
end

function table.moveCell(table,inn,outt)
    local new_t = {}
    for i = 1,#table do
        if i ~= inn then
            new_t[#new_t+1] = table[i]
        end
        if i == outt then
            new_t[#new_t+1] = table[inn]
        end
    end
    return new_t
end

function fif(condition, if_true, if_false)
    if condition then return if_true else return if_false end
end

function for_each_vehicle_material(car, func)
    local i = 1
	for _, comp in ipairs(mad.get_all_vehicle_components(car)) do
		for _, obj in ipairs(comp:get_objects()) do
			for _, mat in ipairs(obj:get_materials()) do
                func(i,mat, comp, obj)
                i = i + 1
			end
		end
	end
end

function get_car_color_rgba(id)
	local col_table = memory.getuint32(0x4C8390, true)
	local clr = memory.getuint32(col_table + (id * 4))
	return clr
end

imgui.OnInitialize(function ()
    local theme = require('Theme\\'..vr.Data.Settings.curr_theme)
    theme.apply_custom_style()

    pedsSkinAtlas = imgui.CreateTextureFromFile(getWorkingDirectory().."\\lib\\ldyom\\images\\peds.jpg")
    weaponsAtlas = imgui.CreateTextureFromFile(getWorkingDirectory().."\\lib\\ldyom\\images\\weapons.png")

    local config = imgui.ImFontConfig()
    config.MergeMode = true
    config.PixelSnapH = true
    config.SizePixels = 14.0;
    config.FontDataOwnedByAtlas = false
    config.GlyphOffset.y = 1.0 -- смещение на 1 пиксеот вниз
    local fa_glyph_ranges = new.ImWchar[3]({ faicons.min_range, faicons.max_range, 0 })
    -- icon
    local faicon = imgui.GetIO().Fonts:AddFontFromMemoryCompressedBase85TTF(faicons.get_font_data_base85(), config.SizePixels, config, fa_glyph_ranges)
end)



local res = {}
res.x,res.y = getScreenResolution()

--Главное меню
imgui.OnFrame(function() return vr.mainMenu[0] end,
function()
    imgui.SetNextWindowSize(imgui.ImVec2(200,400),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-100,res.y/2-200),imgui.Cond.Always)
    imgui.Begin(langt['mainMenu'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)


    local size_b = imgui.ImVec2(160,0)
    if imgui.Button(faicons.ICON_RUNNING..' '..langt['targets'],size_b) then
        vr.mainMenu[0] = false
        vr.targets[0] = true
    end

    if imgui.Button(faicons.ICON_MALE..' '..langt['actors'],size_b) then
        vr.actors[0] = not vr.actors[0]
        vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_CARS..' '..langt['cars'],size_b) then
      vr.cars[0] = not vr.cars[0]
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_ARCHIVE..' '..langt['objects'],size_b) then
      vr.objects[0] = not vr.objects[0]
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_STARS..' '..langt['particles'],size_b) then
      vr.particle[0] = not vr.particle[0]
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_HEART..' '..langt['pickups'],size_b) then
      vr.pickup[0] = true
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_BOMB..' '..langt['explosions'],size_b) then
      vr.explosion[0] = true
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_PARKING..' '..langt['player'],size_b) then
      vr.editmode_player = true
      printHelpForever('HPLA')
      imgui.Process = false
    end
    imgui.Separator()
    if imgui.Button(faicons.ICON_THEATER_MASKS..' '..langt['missions'],size_b) then
      vr.missions[0] = not vr.missions[0]
      vr.mainMenu[0] = false
    end
    imgui.Separator()
    if imgui.Button(faicons.ICON_PLAY..' '..langt['missionStart'],size_b) then
      vr.miss_start = true
    end
    if imgui.Button(faicons.ICON_TOOLS..' '..langt['tools'],size_b) then
      vr.mainMenu[0] = false
      vr.tool[0] = true
    end
    imgui.Separator()
    if imgui.Button(faicons.ICON_INFO..' '..langt['info'],size_b) then
      vr.mainMenu[0] = false
      vr.info[0] = true
    end

    if imgui.Button(faicons.ICON_CONGS..' '..langt['settings'],size_b) then
        vr.mainMenu[0] = false
        vr.settings[0] = true
    end

    imgui.End()
end)

--target
imgui.OnFrame(function() return vr.targets[0] end,
function()

    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_RUNNING..' '..langt['targets'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_target,new('const char* const [?]', #vr.temp_var.list_name_targets, vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets,15)

    if imgui.Button(langt['add']) then
        imgui.OpenPopup("addT")
    end
    if #vr.list_targets > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
        imgui.SameLine()
        if imgui.Button(langt['cut']) then
            vr.temp_var.moveTarget = vr.current_target[0]+1
        end
        if vr.temp_var.moveTarget ~= -1 then
            imgui.SameLine()
            if imgui.Button(langt['paste']) then
                vr.list_targets = table.moveCell(vr.list_targets,vr.temp_var.moveTarget,vr.current_target[0]+1)
                vr.temp_var.list_name_targets = table.moveCell(vr.temp_var.list_name_targets,vr.temp_var.moveTarget,vr.current_target[0]+1)
                vr.temp_var.moveTarget = -1
            end
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()
    -- if imgui.BeginPopupContextItem('hee') then
    --     imgui.InputText('',vr.buf_edit_targets_name,ffi.sizeof(vr.buf_edit_targets_name))
    --     if imgui.Button(langt['apply']) and vr.buf_edit_targets_name[0] ~= '' then
    --     vr.temp_var.list_targets[vr.lb_cur_targets[0]+1]['Name'] =  ffi.string(vr.buf_edit_targets_name)
    --     vr.temp_var.list_name_targets[vr.lb_cur_targets[0]+1] = ffi.string(vr.buf_edit_targets_name)
    --     imgui.StrCopy(vr.buf_edit_targets_name,'')
    --     imgui.CloseCurrentPopup()
    --     end
    --     imgui.PushItemWidth(-1);
    --     imgui.PopItemWidth();
    --     imgui.EndPopup();
    -- end


    -- add target popup
    if imgui.BeginPopup("addT") then
        if imgui.Combo(langt['typeTarget'],vr.temp_var.selTarget,new('const char* const [?]', #vr.temp_var.type_targets_name, vr.temp_var.type_targets_name),#vr.temp_var.type_targets_name) then
            vr.temp_var.selTypeTarget[0] = 0
            if vr.temp_var.selTarget[0] == 3 then
                vr.temp_var.target_type = decodeJson(langt['target_type'])
            elseif vr.temp_var.selTarget[0] == 4 then
                vr.temp_var.target_type = decodeJson(langt['target_type_obj'])
            elseif vr.temp_var.selTarget[0] == 6 then
                vr.temp_var.target_type = decodeJson(langt['target_type_ev'])
            end
        end

        if vr.temp_var.selTarget[0] == 3 or vr.temp_var.selTarget[0] == 4 or vr.temp_var.selTarget[0] == 6 then
            imgui.Combo(langt['type'],vr.temp_var.selTypeTarget,new('const char* const [?]', #vr.temp_var.target_type, vr.temp_var.target_type),#vr.temp_var.target_type)
        end

        if imgui.Button(langt['create']) then
            vr.list_targets[#vr.list_targets+1] = {
                ['name'] = new.char[65](langt['target'].." #"..#vr.list_targets),
                ['type'] = new.int(vr.temp_var.selTarget[0]),
            }
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            if vr.temp_var.selTarget[0] == 0 then
                vr.list_targets[#vr.list_targets].data = {
                    ['pos'] = new.float[3](px,py,pz),
                    ['radius'] = new.float(2),
                    ['text'] = new.char[129](),
                    ['textTime'] = new.float(2),
                    ['colorBlip'] = new.int(0),
                }
            end
            local targets_name = {}
            for i = 1,#vr.list_targets do
                targets_name[#targets_name+1] = vr.list_targets[i].name
            end
            vr.temp_var.list_name_targets = targets_name
            vr.current_target[0] = #vr.list_targets-1
            imgui.CloseCurrentPopup()
        end

        imgui.EndPopup()
    end

    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            table.remove(vr.list_targets,vr.current_target[0]+1)
            table.remove(vr.temp_var.list_name_targets,vr.current_target[0]+1)
            if vr.current_target[0] > 0 then 
                vr.current_target[0] = vr.current_target[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_targets[vr.current_target[0]+1].name,ffi.sizeof(vr.list_targets[vr.current_target[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- target render
    if #vr.list_targets > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_RUNNING..' '..langt['target'],nil, imgui.WindowFlags.AlwaysAutoResize)

        --Checkpoint
        lockPlayerControl(true)
        if vr.list_targets[vr.current_target[0]+1].type[0] == 0 then
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local px,py,pz = getCharCoordinates(PLAYER_PED)
                vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = px
                vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = py
                vr.list_targets[vr.current_target[0]+1]['data'].pos[2] = pz
            end
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerCoordinates'])
            end
            imgui.SameLine()
            imgui.PushItemWidth(270)
            imgui.InputFloat3(langt['position'],vr.list_targets[vr.current_target[0]+1]['data']['pos'],"%.6f")
            imgui.PushItemWidth(-130)
            if imgui.DragFloat(langt['radiusCheckpoint'],vr.list_targets[vr.current_target[0]+1]['data']['radius'],0.05,0,100) then
                vr.updateSphere = true
            end
            imgui.Combo(langt['colorMarker'],vr.list_targets[vr.current_target[0]+1]['data']['colorBlip'],new('const char* const [?]', #vr.temp_var.targets_marker_color, vr.temp_var.targets_marker_color),#vr.temp_var.targets_marker_color)
            imgui.InputText(langt['textTarget'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
            imgui.InputFloat(langt['timeText'],vr.list_targets[vr.current_target[0]+1]['data']['textTime'])
        

            imgui.SetNextWindowBgAlpha(0.50)
            imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
            imgui.SetNextWindowSize(imgui.ImVec2(220,60),imgui.Cond.Always)
            imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

            imgui.Text(vr.temp_var.infoOverlay[1])
            imgui.Text(vr.temp_var.infoOverlay[2])
            imgui.Text(vr.temp_var.infoOverlay[3])

            imgui.End()

            isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()

            --edit
            
            local cx,cy,cz = vr.list_targets[vr.current_target[0]+1]['data'].pos[0],vr.list_targets[vr.current_target[0]+1]['data'].pos[1],vr.list_targets[vr.current_target[0]+1]['data'].pos[2]
            cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
            setFixedCameraPosition(cx,cy,cz)
            pointCameraAtPoint(vr.list_targets[vr.current_target[0]+1]['data'].pos[0],vr.list_targets[vr.current_target[0]+1]['data'].pos[1],vr.list_targets[vr.current_target[0]+1]['data'].pos[2],2)
            if not isWindow then
                if imgui.IsMouseDragging(0) then
                    local dt = imgui.GetIO().MouseDelta
                    vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                end
                if imgui.GetIO().MouseWheel ~= 0 then
                    vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                    if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                end
            end
            if isKeyDown(vkeys.VK_UP) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = vr.list_targets[vr.current_target[0]+1]['data'].pos[0] + 0.1
            elseif isKeyDown(vkeys.VK_DOWN) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = vr.list_targets[vr.current_target[0]+1]['data'].pos[0] - 0.1
            end
            if isKeyDown(vkeys.VK_LEFT) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = vr.list_targets[vr.current_target[0]+1]['data'].pos[1] + 0.1
            elseif isKeyDown(vkeys.VK_RIGHT) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = vr.list_targets[vr.current_target[0]+1]['data'].pos[1] - 0.1
            end
        end

        imgui.End()
    end
end)

--actor
imgui.OnFrame(function() return vr.actors[0] end,
function()

    local isWindow = false

    if not vr.temp_var.editActor then

        imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_MALE..' '..langt['actors'],nil, imgui.WindowFlags.AlwaysAutoResize)

        --List
        imgui.SetNextItemWidth(255)
        imgui.ListBoxStr_arr('', vr.current_actor,new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors,15)

        if imgui.Button(langt['add']) then
            vr.list_actors[#vr.list_actors+1] = {
                ['name'] = new.char[65](langt['actor'].." #"..#vr.list_actors),
            }
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            local angle = getCharHeading(PLAYER_PED)
            vr.list_actors[#vr.list_actors].data = {
                ['pos'] = new.float[3](px,py,pz),
                ['angle'] = new.float(angle),
                ['modelId'] = new.int(0),
                ['modelType'] = new.int(0),
                ['slotSkin'] = new.int(1),
                ['startC'] = new.int(0),
                ['endC'] = new.int(0),
                ['shouldNotDie'] = new.bool(false),
                ['health'] = new.int(100),
                ['weapon'] = new.int(1),
                ['ammo'] = new.int(0),

            }
            local actors_name = {}
            for i = 1,#vr.list_actors do
                actors_name[#actors_name+1] = vr.list_actors[i].name
            end
            vr.temp_var.list_name_actors = actors_name
            vr.current_actor[0] = #vr.list_actors-1
            upd_actor:run(vr.current_actor[0]+1)
        end
        if #vr.list_actors > 0 then
            imgui.SameLine()
            if imgui.Button(langt['rename']) then
                imgui.OpenPopup("rename")
            end
            imgui.SameLine()
            if imgui.Button(langt['delete']) then
                imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
            end
        end

        isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


        if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
            imgui.Text(langt['deleteQues'])

            local size_b = imgui.ImVec2(100,0)

            if imgui.Button(langt['yes'],size_b) then
                deleteChar(vr.list_actors[vr.current_actor[0]+1]['data']['char'])
                table.remove(vr.list_actors,vr.current_actor[0]+1)
                table.remove(vr.temp_var.list_name_actors,vr.current_actor[0]+1)
                if vr.current_actor[0] > 0 then 
                    vr.current_actor[0] = vr.current_actor[0] - 1
                end
                imgui.CloseCurrentPopup()
            end
            imgui.SameLine()
            if imgui.Button(langt['no'],size_b) then
                imgui.CloseCurrentPopup()
            end
        end

        --Rename popup
        if imgui.BeginPopup("rename") then

            imgui.InputText('',vr.list_actors[vr.current_actor[0]+1].name,ffi.sizeof(vr.list_actors[vr.current_actor[0]+1].name))

            if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

            imgui.EndPopup()
        end

        imgui.End()

        -- actor render
        if #vr.list_actors > 0 then
            imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
            imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
            imgui.Begin(faicons.ICON_MALE..' '..langt['actor'],nil, imgui.WindowFlags.AlwaysAutoResize)


            lockPlayerControl(true)
            imgui.PushIDStr("position")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local px,py,pz = getCharCoordinates(PLAYER_PED)
                pz = getGroundZFor3dCoord(px,py,pz)
                vr.list_actors[vr.current_actor[0]+1]['data'].pos[0] = px
                vr.list_actors[vr.current_actor[0]+1]['data'].pos[1] = py
                vr.list_actors[vr.current_actor[0]+1]['data'].pos[2] = pz
                setCharCoordinates(vr.list_actors[vr.current_actor[0]+1]['data']['char'],px,py,pz)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerCoordinates'])
            end

            imgui.SameLine()
            imgui.PushItemWidth(270)
            if imgui.InputFloat3(langt['position'],vr.list_actors[vr.current_actor[0]+1]['data']['pos'],"%.6f") then
                local xx,xy,xz = vr.list_actors[vr.current_actor[0]+1]['data']['pos'][0],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][1],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][2]
                setCharCoordinates(vr.list_actors[vr.current_actor[0]+1]['data']['char'],xx,xy,xz)
            end

            imgui.PushIDStr("angle")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local angle = getCharHeading(PLAYER_PED)
                vr.list_actors[vr.current_actor[0]+1]['data'].angle[0] = angle
                setCharHeading(vr.list_actors[vr.current_actor[0]+1]['data']['char'], angle)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerAngle'])
            end

            imgui.SameLine()
            if imgui.DragFloat(langt['angle'], vr.list_actors[vr.current_actor[0]+1]['data'].angle,1,0,360) then
                setCharHeading(vr.list_actors[vr.current_actor[0]+1]['data']['char'], vr.list_actors[vr.current_actor[0]+1]['data']['angle'][0])
            end

            imgui.Separator()

            if imgui.SliderInt('',vr.list_actors[vr.current_actor[0]+1]['data'].modelType,0,1,tostring(fif(vr.list_actors[vr.current_actor[0]+1]['data'].modelType[0] == 0,langt['ped'],langt['specActor']))) then
                vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] = 0
                upd_actor:run(vr.current_actor[0]+1)
            end

            
            --skin popup open
            if vr.list_actors[vr.current_actor[0]+1]['data'].modelType[0] == 0 then
                if imgui.Button(faicons.ICON_TSHIRT) then
                    imgui.OpenPopup('skins')
                end
                

                imgui.SameLine()

                if imgui.InputInt(langt['model'],vr.list_actors[vr.current_actor[0]+1]['data'].modelId) then
                    local id_a = 0
                    for v = 1,#ID_Actors do
                        if vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] <= ID_Actors[v] then
                            id_a = ID_Actors[v]
                            break
                        end
                    end
                    vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] = id_a
                    upd_actor:run(vr.current_actor[0]+1)
                end
            else
                if imgui.SliderInt(langt['slot'], vr.list_actors[vr.current_actor[0]+1]['data'].slotSkin, 1, 10) or imgui.Combo(langt['model'], vr.list_actors[vr.current_actor[0]+1]['data']['modelId'], new('const char* const [?]', #ID_Spec_Actors, ID_Spec_Actors),#ID_Spec_Actors) then
                    upd_actor:run(vr.current_actor[0]+1)
                end
            end
            imgui.Separator()

            if imgui.TreeNodeStr(langt['сhartics']) then
                imgui.InputInt(langt['countlive'],vr.list_actors[vr.current_actor[0]+1]['data']['health'])
                
                if imgui.ImageButton(weaponsAtlas, imgui.ImVec2(52,52),imgui.ImVec2((vr.list_actors[vr.current_actor[0]+1]['data'].weapon[0]-1)*0.02272727272,0), imgui.ImVec2(vr.list_actors[vr.current_actor[0]+1]['data'].weapon[0]*0.02272727272,1)) then
                    imgui.OpenPopup('weapon')
                end

                imgui.SameLine()
                imgui.Text(langt["weapon"])

                if imgui.InputInt(langt['ammo'], vr.list_actors[vr.current_actor[0]+1]['data'].ammo) then
                    if vr.list_actors[vr.current_actor[0]+1]['data'].ammo[0] < 0 then vr.list_actors[vr.current_actor[0]+1]['data'].ammo[0] = 0 end 
                end

                 --weapon popup
                if imgui.BeginPopup('weapon') then
                    imgui.BeginChild('weapon',imgui.ImVec2(200,450))
            
                    for i = 1,#ID_Weapons do
                        imgui.PushIDStr(tostring(i))
                        if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                            vr.list_actors[vr.current_actor[0]+1]['data'].weapon[0] = i
                            upd_actor:run(vr.current_actor[0]+1)
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

                imgui.TreePop()
            end

            mimgui_addon.ToggleButton(langt['should_live'],vr.list_actors[vr.current_actor[0]+1]['data']['shouldNotDie'])
            

            imgui.Separator()

            if imgui.Button(langt['edithand']) then
                vr.temp_var.editActor = true
                deleteChar(vr.list_actors[vr.current_actor[0]+1]['data']['char'])


                
                local modelId = vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0]
                local weapon = vr.list_actors[vr.current_actor[0]+1]['data']['weapon'][0]
                if vr.list_actors[vr.current_actor[0]+1]['data']['modelType'][0] == 0 then
                    modell = vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0]
                    requestModel(modell)
                    while not hasModelLoaded(modell) do
                        wait(0)
                    end
                else
                    local modell_n = ID_Spec_Actors[vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0]+1]
                    loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor[0]+1]['data']['slotSkin'][0])
                    while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor[0]+1]['data']['slotSkin'][0]) do
                        wait(0)
                    end
                    modell = 290 + vr.list_actors[vr.current_actor[0]+1]['data']['slotSkin'][0]-1
                end
                setPlayerModel(PLAYER_HANDLE, modell)
                local xx,xy,xz = vr.list_actors[vr.current_actor[0]+1]['data']['pos'][0],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][1],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][2]
                setCharCoordinates(PLAYER_PED, xx, xy, xz)
                setCharHeading(PLAYER_PED, vr.list_actors[vr.current_actor[0]+1]['data']['angle'][0])
            end
            
            imgui.PushItemWidth(150)
            imgui.Separator()
            
            local list_tg_m = {langt['toend']}
            for ltg = 1,#vr.temp_var.list_name_targets do
                list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
            end
            imgui.Combo(langt['app_on'],vr.list_actors[vr.current_actor[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
            imgui.Combo(langt['dis_after'],vr.list_actors[vr.current_actor[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
            

            --edit
            imgui.SetNextWindowBgAlpha(0.50)
            imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
            imgui.SetNextWindowSize(imgui.ImVec2(220,60),imgui.Cond.Always)
            imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

            imgui.Text(vr.temp_var.infoOverlay[1])
            imgui.Text(vr.temp_var.infoOverlay[2])

            imgui.End()

            isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()

            
            local cx,cy,cz = vr.list_actors[vr.current_actor[0]+1]['data'].pos[0],vr.list_actors[vr.current_actor[0]+1]['data'].pos[1],vr.list_actors[vr.current_actor[0]+1]['data'].pos[2]
            cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
            setFixedCameraPosition(cx,cy,cz)
            pointCameraAtPoint(vr.list_actors[vr.current_actor[0]+1]['data'].pos[0],vr.list_actors[vr.current_actor[0]+1]['data'].pos[1],vr.list_actors[vr.current_actor[0]+1]['data'].pos[2]+1,2)
            if not isWindow then
                if imgui.IsMouseDragging(0) then
                    local dt = imgui.GetIO().MouseDelta
                    vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                end
                if imgui.GetIO().MouseWheel ~= 0 then
                    vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                    if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                end
            end

            --skin popup
            if imgui.BeginPopup('skins') then
                imgui.BeginChild('skins',imgui.ImVec2(210,450))
        

                for i = 1,#ID_Actors do
                    imgui.PushIDStr(tostring(i))
                    if imgui.ImageButton(pedsSkinAtlas,imgui.ImVec2(55,100),imgui.ImVec2(((i-1)*55)/14630,0),imgui.ImVec2((i*55)/14630,1)) then
                        vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] = ID_Actors[i]
                        upd_actor:run(vr.current_actor[0]+1)
                    end
                    imgui.PopID()
                    if i % 3 ~= 0 then
                        imgui.SameLine()
                    end
                end
        
                imgui.EndChild()
                imgui.EndPopup()
            end

            

            imgui.End()
        end
    else
        imgui.SetNextWindowBgAlpha(0.50)
        imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
        imgui.SetNextWindowSize(imgui.ImVec2(220,20),imgui.Cond.Always)
        imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

        imgui.Text(vr.temp_var.infoOverlay[4])

        imgui.End()
    end
end)

--car
imgui.OnFrame(function() return vr.cars[0] end,
function()

    local isWindow = false

    if not vr.temp_var.editcar then

        imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_MALE..' '..langt['cars'],nil, imgui.WindowFlags.AlwaysAutoResize)

        --List
        imgui.SetNextItemWidth(255)
        imgui.ListBoxStr_arr('', vr.current_car,new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars,15)

        if imgui.Button(langt['add']) then
            vr.list_cars[#vr.list_cars+1] = {
                ['name'] = new.char[65](langt['car'].." #"..#vr.list_cars),
            }
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            local angle = getCharHeading(PLAYER_PED)
            vr.list_cars[#vr.list_cars].data = {
                ['pos'] = new.float[3](px,py,pz),
                ['angle'] = new.float(angle),
                ['modelId'] = new.int(0),
                ['slotSkin'] = new.int(1),
                ['startC'] = new.int(0),
                ['endC'] = new.int(0),
				['shouldNotDie'] = new.bool(false),
				['health'] = new.int(1000),
				['bulletproof'] = new.bool(false),
				['fireproof'] = new.bool(false),
				['explosionproof'] = new.bool(false),
				['collisionproof'] = new.bool(false),
				['meleeproof'] = new.bool(false),
                ['tiresVulnerability'] = new.bool(false),
                ['extendedColor'] = new.bool(false),
                ['primaryColor'] = new.float[4](),
                ['secondaryColor'] = new.float[4](),

            }
            local cars_name = {}
            for i = 1,#vr.list_cars do
                cars_name[#cars_name+1] = vr.list_cars[i].name
            end
            vr.temp_var.list_name_cars = cars_name
            vr.current_car[0] = #vr.list_cars-1
            upd_car:run(vr.current_car[0]+1)
        end
        if #vr.list_cars > 0 then
            imgui.SameLine()
            if imgui.Button(langt['rename']) then
                imgui.OpenPopup("rename")
            end
            imgui.SameLine()
            if imgui.Button(langt['delete']) then
                imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
            end
        end

        isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


        if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
            imgui.Text(langt['deleteQues'])

            local size_b = imgui.ImVec2(100,0)

            if imgui.Button(langt['yes'],size_b) then
                deleteChar(vr.list_cars[vr.current_car[0]+1]['data']['char'])
                table.remove(vr.list_cars,vr.current_car[0]+1)
                table.remove(vr.temp_var.list_name_cars,vr.current_car[0]+1)
                if vr.current_car[0] > 0 then 
                    vr.current_car[0] = vr.current_car[0] - 1
                end
                imgui.CloseCurrentPopup()
            end
            imgui.SameLine()
            if imgui.Button(langt['no'],size_b) then
                imgui.CloseCurrentPopup()
            end
        end

        --Rename popup
        if imgui.BeginPopup("rename") then

            imgui.InputText('',vr.list_cars[vr.current_car[0]+1].name,ffi.sizeof(vr.list_cars[vr.current_car[0]+1].name))

            if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

            imgui.EndPopup()
        end

        imgui.End()

        -- car render
        if #vr.list_cars > 0 then
            imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
            imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
            imgui.Begin(faicons.ICON_MALE..' '..langt['car'],nil, imgui.WindowFlags.AlwaysAutoResize)


            lockPlayerControl(true)
            imgui.PushIDStr("position")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local px,py,pz = getCharCoordinates(PLAYER_PED)
                pz = getGroundZFor3dCoord(px,py,pz)
                vr.list_cars[vr.current_car[0]+1]['data'].pos[0] = px
                vr.list_cars[vr.current_car[0]+1]['data'].pos[1] = py
                vr.list_cars[vr.current_car[0]+1]['data'].pos[2] = pz
                setCarCoordinates(vr.list_cars[vr.current_car[0]+1]['data']['car'],px,py,pz)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerCoordinates'])
            end

            imgui.SameLine()
            imgui.PushItemWidth(270)
            if imgui.InputFloat3(langt['position'],vr.list_cars[vr.current_car[0]+1]['data']['pos'],"%.6f") then
                local xx,xy,xz = vr.list_cars[vr.current_car[0]+1]['data']['pos'][0],vr.list_cars[vr.current_car[0]+1]['data']['pos'][1],vr.list_cars[vr.current_car[0]+1]['data']['pos'][2]
                setCarCoordinates(vr.list_cars[vr.current_car[0]+1]['data']['car'],xx,xy,xz)
            end

            imgui.PushIDStr("angle")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local angle = getCharHeading(PLAYER_PED)
                vr.list_cars[vr.current_car[0]+1]['data'].angle[0] = angle
                setCarHeading(vr.list_cars[vr.current_car[0]+1]['data']['car'], angle)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerAngle'])
            end

            imgui.SameLine()
            if imgui.DragFloat(langt['angle'], vr.list_cars[vr.current_car[0]+1]['data'].angle,1,0,360) then
                setCarHeading(vr.list_cars[vr.current_car[0]+1]['data']['car'], vr.list_cars[vr.current_car[0]+1]['data']['angle'][0])
            end

            if imgui.Button(langt['selectCar']) then
                carSelector.showw(s)
                vr.cars[0] = false
            end

            imgui.Separator()

            if imgui.TreeNodeStr(langt['сhartics']) then
                
                mimgui_addon.ToggleButton(langt['extendedColor'],vr.list_cars[vr.current_car[0]+1]['data'].extendedColor)
                
                if vr.list_cars[vr.current_car[0]+1]['data'].extendedColor[0] then
                    if imgui.TreeNodeStr(langt['colors']) then
                        for_each_vehicle_material(vr.list_cars[vr.current_car[0]+1]['data'].car,function(i,mat, comp, obj)
                            
                            imgui.PushIDInt(i)
                            imgui.PushItemWidth(200)
                            if imgui.ColorEdit4(tostring(i), vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2],imgui.ColorEditFlags.AlphaBar+imgui.ColorEditFlags.NoInputs+imgui.ColorEditFlags.NoLabel) then
                                local new_r, new_g, new_b, a = vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3]
                                mat:set_color(new_r*255, new_g*255, new_b*255, a*255)
                            end
                            imgui.SameLine()
                            imgui.Text(string.format('%s:%s:%s', comp.name, mat:get_texture() and mat:get_texture().name or '[no texture]', bit.tohex(mat:get_pointer())))
                            imgui.PopItemWidth()
                            imgui.PopID()

                        end)
                    end
                else
                    if imgui.ColorEdit4("primaryColorE", vr.list_cars[vr.current_car[0]+1]['data'].primaryColor,imgui.ColorEditFlags.AlphaBar+imgui.ColorEditFlags.NoInputs+imgui.ColorEditFlags.NoLabel) then
                        for_each_vehicle_material(vr.list_cars[vr.current_car[0]+1]['data'].car,function(i,mat, comp, obj)
                            if vr.list_cars[vr.current_car[0]+1]['data'].colors[i][1] == 0 then
                                local r, g, b, a = vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[0],vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[1],vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[2],vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[3]
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0] = r
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1] = g
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2] = b
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3] = a
                                    mat:set_color(r*255, g*255, b*255, a*255)
                                end
                        end)
                    end
                    imgui.SameLine()
                    imgui.Text(langt['primaryColor'])
                    
                    if imgui.ColorEdit4("secondaryColorE", vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor,imgui.ColorEditFlags.AlphaBar+imgui.ColorEditFlags.NoInputs+imgui.ColorEditFlags.NoLabel) then
                        for_each_vehicle_material(vr.list_cars[vr.current_car[0]+1]['data'].car,function(i,mat, comp, obj)
                            if vr.list_cars[vr.current_car[0]+1]['data'].colors[i][1] == 1 then
                                local r, g, b, a = vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[0],vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[1],vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[2],vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[3]
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0] = r
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1] = g
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2] = b
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3] = a
                                    mat:set_color(r*255, g*255, b*255, a*255)
                            end
                        end)
                    end
                    imgui.SameLine()
                    imgui.Text(langt['secondaryColor'])
                end
                
                imgui.Separator()
                
                imgui.InputInt(langt['countlive'],vr.list_cars[vr.current_car[0]+1]['data']['health'])
                
                mimgui_addon.ToggleButton(vr.temp_var.car_unbreak[1],vr.list_cars[vr.current_car[0]+1]['data']['bulletproof'])
                mimgui_addon.ToggleButton(vr.temp_var.car_unbreak[2],vr.list_cars[vr.current_car[0]+1]['data']['fireproof'])
                mimgui_addon.ToggleButton(vr.temp_var.car_unbreak[3],vr.list_cars[vr.current_car[0]+1]['data']['explosionproof'])
                mimgui_addon.ToggleButton(vr.temp_var.car_unbreak[4],vr.list_cars[vr.current_car[0]+1]['data']['collisionproof'])
                mimgui_addon.ToggleButton(vr.temp_var.car_unbreak[5],vr.list_cars[vr.current_car[0]+1]['data']['meleeproof'])
                mimgui_addon.ToggleButton(vr.temp_var.car_unbreak[6],vr.list_cars[vr.current_car[0]+1]['data']['tiresVulnerability'])
                
                imgui.TreePop()


            end

            mimgui_addon.ToggleButton(langt['should_live'],vr.list_cars[vr.current_car[0]+1]['data']['shouldNotDie'])
            



            imgui.Separator()

            if imgui.Button(langt['edithand']) then
                vr.temp_var.editcar = true
                deleteCar(vr.list_cars[vr.current_car[0]+1]['data']['car'])
            end
            
            imgui.PushItemWidth(150)
            imgui.Separator()
            
            local list_tg_m = {langt['toend']}
            for ltg = 1,#vr.temp_var.list_name_targets do
                list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
            end
            imgui.Combo(langt['app_on'],vr.list_cars[vr.current_car[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
            imgui.Combo(langt['dis_after'],vr.list_cars[vr.current_car[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
            

            --edit
            imgui.SetNextWindowBgAlpha(0.50)
            imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
            imgui.SetNextWindowSize(imgui.ImVec2(220,60),imgui.Cond.Always)
            imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

            imgui.Text(vr.temp_var.infoOverlay[1])
            imgui.Text(vr.temp_var.infoOverlay[2])

            imgui.End()

            isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()

            
            local cx,cy,cz = vr.list_cars[vr.current_car[0]+1]['data'].pos[0],vr.list_cars[vr.current_car[0]+1]['data'].pos[1],vr.list_cars[vr.current_car[0]+1]['data'].pos[2]
            cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
            setFixedCameraPosition(cx,cy,cz)
            pointCameraAtPoint(vr.list_cars[vr.current_car[0]+1]['data'].pos[0],vr.list_cars[vr.current_car[0]+1]['data'].pos[1],vr.list_cars[vr.current_car[0]+1]['data'].pos[2]+1,2)
            if not isWindow then
                if imgui.IsMouseDragging(0) then
                    local dt = imgui.GetIO().MouseDelta
                    vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                end
                if imgui.GetIO().MouseWheel ~= 0 then
                    vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                    if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                end
            end

            imgui.End()
        end
    else
        imgui.SetNextWindowBgAlpha(0.50)
        imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
        imgui.SetNextWindowSize(imgui.ImVec2(220,20),imgui.Cond.Always)
        imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

        imgui.Text(vr.temp_var.infoOverlay[4])

        imgui.End()
    end
end)

--settings
imgui.OnFrame(function() return vr.settings[0] end,
function ()
    imgui.SetNextWindowSize(imgui.ImVec2(200,400),imgui.Cond.Always)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-100,res.y/2-200),imgui.Cond.Always)
    imgui.Begin(faicons.ICON_CONGS..' '..langt['settings'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)

    imgui.PushItemWidth(125)
    if imgui.Combo(langt['style'],vr.temp_var.theme_curr,new('const char* const [?]', #vr.temp_var.theme_arr, vr.temp_var.theme_arr),#vr.temp_var.theme_arr) then
        vr.Data.Settings.curr_theme = ffi.string(vr.temp_var.theme_arr[vr.temp_var.theme_curr[0]+1])
        inicfg.save(vr.Data,getWorkingDirectory()..'\\LDYOM_data.ini')

        imgui.OpenPopup(faicons.ICON_EXCLAMATION_TRIANGLE.." "..langt['reload'])
    end

    if imgui.Combo(langt['lang'],vr.temp_var.curr_lang,new('const char* const [?]', #vr.temp_var.lang_list, vr.temp_var.lang_list),#vr.temp_var.lang_list) then
        vr.Data.Settings.curr_lang = LanguageList[vr.temp_var.curr_lang[0]+1]
        inicfg.save(vr.Data,getWorkingDirectory()..'\\LDYOM_data.ini')
        langt = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..vr.Data.Settings.curr_lang..'.ini')['Keys']
        updLang()
    end

    if imgui.BeginPopupModal(faicons.ICON_EXCLAMATION_TRIANGLE.." "..langt['reload'], nil,imgui.WindowFlags.AlwaysAutoResize) then

        imgui.Text(vr.temp_var.reloadQues[1])
        imgui.Text(vr.temp_var.reloadQues[2])
        imgui.Text(vr.temp_var.reloadQues[3])

        local size_b = imgui.ImVec2(160,0)

        if imgui.Button(langt['yes'],size_b) then
            thisScript():reload()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end

        imgui.EndPopup()
    end

    imgui.End()
end)

function updLang()
    vr.temp_var.type_targets_name = decodeJson(langt['targets_list_arr'])

    vr.temp_var.targets_marker_color = decodeJson(langt['targets_marker_color'])

    vr.temp_var.weap_names = decodeJson(langt['weap_names'])
    
    vr.temp_var.infoOverlay = decodeJson(langt['infoOverlay'])
    
    vr.temp_var.reloadQues = decodeJson(langt['reloadQues'])
    
    vr.temp_var.weap_names = decodeJson(langt['weap_names'])
    
    vr.temp_var.car_unbreak = decodeJson(langt['car_unbreak'])
end

function update_actor(actorr)
	local modell
	local xx,xy,xz = vr.list_actors[actorr]['data']['pos'][0], vr.list_actors[actorr]['data']['pos'][1], vr.list_actors[actorr]['data']['pos'][2]
	local angle = vr.list_actors[actorr]['data']['angle'][0]

	if vr.list_actors[actorr]['data']['modelType'][0] == 0 then
		modell = vr.list_actors[actorr]['data']['modelId'][0]
		requestModel(modell)
		while not hasModelLoaded(modell) do
			wait(0)
		end
	elseif vr.list_actors[actorr]['data']['modelType'][0] == 1 then
		local modell_n = ID_Spec_Actors[vr.list_actors[actorr]['data']['modelId'][0]+1]
		loadSpecialCharacter(modell_n,vr.list_actors[actorr]['data']['slotSkin'][0])
		while not hasSpecialCharacterLoaded(vr.list_actors[actorr]['data']['slotSkin'][0]) do
			wait(0)
		end
		modell = 290 + vr.list_actors[actorr]['data']['slotSkin'][0]-1
	end

	deleteChar(vr.list_actors[actorr]['data']['char'])
	print(modell)
	vr.list_actors[actorr]['data']['char'] = createChar(4,modell,xx,xy,xz)
	if vr.list_actors[actorr]['data']['modelType'][0] == 0 then
		markModelAsNoLongerNeeded(modell)
	else
		unloadSpecialCharacter(vr.list_actors[actorr]['data']['slotSkin'][0])
	end
	setCharHeading(vr.list_actors[actorr]['data']['char'], angle)
	setCharCollision(vr.list_actors[actorr]['data']['char'],false)
	requestModel(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]]))
	while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]])) do
		wait(0)
	end
	giveWeaponToChar(vr.list_actors[actorr]['data']['char'], ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]], 1)
	setCurrentCharWeapon(vr.list_actors[actorr]['data']['char'],1)
	markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]]))
end

function update_car(carr)
	local modell = vr.list_cars[carr]['data']['modelId'][0]
	for v = 1,#ID_Cars do
		if vr.list_cars[carr]['data']['modelId'][0] <= ID_Cars[v] then
			modell = ID_Cars[v]
			break
		end
	end
	local xx,xy,xz = vr.list_cars[carr]['data']['pos'][0], vr.list_cars[carr]['data']['pos'][1], vr.list_cars[carr]['data']['pos'][2]
	local angle = vr.list_cars[carr]['data']['angle'][0]

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
	end

	deleteCar(vr.list_cars[carr]['data']['car'])
	vr.list_cars[carr]['data']['car'] = createCar(modell, xx, xy, xz)
	setCarHeading(vr.list_cars[carr]['data']['car'], angle)
	setCarCollision(vr.list_cars[carr]['data']['car'],false)
	freezeCarPosition(vr.list_cars[carr]['data']['car'], true)
	setCarCanBeDamaged(vr.list_cars[carr]['data']['car'], false)
	--changeCarColour(vr.list_cars[carr]['data']['car'], vr.list_cars[carr]['data']['Color_primary'],vr.list_cars[carr]['Car_Data']['Color_secondary'])
    markModelAsNoLongerNeeded(modell)
    vr.list_cars[carr]['data'].colors = {}
    local comps = mad.get_all_vehicle_components(vr.list_cars[carr]['data']['car'])
    for_each_vehicle_material(vr.list_cars[carr]['data'].car,function(i,mat, comp, obj)
        local r,g,b,a = mat:get_color()
        local colorr
        local typee
        if r == 0x3C and g == 0xFF and b == 0x00 then
            local prim = getCarColours(vr.list_cars[carr]['data'].car)
            colorr = imgui.ImColor.ImColorU32(get_car_color_rgba(prim))
            vr.list_cars[carr]['data'].primaryColor[0] = colorr.Value.x
            vr.list_cars[carr]['data'].primaryColor[1] = colorr.Value.y
            vr.list_cars[carr]['data'].primaryColor[2] = colorr.Value.z
            vr.list_cars[carr]['data'].primaryColor[3] = colorr.Value.w
            typee = 0
        elseif r == 0xFF and g == 0x00 and b == 0xAF then
            local _, sec = getCarColours(vr.list_cars[carr]['data'].car)
            colorr = imgui.ImColor.ImColorU32(get_car_color_rgba(sec))
            vr.list_cars[carr]['data'].secondaryColor[0] = colorr.Value.x
            vr.list_cars[carr]['data'].secondaryColor[1] = colorr.Value.y
            vr.list_cars[carr]['data'].secondaryColor[2] = colorr.Value.z
            vr.list_cars[carr]['data'].secondaryColor[3] = colorr.Value.w
            typee = 1
        else
            colorr = imgui.ImColor.ImColorInt(r, g, b, a)
            typee = 2
        end
        vr.list_cars[carr]['data'].colors[#vr.list_cars[carr]['data'].colors+1] = {typee,new.float[4](colorr.Value.x,colorr.Value.y,colorr.Value.z,colorr.Value.w)}

    end)
end

function main()

    bitser.register('my_ctype', ffi.typeof(ffi.new("int",1)))
    bitser.register('my_ctype1', ffi.typeof(ffi.new("int",67)))
    
    local var = {new.int(23),5,new.bool(true)}
    local str = bitser.dumps(var)
    print(str)
    local var1 = bitser.loads(str)
    print(var1[1],var1[3][0])
    print(var1[1],var1[3][0])

    

    vr.Data = inicfg.load(nil,getWorkingDirectory()..'\\LDYOM_data.ini')

    --Load lang
    local lang_list = {}
    for file in lfs.dir(getWorkingDirectory() .. '\\Language') do
        if lfs.attributes(getWorkingDirectory() .. '\\Language\\'..file,"mode") == "file" then
            if string.sub(file, -3) == 'ini' then
                local lang = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..file)
                lang_list[#lang_list+1] = lang['Info']['name']
                LanguageList[#LanguageList+1] = string.sub(file, 0, -5)--lang['Info']['name']
            end
        end
    end
    vr.temp_var.lang_list = lang_list
    for i,v in ipairs(LanguageList) do
        if v == vr.Data.Settings.curr_lang then
            vr.temp_var.curr_lang[0] = i-1
        end
    end
    langt = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..vr.Data.Settings.curr_lang..'.ini')['Keys']
    wait(1)
    updLang()


    --Load Theme
    local theme_list = {}
    for file in lfs.dir(getWorkingDirectory() .. '\\lib\\Theme') do
        if lfs.attributes(getWorkingDirectory() .. '\\lib\\Theme\\'..file,"mode") == "file" then
            if string.sub(file, -3) == 'lua' then
                print(string.sub(file, 0,-5))
                theme_list[#theme_list+1] = string.sub(file, 0,-5)
            end
        end
    end
    vr.temp_var.theme_arr = theme_list
    for i,v in ipairs(theme_list) do
        if v == vr.Data.Settings.curr_theme then
            vr.temp_var.theme_curr[0] = i-1
        end
    end
    
    upd_actor = lua_thread.create_suspended(update_actor)
    upd_car = lua_thread.create_suspended(update_car)

    nodes_s.show[0] = true
    while true do
        wait(0)
        

        if testCheat('top2009') then
			printHelpString('~r~I LOVE TWENTY ONE PILOTS')
        end


        if vr.temp_var.editActor then
            lockPlayerControl(false)
            restoreCamera()
            showCursor(false)
            if isKeyJustPressed(vkeys.VK_F) then
                vr.temp_var.editActor = false

                setPlayerModel(PLAYER_HANDLE, 0)
                local xx,xy,xz = getCharCoordinates(PLAYER_PED)
                local angle = getCharHeading(PLAYER_PED)
                xz = getGroundZFor3dCoord(xx,xy,xz)
                vr.list_actors[vr.current_actor[0]+1]['data']['pos'][0] = xx
                vr.list_actors[vr.current_actor[0]+1]['data']['pos'][1] = xy
                vr.list_actors[vr.current_actor[0]+1]['data']['pos'][2] = xz
                vr.list_actors[vr.current_actor[0]+1]['data']['angle'][0] = angle
                upd_actor:run(vr.current_actor[0]+1)
                lockPlayerControl(true)
            end
        end
        
        if vr.updateSphere then
            wait(0)
            vr.updateSphere = false
        else
            for i = 1, #vr.list_targets do
                if vr.list_targets[i]['type'][0] == 0 then
                    drawSphere(vr.list_targets[i]['data']['pos'][0], vr.list_targets[i]['data']['pos'][1], vr.list_targets[i]['data']['pos'][2],vr.list_targets[i]['data']['radius'][0])
                end
            end
        end
        if isKeyJustPressed(vkeys.VK_E) then
            if vr.mainMenu[0] or vr.settings[0] or vr.targets[0] or vr.actors[0] or vr.cars[0] or carSelector.show then
                vr.mainMenu[0] = false
                vr.settings[0] = false
                vr.targets[0] = false
                vr.actors[0] = false
                vr.cars[0] = false
                carSelector.show = false
                wait(0)
                lockPlayerControl(false)
                restoreCamera()
            else
                vr.mainMenu[0] = true
            end
        end
    end

end