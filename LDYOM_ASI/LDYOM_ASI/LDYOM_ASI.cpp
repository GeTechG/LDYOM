#include "LDYOM_ASI.h"


#include "NodeGraph.h"
#include "ScriptManager.h"

extern bool mission_started;

void MainThread()
{
	bool initialized = false;

	//load lang current
	boost::property_tree::ptree pt;
	read_ini("LDYOM/LDYOM_data.ini", pt);
	assert(!pt.empty());
	curr_lang_string = pt.get<std::string>("Settings.curr_lang");
	curr_theme_string = pt.get<std::string>("Settings.curr_theme");

	//load special id actor
	boost::property_tree::ptree json_spec_actor;
	std::stringstream stream_json_spec_actor;
	stream_json_spec_actor << pt.get<std::string>("Data.ID_Spec_Actors");
	read_json(stream_json_spec_actor, json_spec_actor);
	for (boost::property_tree::ptree::value_type& v : json_spec_actor)
	{
		assert(v.first.empty()); // array elements have no names
		ID_Spec_Actors.push_back(v.second.data());
	}

	//load anims actor
	boost::property_tree::ptree json_anim_ifp_actor;
	std::stringstream stream_json_anim_ifp_actor;
	stream_json_anim_ifp_actor << pt.get<std::string>("Data.Actor_anims");
	read_json(stream_json_anim_ifp_actor, json_anim_ifp_actor);
	for (boost::property_tree::ptree::value_type& v_ifp : json_anim_ifp_actor)
	{
		ptrdiff_t ifp_idx;
		auto it = std::find(Anim_name.begin(), Anim_name.end(), v_ifp.first.data());
		if (it == Anim_name.end())
		{
			ifp_idx = Anim_name.size();
			Anim_name.push_back(v_ifp.first.data());
		}
		else
		{
			ifp_idx = std::distance(Anim_name.begin(), it);
		}
		boost::property_tree::ptree json_anim_actor;
		std::stringstream stream_json_anim_actor;
		stream_json_anim_actor << v_ifp.second.data();
		read_json(stream_json_anim_actor, json_anim_actor);
		for (boost::property_tree::ptree::value_type& v_anim : json_anim_actor)
		{
			assert(v_anim.first.empty()); // array elements have no names
			Anim_list.at(ifp_idx).push_back(v_anim.second.data());
		}
	}

	names_langs = get_filename_list("LDYOM/Language", ".ini");
	names_themes = get_filename_list("LDYOM/Themes", ".toml");
	for (int i = 0; i < names_langs.size(); ++i)
	{
		if (names_langs[i] == curr_lang_string)
		{
			curr_lang = i;
		}
	}
	for (int i = 0; i < names_themes.size(); ++i)
	{
		if (names_themes[i] == curr_theme_string)
		{
			curr_theme = i;
		}
	}
	printLog(names_langs.at(1));


	//load names missions packs
	namesMissionPacks.clear();
	namesMissions.clear();
	std::string path = "LDYOM//Missions_packs//";
	if (!path.empty()) {
		boost::filesystem::path apk_path(path);
		boost::filesystem::directory_iterator iter(apk_path), eod;

		for (boost::filesystem::path i : boost::filesystem::directory_iterator(path))
		{
			if (boost::filesystem::is_directory(i)) {
				printLog(i.string() + "//list.bin");
				if (boost::filesystem::exists(i.string() + "//list.bin"))
				{
					namesMissionPacks.push_back(cp1251_to_utf8(i.filename().string().c_str()));
					namesMissions.emplace_back();
					Manager::LoadListMission(namesMissionPacks.size() - 1);
				}
			}
		}
	}
	//load names storyline
	namesStorylines.clear();
	namesStorylines = get_filename_list("LDYOM//Storylines//", ".bin");

	playerPed = FindPlayerPed();
}

void loadArrayMenu()
{
	langMenu["type_targets_name"] = parseJsonArray<std::string>(langt("targets_list_arr"));
	langMenu["targets_marker_color"] = parseJsonArray<std::string>(langt("targets_marker_color"));
	langMenu["infoOverlay"] = parseJsonArray<std::string>(langt("infoOverlay"));
	langMenu["CutscenePos"] = parseJsonArray<std::string>(langt("CutscenePos"));
	langMenu["CutsceneLook"] = parseJsonArray<std::string>(langt("CutsceneLook"));
	langMenu["Weather_arr"] = parseJsonArray<std::string>(langt("Weather_arr"));
	langMenu["countTraffic"] = parseJsonArray<std::string>(langt("countTraffic"));
	langMenu["targetTypeObj"] = parseJsonArray<std::string>(langt("targetTypeObj"));
	langMenu["weap_names"] = parseJsonArray<std::string>(langt("weap_names"));
	langMenu["place_car"] = parseJsonArray<std::string>(langt("place_car"));
	langMenu["name_groups"] = parseJsonArray<std::string>(langt("name_groups"));
	langMenu["car_unbreak"] = parseJsonArray<std::string>(langt("car_unbreak"));
	langMenu["filterVeh"] = parseJsonArray<std::string>(langt("filterVeh"));
	langMenu["searchType"] = parseJsonArray<std::string>(langt("searchType"));
	langMenu["name_groups"] = parseJsonArray<std::string>(langt("name_groups"));
	langMenu["Relationship_types"] = parseJsonArray<std::string>(langt("Relationship_types"));
	langMenu["CutscenePos"] = parseJsonArray<std::string>(langt("CutscenePos"));
	langMenu["spawn_type_pickup"] = parseJsonArray<std::string>(langt("spawn_type_pickup"));
	langMenu["type_pickup"] = parseJsonArray<std::string>(langt("type_pickup"));
	langMenu["type_expl"] = parseJsonArray<std::string>(langt("type_expl"));
	langMenu["place_type"] = parseJsonArray<std::string>(langt("place_type"));
	langMenu["reloadQues"] = parseJsonArray<std::string>(langt("reloadQues"));
	langMenu["Weather_arr"] = parseJsonArray<std::string>(langt("Weather_arr"));
	langMenu["info_t"] = parseJsonArray<std::string>(langt("info_t"));
	langMenu["timeForStart"] = parseJsonArray<std::string>(langt("timeForStart"));
	langMenu["move_type_ped"] = parseJsonArray<std::string>(langt("move_type_ped"));
	langMenu["move_route_ped"] = parseJsonArray<std::string>(langt("move_route_ped"));
	langMenu["driver_beh"] = parseJsonArray<std::string>(langt("driver_beh"));
	langMenu["speed_walk_to_car"] = parseJsonArray<std::string>(langt("speed_walk_to_car"));
	langMenu["place_in_car"] = parseJsonArray<std::string>(langt("place_in_car"));
	langMenu["open_close"] = parseJsonArray<std::string>(langt("open_close"));
}

void createDirsLDYOM()
{
	const boost::filesystem::path path_LDYOM("LDYOM");
	const boost::filesystem::path path_MissPacks("LDYOM/Missions_packs");
	const boost::filesystem::path path_Storylines("LDYOM/Storylines");

	if (!exists(path_LDYOM))
	{
		if (!create_directory(path_LDYOM))
		{
			printLog("path LDYOM not created");
		}
	}
	if (!exists(path_MissPacks))
	{
		if (!create_directory(path_MissPacks))
		{
			printLog("path Miss pack not created");
		}
	}
	if (!exists(path_Storylines))
	{
		if (!create_directory(path_Storylines))
		{
			printLog("path storyline not created");
		}
	}
};

void foo()
{
	this_coro::wait(2s);
	std::vector<std::string> welcome_t = parseJsonArray<std::string>(langt("welcome_t"));
	std::string welcome;
	welcome.append(welcome_t.at(0));
	welcome.append(VERSION_STR);
	welcome.append(welcome_t.at(1));
	welcome = UTF8_to_CP1251(welcome);
	GXTEncode(welcome);
	CHud::SetHelpMessage(welcome.c_str(), false, false, false);
	CHud::DrawHelpText();

	//this_coro::wait(5s);
	currentMissionPtr->player.updateEditorPed();
	
	while (runningThreads)
	{
		this_coro::wait(0ms);
		if (editmodeCamera)
		{
			CHud::bScriptDontDisplayRadar = true;
			CHud::m_Wants_To_Draw_Hud = false;

			TargetCutscene* targetPtr = static_cast<TargetCutscene*>(currentMissionPtr->list_targets[currentTarget]);
			while (editmodeCamera)
			{
				this_coro::wait(0ms);
				float multy = 1.0f;
				if (KeyPressed(VK_CONTROL))
					multy = 0.5f;

				if (KeyPressed(VK_A) && !KeyPressed(VK_SHIFT))
				{
					float sinn = static_cast<float>(sin(rad(targetPtr->rotate[1] + 90)));
					float coss = static_cast<float>(cos(rad(targetPtr->rotate[1] + 90)));
					targetPtr->pos[0] = targetPtr->pos[0] - 0.2 * sinn * multy;
					targetPtr->pos[1] = targetPtr->pos[1] - 0.2 * coss * multy;
				}
				if (KeyPressed(VK_D) && !KeyPressed(VK_SHIFT))
				{
					float sinn = static_cast<float>(sin(rad(targetPtr->rotate[1] - 90)));
					float coss = static_cast<float>(cos(rad(targetPtr->rotate[1] - 90)));
					targetPtr->pos[0] = targetPtr->pos[0] - 0.2 * sinn * multy;
					targetPtr->pos[1] = targetPtr->pos[1] - 0.2 * coss * multy;
				}
				if (KeyPressed(VK_W) && !KeyPressed(VK_SHIFT))
				{
					float sinn = static_cast<float>(sin(rad(targetPtr->rotate[1])));
					float coss = static_cast<float>(cos(rad(targetPtr->rotate[1])));
					targetPtr->pos[0] = targetPtr->pos[0] + 0.2 * sinn * multy;
					targetPtr->pos[1] = targetPtr->pos[1] + 0.2 * coss * multy;
				}
				if (KeyPressed(VK_S) && !KeyPressed(VK_SHIFT))
				{
					float sinn = static_cast<float>(sin(rad(targetPtr->rotate[1])));
					float coss = static_cast<float>(cos(rad(targetPtr->rotate[1])));
					targetPtr->pos[0] = targetPtr->pos[0] - 0.2 * sinn * multy;
					targetPtr->pos[1] = targetPtr->pos[1] - 0.2 * coss * multy;
				}
				if (KeyPressed(VK_Q) && !KeyPressed(VK_SHIFT))
				{
					targetPtr->pos[2] = targetPtr->pos[2] + 0.2 * multy;
				}
				if (KeyPressed(VK_E) && !KeyPressed(VK_SHIFT))
				{
					targetPtr->pos[2] = targetPtr->pos[2] - 0.2 * multy;
				}

				if (KeyJustPressed(VK_R))
				{
					if (draw_lines)
						draw_lines = false;
					else
						draw_lines = true;
				}

				if (KeyPressed(VK_SHIFT))
				{
					if (KeyPressed(VK_A))
					{
						targetPtr->rotate[1] = targetPtr->rotate[1] - 2 * multy;
					}
					if (KeyPressed(VK_D))
					{
						targetPtr->rotate[1] = targetPtr->rotate[1] + 2 * multy;
					}
					if (KeyPressed(VK_W))
					{
						targetPtr->rotate[0] = targetPtr->rotate[0] - 2 * multy;
					}
					if (KeyPressed(VK_S))
					{
						targetPtr->rotate[0] = targetPtr->rotate[0] + 2 * multy;
					}
					if (KeyPressed(VK_E))
					{
						targetPtr->rotate[2] = targetPtr->rotate[2] - 2 * multy;
					}
					if (KeyPressed(VK_Q))
					{
						targetPtr->rotate[2] = targetPtr->rotate[2] + 2 * multy;
					}
				}

				float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
				float rxx = targetPtr->rotate[0], rxy = targetPtr->rotate[1], rxz = targetPtr->rotate[2];
				float x1 = xx, y1 = xy, z1 = xz;
				x1 = x1 + 2 * sin(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
				y1 = y1 + 2 * cos(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
				z1 = z1 + 2 * cos(static_cast<float>(rad(rxx)));

				Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, rad(rxz), 0);
				Command<Commands::POINT_CAMERA_AT_POINT>(x1, y1, z1, 2);

				//Close editor
				if (KeyPressed(VK_F))
				{
					CHud::m_bHelpMessagePermanent = false;
					TheCamera.Restore();
					editmodeCamera = false;

					targetPtr->rotate[0] = targetPtr->rotate[0] % 360;
					targetPtr->rotate[1] = targetPtr->rotate[1] % 360;
					targetPtr->rotate[2] = targetPtr->rotate[2] % 360;

					bTargets = true;
					CHud::bScriptDontDisplayRadar = false;
					CHud::m_Wants_To_Draw_Hud = true;
				}
			}
		}
		if (editmodeTeleportPlayer)
		{
			std::string HVIEW = UTF8_to_CP1251(langt("HVIEW"));
			GXTEncode(HVIEW);
			TargetTeleport* targetPtr = static_cast<TargetTeleport*>(currentMissionPtr->list_targets[currentTarget]);
			CHud::SetHelpMessage(HVIEW.c_str(), false, true, false);
			playerPed->SetPosn(CVector(targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2]));
			int modell;
			if (targetPtr->modelType == 0)
			{
				modell = targetPtr->modelID;
				CStreaming::RequestModel(modell, 0);
				CStreaming::LoadAllRequestedModels(false);
			}
			else
			{
				std::string modell_n = ID_Spec_Actors[targetPtr->modelID];
				CStreaming::RequestSpecialChar(9, modell_n.c_str(), 0);
				CStreaming::LoadAllRequestedModels(false);
				while (!CStreaming::HasSpecialCharLoaded(9))
				{
					this_coro::wait(0ms);
				}
				modell = 290 + 10 - 1;
			}
			Command<Commands::SET_PLAYER_MODEL>(0, modell);
			CStreaming::SetSpecialCharIsDeletable(9);
			CStreaming::RemoveAllUnusedModels();
			Command<Commands::SET_CHAR_HEADING>(playerPed, targetPtr->angle);
			Command<Commands::SET_AREA_VISIBLE>(targetPtr->interiorID);
			Command<Commands::SET_CHAR_AREA_VISIBLE>(playerPed, targetPtr->interiorID);
			while (editmodeTeleportPlayer)
			{
				//Закрытие редактора
				this_coro::wait(0ms);
				if (KeyJustPressed(VK_F))
				{
					CHud::m_bHelpMessagePermanent = false;
					editmodeTeleportPlayer = false;
					bTargets = true;
					CStreaming::RequestModel(0, 0);
					CStreaming::LoadAllRequestedModels(false);
					Command<Commands::SET_PLAYER_MODEL>(0, 0);
					CStreaming::RemoveAllUnusedModels();
					float xx = playerPed->GetPosition().x, xy = playerPed->GetPosition().y, xz = playerPed->
						      GetPosition().z;
					float angle = deg(playerPed->GetHeading());
					targetPtr->pos[0] = xx;
					targetPtr->pos[1] = xy;
					targetPtr->pos[2] = xz;
					targetPtr->angle = angle;
					targetPtr->interiorID = Command<Commands::GET_AREA_VISIBLE>();
				}
			}
		}
		while (editmodeTimetarg)
		{
			this_coro::wait(0ms);
			TargetTime* targetPtr = static_cast<TargetTime*>(currentMissionPtr->list_targets[currentTarget]);
			if (KeyPressed(VK_I))
			{
				this_coro::wait(100ms);
				targetPtr->time[0] += 1;
			}
			if (KeyPressed(VK_O))
			{
				this_coro::wait(100ms);
				targetPtr->time[0]--;
			}
			if (KeyPressed(VK_K))
			{
				this_coro::wait(100ms);
				targetPtr->time[1]++;
			}
			if (KeyPressed(VK_L))
			{
				this_coro::wait(100ms);
				targetPtr->time[1]--;
			}
			if (targetPtr->time[1] == 60)
			{
				targetPtr->time[0]++;
				targetPtr->time[1] = 0;
			}
			else if (targetPtr->time[1] == -1)
			{
				targetPtr->time[0]--;
				targetPtr->time[1] = 59;
			}
			if (targetPtr->time[0] == 24)
			{
				targetPtr->time[0] = 0;
			}
			else if (targetPtr->time[0] == -1)
			{
				targetPtr->time[0] = 23;
			}
			CClock::SetGameClock(targetPtr->time[0], targetPtr->time[1], CClock::ms_nGameClockDays);
			//Close editor
			if (KeyJustPressed(VK_F))
			{
				CHud::m_bHelpMessagePermanent = false;
				editmodeTimetarg = false;
				CClock::SetGameClock(8, 0, CClock::ms_nGameClockDays);
				bTargets = true;
			}
		}
		if (editActor)
		{
			std::string HVIEW = UTF8_to_CP1251(langt("HVIEW"));
			GXTEncode(HVIEW);
			CHud::SetHelpMessage(HVIEW.c_str(), false, true, false);
			Actor* actorPtr = currentMissionPtr->list_actors[currentActor];
			TheCamera.Restore();
			while (editActor)
			{
				this_coro::wait(0ms);
				if (KeyJustPressed(VK_F))
				{
					CHud::m_bHelpMessagePermanent = false;
					editActor = false;
					bActors = true;

					CStreaming::RequestModel(0, 0);
					CStreaming::LoadAllRequestedModels(false);
					Command<Commands::SET_PLAYER_MODEL>(0, 0);
					CStreaming::RemoveAllUnusedModels();
					float xx = playerPed->GetPosition().x, xy = playerPed->GetPosition().y, xz = playerPed->
						      GetPosition().z;
					float angle = deg(playerPed->GetHeading());
					actorPtr->pos[0] = xx;
					actorPtr->pos[1] = xy;
					actorPtr->pos[2] = xz;
					actorPtr->angle = angle;
					Command<Commands::GET_AREA_VISIBLE>(&actorPtr->interiorID);
					actorPtr->updateEditorPed();
				}
			}
		}
		if (editmodeCar)
		{
			Car* carPtr = static_cast<Car*>(currentMissionPtr->list_cars[currentCar]);
			TheCamera.Restore();
			carPtr->editorCar->m_bUsesCollision = true;
			Command<Commands::FREEZE_CAR_POSITION>(carPtr->editorCar, false);
			carPtr->editorCar->SetHeading(rad(carPtr->angle));
			Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(playerPed, carPtr->editorCar);
			//taskWarpCharIntoCarAsDriver(PLAYER_PED, carPtr->editorCar)
			while (editmodeCar)
			{
				this_coro::wait(0ms);
				if (!Command<Commands::IS_CHAR_IN_CAR>(playerPed, carPtr->editorCar))
				{
					CVector pos = carPtr->editorCar->GetPosition();
					carPtr->pos[0] = pos.x;
					carPtr->pos[1] = pos.y;
					carPtr->pos[2] = pos.z;
					carPtr->angle = deg(carPtr->editorCar->GetHeading());
					carPtr->editorCar->m_bUsesCollision = false;
					Command<Commands::FREEZE_CAR_POSITION>(carPtr->editorCar, true);
					carPtr->editorCar->m_nVehicleFlags.bEngineOn = 0;
					editmodeCar = false;
					bCars = true;
				}
			}
		}
		if (editPlayer)
		{
			std::string HVIEW = UTF8_to_CP1251(langt("HVIEW"));
			GXTEncode(HVIEW);
			CHud::SetHelpMessage(HVIEW.c_str(), false, true, false);
			Player* playerPtr = &currentMissionPtr->player;
			TheCamera.Restore();
			while (editPlayer)
			{
				this_coro::wait(0ms);
				if (KeyJustPressed(VK_F))
				{
					CHud::m_bHelpMessagePermanent = false;
					editPlayer = false;
					bPlayer = true;

					CStreaming::RequestModel(0, 0);
					CStreaming::LoadAllRequestedModels(false);
					Command<Commands::SET_PLAYER_MODEL>(0, 0);
					CStreaming::RemoveAllUnusedModels();
					float xx = playerPed->GetPosition().x, xy = playerPed->GetPosition().y, xz = playerPed->GetPosition().z;
					float angle = deg(playerPed->GetHeading());
					playerPtr->pos[0] = xx;
					playerPtr->pos[1] = xy;
					playerPtr->pos[2] = xz;
					playerPtr->angle = angle;
					Command<Commands::GET_AREA_VISIBLE>(&playerPtr->interiorID);
					playerPtr->updateEditorPed();
				}
			}
		}
		while (editmodeTimemiss)
		{
			this_coro::wait(0ms);
			if (KeyPressed(VK_I))
			{
				this_coro::wait(100ms);
				currentMissionPtr->time[0] += 1;
			}
			if (KeyPressed(VK_O))
			{
				this_coro::wait(100ms);
				currentMissionPtr->time[0]--;
			}
			if (KeyPressed(VK_K))
			{
				this_coro::wait(100ms);
				currentMissionPtr->time[1]++;
			}
			if (KeyPressed(VK_L))
			{
				this_coro::wait(100ms);
				currentMissionPtr->time[1]--;
			}
			if (currentMissionPtr->time[1] == 60)
			{
				currentMissionPtr->time[0]++;
				currentMissionPtr->time[1] = 0;
			}
			else if (currentMissionPtr->time[1] == -1)
			{
				currentMissionPtr->time[0]--;
				currentMissionPtr->time[1] = 59;
			}
			if (currentMissionPtr->time[0] == 24)
			{
				currentMissionPtr->time[0] = 0;
			}
			else if (currentMissionPtr->time[0] == -1)
			{
				currentMissionPtr->time[0] = 23;
			}
			CClock::SetGameClock(currentMissionPtr->time[0], currentMissionPtr->time[1], CClock::ms_nGameClockDays);
			//Close editor
			if (KeyJustPressed(VK_F))
			{
				CHud::m_bHelpMessagePermanent = false;
				editmodeTimemiss = false;
				CClock::SetGameClock(8, 0, CClock::ms_nGameClockDays);
				bMissionSettings = true;
			}
		}
	}
}

void clearScripts()
{
	lang_file.clear();
	loadArrayMenu();
	map_nodes_class.clear();
}

#include "Manager.h"
class LDYOM_ASI
{
public:
	LDYOM_ASI()
	{
		Events::initGameEvent += []
		{
			if (CGame::bMissionPackGame == 7)
			{
				createDirsLDYOM();
				startLog();
				MainThread();
				InitHooks();
				loadArrayMenu();
				currentMissionPtr = new Mission;
				printLog("start");
				instance.add_to_queue(foo);
				ScriptManager::loadScripts();
				init = true;
				langMenu["typesValue"] = parseJsonArray<std::string>(langt("typesValue"));
			}

			
			
		};
		Events::vehicleRenderEvent.before += [](CVehicle* veh)
		{
			if (veh)
				VehicleRenderer::get()->processRender(veh);
		};
		Events::vehicleResetAfterRender += [](CVehicle* veh)
		{
			if (veh)
				VehicleRenderer::get()->postRender(veh);
		};
		Events::reInitGameEvent += []
		{
			if (CGame::bMissionPackGame == 7)
			{
				if (init)
				{
					delete currentMissionPtr;
					delete currentStorylinePtr;
					delete currentNodeGraphPtr;
					bMainMenu = false;
					namesTargets.clear();
					currentTarget = 0;
					bTargets = false;
					namesActors.clear();
					currentActor = 0;
					bActors = false;
					namesCars.clear();
					currentCar = 0;
					bCars = false;
					namesObjects.clear();
					currentObject = 0;
					bObjects = false;
					namesPickups.clear();
					currentPickup = 0;
					bPickups = false;
					namesParticles.clear();
					currentParticle = 0;
					bParticles = false;
					namesExplosions.clear();
					currentExplosion = 0;
					bExplosions = false;
					namesAudios.clear();
					currentAudio = 0;
					bAudios = false;
					bMissionSettings = false;
					bPlayer = false;
					bMissionPacks = false;
					carSelector::bShow = false;
				}

				startLog();
				MainThread();
				loadArrayMenu();
				currentMissionPtr = new Mission;
				currentNodeGraphPtr = new NodeGraph;
				currentMissionPtr->player.updateEditorPed();
				printLog("start");
				init = true;
				printLog("reini");
				ScriptManager::loadScripts();
			}
		};
		Events::processScriptsEvent += []
		{
			if (CGame::bMissionPackGame == 7)
			{
				if (Command<0x0ADC>("TOP2009"))
				{
					CHud::SetHelpMessage("~r~I LOVE TWENTY ONE PILOTS", false, false, false);
					CHud::DrawHelpText();
				}

				if (!storylineMode && !mission_started) {
					if (updateSphere)
						updateSphere = false;
					else
					{
						vector<unsigned int> sphereIdx;
						for (auto v : currentMissionPtr->list_targets)
						{
							if (v->type == 0)
							{
								TargetCheckpoint* v_ptr = static_cast<TargetCheckpoint*>(v);
								if (DistanceBetweenPoints(CVector(v_ptr->pos[0], v_ptr->pos[1], v_ptr->pos[2]),
									TheCamera.GetPosition()) < 100.0f)
									sphereIdx.push_back(CTheScripts::AddScriptSphere(
										sphereIdx.size(), CVector(v_ptr->pos[0], v_ptr->pos[1], v_ptr->pos[2]),
										v_ptr->radius));
							}
						}
						CTheScripts::DrawScriptSpheres();
						for (auto v : sphereIdx)
						{
							CTheScripts::RemoveScriptSphere(v);
						}
					}
				}
				instance.process();
			}
		};
	}
} lDYOM_ASI;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch (dwReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		hhModule = hModule;
		break;
	case DLL_PROCESS_DETACH:
		DeleteHooks();
		delete currentMissionPtr;
		delete currentStorylinePtr;
		delete currentNodeGraphPtr;
		for (auto script : ScriptManager::lua_scripts)
			delete &script.second;
		break;
	}
	return TRUE;
}
