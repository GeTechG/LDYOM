mp = {}
mp.miss = 0
glob = require 'game.globals'
koder = require 'TextToGTX'
encoding = require 'encoding'
encoding.default = 'CP1251'
u8 = encoding.UTF8
mp.thread = {}
mp.actors = {}
mp.cars = {}
mp.objects = {}
mp.pickup = {}
mp.particle = {}
mp.explosion = {}
mp.audio = {}
local thread_miss
local skip = 1
curr_target = 0

function mp.start_mission(listt,lista,listc,listo,listp,listpa,liste,missd,listau)
	misflag = mp.flagmis()
	mp.miss = mp.start()
	imgui.Process = false
	thread_miss = lua_thread.create(mp.main_mission,listt,lista,listc,listo,listp,listpa,liste,missd,listau)
end

function mp.flagmis()
  onmission = getGameGlobal(glob.ONMISSION)
  return onmission
end

function mp.start()
	misflag = mp.flagmis()
	if misflag == 0 then
		doFade(false, 200)
    setPlayerControl(PLAYER_HANDLE, false)
    wait(500)
    doFade(true, 200)
    setGameGlobal(glob.ONMISSION, 1)
    setPlayerControl(PLAYER_HANDLE, true)
    misflag = mp.flagmis()
    mp.miss = 1
    mp.defeat()
	end
	return mp.miss
end

function mp.defeat()
  lua_thread.create(function()
    wait(0)
    while isPlayerPlaying(PLAYER_HANDLE) do
      wait(0)
		end
		setPlayerModel(PLAYER_HANDLE,model.NULL)
		while not isPlayerPlaying(PLAYER_HANDLE) do
      wait(0)
		end
		thread_miss:terminate()
		mp.fall()
		mp.endmiss()
  end)
end

function mp.pass(money)
  setGameGlobal(glob.ONMISSION, 0)
  printStyledString(koder('Затащено'), 4000, 1)
  playMissionPassedTune(1)
  givePlayerMoney(PlayerPed, money, 200)
  mp.miss = 0
end

function mp.respect()
  setGameGlobal(glob.ONMISSION, 0)
  printStyledString(koder('Затащено. ~w~Уважение +'), 4000, 1)
  playMissionPassedTune(1)
  mp.miss = 0
end

function mp.fall()
  setGameGlobal(glob.ONMISSION, 0)
  printWithNumberBig("M_FAIL", 100, 4000, 1)
  mp.miss = 0
end

function audio_player(list_a,list_c,list_o,list_au)
	for a = 1,#list_au do
		if list_au[a]['Audio_Data']['3daudioW'] then
			if list_au[a]['Audio_Data']['Place_type'] == 2 then
				setPlay3dAudioStreamAtChar(mp.audio[a],mp.actors[list_au[a]['Audio_Data']['Place']])
			elseif list_au[a]['Audio_Data']['Place_type'] == 3 then
				setPlay3dAudioStreamAtCar(mp.audio[a],mp.cars[list_au[a]['Audio_Data']['Place']])
			elseif list_au[a]['Audio_Data']['Place_type'] == 4 then
				setPlay3dAudioStreamAtObject(mp.audio[a],mp.objects[list_au[a]['Audio_Data']['Place']])
			end
		end
		if list_au[a]['Audio_Data']['Type'] == 1 then
			if list_au[a]['Audio_Data']['Target'] == curr_target and getAudioStreamState(mp.audio[a]) == -1 then
				setAudioStreamState(mp.audio[a],1)
			elseif list_au[a]['Audio_Data']['To_target'] == curr_target+1 then
				releaseAudioStream(mp.audio[a])
			end
		elseif list_au[a]['Audio_Data']['Type'] == 2 then
			if list_au[a]['Audio_Data']['Actor_act'] == list_a[list_au[a]['Audio_Data']['Actor']]['Actor_Data']['curr_anim']+1 and getAudioStreamState(mp.audio[a]) == -1 then

				setAudioStreamState(mp.audio[a],1)
			end
		elseif list_au[a]['Audio_Data']['Type'] == 3 then
			if list_au[a]['Audio_Data']['Car_act'] == list_c[list_au[a]['Audio_Data']['Car']]['Car_Data']['curr_anim']+1 and getAudioStreamState(mp.audio[a]) == -1 then
				setAudioStreamState(mp.audio[a],1)
			end
		elseif list_au[a]['Audio_Data']['Type'] == 4 then
			if list_au[a]['Audio_Data']['Object_act'] == (list_o[list_au[a]['Audio_Data']['Object']]['Object_Data']['curr_anim'] or -1) and getAudioStreamState(mp.audio[a]) == -1 then
				setAudioStreamState(mp.audio[a],1)
			end
		end
	end
end


function char_is_not_dead(ped)
	lua_thread.create(function()
	while not isCharDead(ped) do
		wait(0)
	end
	thread_miss:terminate()
	mp.fall()
	mp.endmiss()
	end)
end

function mp.play_char_anims(ped,actr)
	local curr_anim = 1
	actr['curr_anim'] = curr_anim
	while curr_anim <= #actr['Anims'] do
		wait(0)
		if actr['Anims'][curr_anim]['Type'] == 1 then
			if actr['Anims'][curr_anim]['Condition'] == 1 then
				if not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']]) then
					requestAnimation(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']])
					while not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']]) do
						wait(0)
					end
				end
				local animm = Anims['Anim_list'][actr['Anims'][curr_anim]['Pack_anim']]
				animm = animm[actr['Anims'][curr_anim]['Anim']]
				if not actr['Anims'][curr_anim]['Unbreakable'] then
					taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['Time']*1000))
				else
					taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['Time']*1000),false,false)
				end
				if not actr['Anims'][curr_anim]['Loop'] then
					wait(actr['Anims'][curr_anim]['Time'] * 1000.0)
				end
				curr_anim = curr_anim + 1
			elseif actr['Anims'][curr_anim]['Condition'] == 2 then
				if curr_target == actr['Anims'][curr_anim]['Target'] then
					if not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']]) then
						requestAnimation(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']])
						while not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']]) do
							wait(0)
						end
					end
					local animm = Anims['Anim_list'][actr['Anims'][curr_anim]['Pack_anim']]
					animm = animm[actr['Anims'][curr_anim]['Anim']]
					if not actr['Anims'][curr_anim]['Unbreakable'] then
						taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['Time']*1000))
					else
						taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['Time']*1000),false,false)
					end
					wait(actr['Anims'][curr_anim]['Time'] * 1000.0)
					curr_anim = curr_anim + 1
				end
			end
		elseif actr['Anims'][curr_anim]['Type'] == 2 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			local type_walk = 4;
			if actr['Anims'][curr_anim]['Type_move'] == 3 then type_walk = 6
			elseif actr['Anims'][curr_anim]['Type_move'] == 4 then type_walk = 7 end
			taskToggleDuck(ped, ternar(actr['Anims'][curr_anim]['Type_move'] == 0,true,false))
			repeat
				for i = 1,#actr['Anims'][curr_anim]['Path'] do
					local x1,y1,z1 = actr['Anims'][curr_anim]['Path'][i][1],actr['Anims'][curr_anim]['Path'][i][2],actr['Anims'][curr_anim]['Path'][i][3]
					taskGoStraightToCoord(ped,x1,y1,z1,type_walk,-1)
					local px,py,pz = getCharCoordinates(ped)
					while getDistanceBetweenCoords3d(x1,y1,z1,px,py,pz) > 1 do
						wait(0)
						px,py,pz = getCharCoordinates(ped)
					end
				end
				wait(0)
			until actr['Anims'][curr_anim]['Type_route'] == 1
			taskToggleDuck(ped, false)

			if actr['Anims'][curr_anim]['wait_end'] then
				while getDistanceBetweenCoords3d(actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][1],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][2],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][3],px,py,pz) > 0.1 do
					wait(0)
					px,py,pz = getCharCoordinates(ped)
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'] == 3 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			local px,py,pz = getCharCoordinates(ped)
			repeat
				for i = 1,#actr['Anims'][curr_anim]['Path'] do
					local x1,y1,z1 = actr['Anims'][curr_anim]['Path'][i][1],actr['Anims'][curr_anim]['Path'][i][2],actr['Anims'][curr_anim]['Path'][i][3]
					taskCarDriveToCoord(ped,mp.cars[actr['Anims'][curr_anim]['Car']],x1,y1,z1,actr['Anims'][curr_anim]['Speed'],0,0,0)
					px,py,pz = getCharCoordinates(ped)
					while getDistanceBetweenCoords3d(px,py,pz,x1,y1,z1) > 5 do
						wait(0)
						px,py,pz = getCharCoordinates(ped)
					end
				end
				wait(0)
			until not actr['Anims'][curr_anim]['loop']

			if actr['Anims'][curr_anim]['wait_end'] then
				while getDistanceBetweenCoords3d(actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][1],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][2],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][3],px,py,pz) > 1 do
					wait(0)
					px,py,pz = getCarCoordinates(mp.cars[actr['Anims'][curr_anim]['Car']])
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'] == 4 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			if actr['Anims'][curr_anim]['wait_end'] then
				if isCharInAnyCar(ped) then
					taskLeaveAnyCar(ped)
					while isCharInAnyCar(ped) do
						wait(0)
					end
				end
			end
			curr_anim = curr_anim + 1

		elseif actr['Anims'][curr_anim]['Type'] == 5 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			taskCarMission(ped,mp.cars[actr['Anims'][curr_anim]['Car_a']],mp.cars[actr['Anims'][curr_anim]['Car_t']],actr['Anims'][curr_anim]['Vehicle_mission']-1,actr['Anims'][curr_anim]['Speed'],actr['Anims'][curr_anim]['trafficFlag'])
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'] == 6 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			if actr['Anims'][curr_anim]['place_in_car'] == 1 then
				setNextDesiredMoveState(actr['Anims'][curr_anim]['speed_walk']+4)
				if actr['Anims'][curr_anim]['teleport'] then
					taskEnterCarAsDriver(ped,mp.cars[actr['Anims'][curr_anim]['Car']],0)
				else
					taskEnterCarAsDriver(ped,mp.cars[actr['Anims'][curr_anim]['Car']],-1)
				end
			else
				setNextDesiredMoveState(actr['Anims'][curr_anim]['speed_walk']+4)
				if actr['Anims'][curr_anim]['teleport'] then
					taskEnterCarAsPassenger(ped,mp.cars[actr['Anims'][curr_anim]['Car']],0,actr['Anims'][curr_anim]['place_in_car']-1)
				else
					taskEnterCarAsPassenger(ped,mp.cars[actr['Anims'][curr_anim]['Car']],-1,actr['Anims'][curr_anim]['place_in_car']-1)
				end
			end
			if actr['Anims'][curr_anim]['wait_end'] then
				while not isCharInAnyCar(ped) do
					wait(0)
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'] == 7 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			while not (actr['Anims'][curr_anim]['To_target']+1 == curr_target) do
				wait(0)
				local xa,ya,za = getCharCoordinates(ped)
				if actr['Anims'][curr_anim]['Target_actor_id'] == 1 then
					local xp,yp,zp = getCharCoordinates(PLAYER_PED)
					print(getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['Radius'],getDistanceBetweenCoords2d(xa,ya,xp,yp))
					if getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['Radius'] then
						taskGotoChar(ped,PLAYER_PED,-1,actr['Anims'][curr_anim]['Radius'])
					end
				else
					local xp,yp,zp = getCharCoordinates(mp.actors[actr['Anims'][curr_anim]['Target_actor_id']-1])
					print(getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['Radius'],getDistanceBetweenCoords2d(xa,ya,xp,yp))
					if getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['Radius'] then
						taskGotoChar(ped,mp.actors[actr['Anims'][curr_anim]['Target_actor_id']-1],-1,actr['Anims'][curr_anim]['Radius'])
					end
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'] == 8 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			requestModel(getWeapontypeModel(ID_Weapons[actr['Anims'][curr_anim]['Weapon']]))
			while not hasModelLoaded(getWeapontypeModel(ID_Weapons[actr['Anims'][curr_anim]['Weapon']])) do
				wait(0)
			end
			giveWeaponToChar(ped, ID_Weapons[actr['Anims'][curr_anim]['Weapon']], actr['Anims'][curr_anim]['Ammo'])
			setCurrentCharWeapon(ped,1)
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'] == 9 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			removeAllCharWeapons(ped)
			curr_anim = curr_anim + 1
		end
		actr['curr_anim'] = curr_anim
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audio)
	end
end

function mp.play_car_anims(car,vehic)
	local curr_anim = 1
	vehic['curr_anim'] = curr_anim
	while curr_anim <= #vehic['Anims'] do
		wait(0)
		if vehic['Anims'][curr_anim]['Type'] == 1 then
			if vehic['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == vehic['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			closeAllCarDoors(car)
			for g = 1,6 do
				print(g)
				if vehic['Anims'][curr_anim]['Doors'][g] then
					openCarDoor(car,g-1)
				end
			end
			curr_anim = curr_anim + 1
		elseif vehic['Anims'][curr_anim]['Type'] == 2 then
			if vehic['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == vehic['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			lockCarDoors(car,vehic['Anims'][curr_anim]['Door_lock'])
			curr_anim = curr_anim + 1
		end
		vehic['curr_anim'] = curr_anim
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audio)
	end
end

function car_is_not_dead(car)
	lua_thread.create(function()
	while not isCarDead(car) do
		wait(0)
	end
	thread_miss:terminate()
	mp.fall()
	mp.endmiss()
	end)
end

function mp.play_obj_anims(obj,obj_data)
	local curr_anim = 1
	obj_data['curr_anim'] = curr_anim
	local rotate_obj = {}
	rotate_obj.x,rotate_obj.y,rotate_obj.z = obj_data['Rotates'][1],obj_data['Rotates'][2],obj_data['Rotates'][3]
	while curr_anim <= #obj_data['Anims'] do
		wait(0)
		local step_pos = {}
		local startpos = {}
		local curpos = {}
		local step_angle = {}
		startpos.x,startpos.y,startpos.z = select(2,getObjectCoordinates(obj)),select(3,getObjectCoordinates(obj)),select(4,getObjectCoordinates(obj))

		step_pos.x = (obj_data['Anims'][curr_anim]['Pos'][1] - startpos.x) / (obj_data['Anims'][curr_anim]['Time'] * 1000)
		step_pos.y = (obj_data['Anims'][curr_anim]['Pos'][2] - startpos.y) / (obj_data['Anims'][curr_anim]['Time'] * 1000)
		step_pos.z = (obj_data['Anims'][curr_anim]['Pos'][3] - startpos.z) / (obj_data['Anims'][curr_anim]['Time'] * 1000)

		step_angle.x = (obj_data['Anims'][curr_anim]['Rotates'][1] - rotate_obj.x) / (obj_data['Anims'][curr_anim]['Time'] * 1000)
		step_angle.y = (obj_data['Anims'][curr_anim]['Rotates'][2] - rotate_obj.y) / (obj_data['Anims'][curr_anim]['Time'] * 1000)
		step_angle.z = (obj_data['Anims'][curr_anim]['Rotates'][3] - rotate_obj.z) / (obj_data['Anims'][curr_anim]['Time'] * 1000)
		if obj_data['Anims'][curr_anim]['Condition'] == 2 then
			while curr_target ~= obj_data['Anims'][curr_anim]['Target'] do
				wait(0)
			end
		end
		local time = 0
		while time <= obj_data['Anims'][curr_anim]['Time'] * 1000 do
			local old_time = os.clock()
			wait(1)
			local delta = (os.clock() - old_time) * 1000

			curpos.x,curpos.y,curpos.z = select(2,getObjectCoordinates(obj)),select(3,getObjectCoordinates(obj)),select(4,getObjectCoordinates(obj))
			setObjectCoordinates(obj, curpos.x+step_pos.x*delta, curpos.y+step_pos.y*delta, curpos.z+step_pos.z*delta)

			rotate_obj.x,rotate_obj.y,rotate_obj.z = rotate_obj.x+step_angle.x*delta,rotate_obj.y+step_angle.y*delta,rotate_obj.z+step_angle.z*delta
			setObjectRotation(obj, rotate_obj.x, rotate_obj.y, rotate_obj.z)

			time = time + 1*delta
		end
		curr_anim = curr_anim + 1
		obj_data['curr_anim'] = curr_anim
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audio)
	end
end

function mp.main_mission(list,list_a,list_c,list_o,list_p,list_pa,list_e,miss_data,list_au)
	printStyledString(koder(u8:decode(miss_data['Name'])), 2000, 2)
	setTimeOfDay(miss_data['Time'][1], miss_data['Time'][2])
	forceWeatherNow(miss_data['Weather'])
	setLaRiots(miss_data['Riot'])
	setCharCoordinates(PLAYER_PED, miss_data['Player']['Pos'][1], miss_data['Player']['Pos'][2], miss_data['Player']['Pos'][3])
	setCharHeading(PLAYER_PED, miss_data['Player']['Angle'])
	if miss_data['Player']['Health'] > 100 then
		setCharMaxHealth(PLAYER_PED,1000)
	end
	setCharHealth(PLAYER_PED,miss_data['Player']['Health'])
	setMaxWantedLevel(6)
	setCharInterior(PLAYER_PED, miss_data['Player']['Interior_id'])
	setInteriorVisible(miss_data['Player']['Interior_id'])
	local modell
	if miss_data['Player']['ped_type'] then
		modell = ID_Actors[miss_data['Player']['ModelId']]
		requestModel(modell)
		while not hasModelLoaded(modell) do
			wait(0)
		end
	else
		local modell_n = ID_Spec_Actors[miss_data['Player']['ModelId']]
		loadSpecialCharacter(modell_n,10)
		while not hasSpecialCharacterLoaded(10) do
			wait(0)
		end
		modell = 290 + 10-1
	end
	setPlayerModel(PLAYER_HANDLE, modell)
	requestModel(getWeapontypeModel(ID_Weapons[miss_data['Player']['Weapon']]))
	while not hasModelLoaded(getWeapontypeModel(ID_Weapons[miss_data['Player']['Weapon']])) do
		wait(0)
	end
	giveWeaponToChar(PLAYER_PED, ID_Weapons[miss_data['Player']['Weapon']], miss_data['Player']['Weap_ammo'])
	setCurrentCharWeapon(PLAYER_PED,1)
	markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[miss_data['Player']['Weapon']]))
	for i = 2,9 do
		if vr.Group_relationships[i] ~= 'NULL' then
			for y = 1,9 do
				if vr.Group_relationships[i][y] ~= 'NULL' then
					if y == 1 then
						setRelationship(vr.Group_relationships[i][y]-1,24+i-2,0)
						setRelationship(vr.Group_relationships[i][y]-1,24+i-2,23)
						setRelationship(vr.Group_relationships[i][y]-1,23,24+i-2)
					else
						setRelationship(vr.Group_relationships[i][y]-1,24+i-2,24+y-2)
					end
				end
			end
		end
	end
	for a = 1,#list_au do
		print(list_au[a]['Audio_Data']['3daudioW'])
		if list_au[a]['Audio_Data']['3daudioW'] then
			mp.audio[a] = load3dAudioStream(vr.Audios_pack[list_au[a]['Audio_Data']['Audio']])
			if list_au[a]['Audio_Data']['Place_type'] == 1 then
				local xx,yy,zz = list_au[a]['Audio_Data']['Pos'][1],list_au[a]['Audio_Data']['Pos'][2],list_au[a]['Audio_Data']['Pos'][3]
				setPlay3dAudioStreamAtCoordinates(mp.audio[a],xx,yy,zz)
			end
		else
			mp.audio[a] = loadAudioStream(vr.Audios_pack[list_au[a]['Audio_Data']['Audio']])
		end
		setAudioStreamLooped(mp.audio[a],list_au[a]['Audio_Data']['Repeat'])
	end
	--mp.thread[#mp.thread+1] = lua_thread.create(audio_player,list_a,list_c,list_o,list_au)
	for i = 1,#list do
		curr_target = i
		for a = 1,#list_a do
			if list_a[a]['Actor_Data']['StartC'] == i then
				if (not list_a[a]['Actor_Data']['Random_spawn']) or (math.random(0,100) > 50) then
					local xx,xy,xz = list_a[a]['Actor_Data']['Pos'][1], list_a[a]['Actor_Data']['Pos'][2], list_a[a]['Actor_Data']['Pos'][3]
					local modell
					if list_a[a]['Actor_Data']['Model_type'] == 1 then
						modell = ID_Actors[list_a[a]['Actor_Data']['ModelId']]
						requestModel(modell)
						while not hasModelLoaded(modell) do
							wait(0)
						end
					elseif list_a[a]['Actor_Data']['Model_type'] == 2 then
						local modell_n = ID_Spec_Actors[list_a[a]['Actor_Data']['ModelId']]
						loadSpecialCharacter(modell_n,list_a[a]['Actor_Data']['Slot_model'])
						while not hasSpecialCharacterLoaded(list_a[a]['Actor_Data']['Slot_model']) do
							wait(0)
						end
						modell = 290 + list_a[a]['Actor_Data']['Slot_model']-1
					end
					if list_a[a]['Actor_Data']['Group'] == 1 then
						mp.actors[a] = createChar(23, modell, xx, xy, xz)
						local g = getPlayerGroup(PLAYER_HANDLE)
						setGroupMember(g,mp.actors[a])
					else
						mp.actors[a] = createChar(24 + list_a[a]['Actor_Data']['Group']-2, modell, xx, xy, xz)
					end
					setCharHealth(mp.actors[a],list_a[a]['Actor_Data']['Health'])
					setCharHeading(mp.actors[a], list_a[a]['Actor_Data']['Angle'])
					requestModel(getWeapontypeModel(ID_Weapons[list_a[a]['Actor_Data']['Weapon']]))
					while not hasModelLoaded(getWeapontypeModel(ID_Weapons[list_a[a]['Actor_Data']['Weapon']])) do
						wait(0)
					end
					giveWeaponToChar(mp.actors[a], ID_Weapons[list_a[a]['Actor_Data']['Weapon']], list_a[a]['Actor_Data']['Ammo'])
					setCurrentCharWeapon(mp.actors[a],1)
					markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[list_a[a]['Actor_Data']['Weapon']]))
					wait(0)
					if #list_a[a]['Actor_Data']['Anims'] > 0 then
						mp.thread[#mp.thread+1] = lua_thread.create(mp.play_char_anims,mp.actors[a], list_a[a]['Actor_Data'])
					end
					if list_a[a]['Actor_Data']['Should_not_die'] == true then
						mp.thread[#mp.thread+1] = lua_thread.create(char_is_not_dead,mp.actors[a])
					end
				end
			end
			if list_a[a]['Actor_Data']['EndC'] ~= 1 and list_a[a]['Actor_Data']['EndC'] == i then
				deleteChar(mp.actors[a])
			end
		end
		for c = 1,#list_c do
			if list_c[c]['Car_Data']['StartC'] == i then
				local md = list_c[c]['Car_Data']['ModelId']
				local xx,xy,xz = list_c[c]['Car_Data']['Pos'][1], list_c[c]['Car_Data']['Pos'][2], list_c[c]['Car_Data']['Pos'][3]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.cars[c] = createCar(md, xx, xy, xz)
				setCarHealth(mp.cars[c],list_c[c]['Car_Data']['Health'])
				setCarHeading(mp.cars[c], list_c[c]['Car_Data']['Angle'])
				setCarProofs(mp.cars[c],list_c[c]['Car_Data']['Bulletproof'],list_c[c]['Car_Data']['Fireproof'],list_c[c]['Car_Data']['Explosionproof'],list_c[c]['Car_Data']['Collisionproof'],list_c[c]['Car_Data']['Meleeproof'])
				setCanBurstCarTires(mp.cars[c], not list_c[c]['Car_Data']['Tires_vulnerability'])
				changeCarColour(mp.cars[c], list_c[c]['Car_Data']['Color_primary'], list_c[c]['Car_Data']['Color_secondary'])
				if list_c[c]['Car_Data']['Should_not_die'] == true then
					mp.thread[#mp.thread+1] = lua_thread.create(car_is_not_dead,mp.cars[c])
				end
				if #list_c[c]['Car_Data']['Anims'] > 0 then
					mp.thread[#mp.thread+1] = lua_thread.create(mp.play_car_anims,mp.cars[c], list_c[c]['Car_Data'])
				end
			end
			if list_c[c]['Car_Data']['EndC'] ~= 1 and list_c[c]['Car_Data']['EndC'] == i then
				deleteCar(mp.cars[c])
			end
		end
		for o = 1,#list_o do
			if list_o[o]['Object_Data']['StartC'] == i then
				local md = list_o[o]['Object_Data']['ModelId']
				local xx,xy,xz = list_o[o]['Object_Data']['Pos'][1], list_o[o]['Object_Data']['Pos'][2], list_o[o]['Object_Data']['Pos'][3]
				local rxx,rxy,rxz = list_o[o]['Object_Data']['Rotates'][1], list_o[o]['Object_Data']['Rotates'][2], list_o[o]['Object_Data']['Rotates'][3]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.objects[o] = createObject(md, xx, xy, xz)
				setObjectCoordinates(mp.objects[o], xx, xy, xz)
				setObjectRotation(mp.objects[o], rxx, rxy, rxz)
				wait(0)
				mp.thread[#mp.thread+1] = lua_thread.create(mp.play_obj_anims,mp.objects[o],list_o[o]['Object_Data'])
			end
			if list_o[o]['Object_Data']['EndC'] ~= 1 and list_o[o]['Object_Data']['EndC'] == i then
				deleteObject(mp.objects[o])
			end
		end
		for p = 1,#list_pa do
			if list_pa[p]['Particle_Data']['StartC'] == i then
				local md = Particle_name[list_pa[p]['Particle_Data']['ModelId']]
				local xx,xy,xz = list_pa[p]['Particle_Data']['Pos'][1], list_pa[p]['Particle_Data']['Pos'][2], list_pa[p]['Particle_Data']['Pos'][3]
				local rxx,rxy,rxz = list_pa[p]['Particle_Data']['Rotates'][1], list_pa[p]['Particle_Data']['Rotates'][2], list_pa[p]['Particle_Data']['Rotates'][3]

				mp.particle[p] = createFxSystem(md, xx, xy, xz, 1)
				playFxSystem(mp.particle[p])
			end
			if list_pa[p]['Particle_Data']['EndC'] ~= 1 and list_pa[p]['Particle_Data']['EndC'] == i then
				killFxSystemNow(mp.particle[p])
			end
		end
		for p = 1,#list_p do
			if list_p[p]['Pickup_Data']['StartC'] == i then
				local xx,xy,xz = list_p[p]['Pickup_Data']['Pos'][1], list_p[p]['Pickup_Data']['Pos'][2], list_p[p]['Pickup_Data']['Pos'][3]
				local spawn_t = 3
				if list_p[p]['Pickup_Data']['spawn_type'] == 1 then
					spawn_t = 2
				elseif list_p[p]['Pickup_Data']['spawn_type'] == 2 then
					spawn_t = 15
				end

				if list_p[p]['Pickup_Data']['Type_pickup'] == 1 then
					local md = getWeapontypeModel(list_p[p]['Pickup_Data']['Weapon'])
					if not isModelAvailable(md) then
						requestModel(md)
						while not isModelAvailable(md) do
							wait(0)
						end
					end

					mp.pickup[p] = createPickupWithAmmo(md, spawn_t, list_p[p]['Pickup_Data']['Ammo'], xx, xy, xz)
				end
				if list_p[p]['Pickup_Data']['Type_pickup'] >= 2 then
					local md = 1240
					if list_p[p]['Pickup_Data']['Type_pickup'] == 3 then
						md = 1242
					elseif list_p[p]['Pickup_Data']['Type_pickup'] == 4 then
						md = 1247
					elseif list_p[p]['Pickup_Data']['Type_pickup'] == 5 then
						md = 1210
					elseif list_p[p]['Pickup_Data']['Type_pickup'] == 6 then
						md = list_p[p]['Pickup_Data']['ModelId']
					end

					if not isModelAvailable(md) then
						requestModel(md)
						while not isModelAvailable(md) do
							wait(0)
						end
					end
					mp.pickup[p] = select(2,createPickup(md, spawn_t, xx, xy, xz))
				end
			end
			if list_p[p]['Pickup_Data']['EndC'] ~= 1 and list_p[p]['Pickup_Data']['EndC'] == i then
				removePickup(mp.pickup[p])
			end
		end
		for e = 1,#list_e do
			if list_e[e]['Explosion_Data']['StartC'] == i then
				local xx,xy,xz = list_e[e]['Explosion_Data']['Pos'][1], list_e[e]['Explosion_Data']['Pos'][2], list_e[e]['Explosion_Data']['Pos'][3]

				if list_e[e]['Explosion_Data']['Type'] == 1 then
					mp.explosion[e] = startScriptFire(xx,xy,xz,list_e[e]['Explosion_Data']['Propagation_fire'],list_e[e]['Explosion_Data']['Size_fire'])
				end
				if list_e[e]['Explosion_Data']['Type'] == 2 then
					addExplosion(xx,xy,xz,list_e[e]['Explosion_Data']['Type_explosion'])
				end

			end
			if list_e[e]['Explosion_Data']['EndC'] ~= 1 and list_e[e]['Explosion_Data']['EndC'] == i then
				if list_e[e]['Explosion_Data']['Type'] == 1 then
					removeScriptFire(mp.explosion[e])
				end
			end
		end
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audio)
		if list[i]['Type'] == 1 then
			lockPlayerControl(false)
			wait(100)
			local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
			rad = list[i]['Target_Data']['Radius']
			--sph[i] = addSphere(xx,xy,xz,rad)
			printString(koder(u8:decode(list[i]['Target_Data']['Text'])),list[i]['Target_Data']['Text_time'] * 1000)
			local check = addBlipForCoord(xx,xy,xz)
			if list[i]['Target_Data']['Color_blip'] > 1 then
				changeBlipColour(check,list[i]['Target_Data']['Color_blip']-2)
			else
				setBlipAsFriendly(check, 1)
			end
			while not locateCharAnyMeans3d(PLAYER_PED,xx,xy,xz,rad,rad,rad,true) do
				wait(0)
			end
			removeBlip(check)
			--removeSphere(sph[i])
		end
		if list[i]['Type'] == 2 then
			lockPlayerControl(false)
			local check = addBlipForCar(mp.cars[list[i]['Target_Data']['Target_car_id']])
			if list[i]['Target_Data']['Color_blip'] > 1 then
				changeBlipColour(check,list[i]['Target_Data']['Color_blip']-2)
			else
				setBlipAsFriendly(check, 1)
			end
			printString(koder(u8:decode(list[i]['Target_Data']['Text'])), 2000)
			while not isCharInCar(PLAYER_PED, mp.cars[list[i]['Target_Data']['Target_car_id']]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'] == 3 then
			lockPlayerControl(false)
			printString(koder(u8:decode(list[i]['Target_Data']['Text'])), 2000)
			if list[i]['Target_Data']['Kill_group'] then
				local peds = {}
				local check = {}
				for a = 1,#mp.actors do
					if mp.actors[a] ~= nil and not isCharDead(mp.actors[a]) then
						if getPedType(mp.actors[a]) == getPedType(mp.actors[list[i]['Target_Data']['Target_actor_id']]) then
							peds[#peds+1] = mp.actors[a]
							check[#peds] = addBlipForChar(peds[#peds])
							if list[i]['Target_Data']['Color_blip'] > 1 then
								changeBlipColour(check[#peds],list[i]['Target_Data']['Color_blip']-2)
							else
								setBlipAsFriendly(check[#peds], 1)
							end
							setBlipAsFriendly(check[#peds], true)
						end
					end
				end
				local count_dead = 0
				while count_dead ~= #peds do
					wait(0)
					count_dead = 0
					for p = 1,#peds do
						if isCharDead(peds[p]) then
							count_dead = count_dead + 1
						end
					end
				end
				for b = 1,#check do
					removeBlip(check[b])
				end
			else
				local check = addBlipForChar(mp.actors[list[i]['Target_Data']['Target_actor_id']])
				if list[i]['Target_Data']['Color_blip'] > 1 then
					changeBlipColour(check,list[i]['Target_Data']['Color_blip']-2)
				else
					setBlipAsFriendly(check, 1)
				end
				setBlipAsFriendly(check, true)
				while not isCharDead(mp.actors[list[i]['Target_Data']['Target_actor_id']]) do
					wait(0)
				end
				removeBlip(check)
			end
		end
		if list[i]['Type'] == 4 then
			if list[i]['Target_Data']['Target_type'] == 1 then
				displayRadar(false)
				displayHud(false)
				lockPlayerControl(true)
        		switchWidescreen(true)
				if i > 1 and not (list[i-1]['Target_Data']['Target_type'] == 1 and list[i-1]['Type'] == 4) then
					doFade(false, 500)
					wait(1000)
					doFade(true, 500)
					skip = 1
				end
				local skipp = lua_thread.create(function()
					while not wasKeyPressed(vkeys.VK_SPACE) do
						wait(0)
					end
					skip = 0
					end)
				if list[i]['Target_Data']['Cutscene_pos'] == 1 then
					if list[i]['Target_Data']['Cutscene_look'] == 1 then
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local rxx,rxy,rxz = list[i]['Target_Data']['Rotates'][1],list[i]['Target_Data']['Rotates'][2],list[i]['Target_Data']['Rotates'][3]
						local x1,y1,z1 = xx,xy,xz
						x1 = x1 + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
						y1 = y1 + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
						z1 = z1 + 2*math.cos(math.rad(rxx))
						if list[i]['Target_Data']['Smooth'] then
							setInterpolationParameters(0,list[i]['Target_Data']['Text_time']*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtPoint(x1, y1, z1, 1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtPoint(x1, y1, z1, 2)
						end
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						wait(list[i]['Target_Data']['Text_time']*1000*skip)
					elseif list[i]['Target_Data']['Cutscene_look'] == 2 then
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['Target_Data']['Look_at']])
						if list[i]['Target_Data']['Smooth'] then
							setInterpolationParameters(0,list[i]['Target_Data']['Text_time']*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtChar(mp.actors[list[i]['Target_Data']['Look_at']],15,1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtChar(mp.actors[list[i]['Target_Data']['Look_at']],15,2)
							print('hs')
						end
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						wait(list[i]['Target_Data']['Text_time']*1000*skip)
					elseif list[i]['Target_Data']['Cutscene_look'] == 3 then
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['Target_Data']['Look_at']])
						if list[i]['Target_Data']['Smooth'] then
							setInterpolationParameters(0,list[i]['Target_Data']['Text_time']*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtCar(mp.cars[list[i]['Target_Data']['Look_at']],15,1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtCar(mp.cars[list[i]['Target_Data']['Look_at']],15,2)
						end
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						wait(list[i]['Target_Data']['Text_time']*1000*skip)
					end
				elseif list[i]['Target_Data']['Cutscene_pos'] == 2 then
					if list[i]['Target_Data']['Cutscene_look'] == 1 then
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local rxx,rxy,rxz = list[i]['Target_Data']['Rotates'][1],list[i]['Target_Data']['Rotates'][2],list[i]['Target_Data']['Rotates'][3]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						while timer < list[i]['Target_Data']['Text_time']*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCharCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
							local rx,ry,rz = cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1)
							rx = rx + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
							ry = ry + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
							rz = rz + 2*math.cos(math.rad(rxx))
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['Target_Data']['Cutscene_look'] == 2 then
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						while timer < list[i]['Target_Data']['Text_time']*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCharCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
							local rx,ry,rz = getCharCoordinates(mp.actors[list[i]['Target_Data']['Look_at']])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['Target_Data']['Cutscene_look'] == 3 then
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						while timer < list[i]['Target_Data']['Text_time']*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCharCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
							local rx,ry,rz = getCarCoordinates(mp.cars[list[i]['Target_Data']['Look_at']])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					end
				elseif list[i]['Target_Data']['Cutscene_pos'] == 3 then
					if list[i]['Target_Data']['Cutscene_look'] == 1 then
						local x1,y1,z1 = getCarCoordinates(mp.cars[list[i]['Target_Data']['Pos_tied']])
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local rxx,rxy,rxz = list[i]['Target_Data']['Rotates'][1],list[i]['Target_Data']['Rotates'][2],list[i]['Target_Data']['Rotates'][3]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						while timer < list[i]['Target_Data']['Text_time']*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['Target_Data']['Pos_tied']])
							local rx,ry,rz = cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1)
							rx = rx + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
							ry = ry + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
							rz = rz + 2*math.cos(math.rad(rxx))
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['Target_Data']['Cutscene_look'] == 2 then
						local x1,y1,z1 = getCarCoordinates(mp.actors[list[i]['Target_Data']['Pos_tied']])
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						while timer < list[i]['Target_Data']['Text_time']*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['Target_Data']['Pos_tied']])
							local rx,ry,rz = getCharCoordinates(mp.actors[list[i]['Target_Data']['Look_at']])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['Target_Data']['Cutscene_look'] == 3 then
						local x1,y1,z1 = getCarCoordinates(mp.cars[list[i]['Target_Data']['Pos_tied']])
						local xx,xy,xz = list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2],list[i]['Target_Data']['Pos'][3]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Text_time']*1000)
						while timer < list[i]['Target_Data']['Text_time']*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['Target_Data']['Pos_tied']])
							local rx,ry,rz = getCarCoordinates(mp.cars[list[i]['Target_Data']['Look_at']])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					end

				end
				if i < #list and not (list[i+1]['Target_Data']['Target_type'] == 1 and list[i+1]['Type'] == 4) then
					doFade(false, 500)
					wait(1000)
					doFade(true, 500)
				end
				skipp:terminate()
				displayRadar(true)
				displayHud(true)
				lockPlayerControl(false)
				switchWidescreen(false)
				restoreCameraJumpcut()
			end
			if list[i]['Target_Data']['Target_type'] == 2 then
				setPlayerControl(PLAYER_HANDLE, false)
				for t = 0,list[i]['Target_Data']['Time']-1 do
					printStyledString(tostring(list[i]['Target_Data']['Time'] - t), 1000, 4)
					wait(1000)
				end
				printStyledString('Go!', 1000, 4)
				wait(1000)
				setPlayerControl(PLAYER_HANDLE, true)
			end
			if list[i]['Target_Data']['Target_type'] == 3 then
				printString(koder(u8:decode(list[i]['Target_Data']['Text'])), list[i]['Target_Data']['Time']*1000)
				wait(list[i]['Target_Data']['Time']*1000)
			end
			if list[i]['Target_Data']['Target_type'] == 4 then
				forceWeatherNow(list[i]['Target_Data']['Weather'])
			end
			if list[i]['Target_Data']['Target_type'] == 5 then
				setTimeOfDay(list[i]['Target_Data']['Clock_time'][1], list[i]['Target_Data']['Clock_time'][2])
			end
			if list[i]['Target_Data']['Target_type'] == 6 then
				setPedDensityMultiplier(1 * list[i]['Target_Data']['Traffic'][1])
				setCarDensityMultiplier(1 * list[i]['Target_Data']['Traffic'][2])
			end
		end
		if list[i]['Type'] == 5 then
			lockPlayerControl(false)
			local check = addBlipForObject(mp.objects[list[i]['Target_Data']['Target_object_id']])
			if list[i]['Target_Data']['Color_blip'] > 1 then
			  changeBlipColour(check,list[i]['Target_Data']['Color_blip']-2)
			else
			  setBlipAsFriendly(check, 0)
				print('12')
			end

			if list[i]['Target_Data']['Target_type'] == 1 then
				while not isCharTouchingObject(PLAYER_PED, mp.objects[list[i]['Target_Data']['Target_object_id']]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'] == 2 then
				while not hasObjectBeenDamaged(mp.objects[list[i]['Target_Data']['Target_object_id']]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'] == 3 then
				while not hasObjectBeenPhotographed(mp.objects[list[i]['Target_Data']['Target_object_id']]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'] == 4 then
				while not hasObjectBeenDamagedByWeapon(mp.objects[list[i]['Target_Data']['Target_object_id']],list[i]['Target_Data']['Weap']) do
					wait(0)
				end
			end
			removeBlip(check)
		end
		if list[i]['Type'] == 6 then
			lockPlayerControl(false)
			local check = addBlipForPickup(mp.pickup[list[i]['Target_Data']['Target_pickup_id']])
			if list[i]['Target_Data']['Color_blip'] > 1 then
				changeBlipColour(check,list[i]['Target_Data']['Color_blip']-2)
			else
				setBlipAsFriendly(check, 1)
			end
			printString(koder(u8:decode(list[i]['Target_Data']['Text'])), 2000)
			while not hasPickupBeenCollected(mp.pickup[list[i]['Target_Data']['Target_pickup_id']]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'] == 7 then
			if list[i]['Target_Data']['Target_type'] == 1 then
				lockPlayerControl(false)
				if isCharInAnyCar(PLAYER_PED) then
					taskLeaveAnyCar(PLAYER_PED)
					while isCharInAnyCar(PLAYER_PED) do
						wait(0)
					end
				end
				setCharInterior(PLAYER_PED, list[i]['Target_Data']['Interior_id'])
				setInteriorVisible(list[i]['Target_Data']['Interior_id'])
				local modell
				if list[i]['Target_Data']['Model_type'] == 1 then
					modell = ID_Actors[list[i]['Target_Data']['ModelId']]
					requestModel(modell)
					while not hasModelLoaded(modell) do
						wait(0)
					end
				else
					local modell_n = ID_Spec_Actors[list[i]['Target_Data']['ModelId']]
					loadSpecialCharacter(modell_n,10)
					while not hasSpecialCharacterLoaded(10) do
						wait(0)
					end
					modell = 290 + 10-1
				end
				setPlayerModel(PLAYER_HANDLE, modell)
				setCharCoordinates(PLAYER_PED, list[i]['Target_Data']['Pos'][1], list[i]['Target_Data']['Pos'][2], list[i]['Target_Data']['Pos'][3])
				requestModel(getWeapontypeModel(list[i]['Target_Data']['Weapon']))
				while not hasModelLoaded(getWeapontypeModel(list[i]['Target_Data']['Weapon'])) do
					wait(0)

				end
				giveWeaponToChar(PLAYER_PED, ID_Weapons[list[i]['Target_Data']['Weapon']], list[i]['Target_Data']['Weap_ammo'])
				setCharHeading(PLAYER_PED, list[i]['Target_Data']['Angle'])
			end
			if list[i]['Target_Data']['Target_type'] == 2 then
				lockPlayerControl(true)
				if not hasAnimationLoaded(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim']]) then
					requestAnimation(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim']])
					while not hasAnimationLoaded(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim']]) do
						wait(0)
					end
				end
				local animm = Anims['Anim_list'][list[i]['Target_Data']['Pack_anim']]
				animm = animm[list[i]['Target_Data']['Anim']]
				print(animm)
				taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][list[i]['Target_Data']['Pack_anim']], 1.0, list[i]['Target_Data']['Loop'], false, false, false, -1)
				wait(1)
			end
			if list[i]['Target_Data']['Target_type'] == 3 then
				lockPlayerControl(false)
				if list[i]['Target_Data']['Car_place'] == 0 then
					taskWarpCharIntoCarAsDriver(PLAYER_PED, mp.cars[list[i]['Target_Data']['Car_id']])
				elseif list[i]['Target_Data']['Car_place'] > 0 then
					taskWarpCharIntoCarAsPassenger(PLAYER_PED, mp.cars[list[i]['Target_Data']['Car_id']],list[i]['Target_Data']['Car_place']-1)
				end
			end
			if list[i]['Target_Data']['Target_type'] == 4 then
				alterWantedLevel(PLAYER_HANDLE, list[i]['Target_Data']['Level_battue'])
				setMaxWantedLevel(6)
				print('hello')
			end
			if list[i]['Target_Data']['Target_type'] == 5 then
				removeAllCharWeapons(PLAYER_PED)
			end
			if list[i]['Target_Data']['Target_type'] == 6 then
				lockPlayerControl(false)
				if not hasModelLoaded(330) then
					requestModel(330)
					while not hasModelLoaded(330) do
						wait(1)
					end
				end
				phone = createObject(330, 0, 0, 0)
				wait(0)
				taskPickUpSecondObject(PLAYER_PED, phone, 0, 0, 0, 6, 16, 'phone_in', 'PED', -1)
				wait(900)
				taskPickUpObject(PLAYER_PED, phone, 0, 0, 0, 6, 16, 'NULL', 'NULL', -1)
				wait(1430)
				skip = 1
				local skipp = lua_thread.create(function()
				while not wasKeyPressed(vkeys.VK_SPACE) do
					wait(0)
				end
				skip = 0
				end)
				for d = 1,#list[i]['Target_Data']['Dialog'] do
					printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Dialog'][d]['Text']))), list[i]['Target_Data']['Dialog'][d]['Text_time']*1000)
					wait(list[i]['Target_Data']['Dialog'][d]['Text_time']*1000*skip)
				end
				skipp:terminate()
				removeObjectElegantly(phone)
				wait(0)
				phone = createObject(330, 0, 0, 0)
				taskPickUpSecondObject(PLAYER_PED, phone, 0, 0, 0, 6, 16, 'phone_out', 'PED', -1)
				taskPickUpObject(PLAYER_PED, phone, 0, 0, 0, 6, 16, 'NULL', 'NULL', -1)
				wait(1570)
				removeObjectElegantly(phone)
				wait(430)
				taskPickUpSecondObject(PLAYER_PED, phone, 0, 0, 0, 6, 16, 'phone_out', 'PED', -1)

			end
			if list[i]['Target_Data']['Target_type'] == 7 then
				givePlayerMoney(PLAYER_HANDLE, list[i]['Target_Data']['Add_money'])
			end
		end
	end
	mp.respect()
	mp.endmiss()
end

function mp.endmiss()
	local miss_data = vr.mission_data
	for t = 1, #mp.thread do
		mp.thread[t]:terminate()
	end
	wait(0)
	if isCharInAnyCar(PLAYER_PED) then
		taskLeaveAnyCar(PLAYER_PED)
		while isCharInAnyCar(PLAYER_PED) do
			wait(0)
		end
	end
	setLaRiots(false)
	setPlayerModel(PLAYER_HANDLE, model.NULL)
	removeAllCharWeapons(PLAYER_PED)
	alterWantedLevel(PLAYER_HANDLE, 0)
	setCharMaxHealth(PLAYER_PED,100)
	setCharHealth(PLAYER_PED,100)
	setPedDensityMultiplier(0)
	setCarDensityMultiplier(0)
	local allchar = getAllChars()
	for c = 1,#allchar do
		if allchar[c] ~= PLAYER_PED then
			deleteChar(allchar[c])
		end
	end
	setCharCoordinates(PLAYER_PED, miss_data['Player']['Pos'][1], miss_data['Player']['Pos'][2], miss_data['Player']['Pos'][3])
	setCharInterior(PLAYER_PED, miss_data['Player']['Interior_id'])
	setInteriorVisible(miss_data['Player']['Interior_id'])
	lockPlayerControl(true)
	setMaxWantedLevel(0)
	for i = 1,#mp.audio do
		setAudioStreamState(mp.audio[i],0)
	end
	for v,h in pairs(mp.actors) do
		deleteChar(mp.actors[v])
	end
	for v,h in pairs(mp.cars) do
		deleteCar(mp.cars[v])
	end
	for v,h in pairs(mp.objects) do
		deleteObject(mp.objects[v])
	end
	for v,h in pairs(mp.pickup) do
		removePickup(mp.pickup[v])
	end
	for v,h in pairs(mp.particle) do
		killFxSystem(mp.particle[v])
	end
	for v,h in pairs(mp.explosion) do
		killFxSystem(mp.explosion[v])
	end
	for a = 1,#vr.list_audio do
		if vr.list_audio[a]['Audio_Data']['3daudioW'] then
			if vr.list_audio[a]['Audio_Data']['Place_type'] == 1 then
				local xx,yy,zz = getCharCoordinates(PLAYER_PED)
				vr.list_audio[a]['Audio_Data']['Pos'] = {xx,yy,zz}
				requestModel(2231)
				while not hasModelLoaded(2231) do
					wait(0)
				end
				vr.list_audio[a]['Audio_Data']['Obj'] = createObject(2231,xx,yy,zz)
				setObjectCollision(vr.list_audio[a]['Audio_Data']['Obj'],false)
			end
		end
	end
	for j = 1,#vr.list_actors do
		update_actor(j)
	end
	for j = 1,#vr.list_cars do
		update_car(j)
	end
	for j = 1,#vr.list_objects do
		update_object(j)
	end
	for j = 1,#vr.list_pickup do
		update_pickup(j)
	end
	for j = 1,#vr.list_particle do
		update_particle(j)
	end
	for j = 1,#vr.list_explosion do
		update_explosion(j)
	end
end

return mp
