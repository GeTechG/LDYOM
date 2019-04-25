script_author('SKIC')

vkeys = require 'vkeys'
imgui = require 'imgui'
koder = require 'TextToGTX'
--debtab = require 'debug_table'
mp = require 'mission_player'
manager_miss = require 'MissManager'
model = require 'lib.game.models'
encoding = require 'encoding'
encoding.default = 'CP1251'
u8 = encoding.UTF8

ID_Actors = {0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288}

ID_Cars = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611}

Anims = {
	['Anim_name'] = {'PED','BOMBER','POOL','ON_LOOKERS','GANGS','PAULNMAC','BOX'},
	['Anim_list'] = {{"IDLE_CHAT","ROADCROSS","ATM","FLEE_LKAROUND_01","FUCKU","walk_armed","seat_up","run_armed","idle_gang1","hit_back","hit_r","hit_wall","hit_l","hita_2","climb_pull","bomber","floor_hit_f","fightshb","fall_fall","drown","floor_hit","IDLE_taxi","swim_tread","XPRESSSCRATCH","XPRESSSCRATCH","HANDSUP","KO_SHOT_STOM","KO_SKID_FRONT","DUCK_COWER","GETUP","SEAT_DOWN","ENDCHAT_03","GETUP_FRONT","GUN_STAND","KO_SPIN_L","KO_SKID_BACK","WALK_DRUNK","FALL_SKYDIVE","FALL_FRONT","RUN_PLAYER","WOMAN_IDLESTANCE","TAP_HAND","CAR_SIT","IDLE_STANCE","COWER","CROUCH_ROLL_R","CROUCH_ROLL_L","ENDCHAT_01","KO_SHOT_FACE","PHONE_TALK","KO_SHOT_FRONT","IDLE_ARMED","WEAPON_CROUCH","IDLE_TIRED","SEAT_IDLE","HANDSCOWER","FIGHTIDLE","FIGHTA_G","CAR_HOOKERTALK","HANDSUP","abseil","FIGHTA_M","FIGHTA_block","FIGHTA_2","gas_cwr","ENDCHAT_02"},{"BOM_PLANT_IN","BOM_PLANT_LOOP","BOM_PLANT_CROUCH_IN","BOM_PLANT_CROUCH_OUT","BOM_PLANT_2IDLE"},{"POOL_XLONG_SHOT","POOL_XLONG_START","POOL_LONG_SHOT","POOL_LONG_START","POOL_MED_START","POOL_MED_SHOT","POOL_SHORT_SHOT","POOL_CHALKCUE"},{"LKUP_LOOP","POINTUP_IN","SHOUT_01"},{"PRTIAL_GNGTLKA","PRTIAL_GNGTLKF","PRTIAL_GNGTLKD","PRTIAL_GNGTLKE","HNDSHKFA_SWT", "GANGS","PRTIAL_GNGTLKH","HNDSHKFA","DRUGS_BUY","PRTIAL_GNGTLKC","PRTIAL_GNGTLKH","PRTIAL_GNGTLKG","DRNKBR_PRTL","PRTIAL_GNGTLKB","PRTIAL_HNDSHK_01","DEALER_DEAL"},{"PISS_IN","PISS_LOOP","PISS_OUT","PNM_LOOP_A","PNM_ARGUE2_A","PNM_ARGUE1_A"},{"boxhipin","bxwlko","catch_box","bxshwlk","bxhwlki","boxshup","boxhipup","boxshdwn","bxhipwlk","bxshwlki"}}
}

local name_mission = ''
local buf_edit_targets_name = imgui.ImBuffer(64);
local buf_edit_actors_name = imgui.ImBuffer(64);
local buf_edit_cars_name = imgui.ImBuffer(64);
local buf_edit_obj_name = imgui.ImBuffer(64);
local buf_edit = imgui.ImFloat(0);
local main_window = imgui.ImBool(false)
local pack_mission_window = imgui.ImBool(false)
local missions_window = imgui.ImBool(false)
local targets_window = imgui.ImBool(false)
local actors_window = imgui.ImBool(false)
local cars_window = imgui.ImBool(false)
local objects_window = imgui.ImBool(false)
local targets_window_s1 = imgui.ImBool(false)
local targets_window_s2 = imgui.ImBool(false)
local missions_n_window = imgui.ImBool(false)
local lb_cur_targets = imgui.ImInt(0)
local lb_cur_actors = imgui.ImInt(0)
local lb_cur_cars = imgui.ImInt(0)
local lb_cur_objects = imgui.ImInt(0)
local lb_cur_pack_mission = imgui.ImInt(0)
local lb_cur_missions = imgui.ImInt(0)
local list_targets = {}
local list_actors = {}
local list_cars = {}
local list_objects = {}
local list_mission_pack = {}
local list_missions = {}
local list_name_targets = {}
local list_name_actors = {}
local list_name_cars = {}
local list_name_objects = {}
local list_name_mission_pack = {}
local list_name_missions = {}
local editmode_target = false
local editmode_camera = false
local editmode_actor = false
local editmode_objects = false
local id_target = 0;
local id_actor = 0;
local id_cars = 0;
local id_obj = 0;
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
		if imgui.Button(u8'Цели') then
			targets_window.v = not targets_window.v
			main_window.v = false
		end
		if imgui.Button(u8'Актёры') then
			actors_window.v = not actors_window.v
			main_window.v = false
		end
		if imgui.Button(u8'Машины') then
			cars_window.v = not cars_window.v
			main_window.v = false
		end
		if imgui.Button(u8'Объекты') then
			objects_window.v = not objects_window.v
			main_window.v = false
		end
		imgui.Separator()
		if imgui.Button(u8'Запустить миссию') then
			miss_start = true
		end
		if imgui.Button(u8'Паки миссий') then
			pack_mission_window.v = not pack_mission_window.v
			--manager_miss.save(list_targets,list_actors,list_cars,list_objects)
			main_window.v = false
		end
		if imgui.Button(u8'Миссии') then
			missions_window.v = not missions_window.v
			main_window.v = false
		end
		imgui.End()
	end

	if missions_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 350, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 175,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Миссии', missions_window)

		imgui.PushItemWidth(-10)


		--Список Объектов
		imgui.ListBox('', lb_cur_missions,list_name_missions,15)

		--Кнопки редактирования
		if imgui.Button(u8'Добавить') then
			list_missions[#list_missions+1] = {
				['Name'] = u8'Миссия #' .. tostring(#list_missions),
				['Prename'] = imgui.ImBuffer(256),
				['Enable'] = imgui.ImBool(false),
				['Mission_Data'] = {
					['Targets'] = list_targets,
					['Actors'] = list_actors,
					['Cars'] = list_cars,
					['Objects'] = list_objects
				}
			}
			list_name_missions[#list_missions] = list_missions[#list_missions]['Name']
			name_mission = list_missions[#list_missions]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Загрузить') and #list_missions > 0 then
			for i = 1,#list_actors do
				deleteChar(list_actors[i]['Actor_Data']['Char'])
			end
			for c = 1,#list_cars do
				deleteCar(list_cars[c]['Car_Data']['Car'])
			end
			list_targets = list_missions[lb_cur_missions.v+1]['Mission_Data']['Targets']
			list_actors = list_missions[lb_cur_missions.v+1]['Mission_Data']['Actors']
			list_cars = list_missions[lb_cur_missions.v+1]['Mission_Data']['Cars']
			list_objects = list_missions[lb_cur_missions.v+1]['Mission_Data']['Objects']
			name_mission = list_missions[lb_cur_missions.v+1]['Name']
			for a = 1, #list_actors do
				list_name_actors[a] = list_actors[a]['Name']
				upd_actor:run(a)
			end
			for t = 1, #list_targets do
				list_name_targets[t] = list_targets[t]['Name']
			end
		end

		imgui.SameLine()
		if imgui.Button(u8'Сохранить') and #list_missions > 0 then
			list_missions[lb_cur_missions.v+1]['Mission_Data'] = {
				['Targets'] = list_targets,
				['Actors'] = list_actors,
				['Cars'] = list_cars,
				['Objects'] = list_objects
			}
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			list_missions = DelCellArr(list_missions,lb_cur_missions.v+1)
			list_name_missions = DelCellArr(list_name_missions,lb_cur_missions.v+1)
		end
		imgui.SameLine()
		if imgui.Button(u8'Настройки') and #list_missions > 0 then
			list_missions[lb_cur_missions.v+1]['Enable'].v = not list_missions[lb_cur_missions.v+1]['Enable'].v
		end

		imgui.End()
		for t = 1,#list_missions do
			if list_missions[t]['Enable'].v then
				local resX,resY = getScreenResolution()
				local sizeX,sizeY = 300, 340
				imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
				imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
				imgui.Begin(list_missions[t]['Name'], list_missions[t]['Enable'])
				imgui.PushItemWidth(-120)
				imgui.InputText(u8'Название миссии',list_missions[t]['Prename'])
				if imgui.Button(u8'Применить') then
					list_missions[t]['Name'] = list_missions[t]['Prename'].v
					list_name_missions[t] = list_missions[t]['Name']
					name_mission = list_missions[t]['Name']
				end
				imgui.End()
			end
		end

	end

	if pack_mission_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Сохранение и загрузка пака миссий', pack_mission_window)

		imgui.PushItemWidth(-10)


		--Список Объектов
		imgui.ListBox('', lb_cur_pack_mission,list_name_mission_pack,15)

		--Кнопки редактирования
		if imgui.Button(u8'Добавить') then
			list_mission_pack[#list_mission_pack+1] = {
				['Name'] = u8'Пак миссий #' .. tostring(#list_mission_pack+1),
				['Enable'] = imgui.ImBool(false),
				['Missions'] = list_missions
			}
			list_name_mission_pack[#list_mission_pack] = list_mission_pack[#list_mission_pack]['Name']
			manager.save(list_mission_pack[#list_mission_pack],#list_mission_pack-1)
		end

		imgui.SameLine()
		if imgui.Button(u8'Загрузить') and #list_mission_pack > 0 then
			for i = 1,#list_actors do
				deleteChar(list_actors[i]['Actor_Data']['Char'])
			end
			for c = 1,#list_cars do
				deleteCar(list_cars[c]['Car_Data']['Car'])
			end
			list_name_missions = {}
			list_missions = list_mission_pack[lb_cur_pack_mission.v+1]['Missions']
			for m = 1,#list_mission_pack[lb_cur_pack_mission.v+1]['Missions'] do
				list_name_missions[m] = list_mission_pack[lb_cur_pack_mission.v+1]['Missions'][m]['Name']
			end
		end
		imgui.SameLine()
		if imgui.Button(u8'Сохранить') and #list_mission_pack > 0 then
			list_mission_pack[lb_cur_pack_mission.v+1]['Missions'] = list_missions
			manager.save(list_mission_pack[lb_cur_pack_mission.v+1],lb_cur_pack_mission.v)
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			list_mission_pack = DelCellArr(list_mission_pack,lb_cur_pack_mission.v+1)
			list_name_mission_pack = DelCellArr(list_name_mission_pack,lb_cur_pack_mission.v+1)
			manager.delete(lb_cur_pack_mission.v)
		end


		imgui.End()
	end

	-- Окно списка целей
	if targets_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
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
		if imgui.Button(u8'Откр/Закр') and #list_targets > 0 then
			list_targets[lb_cur_targets.v+1]['Enable'].v = not list_targets[lb_cur_targets.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			list_targets = DelCellArr(list_targets,lb_cur_targets.v+1)
			list_name_targets = DelCellArr(list_name_targets,lb_cur_targets.v+1)
		end


		imgui.End()
		--Редактор цели
		for i = 1, #list_targets do
			if #list_targets > 0 then
				if list_targets[i]['Enable'].v then
					local resX,resY = getScreenResolution()
					local sizeX,sizeY = 300, 340
					local targets_list_arr = {u8'Чекпоинт',u8'Сесть в машину',u8'Убить Актёра',u8'Катсцена'}
					local targets_marker_color = {u8'Красный',u8'Зелёный',u8'Свело-голубой',u8'Белый',u8'Жёлтый'}
					imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
					imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
					imgui.Begin(list_targets[i]['Name'], list_targets[i]['Enable'])

					imgui.PushItemWidth(-65)

					if imgui.Combo(u8'Тип Цели',list_targets[i]['Type'],targets_list_arr) then
						list_targets[i]['Target_Data'] = {}
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						if list_targets[i]['Type'].v == 0 then
							list_targets[i]['Target_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
							list_targets[i]['Target_Data']['Radius'] = imgui.ImInt(2)
							list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
							list_targets[i]['Target_Data']['Text_time'] = imgui.ImFloat(2)
							list_targets[i]['Target_Data']['Color_blip'] = imgui.ImInt(0)
						end
						if list_targets[i]['Type'].v == 1 then
							list_targets[i]['Target_Data']['Target_car_id'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Color_blip'] = imgui.ImInt(0)
						end
						if list_targets[i]['Type'].v == 2 then
							list_targets[i]['Target_Data']['Target_actor_id'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Color_blip'] = imgui.ImInt(0)
						end
						if list_targets[i]['Type'].v == 3 then
							list_targets[i]['Target_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
							list_targets[i]['Target_Data']['Rotates'] = imgui.ImFloat3(0,0,0)
							list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
							list_targets[i]['Target_Data']['Text_time'] = imgui.ImFloat(2)
							list_targets[i]['Target_Data']['Smooth'] = imgui.ImBool(false)
						end

					end

					if list_targets[i]['Type'].v == 0 then
						imgui.InputFloat3(u8'Позиция',list_targets[i]['Target_Data']['Pos'])

						if imgui.Button(u8'Режим перемещения') then
							editmode_target = true
							printHelpForever('HTARG')
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
						imgui.Combo(u8'Машина',list_targets[i]['Target_Data']['Target_car_id'],list_name_cars)
						imgui.Combo(u8'Цвет маркера',list_targets[i]['Target_Data']['Color_blip'],targets_marker_color)
					end
					if list_targets[i]['Type'].v == 2 then
						imgui.Combo(u8'Актёры',list_targets[i]['Target_Data']['Target_actor_id'],list_name_actors)
						imgui.Combo(u8'Цвет маркера',list_targets[i]['Target_Data']['Color_blip'],targets_marker_color)
					end

					if list_targets[i]['Type'].v == 3 then
						imgui.InputFloat3(u8'Позиция',list_targets[i]['Target_Data']['Pos'])
						imgui.InputFloat3(u8'Угл поворота',list_targets[i]['Target_Data']['Rotates'])
						if imgui.Button(u8'Режим перемещения') then
							editmode_camera = true
							printHelpForever('HOBJ')
							lockPlayerControl(true)
							id_target = i
							imgui.Process = false
						end
						imgui.Checkbox(u8'Движение камеры',list_targets[i]['Target_Data']['Smooth'])
						imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Text'])
						imgui.InputFloat(u8'Время катсцены (сек)',list_targets[i]['Target_Data']['Text_time'])
					end

					imgui.End()
				end
			end
		end
	end

	-- Окно списка актёров
	if actors_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 175,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
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
			xz = getGroundZFor3dCoord(xx,xy,xz)
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
					['EndC'] = imgui.ImInt(0),
					['Anims'] = {}
				}

			}
			upd_actor:run(#list_actors)
			list_name_actors[#list_actors] = list_actors[#list_actors]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Откр/Закр') and #list_actors > 0 then
			list_actors[lb_cur_actors.v+1]['Enable'].v = not list_actors[lb_cur_actors.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			deleteChar(list_actors[lb_cur_actors.v+1]['Actor_Data']['Char'])
			list_actors = DelCellArr(list_actors,lb_cur_actors.v+1)
			list_name_actors = DelCellArr(list_name_actors,lb_cur_actors.v+1)
		end

		imgui.End()
		--Редактор актёра
		for i = 1, #list_actors do
			if #list_actors > 0 then
				if list_actors[i]['Enable'].v then
					local resX,resY = getScreenResolution()
					local sizeX,sizeY = 400, 340
					imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
					imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 175,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
					imgui.Begin(list_actors[i]['Name'], list_actors[i]['Enable'])
					imgui.PushItemWidth(-90)

					if imgui.InputFloat3(u8'Позиция',list_actors[i]['Actor_Data']['Pos']) or imgui.SliderFloat(u8'Угол поворота',list_actors[i]['Actor_Data']['Angle'],-360,360) then
						local xx,xy,xz = list_actors[i]['Actor_Data']['Pos'].v[1],list_actors[i]['Actor_Data']['Pos'].v[2],list_actors[i]['Actor_Data']['Pos'].v[3]
						setCharCoordinates(list_actors[i]['Actor_Data']['Char'],xx,xy,xz)
						setCharHeading(list_actors[i]['Actor_Data']['Char'], list_actors[i]['Actor_Data']['Angle'].v)
					end
					if imgui.InputInt(u8'Номер модели',list_actors[i]['Actor_Data']['ModelId']) then
						local id_a = 0
						for v = 1,#ID_Actors do
							if list_actors[i]['Actor_Data']['ModelId'].v <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						list_actors[i]['Actor_Data']['ModelId'].v = id_a
						upd_actor:run(i)
					end
					if imgui.Button(u8'Редактировать вручную') then
						editmode_actor = true
						id_actor = i
						deleteChar(list_actors[i]['Actor_Data']['Char'])
						printHelpForever('HACT')
					end

					imgui.PushItemWidth(-110)

					imgui.Separator()
					local list_tg_m = {u8'До конца'}
					for ltg = 1,#list_name_targets do
						list_tg_m[#list_tg_m+1] = list_name_targets[ltg]
					end
					imgui.Combo(u8'Появление на',list_actors[i]['Actor_Data']['StartC'],list_name_targets)
					imgui.Combo(u8'Исчезание после',list_actors[i]['Actor_Data']['EndC'],list_tg_m)
					imgui.Separator()
					imgui.Text(u8'Анимации')
					if imgui.Button(u8'Добавить') then
						list_actors[i]['Actor_Data']['Anims'][#list_actors[i]['Actor_Data']['Anims']+1] = {
							['Anim'] = imgui.ImInt(0),
							['Pack_anim'] = imgui.ImInt(0),
							['Loop'] = imgui.ImBool(false),
							['Time'] = imgui.ImFloat(1.0),
							['Condition'] = imgui.ImInt(0)
						}
					end
					imgui.SameLine()
					if imgui.Button(u8'Очистить предпросмотр') then
						taskPlayAnim(list_actors[i]['Actor_Data']['Char'], 'NULL', 'NULL', 1.0, true, false, false, false, -1)
					end
					imgui.PushItemWidth(-150)
					for a = 1,#list_actors[i]['Actor_Data']['Anims'] do
						if imgui.TreeNode(u8:encode(a)) then
							local cond_type = {u8'Ничего',u8'На Цели'}
							if imgui.Combo(u8'Условие',list_actors[i]['Actor_Data']['Anims'][a]['Condition'],cond_type) then
								if list_actors[i]['Actor_Data']['Anims'][a]['Condition'].v == 1 then
									list_actors[i]['Actor_Data']['Anims'][a]['Target'] = imgui.ImInt(0)
								end
							end
							if list_actors[i]['Actor_Data']['Anims'][a]['Condition'].v == 1 then
								imgui.Combo(u8'Цель',list_actors[i]['Actor_Data']['Anims'][a]['Target'],list_name_targets)
							end
							imgui.Combo(u8'Пак',list_actors[i]['Actor_Data']['Anims'][a]['Pack_anim'],Anims['Anim_name'])
							imgui.Combo(u8'Анимация',list_actors[i]['Actor_Data']['Anims'][a]['Anim'],Anims['Anim_list'][list_actors[i]['Actor_Data']['Anims'][a]['Pack_anim'].v+1])
							imgui.InputFloat(u8'Время анимации (сек)',list_actors[i]['Actor_Data']['Anims'][a]['Time'],0)
							imgui.Checkbox(u8'Зацикленно',list_actors[i]['Actor_Data']['Anims'][a]['Loop'])
							if imgui.Button(u8'Предпросмотр') then
								upd_anim_actor:run(i,a)
							end
							imgui.Button(u8'Удалить')
							imgui.TreePop()

						end
					end
					imgui.End()
				end
			end
		end
	end

	-- Окно списка машин
	if cars_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
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
			xz = getGroundZFor3dCoord(xx, xy, xz+10.0)
			list_cars[#list_cars+1] = {
				['Name'] = u8'Машина #' .. tostring(#list_cars+1),
				['Type'] = imgui.ImInt(-1),
				['Enable'] = imgui.ImBool(false),
				['Car_Data'] = {
					['Pos'] = imgui.ImFloat3(xx,xy,xz),
					['Angle'] = imgui.ImFloat(0),
					['ModelId'] = imgui.ImInt(400),
					['StartC'] = imgui.ImInt(0),
					['EndC'] = imgui.ImInt(0)
				}

			}
			upd_car:run(#list_cars)
			list_name_cars[#list_cars] = list_cars[#list_cars]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Откр/Закр') and #list_cars > 0 then
			list_cars[lb_cur_cars.v+1]['Enable'].v = not list_cars[lb_cur_cars.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			deleteCar(list_cars[lb_cur_cars.v+1]['Car_Data']['Car'])
			list_cars = DelCellArr(list_cars,lb_cur_cars.v+1)
			list_name_cars = DelCellArr(list_name_cars,lb_cur_cars.v+1)
		end
		imgui.End()
		--Редактор машин
		for i = 1, #list_cars do
			if #list_cars > 0 then
				if list_cars[i]['Enable'].v then
					local resX,resY = getScreenResolution()
					local sizeX,sizeY = 300, 340
					imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
					imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
					imgui.Begin(list_cars[i]['Name'], list_cars[i]['Enable'])
					imgui.PushItemWidth(-90)

					if imgui.InputFloat3(u8'Позиция',list_cars[i]['Car_Data']['Pos']) or imgui.SliderFloat(u8'Угол поворота',list_cars[i]['Car_Data']['Angle'],-360,360) then
						local xx,xy,xz = list_cars[i]['Car_Data']['Pos'].v[1],list_cars[i]['Car_Data']['Pos'].v[2],list_cars[i]['Car_Data']['Pos'].v[3]
						setCarCoordinates(list_cars[i]['Car_Data']['Car'],xx,xy,xz)
						setCarHeading(list_cars[i]['Car_Data']['Car'], list_cars[i]['Car_Data']['Angle'].v)
					end
					if imgui.InputInt(u8'Номер модели',list_cars[i]['Car_Data']['ModelId']) then
						local id_c = 400
						for v = 1,#ID_Cars do
							if list_cars[i]['Car_Data']['ModelId'].v <= ID_Cars[v] then
								id_c = ID_Cars[v]
								break
							end
						end
						list_cars[i]['Car_Data']['ModelId'].v = id_c
						upd_car:run(i)
					end
					if imgui.Button(u8'Редактировать вручную') then
						editmode_car = true
						id_car = i
						deleteCar(list_cars[i]['Car_Data']['Car'])
					end

					imgui.PushItemWidth(-110)

					imgui.Separator()
					local list_tg_m = {u8'До конца'}
					for ltg = 1,#list_name_targets do
						list_tg_m[#list_tg_m+1] = list_name_targets[ltg]
					end
					imgui.Combo(u8'Появление на',list_cars[i]['Car_Data']['StartC'],list_name_targets)
					imgui.Combo(u8'Исчезание после',list_cars[i]['Car_Data']['EndC'],list_tg_m)

					imgui.End()
				end
			end
		end
	end

	-- Окно списка объектов
	if objects_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Объекты', objects_window)

		imgui.PushItemWidth(-10)


		--Список Объектов
		imgui.ListBox('', lb_cur_objects,list_name_objects,15)
		if imgui.BeginPopupContextItem('hee') then
			imgui.InputText('',buf_edit_obj_name)
			if imgui.Button(u8'Применить') and buf_edit_obj_name.v ~= '' then
				list_objects[lb_cur_objects.v+1]['Name'] = buf_edit_obj_name.v
				list_name_objects[lb_cur_objects.v+1] = buf_edit_obj_name.v
				buf_edit_obj_name.v = ''
				imgui.CloseCurrentPopup()
			end
			imgui.PushItemWidth(-1);
			imgui.PopItemWidth();
			imgui.EndPopup();
		end

		--Кнопки редактирования
		if imgui.Button(u8'Добавить') then
			local xx,xy,xz = getCharCoordinates(PLAYER_PED)
			xz = getGroundZFor3dCoord(xx, xy, xz)
			list_objects[#list_objects+1] = {
				['Name'] = u8'Объект #' .. tostring(#list_objects+1),
				['Type'] = imgui.ImInt(-1),
				['Enable'] = imgui.ImBool(false),
				['Object_Data'] = {
					['Pos'] = imgui.ImFloat3(xx,xy,xz),
					['Rotates'] = imgui.ImFloat3(0,0,0),
					['ModelId'] = imgui.ImInt(400),
					['StartC'] = imgui.ImInt(0),
					['EndC'] = imgui.ImInt(0)
				}

			}
			list_name_objects[#list_objects] = list_objects[#list_objects]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Откр/Закр') and #list_objects > 0 then
			list_objects[lb_cur_objects.v+1]['Enable'].v = not list_objects[lb_cur_objects.v+1]['Enable'].v
		end

		imgui.SameLine()
		if imgui.Button(u8'Удалить') then
			deleteObject(list_objects[lb_cur_objects.v+1]['Object_Data']['Obj'])
			list_objects = DelCellArr(list_objects,lb_cur_objects.v+1)
			list_name_objects = DelCellArr(list_name_objects,lb_cur_objects.v+1)
		end
		imgui.End()
		--Редактор объектов
		for i = 1, #list_objects do
			if #list_objects > 0 then
				if list_objects[i]['Enable'].v then
					local resX,resY = getScreenResolution()
					local sizeX,sizeY = 300, 340
					imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
					imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
					imgui.Begin(list_objects[i]['Name'], list_objects[i]['Enable'])
					imgui.PushItemWidth(-90)

					if imgui.InputFloat3(u8'Позиция',list_objects[i]['Object_Data']['Pos']) or imgui.InputFloat3(u8'Угол поворота',list_objects[i]['Object_Data']['Rotates']) then
						local xx,xy,xz = list_objects[i]['Object_Data']['Pos'].v[1],list_objects[i]['Object_Data']['Pos'].v[2],list_objects[i]['Object_Data']['Pos'].v[3]
						local rxx,rxy,rxz = list_objects[i]['Object_Data']['Rotates'].v[1],list_objects[i]['Object_Data']['Rotates'].v[2],list_objects[i]['Object_Data']['Rotates'].v[3]
						setObjectCoordinates(list_objects[i]['Object_Data']['Obj'],xx,xy,xz)
						setObjectRotation(list_objects[i]['Object_Data']['Obj'], rxx, rxy, rxz)
					end
					if imgui.Button(u8'Редактировать вручную') then
						if list_objects[i]['Object_Data']['Obj'] ~= nil then
							editmode_objects = true
							printHelpForever('HOBJ')
							lockPlayerControl(true)
							id_obj = i
							imgui.Process = false
						end
					end
					imgui.InputInt(u8'Номер модели',list_objects[i]['Object_Data']['ModelId'])
					if imgui.Button(u8'Применить') then
						upd_object:run(i)
					end

					imgui.PushItemWidth(-110)

					imgui.Separator()
					local list_tg_m = {u8'До конца'}
					for ltg = 1,#list_name_targets do
						list_tg_m[#list_tg_m+1] = list_name_targets[ltg]
					end
					imgui.Combo(u8'Появление на',list_objects[i]['Object_Data']['StartC'],list_name_targets)
					imgui.Combo(u8'Исчезание после',list_objects[i]['Object_Data']['EndC'],list_tg_m)

					imgui.End()
				end
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
	local angle = list_actors[actorr]['Actor_Data']['Angle'].v

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(1)
	end
	local last_id = 288
	for v = 1,#ID_Actors do
		if modell == ID_Actors[v] then
			last_id = ID_Cars[v-1]
			break
		end
	end
	markModelAsNoLongerNeeded(last_id)

	deleteChar(list_actors[actorr]['Actor_Data']['Char'])
	list_actors[actorr]['Actor_Data']['Char'] = createChar(4,modell,xx,xy,xz)
	setCharHeading(list_actors[actorr]['Actor_Data']['Char'], angle)
	setCharCollision(list_actors[actorr]['Actor_Data']['Char'],false)
end

function update_actor_animation(actorr,anim)
	if not hasAnimationLoaded(Anims['Anim_name'][list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'].v+1]) then
		requestAnimation(Anims['Anim_name'][list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'].v+1])
		while hasAnimationLoaded(Anims['Anim_name'][list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'].v+1]) do
			wait(0)
		end
	end
	local animm = Anims['Anim_list'][list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'].v+1]
	animm = animm[list_actors[actorr]['Actor_Data']['Anims'][anim]['Anim'].v+1]
	taskPlayAnim(list_actors[actorr]['Actor_Data']['Char'], animm, Anims['Anim_name'][list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'].v+1], 1.0, list_actors[actorr]['Actor_Data']['Anims'][anim]['Loop'].v, false, false, false, -1)
end

function update_car(carr)
	local modell = list_cars[carr]['Car_Data']['ModelId'].v
	local xx,xy,xz = list_cars[carr]['Car_Data']['Pos'].v[1], list_cars[carr]['Car_Data']['Pos'].v[2], list_cars[carr]['Car_Data']['Pos'].v[3]
	local angle = list_cars[carr]['Car_Data']['Angle'].v

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(1)
	end
	local last_id = 611
	for v = 1,#ID_Cars do
		if modell == ID_Cars[v] then
			last_id = ID_Cars[v-1]
			break
		end
	end
	markModelAsNoLongerNeeded(last_id)

	deleteCar(list_cars[carr]['Car_Data']['Car'])
	list_cars[carr]['Car_Data']['Car'] = createCar(modell, xx, xy, xz)
	setCarHeading(list_cars[carr]['Car_Data']['Car'], angle)
	setCarCollision(list_cars[carr]['Car_Data']['Car'],false)
	freezeCarPosition(list_cars[carr]['Car_Data']['Car'], true)
	setCarCanBeDamaged(list_cars[carr]['Car_Data']['Car'], false)
	changeCarColour(list_cars[carr]['Car_Data']['Car'], 0, 1)
end

function update_object(objj)
	local modell = list_objects[objj]['Object_Data']['ModelId'].v
	local xx,xy,xz = list_objects[objj]['Object_Data']['Pos'].v[1], list_objects[objj]['Object_Data']['Pos'].v[2], list_objects[objj]['Object_Data']['Pos'].v[3]
	local rxx,rxy,rxz = list_objects[objj]['Object_Data']['Rotates'].v[1], list_objects[objj]['Object_Data']['Rotates'].v[2], list_objects[objj]['Object_Data']['Rotates'].v[3]

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(1)
	end

	deleteObject(list_objects[objj]['Object_Data']['Obj'])
	list_objects[objj]['Object_Data']['Obj'] = createObject(modell, xx, xy, xz)
	setObjectCollision(list_objects[objj]['Object_Data']['Obj'], false)
	setObjectRotation(list_objects[objj]['Object_Data']['Obj'], rxx, rxy, rxz)
end



function main()
	setGxtEntry('HTARG', koder('Двигать: ~r~WASD ~n~~w~Замедлить: ~r~CTRL~w~~n~Выйти: ~r~F'))
	setGxtEntry('HACT', koder('Следующий/Прошлый скин: ~r~I/O~w~~n~Через 10 Следующий/Прошлый: ~r~K/L~w~~n~Выйти: ~r~F'))
	setGxtEntry('HOBJ', koder('Двигать: ~r~WASDQE~w~ ~n~Крутить: ~r~SHIFT + WASDQE~w~~n~Замедлить: ~r~CTRL~w~~n~Выйти: ~r~F'))
	--chr = createObject(1253, 0, 0, 0, 0)

	list_mission_pack = manager.loadMPack()
	for mp = 1,#list_mission_pack do
		list_name_mission_pack[mp] = list_mission_pack[mp]['Name']
	end

	upd_actor = lua_thread.create_suspended(update_actor)
	upd_car = lua_thread.create_suspended(update_car)
	upd_anim_actor = lua_thread.create_suspended(update_actor_animation)
	upd_object = lua_thread.create_suspended(update_object)
	mp.tread_play_actr_anim = lua_thread.create_suspended(mp.play_char_anims)
	while true do
		wait(0)
		removeSphere(r)
		--Редактор позиции объекта
		while editmode_target do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				editmode_target = false
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) then
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) then
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) then
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) then
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
			printHelpForever('HACT')
		end
		while editmode_actor do
			--Закрытие редактора
			wait(0)
			if wasKeyPressed(vkeys.VK_F) then
				clearHelp()
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

		while editmode_objects do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				editmode_objects = false
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
				list_objects[id_obj]['Object_Data']['Pos'].v[2] = list_objects[id_obj]['Object_Data']['Pos'].v[2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				list_objects[id_obj]['Object_Data']['Pos'].v[2] = list_objects[id_obj]['Object_Data']['Pos'].v[2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				list_objects[id_obj]['Object_Data']['Pos'].v[1] = list_objects[id_obj]['Object_Data']['Pos'].v[1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				list_objects[id_obj]['Object_Data']['Pos'].v[1] = list_objects[id_obj]['Object_Data']['Pos'].v[1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				list_objects[id_obj]['Object_Data']['Pos'].v[3] = list_objects[id_obj]['Object_Data']['Pos'].v[3] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				list_objects[id_obj]['Object_Data']['Pos'].v[3] = list_objects[id_obj]['Object_Data']['Pos'].v[3] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					list_objects[id_obj]['Object_Data']['Rotates'].v[2] = list_objects[id_obj]['Object_Data']['Rotates'].v[2] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					list_objects[id_obj]['Object_Data']['Rotates'].v[2] = list_objects[id_obj]['Object_Data']['Rotates'].v[2] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					list_objects[id_obj]['Object_Data']['Rotates'].v[1] = list_objects[id_obj]['Object_Data']['Rotates'].v[1] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					list_objects[id_obj]['Object_Data']['Rotates'].v[1] = list_objects[id_obj]['Object_Data']['Rotates'].v[1] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					list_objects[id_obj]['Object_Data']['Rotates'].v[3] = list_objects[id_obj]['Object_Data']['Rotates'].v[3] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					list_objects[id_obj]['Object_Data']['Rotates'].v[3] = list_objects[id_obj]['Object_Data']['Rotates'].v[3] + 0.2 * multy
				end
			end
			--Отрисовка в редакторе
			local xx,xy,xz = list_objects[id_obj]['Object_Data']['Pos'].v[1],list_objects[id_obj]['Object_Data']['Pos'].v[2],list_objects[id_obj]['Object_Data']['Pos'].v[3]
			local rxx,rxy,rxz = list_objects[id_obj]['Object_Data']['Rotates'].v[1],list_objects[id_obj]['Object_Data']['Rotates'].v[2],list_objects[id_obj]['Object_Data']['Rotates'].v[3]
			setObjectCoordinates(list_objects[id_obj]['Object_Data']['Obj'], xx, xy, xz)
			setObjectRotation(list_objects[id_obj]['Object_Data']['Obj'], rxx, rxy, rxz)
		end
		if editmode_camera then
			displayRadar(false)
			displayHud(false)
		end
		while editmode_camera do
			wait(0)
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end

			if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]+90))
				local coss = math.cos(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]+90))
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] - 0.2 *sinn * multy
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]-90))
				local coss = math.cos(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]-90))
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] - 0.2 *sinn * multy
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]))
				local coss = math.cos(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]))
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] + 0.2 *sinn * multy
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] + 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				local sinn = math.sin(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]))
				local coss = math.cos(math.rad(list_targets[id_target]['Target_Data']['Rotates'].v[2]))
				list_targets[id_target]['Target_Data']['Pos'].v[1] = list_targets[id_target]['Target_Data']['Pos'].v[1] - 0.2 *sinn * multy
				list_targets[id_target]['Target_Data']['Pos'].v[2] = list_targets[id_target]['Target_Data']['Pos'].v[2] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				list_targets[id_target]['Target_Data']['Pos'].v[3] = list_targets[id_target]['Target_Data']['Pos'].v[3] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				list_targets[id_target]['Target_Data']['Pos'].v[3] = list_targets[id_target]['Target_Data']['Pos'].v[3] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					list_targets[id_target]['Target_Data']['Rotates'].v[2] = list_targets[id_target]['Target_Data']['Rotates'].v[2] - 0.5 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					list_targets[id_target]['Target_Data']['Rotates'].v[2] = list_targets[id_target]['Target_Data']['Rotates'].v[2] + 0.5 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					list_targets[id_target]['Target_Data']['Rotates'].v[1] = list_targets[id_target]['Target_Data']['Rotates'].v[1] - 0.5 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					list_targets[id_target]['Target_Data']['Rotates'].v[1] = list_targets[id_target]['Target_Data']['Rotates'].v[1] + 0.5 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					list_targets[id_target]['Target_Data']['Rotates'].v[3] = list_targets[id_target]['Target_Data']['Rotates'].v[3] - 0.5 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					list_targets[id_target]['Target_Data']['Rotates'].v[3] = list_targets[id_target]['Target_Data']['Rotates'].v[3] + 0.2 * multy
				end
			end

			local xx,xy,xz = list_targets[id_target]['Target_Data']['Pos'].v[1],list_targets[id_target]['Target_Data']['Pos'].v[2],list_targets[id_target]['Target_Data']['Pos'].v[3]
			local rxx,rxy,rxz = list_targets[id_target]['Target_Data']['Rotates'].v[1],list_targets[id_target]['Target_Data']['Rotates'].v[2],list_targets[id_target]['Target_Data']['Rotates'].v[3]
			local x1,y1,z1 = xx,xy,xz
			local dx,dy,dz = xx+2-x1,xy+2-y1,xz+2-z1
			x1 = x1 + dx*math.sin(math.rad(rxy))
			y1 = y1 + dy*math.cos(math.rad(rxy))
			z1 = z1
			dx,dy,dz = xx+2-x1,xy+2-y1,xz+2-z1
			x1 = x1 + dx*math.sin(math.rad(rxy))
			y1 = y1 + dy*math.cos(math.rad(rxy))
			z1 = z1 + dz*math.cos(math.rad(rxx))

			--setObjectCoordinates(chr, x1, y1, z1)
			setFixedCameraPosition(xx, xy, xz,0,rxz,0)
			pointCameraAtPoint(x1, y1, z1, 2)
			setCinemaCamera(false)

			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				restoreCamera()
				editmode_camera = false
				displayRadar(true)
				displayHud(true)
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
			for c = 1,#list_cars do
				deleteCar(list_cars[c]['Car_Data']['Car'])
			end
			mp.start_mission(list_targets,list_actors,list_cars,list_objects,name_mission)
			miss_start = false
		end

		while not mp.miss == 0 do
			wait(0)
		end

		--Откр/Закр. Меню
		if wasKeyPressed(vkeys.VK_U) then
			main_window.v = not main_window.v
			targets_window.v = false
			actors_window.v = false
			cars_window.v = false
			objects_window.v = false
			missions_window.v = false
			pack_mission_window.v = false

		end

		imgui.Process = main_window.v or targets_window.v or actors_window.v or cars_window.v or objects_window.v or missions_window.v or pack_mission_window.v

		-- Дебаг: позиция
		if wasKeyPressed(vkeys.VK_Q) then
			px,py,pz = getCharCoordinates(PLAYER_PED)
			ang = getCharHeading(PLAYER_PED)
			print(px .. ' ' .. py .. ' ' .. pz .. ' ' .. ang)
		end
	end
end
