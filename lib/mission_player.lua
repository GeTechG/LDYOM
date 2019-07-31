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
local thread_miss
local skip = 1
curr_target = 0

function mp.start_mission(listt,lista,listc,listo,listp,listpa,liste,missd)
	misflag = mp.flagmis()
	mp.miss = mp.start()
	imgui.Process = false
	thread_miss = lua_thread.create(mp.main_mission,listt,lista,listc,listo,listp,listpa,liste,missd)
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
	actr['Anims']['curr_anim'] = curr_anim
	while curr_anim <= #actr['Anims'] do
		wait(0)
		if actr['Anims'][curr_anim]['Type'][0] == 0 then
			if actr['Anims'][curr_anim]['Condition'][0] == 0 then
				if not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1]) then
					requestAnimation(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1])
					while not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1]) do
						wait(0)
					end
				end
				local animm = Anims['Anim_list'][actr['Anims'][curr_anim]['Pack_anim'][0]+1]
				animm = animm[actr['Anims'][curr_anim]['Anim'][0]+1]
				if not actr['Anims'][curr_anim]['Unbreakable'][0] then
					taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1], 1.0, actr['Anims'][curr_anim]['Loop'][0], false, false, false, -1)
				else
					taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1], 1.0, actr['Anims'][curr_anim]['Loop'][0], false, false, false, -1,false,false)
				end
				wait(actr['Anims'][curr_anim]['Time'][0] * 1000.0)
				curr_anim = curr_anim + 1
			elseif actr['Anims'][curr_anim]['Condition'][0] == 1 then
				if curr_target == actr['Anims'][curr_anim]['Target'][0]+1 then
					if not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1]) then
						requestAnimation(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1])
						while not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1]) do
							wait(0)
						end
					end
					local animm = Anims['Anim_list'][actr['Anims'][curr_anim]['Pack_anim'][0]+1]
					animm = animm[actr['Anims'][curr_anim]['Anim'][0]+1]
					if not actr['Anims'][curr_anim]['Unbreakable'][0] then
						taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1], 1.0, actr['Anims'][curr_anim]['Loop'][0], false, false, false, -1)
					else
						taskPlayAnimWithFlags(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'][0]+1], 1.0, actr['Anims'][curr_anim]['Loop'][0], false, false, false, -1,false,false)
					end
					wait(actr['Anims'][curr_anim]['Time'][0] * 1000.0)
					curr_anim = curr_anim + 1
				end
			end
		elseif actr['Anims'][curr_anim]['Type'][0] == 1 then
			if actr['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == actr['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			flushPatrolRoute()
			local c = 0
			local type_walk = 4;
			if actr['Anims'][curr_anim]['Type_move'][0] == 2 then type_walk = 6
			elseif actr['Anims'][curr_anim]['Type_move'][0] == 3 then type_walk = 7 end
			taskToggleDuck(ped, ternar(actr['Anims'][curr_anim]['Type_move'][0] == 0,true,false))
			for i = 1,#actr['Anims'][curr_anim]['Path'] do
				local x1,y1,z1 = actr['Anims'][curr_anim]['Path'][i][1],actr['Anims'][curr_anim]['Path'][i][2],actr['Anims'][curr_anim]['Path'][i][3]
				local px,py,pz = getCharCoordinates(ped)
				if i % 7 == 0 then
					taskFollowPatrolRoute(ped,type_walk,actr['Anims'][curr_anim]['Type_route'][0])
					while getDistanceBetweenCoords3d(actr['Anims'][curr_anim]['Path'][i-1][1],actr['Anims'][curr_anim]['Path'][i-1][2],actr['Anims'][curr_anim]['Path'][i-1][3],px,py,pz) > 0.1 do
						wait(0)
						px,py,pz = getCharCoordinates(ped)
					end
					flushPatrolRoute()
				end
				extendPatrolRoute(x1,y1,z1,'NONE','NONE')
				taskFollowPatrolRoute(ped,type_walk,actr['Anims'][curr_anim]['Type_route'][0])
			end
			
			if actr['Anims'][curr_anim]['wait_end'][0] then
				while getDistanceBetweenCoords3d(actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][1],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][2],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][3],px,py,pz) > 0.1 do
					wait(0)
					px,py,pz = getCharCoordinates(ped)
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'][0] == 2 then
			if actr['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == actr['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			flushRoute()
			local c = 0
			local px,py,pz = getCarCoordinates(mp.cars[actr['Anims'][curr_anim]['Car'][0]+1])
			for i = 1,#actr['Anims'][curr_anim]['Path'] do
			  local x1,y1,z1 = actr['Anims'][curr_anim]['Path'][i][1],actr['Anims'][curr_anim]['Path'][i][2],actr['Anims'][curr_anim]['Path'][i][3]
			  extendRoute(x1,y1,z1)
			  c = c + 1
			  px,py,pz = getCarCoordinates(mp.cars[actr['Anims'][curr_anim]['Car'][0]+1])
			  if c == 7 then
				taskDrivePointRoute(ped,mp.cars[actr['Anims'][curr_anim]['Car'][0]+1],actr['Anims'][curr_anim]['Speed'][0])
				while getDistanceBetweenCoords3d(actr['Anims'][curr_anim]['Path'][i][1],actr['Anims'][curr_anim]['Path'][i][2],actr['Anims'][curr_anim]['Path'][i][3],px,py,pz) > 1 do
				  wait(0)
				  px,py,pz = getCarCoordinates(mp.cars[actr['Anims'][curr_anim]['Car'][0]+1])
				end
				flushRoute()
				c = 0
			  end
			end
			taskDrivePointRoute(ped,mp.cars[actr['Anims'][curr_anim]['Car'][0]+1],actr['Anims'][curr_anim]['Speed'][0])
			if actr['Anims'][curr_anim]['wait_end'][0] then
				while getDistanceBetweenCoords3d(actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][1],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][2],actr['Anims'][curr_anim]['Path'][#actr['Anims'][curr_anim]['Path']][3],px,py,pz) > 1 do
					wait(0)
					px,py,pz = getCarCoordinates(mp.cars[actr['Anims'][curr_anim]['Car'][0]+1])
				end
			end
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'][0] == 3 then
			if actr['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == actr['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			if actr['Anims'][curr_anim]['wait_end'][0] then
				if isCharInAnyCar(ped) then
					taskLeaveAnyCar(ped)
					while isCharInAnyCar(ped) do
						wait(0)
					end
				end
			end
			curr_anim = curr_anim + 1
		
		elseif actr['Anims'][curr_anim]['Type'][0] == 4 then
			if actr['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == actr['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			taskCarMission(ped,mp.cars[actr['Anims'][curr_anim]['Car_a'][0]+1],mp.cars[actr['Anims'][curr_anim]['Car_t'][0]+1],actr['Anims'][curr_anim]['Vehicle_mission'][0]-1,actr['Anims'][curr_anim]['Speed'][0],actr['Anims'][curr_anim]['trafficFlag'][0])
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Type'][0] == 5 then
			if actr['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == actr['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			if actr['Anims'][curr_anim]['place_in_car'][0] == 0 then
				setNextDesiredMoveState(actr['Anims'][curr_anim]['speed_walk'][0]+4)
				taskEnterCarAsDriver(ped,mp.cars[actr['Anims'][curr_anim]['Car'][0]+1],-1)
			else
				setNextDesiredMoveState(actr['Anims'][curr_anim]['speed_walk'][0]+4)
				taskEnterCarAsPassenger(ped,mp.cars[actr['Anims'][curr_anim]['Car'][0]+1],-1,actr['Anims'][curr_anim]['place_in_car'][0]-1)
			end
			if actr['Anims'][curr_anim]['wait_end'][0] then
				while not isCharInAnyCar(ped) do
					wait(0)
				end
			end
			curr_anim = curr_anim + 1
		end
		actr['Anims']['curr_anim'] = curr_anim
	end
end

function mp.play_car_anims(car,vehic)
	local curr_anim = 1
	vehic['Anims']['curr_anim'] = curr_anim
	while curr_anim <= #vehic['Anims'] do
		wait(0)
		if vehic['Anims'][curr_anim]['Type'][0] == 0 then
			if vehic['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == vehic['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			closeAllCarDoors(car)
			for g = 1,6 do
				print(g)
				if vehic['Anims'][curr_anim]['Doors'][g][0] then
					openCarDoor(car,g-1)
				end
			end
			curr_anim = curr_anim + 1
		elseif vehic['Anims'][curr_anim]['Type'][0] == 1 then
			if vehic['Anims'][curr_anim]['Condition'][0] == 1 then
				while not (curr_target == vehic['Anims'][curr_anim]['Target'][0]+1) do
					wait(0)
				end
			end
			lockCarDoors(car,vehic['Anims'][curr_anim]['Door_lock'][0]+1)
			curr_anim = curr_anim + 1
		end
		vehic['Anims']['curr_anim'] = curr_anim
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
	local rotate_obj = {}
	rotate_obj.x,rotate_obj.y,rotate_obj.z = obj_data['Rotates'][0],obj_data['Rotates'][1],obj_data['Rotates'][2]
	while curr_anim <= #obj_data['Anims'] do
		wait(0)
		local step_pos = {}
		local startpos = {}
		local curpos = {}
		local step_angle = {}
		startpos.x,startpos.y,startpos.z = select(2,getObjectCoordinates(obj)),select(3,getObjectCoordinates(obj)),select(4,getObjectCoordinates(obj))

		step_pos.x = (obj_data['Anims'][curr_anim]['Pos'][0] - startpos.x) / (obj_data['Anims'][curr_anim]['Time'][0] * 1000)
		step_pos.y = (obj_data['Anims'][curr_anim]['Pos'][1] - startpos.y) / (obj_data['Anims'][curr_anim]['Time'][0] * 1000)
		step_pos.z = (obj_data['Anims'][curr_anim]['Pos'][2] - startpos.z) / (obj_data['Anims'][curr_anim]['Time'][0] * 1000)

		step_angle.x = (obj_data['Anims'][curr_anim]['Rotates'][0] - rotate_obj.x) / (obj_data['Anims'][curr_anim]['Time'][0] * 1000)
		step_angle.y = (obj_data['Anims'][curr_anim]['Rotates'][1] - rotate_obj.y) / (obj_data['Anims'][curr_anim]['Time'][0] * 1000)
		step_angle.z = (obj_data['Anims'][curr_anim]['Rotates'][2] - rotate_obj.z) / (obj_data['Anims'][curr_anim]['Time'][0] * 1000)
		if obj_data['Anims'][curr_anim]['Condition'][0] == 1 then
			while curr_target ~= obj_data['Anims'][curr_anim]['Target'][0]+1 do
				wait(0)
			end
		end
		local time = 0
		while time <= obj_data['Anims'][curr_anim]['Time'][0] * 1000 do
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
	end
end

function mp.main_mission(list,list_a,list_c,list_o,list_p,list_pa,list_e,miss_data)
	printStyledString(koder(u8:decode(miss_data['Name'])), 2000, 2)
	setTimeOfDay(miss_data['Time'][1], miss_data['Time'][2])
	forceWeatherNow(miss_data['Weather'][0])
	setLaRiots(miss_data['Riot'][0])
	setCharCoordinates(PLAYER_PED, miss_data['Player']['Pos'][1], miss_data['Player']['Pos'][2], miss_data['Player']['Pos'][3])
	setCharHeading(PLAYER_PED, miss_data['Player']['Angle'])
	setMaxWantedLevel(6)
	setCharInterior(PLAYER_PED, miss_data['Player']['Interior_id'])
	setInteriorVisible(miss_data['Player']['Interior_id'])
	if not hasModelLoaded(miss_data['Player']['ModelId']) then
		requestModel(miss_data['Player']['ModelId'])
		while not hasModelLoaded(miss_data['Player']['ModelId']) do
			wait(0)
		end
	end
	setPlayerModel(PLAYER_HANDLE, miss_data['Player']['ModelId'])
	giveWeaponToChar(PLAYER_PED, miss_data['Player']['Weapon'], miss_data['Player']['Weap_ammo'])
	for i = 1,#list do
		curr_target = i
		for a = 1,#list_a do
			if list_a[a]['Actor_Data']['StartC'][0] + 1 == i then
				local md = list_a[a]['Actor_Data']['ModelId'][0]
				local xx,xy,xz = list_a[a]['Actor_Data']['Pos'][0], list_a[a]['Actor_Data']['Pos'][1], list_a[a]['Actor_Data']['Pos'][2]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.actors[a] = createChar(4, md, xx, xy, xz)
				setCharHealth(mp.actors[a],list_a[a]['Actor_Data']['Health'][0])
				setCharHeading(mp.actors[a], list_a[a]['Actor_Data']['Angle'][0])
				wait(0)
				if #list_a[a]['Actor_Data']['Anims'] > 0 then
					mp.thread[#mp.thread+1] = lua_thread.create(mp.play_char_anims,mp.actors[a], list_a[a]['Actor_Data'])
				end
				if list_a[a]['Actor_Data']['Should_not_die'][0] == true then
					mp.thread[#mp.thread+1] = lua_thread.create(char_is_not_dead,mp.actors[a])
				end
			end
			if list_a[a]['Actor_Data']['EndC'][0] ~= 0 and list_a[a]['Actor_Data']['EndC'][0] + 1 == i then
				deleteChar(mp.actors[a])
			end
		end
		for c = 1,#list_c do
			if list_c[c]['Car_Data']['StartC'][0] + 1 == i then
				local md = list_c[c]['Car_Data']['ModelId'][0]
				local xx,xy,xz = list_c[c]['Car_Data']['Pos'][0], list_c[c]['Car_Data']['Pos'][1], list_c[c]['Car_Data']['Pos'][2]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.cars[c] = createCar(md, xx, xy, xz)
				setCarHealth(mp.cars[c],list_c[c]['Car_Data']['Health'][0])
				setCarHeading(mp.cars[c], list_c[c]['Car_Data']['Angle'][0])
				setCarProofs(mp.cars[c],list_c[c]['Car_Data']['Bulletproof'][0],list_c[c]['Car_Data']['Fireproof'][0],list_c[c]['Car_Data']['Explosionproof'][0],list_c[c]['Car_Data']['Collisionproof'][0],list_c[c]['Car_Data']['Meleeproof'][0])
				setCanBurstCarTires(mp.cars[c], not list_c[c]['Car_Data']['Tires_vulnerability'][0])
				changeCarColour(mp.cars[c], list_c[c]['Car_Data']['Color_primary'][0], list_c[c]['Car_Data']['Color_secondary'][0])
				if list_c[c]['Car_Data']['Should_not_die'][0] == true then
					mp.thread[#mp.thread+1] = lua_thread.create(car_is_not_dead,mp.cars[c])
				end
				if #list_c[c]['Car_Data']['Anims'] > 0 then
					mp.thread[#mp.thread+1] = lua_thread.create(mp.play_car_anims,mp.cars[c], list_c[c]['Car_Data'])
				end
			end
			if list_c[c]['Car_Data']['EndC'][0] ~= 0 and list_c[c]['Car_Data']['EndC'][0] + 1 == i then
				deleteCar(mp.cars[c])
			end
		end
		for o = 1,#list_o do
			if list_o[o]['Object_Data']['StartC'][0] + 1 == i then
				local md = list_o[o]['Object_Data']['ModelId'][0]
				local xx,xy,xz = list_o[o]['Object_Data']['Pos'][0], list_o[o]['Object_Data']['Pos'][1], list_o[o]['Object_Data']['Pos'][2]
				local rxx,rxy,rxz = list_o[o]['Object_Data']['Rotates'][0], list_o[o]['Object_Data']['Rotates'][1], list_o[o]['Object_Data']['Rotates'][2]
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
			if list_o[o]['Object_Data']['EndC'][0] ~= 0 and list_o[o]['Object_Data']['EndC'][0] + 1 == i then
				deleteObject(mp.objects[o])
			end
		end
		for p = 1,#list_pa do
			if list_pa[p]['Particle_Data']['StartC'][0] + 1 == i then
				local md = Particle_name[list_pa[p]['Particle_Data']['ModelId'][0]+1]
				local xx,xy,xz = list_pa[p]['Particle_Data']['Pos'][0], list_pa[p]['Particle_Data']['Pos'][1], list_pa[p]['Particle_Data']['Pos'][2]
				local rxx,rxy,rxz = list_pa[p]['Particle_Data']['Rotates'][0], list_pa[p]['Particle_Data']['Rotates'][1], list_pa[p]['Particle_Data']['Rotates'][2]

				mp.particle[p] = createFxSystem(md, xx, xy, xz, 1)
				playFxSystem(mp.particle[p])
			end
			if list_pa[p]['Particle_Data']['EndC'][0] ~= 0 and list_pa[p]['Particle_Data']['EndC'][0] + 1 == i then
				killFxSystemNow(mp.particle[p])
			end
		end
		for p = 1,#list_p do
			if list_p[p]['Pickup_Data']['StartC'][0] + 1 == i then
				local xx,xy,xz = list_p[p]['Pickup_Data']['Pos'][0], list_p[p]['Pickup_Data']['Pos'][1], list_p[p]['Pickup_Data']['Pos'][2]
				local spawn_t = 3
				if list_p[p]['Pickup_Data']['spawn_type'][0] == 1 then
					spawn_t = 2
				elseif list_p[p]['Pickup_Data']['spawn_type'][0] == 2 then
					spawn_t = 15
				end

				if list_p[p]['Pickup_Data']['Type_pickup'][0] == 0 then
					local md = getWeapontypeModel(list_p[p]['Pickup_Data']['Weapon'][0])
					if not isModelAvailable(md) then
						requestModel(md)
						while not isModelAvailable(md) do
							wait(0)
						end
					end

					mp.pickup[p] = createPickupWithAmmo(md, spawn_t, list_p[p]['Pickup_Data']['Ammo'][0], xx, xy, xz)
				end
				if list_p[p]['Pickup_Data']['Type_pickup'][0] >= 1 then
					local md = 1240
					if list_p[p]['Pickup_Data']['Type_pickup'][0] == 2 then
						md = 1242
					elseif list_p[p]['Pickup_Data']['Type_pickup'][0] == 3 then
						md = 1247
					elseif list_p[p]['Pickup_Data']['Type_pickup'][0] == 4 then
						md = 1210
					elseif list_p[p]['Pickup_Data']['Type_pickup'][0] == 5 then
						md = list_p[p]['Pickup_Data']['ModelId'][0]
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
			if list_p[p]['Pickup_Data']['EndC'][0] ~= 0 and list_p[p]['Pickup_Data']['EndC'][0] + 1 == i then
				removePickup(mp.pickup[p])
			end
		end
		for e = 1,#list_e do
			if list_e[e]['Explosion_Data']['StartC'][0] + 1 == i then
				local xx,xy,xz = list_e[e]['Explosion_Data']['Pos'][0], list_e[e]['Explosion_Data']['Pos'][1], list_e[e]['Explosion_Data']['Pos'][2]
				
				if list_e[e]['Explosion_Data']['Type'][0] == 0 then
					mp.explosion[e] = startScriptFire(xx,xy,xz,list_e[e]['Explosion_Data']['Propagation_fire'][0],list_e[e]['Explosion_Data']['Size_fire'][0])
				end
				if list_e[e]['Explosion_Data']['Type'][0] == 1 then
					addExplosion(xx,xy,xz,list_e[e]['Explosion_Data']['Type_explosion'][0])
				end

			end
			if list_e[e]['Explosion_Data']['EndC'][0] ~= 0 and list_e[e]['Explosion_Data']['EndC'][0] + 1 == i then
				if list_e[e]['Explosion_Data']['Type'][0] == 0 then
					removeScriptFire(mp.explosion[e])
				end
			end
		end
		if list[i]['Type'][0] == 0 then
			lockPlayerControl(false)
			wait(100)
			local xx,xy,xz = list[i]['Target_Data']['Pos'][0],list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2]
			rad = list[i]['Target_Data']['Radius'][0]
			--sph[i] = addSphere(xx,xy,xz,rad)
			printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Text']))),list[i]['Target_Data']['Text_time'][0] * 1000)
			local check = addBlipForCoord(xx,xy,xz)
			if list[i]['Target_Data']['Color_blip'][0] > 0 then
				changeBlipColour(check,list[i]['Target_Data']['Color_blip'][0]-1)
			else
				setBlipAsFriendly(check, 1)
			end
			while not locateCharAnyMeans3d(PLAYER_PED,xx,xy,xz,rad,rad,rad,true) do
				wait(0)
			end
			removeBlip(check)
			--removeSphere(sph[i])
		end
		if list[i]['Type'][0] == 1 then
			lockPlayerControl(false)
			local check = addBlipForCar(mp.cars[list[i]['Target_Data']['Target_car_id'][0]+1])
			if list[i]['Target_Data']['Color_blip'][0] > 0 then
				changeBlipColour(check,list[i]['Target_Data']['Color_blip'][0]-1)
			else
				setBlipAsFriendly(check, 1)
			end
			printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Text']))), 2000)
			while not isCharInCar(PLAYER_PED, mp.cars[list[i]['Target_Data']['Target_car_id'][0]+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'][0] == 2 then
			lockPlayerControl(false)
			local check = addBlipForChar(mp.actors[list[i]['Target_Data']['Target_actor_id'][0]+1])
			if list[i]['Target_Data']['Color_blip'][0] > 0 then
			  changeBlipColour(check,list[i]['Target_Data']['Color_blip'][0]-1)
			else
			  setBlipAsFriendly(check, 1)
			end
			setBlipAsFriendly(check, true)
			printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Text']))), 2000)
			while not isCharDead(mp.actors[list[i]['Target_Data']['Target_actor_id'][0]+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'][0] == 3 then
			if list[i]['Target_Data']['Target_type'][0] == 0 then
				displayRadar(false)
				displayHud(false)
				lockPlayerControl(true)
        		switchWidescreen(true)
				if i > 1 and not (list[i-1]['Target_Data']['Target_type'][0] == 0 and list[i-1]['Type'][0] == 3) then
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
				local xx,xy,xz = list[i]['Target_Data']['Pos'][0],list[i]['Target_Data']['Pos'][1],list[i]['Target_Data']['Pos'][2]
				local rxx,rxy,rxz = list[i]['Target_Data']['Rotates'][0],list[i]['Target_Data']['Rotates'][1],list[i]['Target_Data']['Rotates'][2]
				local x1,y1,z1 = xx,xy,xz
				x1 = x1 + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
				y1 = y1 + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
				z1 = z1 + 2*math.cos(math.rad(rxx))
				if list[i]['Target_Data']['Smooth'][0] then
					setInterpolationParameters(0,list[i]['Target_Data']['Text_time'][0]*1000)
					setFixedCameraPosition(xx, xy, xz)
					pointCameraAtPoint(x1, y1, z1, 1)
				else
					setFixedCameraPosition(xx, xy, xz)
					pointCameraAtPoint(x1, y1, z1, 2)
				end

				printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Text']))), list[i]['Target_Data']['Text_time'][0]*1000)
       			wait(list[i]['Target_Data']['Text_time'][0]*1000*skip)
				if i < #list and not (list[i+1]['Target_Data']['Target_type'][0] == 0 and list[i+1]['Type'][0] == 3) then
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
			if list[i]['Target_Data']['Target_type'][0] == 1 then
				setPlayerControl(PLAYER_HANDLE, false)
				for t = 0,list[i]['Target_Data']['Time'][0]-1 do
					printStyledString(tostring(list[i]['Target_Data']['Time'][0] - t), 1000, 4)
					wait(1000)
				end
				printStyledString('Go!', 1000, 4)
				wait(1000)
				setPlayerControl(PLAYER_HANDLE, true)
			end
			if list[i]['Target_Data']['Target_type'][0] == 2 then
				printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Text']))), list[i]['Target_Data']['Time'][0]*1000)
				wait(list[i]['Target_Data']['Time'][0]*1000)
			end
			if list[i]['Target_Data']['Target_type'][0] == 3 then
				forceWeatherNow(list[i]['Target_Data']['Weather'][0])
			end
			if list[i]['Target_Data']['Target_type'][0] == 4 then
				setTimeOfDay(list[i]['Target_Data']['Clock_time'][1], list[i]['Target_Data']['Clock_time'][2])
			end
			if list[i]['Target_Data']['Target_type'][0] == 5 then
				setPedDensityMultiplier(1 * list[i]['Target_Data']['Traffic'][1][0])
				setCarDensityMultiplier(1 * list[i]['Target_Data']['Traffic'][2][0])
			end
		end
		if list[i]['Type'][0] == 4 then
			lockPlayerControl(false)
			local check = addBlipForObject(mp.objects[list[i]['Target_Data']['Target_object_id'][0]+1])
			if list[i]['Target_Data']['Color_blip'][0] > 0 then
			  changeBlipColour(check,list[i]['Target_Data']['Color_blip'][0]-1)
			else
			  setBlipAsFriendly(check, 0)
				print('12')
			end

			if list[i]['Target_Data']['Target_type'][0] == 0 then
				while not isCharTouchingObject(PLAYER_PED, mp.objects[list[i]['Target_Data']['Target_object_id'][0]+1]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'][0] == 1 then
				while not hasObjectBeenDamaged(mp.objects[list[i]['Target_Data']['Target_object_id'][0]+1]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'][0] == 2 then
				while not hasObjectBeenPhotographed(mp.objects[list[i]['Target_Data']['Target_object_id'][0]+1]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'][0] == 3 then
				while not hasObjectBeenDamagedByWeapon(mp.objects[list[i]['Target_Data']['Target_object_id'][0]+1],list[i]['Target_Data']['Weap'][0]) do
					wait(0)
				end
			end
			removeBlip(check)
		end
		if list[i]['Type'][0] == 5 then
			lockPlayerControl(false)
			local check = addBlipForPickup(mp.pickup[list[i]['Target_Data']['Target_pickup_id'][0]+1])
			if list[i]['Target_Data']['Color_blip'][0] > 0 then
				changeBlipColour(check,list[i]['Target_Data']['Color_blip'][0]-1)
			else
				setBlipAsFriendly(check, 1)
			end
			printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Text']))), 2000)
			while not hasPickupBeenCollected(mp.pickup[list[i]['Target_Data']['Target_pickup_id'][0]+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'][0] == 6 then
			if list[i]['Target_Data']['Target_type'][0] == 0 then
				lockPlayerControl(false)
				if isCharInAnyCar(PLAYER_PED) then
					taskLeaveAnyCar(PLAYER_PED)
					while isCharInAnyCar(PLAYER_PED) do
						wait(0)
					end
				end
				setCharInterior(PLAYER_PED, list[i]['Target_Data']['Interior_id'])
				setInteriorVisible(list[i]['Target_Data']['Interior_id'])
				if not hasModelLoaded(list[i]['Target_Data']['ModelID'][0]) then
					requestModel(list[i]['Target_Data']['ModelID'][0])
					while not hasModelLoaded(list[i]['Target_Data']['ModelID'][0]) do
						wait(0)

					end
				end
				setPlayerModel(PLAYER_HANDLE, list[i]['Target_Data']['ModelID'][0])
				setCharCoordinates(PLAYER_PED, list[i]['Target_Data']['Pos'][0], list[i]['Target_Data']['Pos'][1], list[i]['Target_Data']['Pos'][2])
				requestModel(getWeapontypeModel(list[i]['Target_Data']['Weapon'][0]))
				while not hasModelLoaded(getWeapontypeModel(list[i]['Target_Data']['Weapon'][0])) do
					wait(0)

				end
				giveWeaponToChar(PLAYER_PED, list[i]['Target_Data']['Weapon'][0], list[i]['Target_Data']['Weap_ammo'][0])
				setCharHeading(PLAYER_PED, list[i]['Target_Data']['Angle'][0])
			end
			if list[i]['Target_Data']['Target_type'][0] == 1 then
				lockPlayerControl(true)
				if not hasAnimationLoaded(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'][0]+1]) then
					requestAnimation(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'][0]+1])
					while not hasAnimationLoaded(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'][0]+1]) do
						wait(0)
					end
				end
				local animm = Anims['Anim_list'][list[i]['Target_Data']['Pack_anim'][0]+1]
				animm = animm[list[i]['Target_Data']['Anim'][0]+1]
				print(animm)
				taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'][0]+1], 1.0, list[i]['Target_Data']['Loop'][0], false, false, false, -1)
				wait(1)
			end
			if list[i]['Target_Data']['Target_type'][0] == 2 then
				lockPlayerControl(false)
				if list[i]['Target_Data']['Car_place'][0] == 0 then
					taskWarpCharIntoCarAsDriver(PLAYER_PED, mp.cars[list[i]['Target_Data']['Car_id'][0]+1])
				elseif list[i]['Target_Data']['Car_place'][0] > 0 then
					taskWarpCharIntoCarAsPassenger(PLAYER_PED, mp.cars[list[i]['Target_Data']['Car_id'][0]+1],list[i]['Target_Data']['Car_place'][0]-1)
				end
			end
			if list[i]['Target_Data']['Target_type'][0] == 3 then
				alterWantedLevel(PLAYER_HANDLE, list[i]['Target_Data']['Level_battue'][0])
				setMaxWantedLevel(6)
				print('hello')
			end
			if list[i]['Target_Data']['Target_type'][0] == 4 then
				removeAllCharWeapons(PLAYER_PED)
			end
			if list[i]['Target_Data']['Target_type'][0] == 5 then
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
					printString(koder(u8:decode(ffi.string(list[i]['Target_Data']['Dialog'][d]['Text']))), list[i]['Target_Data']['Dialog'][d]['Text_time'][0]*1000)
          wait(list[i]['Target_Data']['Dialog'][d]['Text_time'][0]*1000*skip)
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
			if list[i]['Target_Data']['Target_type'][0] == 6 then
				givePlayerMoney(PLAYER_HANDLE, list[i]['Target_Data']['Add_money'][0])
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
	setMaxWantedLevel(0)
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
	for j = 1,#vr.list_actors do
		upd_actor:run(j)
	end
	for j = 1,#vr.list_cars do
		upd_car:run(j)
	end
	for j = 1,#vr.list_objects do
		upd_object:run(j)
	end
	for j = 1,#vr.list_pickup do
		upd_pickup:run(j)
	end
	for j = 1,#vr.list_particle do
		upd_particle:run(j)
	end
	for j = 1,#vr.list_explosion do
		upd_explosion:run(j)
	end
end

return mp
