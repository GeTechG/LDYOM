manager = {}

require 'libstd.deps' {
    'luarocks:json4lua',
 }

encoding = require 'encoding'
--debtab = require 'debug_table'
encoding.default = 'CP1251'
json = require 'json'
u8 = encoding.UTF8
path = getWorkingDirectory() .. "\\Missions_pack\\"
path_backup = getWorkingDirectory() .. "\\Backup\\"

local CountMPacks = 0

function manager.save(pack,num)
	local packk = {
		['Name'] = pack['Name'],
		['Missions'] = {}
	}
	for p = 1, #pack['Missions'] do
		packk['Missions'][p] = {
			['Name'] = pack['Missions'][p]['Name'],
			['Mission_Data'] = {
				['Targets'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Targets']),
				['Actors'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Actors']),
				['Cars'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Cars']),
				['Objects'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Objects']),
				['Pickup'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Pickup']),
				['Particle'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Particle']),
				['Explosion'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Explosion']),
				['Miss_data'] = {
					['Name'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Name'],
					['Time'] = {pack['Missions'][p]['Mission_Data']['Miss_data']['Time'][1],pack['Missions'][p]['Mission_Data']['Miss_data']['Time'][2]},
					['Weather'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Weather'][0],
					['Riot'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Riot'][0],
					['Player'] = {
						['Pos'] = {pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Pos'][1],pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Pos'][2],pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Pos'][3]},
						['Angle'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Angle'],
						['ModelId'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['ModelId'],
						['Weapon'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Weapon'],
						['Weap_ammo'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Weap_ammo'],
						['Interior_id'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Interior_id']
					}
				}
			}
		}
	end
	createDirectory(path_backup)
	local old_save
	if doesFileExist(path .. 'LDYOM' .. tostring(num) .. '.json') then
		local f = io.open(path .. 'LDYOM' .. tostring(num) .. '.json',"r")
		old_save = f:read()
		f:close()
		local f = io.open(path_backup .. 'LDYOM' .. tostring(num) .. '_' .. os.time() .. '.json',"w")
		f:write(old_save)
		f:close()
	end
	local f = io.open(path .. 'LDYOM' .. tostring(num) .. '.json',"w")
	f:write(json.encode(packk))
	f:close()
end

function manager.load(num)
	f = io.open(path .. 'LDYOM' .. tostring(num) .. '.json',"r")
	local pack = json.decode(f:read())
	f:close()
	packk = {
		['Name'] = pack['Name'],
		['Missions'] = {}
	}
	for p = 1, #pack['Missions'] do
		--print(debtab.tostring(pack['Missions'][1]))
		packk['Missions'][p] = {
			['Name'] = pack['Missions'][p]['Name'],
			['Prename'] = new.char[128](),
			['Enable'] = new.bool(false),
			['Mission_Data'] = {
				['Targets'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Targets'],0),
				['Actors'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Actors'],1),
				['Cars'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Cars'],2),
				['Objects'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Objects'],3),
				['Pickup'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Pickup'] or {},4),
				['Particle'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Particle'] or {},5),
				['Explosion'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Explosion'] or {},6),
				['Miss_data'] = {
					['Name'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Name'],
					['Time'] = {pack['Missions'][p]['Mission_Data']['Miss_data']['Time'][1],pack['Missions'][p]['Mission_Data']['Miss_data']['Time'][2]},
					['Weather'] = new.int(pack['Missions'][p]['Mission_Data']['Miss_data']['Weather']),
					['Riot'] = new.bool(pack['Missions'][p]['Mission_Data']['Miss_data']['Riot']),
					['Player'] = {
						['Pos'] = {pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Pos'][1],pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Pos'][2],pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Pos'][3]},
						['Angle'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Angle'],
						['ModelId'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['ModelId'],
						['Weapon'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Weapon'],
						['Weap_ammo'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Weap_ammo'],
						['Interior_id'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Player']['Interior_id']
					}
				}
			}
		}
	end
	return packk
end

function manager.delete(num)
	os.remove(path .. 'LDYOM' .. tostring(num) .. '.json')
end

function manager.sorterTable(orig)
	local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[manager.sorterTable(orig_key)] = manager.sorterTable(orig_value)
        end
		setmetatable(copy, manager.sorterTable(getmetatable(orig)))
	elseif orig_type == 'userdata' then
		if type(orig[0]) == 'userdata' then
			copy = {orig[0][1],orig[0][2],orig[0][3]}
		else
			copy = orig[0]
		end

    else
        copy = orig
    end
    return copy
end

function manager.sorterJson(table,typee)
	local tableEnd = {}
	if typee == 0 then
		for tt = 1, #table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = new.int(table[tt]['Type']),
				['Target_Data'] = {}
			}
			if 	tableEnd[tt]['Type'][0] == 0 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = new.float[3](table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][2],table[tt]['Target_Data']['Pos'][3]),
					['Radius'] = new.int(table[tt]['Target_Data']['Radius']),
					['Text'] = new.char[128](),
					['Text_time'] = new.float(table[tt]['Target_Data']['Text_time']),
					['Color_blip'] = new.int(table[tt]['Target_Data']['Color_blip']),
				}
				imgui.StrCopy(tableEnd[tt]['Target_Data']['Text'],table[tt]['Target_Data']['Text'])
			end
			if 	tableEnd[tt]['Type'][0] == 1 then
				tableEnd[tt]['Target_Data'] = {
				['Target_car_id'] = new.int(table[tt]['Target_Data']['Target_car_id']),
				['Color_blip'] = new.int(table[tt]['Target_Data']['Color_blip']),
				['Text'] = new.char[128]()
			}
			imgui.StrCopy(tableEnd[tt]['Target_Data']['Text'],ternar(table[tt]['Target_Data']['Text'] ~= nil, table[tt]['Target_Data']['Text'],''))
			end
			if tableEnd[tt]['Type'][0] == 2 then
				tableEnd[tt]['Target_Data'] = {
				['Target_actor_id'] = new.int(table[tt]['Target_Data']['Target_actor_id']),
				['Color_blip'] = new.int(table[tt]['Target_Data']['Color_blip']),
				['Text'] = new.char[128]()
			}
			imgui.StrCopy(tableEnd[tt]['Target_Data']['Text'],table[tt]['Target_Data']['Text'] or '')
			end
			if tableEnd[tt]['Type'][0] == 3 then
				tableEnd[tt]['Target_Data'] = {
					['Target_type'] = new.int(table[tt]['Target_Data']['Target_type']),
					['Pos'] = new.float[3](table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][2],table[tt]['Target_Data']['Pos'][3]),
					['Rotates'] = new.float[3](table[tt]['Target_Data']['Rotates'][1],table[tt]['Target_Data']['Rotates'][2],table[tt]['Target_Data']['Rotates'][3]),
					['Text'] = new.char[128](),
					['Text_time'] = new.float(table[tt]['Target_Data']['Text_time']),
					['Smooth'] = new.bool(table[tt]['Target_Data']['Smooth']),
					['Time'] = new.int(table[tt]['Target_Data']['Time']),
					['Weather'] = new.int(table[tt]['Target_Data']['Weather']),
					['Clock_time'] = table[tt]['Target_Data']['Clock_time'],
					['Traffic'] = {new.int(table[tt]['Target_Data']['Traffic'][1]),new.int(table[tt]['Target_Data']['Traffic'][2])}

				}
				imgui.StrCopy(tableEnd[tt]['Target_Data']['Text'],table[tt]['Target_Data']['Text'])
			end
			if tableEnd[tt]['Type'][0] == 4 then
				tableEnd[tt]['Target_Data'] = {
					['Target_object_id'] = new.int(table[tt]['Target_Data']['Target_type']),
					['Color_blip'] = new.int(table[tt]['Target_Data']['Color_blip']),
					['Target_type'] = new.int(table[tt]['Target_Data']['Target_type']),
					['Weap'] = new.int(table[tt]['Target_Data']['Weap']),
					['Text'] = new.char[128]()
				}
				imgui.StrCopy(tableEnd[tt]['Target_Data']['Text'],table[tt]['Target_Data']['Text'])
			end
			if tableEnd[tt]['Type'][0] == 5 then
				tableEnd[tt]['Target_Data'] = {
				['Target_pickup_id'] = new.int(table[tt]['Target_Data']['Target_pickup_id']),
				['Color_blip'] = new.int(table[tt]['Target_Data']['Color_blip']),
				['Text'] = new.char[128]()
			}
			imgui.StrCopy(tableEnd[tt]['Target_Data']['Text'],table[tt]['Target_Data']['Text'])
		end
			if tableEnd[tt]['Type'][0] == 6 then
				tableEnd[tt]['Target_Data'] = {
					['Target_type'] = new.int(table[tt]['Target_Data']['Target_type']),
					['Pos'] = new.float[3](table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][2],table[tt]['Target_Data']['Pos'][3]),
					['ModelID'] = new.int(table[tt]['Target_Data']['ModelID']),
					['Angle'] = new.float(table[tt]['Target_Data']['Angle']),
					['Weapon'] = new.int(table[tt]['Target_Data']['Weapon']),
					['Weap_ammo'] = new.int(table[tt]['Target_Data']['Weap_ammo']),
					['Anim'] = new.int(table[tt]['Target_Data']['Anim']),
					['Pack_anim'] = new.int(table[tt]['Target_Data']['Pack_anim']),
					['Loop'] = new.bool(table[tt]['Target_Data']['Loop']),
					['Car_id'] = new.int(table[tt]['Target_Data']['Car_id']),
					['Car_place'] = new.int(table[tt]['Target_Data']['Car_place']),
					['Level_battue'] = new.int(table[tt]['Target_Data']['Level_battue']),
					['Dialog'] = {},
					['Dialog_id'] = new.int(0),
					['Add_money'] = new.int(table[tt]['Target_Data']['Add_money']),
					['Interior_id'] = table[tt]['Target_Data']['Interior_id']
				}
				for d = 1,#table[tt]['Target_Data']['Dialog'] do
					tableEnd[tt]['Target_Data']['Dialog'][d] = {
						['Text'] = new.char[128](),
						['Text_time'] = new.float(table[tt]['Target_Data']['Dialog'][d]['Text_time'])
					}
					imgui.StrCopy(tableEnd[tt]['Target_Data']['Dialog'][d]['Text'],table[tt]['Target_Data']['Dialog'][d]['Text'])
				end
			end
		end
	end
	if typee == 1 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = new.int(table[tt]['Type']),
				['Actor_Data'] = {
					['Pos'] = new.float[3](table[tt]['Actor_Data']['Pos'][1],table[tt]['Actor_Data']['Pos'][2],table[tt]['Actor_Data']['Pos'][3]),
					['Angle'] = new.float(table[tt]['Actor_Data']['Angle']),
					['ModelId'] = new.int(table[tt]['Actor_Data']['ModelId']),
					['StartC'] = new.int(table[tt]['Actor_Data']['StartC']),
					['EndC'] = new.int(table[tt]['Actor_Data']['EndC']),
					['Anims'] = {},
					['Anim_id'] = new.int(0),
					['Should_not_die'] = new.bool(table[tt]['Actor_Data']['Should_not_die'] or false),
					['Health'] = new.int(table[tt]['Actor_Data']['Health'] or 100),
				}
			}
			for t = 1, #table[tt]['Actor_Data']['Anims'] do
				tableEnd[tt]['Actor_Data']['Anims'][t] = {
				['Type'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Type'] or 0)
				}

				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'][0] == 0 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Anim'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Anim'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Pack_anim'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Pack_anim'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Loop'] = new.bool(table[tt]['Actor_Data']['Anims'][t]['Loop'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Time'] = new.float(table[tt]['Actor_Data']['Anims'][t]['Time'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Condition'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Unbreakable'] = new.bool(ternar(table[tt]['Actor_Data']['Anims'][t]['Unbreakable'] ~= nil,table[tt]['Actor_Data']['Anims'][t]['Unbreakable'],false))
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'][0] == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'][0] == 1 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Path'] = {}
					for i,_ in pairs(table[tt]['Actor_Data']['Anims'][t]['Path']) do
						tableEnd[tt]['Actor_Data']['Anims'][t]['Path'][tonumber(i)] = table[tt]['Actor_Data']['Anims'][t]['Path'][i]
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Type_move'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Type_move'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Type_route'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Type_route'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Cur_point'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Cur_point'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vis_point'] = new.bool(false)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'][0] == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'][0] == 2 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Path'] = {}
					for i,_ in pairs(table[tt]['Actor_Data']['Anims'][t]['Path']) do
						tableEnd[tt]['Actor_Data']['Anims'][t]['Path'][tonumber(i)] = table[tt]['Actor_Data']['Anims'][t]['Path'][i]
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Car'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Speed'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Speed'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Cur_point'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Cur_point'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vis_point'] = new.bool(false)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'][0] == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'][0] == 3 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'][0] == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'][0] == 4 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'][0] == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car_a'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Car_a'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car_t'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Car_t'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Speed'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Speed'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['trafficFlag'] = new.int(table[tt]['Actor_Data']['Anims'][t]['trafficFlag'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vehicle_mission'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Vehicle_mission'])

				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'][0] == 5 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'][0] == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['place_in_car'] = new.int(table[tt]['Actor_Data']['Anims'][t]['place_in_car'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car'] = new.int(table[tt]['Actor_Data']['Anims'][t]['Car'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['speed_walk'] = new.int(table[tt]['Actor_Data']['Anims'][t]['speed_walk'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['wait_end'] = new.bool(table[tt]['Actor_Data']['Anims'][t]['wait_end'])
					
				end
			end
		end
	end
	if typee == 2 then
		for tt = 1,#table do
			tableEnd[tt] = {
			['Name'] = table[tt]['Name'],
			['Type'] = new.int(table[tt]['Type']),
			['Car_Data'] = {
				['Pos'] = new.float[3](table[tt]['Car_Data']['Pos'][1],table[tt]['Car_Data']['Pos'][2],table[tt]['Car_Data']['Pos'][3]),
				['Angle'] = new.float(table[tt]['Car_Data']['Angle']),
				['ModelId'] = new.int(table[tt]['Car_Data']['ModelId']),
				['StartC'] = new.int(table[tt]['Car_Data']['StartC']),
				['EndC'] = new.int(table[tt]['Car_Data']['EndC']),
				['Color_primary'] = new.int(table[tt]['Car_Data']['Color_primary'] or 0),
				['Color_secondary'] = new.int(table[tt]['Car_Data']['Color_secondary'] or 0),
				['Should_not_die'] = new.bool(table[tt]['Car_Data']['Should_not_die'] or false),
				['Health'] = new.int(table[tt]['Car_Data']['Health'] or 1000),
				['Bulletproof'] = new.bool(table[tt]['Car_Data']['Bulletproof'] or false),
				['Fireproof'] = new.bool(table[tt]['Car_Data']['Fireproof'] or false),
				['Explosionproof'] = new.bool(table[tt]['Car_Data']['Explosionproof'] or false),
				['Collisionproof'] = new.bool(table[tt]['Car_Data']['Collisionproof'] or false),
				['Meleeproof'] = new.bool(table[tt]['Car_Data']['Meleeproof'] or false),
				['Tires_vulnerability'] = new.bool(table[tt]['Car_Data']['Tires_vulnerability'] or false),
				['Anims'] = {},
				['Anim_id'] = new.int(0)
				}
			}
			if table[tt]['Car_Data']['Anims'] then
				for t = 1, #table[tt]['Car_Data']['Anims'] do
					tableEnd[tt]['Car_Data']['Anims'][t] = {
					['Type'] = new.int(table[tt]['Car_Data']['Anims'][t]['Type'] or 0)
					}
					if tableEnd[tt]['Car_Data']['Anims'][t]['Type'][0] == 0 then
						tableEnd[tt]['Car_Data']['Anims'][t]['Doors'] = {new.bool(table[tt]['Car_Data']['Anims'][t]['Doors'][1]),new.bool(table[tt]['Car_Data']['Anims'][t]['Doors'][2]),new.bool(table[tt]['Car_Data']['Anims'][t]['Doors'][3]),new.bool(table[tt]['Car_Data']['Anims'][t]['Doors'][4]),new.bool(table[tt]['Car_Data']['Anims'][t]['Doors'][5]),new.bool(table[tt]['Car_Data']['Anims'][t]['Doors'][6])}
						tableEnd[tt]['Car_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Car_Data']['Anims'][t]['Condition'])
						
						if tableEnd[tt]['Car_Data']['Anims'][t]['Condition'][0] == 1 then
							tableEnd[tt]['Car_Data']['Anims'][t]['Target'] = new.int(table[tt]['Car_Data']['Anims'][t]['Target'])
						end
					end
					if tableEnd[tt]['Car_Data']['Anims'][t]['Type'][0] == 1 then
						tableEnd[tt]['Car_Data']['Anims'][t]['Door_lock'] = new.int(table[tt]['Car_Data']['Anims'][t]['Door_lock'])
						tableEnd[tt]['Car_Data']['Anims'][t]['Condition'] = new.int(table[tt]['Car_Data']['Anims'][t]['Condition'])
						
						if tableEnd[tt]['Car_Data']['Anims'][t]['Condition'][0] == 1 then
							tableEnd[tt]['Car_Data']['Anims'][t]['Target'] = new.int(table[tt]['Car_Data']['Anims'][t]['Target'])
						end
					end
				end
			end
		end
	end
	if typee == 3 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = new.int(table[tt]['Type']),
				['Object_Data'] = {
					['Pos'] = new.float[3](table[tt]['Object_Data']['Pos'][1],table[tt]['Object_Data']['Pos'][2],table[tt]['Object_Data']['Pos'][3]),
					['Rotates'] = new.float[3](table[tt]['Object_Data']['Rotates'][1],table[tt]['Object_Data']['Rotates'][2],table[tt]['Object_Data']['Rotates'][3]),
					['ModelId'] = new.int(table[tt]['Object_Data']['ModelId']),
					['StartC'] = new.int(table[tt]['Object_Data']['StartC']),
					['EndC'] = new.int(table[tt]['Object_Data']['EndC']),
					['Anims'] = {},
					['Anim_id'] = new.int(0)
				}
			}
			for t = 1, #(table[tt]['Object_Data']['Anims'] or {}) do
				tableEnd[tt]['Object_Data']['Anims'][t] = {
				['Time'] = new.float(table[tt]['Object_Data']['Anims'][t]['Time']),
				['Condition'] = new.int(table[tt]['Object_Data']['Anims'][t]['Condition']),
				['Pos'] = new.float[3](table[tt]['Object_Data']['Anims'][t]['Pos'][1],table[tt]['Object_Data']['Anims'][t]['Pos'][2],table[tt]['Object_Data']['Anims'][t]['Pos'][3]),
				['Rotates'] = new.float[3](table[tt]['Object_Data']['Anims'][t]['Rotates'][1],table[tt]['Object_Data']['Anims'][t]['Rotates'][2],table[tt]['Object_Data']['Anims'][t]['Rotates'][3]),
				}
				if tableEnd[tt]['Object_Data']['Anims'][t]['Condition'][0] == 1 then
					tableEnd[tt]['Object_Data']['Anims'][t]['Target'] = new.int(table[tt]['Object_Data']['Anims'][t]['Target'])
				end
			end
		end
	end
	if typee == 4 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = new.int(table[tt]['Type']),
				['Pickup_Data'] = {
					['Type_pickup'] = new.int(table[tt]['Pickup_Data']['Type_pickup']),
					['Pos'] = new.float[3](table[tt]['Pickup_Data']['Pos'][1],table[tt]['Pickup_Data']['Pos'][2],table[tt]['Pickup_Data']['Pos'][3]),
					['StartC'] = new.int(table[tt]['Pickup_Data']['StartC']),
					['EndC'] = new.int(table[tt]['Pickup_Data']['EndC']),
					['spawn_type'] = new.int(table[tt]['Pickup_Data']['spawn_type'])
				}
			}
			if table[tt]['Pickup_Data']['Type_pickup'] == 0 then
				tableEnd[tt]['Pickup_Data']['Ammo'] = new.int(table[tt]['Pickup_Data']['Ammo'])
				tableEnd[tt]['Pickup_Data']['Weapon'] = new.int(table[tt]['Pickup_Data']['Weapon'])
			end
			if table[tt]['Pickup_Data']['Type_pickup'] == 5 then
				tableEnd[tt]['Pickup_Data']['ModelId'] = new.int(table[tt]['Pickup_Data']['ModelId'])
			end
		end
	end
	if typee == 5 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = new.int(table[tt]['Type']),
				['Particle_Data'] = {
					['Pos'] = new.float[3](table[tt]['Particle_Data']['Pos'][1],table[tt]['Particle_Data']['Pos'][2],table[tt]['Particle_Data']['Pos'][3]),
					['Rotates'] = new.float[3](table[tt]['Particle_Data']['Rotates'][1],table[tt]['Particle_Data']['Rotates'][2],table[tt]['Particle_Data']['Rotates'][3]),
					['Rotates'] = new.float[3](table[tt]['Particle_Data']['Rotates'][1],table[tt]['Particle_Data']['Rotates'][2],table[tt]['Particle_Data']['Rotates'][3]),
					['ModelId'] = new.int(table[tt]['Particle_Data']['ModelId']),
					['StartC'] = new.int(table[tt]['Particle_Data']['StartC']),
					['EndC'] = new.int(table[tt]['Particle_Data']['EndC'])
				}
			}
		end
	end
	if typee == 6 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = new.int(table[tt]['Type']),
				['Explosion_Data'] = {
					['Pos'] = new.float[3](table[tt]['Explosion_Data']['Pos'][1],table[tt]['Explosion_Data']['Pos'][2],table[tt]['Explosion_Data']['Pos'][3]),
					['Type'] = new.int(table[tt]['Explosion_Data']['Type']),
					['StartC'] = new.int(table[tt]['Explosion_Data']['StartC']),
					['EndC'] = new.int(table[tt]['Explosion_Data']['EndC'])
				}
			}
			if tableEnd[tt]['Explosion_Data']['Type'][0] == 0 then
				tableEnd[tt]['Explosion_Data']['Size_fire'] = new.int(table[tt]['Explosion_Data']['Size_fire'])
				tableEnd[tt]['Explosion_Data']['Propagation_fire'] = new.int(table[tt]['Explosion_Data']['Propagation_fire'])
			end
			if tableEnd[tt]['Explosion_Data']['Type'][0] == 1 then
				tableEnd[tt]['Explosion_Data']['Type_explosion'] = new.int(table[tt]['Explosion_Data']['Type_explosion'])
			end
		end
	end
	return tableEnd
end

function manager.loadMPack()
	local c = 0
	mpack = {}
	createDirectory(path)
	while doesFileExist(path .. 'LDYOM' .. tostring(c) .. '.json') do
		wait(0)
		mpack[c+1] = manager.load(c)
		c = c + 1
	end
	return mpack
end


function ternar(condition,truee,falses)
	if condition then
		return truee
	else
		return falses
	end
end

return manager
