vkeys = require 'vkeys'
imgui = require 'imgui'
koder = require 'TextToGTX'
mp = require 'mission_player'
model = require 'lib.game.models'
encoding = require 'encoding'
encoding.default = 'CP1251'
u8 = encoding.UTF8

ID_Actors = {0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288}

local buf_edit_targets_name = imgui.ImBuffer(64);
local buf_edit_actors_name = imgui.ImBuffer(64);
local buf_edit_cars_name = imgui.ImBuffer(64);
local buf_edit = imgui.ImFloat(0);
local main_window = imgui.ImBool(false)
local targets_window = imgui.ImBool(false)
local actors_window = imgui.ImBool(false)
local cars_window = imgui.ImBool(false)
local targets_window_s1 = imgui.ImBool(false)
local targets_window_s2 = imgui.ImBool(false)
local lb_cur_targets = imgui.ImInt(0)
local lb_cur_actors = imgui.ImInt(0)
local lb_cur_cars = imgui.ImInt(0)
local list_targets = {}
local list_actors = {}
local list_cars = {}
local list_name_targets = {}
local list_name_actors = {}
local list_name_cars = {}
local editmode_target = false
local editmode_actor = false
local editmode_actor = false
local id_target = 0;
local id_actor = 0;
local id_cars = 0;
local r
local miss_start = false
local update_actor = -1

function imgui.OnDrawFrame()
	-- Главное меню
	if main_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 200, 400
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Главное меню', main_window)
		imgui.Checkbox(u8'Цели',targets_window)
		imgui.Checkbox(u8'Актёры',actors_window)
		--imgui.Checkbox(u8'Машины',cars_window)
		imgui.Separator()
		if imgui.Button(u8'Запустить миссию') then
			miss_start = true
		end
		imgui.End()
	end
	-- Окно списка целей
	if targets_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 250,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Цели', targets_window)

		imgui.PushItemWidth(-10)


		--Список Объектов
		imgui.ListBox('', lb_cur_targets,list_name_targets,15)
		if imgui.BeginPopupContextItem('hee') then
			imgui.InputText('',buf_edit_targets_name)
			if imgui.Button(u8'Применить') and buf_edit_targets_name.v ~= '' then
				list_targets[lb_cur_targets.v+1]['Name'] = buf_edit_targets_name.v
				list_name_targets[lb_cur_targets.v+1] = buf_edit_targets_name.v
				buf_edit_targets_name.v = ''
				imgui.CloseCurrentPopup()
			end
			imgui.PushItemWidth(-1);
			imgui.PopItemWidth();
			imgui.EndPopup();
		end

		--Кнопки редактирования
		if imgui.Button(u8'Добавить') then
			list_targets[#list_targets+1] = {
				['Name'] = u8'Цель #' .. tostring(#list_targets+1),
				['Type'] = imgui.ImInt(-1),
				['Enable'] = imgui.ImBool(false),
				['Target_Data'] = {}
			}
			list_name_targets[#list_targets] = list_targets[#list_targets]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Откр/Закр') then
			list_targets[lb_cur_targets.v+1]['Enable'].v = not list_targets[lb_cur_targets.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			list_targets = DelCellArr(list_targets,lb_cur_targets.v+1)
			list_name_targets = DelCellArr(list_name_targets,lb_cur_targets.v+1)
		end

		imgui.End()
	end

	--Редактор цели
	for i = 1, #list_targets do
		if #list_targets > 0 then
			if list_targets[i]['Enable'].v then
				local resX,resY = getScreenResolution()
				local sizeX,sizeY = 300, 340
				local targets_list_arr = {u8'Чекпоинт'}--,u8'Сесть в машину'}
				local targets_marker_color = {u8'Красный',u8'Зелёный',u8'Свело-голубой',u8'Белый',u8'Жёлтый'}
				imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
				imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 250,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
				imgui.Begin(list_targets[i]['Name'], list_targets[i]['Enable'])

				imgui.PushItemWidth(-65)

				if imgui.Combo(u8'Тип Цели',list_targets[i]['Type'],targets_list_arr) then
					if list_targets[i]['Type'].v == 0 then
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						list_targets[i]['Target_Data'] = {}
						list_targets[i]['Target_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
						list_targets[i]['Target_Data']['Radius'] = imgui.ImInt(2)
						list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
						list_targets[i]['Target_Data']['Text_time'] = imgui.ImFloat(2)
						list_targets[i]['Target_Data']['Color_blip'] = imgui.ImInt(0)
					end

				end

				if list_targets[i]['Type'].v == 0 then
					imgui.InputFloat3(u8'Позиция',list_targets[i]['Target_Data']['Pos'])

					if imgui.Button(u8'Режим перемещения') then
						editmode_target = true
						printString(koder('Двигать: Стрелки ~n~Выйти: F'),999999)
						lockPlayerControl(true)
						id_target = i
						imgui.Process = false
					end
					imgui.PushItemWidth(-130)
					imgui.DragInt(u8'Радиус чекпоинта',list_targets[i]['Target_Data']['Radius'],0.1,-100,100)
					imgui.Combo(u8'Цвет маркера',list_targets[i]['Target_Data']['Color_blip'],targets_marker_color)
					imgui.InputText(u8'Текст цели',list_targets[i]['Target_Data']['Text'])
					imgui.InputFloat(u8'Время текста (сек)',list_targets[i]['Target_Data']['Text_time'])
				end
				if list_targets[i]['Type'].v == 1 then
				end

				imgui.End()
			end
		end
	end

	-- Окно списка актёров
	if actors_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 250,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Актёры', actors_window)

		imgui.PushItemWidth(-10)


		--Список Актёров
		imgui.ListBox('', lb_cur_actors,list_name_actors,15)
		if imgui.BeginPopupContextItem('hee') then
			imgui.InputText('',buf_edit_actors_name)
			if imgui.Button(u8'Применить') and buf_edit_actors_name.v ~= '' then
				list_actors[lb_cur_actors.v+1]['Name'] = buf_edit_actors_name.v
				list_name_actors[lb_cur_actors.v+1] = buf_edit_actors_name.v
				buf_edit_actors_name.v = ''
				imgui.CloseCurrentPopup()
			end
			imgui.PushItemWidth(-1);
			imgui.PopItemWidth();
			imgui.EndPopup();
		end

		--Кнопки редактирования
		if imgui.Button(u8'Добавить') then
			local xx,xy,xz = getCharCoordinates(PLAYER_PED)
			local angle = getCharHeading(PLAYER_PED)
			list_actors[#list_actors+1] = {
				['Name'] = u8'Актёр #' .. tostring(#list_actors+1),
				['Type'] = imgui.ImInt(-1),
				['Enable'] = imgui.ImBool(false),
				['Actor_Data'] = {
					['Pos'] = imgui.ImFloat3(xx,xy,xz),
					['Angle'] = imgui.ImFloat(angle),
					['ModelId'] = imgui.ImInt(0),
					['StartC'] = imgui.ImInt(0),
					['EndC'] = imgui.ImInt(0)
				}

			}
			upd_actor:run(#list_actors)
			list_name_actors[#list_actors] = list_actors[#list_actors]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Откр/Закр') then
			list_actors[lb_cur_actors.v+1]['Enable'].v = not list_actors[lb_cur_actors.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			list_actors = DelCellArr(list_actors,lb_cur_actors.v+1)
			list_name_actors = DelCellArr(list_name_actors,lb_cur_actors.v+1)
		end
		imgui.End()
	end

	--Редактор актёра
	for i = 1, #list_actors do
		if #list_actors > 0 then
			if list_actors[i]['Enable'].v then
				local resX,resY = getScreenResolution()
				local sizeX,sizeY = 300, 340
				imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
				imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 250,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
				imgui.Begin(list_actors[i]['Name'], list_actors[i]['Enable'])
				imgui.PushItemWidth(-90)

				if imgui.InputFloat3(u8'Позиция',list_actors[i]['Actor_Data']['Pos']) or imgui.SliderFloat(u8'Угол поворота',list_actors[i]['Actor_Data']['Angle'],-360,360) then
					upd_actor:run(i)
				end
				if imgui.InputInt(u8'Номер модели',list_actors[i]['Actor_Data']['ModelId']) then
					local id_a = 0
					for v = 1,#ID_Actors do
						if list_actors[i]['Actor_Data']['ModelId'].v <= ID_Actors[v] then
							id_a = ID_Actors[v]
							break
						end
					end
					print(id_a)
					list_actors[i]['Actor_Data']['ModelId'].v = id_a
					upd_actor:run(i)
				end
				if imgui.Button(u8'Редактировать вручную') then
					editmode_actor = true
					id_actor = i
					deleteChar(list_actors[i]['Actor_Data']['Char'])
				end

				imgui.PushItemWidth(-110)

				imgui.Separator()
				imgui.Combo(u8'Появление на',list_actors[i]['Actor_Data']['StartC'],list_name_targets)
				imgui.Combo(u8'Исчезание после',list_actors[i]['Actor_Data']['EndC'],list_name_targets)

				imgui.End()
			end
		end
	end

	-- Окно списка машин
	if cars_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 250,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Машины', cars_window)

		imgui.PushItemWidth(-10)


		--Список Машин
		imgui.ListBox('', lb_cur_cars,list_name_cars,15)
		if imgui.BeginPopupContextItem('hee') then
			imgui.InputText('',buf_edit_cars_name)
			if imgui.Button(u8'Применить') and buf_edit_actors_name.v ~= '' then
				list_cars[lb_cur_cars.v+1]['Name'] = buf_edit_actors_name.v
				list_name_cars[lb_cur_cars.v+1] = buf_edit_actors_name.v
				buf_edit_actors_name.v = ''
				imgui.CloseCurrentPopup()
			end
			imgui.PushItemWidth(-1);
			imgui.PopItemWidth();
			imgui.EndPopup();
		end

		--Кнопки редактирования
		if imgui.Button(u8'Добавить') then
			local xx,xy,xz = getCharCoordinates(PLAYER_PED)
			list_cars[#list_cars+1] = {
				['Name'] = u8'Машина #' .. tostring(#list_cars+1),
				['Type'] = imgui.ImInt(-1),
				['Enable'] = imgui.ImBool(false),
				['Car_Data'] = {
					['Pos'] = imgui.ImFloat3(xx,xy,xz),
					['Angle'] = imgui.ImFloat(0),
					['ModelId'] = imgui.ImInt(0),
					['StartC'] = imgui.ImInt(0),
					['EndC'] = imgui.ImInt(0)
				}

			}
			list_name_cars[#list_cars] = list_cars[#list_cars]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Откр/Закр') then
			list_cars[lb_cur_cars.v+1]['Enable'].v = not list_cars[lb_cur_cars.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			list_cars = DelCellArr(list_cars,lb_cur_cars.v+1)
			list_name_cars = DelCellArr(list_name_cars,lb_cur_cars.v+1)
		end
		imgui.End()
	end

	--Редактор машин
	for i = 1, #list_cars do
		if #list_cars > 0 then
			if list_cars[i]['Enable'].v then
				local resX,resY = getScreenResolution()
				local sizeX,sizeY = 300, 340
				imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
				imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 250,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
				imgui.Begin(list_cars[i]['Name'], list_cars[i]['Enable'])
				imgui.PushItemWidth(-90)

				if imgui.InputFloat3(u8'Позиция',list_cars[i]['Car_Data']['Pos']) or imgui.SliderFloat(u8'Угол поворота',list_cars[i]['Car_Data']['Angle'],-360,360) then
					upd_actor:run(i)
				end
				if imgui.InputInt(u8'Номер модели',list_cars[i]['Car_Data']['ModelId']) then
					local id_a = 0
					for v = 1,#ID_Actors do
						if list_cars[i]['Car_Data']['ModelId'].v <= ID_Actors[v] then
							id_a = ID_Actors[v]
							break
						end
					end
					print(id_a)
					list_cars[i]['Car_Data']['ModelId'].v = id_a
					upd_actor:run(i)
				end
				if imgui.Button(u8'Редактировать вручную') then
					editmode_actor = true
					id_actor = i
					deleteChar(list_cars[i]['Car_Data']['Car'])
				end

				imgui.PushItemWidth(-110)

				imgui.Separator()
				imgui.Combo(u8'Появление на',list_cars[i]['Car_Data']['StartC'],list_name_targets)
				imgui.Combo(u8'Исчезание после',list_cars[i]['Car_Data']['EndC'],list_name_targets)

				imgui.End()
			end
		end
	end
end

-- Удаление ячейки
function DelCellArr(arr,i)
	local array = {}
	for v,k in ipairs(arr) do
		if v ~= i then
			array[#array+1] = k
		end
	end
	return array
end

function update_actor(actorr)
	local modell = list_actors[actorr]['Actor_Data']['ModelId'].v
	local xx,xy,xz = list_actors[actorr]['Actor_Data']['Pos'].v[1], list_actors[actorr]['Actor_Data']['Pos'].v[2], list_actors[actorr]['Actor_Data']['Pos'].v[3]
	xz = getGroundZFor3dCoord(xx,xy,xz)
	local angle = list_actors[actorr]['Actor_Data']['Angle'].v

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(1)
	end

	deleteChar(list_actors[actorr]['Actor_Data']['Char'])
	list_actors[actorr]['Actor_Data']['Char'] = createChar(4,modell,xx,xy,xz)
	setCharHeading(list_actors[actorr]['Actor_Data']['Char'], angle)
	setCharCollision(list_actors[actorr]['Actor_Data']['Char'],false)
end

function main()
	upd_actor = lua_thread.create_suspended(update_actor)
	while true do
		wait(0)
		removeSphere(r)
		--Редактор позиции объекта
		while editmode_target do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearPrints()
				editmode_target = false
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_SHIFT) then
				multy = 0.5
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_LEFT) then
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_RIGHT) then
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_UP) then
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_DOWN) then
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SPACE) then
				list_targets[id_target]['Target_Data']['Radius'].v = list_targets[id_target]['Target_Data']['Radius'].v + 1 * multy
			end
			--Отрисовка в редакторе
			local xx,xy,xz = list_targets[id_target]['Target_Data']['Pos'].v[1],list_targets[id_target]['Target_Data']['Pos'].v[2],list_targets[id_target]['Target_Data']['Pos'].v[3]
			drawSphere(xx, xy, xz,list_targets[id_target]['Target_Data']['Radius'].v)
		end

		if editmode_actor then
			imgui.Process = false
			requestModel(model.BMORI)
			while not hasModelLoaded(list_actors[id_actor]['Actor_Data']['ModelId'].v) do
				wait(1)
			end
			setPlayerModel(PLAYER_HANDLE, list_actors[id_actor]['Actor_Data']['ModelId'].v)
			local xx,xy,xz = list_actors[id_actor]['Actor_Data']['Pos'].v[1],list_actors[id_actor]['Actor_Data']['Pos'].v[2],list_actors[id_actor]['Actor_Data']['Pos'].v[3]
			setCharCoordinates(PLAYER_PED, xx, xy, xz)
			setCharHeading(PLAYER_PED, list_actors[id_actor]['Actor_Data']['Angle'].v)
			printString(koder('Следующий/Прошлый скин: I/O~n~Через 10 Следующий/Прошлый: K/L~n~Выйти: F'), 99999)
		end
		while editmode_actor do
			--Закрытие редактора
			wait(0)
			if wasKeyPressed(vkeys.VK_F) then
				clearPrints()
				editmode_actor = false
				setPlayerModel(PLAYER_HANDLE, model.NULL)
				local xx,xy,xz = getCharCoordinates(PLAYER_PED)
				local angle = getCharHeading(PLAYER_PED)
				xz = getGroundZFor3dCoord(xx,xy,xz)
				list_actors[id_actor]['Actor_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
				list_actors[id_actor]['Actor_Data']['Angle'].v = angle

				upd_actor:run(id_actor)
				clearPrints()
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				list_actors[id_actor]['Actor_Data']['ModelId'].v = list_actors[id_actor]['Actor_Data']['ModelId'].v + 1
				local id_a = 0
				for v = 1,#ID_Actors do
					if list_actors[id_actor]['Actor_Data']['ModelId'].v <= ID_Actors[v] then
						id_a = ID_Actors[v]
						break
					end
				end
				list_actors[id_actor]['Actor_Data']['ModelId'].v = id_a
				requestModel(list_actors[id_actor]['Actor_Data']['ModelId'].v)
				while not hasModelLoaded(list_actors[id_actor]['Actor_Data']['ModelId'].v) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, list_actors[id_actor]['Actor_Data']['ModelId'].v)
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				list_actors[id_actor]['Actor_Data']['ModelId'].v = list_actors[id_actor]['Actor_Data']['ModelId'].v - 1
				local id_a = 0
				for v = 1,#ID_Actors do
					if list_actors[id_actor]['Actor_Data']['ModelId'].v <= ID_Actors[v] then
						id_a = ID_Actors[v]
						break
					end
				end
				list_actors[id_actor]['Actor_Data']['ModelId'].v = id_a
				requestModel(list_actors[id_actor]['Actor_Data']['ModelId'].v)
				while not hasModelLoaded(list_actors[id_actor]['Actor_Data']['ModelId'].v) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, list_actors[id_actor]['Actor_Data']['ModelId'].v)
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				list_actors[id_actor]['Actor_Data']['ModelId'].v = list_actors[id_actor]['Actor_Data']['ModelId'].v + 10
				local id_a = 0
				for v = 1,#ID_Actors do
					if list_actors[id_actor]['Actor_Data']['ModelId'].v <= ID_Actors[v] then
						id_a = ID_Actors[v]
						break
					end
				end
				list_actors[id_actor]['Actor_Data']['ModelId'].v = id_a
				requestModel(list_actors[id_actor]['Actor_Data']['ModelId'].v)
				while not hasModelLoaded(list_actors[id_actor]['Actor_Data']['ModelId'].v) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, list_actors[id_actor]['Actor_Data']['ModelId'].v)
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				list_actors[id_actor]['Actor_Data']['ModelId'].v = list_actors[id_actor]['Actor_Data']['ModelId'].v - 10
				local id_a = 0
				for v = 1,#ID_Actors do
					if list_actors[id_actor]['Actor_Data']['ModelId'].v <= ID_Actors[v] then
						id_a = ID_Actors[v]
						break
					end
				end
				list_actors[id_actor]['Actor_Data']['ModelId'].v = id_a
				requestModel(list_actors[id_actor]['Actor_Data']['ModelId'].v)
				while not hasModelLoaded(list_actors[id_actor]['Actor_Data']['ModelId'].v) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, list_actors[id_actor]['Actor_Data']['ModelId'].v)
			end
		end

		for i = 1, #list_targets do
			if list_targets[i]['Type'].v == 0 then
				drawSphere(list_targets[i]['Target_Data']['Pos'].v[1], list_targets[i]['Target_Data']['Pos'].v[2], list_targets[i]['Target_Data']['Pos'].v[3],list_targets[i]['Target_Data']['Radius'].v)
			end
		end

		if miss_start then
			for i = 1,#list_actors do
				deleteChar(list_actors[i]['Actor_Data']['Char'])
			end
			mp.start_mission(list_targets,list_actors)
			miss_start = false
		end

		while not mp.miss == 0 do
			wait(0)
		end

		--Откр/Закр. Меню
		if wasKeyPressed(vkeys.VK_U) then
			main_window.v = not main_window.v
		end
		imgui.Process = main_window.v
		-- Дебаг: позиция
		if wasKeyPressed(vkeys.VK_Q) then
			px,py,pz = getCharCoordinates(PLAYER_PED)
			ang = getCharHeading(PLAYER_PED)
			print(px .. ' ' .. py .. ' ' .. pz .. ' ' .. ang)
		end
	end
end
