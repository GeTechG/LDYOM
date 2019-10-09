manager = {}

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
				['Targets'] = pack['Missions'][p]['Mission_Data']['Targets'],
				['Actors'] = pack['Missions'][p]['Mission_Data']['Actors'],
				['Cars'] = pack['Missions'][p]['Mission_Data']['Cars'],
				['Objects'] = pack['Missions'][p]['Mission_Data']['Objects'],
				['Pickup'] = pack['Missions'][p]['Mission_Data']['Pickup'],
				['Particle'] = pack['Missions'][p]['Mission_Data']['Particle'],
				['Explosion'] = pack['Missions'][p]['Mission_Data']['Explosion'],
				['Miss_data'] = {
					['Name'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Name'],
					['Time'] = {pack['Missions'][p]['Mission_Data']['Miss_data']['Time'][1],pack['Missions'][p]['Mission_Data']['Miss_data']['Time'][2]},
					['Weather'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Weather'],
					['Riot'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Riot'],
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
	packk = manager.sorterTable(pack)
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
	local packk = json.decode(f:read())
	f:close()
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
	elseif orig_type == 'cdata' then
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
				['Type'] = table[tt]['Type'],
				['Target_Data'] = {}
			}
			if 	tableEnd[tt]['Type'] == 1 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = table[tt]['Target_Data']['Pos'],
					['Radius'] = table[tt]['Target_Data']['Radius'],
					['Text'] = table[tt]['Target_Data']['Text'],
					['Text_time'] = table[tt]['Target_Data']['Text_time'],
					['Color_blip'] = table[tt]['Target_Data']['Color_blip'],
				}
			end
			if 	tableEnd[tt]['Type'] == 2 then
				tableEnd[tt]['Target_Data'] = {
				['Target_car_id'] = table[tt]['Target_Data']['Target_car_id'],
				['Color_blip'] = table[tt]['Target_Data']['Color_blip'],
				['Text'] = ternar(table[tt]['Target_Data']['Text'] ~= nil, table[tt]['Target_Data']['Text'],'')
			}
			end
			if tableEnd[tt]['Type'] == 3 then
				tableEnd[tt]['Target_Data'] = {
				['Target_actor_id'] = table[tt]['Target_Data']['Target_actor_id'],
				['Color_blip'] = table[tt]['Target_Data']['Color_blip'],
				['Text'] = table[tt]['Target_Data']['Text'] or ''
			}
			end
			if tableEnd[tt]['Type'] == 4 then
				tableEnd[tt]['Target_Data'] = {
					['Target_type'] = table[tt]['Target_Data']['Target_type'],
					['Pos'] = table[tt]['Target_Data']['Pos'],
					['Rotates'] = table[tt]['Target_Data']['Rotates'],
					['Text'] = table[tt]['Target_Data']['Text'],
					['Text_time'] = table[tt]['Target_Data']['Text_time'],
					['Smooth'] = table[tt]['Target_Data']['Smooth'],
					['Time'] = table[tt]['Target_Data']['Time'],
					['Weather'] = table[tt]['Target_Data']['Weather'],
					['Clock_time'] = table[tt]['Target_Data']['Clock_time'],
					['Traffic'] = table[tt]['Target_Data']['Traffic']

				}
			end
			if tableEnd[tt]['Type'] == 5 then
				tableEnd[tt]['Target_Data'] = {
					['Target_object_id'] = table[tt]['Target_Data']['Target_type'],
					['Color_blip'] = table[tt]['Target_Data']['Color_blip'],
					['Target_type'] = table[tt]['Target_Data']['Target_type'],
					['Weap'] = table[tt]['Target_Data']['Weap'],
					['Text'] = table[tt]['Target_Data']['Text']
				}
			end
			if tableEnd[tt]['Type'] == 6 then
				tableEnd[tt]['Target_Data'] = {
				['Target_pickup_id'] = table[tt]['Target_Data']['Target_pickup_id'],
				['Color_blip'] = table[tt]['Target_Data']['Color_blip'],
				['Text'] = table[tt]['Target_Data']['Text']
			}
		end
			if tableEnd[tt]['Type'] == 7 then
				tableEnd[tt]['Target_Data'] = {
					['Target_type'] = table[tt]['Target_Data']['Target_type'],
					['Pos'] = table[tt]['Target_Data']['Pos'],
					['ModelID'] = table[tt]['Target_Data']['ModelID'],
					['Angle'] = table[tt]['Target_Data']['Angle'],
					['Weapon'] = table[tt]['Target_Data']['Weapon'],
					['Weap_ammo'] = table[tt]['Target_Data']['Weap_ammo'],
					['Anim'] = table[tt]['Target_Data']['Anim'],
					['Pack_anim'] = table[tt]['Target_Data']['Pack_anim'],
					['Loop'] = table[tt]['Target_Data']['Loop'],
					['Car_id'] = table[tt]['Target_Data']['Car_id'],
					['Car_place'] = table[tt]['Target_Data']['Car_place'],
					['Level_battue'] = table[tt]['Target_Data']['Level_battue'],
					['Dialog'] = {},
					['Dialog_id'] = 0,
					['Add_money'] = table[tt]['Target_Data']['Add_money'],
					['Interior_id'] = table[tt]['Target_Data']['Interior_id']
				}
				for d = 1,#table[tt]['Target_Data']['Dialog'] do
					tableEnd[tt]['Target_Data']['Dialog'][d] = {
						['Text'] = table[tt]['Target_Data']['Dialog'][d]['Text'],
						['Text_time'] = table[tt]['Target_Data']['Dialog'][d]['Text_time']
					}
				end
			end
		end
	end
	if typee == 1 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = table[tt]['Type'],
				['Actor_Data'] = {
					['Pos'] = table[tt]['Actor_Data']['Pos'],
					['Angle'] = table[tt]['Actor_Data']['Angle'],
					['ModelId'] = table[tt]['Actor_Data']['ModelId'],
					['StartC'] = table[tt]['Actor_Data']['StartC'],
					['EndC'] = table[tt]['Actor_Data']['EndC'],
					['Anims'] = {},
					['Anim_id'] = 0,
					['Should_not_die'] = table[tt]['Actor_Data']['Should_not_die'] or false,
					['Health'] = table[tt]['Actor_Data']['Health'] or 100,
				}
			}
			for t = 1, #table[tt]['Actor_Data']['Anims'] do
				tableEnd[tt]['Actor_Data']['Anims'][t] = {
				['Type'] = table[tt]['Actor_Data']['Anims'][t]['Type'] or 0
				}

				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'] == 1 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Anim'] = table[tt]['Actor_Data']['Anims'][t]['Anim']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Pack_anim'] = table[tt]['Actor_Data']['Anims'][t]['Pack_anim']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Loop'] = table[tt]['Actor_Data']['Anims'][t]['Loop']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Time'] = table[tt]['Actor_Data']['Anims'][t]['Time']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Unbreakable'] = ternar(table[tt]['Actor_Data']['Anims'][t]['Unbreakable'] ~= nil,table[tt]['Actor_Data']['Anims'][t]['Unbreakable'],false)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 2 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target']
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'] == 2 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Path'] = {}
					for i,_ in pairs(table[tt]['Actor_Data']['Anims'][t]['Path']) do
						tableEnd[tt]['Actor_Data']['Anims'][t]['Path'][tonumber(i)] = table[tt]['Actor_Data']['Anims'][t]['Path'][i]
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Type_move'] = table[tt]['Actor_Data']['Anims'][t]['Type_move']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Type_route'] = table[tt]['Actor_Data']['Anims'][t]['Type_route']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Cur_point'] = table[tt]['Actor_Data']['Anims'][t]['Cur_point']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vis_point'] = false
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 2 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target']
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'] == 3 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Path'] = {}
					for i,_ in pairs(table[tt]['Actor_Data']['Anims'][t]['Path']) do
						tableEnd[tt]['Actor_Data']['Anims'][t]['Path'][tonumber(i)] = table[tt]['Actor_Data']['Anims'][t]['Path'][i]
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car'] = table[tt]['Actor_Data']['Anims'][t]['Car']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Speed'] = table[tt]['Actor_Data']['Anims'][t]['Speed']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Cur_point'] = table[tt]['Actor_Data']['Anims'][t]['Cur_point']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vis_point'] = false
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 2 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target']
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'] == 4 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 2 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target']
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'] == 5 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 2 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target']
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car_a'] = table[tt]['Actor_Data']['Anims'][t]['Car_a']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car_t'] = table[tt]['Actor_Data']['Anims'][t]['Car_t']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Speed'] = table[tt]['Actor_Data']['Anims'][t]['Speed']
					tableEnd[tt]['Actor_Data']['Anims'][t]['trafficFlag'] = table[tt]['Actor_Data']['Anims'][t]['trafficFlag']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vehicle_mission'] = table[tt]['Actor_Data']['Anims'][t]['Vehicle_mission']

				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'] == 6 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 2 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target']
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['place_in_car'] = table[tt]['Actor_Data']['Anims'][t]['place_in_car']
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car'] = table[tt]['Actor_Data']['Anims'][t]['Car']
					tableEnd[tt]['Actor_Data']['Anims'][t]['speed_walk'] = table[tt]['Actor_Data']['Anims'][t]['speed_walk']
					tableEnd[tt]['Actor_Data']['Anims'][t]['wait_end'] = table[tt]['Actor_Data']['Anims'][t]['wait_end']
					
				end
			end
		end
	end
	if typee == 2 then
		for tt = 1,#table do
			tableEnd[tt] = {
			['Name'] = table[tt]['Name'],
			['Type'] = table[tt]['Type'],
			['Car_Data'] = {
				['Pos'] = table[tt]['Car_Data']['Pos'],
				['Angle'] = table[tt]['Car_Data']['Angle'],
				['ModelId'] = table[tt]['Car_Data']['ModelId'],
				['StartC'] = table[tt]['Car_Data']['StartC'],
				['EndC'] = table[tt]['Car_Data']['EndC'],
				['Color_primary'] = table[tt]['Car_Data']['Color_primary'] or 0,
				['Color_secondary'] = table[tt]['Car_Data']['Color_secondary'] or 0,
				['Should_not_die'] = table[tt]['Car_Data']['Should_not_die'] or false,
				['Health'] = table[tt]['Car_Data']['Health'] or 1000,
				['Bulletproof'] = table[tt]['Car_Data']['Bulletproof'] or false,
				['Fireproof'] = table[tt]['Car_Data']['Fireproof'] or false,
				['Explosionproof'] = table[tt]['Car_Data']['Explosionproof'] or false,
				['Collisionproof'] = table[tt]['Car_Data']['Collisionproof'] or false,
				['Meleeproof'] = table[tt]['Car_Data']['Meleeproof'] or false,
				['Tires_vulnerability'] = table[tt]['Car_Data']['Tires_vulnerability'] or false,
				['Anims'] = {},
				['Anim_id'] = 0
				}
			}
			if table[tt]['Car_Data']['Anims'] then
				for t = 1, #table[tt]['Car_Data']['Anims'] do
					tableEnd[tt]['Car_Data']['Anims'][t] = {
					['Type'] = table[tt]['Car_Data']['Anims'][t]['Type'] or 0
					}
					if tableEnd[tt]['Car_Data']['Anims'][t]['Type'] == 1 then
						tableEnd[tt]['Car_Data']['Anims'][t]['Doors'] = table[tt]['Car_Data']['Anims'][t]['Doors']
						tableEnd[tt]['Car_Data']['Anims'][t]['Condition'] = table[tt]['Car_Data']['Anims'][t]['Condition']
						
						if tableEnd[tt]['Car_Data']['Anims'][t]['Condition'] == 2 then
							tableEnd[tt]['Car_Data']['Anims'][t]['Target'] = table[tt]['Car_Data']['Anims'][t]['Target']
						end
					end
					if tableEnd[tt]['Car_Data']['Anims'][t]['Type'] == 2 then
						tableEnd[tt]['Car_Data']['Anims'][t]['Door_lock'] = table[tt]['Car_Data']['Anims'][t]['Door_lock']
						tableEnd[tt]['Car_Data']['Anims'][t]['Condition'] = table[tt]['Car_Data']['Anims'][t]['Condition']
						
						if tableEnd[tt]['Car_Data']['Anims'][t]['Condition'] == 2 then
							tableEnd[tt]['Car_Data']['Anims'][t]['Target'] = table[tt]['Car_Data']['Anims'][t]['Target']
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
				['Type'] = table[tt]['Type'],
				['Object_Data'] = {
					['Pos'] = table[tt]['Object_Data']['Pos'],
					['Rotates'] = table[tt]['Object_Data']['Rotates'],
					['ModelId'] = table[tt]['Object_Data']['ModelId'],
					['StartC'] = table[tt]['Object_Data']['StartC'],
					['EndC'] = table[tt]['Object_Data']['EndC'],
					['Anims'] = {},
					['Anim_id'] = 0
				}
			}
			for t = 1, #(table[tt]['Object_Data']['Anims'] or {}) do
				tableEnd[tt]['Object_Data']['Anims'][t] = {
				['Time'] = table[tt]['Object_Data']['Anims'][t]['Time'],
				['Condition'] = table[tt]['Object_Data']['Anims'][t]['Condition'],
				['Pos'] = table[tt]['Object_Data']['Anims'][t]['Pos'],
				['Rotates'] = table[tt]['Object_Data']['Anims'][t]['Rotates'],
				}
				if tableEnd[tt]['Object_Data']['Anims'][t]['Condition'] == 2 then
					tableEnd[tt]['Object_Data']['Anims'][t]['Target'] = table[tt]['Object_Data']['Anims'][t]['Target']
				end
			end
		end
	end
	if typee == 4 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = table[tt]['Type'],
				['Pickup_Data'] = {
					['Type_pickup'] = table[tt]['Pickup_Data']['Type_pickup'],
					['Pos'] = table[tt]['Pickup_Data']['Pos'],
					['StartC'] = table[tt]['Pickup_Data']['StartC'],
					['EndC'] = table[tt]['Pickup_Data']['EndC'],
					['spawn_type'] = table[tt]['Pickup_Data']['spawn_type']
				}
			}
			if table[tt]['Pickup_Data']['Type_pickup'] == 1 then
				tableEnd[tt]['Pickup_Data']['Ammo'] = table[tt]['Pickup_Data']['Ammo']
				tableEnd[tt]['Pickup_Data']['Weapon'] = table[tt]['Pickup_Data']['Weapon']
			end
			if table[tt]['Pickup_Data']['Type_pickup'] == 6 then
				tableEnd[tt]['Pickup_Data']['ModelId'] = table[tt]['Pickup_Data']['ModelId']
			end
		end
	end
	if typee == 5 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = table[tt]['Type'],
				['Particle_Data'] = {
					['Pos'] = table[tt]['Particle_Data']['Pos'],
					['Rotates'] = table[tt]['Particle_Data']['Rotates'],
					['ModelId'] = table[tt]['Particle_Data']['ModelId'],
					['StartC'] = table[tt]['Particle_Data']['StartC'],
					['EndC'] = table[tt]['Particle_Data']['EndC']
				}
			}
		end
	end
	if typee == 6 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = table[tt]['Type'],
				['Explosion_Data'] = {
					['Pos'] = table[tt]['Explosion_Data']['Pos'],
					['Type'] = table[tt]['Explosion_Data']['Type'],
					['StartC'] = table[tt]['Explosion_Data']['StartC'],
					['EndC'] = table[tt]['Explosion_Data']['EndC']
				}
			}
			if tableEnd[tt]['Explosion_Data']['Type'] == 1 then
				tableEnd[tt]['Explosion_Data']['Size_fire'] = table[tt]['Explosion_Data']['Size_fire']
				tableEnd[tt]['Explosion_Data']['Propagation_fire'] = table[tt]['Explosion_Data']['Propagation_fire']
			end
			if tableEnd[tt]['Explosion_Data']['Type'] == 2 then
				tableEnd[tt]['Explosion_Data']['Type_explosion'] = table[tt]['Explosion_Data']['Type_explosion']
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
