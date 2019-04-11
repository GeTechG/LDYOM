mp = {}
mp.miss = 0
glob = require 'game.globals'
koder = require 'TextToGTX'
encoding = require 'encoding'
encoding.default = 'CP1251'
u8 = encoding.UTF8
mp.actors = {}


function mp.start_mission(listt,lista)
	misflag = mp.flagmis()
	mp.miss = mp.start()
	imgui.Process = false
	mp.main_mission(listt,lista)
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

function mp.main_mission(list,list_a)
	for i = 1,#list do
		for a = 1,#list_a do
			if list_a[a]['Actor_Data']['StartC'].v + 1 == i then
				local md = list_a[a]['Actor_Data']['ModelId'].v
				local xx,xy,xz = list_a[a]['Actor_Data']['Pos'].v[1], list_a[a]['Actor_Data']['Pos'].v[2], list_a[a]['Actor_Data']['Pos'].v[3]
				requestModel(md)
				while not isModelAvailable(md) do
					wait(1)
				end
				mp.actors[a] = createChar(4, md, xx, xy, xz)
				setCharHeading(mp.actors[a], list_a[a]['Actor_Data']['Angle'].v)
			end
			if list_a[a]['Actor_Data']['EndC'].v + 2 == i then
				deleteChar(mp.actors[a])
			end
		end
		if list[i]['Type'].v == 0 then
			wait(100)
			local xx,xy,xz = list[i]['Target_Data']['Pos'].v[1],list[i]['Target_Data']['Pos'].v[2],list[i]['Target_Data']['Pos'].v[3]
			rad = list[i]['Target_Data']['Radius'].v
			--sph[i] = addSphere(xx,xy,xz,rad)
			resultat,ped = getPlayerChar(PLAYER_HANDLE)
			printString(koder(u8:decode(list[i]['Target_Data']['Text'].v)),list[i]['Target_Data']['Text_time'].v * 1000)
			local check = addBlipForCoord(xx,xy,xz)
			changeBlipColour(check,list[i]['Target_Data']['Color_blip'].v)
			while not locateCharOnFoot3d(ped,xx,xy,xz,rad,rad,rad,true) do
				wait(0)
			end
			removeBlip(check)
			--removeSphere(sph[i])
		end
	end
	for v,h in pairs(mp.actors) do
		deleteChar(mp.actors[v])
	end
	for j = 1,#list_a do
		upd_actor:run(j)
	end
	mp.pass(1)
end

return mp
