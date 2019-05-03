mp = {}
mp.miss = 0
glob = require 'game.globals'
koder = require 'TextToGTX'
encoding = require 'encoding'
encoding.default = 'CP1251'
u8 = encoding.UTF8
mp.actors = {}
mp.cars = {}
mp.objects = {}
curr_target = 0

function mp.start_mission(listt,lista,listc,listo,missd)
	misflag = mp.flagmis()
	mp.miss = mp.start()
	imgui.Process = false
	mp.main_mission(listt,lista,listc,listo,missd)
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
    fall()
  end)
end

function mp.pass(money)
  setGameGlobal(glob.ONMISSION, 0)
  printStyledString(koder('Затащено'), 4000, 1)
  playMissionPassedTune(1)
  givePlayerMoney(PlayerPed, money, 200)
  mp.miss = 0
end

function mp.fall()
  setGameGlobal(glob.ONMISSION, 0)
  printWithNumberBig("M_FAIL", 100, 4000, 1)
  mp.miss = 0
end

function mp.play_char_anims(ped,actr)
	local curr_anim = 1
	while curr_anim <= #actr['Anims'] do
		wait(0)
		if actr['Anims'][curr_anim]['Condition'].v == 0 then
			if not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'].v+1]) then
				requestAnimation(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'].v+1])
				while not hasAnimationLoaded(Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'].v+1]) do
					wait(0)
				end
			end
			local animm = Anims['Anim_list'][actr['Anims'][curr_anim]['Pack_anim'].v+1]
			animm = animm[actr['Anims'][curr_anim]['Anim'].v+1]
			taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'].v+1], 1.0, actr['Anims'][curr_anim]['Loop'].v, false, false, false, -1)
			wait(actr['Anims'][curr_anim]['Time'].v * 1000.0)
			curr_anim = curr_anim + 1
		elseif actr['Anims'][curr_anim]['Condition'].v == 1 then
			if curr_target == actr['Anims'][curr_anim]['Target'].v+1 then
				local animm = Anims['Anim_list'][actr['Anims'][curr_anim]['Pack_anim'].v+1]
				animm = animm[actr['Anims'][curr_anim]['Anim'].v+1]
				taskPlayAnim(ped, animm, Anims['Anim_name'][actr['Anims'][curr_anim]['Pack_anim'].v+1], 1.0, actr['Anims'][curr_anim]['Loop'].v, false, false, false, -1)
				wait(actr['Anims'][curr_anim]['Time'].v * 1000.0)
				curr_anim = curr_anim + 1
			end
		end
	end
end

function mp.main_mission(list,list_a,list_c,list_o,miss_data)
	printStyledString(koder(u8:decode(miss_data['Name'])), 2000, 2)
	setTimeOfDay(miss_data['Time'][1], miss_data['Time'][2])
	forceWeatherNow(miss_data['Weather'].v)
	setLaRiots(miss_data['Riot'].v)
	setCharCoordinates(PLAYER_PED, miss_data['Player']['Pos'][1], miss_data['Player']['Pos'][2], miss_data['Player']['Pos'][3])
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
			if list_a[a]['Actor_Data']['StartC'].v + 1 == i then
				local md = list_a[a]['Actor_Data']['ModelId'].v
				local xx,xy,xz = list_a[a]['Actor_Data']['Pos'].v[1], list_a[a]['Actor_Data']['Pos'].v[2], list_a[a]['Actor_Data']['Pos'].v[3]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.actors[a] = createChar(4, md, xx, xy, xz)
				setCharHeading(mp.actors[a], list_a[a]['Actor_Data']['Angle'].v)
				wait(0)
				mp.tread_play_actr_anim:run(mp.actors[a], list_a[a]['Actor_Data'])
			end
			if list_a[a]['Actor_Data']['EndC'].v ~= 0 and list_a[a]['Actor_Data']['EndC'].v + 1 == i then
				deleteChar(mp.actors[a])
			end
		end
		for c = 1,#list_c do
			if list_c[c]['Car_Data']['StartC'].v + 1 == i then
				local md = list_c[c]['Car_Data']['ModelId'].v
				local xx,xy,xz = list_c[c]['Car_Data']['Pos'].v[1], list_c[c]['Car_Data']['Pos'].v[2], list_c[c]['Car_Data']['Pos'].v[3]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(0)
				end
				mp.cars[c] = createCar(md, xx, xy, xz)
				setCarHeading(mp.cars[c], list_c[c]['Car_Data']['Angle'].v)
			end
			if list_c[c]['Car_Data']['EndC'].v ~= 0 and list_c[c]['Car_Data']['EndC'].v + 1 == i then
				deleteCar(mp.cars[c])
			end
		end
		for o = 1,#list_o do
			if list_o[o]['Object_Data']['StartC'].v + 1 == i then
				local md = list_o[o]['Object_Data']['ModelId'].v
				local xx,xy,xz = list_o[o]['Object_Data']['Pos'].v[1], list_o[o]['Object_Data']['Pos'].v[2], list_o[o]['Object_Data']['Pos'].v[3]
				local rxx,rxy,rxz = list_o[o]['Object_Data']['Rotates'].v[1], list_o[o]['Object_Data']['Rotates'].v[2], list_o[o]['Object_Data']['Rotates'].v[3]

				mp.objects[o] = createObject(md, xx, xy, xz)
				setObjectCoordinates(mp.objects[o], xx, xy, xz)
				setObjectRotation(mp.objects[o], rxx, rxy, rxz)
			end
			if list_o[o]['Object_Data']['EndC'].v ~= 0 and list_o[o]['Object_Data']['EndC'].v + 1 == i then
				deleteObject(mp.objects[o])
			end
		end
		if list[i]['Type'].v == 0 then
			lockPlayerControl(false)
			wait(100)
			local xx,xy,xz = list[i]['Target_Data']['Pos'].v[1],list[i]['Target_Data']['Pos'].v[2],list[i]['Target_Data']['Pos'].v[3]
			rad = list[i]['Target_Data']['Radius'].v
			--sph[i] = addSphere(xx,xy,xz,rad)
			printString(koder(u8:decode(list[i]['Target_Data']['Text'].v)),list[i]['Target_Data']['Text_time'].v * 1000)
			local check = addBlipForCoord(xx,xy,xz)
			changeBlipColour(check,list[i]['Target_Data']['Color_blip'].v)
			while not locateCharAnyMeans3d(PLAYER_PED,xx,xy,xz,rad,rad,rad,true) do
				wait(0)
			end
			removeBlip(check)
			--removeSphere(sph[i])
		end
		if list[i]['Type'].v == 1 then
			lockPlayerControl(false)
			local check = addBlipForCar(mp.cars[list[i]['Target_Data']['Target_car_id'].v+1])
			changeBlipColour(check, 1)
			printString(koder('Цель: сесть в ~b~машину'), 2000)
			while not isCharInCar(PLAYER_PED, mp.cars[list[i]['Target_Data']['Target_car_id'].v+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'].v == 2 then
			lockPlayerControl(false)
			local check = addBlipForChar(mp.actors[list[i]['Target_Data']['Target_actor_id'].v+1])
			changeBlipColour(check, 0)
			printString(koder('Цель: убей ~r~человека'), 2000)
			while not isCharDead(mp.actors[list[i]['Target_Data']['Target_actor_id'].v+1]) do
				wait(0)
			end
			removeBlip(check)
		end
		if list[i]['Type'].v == 3 then
			if list[i]['Target_Data']['Target_type'].v == 0 then
				displayRadar(false)
				displayHud(false)
				lockPlayerControl(true)
				local xx,xy,xz = list[i]['Target_Data']['Pos'].v[1],list[i]['Target_Data']['Pos'].v[2],list[i]['Target_Data']['Pos'].v[3]
				local rxx,rxy,rxz = list[i]['Target_Data']['Rotates'].v[1],list[i]['Target_Data']['Rotates'].v[2],list[i]['Target_Data']['Rotates'].v[3]
				local x1,y1,z1 = xx,xy,xz
				local dx,dy,dz = xx+2-x1,xy+2-y1,xz+2-z1
				x1 = x1 + dx*math.sin(math.rad(rxy))
				y1 = y1 + dy*math.cos(math.rad(rxy))
				z1 = z1
				dx,dy,dz = xx+2-x1,xy+2-y1,xz+2-z1
				x1 = x1 + dx*math.sin(math.rad(rxy))
				y1 = y1 + dy*math.cos(math.rad(rxy))
				z1 = z1 + dz*math.cos(math.rad(rxx))
				if list[i]['Target_Data']['Smooth'].v then
					setInterpolationParameters(0,list[i]['Target_Data']['Text_time'].v*1000)
					setFixedCameraPosition(xx, xy, xz)
					pointCameraAtPoint(x1, y1, z1, 1)
				else
					setFixedCameraPosition(xx, xy, xz)
					pointCameraAtPoint(x1, y1, z1, 2)
				end

				printString(koder(u8:decode(list[i]['Target_Data']['Text'].v)), list[i]['Target_Data']['Text_time'].v*1000)
				wait(list[i]['Target_Data']['Text_time'].v*1000)
				displayRadar(true)
				displayHud(true)
				lockPlayerControl(false)
				restoreCameraJumpcut()
			end
			if list[i]['Target_Data']['Target_type'].v == 1 then
				setPlayerControl(PLAYER_HANDLE, false)
				for t = 0,list[i]['Target_Data']['Time'].v-1 do
					printStyledString(tostring(list[i]['Target_Data']['Time'].v - t), 1000, 4)
					wait(1000)
				end
				printStyledString('Go!', 1000, 4)
				wait(1000)
				setPlayerControl(PLAYER_HANDLE, true)
			end
			if list[i]['Target_Data']['Target_type'].v == 2 then
				printString(koder(u8:decode(list[i]['Target_Data']['Text'].v)), list[i]['Target_Data']['Time'].v*1000)
				wait(list[i]['Target_Data']['Time'].v*1000)
			end
			if list[i]['Target_Data']['Target_type'].v == 3 then
				forceWeatherNow(list[i]['Target_Data']['Weather'].v)
			end
			if list[i]['Target_Data']['Target_type'].v == 4 then
				setTimeOfDay(list[i]['Target_Data']['Clock_time'][1], list[i]['Target_Data']['Clock_time'][2])
			end
			if list[i]['Target_Data']['Target_type'].v == 5 then
				setPedDensityMultiplier(1 * list[i]['Target_Data']['Traffic'][1].v)
				setCarDensityMultiplier(1 * list[i]['Target_Data']['Traffic'][2].v)
			end
		end
		if list[i]['Type'].v == 4 then
			lockPlayerControl(false)
			local check = addBlipForObject(mp.objects[list[i]['Target_Data']['Target_object_id'].v+1])
			changeBlipColour(check, list[i]['Target_Data']['Color_blip'].v)
			printString(koder(u8:decode(list[i]['Target_Data']['Text'].v)), 2000)

			if list[i]['Target_Data']['Target_type'].v == 0 then
				while not isCharTouchingObject(PLAYER_PED, mp.objects[list[i]['Target_Data']['Target_object_id'].v+1]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'].v == 1 then
				while not hasObjectBeenDamaged(mp.objects[list[i]['Target_Data']['Target_object_id'].v+1]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'].v == 2 then
				while not hasObjectBeenPhotographed(mp.objects[list[i]['Target_Data']['Target_object_id'].v+1]) do
					wait(0)
				end
			end
			if list[i]['Target_Data']['Target_type'].v == 3 then
				while not hasObjectBeenDamagedByWeapon(mp.objects[list[i]['Target_Data']['Target_object_id'].v+1],list[i]['Target_Data']['Weap'].v) do
					wait(0)
				end
			end
			removeBlip(check)
		end
		if list[i]['Type'].v == 5 then
			if list[i]['Target_Data']['Target_type'].v == 0 then
				lockPlayerControl(false)
				if isCharInAnyCar(PLAYER_PED) then
					taskLeaveAnyCar(PLAYER_PED)
					while isCharInAnyCar(PLAYER_PED) do
						wait(0)
					end
				end
				setCharInterior(PLAYER_PED, list[i]['Target_Data']['Interior_id'])
				setInteriorVisible(list[i]['Target_Data']['Interior_id'])
				if not hasModelLoaded(list[i]['Target_Data']['ModelID'].v) then
					requestModel(list[i]['Target_Data']['ModelID'].v)
					while not hasModelLoaded(list[i]['Target_Data']['ModelID'].v) do
						wait(0)
					end
				end
				setPlayerModel(PLAYER_HANDLE, list[i]['Target_Data']['ModelID'].v)
				setCharCoordinates(PLAYER_PED, list[i]['Target_Data']['Pos'].v[1], list[i]['Target_Data']['Pos'].v[2], list[i]['Target_Data']['Pos'].v[3])
				giveWeaponToChar(PLAYER_PED, list[i]['Target_Data']['Weapon'].v, list[i]['Target_Data']['Weap_ammo'].v)
				setCharHeading(PLAYER_PED, list[i]['Target_Data']['Angle'].v)
			end
			if list[i]['Target_Data']['Target_type'].v == 1 then
				lockPlayerControl(true)
				if not hasAnimationLoaded(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'].v+1]) then
					requestAnimation(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'].v+1])
					while hasAnimationLoaded(Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'].v+1]) do
						wait(0)
					end
				end
				local animm = Anims['Anim_list'][list[i]['Target_Data']['Pack_anim'].v+1]
				animm = animm[list[i]['Target_Data']['Anim'].v+1]
				print(animm)
				taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][list[i]['Target_Data']['Pack_anim'].v+1], 1.0, list[i]['Target_Data']['Loop'].v, false, false, false, -1)
				wait(1)
			end
			if list[i]['Target_Data']['Target_type'].v == 2 then
				lockPlayerControl(false)
				if list[i]['Target_Data']['Car_place'].v == 0 then
					taskWarpCharIntoCarAsDriver(PLAYER_PED, mp.cars[list[i]['Target_Data']['Car_id'].v+1])
				elseif list[i]['Target_Data']['Car_place'].v > 0 then
					taskWarpCharIntoCarAsPassenger(PLAYER_PED, mp.cars[list[i]['Target_Data']['Car_id'].v+1],list[i]['Target_Data']['Car_place'].v-1)
				end
			end
			if list[i]['Target_Data']['Target_type'].v == 3 then
				alterWantedLevel(PLAYER_HANDLE, list[i]['Target_Data']['Level_battue'].v)
				setMaxWantedLevel(6)
				print('hello')
			end
			if list[i]['Target_Data']['Target_type'].v == 4 then
				removeAllCharWeapons(PLAYER_PED)
			end
			if list[i]['Target_Data']['Target_type'].v == 5 then
				lockPlayerControl(false)
				if not hasModelLoaded(330) then
					requestModel(330)
					while not hasModelLoaded(330) do
						wait(1)
					end
				end
				phone = createObject(330, 0, 0, 0)
				wait(1)
				setObjectCoordinates(phone, 2488.1010742188, -1666.6873779297, 13.34375 )
				--ObjectDynamic(phones, true)
				taskPlayAnim(PLAYER_PED, 'phone_in', 'PED', 4.0, false, false, false, true, -1)
				wait(900)
				taskPickUpObject(PLAYER_PED, phone, 0, 0, 0, 6, 16, 'NULL', 'NULL', -1)
				wait(1430)
				taskPlayAnim(PLAYER_PED, 'PHONE_TALK', 'PED', 4.0, true, false, false, true, -1)
				for d = 1,#list[i]['Target_Data']['Dialog'] do
					printString(koder(u8:decode(list[i]['Target_Data']['Dialog'][d]['Text'].v)), list[i]['Target_Data']['Dialog'][d]['Text_time'].v*1000)
					wait(list[i]['Target_Data']['Dialog'][d]['Text_time'].v*1000)
				end
				taskPlayAnim(PLAYER_PED, 'PHONE_OUT', 'PED', 4.0, false, false, false, false, -1)
				wait(1570)
				removeObjectElegantly(phone)
				wait(430)

			end
			if list[i]['Target_Data']['Target_type'].v == 6 then
				givePlayerMoney(PLAYER_HANDLE, list[i]['Target_Data']['Add_money'].v)
			end
		end
	end
	mp.pass(1)
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
	for j = 1,#list_a do
		upd_actor:run(j)
	end
	for j = 1,#list_c do
		upd_car:run(j)
	end
	for j = 1,#list_o do
		upd_object:run(j)
	end
end

return mp
