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

Number_Weapons = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46}

Anims = {
	['Anim_name'] = {'PED','BOMBER','POOL','ON_LOOKERS','GANGS','PAULNMAC','BOX'},
	['Anim_list'] = {{"IDLE_CHAT","ROADCROSS","ATM","FLEE_LKAROUND_01","FUCKU","walk_armed","seat_up","run_armed","idle_gang1","hit_back","hit_r","hit_wall","hit_l","hita_2","climb_pull","bomber","floor_hit_f","fightshb","fall_fall","drown","floor_hit","IDLE_taxi","swim_tread","XPRESSSCRATCH","XPRESSSCRATCH","HANDSUP","KO_SHOT_STOM","KO_SKID_FRONT","DUCK_COWER","GETUP","SEAT_DOWN","ENDCHAT_03","GETUP_FRONT","GUN_STAND","KO_SPIN_L","KO_SKID_BACK","WALK_DRUNK","FALL_SKYDIVE","FALL_FRONT","RUN_PLAYER","WOMAN_IDLESTANCE","TAP_HAND","CAR_SIT","IDLE_STANCE","COWER","CROUCH_ROLL_R","CROUCH_ROLL_L","ENDCHAT_01","KO_SHOT_FACE","PHONE_TALK","KO_SHOT_FRONT","IDLE_ARMED","WEAPON_CROUCH","IDLE_TIRED","SEAT_IDLE","HANDSCOWER","FIGHTIDLE","FIGHTA_G","CAR_HOOKERTALK","HANDSUP","abseil","FIGHTA_M","FIGHTA_block","FIGHTA_2","gas_cwr","ENDCHAT_02"},
	{"BOM_PLANT_IN","BOM_PLANT_LOOP","BOM_PLANT_CROUCH_IN","BOM_PLANT_CROUCH_OUT","BOM_PLANT_2IDLE"},
	{"POOL_XLONG_SHOT","POOL_XLONG_START","POOL_LONG_SHOT","POOL_LONG_START","POOL_MED_START","POOL_MED_SHOT","POOL_SHORT_SHOT","POOL_CHALKCUE"},
	{"LKUP_LOOP","POINTUP_IN","SHOUT_01"},
	{"PRTIAL_GNGTLKA","PRTIAL_GNGTLKF","PRTIAL_GNGTLKD","PRTIAL_GNGTLKE","HNDSHKFA_SWT", "GANGS","PRTIAL_GNGTLKH","HNDSHKFA","DRUGS_BUY","PRTIAL_GNGTLKC","PRTIAL_GNGTLKH","PRTIAL_GNGTLKG","DRNKBR_PRTL","PRTIAL_GNGTLKB","PRTIAL_HNDSHK_01","DEALER_DEAL"},
	{"PISS_IN","PISS_LOOP","PISS_OUT","PNM_LOOP_A","PNM_ARGUE2_A","PNM_ARGUE1_A"},
	{"boxhipin","bxwlko","catch_box","bxshwlk","bxhwlki","boxshup","boxhipup","boxshdwn","bxhipwlk","bxshwlki"}
	}
}

Weather_str = {u8'Очень солнечно (LS)',u8'Солнечно (LS)',u8'Очень солнечно, смог (LS)',u8'Солнечно, смог (LS)',u8'Облака (LS)',u8'Солнечно (SF)',u8'Очень солнечно (SF)',u8'Облака (SF)',u8'Дождливая (SF)',u8'Туманная (SF)',u8'Солнечно (LV)',u8'Очень солнечно (LV)',u8'Облака (LV)',u8'Очень солнечно (CS)',u8'Солнечно (CS)',u8'Облака (CS)',u8'Дождливая (CS)',u8'Очень солнечно (Des)',u8'Солнечно (Des)',u8'Песчаная буря (Des)',u8'Подводная', u8'Дополнительно 1',u8'Дополнительно 2'}

local mission_data = {
	['Name'] = '',
	['Time'] = {0,0},
	['Weather'] = imgui.ImInt(0),
	['Riot'] = imgui.ImBool(false),
	['Player'] = {
		['Pos'] = {2488,-1666,13},
		['Angle'] = 0,
		['ModelId'] = 0,
		['Weapon'] = 0,
		['Weap_ammo'] = 0,
		['Interior_id'] = 0
	}
}
local buf_edit_targets_name = imgui.ImBuffer(64)
local buf_edit_actors_name = imgui.ImBuffer(64)
local buf_edit_cars_name = imgui.ImBuffer(64)
local buf_edit_obj_name = imgui.ImBuffer(64)
local buf_edit = imgui.ImFloat(0)
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
local info_window = imgui.ImBool(false)
local lb_cur_targets = imgui.ImInt(0)
local lb_cur_actors = imgui.ImInt(0)
local lb_cur_cars = imgui.ImInt(0)
local lb_cur_objects = imgui.ImInt(0)
local lb_cur_pack_mission = imgui.ImInt(0)
local lb_cur_missions = imgui.ImInt(0)
local buffer_target = -1
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
local editmode_timemiss = false
local editmode_timetarg = false
local editmode_player = false
local editmode_teleport_player = false
local editmode_preview_player_anim = false
local editmode_car = false
local id_target = 0;
local id_actor = 0;
local id_car = 0;
local id_obj = 0;
local id_timetarg = 0
local id_teleport_player = 0
local id_preview_player_anim = 0
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
		if imgui.Button(u8'Игрок') then
			editmode_player = true
			printHelpForever('HPLA')
			imgui.Process = false
		end
		imgui.Separator()
		if imgui.Button(u8'Паки миссий') then
			pack_mission_window.v = not pack_mission_window.v
			--manager_miss.save(list_targets,list_actors,list_cars,list_objects)
			main_window.v = false
		end
		if imgui.Button(u8'Миссии') then
			missions_window.v = not missions_window.v
			main_window.v = false
		end
		imgui.Separator()
		if imgui.Button(u8'Запустить миссию') then
			miss_start = true
		end
		imgui.Separator()
		if imgui.Button(u8'Инфо') then
			main_window.v = false
			info_window.v = true
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
			mission_data['Name'] = u8'Миссия #' .. tostring(#list_missions)
			list_missions[#list_missions+1] = {
				['Name'] = mission_data['Name'],
				['Prename'] = imgui.ImBuffer(256),
				['Enable'] = imgui.ImBool(false),
				['Mission_Data'] = {
					['Targets'] = list_targets,
					['Actors'] = list_actors,
					['Cars'] = list_cars,
					['Objects'] = list_objects,
					['Miss_data'] = mission_data
				}
			}
			list_name_missions[#list_missions] = list_missions[#list_missions]['Name']
		end

		imgui.SameLine()
		if imgui.Button(u8'Загрузить') and #list_missions > 0 then
			lua_thread.create(load_mission)
		end

		imgui.SameLine()
		if imgui.Button(u8'Сохранить') and #list_missions > 0 then
			list_missions[lb_cur_missions.v+1]['Mission_Data'] = {
				['Targets'] = list_targets,
				['Actors'] = list_actors,
				['Cars'] = list_cars,
				['Objects'] = list_objects,
				['Miss_data'] = mission_data
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
		-- Настройки
		for t = 1,#list_missions do
			if list_missions[t]['Enable'].v then
				local resX,resY = getScreenResolution()
				local sizeX,sizeY = 300, 340
				imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
				imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
				imgui.Begin(u8'Настройки: ' .. list_missions[t]['Name'], list_missions[t]['Enable'])
				imgui.PushItemWidth(-120)
				imgui.InputText(u8'Название миссии',list_missions[t]['Prename'])
				if imgui.Button(u8'Применить') then
					list_missions[t]['Name'] = list_missions[t]['Prename'].v
					list_name_missions[t] = list_missions[t]['Name']
					mission_data['Name'] = list_missions[t]['Name']
				end
				imgui.Separator()
				if imgui.Button(u8'Игровое время миссии') then
					editmode_timemiss = true
					printHelpForever('HMTIM')
					imgui.Process = false
				end

				if imgui.Combo(u8'Погода',mission_data['Weather'],Weather_str) then
					forceWeatherNow(mission_data['Weather'].v)
				end
				imgui.Checkbox(u8'Режим бунта',mission_data['Riot'])
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
		local sizeX,sizeY = 300, 360
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
		if imgui.Button(u8'Вырезать') and #list_targets > 0 then
			buffer_target = lb_cur_targets.v + 1
		end
		imgui.SameLine()
		if buffer_target ~= -1 then
			if imgui.Button(u8'Вставить') then
				list_targets = MoveCellArr(list_targets,buffer_target,lb_cur_targets.v+1)
				list_name_targets = MoveCellArr(list_name_targets,buffer_target,lb_cur_targets.v+1)
				buffer_target = -1
			end
		end

		imgui.End()
		--Редактор цели
		for i = 1, #list_targets do
			if #list_targets > 0 then
				if list_targets[i]['Enable'].v then
					local resX,resY = getScreenResolution()
					local sizeX,sizeY = 300, 340
					local targets_list_arr = {u8'Чекпоинт',u8'Сесть в машину',u8'Убить Актёра',u8'Окр. среда',u8'Объект',u8'Игрок'}
					local targets_marker_color = {u8'Красный',u8'Зелёный',u8'Свело-голубой',u8'Белый',u8'Жёлтый'}
					local weap_names = {u8'Кулак',u8'Кастет',u8'Клюшка для гольфа',u8'Полицейская дубинка',u8'Нож',u8'Бейсбольная бита',u8'Лопата',u8'Кий',u8'Катана',u8'Бензопила',u8'Двухсторонний дилдо',u8'Дилдо',u8'Вибратор',u8'Серебряный вибратор',u8'Букет цветов',u8'Трость',u8'Граната',u8'Слезоточивый газ',u8'Коктейль Молотова',u8'Пистолет 9мм',u8'Пистолет 9мм с глушителем',u8'Пистолет Дезерт Игл',u8'Обычный дробовик',u8'Обрез',u8'Скорострельный дробовик',u8'Узи',u8'MP5',u8'Автомат Калашникова',u8'Винтовка M4',u8'Tec-9',u8'Охотничье ружье',u8'Снайперская винтовка',u8'РПГ',u8'Самонаводящиеся ракеты HS',u8'Огнемет',u8'Миниган',u8'Сумка с тротилом',u8'Детонатор к сумке',u8'Баллончик с краской',u8'Огнетушитель',u8'Фотоаппарат',u8'Прибор ночного видения',u8'Тепловизор',u8'Парашют'}
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
							list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
						end
						if list_targets[i]['Type'].v == 2 then
							list_targets[i]['Target_Data']['Target_actor_id'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Color_blip'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
						end
						if list_targets[i]['Type'].v == 3 then
							list_targets[i]['Target_Data']['Target_type'] = imgui.ImInt(-1)
							list_targets[i]['Target_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
							list_targets[i]['Target_Data']['Rotates'] = imgui.ImFloat3(0,0,0)
							list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
							list_targets[i]['Target_Data']['Text_time'] = imgui.ImFloat(2)
							list_targets[i]['Target_Data']['Smooth'] = imgui.ImBool(false)
							list_targets[i]['Target_Data']['Time'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Weather'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Clock_time'] = {0,0}
							list_targets[i]['Target_Data']['Traffic'] = {imgui.ImInt(1),imgui.ImInt(1)}
						end
						if list_targets[i]['Type'].v == 4 then
							list_targets[i]['Target_Data']['Target_object_id'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Color_blip'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Target_type'] = imgui.ImInt(-1)
							list_targets[i]['Target_Data']['Weap'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Text'] = imgui.ImBuffer(128)
						end
						if list_targets[i]['Type'].v == 5 then
							list_targets[i]['Target_Data']['Target_type'] = imgui.ImInt(-1)
							list_targets[i]['Target_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
							list_targets[i]['Target_Data']['ModelID'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Angle'] = imgui.ImFloat(0)
							list_targets[i]['Target_Data']['Weapon'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Weap_ammo'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Anim'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Pack_anim'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Loop'] = imgui.ImBool(false)
							list_targets[i]['Target_Data']['Car_id'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Car_place'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Level_battue'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Dialog'] = {}
							list_targets[i]['Target_Data']['Dialog_id'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Add_money'] = imgui.ImInt(0)
							list_targets[i]['Target_Data']['Interior_id'] = getActiveInterior()
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
						imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Text'])
					end
					if list_targets[i]['Type'].v == 2 then
						imgui.Combo(u8'Актёры',list_targets[i]['Target_Data']['Target_actor_id'],list_name_actors)
						imgui.Combo(u8'Цвет маркера',list_targets[i]['Target_Data']['Color_blip'],targets_marker_color)
						imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Text'])
					end

					if list_targets[i]['Type'].v == 3 then
						local target_type = {u8'Катсцена',u8'Обратный отсчёт',u8'Тайм-аут',u8'Погода',u8'Время',u8'Траффик Педов/Машин'}
						imgui.Combo(u8'Тип',list_targets[i]['Target_Data']['Target_type'],target_type)
						if list_targets[i]['Target_Data']['Target_type'].v == 0 then
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
						if list_targets[i]['Target_Data']['Target_type'].v == 1 then
							imgui.InputInt(u8'Время',list_targets[i]['Target_Data']['Time'])
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 2 then
							imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Text'])
							imgui.InputInt(u8'Тайм-аут',list_targets[i]['Target_Data']['Time'])
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 3 then
							if imgui.Combo(u8'Погода',list_targets[i]['Target_Data']['Weather'],Weather_str) then
								forceWeatherNow(list_targets[i]['Target_Data']['Weather'].v)
							end
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 4 then
							if imgui.Button(u8'Игровое время миссии') then
								editmode_timetarg = true
								printHelpForever('HMTIM')
								imgui.Process = false
								id_timetarg = i
							end
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 5 then
							local count = {u8'Нет',u8'Обычное',u8'Много'}
							imgui.PushItemWidth(-100)
							imgui.Combo(u8'Кол-во педов',list_targets[i]['Target_Data']['Traffic'][1],count)
							imgui.Combo(u8'Кол-во машин',list_targets[i]['Target_Data']['Traffic'][2],count)
						end
					end
					if list_targets[i]['Type'].v == 4 then
						local target_type = {u8'Прикоснуться',u8'Повредить',u8'Сфотографровать',u8'Повредить оружием'}
						imgui.Combo(u8'Объект',list_targets[i]['Target_Data']['Target_object_id'],list_name_objects)
						imgui.Combo(u8'Тип',list_targets[i]['Target_Data']['Target_type'],target_type)
						if list_targets[i]['Target_Data']['Target_type'].v == 3 then
							imgui.Combo(u8'Оружие',list_targets[i]['Target_Data']['Weap'],weap_names)
						end
						imgui.Combo(u8'Цвет маркера',list_targets[i]['Target_Data']['Color_blip'],targets_marker_color)
						imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Text'])
					end

					if list_targets[i]['Type'].v == 5 then
						local target_type = {u8'Телепорт',u8'Анимация игрока',u8'Телепорт в машину',u8'Уровень розыска',u8'Убрать оружие',u8'Разговор по телефону',u8'Добавить денег'}
						imgui.Combo(u8'Тип',list_targets[i]['Target_Data']['Target_type'],target_type)
						if list_targets[i]['Target_Data']['Target_type'].v == 0 then
							imgui.InputFloat3(u8'Позиция',list_targets[i]['Target_Data']['Pos'])
							imgui.DragFloat(u8'Угол',list_targets[i]['Target_Data']['Angle'],0.5,-360,360)
							imgui.InputInt(u8'Модель',list_targets[i]['Target_Data']['ModelID'])
							imgui.PushItemWidth(-100)
							imgui.InputInt(u8'Кол-во патронов',list_targets[i]['Target_Data']['Weap_ammo'],10)
							if imgui.Button(u8'Редактировать вручную') then
								id_teleport_player = i
								imgui.Process = false
								printHelpForever('HPLA')
								editmode_teleport_player = true
							end
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 1 then
							imgui.Combo(u8'Пак',list_targets[i]['Target_Data']['Pack_anim'],Anims['Anim_name'])
							imgui.Combo(u8'Анимация',list_targets[i]['Target_Data']['Anim'],Anims['Anim_list'][list_targets[i]['Target_Data']['Pack_anim'].v+1])
							imgui.Checkbox(u8'Зацикленно',list_targets[i]['Target_Data']['Loop'])
							if imgui.Button(u8'Предпросмотр') then
								id_preview_player_anim = i
								editmode_preview_player_anim = true
								lockPlayerControl(true)
								imgui.Process = false
								printHelpForever('HVIEW')
							end
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 2 then
							local place_car = {u8'Место водителя',u8'Место пассажира',u8'Заднее левое место',u8'Заднее правое место'}
							imgui.Combo(u8'Машина',list_targets[i]['Target_Data']['Car_id'],list_name_cars)
							imgui.Combo(u8'Место',list_targets[i]['Target_Data']['Car_place'],place_car)
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 3 then
							imgui.PushItemWidth(-100)
							imgui.SliderInt(u8'Уровень розыска',list_targets[i]['Target_Data']['Level_battue'],0,6)
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 5 then
							imgui.Text(u8'Диалог')
							if imgui.Button(u8'Добавить') then
								list_targets[i]['Target_Data']['Dialog'][#list_targets[i]['Target_Data']['Dialog']+1] = {
									['Text'] = imgui.ImBuffer(128),
									['Text_time'] = imgui.ImFloat(0)
								}
							end
							imgui.PushItemWidth(-150)
							imgui.SameLine()
							imgui.Combo(u8'Номер',list_targets[i]['Target_Data']['Dialog_id'],range(#list_targets[i]['Target_Data']['Dialog']))
							imgui.SameLine()
							if imgui.Button(u8'Удалить') then
								list_targets[i]['Target_Data']['Dialog'] = DelCellArr(list_targets[i]['Target_Data']['Dialog'],list_targets[i]['Target_Data']['Dialog_id'].v+1)
							end
							imgui.PushItemWidth(-70)
							for d = 1,#list_targets[i]['Target_Data']['Dialog'] do
								if imgui.TreeNode(u8:encode(d)) then
									imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Dialog'][d]['Text'])
									imgui.InputFloat(u8'Время',list_targets[i]['Target_Data']['Dialog'][d]['Text_time'])
									imgui.TreePop()

								end
							end
							--imgui.InputText(u8'Текст',list_targets[i]['Target_Data']['Text'])
						end
						if list_targets[i]['Target_Data']['Target_type'].v == 6 then
							imgui.InputInt(u8'Добавить',list_targets[i]['Target_Data']['Add_money'])
						end
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
					['Anims'] = {},
					['Anim_id'] = imgui.ImInt(0)
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
		-- if imgui.Button(u8'Дублировать') then
		-- 	list_actors[#list_actors+1] = {
		-- 		['Name'] = list_actors[lb_cur_actors.v+1]['Name'] .. u8'д',
		-- 		['Type'] = imgui.ImInt(list_actors[lb_cur_actors.v+1]['Type'].v),
		-- 		['Enable'] = imgui.ImBool(list_actors[lb_cur_actors.v+1]['Enable'].v),
		-- 		['Actor_Data'] = {
		-- 			['Pos'] = imgui.ImFloat3(list_actors[lb_cur_actors.v+1]['Actor_Data']['Pos'].v[1],list_actors[lb_cur_actors.v+1]['Actor_Data']['Pos'].v[2],list_actors[lb_cur_actors.v+1]['Actor_Data']['Pos'].v[3]),
		-- 			['Angle'] = imgui.ImFloat(list_actors[lb_cur_actors.v+1]['Actor_Data']['Angle'].v),
		-- 			['ModelId'] = imgui.ImInt(list_actors[lb_cur_actors.v+1]['Actor_Data']['ModelId'].v),
		-- 			['StartC'] = imgui.ImInt(list_actors[lb_cur_actors.v+1]['Actor_Data']['StartC'].v),
		-- 			['EndC'] = imgui.ImInt(list_actors[lb_cur_actors.v+1]['Actor_Data']['EndC'].v),
		-- 			['Anims'] = {}
		-- 		}
		-- 	}
		-- 	list_name_actors[#list_actors] = list_actors[lb_cur_actors.v+1]['Name']
		-- end

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
							['Condition'] = imgui.ImInt(0),
							['Unbreakable'] = imgui.ImBool(false)
						}
					end
					imgui.SameLine()
					imgui.PushItemWidth(-250)
					imgui.Combo(u8'Номер',list_actors[i]['Actor_Data']['Anim_id'],range(#list_actors[i]['Actor_Data']['Anims']))
					imgui.SameLine()
					if imgui.Button(u8'Удалить') then
						list_actors[i]['Actor_Data']['Anims'] = DelCellArr(list_actors[i]['Actor_Data']['Anims'],list_actors[i]['Actor_Data']['Anim_id'].v+1)
					end
					if imgui.Button(u8'Очистить предпросмотр') then
						taskPlayAnim(list_actors[i]['Actor_Data']['Char'], "WALK_START", 'PED', 1.0, false, false, false, false, -1)
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
							imgui.Checkbox(u8'Неуязвим',list_actors[i]['Actor_Data']['Anims'][a]['Unbreakable'])
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
			if imgui.Button(u8'Применить') and buf_edit_cars_name.v ~= '' then
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
						imgui.Process = false
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
			print(list_objects[#list_objects]['Object_Data']['Pos'].v[3])
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

	if info_window.v then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 200, 400
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
		imgui.Begin(u8'Инфо', info_window)
		imgui.Text(u8'Автор:')
		imgui.SameLine()
		imgui.TextColored(imgui.ImVec4(1.0,0.0,0.0,1.0),'SKIC')
		imgui.Text(u8'Версия:')
		imgui.SameLine()
		imgui.Text('Alpha 0.5.4')

		imgui.End()
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

function range(r)
	local arr = {}
	for i = 1,r do
		arr[#arr+1] = tostring(i)
	end
	return arr
end

-- Перемещение ячейки
function MoveCellArr(arr,i,j)
	local array = {}
	for v,k in ipairs(arr) do
		if v ~= i then
			array[#array+1] = k
		end
		if v == j then
			array[#array+1] = arr[i]
		end
	end
	return array
end

function deepcopymod1(obj)
  local lookup_table = {}
  local function _copy(object)
    if type(object) == "function" then
      return loadstring(string.dump(object))
    elseif type(object) ~= "table" then
      return object
    elseif lookup_table[object] then
      return lookup_table[object]
    end
    local new_table = {}
    lookup_table[object] = new_table
    for index, value in pairs(object) do
      new_table[_copy(index)] = _copy(value)
    end
    return setmetatable(new_table, _copy(getmetatable(object)))
  end
  return _copy(obj)
end

function update_actor(actorr)
	local modell = list_actors[actorr]['Actor_Data']['ModelId'].v
	local xx,xy,xz = list_actors[actorr]['Actor_Data']['Pos'].v[1], list_actors[actorr]['Actor_Data']['Pos'].v[2], list_actors[actorr]['Actor_Data']['Pos'].v[3]
	local angle = list_actors[actorr]['Actor_Data']['Angle'].v

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
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
		while not hasAnimationLoaded(Anims['Anim_name'][list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'].v+1]) do
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
		wait(0)
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
		wait(0)
	end

	deleteObject(list_objects[objj]['Object_Data']['Obj'])
	list_objects[objj]['Object_Data']['Obj'] = createObject(modell, xx, xy, xz)
	setObjectCoordinates(list_objects[objj]['Object_Data']['Obj'], xx, xy, xz)
	setObjectCollision(list_objects[objj]['Object_Data']['Obj'], false)
	setObjectRotation(list_objects[objj]['Object_Data']['Obj'], rxx, rxy, rxz)
end

function load_mission()
	for i = 1,#list_actors do
		deleteChar(list_actors[i]['Actor_Data']['Char'])
	end
	for c = 1,#list_cars do
		deleteCar(list_cars[c]['Car_Data']['Car'])
	end
	for o = 1,#list_objects do
		deleteObject(list_objects[o]['Object_Data']['Obj'])
	end
	list_targets = list_missions[lb_cur_missions.v+1]['Mission_Data']['Targets']
	list_actors = list_missions[lb_cur_missions.v+1]['Mission_Data']['Actors']
	list_cars = list_missions[lb_cur_missions.v+1]['Mission_Data']['Cars']
	list_objects = list_missions[lb_cur_missions.v+1]['Mission_Data']['Objects']
	mission_data = list_missions[lb_cur_missions.v+1]['Mission_Data']['Miss_data']
	wait(1)
	for a = 1, #list_actors do
		list_name_actors[a] = list_actors[a]['Name']
		upd_actor:run(a)
	end
	for o = 1, #list_objects do
		list_name_objects[o] = list_objects[o]['Name']
		upd_object:run(o)
	end
	for c = 1, #list_cars do
		list_name_cars[c] = list_cars[c]['Name']
		upd_car:run(c)
	end
	for t = 1, #list_targets do
		list_name_targets[t] = list_targets[t]['Name']
	end
	setCharCoordinates(PLAYER_PED, mission_data['Player']['Pos'][1], mission_data['Player']['Pos'][2], mission_data['Player']['Pos'][3])
	setCharInterior(PLAYER_PED, mission_data['Player']['Interior_id'])
	setInteriorVisible(mission_data['Player']['Interior_id'])
end

function main()
	setGxtEntry('HTARG', koder('Двигать: ~r~WS/AD ~n~~w~Замедлить: ~r~CTRL~w~~n~Выйти: ~r~F'))
	setGxtEntry('HACT', koder('Следующий/Прошлый скин: ~r~I/O~w~~n~Через 10 Следующий/Прошлый: ~r~K/L~w~~n~Выйти: ~r~F'))
	setGxtEntry('HOBJ', koder('Двигать: ~r~AD/WS/QE~w~ ~n~Крутить: ~r~SHIFT + AD/WS/QE~w~~n~Замедлить: ~r~CTRL~w~~n~Выйти: ~r~F'))
	setGxtEntry('HMTIM', koder('Часы: ~r~I/O~w~~n~Минуты: ~r~K/L~w~~n~Выйти: ~r~F'))
	setGxtEntry('HPLA', koder('Следующий/Прошлый скин: ~r~I/O~w~~n~Через 10 Следующий/Прошлый: ~r~K/L~w~~n~Следующие/Прошлое оружие: ~r~P/[~n~~w~Применить: ~r~Space~n~~w~Выйти: ~r~F'))
	setGxtEntry('HPLA2', koder('Больше/Меньше патронов: ~r~I/O~n~~w~Применить: ~r~Space~n~~w~Выйти: ~r~F'))
	setGxtEntry('HVIEW',koder('Выйти: ~r~F'))
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
			list_targets[id_target]['Target_Data']['Pos'].v[3] = getGroundZFor3dCoord(xx, xy, xz+2)
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
				if list_actors[id_actor]['Actor_Data']['ModelId'].v < 0 then
					list_actors[id_actor]['Actor_Data']['ModelId'].v = 288
				end
				local id_a = 288
				for v = 1,#ID_Actors do
					if list_actors[id_actor]['Actor_Data']['ModelId'].v < ID_Actors[v] then
						id_a = ID_Actors[v-1]
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
				if list_actors[id_actor]['Actor_Data']['ModelId'].v < 0 then
					list_actors[id_actor]['Actor_Data']['ModelId'].v = 289 + list_actors[id_actor]['Actor_Data']['ModelId'].v
				end
				local id_a = 288
				for v = 1,#ID_Actors do
					if list_actors[id_actor]['Actor_Data']['ModelId'].v < ID_Actors[v] then
						id_a = ID_Actors[v-1]
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
				multy = 0.2
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
			print(list_objects[id_obj]['Object_Data']['Pos'].v[3])
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
					list_targets[id_target]['Target_Data']['Rotates'].v[2] = list_targets[id_target]['Target_Data']['Rotates'].v[2] - 2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					list_targets[id_target]['Target_Data']['Rotates'].v[2] = list_targets[id_target]['Target_Data']['Rotates'].v[2] + 2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					list_targets[id_target]['Target_Data']['Rotates'].v[1] = list_targets[id_target]['Target_Data']['Rotates'].v[1] - 2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					list_targets[id_target]['Target_Data']['Rotates'].v[1] = list_targets[id_target]['Target_Data']['Rotates'].v[1] + 2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					list_targets[id_target]['Target_Data']['Rotates'].v[3] = list_targets[id_target]['Target_Data']['Rotates'].v[3] - 2 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					list_targets[id_target]['Target_Data']['Rotates'].v[3] = list_targets[id_target]['Target_Data']['Rotates'].v[3] + 2 * multy
				end
			end

			local xx,xy,xz = list_targets[id_target]['Target_Data']['Pos'].v[1],list_targets[id_target]['Target_Data']['Pos'].v[2],list_targets[id_target]['Target_Data']['Pos'].v[3]
			local rxx,rxy,rxz = list_targets[id_target]['Target_Data']['Rotates'].v[1],list_targets[id_target]['Target_Data']['Rotates'].v[2],list_targets[id_target]['Target_Data']['Rotates'].v[3]
			local x1,y1,z1 = xx,xy,xz
			x1 = x1 + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
			y1 = y1 + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
			z1 = z1 + 2*math.cos(math.rad(rxx))

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

		while editmode_timemiss do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				editmode_timemiss = false
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				mission_data['Time'][1] = mission_data['Time'][1] + 1
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				mission_data['Time'][1] = mission_data['Time'][1] - 1
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				mission_data['Time'][2] = mission_data['Time'][2] + 1
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				mission_data['Time'][2] = mission_data['Time'][2] - 1
			end
			if mission_data['Time'][2] == 60 then
				mission_data['Time'][1] = mission_data['Time'][1] + 1
				mission_data['Time'][2] = 0
			elseif mission_data['Time'][2] == -1 then
				mission_data['Time'][1] = mission_data['Time'][1] - 1
				mission_data['Time'][2] = 59
			end
			if mission_data['Time'][1] == 24 then
				mission_data['Time'][1] = 0
			elseif mission_data['Time'][1] == -1 then
				mission_data['Time'][1] = 23
			end
			setTimeOfDay(mission_data['Time'][1], mission_data['Time'][2])
		end

		while editmode_timetarg do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				editmode_timetarg = false
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				list_targets[id_timetarg]['Target_Data']['Clock_time'][1] = list_targets[id_timetarg]['Target_Data']['Clock_time'][1] + 1
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				list_targets[id_timetarg]['Target_Data']['Clock_time'][1] = list_targets[id_timetarg]['Target_Data']['Clock_time'][1] - 1
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				list_targets[id_timetarg]['Target_Data']['Clock_time'][2] = list_targets[id_timetarg]['Target_Data']['Clock_time'][2] + 1
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				list_targets[id_timetarg]['Target_Data']['Clock_time'][2] = list_targets[id_timetarg]['Target_Data']['Clock_time'][2] - 1
			end
			if list_targets[id_timetarg]['Target_Data']['Clock_time'][2] == 60 then
				list_targets[id_timetarg]['Target_Data']['Clock_time'][1] = list_targets[id_timetarg]['Target_Data']['Clock_time'][1] + 1
				list_targets[id_timetarg]['Target_Data']['Clock_time'][2] = 0
			elseif list_targets[id_timetarg]['Target_Data']['Clock_time'][2] == -1 then
				list_targets[id_timetarg]['Target_Data']['Clock_time'][1] = list_targets[id_timetarg]['Target_Data']['Clock_time'][1] - 1
				list_targets[id_timetarg]['Target_Data']['Clock_time'][2] = 59
			end
			if list_targets[id_timetarg]['Target_Data']['Clock_time'][1] == 24 then
				list_targets[id_timetarg]['Target_Data']['Clock_time'][1] = 0
			elseif list_targets[id_timetarg]['Target_Data']['Clock_time'][1] == -1 then
				list_targets[id_timetarg]['Target_Data']['Clock_time'][1] = 23
			end
			setTimeOfDay(list_targets[id_timetarg]['Target_Data']['Clock_time'][1], list_targets[id_timetarg]['Target_Data']['Clock_time'][2])
		end

		if editmode_player then
			local stage = 0
			local modelID = 0
			local weapon = 0
			local ammo = 0
			while editmode_player do
				--Закрытие редактора
				wait(0)
				if wasKeyPressed(vkeys.VK_F) then
					clearHelp()
					editmode_player = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					removeAllCharWeapons(PLAYER_PED)
					lockPlayerControl(false)
				end
				if wasKeyPressed(vkeys.VK_SPACE) then
					if stage == 0 then
						lockPlayerControl(true)
						printHelpForever('HPLA2')
						stage = stage + 1
					elseif stage == 1 then
						clearHelp()
						editmode_player = false
						setPlayerModel(PLAYER_HANDLE, model.NULL)
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						local angle = getCharHeading(PLAYER_PED)
						xz = getGroundZFor3dCoord(xx,xy,xz)
						mission_data['Player']['Pos'] = {xx,xy,xz}
						mission_data['Player']['Angle'] = angle
						mission_data['Player']['ModelId'] = modelID
						mission_data['Player']['Weapon'] = weapon
						mission_data['Player']['Weap_ammo'] = ammo
						mission_data['Player']['Interior_id'] = getActiveInterior()
						removeAllCharWeapons(PLAYER_PED)
						lockPlayerControl(false)
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						modelID = modelID + 1
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						modelID = modelID - 1
						if modelID < 0 then
							modelID = 288
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						modelID = modelID + 10
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						modelID = modelID - 10
						if modelID < 0 then
							modelID = 289 + modelID
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						local id_w = 0
						for v = 1,#Number_Weapons do
							if weapon <= Number_Weapons[v] then
								id_w = Number_Weapons[v]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v-1]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						print(weapon)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 0 then
							weapon = 46
						end
						local id_w = 46
						for v = 1,#Number_Weapons do
							if weapon < Number_Weapons[v] then
								id_w = Number_Weapons[v-1]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
				elseif stage == 1 then
					if isKeyDown(vkeys.VK_I) then
						wait(50)
						ammo = ammo + 10
					end
					if isKeyDown(vkeys.VK_O) and ammo > 0 then
						wait(50)
						ammo = ammo - 10
					end
					printString(koder('Патронов: ' .. '~r~' .. ammo), 0)
				end
			end
		end

		if editmode_teleport_player then
			local stage = 0
			local modelID = list_targets[id_teleport_player]['Target_Data']['ModelID'].v
			local weapon = list_targets[id_teleport_player]['Target_Data']['Weapon'].v
			local ammo = list_targets[id_teleport_player]['Target_Data']['Weap_ammo'].v
			setCharCoordinates(PLAYER_PED, list_targets[id_teleport_player]['Target_Data']['Pos'].v[1], list_targets[id_teleport_player]['Target_Data']['Pos'].v[2], list_targets[id_teleport_player]['Target_Data']['Pos'].v[3])
			if not hasModelLoaded(list_targets[id_teleport_player]['Target_Data']['ModelID'].v) then
				requestModel(list_targets[id_teleport_player]['Target_Data']['ModelID'].v)
				while not hasModelLoaded(list_targets[id_teleport_player]['Target_Data']['ModelID'].v) do
					wait(1)
				end
			end
			setPlayerModel(PLAYER_HANDLE, list_targets[id_teleport_player]['Target_Data']['ModelID'].v)
			setCharHeading(PLAYER_PED, list_targets[id_teleport_player]['Target_Data']['Angle'].v)
			giveWeaponToChar(PLAYER_PED, list_targets[id_teleport_player]['Target_Data']['Weapon'].v, 99999)
			setInteriorVisible(list_targets[id_teleport_player]['Target_Data']['Interior_id'])
			setCharInterior(PLAYER_PED, list_targets[id_teleport_player]['Target_Data']['Interior_id'])
			while editmode_teleport_player do
				--Закрытие редактора
				wait(0)
				if wasKeyPressed(vkeys.VK_F) then
					clearHelp()
					editmode_teleport_player = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					removeAllCharWeapons(PLAYER_PED)
					lockPlayerControl(false)
				end
				if wasKeyPressed(vkeys.VK_SPACE) then
					if stage == 0 then
						lockPlayerControl(true)
						printHelpForever('HPLA2')
						stage = stage + 1
					elseif stage == 1 then
						clearHelp()
						editmode_teleport_player = false
						setPlayerModel(PLAYER_HANDLE, model.NULL)
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						local angle = getCharHeading(PLAYER_PED)
						xz = getGroundZFor3dCoord(xx,xy,xz)
						list_targets[id_teleport_player]['Target_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
						list_targets[id_teleport_player]['Target_Data']['Angle'].v = angle
						list_targets[id_teleport_player]['Target_Data']['ModelID'].v = modelID
						list_targets[id_teleport_player]['Target_Data']['Weapon'].v = weapon
						list_targets[id_teleport_player]['Target_Data']['Weap_ammo'].v = ammo
						list_targets[id_teleport_player]['Target_Data']['Interior_id'] = getActiveInterior()
						removeAllCharWeapons(PLAYER_PED)
						lockPlayerControl(false)
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						modelID = modelID + 1
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						modelID = modelID - 1
						if modelID < 0 then
							modelID = 288
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						modelID = modelID + 10
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						modelID = modelID - 10
						if modelID < 0 then
							modelID = 289 + modelID
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						local id_w = 0
						for v = 1,#Number_Weapons do
							if weapon <= Number_Weapons[v] then
								id_w = Number_Weapons[v]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v-1]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						print(weapon)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 0 then
							weapon = 46
						end
						local id_w = 46
						for v = 1,#Number_Weapons do
							if weapon < Number_Weapons[v] then
								id_w = Number_Weapons[v-1]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
				elseif stage == 1 then
					if isKeyDown(vkeys.VK_I) then
						wait(50)
						ammo = ammo + 10
					end
					if isKeyDown(vkeys.VK_O) and ammo > 0 then
						wait(50)
						ammo = ammo - 10
					end
					printString(koder('Патронов: ' .. '~r~' .. ammo), 0)
				end
			end
		end

		if editmode_preview_player_anim then
			if not hasAnimationLoaded(Anims['Anim_name'][list_targets[id_preview_player_anim]['Target_Data']['Pack_anim'].v+1]) then
				requestAnimation(Anims['Anim_name'][list_targets[id_preview_player_anim]['Target_Data']['Pack_anim'].v+1])
				while not hasAnimationLoaded(Anims['Anim_name'][list_targets[id_preview_player_anim]['Target_Data']['Pack_anim'].v+1]) do
					wait(0)
				end
			end
			local animm = Anims['Anim_list'][list_targets[id_preview_player_anim]['Target_Data']['Pack_anim'].v+1]
			animm = animm[list_targets[id_preview_player_anim]['Target_Data']['Anim'].v+1]
			taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][list_targets[id_preview_player_anim]['Target_Data']['Pack_anim'].v+1], 1.0, list_targets[id_preview_player_anim]['Target_Data']['Loop'].v, false, false, false, -1)
			while editmode_preview_player_anim do
				wait(0)
				if wasKeyPressed(vkeys.VK_F) then
					clearHelp()
					editmode_preview_player_anim = false
					taskPlayAnim(PLAYER_PED, "WALK_START", 'PED', 1.0, false, false, false, false, -1)
					lockPlayerControl(false)
				end
			end
		end

		if editmode_car then
			local carr = createCar(list_cars[id_car]['Car_Data']['ModelId'].v, list_cars[id_car]['Car_Data']['Pos'].v[1], list_cars[id_car]['Car_Data']['Pos'].v[2], list_cars[id_car]['Car_Data']['Pos'].v[3])
			setCarHeading(carr, list_cars[id_car]['Car_Data']['Angle'].v)
			taskWarpCharIntoCarAsDriver(PLAYER_PED, carr)
			while editmode_car do
				wait(0)
				if not isCharInCar(PLAYER_PED, carr) then
					local xx,xy,xz = getCarCoordinates(carr)
					list_cars[id_car]['Car_Data']['Pos'] = imgui.ImFloat3(xx,xy,xz)
					list_cars[id_car]['Car_Data']['Angle'].v = getCarHeading(carr)
					deleteCar(carr)
					upd_car:run(id_car)
					editmode_car = false
				end
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
			for o = 1,#list_objects do
				deleteCar(list_objects[o]['Object_Data']['Obj'])
			end
			mp.start_mission(list_targets,list_actors,list_cars,list_objects,mission_data)
			miss_start = false
		end

		while not mp.miss == 0 do
			wait(0)
		end

		--Откр/Закр. Меню
		if wasKeyPressed(vkeys.VK_U) then
			if main_window.v or targets_window.v or actors_window.v or cars_window.v or objects_window.v or missions_window.v or pack_mission_window.v or info_window.v then
				main_window.v = false
				targets_window.v = false
				actors_window.v = false
				cars_window.v = false
				objects_window.v = false
				missions_window.v = false
				pack_mission_window.v = false
				info_window.v = false
			else
				main_window.v = not main_window.v
			end
		end

		imgui.Process = main_window.v or targets_window.v or actors_window.v or cars_window.v or objects_window.v or missions_window.v or pack_mission_window.v or info_window.v

		-- Дебаг: позиция
		if wasKeyPressed(vkeys.VK_Q) then
			px,py,pz = getCharCoordinates(PLAYER_PED)
			ang = getCharHeading(PLAYER_PED)
			print(px .. ' ' .. py .. ' ' .. pz .. ' ' .. ang)
			print(getCharActiveInterior(PLAYER_PED))
		end
	end
end
