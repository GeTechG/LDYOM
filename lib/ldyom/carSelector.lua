script_authors("Um Geek")
script_url("https://gta-geek.blogspot.com.br/")

-- BETA VERSION 1.7
inicfg = require 'inicfg'
ffi = require 'ffi'
local memory = require 'memory' -- uso em filtros
local encoding = require 'encoding'
local rename_vehicles = nil -- 

encoding = require 'encoding'
u8 = encoding.UTF8
encoding.default = 'CP1251'

ex = {}

show = false

local path = string.format('%s\\moonloader\\lib\\ldyom\\resource\\call vehicle by image\\',getGameDirectory())
local texts = {
    tittle = '',
    readme = '', -- load file
    text_1 = 'Marcar veiculo como indiscartavel',
    text_2 = '',
    text_3 = 'Ops! indisponivel!',    
    text_4 = '',
    text_5 = 'Fechar interface depois de chamar', 
    text_6 = 'Settings',
    text_7 = 'Spawnar e entrar no veiculo',  
    text_8 = 'Usar nome gxt na lista',   
    text_9 = 'Buscar pelo nome gxt', 
    text_10 = 'Mostrar em janela grande ao entrar',  
    text_11 = 'Add/remove favorite',   
    text_12 = '', 
    text_13 = 'Ainda nГЈo possui veiculos no historico',
    text_14 = 'Spawnar helicopteros voando.',   
    text_15 = 'Mostrar filtros como lista',   
    text_16 = 'Carregar imagens ao iniciar jogo',                  
}

local settings = {
    ['settings'] = {
        ['no_remove_vehicle'] = false,
        ['exit_on_spawner'] = true, 
        ['warp_in_vehicle'] = true,
        ['use_gxt_name_in_list'] = false,
        ['find_using_gxt_name'] = false,
        ['gallery_columns'] = 10,
        ['gallery_image_size'] = 97.0,
        ['gallery_num_pictures'] = 70,
        ['show_gallery'] = true,
        ['spawn_helis_flying'] = true,
        ['preload'] = false,  
        ['filter_list'] = true,       
    }
}

local ini = texts.tittle..'.ini'
local presets = inicfg.load(nil,ini);
if presets then 
    settings = presets
else 
    inicfg.save(settings,ini)
end

local list_types = 0
local break_process = false
local pictures = {}
local favorites = {}
local gui = {
    no_remove = new.bool(settings['settings']['no_remove_vehicle']),
    exit = new.bool(settings['settings']['exit_on_spawner']),
    gxt = new.bool(settings['settings']['use_gxt_name_in_list']),
    warp = new.bool(settings['settings']['warp_in_vehicle']),
    find_gxt = new.bool(settings['settings']['find_using_gxt_name']),    
    gallery_columns = new.int(settings['settings']['gallery_columns']),
    gallery_num_pictures = new.int(settings['settings']['gallery_num_pictures']), 
    gallery_image_size = new.float(settings['settings']['gallery_image_size']),  
    show_gallery = new.bool(settings['settings']['show_gallery']),  
    spawn_helis_flying = new.bool(settings['settings']['spawn_helis_flying']), 
    preload = new.bool(settings['settings']['preload']),
    filter_list = new.bool(settings['settings']['filter_list']),           
    configs = new.bool(false),
    add_to_favorites = new.bool(false),
    string1 = new.char[24](''),
    string2 = new.char[24](''),
}

local filter_names = {
    [0] = 'All',
    [1] = 'Cars',
    [2] = 'Planes',
    [3] = 'Boats',
    [4] = 'Helis',
    [5] = 'Others',
}

function save_data_settings()
    settings['settings']['no_remove_vehicle'] = gui.no_remove[0]
    settings['settings']['exit_on_spawner'] = gui.exit[0]
    settings['settings']['warp_in_vehicle'] = gui.warp[0] 
    settings['settings']['use_gxt_name_in_list'] = gui.gxt[0]  
    settings['settings']['find_using_gxt_name'] = gui.find_gxt[0]
    settings['settings']['gallery_columns'] = gui.gallery_columns[0]
    settings['settings']['gallery_num_pictures'] = gui.gallery_num_pictures[0]
    settings['settings']['gallery_image_size'] = gui.gallery_image_size[0] 
    settings['settings']['show_gallery'] = gui.show_gallery[0]  
    settings['settings']['spawn_helis_flying'] = gui.spawn_helis_flying[0] 
    settings['settings']['preload'] = gui.preload[0]   
    settings['settings']['filter_list'] = gui.filter_list[0]     
    inicfg.save(settings,ini)
end

local show_vehicle_gallery = false
local show_favorites = false
local show_rename = nil
local start_in_number = 0
local enable_window = false
local show_filter_list = false
local show_gallery_settings = false
local popup_menu_active = -1

------------------------------------------------
-- Command keys
------------------------------------------------
local commands = {
    rename = 0x52, -- r
    favorite = 0x46, -- f
    key_1 = 0x57, -- w 
    key_2 = 0x55, -- u
}

local cabs = {
    [1] = {403,514,515}, -- trucks
    [2] = {485,583,606,607}, -- baggages
    [3] = {531}, -- tractor
    [4] = {552}, -- utility
    [5] = {525}, -- towtruck
}

local trailes = {
    [1] = {435,450,584,591}, -- trucks
    [2] = {606,607,608}, -- baggages
    [3] = {610}, -- tractor
    [4] = {611}, -- utility
    [5] = {} -- towtruck
}
local Jetpack
if doesFileExist(getGameDirectory()..'\\moonloader\\rename_vehicles.lua') then 
    rename_vehicles = import 'rename_vehicles.lua'
end

imgui.OnInitialize(
function()
    Jetpack = imgui.CreateTextureFromFile(string.format('%sJetpack-icon.png',path))
    if #pictures <= 0 then 
        if not gui.preload[0] then
            load_all_vehicles()
        end
    end
end)

function attach_trailer_in_vehicle(vehicle,trailer)
    local rs = 0
    local m1 = getCarModel(vehicle)
    local m2 = getCarModel(trailer)
    if isThisModelACar(m2) then 
        trailes[5][1] = m2 
    end 
    for i=1, #cabs do 
        for j=1, #cabs[i] do 
            if cabs[i][j] == m1 then 
                rs = i 
                break 
            end
        end
    end
    if trailes[rs] then 
        for i=1, #trailes[rs] do 
            if trailes[rs][i] == m2 then 
                local prev_t = localGetTrailerInVehicle(vehicle)
                if doesVehicleExist(prev_t) then
                    deleteCar(prev_t)
                end
                attachTrailerToCab(trailer,vehicle)
                return true
            end
        end
    end
    return false
end

imgui.OnFrame(function() return show end,
function() 
    local sx, sy = getScreenResolution()
    local sw,sh = 420,120
    local imgw,imgh = sh+30,(sh+30)/2
    local win_bas_w = 280
    local win_bas_h = sy-(sh+12)    
    if show then
        imgui.SetNextWindowPos(imgui.ImVec2(6,sy-(sh+3)),imgui.Cond.FirstUseEver)
        imgui.SetNextWindowSize(imgui.ImVec2(sx-10,sh), imgui.Cond.FirstUseEver)
        imgui.Begin('Vehicle List')
        --
        if not show_vehicle_gallery and not gui.configs[0] then
            local itens = pictures 
            if show_favorites then 
                itens = {}
                for i=1, #favorites do 
                    for j,e in ipairs(pictures) do
                        if e.id == favorites[i] then 
                            itens[i] = e
                        end
                    end
                end
            end
            --imgui.Separator()
            for i,e in ipairs(itens) do
                if model_filter_1(e.id) then
                    local name = e.name
                    if gui.find_gxt[0] then 
                        name = getNameOfVehicleModel(e.id)
                    end
                    if find_string_in_string(ffi.string(gui.string1),name) then
                        if imgui.ImageButton(e.pic,imgui.ImVec2(imgw,imgh),imgui.ImVec2(0,0),imgui.ImVec2(1,1),1,imgui.ImVec4(0,0,0,0),imgui.ImVec4(1,1,1,1)) then 
                            quick_vehicle_spawner(e)
                        end 
                        if imgui.IsItemHovered() then
                            imgui.SetTooltip(string.format('%s:%s',e.id,e.name))
                        end                  
                        if i < #itens then 
                            imgui.SameLine()
                        end                        
                    end
                end
            end          
            if imgui.IsKeyPressed(commands.favorite) then 
                show_favorites = not show_favorites
            end
        else 
            --imgui.Separator()
            if gui.configs[0] then
                if not show_gallery_settings then
                    if imgui.Button('Save settings',imgui.ImVec2(200.0,20))  then 
                        save_data_settings()
                        gui.configs[0] = not gui.configs[0]  
                    end   
                    imgui.SameLine()
                    if imgui.Button('Gallery settings',imgui.ImVec2(200.0,20)) then 
                        show_gallery_settings = not show_gallery_settings 
                        show_vehicle_gallery = true
                    end 
                    imgui.SameLine()
                    if imgui.Button('Exit settings',imgui.ImVec2(200.0,20)) then 
                        show_gallery_settings = false 
                        gui.configs[0] = false
                    end 

                    imgui.Separator()                    
                    imgui.BeginGroup()
                    imgui.Checkbox(texts.text_1, gui.no_remove) 
                    imgui.Checkbox(texts.text_5, gui.exit) 
                    imgui.Checkbox(texts.text_7, gui.warp) 
                    imgui.EndGroup()

                    imgui.SameLine()
                    imgui.BeginGroup()
                    imgui.Checkbox(texts.text_9, gui.find_gxt) 
                    imgui.Checkbox(texts.text_10, gui.show_gallery) 
                    imgui.Checkbox(texts.text_14, gui.spawn_helis_flying)
                    imgui.EndGroup() 

                    imgui.SameLine()
                    imgui.BeginGroup()
                    imgui.Checkbox(texts.text_15, gui.filter_list) 
                    imgui.Checkbox(texts.text_16, gui.preload)      
                    imgui.Checkbox(texts.text_8, gui.gxt) 
                    imgui.EndGroup()  
                else 
                    if imgui.Button('Save settings',imgui.ImVec2(200.0,20))  then 
                        save_data_settings()
                        gui.configs[0] = not gui.configs[0]  
                    end  
                    imgui.SameLine()
                    if imgui.Button('Exit gallery settings',imgui.ImVec2(200.0,20)) then 
                        show_gallery_settings = not show_gallery_settings 
                    end   
                    imgui.SameLine()
                    if imgui.Button('Exit settings',imgui.ImVec2(200.0,20)) then 
                        show_gallery_settings = false 
                        gui.configs[0] = false
                    end 
                    imgui.Separator()
                    imgui.SliderFloat("Images size", gui.gallery_image_size, imgw/6, imgw*8)        
                    imgui.InputInt('Pictures',gui.gallery_num_pictures,1,#pictures,0)
                    imgui.InputInt('Columns',gui.gallery_columns,1,100,0)
                end 
            else
                if isCharInAnyCar(PLAYER_PED) then
                    local vehicle_data = ''
                    local veh_e = nil
                    local veh_i = -1
                    local veh_h = nil
                    local veh_n = ''
                    for i,e in ipairs(pictures) do
                        if isCharInModel(PLAYER_PED,e.id) then 
                            veh_e = e
                            veh_i = i
                            veh_h = getCarCharIsUsing(PLAYER_PED)
                            break
                        end
                    end
                    if veh_e then
                        imgui.BeginGroup()
                        imgui.Image(veh_e.pic,imgui.ImVec2(120,80),imgui.ImVec2(0,0),imgui.ImVec2(1,1),imgui.ImVec4(1,1,1,1),imgui.ImVec4(0,0,0,0))
                        imgui.EndGroup()

                        imgui.SameLine()
                        imgui.BeginGroup()        
                        imgui.Text(string.format('Model: %s\nName: %s',veh_e.id,veh_e.name))
                        imgui.Separator() 
                        if imgui.Button('Fix vehicle',imgui.ImVec2(150.0,22)) then 
                            fixCar(veh_h)
                            break_process = true
                        end             
                        imgui.EndGroup()
                    end                                                           
                end
            end 
        end
        imgui.End()

        if true then
            imgui.SetNextWindowPos(imgui.ImVec2(6,4),imgui.Cond.FirstUseEver)
            imgui.SetNextWindowSize(imgui.ImVec2(win_bas_w,win_bas_h), imgui.Cond.FirstUseEver)
            imgui.Begin(texts.tittle)
            imgui.Separator() 
            if imgui.MenuItemBool('Close Spawner','CL', false, true) then 
                show = false 
            end 
            -- settings
            imgui.Separator() 
            if imgui.MenuItemBool('Settings','ST', false, true) then 
                gui.configs[0] = not gui.configs[0]  
            end 
            -- 
            imgui.Separator()  
            if not show_vehicle_gallery then
                if imgui.MenuItemBool('Full Display','D1', false, true) then 
                    show_vehicle_gallery = true 
                end    
            else 
                if imgui.MenuItemBool('Small Display','D2', false, true) then 
                    show_vehicle_gallery = false 
                end    
            end     
            --
            imgui.Separator() 
            if imgui.MenuItemBool('show ( '..filter_names[list_types]..' )','SH', false, true) then 
                if not gui.filter_list[0] then
                    list_types = list_types + 1
                    if list_types > #filter_names then 
                        list_types = 0 
                    end
                else 
                    show_filter_list = not show_filter_list
                end
            end 
            if show_filter_list and gui.filter_list[0] then 
                for i=0, #filter_names do 
                    imgui.Bullet()
                    if imgui.MenuItemBool(filter_names[i],'FT'..i, false, true) then 
                        list_types = i
                    end
                end
            end

            imgui.Separator() 
            if popup_menu_active <= -1 then
                imgui.InputText("Find",gui.string1,ffi.sizeof(gui.string1))
                imgui.Separator() 
            end

            for i,e in ipairs(pictures) do
                if model_filter_1(e.id) then
                    local name = e.name
                    if gui.find_gxt[0] then 
                        name = getNameOfVehicleModel(e.id)
                    end
                    if find_string_in_string(ffi.string(gui.string1),name) then
                        if popup_menu_active ~= e.id then
                            imgui.Bullet()
                            if imgui.MenuItemBool(name, tostring('M'..e.id), false, true) then 
                            
                            end 
                            if imgui.IsItemHovered() then
                                imgui.BeginTooltip()
                                imgui.Image(e.pic,imgui.ImVec2(100,66),imgui.ImVec2(0,0),imgui.ImVec2(1,1),imgui.ImVec4(1,1,1,1),imgui.ImVec4(0,0,0,0))
                                imgui.EndTooltip()
                                if imgui.IsMouseClicked(0,false) then 
                                    if popup_menu_active <= -1 then
                                        quick_vehicle_spawner(e)
                                    end
                                elseif imgui.IsMouseClicked(1,false) then 
                                    popup_menu_active = e.id
                                    imgui.StrCopy(gui.string2,e.name)
                                end 
                            end  
                        else 
                            -- start rename
                            if rename_vehicles then
                                imgui.Separator()                                
                                if rename_vehicles.check(e.id) then 
                                    imgui.Text(e.name)
                                    imgui.Separator() 
                                    if imgui.MenuItemBool('Reset name','RES', false, true) then 
                                        rename_vehicles.rename(nil,e.id)
                                        load_vehicle_names()
                                        gui.string2[0] = ''  
                                        popup_menu_active = -1     
                                    end    
                                else 
                                    imgui.InputText("Name",gui.string2,ffi.sizeof(gui.string2))
                                    imgui.Separator() 
                                    if imgui.MenuItemBool('Rename vehicle','REM', false, true)  then 
                                        rename_vehicles.rename(gui.string2[0],e.id)
                                        load_vehicle_names()
                                        gui.string2[0] = ''
                                        popup_menu_active = -1 
                                    end
                                end
                            else 
                                imgui.Separator() 
                                imgui.Bullet()
                                imgui.Text(e.name)
                                imgui.Separator() 
                            end
                            --- end rename    
                            -- start favorite 
                            if not check_item_int_list(e.id,favorites) then
                                if imgui.MenuItemBool('Add favorite','ADD', false, true) then 
                                    table.insert(favorites,e.id)
                                    popup_menu_active = -1 
                                end
                            else 
                                if imgui.MenuItemBool('Remove favorite','REF', false, true) then 
                                    for f=1, #favorites do 
                                        if favorites[f] == e.id then 
                                            table.remove(favorites,f)
                                        end
                                    end
                                    popup_menu_active = -1 
                                end 
                            end    
                            if imgui.MenuItemBool('Cancel','CAN', false, true) then 
                                popup_menu_active = -1 
                            end                    
                            -- end favorite
                            imgui.Separator()  
                        end
                    end  
                end  
            end
            imgui.End()
        end

        if show_vehicle_gallery then 
            local drawInX = 8 + win_bas_w
            imgui.SetNextWindowPos(imgui.ImVec2(drawInX,4),imgui.Cond.FirstUseEver)
            imgui.SetNextWindowSize(imgui.ImVec2(sx-(drawInX+4),win_bas_h), imgui.Cond.FirstUseEver)
            imgui.Begin('Vehicle Gallery')
            local itens = 0
            local gallery_num_pictures = 0
            local line_return = function() 
                gallery_num_pictures = gallery_num_pictures + 1
                if itens < (gui.gallery_columns[0] - 1)then 
                    imgui.SameLine()
                    itens = itens + 1
                else 
                    itens = 0
                end
            end
            imgui.Separator()  
            if imgui.Button('<<',imgui.ImVec2(90.0,20)) then
                if start_in_number >= gui.gallery_num_pictures[0] then
                    start_in_number = start_in_number - gui.gallery_num_pictures[0] 
                end
            end 
            imgui.SameLine()
            if imgui.Button('>>',imgui.ImVec2(90.0,20)) then
                start_in_number = start_in_number + gui.gallery_num_pictures[0] 
            end    
            imgui.SameLine()
            if show_favorites then
                if imgui.Button('Home',imgui.ImVec2(120.0,20)) then
                    start_in_number = 0
                    show_favorites = false
                end     
            else 
                if imgui.Button('Favorites ( '..#favorites..' )',imgui.ImVec2(120.0,20)) then
                    start_in_number = 0
                    show_favorites = true
                end                    
            end
            imgui.SameLine()
            imgui.Text((start_in_number+1)..' to '..(start_in_number + gui.gallery_num_pictures[0]))
            imgui.Separator() 
            local itens = pictures 
            if show_favorites then 
                itens = {}
                for i=1, #favorites do 
                    for j,e in ipairs(pictures) do
                        if e.id == favorites[i] then 
                            itens[i] = e
                        end
                    end
                end
            end
            for i,e in ipairs(itens) do
                local name = e.name
                if gui.find_gxt[0] then 
                    name = getNameOfVehicleModel(e.id)
                end
                if find_string_in_string(ffi.string(gui.string1),name) then
                    if start_in_number <= i then
                        if model_filter_1(e.id) and gallery_num_pictures < gui.gallery_num_pictures[0] then
                            if imgui.ImageButton(e.pic,imgui.ImVec2(gui.gallery_image_size[0],gui.gallery_image_size[0]/1.4),imgui.ImVec2(0,0),imgui.ImVec2(1,1),1,imgui.ImVec4(0,0,0,0),imgui.ImVec4(1,1,1,1)) then 
                                if not isKeyDown(commands.favorite) then
                                    quick_vehicle_spawner(e)
                                else 
                                    if show_favorites then
                                        table.remove(favorites,i)
                                    else
                                        if not check_item_int_list(e.id,favorites) then
                                            table.insert(favorites,e.id)
                                        end
                                    end
                                end
                            end 
                            if imgui.IsItemHovered() then
                                local in_favorite = '' 
                                for i=1, #favorites do 
                                    if favorites[i] == e.id then 
                                        in_favorite = ' ( Favorite )'
                                    end
                                end
                                imgui.SetTooltip(string.format('%s\n%s%s',e.id,e.name,in_favorite))
                            end   
                            line_return()
                        end
                    end
                end
            end
            imgui.End()
        end
    end
end)

local VEHICLE = nil
function quick_vehicle_spawner(e)
    local model = e.id
    vr.list_cars[vr.current_car[0]+1]['data'].modelId[0] = model
    show = false
    vr.cars[0] = true
    upd_car:run(vr.current_car[0]+1)
end

function localGetTrailerInVehicle(car)
	local tralier = getVehiclePointerHandle(readMemory(getCarPointer(car)+1224,4,false))
	if doesVehicleExist(tralier)then return tralier end
	return -1
end

function create_new_vehicle(model,x,y,z,angle)
    local vehicle = createCar(model,x,y,z)
    lockCarDoors(vehicle,0)
    setCarHeading(vehicle,angle)
    return vehicle
end

function load_all_vehicles()
    if #pictures <= 0 then 
        for i=400, 611 do 
            if isModelAvailable(i) then
                local name = getNameOfVehicleModel(i)
                if not gui.gxt[0] then
                    name = getGxtText(name)
                    name = u8:decode(name) 
                end
                if name == '' then name = 'Model-'..i end
                table.insert(pictures,{name=name,id=i,pic=imgui.CreateTextureFromFile(string.format('%sVehicle_%i.jpg',path,i))})
            end
        end
        load_extra_models();
    end
end

function load_vehicle_names()
    for i,e in ipairs(pictures) do
        if isModelAvailable(e.id) then
            local name = getNameOfVehicleModel(e.id)
            if not gui.gxt[0] then
                name = getGxtText(name)
                name = u8:decode(name) 
            end
            if name == '' then name = 'Model-'..e.id end
            pictures[i].name = name
        end
    end
end

function ex.showw()
    load_vehicle_names()
    setPlayerControl(PLAYER_HANDLE,false)
    imgui.StrCopy(gui.string1,'')
    break_process = false
    show = true
    show_vehicle_gallery = gui.show_gallery[0]
end

function ex.hide()
    imgui.StrCopy(gui.string1,'')
    break_process = false
    show = false
end

function load_extra_models()
    local data = path..'_extras.txt'
    if doesFileExist(data) then 
        local handle = io.open(data, "r")
        for line in handle:lines() do
            local s = string.format('%s%i.jpg',path,line)
            if doesFileExist(s) then
                local i = tonumber(line)
                if isModelAvailable(i) then
                    local name = getNameOfVehicleModel(i)
                    if not gui.gxt[0] then
                        name = getGxtText(name)
                        name = u8:decode(name) 
                    end
                    if name == '' then name = 'Model-'..i end
                    table.insert(pictures,{name=name,id=i,pic=imgui.CreateTextureFromFile(s)})
                end
            else
                --print('No found:'..s)
            end
        end 
        io.close(handle) -- close file  
    end
end

function onExitScript(quitGame)
    for i,e in ipairs(pictures) do
        imgui.ReleaseTexture(e.pic)
    end
end

function load_lua_in_text_file(file_name)
	if doesFileExist(file_name) then 
		local hFile = io.open(file_name,"r") -- open file
		if hFile then
			local script_string = hFile:read("*a");
            io.close(hFile) -- close file
            if type(script_string)=='string' then
                return loadstring(script_string)()
            end
		end
	end
	return nil  
end 

local ex_filters = {}
function load_ex_filters()
    local filters_path = getGameDirectory()..'\\moonloader\\lib\\lib\\resource\\call vehicle by image\\_filters\\'
    for i=6, 36 do 
        local ft_path = filters_path..'filter_'..(i - 5)..'.fil'
        if not doesFileExist(ft_path) then 
            ft_path = filters_path..'filter_'..(i - 5)..'.lua'
        end
        local z_filter = load_lua_in_text_file(ft_path)
        if z_filter then 
            filter_names[i] = z_filter.name 
            ex_filters[i] = z_filter.filter
        end
    end
end
load_ex_filters()

function model_filter_1(id)
    if list_types == 0 then 
        return true
    elseif isThisModelACar(id) and list_types == 1 then 
        return true
    elseif isThisModelAPlane(id) and list_types == 2 then 
        return true
    elseif isThisModelABoat(id) and list_types == 3 then 
        return true
    elseif isThisModelAHeli(id) and list_types == 4 then 
        return true 
    elseif list_types == 5 and not model_filter_2(id)  then  
        return true    
    else 
        local ex_filter = ex_filters[list_types]
        if ex_filter then
            return ex_filter(id)
        end
    end
    return false
end

function model_filter_2(id)
    if isThisModelACar(id) 
    or isThisModelAPlane(id)
    or isThisModelABoat(id)
    or isThisModelAHeli(id) then
        return true
    end 
    return false
end

function model_filter_3(id)
    local trains_models = {449,537,538,569,570,586}
    for i=1, #trains_models do 
        if id == trains_models[i] then
            return true
        end
    end
    return false
end

function ex_filter_list(id,list)
    for i=1, #list do 
        if id == list[i] then 
            return true
        end
    end
    return false
end

function convert_string_to_table(str)
    local t = {}
    --print(str)
    for i = 1, #str do
        t[i] = str:sub(i, i)
    end
    return t
end

function find_string_in_string(s1,s2)
    if s1 and s2 then 
        local t1 = convert_string_to_table(s1)
        local t2 = convert_string_to_table(s2)
        t1 = convert_string_case(t1)
        t2 = convert_string_case(t2)
        local comp = ''
        if #t1 <= 0 then return true end
        for i=1, #t1 do comp = comp..t1[i] end
        for i=1, #t2 do 
            local result = ''
            for j=i, #t2 do 
                result = result..t2[j]
                if comp == result then 
                    return true
                end
            end
        end
    end
    return false
end

function convert_string_case(t2)
    local t3 = convert_string_to_table("ABCDEFGHIJKLMNOPQRSTUVXYWZ")
    local t4 = convert_string_to_table("abcdefghijklmnopqrstuvxywz")
    for i,out in ipairs(t3) do
        for j=1, #t2 do 
            if t2[j] == out then 
                t2[j] = t4[i]
            end               
        end
    end
    return t2
end

function check_item_int_list(item,list)
    for _,item2 in ipairs(list) do
        if item == item2 then 
            return true
        end
    end
    return false
end

function save_json_file(name,list)
	local file_name = string.format("%s\\moonloader\\config\\%s.json",getGameDirectory(),thisScript().name..'-'..name);
	local hFile = io.open(file_name,"w");
	if hFile then
		hFile:write(encodeJson(list));
		hFile:close();
		--print(string.format("config/%s.json saved!",name));
	end
end

function load_json_file(name)
	local file_name = string.format("%s\\moonloader\\config\\%s.json",getGameDirectory(),thisScript().name..'-'..name);
	if doesFileExist(file_name) then 
		local hFile = io.open(file_name,"r"); -- open file
		if hFile then
			local text = hFile:read("*a");
            io.close(hFile); -- close file
            --print(string.format("config/%s.json loaded!",name));
			return decodeJson(text);
		end
	end
	return {};
end

texts.readme = texts.readme .. 'Com o click secundario do mouse em um item da lista de nomes abre um submenu com algumas opcoes como renomear e adicionar a favoritos\n'
texts.readme = texts.readme .. 'No modo SMALL use a tecla F pra navegar entre favoritos e todos os veiculos\n'
texts.readme = texts.readme .. 'Visualizacoes de imagens de modelos adicionados so aparecera caso o modelo esteja disponivel em jogo mesmo que configurado\n'
texts.readme = texts.readme .. 'Veja as configuracoes disponiveis no menu Settings, incluindo algumas funcoes do mod que vem desligadas.\n'
texts.readme = texts.readme .. 'Estando fora de um veiculo pode acessar o JETPACK clicando na imagem do lado. Caso esteja em um veiculo vera informacoes do mesmo.'

return ex