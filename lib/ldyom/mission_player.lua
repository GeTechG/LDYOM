mp = {}
mp.miss = 0
glob = require 'game.globals'
koder = require 'ldyom.TextToGTX'
encoding = require 'encoding'
encoding.default = 'CP1251'
model = require 'lib.game.models'
u8 = encoding.UTF8
mp.thread = {}
mp.actors = {}
mp.cars = {}
mp.objects = {}
mp.pickups = {}
mp.particles = {}
mp.explosions = {}
mp.audio = {}
local thread_miss
local skip = 1
curr_target = 0
mp.mission_work = false

function mp.start_mission(listt,lista,listc,listo,listp,listpa,liste,listau)
	misflag = mp.flagmis()
	mp.miss = mp.start()
	mp.mission_work = true
	thread_miss = lua_thread.create(mp.main_mission,listt,lista,listc,listo,listp,listpa,liste,listau)
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
  --printStyledString(koder('Затащено. ~w~Уважение +'), 4000, 1)
  printBig("M_PASSR", 4000, 1)
  playMissionPassedTune(1)
  mp.miss = 0
end

function mp.fall()
  setGameGlobal(glob.ONMISSION, 0)
  printWithNumberBig("M_FAIL", 100, 4000, 1)
  mp.miss = 0
end

function audio_player(a)
	local pathAudio = getWorkingDirectory()..'\\Missions_pack\\Pack missions ya\\audio\\'..vr.temp_var.list_audios_name[vr.list_audios[a]['data']['sound'][0]+1]..'.mp3'
	if vr.list_audios[a]['data']['audio3d'][0] then
		mp.audio[a] = load3dAudioStream(pathAudio)
		if vr.list_audios[a]['data']['audio3dType'][0] == 0 then
			local xx,yy,zz = vr.list_audios[a]['data']['pos'][0],vr.list_audios[a]['data']['pos'][1],vr.list_audios[a]['data']['pos'][2]
			setPlay3dAudioStreamAtCoordinates(mp.audio[a],xx,yy,zz)
		elseif vr.list_audios[a]['data']['audio3dType'][0] == 1 then
			setPlay3dAudioStreamAtChar(mp.audio[a],mp.actors[vr.list_audios[a]['data']['audio3dAttach'][0]+1])
		elseif vr.list_audios[a]['data']['audio3dType'][0] == 2 then
			setPlay3dAudioStreamAtCar(mp.audio[a],mp.cars[vr.list_audios[a]['data']['audio3dAttach'][0]+1])
		elseif vr.list_audios[a]['data']['audio3dType'][0] == 3 then
			setPlay3dAudioStreamAtObject(mp.audio[a],mp.objects[vr.list_audios[a]['data']['audio3dAttach'][0]+1])
		end
	else
		mp.audio[a] = loadAudioStream(pathAudio)
	end
	setAudioStreamState(mp.audio[a],1)
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
		if actr['Anims'][curr_anim]['type'][0] == 0 then
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
					taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['time'][0]*1000))
				else
					taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['time'][0]*1000),false,false)
				end
				if not actr['Anims'][curr_anim]['Loop'] then
					wait(actr['Anims'][curr_anim]['time'][0] * 1000.0)
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
						taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['time'][0]*1000))
					else
						taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim']], 1.0, actr['Anims'][curr_anim]['Loop'], true, true, actr['Anims'][curr_anim]['stopEndFrame'], ternar(actr['Anims'][curr_anim]['Loop'],-1,actr['Anims'][curr_anim]['time'][0]*1000),false,false)
					end
					wait(actr['Anims'][curr_anim]['time'][0] * 1000.0)
					curr_anim = curr_anim + 1
				end
			end
		elseif actr['Anims'][curr_anim]['type'][0] == 1 then
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
		elseif actr['Anims'][curr_anim]['type'][0] == 2 then
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
		elseif actr['Anims'][curr_anim]['type'][0] == 3 then
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

		elseif actr['Anims'][curr_anim]['type'][0] == 4 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			taskCarMission(ped,mp.cars[actr['Anims'][curr_anim]['Car_a']],mp.cars[actr['Anims'][curr_anim]['Car_t']],actr['Anims'][curr_anim]['Vehicle_mission']-1,actr['Anims'][curr_anim]['Speed'],actr['Anims'][curr_anim]['trafficFlag'])
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['type'][0] == 5 then
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
		elseif actr['Anims'][curr_anim]['type'][0] == 6 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			while not (actr['Anims'][curr_anim]['To_target']+1 == curr_target) do
				wait(0)
				local xa,ya,za = getCharCoordinates(ped)
				if actr['Anims'][curr_anim]['actor'][0]+1 == 1 then
					local xp,yp,zp = getCharCoordinates(PLAYER_PED)
					print(getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['radius'][0],getDistanceBetweenCoords2d(xa,ya,xp,yp))
					if getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['radius'][0] then
						taskGotoChar(ped,PLAYER_PED,-1,actr['Anims'][curr_anim]['radius'][0])
					end
				else
					local xp,yp,zp = getCharCoordinates(mp.actors[actr['Anims'][curr_anim]['actor'][0]+1-1])
					print(getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['radius'][0],getDistanceBetweenCoords2d(xa,ya,xp,yp))
					if getDistanceBetweenCoords2d(xa,ya,xp,yp) > actr['Anims'][curr_anim]['radius'][0] then
						taskGotoChar(ped,mp.actors[actr['Anims'][curr_anim]['actor'][0]+1-1],-1,actr['Anims'][curr_anim]['radius'][0])
					end
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['type'][0] == 7 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			requestModel(getWeapontypeModel(ID_Weapons[actr['Anims'][curr_anim]['weapon'][0]]))
			while not hasModelLoaded(getWeapontypeModel(ID_Weapons[actr['Anims'][curr_anim]['weapon'][0]])) do
				wait(0)
			end
			giveWeaponToChar(ped, ID_Weapons[actr['Anims'][curr_anim]['weapon'][0]], actr['Anims'][curr_anim]['ammo'][0])
			setCurrentCharWeapon(ped,1)
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['type'][0] == 8 then
			if actr['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == actr['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			removeAllCharWeapons(ped)
			curr_anim = curr_anim + 1
		end
		actr['curr_anim'] = curr_anim
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audios)
	end
end

function mp.play_car_anims(car,vehic)
	local curr_anim = 1
	vehic['curr_anim'] = curr_anim
	while curr_anim <= #vehic['Anims'] do
		wait(0)
		if vehic['Anims'][curr_anim]['type'][0] == 0 then
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
		elseif vehic['Anims'][curr_anim]['type'][0] == 1 then
			if vehic['Anims'][curr_anim]['Condition'] == 2 then
				while not (curr_target == vehic['Anims'][curr_anim]['Target']) do
					wait(0)
				end
			end
			lockCarDoors(car,vehic['Anims'][curr_anim]['Door_lock'])
			curr_anim = curr_anim + 1
		end
		vehic['curr_anim'] = curr_anim
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audios)
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
	rotate_obj.x,rotate_obj.y,rotate_obj.z = obj_data['rotate'][0],obj_data['rotate'][1],obj_data['rotate'][2]
	while curr_anim <= #obj_data['Anims'] do
		wait(0)
		local step_pos = {}
		local startpos = {}
		local curpos = {}
		local step_angle = {}
		startpos.x,startpos.y,startpos.z = select(2,getObjectCoordinates(obj)),select(3,getObjectCoordinates(obj)),select(4,getObjectCoordinates(obj))

		step_pos.x = (obj_data['Anims'][curr_anim]['pos'][0] - startpos.x) / (obj_data['Anims'][curr_anim]['time'][0] * 1000)
		step_pos.y = (obj_data['Anims'][curr_anim]['pos'][1] - startpos.y) / (obj_data['Anims'][curr_anim]['time'][0] * 1000)
		step_pos.z = (obj_data['Anims'][curr_anim]['pos'][2] - startpos.z) / (obj_data['Anims'][curr_anim]['time'][0] * 1000)

		step_angle.x = (obj_data['Anims'][curr_anim]['rotate'][0] - rotate_obj.x) / (obj_data['Anims'][curr_anim]['time'][0] * 1000)
		step_angle.y = (obj_data['Anims'][curr_anim]['rotate'][1] - rotate_obj.y) / (obj_data['Anims'][curr_anim]['time'][0] * 1000)
		step_angle.z = (obj_data['Anims'][curr_anim]['rotate'][2] - rotate_obj.z) / (obj_data['Anims'][curr_anim]['time'][0] * 1000)
		if obj_data['Anims'][curr_anim]['Condition'] == 2 then
			while curr_target ~= obj_data['Anims'][curr_anim]['Target'] do
				wait(0)
			end
		end
		local time = 0
		while time <= obj_data['Anims'][curr_anim]['time'][0] * 1000 do
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
		audio_player(vr.list_actors,vr.list_cars,vr.list_objects,vr.list_audios)
	end
end

function mp.main_mission(list,list_a,list_c,list_o,list_p,list_pa,list_e,list_au)
	--printStyledString(koder(u8:decode(miss_data['Name'])), 2000, 2)
	--setTimeOfDay(miss_data['time'][0][1], miss_data['time'][0][2])
	--forceWeatherNow(miss_data['weather'][0])
	--setLaRiots(miss_data['Riot'])
	setCharCoordinates(PLAYER_PED, vr.missData['player']['pos'][0], vr.missData['player']['pos'][1], vr.missData['player']['pos'][2])
	setCharHeading(PLAYER_PED, vr.missData['player']['angle'][0])
	if vr.missData['player']['health'][0] > 100 then
		setCharMaxHealth(PLAYER_PED,vr.missData['player']['health'][0])
	end
	setCharHealth(PLAYER_PED,vr.missData['player']['health'][0])
	-- setMaxWantedLevel(6)
	setCharInterior(PLAYER_PED, vr.missData['player']['interiorId'][0])
	setInteriorVisible(vr.missData['player']['interiorId'][0])
	local modell
	if vr.missData['player']['modelType'][0] == 0 then
		modell = vr.missData['player']['modelId'][0]
		requestModel(modell)
		while not hasModelLoaded(modell) do
			wait(0)
		end
	else
		local modell_n = ID_Spec_Actors[vr.missData['player']['modelId'][0]+1]
		loadSpecialCharacter(modell_n,10)
		while not hasSpecialCharacterLoaded(10) do
			wait(0)
		end
		modell = 290 + 10-1
	end
	setPlayerModel(PLAYER_HANDLE, modell)
	requestModel(getWeapontypeModel(ID_Weapons[vr.missData['player']['weapon'][0]]))
	while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.missData['player']['weapon'][0]])) do
		wait(0)
	end
	giveWeaponToChar(PLAYER_PED, ID_Weapons[vr.missData['player']['weapon'][0]], vr.missData['player']['ammo'][0])
	setCurrentCharWeapon(PLAYER_PED,1)
	markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[vr.missData['player']['weapon'][0]]))
	for i = 2,9 do
		if vr.missData.groupRelations[i] ~= 'NULL' then
			for y = 1,9 do
				if vr.missData.groupRelations[i][y] ~= 'NULL' then
					if y == 1 then
						setRelationship(vr.missData.groupRelations[i][y][0],24+i-2,0)
						setRelationship(vr.missData.groupRelations[i][y][0],24+i-2,23)
						setRelationship(vr.missData.groupRelations[i][y][0],23,24+i-2)
					else
						setRelationship(vr.missData.groupRelations[i][y][0],24+i-2,24+y-2)
					end
				end
			end
		end
	end
	--mp.thread[#mp.thread+1] = lua_thread.create(audio_player,list_a,list_c,list_o,list_au)
	for i = 1,#list do
		curr_target = i
		for a = 1,#list_a do
			if list_a[a]['data']['startC'][0]+1 == i then
				if (not list_a[a]['data']["randomSpawn"][0]) or (math.random(0,100) > 50) then
					local xx,xy,xz = list_a[a]['data']['pos'][0], list_a[a]['data']['pos'][1], list_a[a]['data']['pos'][2]
					local modell
					if list_a[a]['data']['modelType'][0] == 0 then
						modell = list_a[a]['data']['modelId'][0]
						requestModel(modell)
						while not hasModelLoaded(modell) do
							wait(0)
						end
					elseif list_a[a]['data']['modelType'][0] == 1 then
						local modell_n = ID_Spec_Actors[list_a[a]['data']['modelId'][0]+1]
						loadSpecialCharacter(modell_n,list_a[a]['data']['slotSkin'][0])
						while not hasSpecialCharacterLoaded(list_a[a]['data']['slotSkin'][0]) do
							wait(0)
						end
						modell = 290 + list_a[a]['data']['slotSkin'][0]-1
					end
					if list_a[a]['data']['group'][0] == 0 then
						mp.actors[a] = createChar(23, modell, xx, xy, xz)
						local g = getPlayerGroup(PLAYER_HANDLE)
						setGroupMember(g,mp.actors[a])
					else
						mp.actors[a] = createChar(24 + list_a[a]['data']['group'][0]-1, modell, xx, xy, xz)
					end
					setCharHealth(mp.actors[a],list_a[a]['data']['health'][0])
					setCharHeading(mp.actors[a], list_a[a]['data']['angle'][0])
					requestModel(getWeapontypeModel(ID_Weapons[list_a[a]['data']['weapon'][0]]))
					while not hasModelLoaded(getWeapontypeModel(ID_Weapons[list_a[a]['data']['weapon'][0]])) do
						wait(0)
					end
					giveWeaponToChar(mp.actors[a], ID_Weapons[list_a[a]['data']['weapon'][0]], list_a[a]['data']['ammo'][0])
					setCurrentCharWeapon(mp.actors[a],1)
					markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[list_a[a]['data']['weapon'][0]]))
					wait(0)
					-- if #list_a[a]['data']['Anims'] > 0 then
					-- 	mp.thread[#mp.thread+1] = lua_thread.create(mp.play_char_anims,mp.actors[a], list_a[a]['data'])
					-- end
					if list_a[a]['data']['shouldNotDie'][0] == true then
						mp.thread[#mp.thread+1] = lua_thread.create(char_is_not_dead,mp.actors[a])
					end
				end
			end
			if list_a[a]['data']['endC'][0]+1 ~= 1 and list_a[a]['data']['endC'][0]+1 == i then
				deleteChar(mp.actors[a])
			end
		end
		for c = 1,#list_c do
			if list_c[c]['data']['startC'][0]+1 == i then
				local md = list_c[c]['data']['modelId'][0]
				local xx,xy,xz = list_c[c]['data']['pos'][0], list_c[c]['data']['pos'][1], list_c[c]['data']['pos'][2]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.cars[c] = createCar(md, xx, xy, xz)
				setCarHealth(mp.cars[c],list_c[c]['data']['health'][0])
				setCarHeading(mp.cars[c], list_c[c]['data']['angle'][0])
				setCarProofs(mp.cars[c],list_c[c]['data']['bulletproof'][0],list_c[c]['data']['fireproof'][0],list_c[c]['data']['explosionproof'][0],list_c[c]['data']['collisionproof'][0],list_c[c]['data']['meleeproof'][0])
				setCanBurstCarTires(mp.cars[c], not list_c[c]['data']['tiresVulnerability'][0])
				for_each_vehicle_material(mp.cars[c],function(i,mat, comp, obj)
					local new_r, new_g, new_b, a = list_c[c]['data'].colors[i][2][0],list_c[c]['data'].colors[i][2][1],list_c[c]['data'].colors[i][2][2],list_c[c]['data'].colors[i][2][3]
					mat:set_color(new_r*255, new_g*255, new_b*255, a*255)
				end)
				if list_c[c]['data']['shouldNotDie'][0] == true then
					mp.thread[#mp.thread+1] = lua_thread.create(car_is_not_dead,mp.cars[c])
				end
				-- if #list_c[c]['data']['Anims'] > 0 then
				-- 	mp.thread[#mp.thread+1] = lua_thread.create(mp.play_car_anims,mp.cars[c], list_c[c]['data'])
				-- end
			end
			if list_c[c]['data']['endC'][0]+1 ~= 1 and list_c[c]['data']['endC'][0]+1 == i then
				deleteCar(mp.cars[c])
			end
		end
		for o = 1,#list_o do
			if list_o[o]['data']['startC'][0]+1 == i then
				local md = list_o[o]['data']['modelId'][0]
				local xx,xy,xz = list_o[o]['data']['pos'][0], list_o[o]['data']['pos'][1], list_o[o]['data']['pos'][2]
				local rxx,rxy,rxz = list_o[o]['data']['rotate'][0], list_o[o]['data']['rotate'][1], list_o[o]['data']['rotate'][2]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.objects[o] = createObject(md, xx, xy, xz)
				setObjectCoordinates(mp.objects[o], xx, xy, xz)
				setObjectRotation(mp.objects[o], rxx, rxy, rxz)
				wait(0)
				--mp.thread[#mp.thread+1] = lua_thread.create(mp.play_obj_anims,mp.objects[o],list_o[o]['data'])
			end
			if list_o[o]['data']['endC'][0]+1 ~= 1 and list_o[o]['data']['endC'][0]+1 == i then
				deleteObject(mp.objects[o])
			end
		end
		for p = 1,#list_pa do
			if list_pa[p]['data']['startC'][0]+1 == i then
				local md = Particle_name[list_pa[p]['data']['modelId'][0]+1]
				local xx,xy,xz = list_pa[p]['data']['pos'][0], list_pa[p]['data']['pos'][1], list_pa[p]['data']['pos'][2]
				local rxx,rxy,rxz = list_pa[p]['data']['rotate'][0], list_pa[p]['data']['rotate'][1], list_pa[p]['data']['rotate'][2]

				mp.particles[p] = {}
				if list_pa[p]['data'].tied[0] == 0 then
					if not hasModelLoaded(327) then
						requestModel(327)
						while not hasModelLoaded(327) do
							wait(0)
						end
					end
					mp.particles[p][2] = createObject(327, xx, xy, xz)
					mp.particles[p][1] = createFxSystemOnObjectWithDirection(md,mp.particles[p][2],0,0,0,rxx,rxy,rxz, 1)
					playFxSystem(mp.particles[p][1])
					wait(0)
					setObjectVisible(mp.particles[p][2],false)
					setObjectCoordinates(mp.particles[p][2], xx, xy, xz)
				elseif list_pa[p]['data'].tied[0] == 1 then
					local xt,yt,zt = getCharCoordinates(mp.actors[list_pa[p]['data']['tiedId'][0]+1])
					local angleTied = getCharHeading(mp.actors[list_pa[p]['data']['tiedId'][0]+1])
					xx,xy,xz = xx-xt,xy-yt,xz-zt
					xx,xy = rotateVec2(xx,xy,-angleTied)
					local xt,yt,zt = getCharCoordinates(mp.actors[list_pa[p]['data']['tiedId'][0]+1])
					xx,xy,xz = xx-xt,xy-yt,xz-zt
					mp.particles[p][1] = createFxSystemOnCharWithDirection(md,mp.actors[list_pa[p]['data']['tiedId'][0]+1],xx,xy,xz,rxx,rxy,rxz, 1)
					playFxSystem(mp.particles[p][1])
				elseif list_pa[p]['data'].tied[0] == 2 then
					local xt,yt,zt = getCarCoordinates(mp.cars[list_pa[p]['data']['tiedId'][0]+1])
					local angleTied = getCarHeading(mp.cars[list_pa[p]['data']['tiedId'][0]+1])
					xx,xy,xz = xx-xt,xy-yt,xz-zt
					xx,xy = rotateVec2(xx,xy,-angleTied)
					mp.particles[p][1] = createFxSystemOnCarWithDirection(md,mp.cars[list_pa[p]['data']['tiedId'][0]+1],xx,xy,xz,rxx,angleTied-rxy,rxz, 1)
					playFxSystem(mp.particles[p][1])
				elseif list_pa[p]['data'].tied[0] == 3 then
					local xt,yt,zt = getObjectCoordinates(mp.objects[list_pa[p]['data']['tiedId'][0]+1])
					local angleTied = getObjectHeading(mp.objects[list_pa[p]['data']['tiedId'][0]+1])
					xx,xy,xz = xx-xt,xy-yt,xz-zt
					xx,xy = rotateVec2(xx,xy,-angleTied)
					mp.particles[p][1] = createFxSystemOnObjectWithDirection(md,mp.objects[list_pa[p]['data']['tiedId'][0]+1],xx,xy,xz,rxx,angleTied-rxy,rxz, 1)
					playFxSystem(mp.particles[p][1])
				end
			end
			if list_pa[p]['data']['endC'][0]+1 ~= 1 and list_pa[p]['data']['endC'][0]+1 == i then
					killFxSystemNow(mp.particles[p][1])
					if mp.particles[p][2] then deleteObject(mp.particles[p][2]) end
			end
		end
		for p = 1,#list_p do
			if list_p[p]['data']['startC'][0]+1 == i then
				local xx,xy,xz = list_p[p]['data']['pos'][0], list_p[p]['data']['pos'][1], list_p[p]['data']['pos'][2]
				local spawn_t = 3
				if list_p[p]['data']['spawnType'][0] == 1 then
					spawn_t = 2
				elseif list_p[p]['data']['spawnType'][0] == 2 then
					spawn_t = 15
				end

				if list_p[p]['data']['type'][0] == 0 then
					local md = getWeapontypeModel(ID_Weapons[list_p[p]['data']['weapon'][0]])
					if not isModelAvailable(md) then
						requestModel(md)
						while not isModelAvailable(md) do
							wait(0)
						end
					end
					mp.pickups[p] = createPickupWithAmmo(md, spawn_t, list_p[p]['data']['ammo'][0], xx, xy, xz)
				end
				if list_p[p]['data']['type'][0] >= 1 then
					local md = 1240
					if list_p[p]['data']['type'][0] == 2 then
						md = 1242
					elseif list_p[p]['data']['type'][0] == 3 then
						md = 1247
					elseif list_p[p]['data']['type'][0] == 4 then
						md = 1241
					elseif list_p[p]['data']['type'][0] == 5 then
						md = list_p[p]['data']['modelId'][0]
					end

					if not isModelAvailable(md) then
						requestModel(md)
						while not isModelAvailable(md) do
							wait(0)
						end
					end
					mp.pickups[p] = select(2,createPickup(md, spawn_t, xx, xy, xz))
				end
			end
			if list_p[p]['data']['endC'][0]+1 ~= 1 and list_p[p]['data']['endC'][0]+1 == i then
				removePickup(mp.pickups[p])
			end
		end
		for e = 1,#list_e do
			if list_e[e]['data']['startC'][0]+1 == i then
				local xx,xy,xz = list_e[e]['data']['pos'][0], list_e[e]['data']['pos'][1], list_e[e]['data']['pos'][2]

				if list_e[e]['data']['type'][0] == 0 then
					mp.explosions[e] = startScriptFire(xx,xy,xz,list_e[e]['data']['propagationFire'][0],list_e[e]['data']['sizeFire'][0])
				end
				if list_e[e]['data']['type'][0] == 1 then
					addExplosion(xx,xy,xz,list_e[e]['data']['typeExplosion'][0])
				end

			end
			if list_e[e]['data']['endC'][0]+1 ~= 1 and list_e[e]['data']['endC'][0]+1 == i then
				if list_e[e]['data']['type'][0] == 0 then
					removeScriptFire(mp.explosions[e])
				end
			end
		end
		for a = 1,#list_au do
			if list_au[a]['data'].useTarget[0] then
				if list_au[a]['data'].startC[0]+1 == i then
					audio_player(a)
				end
				if list_au[a]['data']['endC'][0]+1 ~= 1 and list_au[a]['data']['endC'][0]+1 == i then
					setAudioStreamState(mp.audio[a],0)
					releaseAudioStream(mp.audio[a])
				end
			end
		end
		if list[i]['type'][0] == 0 then
			lockPlayerControl(false)
			wait(100)
			local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
			rad = list[i]['data']['radius'][0]
			--sph[i] = addSphere(xx,xy,xz,rad)
			printString(koder(cyr(ffi.string(list[i]['data']['text']))),list[i]['data']['textTime'][0] * 1000)
			local check = addBlipForCoord(xx,xy,xz)
			if list[i]['data']['colorBlip'][0] > 1 then
				changeBlipColour(check,list[i]['data']['colorBlip'][0]-2)
			else
				setBlipAsFriendly(check, 1)
			end
			while not locateCharAnyMeans3d(PLAYER_PED,xx,xy,xz,rad,rad,rad,true) do
				wait(0)
			end
			removeBlip(check)
			--removeSphere(sph[i])
		end
		if list[i]['type'][0] == 1 then
			lockPlayerControl(false)
			local check = addBlipForCar(mp.cars[list[i]['data']['car'][0]+1])
			if list[i]['data']['colorBlip'][0] > 1 then
				changeBlipColour(check,list[i]['data']['colorBlip'][0]-2)
			else
				setBlipAsFriendly(check, 1)
			end
			printString(koder(cyr(ffi.string(list[i]['data']['text']))),list[i]['data']['textTime'][0] * 1000)
			while not isCharInCar(PLAYER_PED, mp.cars[list[i]['data']['car'][0]+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['type'][0] == 2 then
			lockPlayerControl(false)
			printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['textTime'][0]*1000)
			if list[i]['data']['killGroup'][0] then
				local peds = {}
				local check = {}
				for a = 1,#mp.actors do
					if mp.actors[a] ~= nil and not isCharDead(mp.actors[a]) then
						if getPedType(mp.actors[a]) == getPedType(mp.actors[list[i]['data']['actor'][0]+1]) then
							peds[#peds+1] = mp.actors[a]
							check[#peds] = addBlipForChar(peds[#peds])
							if list[i]['data']['colorBlip'][0] > 1 then
								changeBlipColour(check[#peds],list[i]['data']['colorBlip'][0]-2)
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
				local check = addBlipForChar(mp.actors[list[i]['data']['actor'][0]+1])
				if list[i]['data']['colorBlip'][0] > 1 then
					changeBlipColour(check,list[i]['data']['colorBlip'][0]-2)
				else
					setBlipAsFriendly(check, 1)
				end
				setBlipAsFriendly(check, true)
				while not isCharDead(mp.actors[list[i]['data']['actor'][0]+1]) do
					wait(0)
				end
				removeBlip(check)
			end
		end
		if list[i]['type'][0] == 3 then
			if list[i]['targetType'][0] == 0 then
				displayRadar(false)
				displayHud(false)
				lockPlayerControl(true)
        		switchWidescreen(true)
				if i > 1 and not (list[i-1]['targetType'][0] == 0 and list[i-1]['type'][0] == 3) then
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
				if list[i]['data']['tied'][0] == 0 then
					if list[i]['data']['follow'][0] == 0 then
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local rxx,rxy,rxz = list[i]['data']['rotate'][0],list[i]['data']['rotate'][1],list[i]['data']['rotate'][2]
						local x1,y1,z1 = xx,xy,xz
						x1 = x1 + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
						y1 = y1 + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
						z1 = z1 + 2*math.cos(math.rad(rxx))
						if list[i]['data']['moveCam'][0] then
							setInterpolationParameters(0,list[i]['data']['time'][0]*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtPoint(x1, y1, z1, 1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtPoint(x1, y1, z1, 2)
						end
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						wait(list[i]['data']['time'][0]*1000*skip)
					elseif list[i]['data']['follow'][0] == 1 then
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['data']['followId'][0]+1])
						if list[i]['data']['moveCam'][0] then
							setInterpolationParameters(0,list[i]['data']['time'][0]*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtChar(mp.actors[list[i]['data']['followId'][0]+1],15,1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtChar(mp.actors[list[i]['data']['followId'][0]+1],15,2)
						end
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						wait(list[i]['data']['time'][0]*1000*skip)
					elseif list[i]['data']['follow'][0] == 2 then
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local x1,y1,z1 = getCharCoordinates(mp.cars[list[i]['data']['followId'][0]+1])
						if list[i]['data']['moveCam'][0] then
							setInterpolationParameters(0,list[i]['data']['time'][0]*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtCar(mp.cars[list[i]['data']['followId'][0]+1],15,1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAtCar(mp.cars[list[i]['data']['followId'][0]+1],15,2)
						end
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						wait(list[i]['data']['time'][0]*1000*skip)
					elseif list[i]['data']['follow'][0] == 3 then
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local x1,y1,z1 = getObjectCoordinates(mp.objects[list[i]['data']['followId'][0]+1])
						if list[i]['data']['moveCam'][0] then
							setInterpolationParameters(0,list[i]['data']['time'][0]*1000)
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAt(x1,y1,z1,1)
						else
							setFixedCameraPosition(xx, xy, xz)
							pointCameraAt(x1,y1,z1,2)
						end
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						wait(list[i]['data']['time'][0]*1000*skip)
					end
				elseif list[i]['data']['tied'][0] == 1 then
					if list[i]['data']['follow'][0] == 0 then
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local rxx,rxy,rxz = list[i]['data']['rotate'][0],list[i]['data']['rotate'][1],list[i]['data']['rotate'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCharCoordinates(mp.actors[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1)
							rx = rx + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
							ry = ry + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
							rz = rz + 2*math.cos(math.rad(rxx))
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['data']['follow'][0] == 1 then
						local x1,y1,z1 = getCharCoordinates(mp.actors[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCharCoordinates(mp.actors[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = getCharCoordinates(mp.actors[list[i]['data']['followId'][0]+1])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['data']['follow'][0] == 2 then
						local x1,y1,z1 = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = getCarCoordinates(mp.cars[list[i]['data']['followId'][0]+1])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['data']['follow'][0] == 3 then
						local x1,y1,z1 = getObjectCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getObjectCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = getObjectCoordinates(mp.cars[list[i]['data']['followId'][0]+1])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					end
				elseif list[i]['data']['tied'][0] == 2 then
					if list[i]['data']['follow'][0] == 0 then
						local x1,y1,z1 = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local rxx,rxy,rxz = list[i]['data']['rotate'][0],list[i]['data']['rotate'][1],list[i]['data']['rotate'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1)
							rx = rx + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
							ry = ry + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
							rz = rz + 2*math.cos(math.rad(rxx))
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['data']['follow'][0] == 1 then
						local x1,y1,z1 = getCarCoordinates(mp.actors[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = getCharCoordinates(mp.actors[list[i]['data']['followId'][0]+1])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['data']['follow'][0] == 2 then
						local x1,y1,z1 = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getCarCoordinates(mp.cars[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = getCarCoordinates(mp.cars[list[i]['data']['followId'][0]+1])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					elseif list[i]['data']['follow'][0] == 3 then
						local x1,y1,z1 = getObjectCoordinates(mp.objects[list[i]['data']['tiedId'][0]+1])
						local xx,xy,xz = list[i]['data']['pos'][0],list[i]['data']['pos'][1],list[i]['data']['pos'][2]
						local timer = 0.0
						local last_time = os.clock()
						printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
						while timer < list[i]['data']['time'][0]*skip do
							wait(0)
							local cur_x,cur_y,cur_z = getObjectCoordinates(mp.objects[list[i]['data']['tiedId'][0]+1])
							local rx,ry,rz = getObjectCoordinates(mp.objects[list[i]['data']['followId'][0]+1])
							setFixedCameraPosition(cur_x+(xx-x1),cur_y+(xy-y1),cur_z+(xz-z1))
							pointCameraAtPoint(rx,ry,rz,2)
							timer = timer + (os.clock()-last_time)
							last_time = os.clock()
						end
					end

				end
				if i < #list and not (list[i+1]['targetType'][0] == 0 and list[i+1]['type'][0] == 3) then
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
			if list[i]['targetType'][0] == 1 then
				setPlayerControl(PLAYER_HANDLE, false)
				for t = 0,list[i]['data']['time'][0]-1 do
					printStyledString(tostring(list[i]['data']['time'][0] - t), 1000, 4)
					wait(1000)
				end
				printStyledString('Go!', 1000, 4)
				wait(1000)
				setPlayerControl(PLAYER_HANDLE, true)
			end
			if list[i]['targetType'][0] == 2 then
				printString(koder(cyr(ffi.string(list[i]['data']['text']))), list[i]['data']['time'][0]*1000)
				wait(list[i]['data']['time'][0]*1000)
			end
			if list[i]['targetType'][0] == 3 then
				forceWeatherNow(list[i]['data']['weather'][0])
			end
			if list[i]['targetType'][0] == 4 then
				setTimeOfDay(list[i]['data']['time'][0], list[i]['data']['time'][1])
			end
			if list[i]['targetType'][0] == 5 then
				setPedDensityMultiplier(1 * list[i]['data']['peds'][0])
				setCarDensityMultiplier(1 * list[i]['data']['cars'][0])
			end
		end
		if list[i]['type'][0] == 4 then
			lockPlayerControl(false)
			local check = addBlipForObject(mp.objects[list[i]['data']['object'][0]+1])
			if list[i]['data']['colorBlip'][0] > 1 then
			  changeBlipColour(check,list[i]['data']['colorBlip'][0]-2)
			else
			  setBlipAsFriendly(check, 0)
			end
			printString(koder(cyr(ffi.string(list[i]['data']['text']))),list[i]['data']['textTime'][0] * 1000)
			if list[i]['data']['type'] == 0 then
				while not isCharTouchingObject(PLAYER_PED, mp.objects[list[i]['data']['object'][0]+1]) do
					wait(0)
				end
			end
			if list[i]['data']['type'] == 1 then
				while not hasObjectBeenDamaged(mp.objects[list[i]['data']['object'][0]+1]) do
					wait(0)
				end
			end
			if list[i]['data']['type'] == 2 then
				while not hasObjectBeenPhotographed(mp.objects[list[i]['data']['object'][0]+1]) do
					wait(0)
				end
			end
			if list[i]['data']['type'] == 3 then
				while not hasObjectBeenDamagedByWeapon(mp.objects[list[i]['data']['object'][0]+1],list[i]['data']['Weap']) do
					wait(0)
				end
			end
			removeBlip(check)
		end
		if list[i]['type'][0] == 5 then
			lockPlayerControl(false)
			local check = addBlipForPickup(mp.pickups[list[i]['data']['pickup'][0]+1])
			if list[i]['data']['colorBlip'][0] > 1 then
				changeBlipColour(check,list[i]['data']['colorBlip'][0]-2)
			else
				setBlipAsFriendly(check, 1)
			end
			printString(koder(cyr(ffi.string(list[i]['data']['text']))),list[i]['data']['textTime'][0] * 1000)
			while not hasPickupBeenCollected(mp.pickups[list[i]['data']['pickup'][0]+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['type'][0] == 6 then
			if list[i]['targetType'][0] == 0 then
				lockPlayerControl(false)
				if isCharInAnyCar(PLAYER_PED) then
					taskLeaveAnyCar(PLAYER_PED)
					while isCharInAnyCar(PLAYER_PED) do
						wait(0)
					end
				end
				setCharInterior(PLAYER_PED, list[i]['data']['interiorId'][0])
				setInteriorVisible(list[i]['data']['interiorId'][0])
				local modell
				if list[i]['data']['modelType'][0] == 0 then
					modell = list[i]['data']['modelId'][0]
					requestModel(modell)
					while not hasModelLoaded(modell) do
						wait(0)
					end
				else
					local modell_n = ID_Spec_Actors[list[i]['data']['modelId'][0]+1]
					loadSpecialCharacter(modell_n,10)
					while not hasSpecialCharacterLoaded(10) do
						wait(0)
					end
					modell = 290 + 10-1
				end
				setPlayerModel(PLAYER_HANDLE, modell)
				setCharCoordinates(PLAYER_PED, list[i]['data']['pos'][0], list[i]['data']['pos'][1], list[i]['data']['pos'][2])
				requestModel(getWeapontypeModel(list[i]['data']['weapon'][0]))
				while not hasModelLoaded(getWeapontypeModel(list[i]['data']['weapon'][0])) do
					wait(0)

				end
				giveWeaponToChar(PLAYER_PED, ID_Weapons[list[i]['data']['weapon'][0]], list[i]['data']['Weap_ammo'])
				setCharHeading(PLAYER_PED, list[i]['data']['angle'][0])
			end
			if list[i]['targetType'][0] == 1 then
				lockPlayerControl(true)
				if not hasAnimationLoaded(Anims['Anim_name'][list[i]['data']['Pack_anim']]) then
					requestAnimation(Anims['Anim_name'][list[i]['data']['Pack_anim']])
					while not hasAnimationLoaded(Anims['Anim_name'][list[i]['data']['Pack_anim']]) do
						wait(0)
					end
				end
				local animm = Anims['Anim_list'][list[i]['data']['Pack_anim']]
				animm = animm[list[i]['data']['Anim']]
				print(animm)
				taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][list[i]['data']['Pack_anim']], 1.0, list[i]['data']['Loop'], false, false, false, -1)
				wait(1)
			end
			if list[i]['targetType'][0] == 2 then
				lockPlayerControl(false)
				if list[i]['data']['carPlace'][0] == 0 then
					taskWarpCharIntoCarAsDriver(PLAYER_PED, mp.cars[list[i]['data']['car'][0]+1])
				elseif list[i]['data']['carPlace'][0] > 0 then
					taskWarpCharIntoCarAsPassenger(PLAYER_PED, mp.cars[list[i]['data']['car'][0]+1],list[i]['data']['carPlace'][0]-1)
				end
			end
			if list[i]['targetType'][0] == 3 then
				alterWantedLevel(PLAYER_HANDLE, list[i]['data']['levelWanted'][0])
				setMaxWantedLevel(6)
			end
			if list[i]['targetType'][0] == 4 then
				removeAllCharWeapons(PLAYER_PED)
			end
			if list[i]['targetType'][0] == 5 then
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
				for d = 1,#list[i]['data']['dialogs'] do
					printString(koder(cyr(ffi.string(list[i]['data']['dialogs'][d]['text']))), list[i]['data']['dialogs'][d]['textTime'][0]*1000)
					wait(list[i]['data']['dialogs'][d]['textTime'][0]*1000*skip)
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
			if list[i]['data']['Target_type'] == 7 then
				givePlayerMoney(PLAYER_HANDLE, list[i]['data']['money'][0])
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
	setCharCoordinates(PLAYER_PED, vr.missData['player']['pos'][0], vr.missData['player']['pos'][1], vr.missData['player']['pos'][2])
	setCharInterior(PLAYER_PED, vr.missData['player']['interiorId'][0])
	setInteriorVisible(vr.missData['player']['interiorId'][0])
	lockPlayerControl(false)
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
	for v,h in pairs(mp.pickups) do
		removePickup(mp.pickups[v])
	end
	for v,h in pairs(mp.particles) do
		killFxSystem(mp.particles[v][1])
		if mp.particles[v][2] then deleteObject(mp.particles[v][2]) end
	end
	for v,h in pairs(mp.explosions) do
		killFxSystem(mp.explosions[v])
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
	for j = 1,#vr.list_pickups do
		update_pickup(j)
	end
	for j = 1,#vr.list_particles do
		update_particle(j)
	end
	for j = 1,#vr.list_explosions do
		update_explosion(j)
	end
	for j = 1,#vr.list_audios do
		update_audio(j)
	end
	mp.mission_work = false
end

return mp
