script_name('rename_vehicles')
script_author('Um Geek')

local memory = require 'memory' -- uso em filtros
local support = {}
local chars = {}
local names = {}
local buffer = 0x00969110

local settings = {
    KEY_1 = 0x11; -- CTRL    
    KEY_2 = 0x4E; -- N
}

local texts = {
    text_0 = '~y~DEL~w~: Restaurar ~n~~y~ENTER~w~: Confirma ~n~~y~BACK~w~: Apagar letra ~n~~y~TAB~w~:Cancelar';
    text_1 = 'Renomeado com sucesso!';
    text_2 = 'Restaurado com sucesso!';
    text_3 = 'Digite um novo nome';
}
setGxtEntry('REMVE1',texts.text_0)

function support.save_json_file(name,list)
	local file_name = string.format("%s\\moonloader\\config\\%s.json",getGameDirectory(),thisScript().name..'-'..name);
	local hFile = io.open(file_name,"w");
	if hFile then
		hFile:write(encodeJson(list));
		hFile:close();
	end
end

function support.load_json_file(name)
	local file_name = string.format("%s\\moonloader\\config\\%s.json",getGameDirectory(),thisScript().name..'-'..name);
	if doesFileExist(file_name) then 
		local hFile = io.open(file_name,"r"); -- open file
		if hFile then
			local text = hFile:read("*a");
            io.close(hFile); -- close file
			return decodeJson(text);
		end
	end
	return {};
end

function support.apply_names()
    for i,e in ipairs(names) do 
        if e.gxt and e.rename and e.name then 
            if e.reset then 
                setGxtEntry(e.gxt,e.name)
                names[i] = nil
            else 
                setGxtEntry(e.gxt,e.rename)
            end
        else 
            names[i] = nil
        end
    end
end

function support.input_name_in_list(data) 
    local result = false
    for i=1, #names do 
        if names[i].model == data.model then 
            if data.rename then
                names[i].rename = data.rename
                printHelpString(texts.text_1)
            else 
                if names[i].name then
                    names[i].reset = true
                    printHelpString(texts.text_2)
                end
            end
            result = true
            break
        end
    end 
    if not result then 
        data.gxt = getNameOfVehicleModel(data.model)
        data.name = getGxtText(data.gxt)
        table.insert(names,data)
        printHelpString(texts.text_1)
    end
end

function support.get_vehicle_name_data(model)
    for i=1, #names do 
        if names[i].model == model then 
            return names[i]
        end
    end 
    return nil
end

function support.convert_string_to_table(str)
    local t = {}
    for i = 1, #str do
        t[i] = str:sub(i, i)
    end
    return t
end

function support.rename_vehicle_name(name,model)
    support.input_name_in_list({rename=name,model=model}) 
    support.apply_names()
    support.save_json_file('names',names)
end

EXPORTS = {
    rename = support.rename_vehicle_name,
    check = support.get_vehicle_name_data,
}

function main()
    names = support.load_json_file('names')
    chars[1] = support.convert_string_to_table("ABCDEFGHIJKLMNOPQRSTUVXYWZ")
    chars[2] = support.convert_string_to_table("abcdefghijklmnopqrstuvxywz")
    support.apply_names()
    while true do 
        if isPlayerControlOn(PLAYER_PED) then
            if isKeyDown(settings.KEY_1) and wasKeyPressed(settings.KEY_2) then
                if isCharInAnyCar(PLAYER_PED) then 
                    local vehicle = getCarCharIsUsing(PLAYER_PED)
                    local model = getCarModel(vehicle)
                    local data = support.get_vehicle_name_data(model)
                    local input_name = ''
                    setPlayerControl(PLAYER_HANDLE,false)
                    setCameraBehindPlayer()
                    printHelpForever('REMVE1')
                    if data then 
                        input_name = data.rename
                    end
                    local ikey = memory.read(buffer,1,false)
                    while isKeyDown(ikey) do 
                        wait(4)
                    end
                    while not isKeyDown(0x0D) do -- enter 
                        local ikey = memory.read(buffer,1,false)
                        if wasKeyPressed(ikey) then
                            local skey = memory.tostring(buffer,1,false)
                            if not isKeyDown(0x10) then -- shift
                                for i=1, #chars[1] do 
                                    if skey == chars[1][i] then
                                        skey = chars[2][i]
                                    end
                                end
                            end
                            input_name = input_name .. skey
                        end
                        if wasKeyPressed(0x08) then -- back 
                            local t_name = support.convert_string_to_table(input_name)
                            input_name = ''
                            for i=1, (#t_name-1) do 
                                input_name = input_name ..t_name[i] 
                            end
                        elseif wasKeyPressed(0x20) then -- space 
                            input_name = input_name .. ' '
                        elseif wasKeyPressed(0x2E) then 
                            if data then 
                                input_name = nil
                                break
                            end  
                        elseif isKeyDown(0x09) then -- tab   
                            break               
                        end
                        printStringNow(string.format('%s~n~( ~y~%s~w~ )',texts.text_3,input_name),100)
                        wait(0)
                    end
                    clearHelp()
                    if not isKeyDown(0x09) then -- tab
                        support.input_name_in_list({rename=input_name,model=model}) 
                        support.apply_names()
                        support.save_json_file('names',names)
                    end
                    while isKeyDown(0x0D) do -- enter 
                        wait(4)
                    end 
                    setPlayerControl(PLAYER_HANDLE,true)
                    restoreCamera()
                end
            end
        end
        wait(4)
    end
end

function onExitScript(quitGame)
end
