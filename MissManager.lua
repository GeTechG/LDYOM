manager = {}

json = require 'json'
encoding = require 'encoding'
--debtab = require 'debug_table'
encoding.default = 'CP1251'
u8 = encoding.UTF8
path = getWorkingDirectory() .. "\\Missions_pack\\"

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
				['Targets'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Targets'],0),
				['Actors'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Actors'],1),
				['Cars'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Cars'],2),
				['Objects'] = manager.sorterTable(pack['Missions'][p]['Mission_Data']['Objects'],3)
			}
		}
	end
	f = io.open(path .. 'LDYOM' .. tostring(num) .. '.json',"w")
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
			['Prename'] = imgui.ImBuffer(256),
			['Enable'] = imgui.ImBool(false),
			['Mission_Data'] = {
				['Targets'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Targets'],0),
				['Actors'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Actors'],1),
				['Cars'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Cars'],2),
				['Objects'] = manager.sorterJson(pack['Missions'][p]['Mission_Data']['Objects'],3)
			}
		}
	end
	return packk
end

function manager.delete(num)
	os.remove(path .. 'LDYOM' .. tostring(num) .. '.json')
end


function manager.sorterTable(table,typee)
	tableEnd = {}
	if typee == 0 then
		for tt = 1,#table do
			tableEnd[tt] = {}
			tableEnd[tt]['Name'] = u8:decode(table[tt]['Name'])
			tableEnd[tt]['Type'] = table[tt]['Type'].v
			if table[tt]['Type'].v == 0 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = {table[tt]['Target_Data']['Pos'].v[1],table[tt]['Target_Data']['Pos'].v[2],table[tt]['Target_Data']['Pos'].v[3]},
					['Radius'] = table[tt]['Target_Data']['Radius'].v,
					['Text'] = u8:decode(table[tt]['Target_Data']['Text'].v),
					['Text_time'] = table[tt]['Target_Data']['Text_time'].v,
					['Color_blip'] = table[tt]['Target_Data']['Color_blip'].v
				}
			end
			if table[tt]['Type'].v == 1 then
				tableEnd[tt]['Target_Data'] = {
					['Target_car_id'] = table[tt]['Target_Data']['Target_car_id'].v,
					['Color_blip'] = table[tt]['Target_Data']['Color_blip'].v
				}
			end
			if table[tt]['Type'].v == 2 then
				tableEnd[tt]['Target_Data'] = {
					['Target_actor_id'] = table[tt]['Target_Data']['Target_actor_id'].v,
					['Color_blip'] = table[tt]['Target_Data']['Color_blip'].v
				}
			end
			if table[tt]['Type'].v == 3 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = {table[tt]['Target_Data']['Pos'].v[1],table[tt]['Target_Data']['Pos'].v[2],table[tt]['Target_Data']['Pos'].v[3]},
					['Rotates'] = {table[tt]['Target_Data']['Rotates'].v[1],table[tt]['Target_Data']['Rotates'].v[2],table[tt]['Target_Data']['Rotates'].v[3]},
					['Text'] = u8:decode(table[tt]['Target_Data']['Text'].v),
					['Text_time'] = table[tt]['Target_Data']['Text_time'].v,
					['Smooth'] = table[tt]['Target_Data']['Smooth'].v
				}
			end
		end
	end
	if typee == 1 then
		for tt = 1, #table do
			tableEnd[tt] = {}
			tableEnd[tt]['Name'] = u8:decode(table[tt]['Name'])
			tableEnd[tt]['Type'] = table[tt]['Type'].v
			tableEnd[tt]['Actor_Data'] = {
				['Pos'] = {table[tt]['Actor_Data']['Pos'].v[1],table[tt]['Actor_Data']['Pos'].v[2],table[tt]['Actor_Data']['Pos'].v[3]},
				['Angle'] = table[tt]['Actor_Data']['Angle'].v,
				['ModelId'] = table[tt]['Actor_Data']['ModelId'].v,
				['StartC'] = table[tt]['Actor_Data']['StartC'].v,
				['EndC'] = table[tt]['Actor_Data']['EndC'].v,
				['Anims'] = {}
			}
			for t = 1, #table[tt]['Actor_Data']['Anims'] do
					tableEnd[tt]['Actor_Data']['Anims'][t] = {
					['Anim'] = table[tt]['Actor_Data']['Anims'][t]['Anim'].v,
					['Pack_anim'] = table[tt]['Actor_Data']['Anims'][t]['Pack_anim'].v,
					['Loop'] = table[tt]['Actor_Data']['Anims'][t]['Loop'].v,
					['Time'] = table[tt]['Actor_Data']['Anims'][t]['Time'].v,
					['Condition'] = table[tt]['Actor_Data']['Anims'][t]['Condition'].v
				}
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'] == 1 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = table[tt]['Actor_Data']['Anims'][t]['Target'].v
				end
			end
		end
	end
	if typee == 2 then
		for tt = 1, #table do
			tableEnd[tt] = {}
			tableEnd[tt]['Name'] = u8:decode(table[tt]['Name'])
			tableEnd[tt]['Type'] = table[tt]['Type'].v
			tableEnd[tt]['Car_Data'] = {
				['Pos'] = {table[tt]['Car_Data']['Pos'].v[1],table[tt]['Car_Data']['Pos'].v[2],table[tt]['Car_Data']['Pos'].v[3]},
				['Angle'] = table[tt]['Car_Data']['Angle'].v,
				['ModelId'] = table[tt]['Car_Data']['ModelId'].v,
				['StartC'] = table[tt]['Car_Data']['StartC'].v,
				['EndC'] = table[tt]['Car_Data']['EndC'].v
			}
		end
	end
	if typee == 3 then
		for tt = 1, #table do
			tableEnd[tt] = {}
			tableEnd[tt]['Name'] = u8:decode(table[tt]['Name'])
			tableEnd[tt]['Type'] = table[tt]['Type'].v
			tableEnd[tt]['Object_Data'] = {
				['Pos'] = {table[tt]['Object_Data']['Pos'].v[1],table[tt]['Object_Data']['Pos'].v[2],table[tt]['Object_Data']['Pos'].v[3]},
				['Rotates'] = {table[tt]['Object_Data']['Rotates'].v[1],table[tt]['Object_Data']['Rotates'].v[2],table[tt]['Object_Data']['Rotates'].v[3]},
				['ModelId'] = table[tt]['Object_Data']['ModelId'].v,
				['StartC'] = table[tt]['Object_Data']['StartC'].v,
				['EndC'] = table[tt]['Object_Data']['EndC'].v,
			}
		end
	end
	return tableEnd
end

function manager.sorterJson(table,typee)
	local tableEnd = {}
	if typee == 0 then
		for tt = 1, #table do
			tableEnd[tt] = {
				['Name'] = u8:encode(table[tt]['Name']),
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Enable'] = imgui.ImBool(false),
				['Target_Data'] = {}
			}
			if 	tableEnd[tt]['Type'].v == 0 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][1]),
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
				['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip'])
			}
			end
			if 	tableEnd[tt]['Type'].v == 2 then
				tableEnd[tt]['Target_Data'] = {
				['Target_actor_id'] = imgui.ImInt(table[tt]['Target_Data']['Target_actor_id']),
				['Color_blip'] = imgui.ImInt(table[tt]['Target_Data']['Color_blip'])
			}
			end
			if tableEnd[tt]['Type'].v == 3 then
				tableEnd[tt]['Target_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][1],table[tt]['Target_Data']['Pos'][1]),
					['Rotates'] = imgui.ImFloat3(table[tt]['Target_Data']['Rotates'][1],table[tt]['Target_Data']['Rotates'][1],table[tt]['Target_Data']['Rotates'][1]),
					['Text'] = imgui.ImBuffer(128),
					['Text_time'] = imgui.ImFloat(table[tt]['Target_Data']['Text_time']),
					['Smooth'] = imgui.ImBool(table[tt]['Target_Data']['Smooth'])
				}
				tableEnd[tt]['Target_Data']['Text'].v = table[tt]['Target_Data']['Text']
			end
		end
	end
	if typee == 1 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = u8:encode(table[tt]['Name']),'NoName',
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Enable'] = imgui.ImBool(false),
				['Actor_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Actor_Data']['Pos'][1],table[tt]['Actor_Data']['Pos'][2],table[tt]['Actor_Data']['Pos'][3]),
					['Angle'] = imgui.ImFloat(table[tt]['Actor_Data']['Angle']),
					['ModelId'] = imgui.ImInt(table[tt]['Actor_Data']['ModelId']),
					['StartC'] = imgui.ImInt(table[tt]['Actor_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Actor_Data']['EndC']),
					['Anims'] = {}
				}
			}
			for t = 1, #table[tt]['Actor_Data']['Anims'] do
				tableEnd[tt]['Actor_Data']['Anims'][t] = {
				['Anim'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Anim']),
				['Pack_anim'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Pack_anim']),
				['Loop'] = imgui.ImBool(table[tt]['Actor_Data']['Anims'][t]['Loop']),
				['Time'] = imgui.ImFloat(table[tt]['Actor_Data']['Anims'][t]['Time']),
				['Condition'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Condition'])
				}
				if tableEnd[tt]['Actor_Data']['Anims'][t]['Condition'].v == 1 then
					tableEnd[tt]['Actor_Data']['Anims'][t]['Target'] = imgui.ImInt(table[tt]['Actor_Data']['Anims'][t]['Target'])
				end
			end
		end
	end
	if typee == 2 then
		for tt = 1,#table do
			tableEnd[tt] = {
			['Name'] = u8:encode(table[tt]['Name']),
			['Type'] = imgui.ImInt(table[tt]['Type']),
			['Enable'] = imgui.ImBool(false),
			['Car_Data'] = {
				['Pos'] = imgui.ImFloat3(table[tt]['Actor_Data']['Pos'][1],table[tt]['Actor_Data']['Pos'][2],table[tt]['Actor_Data']['Pos'][3]),
				['Angle'] = imgui.ImFloat(table[tt]['Actor_Data']['Angle']),
				['ModelId'] = imgui.ImInt(table[tt]['Actor_Data']['ModelId']),
				['StartC'] = imgui.ImInt(table[tt]['Actor_Data']['StartC']),
				['EndC'] = imgui.ImInt(table[tt]['Actor_Data']['EndC'])
			}
		}
		end
	end
	if typee == 3 then
		for tt = 1,#table do
			tableEnd[tt] = {
				['Name'] = u8:encode(table[tt]['Name']),
				['Type'] = imgui.ImInt(table[tt]['Type']),
				['Enable'] = imgui.ImBool(false),
				['Object_Data'] = {
					['Pos'] = imgui.ImFloat3(table[tt]['Object_Data']['Pos'][1],table[tt]['Object_Data']['Pos'][2],table[tt]['Object_Data']['Pos'][3]),
					['Rotates'] = imgui.ImFloat3(table[tt]['Object_Data']['Rotates'][1],table[tt]['Object_Data']['Rotates'][2],table[tt]['Object_Data']['Rotates'][3]),
					['Angle'] = imgui.ImFloat(table[tt]['Object_Data']['Angle']),
					['ModelId'] = imgui.ImInt(table[tt]['Object_Data']['ModelId']),
					['StartC'] = imgui.ImInt(table[tt]['Object_Data']['StartC']),
					['EndC'] = imgui.ImInt(table[tt]['Object_Data']['EndC'])
				}
			}
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
