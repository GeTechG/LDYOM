manager = {}

encoding = require 'encoding'
--debtab = require 'debug_table'
encoding.default = 'CP1251'
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
					['Weather'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Weather'].v,
					['Riot'] = pack['Missions'][p]['Mission_Data']['Miss_data']['Riot'].v,
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
	f:write(encodeJson(packk))
	f:close()
end

function manager.load(num)
	f = io.open(path .. 'LDYOM' .. tostring(num) .. '.json',"r")
	local pack = decodeJson(f:read())
	f:close()
	packk = {
		['Name'] = pack['Name'],
		['Missions'] = {}
	}
	for p = 1, #pack['Missions'] do
		--print(debtab.tostring(pack['Missions'][1]))
		packk['Missions'][p] = {
			['Name'] = pack['Missions'][p]['Name'],
			['Prename'] = imgui.ImBuffer(256),
			['Enable'] = imgui.ImBool(false),
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
					['Weather'] = imgui.ImInt(pack['Missions'][p]['Mission_Data']['Miss_data']['Weather']),
					['Riot'] = imgui.ImBool(pack['Missions'][p]['Mission_Data']['Miss_data']['Riot']),
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
		if type(orig.v) == 'userdata' then
			copy = {orig.v[1],orig.v[2],orig.v[3]}
		else
			copy = orig.v
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
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Target_Data'] = {}
			}
			if 	tableEnd[tt]['Type'].v == 0 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][2],table[tt]['Target_Data']['Pos'][3]),
					['Radius'] = imgui.ImInt(table[tt]['Target_Data']['Radius']),
					['Text'] = imgui.ImBuffer(128),
					['Text_time'] = imgui.ImFloat(table[tt]['Target_Data']['Text_time']),
					['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip']),
				}
				tableEnd[tt]['Target_Data']['Text'].v = table[tt]['Target_Data']['Text']
			end
			if 	tableEnd[tt]['Type'].v == 1 then
				tableEnd[tt]['Target_Data'] = {
				['Target_car_id'] = imgui.ImInt(table[tt]['Target_Data']['Target_car_id']),
				['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip']),
				['Text'] = imgui.ImBuffer(128)
			}
			tableEnd[tt]['Target_Data']['Text'].v = ternar(table[tt]['Target_Data']['Text'] ~= nil, table[tt]['Target_Data']['Text'],'')
			end
			if tableEnd[tt]['Type'].v == 2 then
				tableEnd[tt]['Target_Data'] = {
				['Target_actor_id'] = imgui.ImInt(table[tt]['Target_Data']['Target_actor_id']),
				['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip']),
				['Text'] = imgui.ImBuffer(128)
			}
			tableEnd[tt]['Target_Data']['Text'].v = table[tt]['Target_Data']['Text'] or ''
			end
			if tableEnd[tt]['Type'].v == 3 then
				tableEnd[tt]['Target_Data'] = {
					['Target_type'] = imgui.ImInt(table[tt]['Target_Data']['Target_type']),
					['Pos'] = imgui.ImFloat3(table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][2],table[tt]['Target_Data']['Pos'][3]),
					['Rotates'] = imgui.ImFloat3(table[tt]['Target_Data']['Rotates'][1],table[tt]['Target_Data']['Rotates'][2],table[tt]['Target_Data']['Rotates'][3]),
					['Text'] = imgui.ImBuffer(128),
					['Text_time'] = imgui.ImFloat(table[tt]['Target_Data']['Text_time']),
					['Smooth'] = imgui.ImBool(table[tt]['Target_Data']['Smooth']),
					['Time'] = imgui.ImInt(table[tt]['Target_Data']['Time']),
					['Weather'] = imgui.ImInt(table[tt]['Target_Data']['Weather']),
					['Clock_time'] = table[tt]['Target_Data']['Clock_time'],
					['Traffic'] = {imgui.ImInt(table[tt]['Target_Data']['Traffic'][1]),imgui.ImInt(table[tt]['Target_Data']['Traffic'][2])}

				}
				tableEnd[tt]['Target_Data']['Text'].v = table[tt]['Target_Data']['Text']
			end
			if tableEnd[tt]['Type'].v == 4 then
				tableEnd[tt]['Target_Data'] = {
					['Target_object_id'] = imgui.ImInt(table[tt]['Target_Data']['Target_type']),
					['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip']),
					['Target_type'] = imgui.ImInt(table[tt]['Target_Data']['Target_type']),
					['Weap'] = imgui.ImInt(table[tt]['Target_Data']['Weap']),
					['Text'] = imgui.ImBuffer(128)
				}
				tableEnd[tt]['Target_Data']['Text'].v = table[tt]['Target_Data']['Text']
			end
			if tableEnd[tt]['Type'].v == 5 then
				tableEnd[tt]['Target_Data'] = {
				['Target_pickup_id'] = imgui.ImInt(table[tt]['Target_Data']['Target_pickup_id']),
				['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip']),
				['Text'] = imgui.ImBuffer(128)
			}
			tableEnd[tt]['Target_Data']['Text'].v = table[tt]['Target_Data']['Text']
		end
			if tableEnd[tt]['Type'].v == 6 then
				tableEnd[tt]['Target_Data'] = {
					['Target_type'] = imgui.ImInt(table[tt]['Target_Data']['Target_type']),
					['Pos'] = imgui.ImFloat3(table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][2],table[tt]['Target_Data']['Pos'][3]),
					['ModelID'] = imgui.ImInt(table[tt]['Target_Data']['ModelID']),
					['Angle'] = imgui.ImFloat(table[tt]['Target_Data']['Angle']),
					['Weapon'] = imgui.ImInt(table[tt]['Target_Data']['Weapon']),
					['Weap_ammo'] = imgui.ImInt(table[tt]['Target_Data']['Weap_ammo']),
					['Anim'] = imgui.ImInt(table[tt]['Target_Data']['Anim']),
					['Pack_anim'] = imgui.ImInt(table[tt]['Target_Data']['Pack_anim']),
					['Loop'] = imgui.ImBool(table[tt]['Target_Data']['Loop']),
					['Car_id'] = imgui.ImInt(table[tt]['Target_Data']['Car_id']),
					['Car_place'] = imgui.ImInt(table[tt]['Target_Data']['Car_place']),
					['Level_battue'] = imgui.ImInt(table[tt]['Target_Data']['Level_battue']),
					['Dialog'] = {},
					['Dialog_id'] = imgui.ImInt(0),
					['Add_money'] = imgui.ImInt(table[tt]['Target_Data']['Add_money']),
					['Interior_id'] = table[tt]['Target_Data']['Interior_id']
				}
				for d = 1,#table[tt]['Target_Data']['Dialog'] do
					tableEnd[tt]['Target_Data']['Dialog'][d] = {
						['Text'] = imgui.ImBuffer(128),
						['Text_time'] = imgui.ImFloat(table[tt]['Target_Data']['Dialog'][d]['Text_time'])
					}
					tableEnd[tt]['Target_Data']['Dialog'][d]['Text'].v = table[tt]['Target_Data']['Dialog'][d]['Text']
				end
			end
		end
	end
	if typee == 1 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Actor_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Actor_Data']['Pos'][1],table[tt]['Actor_Data']['Pos'][2],table[tt]['Actor_Data']['Pos'][3]),
					['Angle'] = imgui.ImFloat(table[tt]['Actor_Data']['Angle']),
					['ModelId'] = imgui.ImInt(table[tt]['Actor_Data']['ModelId']),
					['StartC'] = imgui.ImInt(table[tt]['Actor_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Actor_Data']['EndC']),
					['Anims'] = {},
					['Anim_id'] = imgui.ImInt(0),
					['Should_not_die'] = imgui.ImBool(table[tt]['Actor_Data']['Should_not_die'] or false),
					['Health'] = imgui.ImInt(table[tt]['Actor_Data']['Health'] or 100),
				}
			}
			for t = 1, #table[tt]['Actor_Data']['Anims'] do
				tableEnd[tt]['Actor_Data']['Anims'][t] = {
				['Type'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Type'] or 0)
				}

				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'].v == 0 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Anim'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Anim'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Pack_anim'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Pack_anim'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Loop'] = imgui.ImBool(table[tt]['Actor_Data']['Anims'][t]['Loop'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Time'] = imgui.ImFloat(table[tt]['Actor_Data']['Anims'][t]['Time'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Condition'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Unbreakable'] = imgui.ImBool(ternar(table[tt]['Actor_Data']['Anims'][t]['Unbreakable'] ~= nil,table[tt]['Actor_Data']['Anims'][t]['Unbreakable'],false))
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'].v == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'].v == 1 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Path'] = {}
					for i,_ in pairs(table[tt]['Actor_Data']['Anims'][t]['Path']) do
						tableEnd[tt]['Actor_Data']['Anims'][t]['Path'][tonumber(i)] = table[tt]['Actor_Data']['Anims'][t]['Path'][i]
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Type_move'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Type_move'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Type_route'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Type_route'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Cur_point'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Cur_point'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vis_point'] = imgui.ImBool(false)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'].v == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'].v == 2 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Path'] = {}
					for i,_ in pairs(table[tt]['Actor_Data']['Anims'][t]['Path']) do
						tableEnd[tt]['Actor_Data']['Anims'][t]['Path'][tonumber(i)] = table[tt]['Actor_Data']['Anims'][t]['Path'][i]
					end
					tableEnd[tt]['Actor_Data']['Anims'][t]['Car'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Car'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Speed'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Speed'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Cur_point'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Cur_point'])
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					tableEnd[tt]['Actor_Data']['Anims'][t]['Vis_point'] = imgui.ImBool(false)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'].v == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Type'].v == 3 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Condition'] or 0)
					
					if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'].v == 1 then
						tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Target'])
					end
				end
			end
		end
	end
	if typee == 2 then
		for tt = 1,#table do
			tableEnd[tt] = {
			['Name'] = table[tt]['Name'],
			['Type'] = imgui.ImInt(table[tt]['Type']),
			['Car_Data'] = {
				['Pos'] = imgui.ImFloat3(table[tt]['Car_Data']['Pos'][1],table[tt]['Car_Data']['Pos'][2],table[tt]['Car_Data']['Pos'][3]),
				['Angle'] = imgui.ImFloat(table[tt]['Car_Data']['Angle']),
				['ModelId'] = imgui.ImInt(table[tt]['Car_Data']['ModelId']),
				['StartC'] = imgui.ImInt(table[tt]['Car_Data']['StartC']),
				['EndC'] = imgui.ImInt(table[tt]['Car_Data']['EndC']),
				['Color_primary'] = imgui.ImInt(table[tt]['Car_Data']['Color_primary'] or 0),
				['Color_secondary'] = imgui.ImInt(table[tt]['Car_Data']['Color_secondary'] or 0),
				['Should_not_die'] = imgui.ImBool(table[tt]['Car_Data']['Should_not_die'] or false),
				['Health'] = imgui.ImInt(table[tt]['Car_Data']['Health'] or 1000),
				['Bulletproof'] = imgui.ImBool(table[tt]['Car_Data']['Bulletproof'] or false),
				['Fireproof'] = imgui.ImBool(table[tt]['Car_Data']['Fireproof'] or false),
				['Explosionproof'] = imgui.ImBool(table[tt]['Car_Data']['Explosionproof'] or false),
				['Collisionproof'] = imgui.ImBool(table[tt]['Car_Data']['Collisionproof'] or false),
				['Meleeproof'] = imgui.ImBool(table[tt]['Car_Data']['Meleeproof'] or false),
				['Tires_vulnerability'] = imgui.ImBool(table[tt]['Car_Data']['Tires_vulnerability'] or false),
			}
		}
		end
	end
	if typee == 3 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Object_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Object_Data']['Pos'][1],table[tt]['Object_Data']['Pos'][2],table[tt]['Object_Data']['Pos'][3]),
					['Rotates'] = imgui.ImFloat3(table[tt]['Object_Data']['Rotates'][1],table[tt]['Object_Data']['Rotates'][2],table[tt]['Object_Data']['Rotates'][3]),
					['ModelId'] = imgui.ImInt(table[tt]['Object_Data']['ModelId']),
					['StartC'] = imgui.ImInt(table[tt]['Object_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Object_Data']['EndC']),
					['Anims'] = {},
					['Anim_id'] = imgui.ImInt(0)
				}
			}
			for t = 1, #(table[tt]['Object_Data']['Anims'] or {}) do
				tableEnd[tt]['Object_Data']['Anims'][t] = {
				['Time'] = imgui.ImFloat(table[tt]['Object_Data']['Anims'][t]['Time']),
				['Condition'] = imgui.ImInt(table[tt]['Object_Data']['Anims'][t]['Condition']),
				['Pos'] = imgui.ImFloat3(table[tt]['Object_Data']['Anims'][t]['Pos'][1],table[tt]['Object_Data']['Anims'][t]['Pos'][2],table[tt]['Object_Data']['Anims'][t]['Pos'][3]),
				['Rotates'] = imgui.ImFloat3(table[tt]['Object_Data']['Anims'][t]['Rotates'][1],table[tt]['Object_Data']['Anims'][t]['Rotates'][2],table[tt]['Object_Data']['Anims'][t]['Rotates'][3]),
				}
				if tableEnd[tt]['Object_Data']['Anims'][t]['Condition'].v == 1 then
					tableEnd[tt]['Object_Data']['Anims'][t]['Target'] = imgui.ImInt(table[tt]['Object_Data']['Anims'][t]['Target'])
				end
			end
		end
	end
	if typee == 4 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Pickup_Data'] = {
					['Type_pickup'] = imgui.ImInt(table[tt]['Pickup_Data']['Type_pickup']),
					['Pos'] = imgui.ImFloat3(table[tt]['Pickup_Data']['Pos'][1],table[tt]['Pickup_Data']['Pos'][2],table[tt]['Pickup_Data']['Pos'][3]),
					['StartC'] = imgui.ImInt(table[tt]['Pickup_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Pickup_Data']['EndC']),
					['spawn_type'] = imgui.ImInt(table[tt]['Pickup_Data']['spawn_type'])
				}
			}
			if table[tt]['Pickup_Data']['Type_pickup'] == 0 then
				tableEnd[tt]['Pickup_Data']['Ammo'] = imgui.ImInt(table[tt]['Pickup_Data']['Ammo'])
				tableEnd[tt]['Pickup_Data']['Weapon'] = imgui.ImInt(table[tt]['Pickup_Data']['Weapon'])
			end
			if table[tt]['Pickup_Data']['Type_pickup'] == 5 then
				tableEnd[tt]['Pickup_Data']['ModelId'] = imgui.ImInt(table[tt]['Pickup_Data']['ModelId'])
			end
		end
	end
	if typee == 5 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Particle_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Particle_Data']['Pos'][1],table[tt]['Particle_Data']['Pos'][2],table[tt]['Particle_Data']['Pos'][3]),
					['Rotates'] = imgui.ImFloat3(table[tt]['Particle_Data']['Rotates'][1],table[tt]['Particle_Data']['Rotates'][2],table[tt]['Particle_Data']['Rotates'][3]),
					['Rotates'] = imgui.ImFloat3(table[tt]['Particle_Data']['Rotates'][1],table[tt]['Particle_Data']['Rotates'][2],table[tt]['Particle_Data']['Rotates'][3]),
					['ModelId'] = imgui.ImInt(table[tt]['Particle_Data']['ModelId']),
					['StartC'] = imgui.ImInt(table[tt]['Particle_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Particle_Data']['EndC'])
				}
			}
		end
	end
	if typee == 6 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = table[tt]['Name'],
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Explosion_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Explosion_Data']['Pos'][1],table[tt]['Explosion_Data']['Pos'][2],table[tt]['Explosion_Data']['Pos'][3]),
					['Type'] = imgui.ImInt(table[tt]['Explosion_Data']['Type']),
					['StartC'] = imgui.ImInt(table[tt]['Explosion_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Explosion_Data']['EndC'])
				}
			}
			if tableEnd[tt]['Explosion_Data']['Type'].v == 0 then
				tableEnd[tt]['Explosion_Data']['Size_fire'] = imgui.ImInt(table[tt]['Explosion_Data']['Size_fire'])
				tableEnd[tt]['Explosion_Data']['Propagation_fire'] = imgui.ImInt(table[tt]['Explosion_Data']['Propagation_fire'])
			end
			if tableEnd[tt]['Explosion_Data']['Type'].v == 1 then
				tableEnd[tt]['Explosion_Data']['Type_explosion'] = imgui.ImInt(table[tt]['Explosion_Data']['Type_explosion'])
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
