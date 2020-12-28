#include "MissionPlayer.h"

#include <CText.h>
#include "CStats.h"
#include "CTheScripts.h"
#include "CCamera.h"
#include "libs/coro_wait.h"
#include "CHud.h"
#include "CMessages.h"
#include "CWeather.h"
#include "CClock.h"
#include "CPopulation.h"
#include "CPlayerData.h"
#include "CRadar.h"
#include "CPool.h"
#include "Manager.h"
#include "NodeGraph.h"

using namespace std::chrono_literals;

extern void printLog(std::string print_text);
extern CPed *playerPed;
extern inline double rad(float degrees);
extern inline double deg(float radians);
extern coro_wait instance;
extern std::string UTF8_to_CP1251(std::string const& utf8);
extern void GXTEncode(std::string& str);
extern bool is_utf8(const char* string);
extern bool KeyJustPressed(unsigned int key);
extern vector<std::string> namesMissionPacks;
extern vector<vector<std::string>> namesMissions;
extern Storyline* currentStorylinePtr;
extern std::string replace_symb(std::string& str);

bool mission_started = false;
bool storyline_started = false;
bool defeat = false;
bool nodeSignal = false;
list<CPed*> missionActors{};
list<CVehicle*> missionVehicle{};
list<CObject*> missionObject{};
list<pair<unsigned int, CObject*>> missionParticle{};
list<int> missionPickup{};
list<int> missionExplosion{};
list<int> missionAudio{};

void failMission()
{
	unsigned int textPtr;
	Command<0x0ADE>("M_FAIL", &textPtr);
	CMessages::AddBigMessageWithNumber(reinterpret_cast<char*>(textPtr), 4000, 0, 100, NULL, NULL, NULL, NULL, NULL);
}

void respectMission()
{
	unsigned int textPtr;
	Command<0x0ADE>("M_PASSR", &textPtr);
	CMessages::AddBigMessageQ(reinterpret_cast<char*>(textPtr), 4000, 0);
	Command<Commands::PLAY_MISSION_PASSED_TUNE>(1);
}

void testDefeat()
{
	while (mission_started)
	{
		this_coro::wait(0ms);
		if (CWorld::Players[0].m_nPlayerState != ePlayerState::PLAYERSTATE_PLAYING)
		{
			Command<Commands::SET_PLAYER_MODEL>(0, 0);
			while (CWorld::Players[0].m_nPlayerState == ePlayerState::PLAYERSTATE_PLAYING)
			{
				this_coro::wait(0ms);
			}
			defeat = true;
			mission_started = false;
			failMission();
		}
	}
}

int current_mission_target = 0;

extern NodeGraph* currentNodeGraphPtr;

void MissionPlayer::start_mission(Mission* mission, bool clearSelf)
{
	assert(CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] == 0);

	//start
	defeat = false;
	mission_started = true;
	TheCamera.Fade(0.2f, 0);
	this_coro::wait(200ms);

	//set player
	playerPed->m_fCurrentRotation = static_cast<float>(rad(mission->player.angle));
	playerPed->SetPosn(mission->player.pos[0], mission->player.pos[1], mission->player.pos[2]);
	Command<Commands::REQUEST_COLLISION>(mission->player.pos[0], mission->player.pos[1]);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(mission->wanted_min);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetMaximumWantedLevel(mission->wanted_max);
	if (mission->player.health > 100)
	{
		playerPed->m_fMaxHealth = mission->player.health;
	}
	playerPed->m_fHealth = mission->player.health;
	Command<Commands::SET_CHAR_AREA_VISIBLE>(playerPed, mission->player.interiorID);
	int modell;
	if (mission->player.modelType == 0) {
		modell = mission->player.modelID;
		CStreaming::RequestModel(modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	else if (mission->player.modelType == 1) {
		std::string modell_n = ID_Spec_Actors[mission->player.modelID];
		CStreaming::RequestSpecialChar(9, modell_n.c_str(), 0);
		CStreaming::LoadAllRequestedModels(false);
		modell = 290 + 10 - 1;
	}
	Command<Commands::SET_PLAYER_MODEL>(0, modell);
	int weap_modell;
	Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[mission->player.weapon], &weap_modell);
	if (weap_modell != -1) {
		CStreaming::RequestModel(weap_modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	playerPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[mission->player.weapon]), 1, false);
	playerPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[mission->player.weapon]));
	CStreaming::RemoveAllUnusedModels();
	
	
	//show name mission.
	
	std::string name_miss = is_utf8(mission->name.c_str())? UTF8_to_CP1251(mission->name) : mission->name;
	GXTEncode(name_miss);
	CMessages::AddBigMessage(const_cast<char*>(name_miss.c_str()), 2000, 1);

	//set world
	CWeather::ForceWeatherNow(mission->weather);
	Command<Commands::SET_AREA_VISIBLE>(mission->player.interiorID);
	CClock::SetGameClock(mission->time[0], mission->time[1], CClock::CurrentDay);
	Command<Commands::SET_LA_RIOTS>(mission->riot);
	Command<Commands::SET_PED_DENSITY_MULTIPLIER>((float)mission->traffic_ped);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>((float)mission->traffic_car);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (mission->groupRelations[i][j] != -1)
			{
				if (j == 0)
				{
					Command<Commands::SET_RELATIONSHIP>(mission->groupRelations[i][j], 24 + i, 0);
					Command<Commands::SET_RELATIONSHIP>(mission->groupRelations[i][j], 24 + i, 23);
					Command<Commands::SET_RELATIONSHIP>(mission->groupRelations[i][j], 23, 24 + i);
				} else
				{
					if (i != j - 1) {
						Command<Commands::SET_RELATIONSHIP>(mission->groupRelations[i][j], 24 + i, 24 + j-1);
					}
				}
			}
		}
	}
	current_mission_target = 0;

	//load audio
	for (auto audio : mission->list_audios)
	{
		audio->loadAudio();
	}
	
	this_coro::wait(300ms);
	TheCamera.Fade(0.2f, 1);
	CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 1;
	
	instance.add_to_queue(testDefeat);

	//call start node
	std::string name_node = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeStart");
	NodeGraph::callAllNodeName(name_node, currentNodeGraphPtr, mission);
	
	
	unsigned char skip = 1;
	
	//targets loop
	while (current_mission_target < mission->list_targets.size() && mission_started)
	{
		for (int i = 0; i < mission->list_actors.size(); ++i)
		{
			if (mission->list_actors[i]->useTarget)
			{
				if (mission->list_actors[i]->startC  == current_mission_target)
				{
					instance.add_to_queue([actor = mission->list_actors[i]] { 
						actor->updateMissionPed();
						missionActors.push_back(actor->missionPed);
					});
				}
			}
		}
		for (int i = 0; i < mission->list_cars.size(); ++i)
		{
			if (mission->list_cars[i]->useTarget)
			{
				if (mission->list_cars[i]->startC == current_mission_target)
				{
					instance.add_to_queue([car = mission->list_cars[i]]{ 
						car->updateMissionCar();
						missionVehicle.push_back(car->missionCar);
					});
				}
			}
		}
		for (int i = 0; i < mission->list_objects.size(); ++i)
		{
			if (mission->list_objects[i]->useTarget)
			{
				if (mission->list_objects[i]->startC == current_mission_target)
				{
					instance.add_to_queue([object = mission->list_objects[i]]{ 
						object->updateMissionObject();
						missionObject.push_back(object->missionObject);
					});
				}
			}
		}
		for (int i = 0; i < mission->list_particles.size(); ++i)
		{
			if (mission->list_particles[i]->useTarget)
			{
				if (mission->list_particles[i]->startC == current_mission_target)
				{
					instance.add_to_queue([particle = mission->list_particles[i], mission]
					{
						particle->updateMissionParticle(mission);
						missionParticle.push_back(particle->missionParticle);
					});
				}
			}
		}
		for (int i = 0; i < mission->list_pickups.size(); ++i)
		{
			if (mission->list_pickups[i]->useTarget)
			{
				if (mission->list_pickups[i]->startC == current_mission_target)
				{
					instance.add_to_queue([pickup = mission->list_pickups[i]]{ 
						pickup->updateMissionPickup();
						missionPickup.push_back(pickup->missionPickup);
					});
				}
			}
		}
		for (int i = 0; i < mission->list_explosions.size(); ++i)
		{
			if (mission->list_explosions[i]->useTarget)
			{
				if (mission->list_explosions[i]->startC == current_mission_target)
				{
					instance.add_to_queue([explosion = mission->list_explosions[i]]{ 
						explosion->updateMissionExplosion();
						missionExplosion.push_back(explosion->missionFire);
					});
				}
			}
		}
		for (int i = 0; i < mission->list_audios.size(); ++i)
		{
			if (mission->list_audios[i]->useTarget)
			{
				if (mission->list_audios[i]->startC == current_mission_target)
				{
					instance.add_to_queue([audio = mission->list_audios[i], mission]
					{
						audio->play(mission);
						missionAudio.push_back(audio->missionAudio);
					});
				}
			}
		}
		

		this_coro::wait(1ms);
		
		switch (mission->list_targets[current_mission_target]->type)
		{
			case 0: {
				TargetCheckpoint* targetPtr = static_cast<TargetCheckpoint*>(mission->list_targets[current_mission_target]);
				std::string str_text = targetPtr->text;
				str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
				GXTEncode(str_text);
				CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->textTime*1000.0f, 0, false);
				int blip = NULL;
				if (targetPtr->colorBlip > 0)
				{
					Command<Commands::ADD_BLIP_FOR_COORD>(targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], &blip);
					if (targetPtr->colorBlip != 6)
					{
						CRadar::ChangeBlipColour(blip, targetPtr->colorBlip-1);
					}
					else
					{
						CRadar::SetBlipFriendly(blip, 1);
					}
				}
				unsigned sphere = CTheScripts::AddScriptSphere(600, CVector(targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2]), 2.0f);
				while (!Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(playerPed, targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], targetPtr->radius, targetPtr->radius, targetPtr->radius, false) && mission_started)
				{
					this_coro::wait(0ms);
					CTheScripts::DrawScriptSpheres();
				}
				CTheScripts::RemoveScriptSphere(sphere);
				if (blip != NULL)
				{
					CRadar::ClearBlip(blip);
				}
				break;
			}
			case 1: {
				TargetCar* targetPtr = static_cast<TargetCar*>(mission->list_targets[current_mission_target]);
				std::string str_text = targetPtr->text;
				str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
				GXTEncode(str_text);
				CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->textTime*1000.0f, 0, false);
				int blip = NULL;
				if (targetPtr->colorBlip > 0)
				{
					Command<Commands::ADD_BLIP_FOR_CAR>(mission->list_cars[targetPtr->car]->missionCar, &blip);
					if (targetPtr->colorBlip != 6)
					{
						CRadar::ChangeBlipColour(blip, targetPtr->colorBlip-1);
					}
					else
					{
						CRadar::SetBlipFriendly(blip, 1);
					}
				}
				while (!mission->list_cars[targetPtr->car]->missionCar->IsDriver(playerPed) && mission_started)
				{
					this_coro::wait(0ms);
				}
				if (blip != NULL)
				{
					CRadar::ClearBlip(blip);
				}
				break;
			}
			case 2: {
				TargetActor* targetPtr = static_cast<TargetActor*>(mission->list_targets[current_mission_target]);
				std::string str_text = targetPtr->text;
				str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
				GXTEncode(str_text);
				CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->textTime*1000.0f, 0, false);
				if (targetPtr->killGroup)
				{
					list<int> blips;
					list<Actor*> group_actors;
					for (auto* actor : mission->list_actors)
					{
						if (actor->missionPed != nullptr && !CTheScripts::pActiveScripts->IsPedDead(actor->missionPed))
						{
							if (actor->missionPed->m_nPedType == mission->list_actors[targetPtr->actor]->missionPed->m_nPedType)
							{
								group_actors.push_back(actor);
							}
							if (targetPtr->colorBlip > 0)
							{
								int blip = NULL;
								Command<Commands::ADD_BLIP_FOR_CHAR>(actor->missionPed, &blip);
								blips.push_back(blip);
								if (targetPtr->colorBlip != 6)
								{
									CRadar::ChangeBlipColour(blips.back(), targetPtr->colorBlip - 1);
								}
								else
								{
									CRadar::SetBlipFriendly(blips.back(), 1);
								}
							}
						}
					}
					int count_dead = 0;
					while (count_dead < group_actors.size() && mission_started)
					{
						this_coro::wait(0ms);
						count_dead = 0;
						for (auto* actor : group_actors)
						{
							if (CTheScripts::pActiveScripts->IsPedDead(actor->missionPed))
							{
								count_dead++;
							}
						}
					}
					if (targetPtr->colorBlip > 0)
					{
						for (auto blip : blips)
						{
							CRadar::ClearBlip(blip);
						}
					}
				}
				else {
					int blip = NULL;
					if (targetPtr->colorBlip > 0)
					{
						Command<Commands::ADD_BLIP_FOR_CHAR>(mission->list_actors[targetPtr->actor]->missionPed, &blip);
						if (targetPtr->colorBlip != 6)
						{
							CRadar::ChangeBlipColour(blip, targetPtr->colorBlip - 1);
						}
						else
						{
							CRadar::SetBlipFriendly(blip, 1);
						}
					}
					if (targetPtr->hit)
					{
						while (!(mission->list_actors[targetPtr->actor]->missionPed->m_pLastEntityDamage == playerPed) && mission_started)
						{
							this_coro::wait(0ms);
						}
					}
					else {
						while (!CTheScripts::pActiveScripts->IsPedDead(mission->list_actors[targetPtr->actor]->missionPed) && mission_started)
						{
							this_coro::wait(0ms);
						}
					}
					if (blip != NULL)
					{
						CRadar::ClearBlip(blip);
					}
				}
				break;
			}
			case 3: {
				switch (mission->list_targets[current_mission_target]->targetType)
				{
					case 0: {
						TargetCutscene* targetPtr = static_cast<TargetCutscene*>(mission->list_targets[current_mission_target]);

						CHud::bScriptDontDisplayRadar = true;
						CHud::m_Wants_To_Draw_Hud = false;
						TheCamera.m_bWideScreenOn = true;

						if (current_mission_target > 0 && !(mission->list_targets[current_mission_target - 1]->type == 3 && mission->list_targets[current_mission_target - 1]->targetType == 0))
						{
							TheCamera.Fade(500, 0);
							this_coro::wait(1s);
							TheCamera.Fade(500, 1);
							skip = 1;
						}

						bool useSkip = true;

						instance.add_to_queue([&useSkip, &skip]
						{
							while (useSkip && mission_started)
							{
								if (KeyJustPressed(VK_SPACE))
								{
									skip = 0;
								}
								this_coro::wait(0ms);
							}
						});

						switch (targetPtr->tied) {
						case 0: {
							switch (targetPtr->follow) {
							case 0: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float rxx = targetPtr->rotate[0], rxy = targetPtr->rotate[1], rxz = targetPtr->rotate[2];
								float x1 = xx, y1 = xy, z1 = xz;
								x1 = x1 + 2 * sin(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
								y1 = y1 + 2 * cos(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
								z1 = z1 + 2 * cos(static_cast<float>(rad(rxx)));
								if (targetPtr->moveCam)
								{
									Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time*1000));
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, rad(rxz), 0);
									Command<Commands::POINT_CAMERA_AT_POINT>(x1, y1, z1, 1);
								}
								else
								{
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, rad(rxz), 0);
									Command<Commands::POINT_CAMERA_AT_POINT>(x1, y1, z1, 2);
								}

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);
								this_coro::wait(targetPtr->time*1000.0*skip);
								break;
							}
							case 1: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								if (targetPtr->moveCam)
								{
									Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);
									Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 1);
								}
								else
								{
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);
									Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 2);
								}

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);
								this_coro::wait(targetPtr->time*1000.0f*skip);

								break;
							}
							case 2: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								if (targetPtr->moveCam)
								{
									Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);
									Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 1);
								}
								else
								{
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);
									Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 2);
								}

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);
								this_coro::wait(targetPtr->time*1000.0f*skip);

								break;
							}
							case 3: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								CVector& pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								unsigned int timer = 0;
								auto last_time = clock();
								if (targetPtr->moveCam)
								{

									Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);

									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										Command<Commands::POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, 1);
										timer += clock() - last_time;
										this_coro::wait(0ms);
									}
								}
								else
								{
									Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);

									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										Command<Commands::POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, 2);
										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
								}

								break;
							}
							}
							break;
						}
						case 1: {
							switch (targetPtr->follow) {
							case 0: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float rxx = targetPtr->rotate[0], rxy = targetPtr->rotate[1], rxz = targetPtr->rotate[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									float rx = curr_pos.x + (xx - x1), ry = curr_pos.y + (xy - y1), rz = curr_pos.z + (xz - z1);

									rx = rx + 2 * sin(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
									ry = ry + 2 * cos(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
									rz = rz + 2 * cos(static_cast<float>(rad(rxx)));

									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(rx, ry, rz, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(rx, ry, rz, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 1: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
								CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(actor_pos.x, actor_pos.y, actor_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(actor_pos.x, actor_pos.y, actor_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 2: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
								CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(car_pos.x, car_pos.y, car_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(car_pos.x, car_pos.y, car_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 3: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
								CVector& obj_pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(obj_pos.x, obj_pos.y, obj_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(obj_pos.x, obj_pos.y, obj_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							}
							break;
						}
						case 2: {
							switch (targetPtr->follow) {
							case 0: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float rxx = targetPtr->rotate[0], rxy = targetPtr->rotate[1], rxz = targetPtr->rotate[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									float rx = curr_pos.x + (xx - x1), ry = curr_pos.y + (xy - y1), rz = curr_pos.z + (xz - z1);

									rx = rx + 2 * sin(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
									ry = ry + 2 * cos(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
									rz = rz + 2 * cos(static_cast<float>(rad(rxx)));

									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(rx, ry, rz, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(rx, ry, rz, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 1: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
								CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(actor_pos.x, actor_pos.y, actor_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(actor_pos.x, actor_pos.y, actor_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 2: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
								CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(car_pos.x, car_pos.y, car_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(car_pos.x, car_pos.y, car_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 3: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
								CVector& obj_pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(obj_pos.x, obj_pos.y, obj_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(obj_pos.x, obj_pos.y, obj_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							}
							break;
						}
						case 3: {
							switch (targetPtr->follow) {
							case 0: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float rxx = targetPtr->rotate[0], rxy = targetPtr->rotate[1], rxz = targetPtr->rotate[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									float rx = curr_pos.x + (xx - x1), ry = curr_pos.y + (xy - y1), rz = curr_pos.z + (xz - z1);

									rx = rx + 2 * sin(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
									ry = ry + 2 * cos(static_cast<float>(rad(rxy))) * sin(static_cast<float>(rad(rxx)));
									rz = rz + 2 * cos(static_cast<float>(rad(rxx)));

									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(rx, ry, rz, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(rx, ry, rz, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 1: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
								CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(actor_pos.x, actor_pos.y, actor_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(actor_pos.x, actor_pos.y, actor_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 2: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
								CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
								while (timer < targetPtr->time * 1000 * skip && mission_started)
								{
									if (targetPtr->moveCam) {
										Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(car_pos.x, car_pos.y, car_pos.z, 1);
									}
									else
									{
										Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
										Command<Commands::POINT_CAMERA_AT_POINT>(car_pos.x, car_pos.y, car_pos.z, 2);
									}

									timer += clock() - last_time;
									last_time = clock();
									this_coro::wait(0ms);
								}

								break;
							}
							case 3: {
								float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
								float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

								unsigned int timer = 0;
								auto last_time = clock();

								std::string str_text = targetPtr->text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);

								CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
							}

									break;
							}
						}
						break;
						}

						if (current_mission_target >= mission->list_targets.size()-1 || !(mission->list_targets[current_mission_target+1]->type == 3 && mission->list_targets[current_mission_target+1]->targetType == 0)) {
							TheCamera.Fade(500, 0);
							this_coro::wait(0.5s);
							CHud::bScriptDontDisplayRadar = false;
							CHud::m_Wants_To_Draw_Hud = true;
							TheCamera.m_bWideScreenOn = false;
							TheCamera.RestoreWithJumpCut();
							this_coro::wait(0.5s);
							TheCamera.Fade(500, 1);
						}
							
						break;
					}
					case 1: {
						TargetCountdown* targetPtr = static_cast<TargetCountdown*>(mission->list_targets[current_mission_target]);
						Command<Commands::SET_PLAYER_CONTROL>(0, 0);

						for (int i = targetPtr->time; i > 0; i--)
						{
							auto str = std::to_string(i);
							CMessages::AddBigMessageQ(const_cast<char*>(str.c_str()), 1000, 3);
							this_coro::wait(1s);
						}
							
						CMessages::AddBigMessageQ("Go!", 1000, 3);
						this_coro::wait(1s);
						Command<Commands::SET_PLAYER_CONTROL>(0, 1);
							
						break;
					}
					case 2: {
						TargetTimeout* targetPtr = static_cast<TargetTimeout*>(mission->list_targets[current_mission_target]);
							
						std::string str_text = targetPtr->text;
						str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
						GXTEncode(str_text);
						CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000.0f, 0, false);
						this_coro::wait(targetPtr->time*1000);

						break;
					}
					case 3: {
						TargetWeather* targetPtr = static_cast<TargetWeather*>(mission->list_targets[current_mission_target]);

						CWeather::ForceWeatherNow(targetPtr->weather);
							
						break;
					}
					case 4: {
						TargetTime* targetPtr = static_cast<TargetTime*>(mission->list_targets[current_mission_target]);

						CClock::SetGameClock(targetPtr->time[0], targetPtr->time[1], CClock::CurrentDay);
							
						break;
					}
					case 5: {
						TargetTraffic* targetPtr = static_cast<TargetTraffic*>(mission->list_targets[current_mission_target]);

						Command<Commands::SET_PED_DENSITY_MULTIPLIER>((float)targetPtr->peds);
						Command<Commands::SET_CAR_DENSITY_MULTIPLIER>((float)targetPtr->cars);

						break;
					}
					default: break;
				}
				break;
			}
			case 4: {
				TargetObject* targetPtr = static_cast<TargetObject*>(mission->list_targets[current_mission_target]);

				int blip = NULL;
				if (targetPtr->colorBlip > 0)
				{
					Command<Commands::ADD_BLIP_FOR_OBJECT>(mission->list_objects[targetPtr->object]->missionObject, &blip);
					if (targetPtr->colorBlip != 6)
					{
						CRadar::ChangeBlipColour(blip, targetPtr->colorBlip - 1);
					}
					else
					{
						CRadar::SetBlipFriendly(blip, 1);
					}
				}

				std::string str_text = targetPtr->text;
				str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
				GXTEncode(str_text);
				CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->textTime * 1000, 0, false);
					
				switch (targetPtr->typeUse)
				{
				case 0:

					while (!Command<Commands::IS_CHAR_TOUCHING_OBJECT>(playerPed,mission->list_objects[targetPtr->object]->missionObject) && mission_started)
					{
						this_coro::wait(0ms);
					}

					break;
				case 1:
					while (mission->list_objects[targetPtr->object]->missionObject->m_nLastWeaponDamage == 255 && mission_started)
					{
						this_coro::wait(0ms);
					}
					mission->list_objects[targetPtr->object]->missionObject->m_nLastWeaponDamage = 255;
					break;
				case 2:

					while (!Command<Commands::HAS_OBJECT_BEEN_PHOTOGRAPHED>(mission->list_objects[targetPtr->object]->missionObject) && mission_started)
					{
						this_coro::wait(0ms);
					}

					break;
				case 3:

					while (!Command<Commands::HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON>(mission->list_objects[targetPtr->object]->missionObject,targetPtr->weapon) && mission_started)
					{
						this_coro::wait(0ms);
					}

					break;
				}
				if (blip != NULL)
				{
					CRadar::ClearBlip(blip);
				}

				
					
				break;
			}
			case 5: {
				TargetPickup* targetPtr = static_cast<TargetPickup*>(mission->list_targets[current_mission_target]);
				std::string str_text = targetPtr->text;
				str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
				GXTEncode(str_text);
				CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->textTime*1000.0f, 0, false);
				int blip = NULL;
				if (targetPtr->colorBlip > 0)
				{
					Command<Commands::ADD_BLIP_FOR_PICKUP>(mission->list_pickups[targetPtr->pickup]->missionPickup, &blip);
					if (targetPtr->colorBlip != 6)
					{
						CRadar::ChangeBlipColour(blip, targetPtr->colorBlip - 1);
					}
					else
					{
						CRadar::SetBlipFriendly(blip, 1);
					}
				}
				while (!Command<Commands::HAS_PICKUP_BEEN_COLLECTED>(mission->list_pickups[targetPtr->pickup]->missionPickup) && mission_started)
				{
					this_coro::wait(0ms);
				}
				if (blip != NULL)
				{
					CRadar::ClearBlip(blip);
				}
				break;
			}
			case 6: {

				switch (mission->list_targets[current_mission_target]->targetType)
				{
					case 0: {
						TargetTeleport* targetPtr = static_cast<TargetTeleport*>(mission->list_targets[current_mission_target]);

						if (Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed))
						{
							Command<Commands::TASK_LEAVE_ANY_CAR>(playerPed);
							while (Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) && mission_started)
							{
								this_coro::wait(0ms);
							}
						}
						Command<Commands::SET_CHAR_AREA_VISIBLE>(playerPed, targetPtr->interiorID);
						Command<Commands::SET_AREA_VISIBLE>(targetPtr->interiorID);

						int modell;
						if (targetPtr->modelType == 0) {
							modell = targetPtr->modelID;
							CStreaming::RequestModel(modell, 0);
							CStreaming::LoadAllRequestedModels(false);
						}
						else if (targetPtr->modelType == 1) {
							const std::string modell_n = ID_Spec_Actors[targetPtr->modelID];
							CStreaming::RequestSpecialChar(9, modell_n.c_str(), 0);
							CStreaming::LoadAllRequestedModels(false);
							modell = 290 + 10 - 1;
						}
						Command<Commands::SET_PLAYER_MODEL>(0, modell);
						playerPed->SetPosn(targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2]);
						int weap_modell;
						Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[targetPtr->weapon], &weap_modell);
						if (weap_modell != -1) {
							CStreaming::RequestModel(weap_modell, 0);
							CStreaming::LoadAllRequestedModels(false);
						}
						playerPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[targetPtr->weapon]), 1, false);
						playerPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[targetPtr->weapon]));
						CStreaming::RemoveAllUnusedModels();
						playerPed->m_fHeadingChangeRate = rad(targetPtr->angle);
							
						break;
					}
					case 1: {
						TargetAnimation* targetPtr = static_cast<TargetAnimation*>(mission->list_targets[current_mission_target]);

						Command<Commands::SET_PLAYER_CONTROL>(0, 0);
							
						if (!Command<Commands::HAS_ANIMATION_LOADED>(Anim_name[targetPtr->pack].c_str()))
							Command<Commands::REQUEST_ANIMATION>(Anim_name[targetPtr->pack].c_str());
						vector<std::string> anims = Anim_list[targetPtr->pack];
						Command<Commands::TASK_PLAY_ANIM>(playerPed, anims[targetPtr->anim].c_str(), Anim_name[targetPtr->pack].c_str(), 1.0f, targetPtr->looped, false, false, false, -1);

						Command<Commands::SET_PLAYER_CONTROL>(0, 1);
							
						break;
					}
					case 2: {
						TargetTeleportToVehicle* targetPtr = static_cast<TargetTeleportToVehicle*>(mission->list_targets[current_mission_target]);
						if (targetPtr->carPlace == 0)
							Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(playerPed, mission->list_cars[targetPtr->car]->missionCar);
						else
							Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(playerPed, mission->list_cars[targetPtr->car]->missionCar, targetPtr->carPlace - 1);
						break;
					}
					case 3: {
						TargetLevelWanted* targetPtr = static_cast<TargetLevelWanted*>(mission->list_targets[current_mission_target]);
						CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevel(targetPtr->levelWanted);
						break;
					}
					case 4: {
						playerPed->ClearWeapons();
						break;
					}
					case 5: {
						TargetDialog* targetPtr = static_cast<TargetDialog*>(mission->list_targets[current_mission_target]);

						CStreaming::RequestModel(330, 0);
						CStreaming::LoadAllRequestedModels(false);

						int phone;
						Command<Commands::CREATE_OBJECT>(330, 0, 0, 0, &phone);
						this_coro::wait(0ms);

						Command<Commands::TASK_PICK_UP_SECOND_OBJECT>(playerPed, phone, 0, 0, 0, 6, 16, "phone_in", "PED", -1);
						this_coro::wait(900ms);
						Command<Commands::TASK_PICK_UP_OBJECT>(playerPed, phone, 0, 0, 0, 6, 16, "NULL", "NULL", -1);
						this_coro::wait(1430ms);
						skip = 1;

						bool useSkip = true;

						instance.add_to_queue([&useSkip, &skip]
						{
							while (useSkip && mission_started)
							{
								if (KeyJustPressed(VK_SPACE))
								{
									skip = 0;
								}
								this_coro::wait(0ms);
							}
						});

						for (auto dialog : targetPtr->dialogs)
						{
							std::string str_text = dialog.text;
							str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
							GXTEncode(str_text);
							CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), dialog.textTime*1000, 0, false);
							this_coro::wait(dialog.textTime*1000*skip);
						}
						useSkip = false;

						Command<Commands::REMOVE_OBJECT_ELEGANTLY>(phone);
						int phone2;
						Command<Commands::CREATE_OBJECT>(330, 0, 0, 0, &phone2);

						Command<Commands::TASK_PICK_UP_SECOND_OBJECT>(playerPed, phone2, 0, 0, 0, 6, 16, "phone_out", "PED", -1);
						Command<Commands::TASK_PICK_UP_OBJECT>(playerPed, phone2, 0, 0, 0, 6, 16, "NULL", "NULL", -1);
						this_coro::wait(1570ms);
						Command<Commands::REMOVE_OBJECT_ELEGANTLY>(phone2);
						this_coro::wait(430ms);
						Command<Commands::TASK_PICK_UP_SECOND_OBJECT>(playerPed, phone2, 0, 0, 0, 6, 16, "phone_out", "PED", -1);
							
						break;
					}
					case 6: {
						TargetMoney* targetPtr = static_cast<TargetMoney*>(mission->list_targets[current_mission_target]);

						CWorld::Players[0].m_nMoney += targetPtr->money;
							
						break;
					}
				}
					
				break;
			}
			case 7: {
				while (!nodeSignal && mission_started)
				{
					this_coro::wait(0ms);
				}
				nodeSignal = false;
			}
		}

		for (int i = 0; i < mission->list_actors.size(); ++i)
		{
			if (mission->list_actors[i]->useTarget)
			{
				if (mission->list_actors[i]->endC != 0 && mission->list_actors[i]->endC-1 == current_mission_target)
				{
					mission->list_actors[i]->removeMissionPed();
				}
			}
		}
		for (int i = 0; i < mission->list_cars.size(); ++i)
		{
			if (mission->list_cars[i]->useTarget)
			{
				if (mission->list_cars[i]->endC != 0 && mission->list_cars[i]->endC-1 == current_mission_target)
				{
					mission->list_cars[i]->removeMissionCar();
				}
			}
		}
		for (int i = 0; i < mission->list_objects.size(); ++i)
		{
			if (mission->list_objects[i]->useTarget)
			{
				if (mission->list_objects[i]->endC != 0 && mission->list_objects[i]->endC-1 == current_mission_target)
				{
					mission->list_objects[i]->removeMissionObject();
				}
			}
		}
		for (int i = 0; i < mission->list_particles.size(); ++i)
		{
			if (mission->list_particles[i]->useTarget)
			{
				if (mission->list_particles[i]->endC != 0 && mission->list_particles[i]->endC-1 == current_mission_target)
				{
					mission->list_particles[i]->removeMissionParticle();
				}
			}
		}
		for (int i = 0; i < mission->list_pickups.size(); ++i)
		{
			if (mission->list_pickups[i]->useTarget)
			{
				if (mission->list_pickups[i]->endC != 0 && mission->list_pickups[i]->endC-1 == current_mission_target)
				{
					mission->list_pickups[i]->removeMissionPickup();
				}
			}
		}
		for (int i = 0; i < mission->list_explosions.size(); ++i)
		{
			if (mission->list_explosions[i]->useTarget)
			{
				if (mission->list_explosions[i]->endC != 0 && mission->list_explosions[i]->endC-1 == current_mission_target)
				{
					mission->list_explosions[i]->removeMissionExplosion();
				}
			}
		}
		for (int i = 0; i < mission->list_audios.size(); ++i)
		{
			if (mission->list_audios[i]->useTarget)
			{
				if (mission->list_audios[i]->endC != 0 && mission->list_audios[i]->endC-1 == current_mission_target)
				{
					mission->list_audios[i]->stop();
				}
			}
		}
		
		current_mission_target++;
	}

	//unload audio
	for (auto audio : mission->list_audios)
	{
		audio->unloadAudio();
	}

	if (!defeat)
	{
		respectMission();
	}
	mission_started = false;
	CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 0;
	this_coro::wait(1500ms);

	if (clearSelf)
	{
		if (Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed))
		{
			Command<Commands::TASK_LEAVE_ANY_CAR>(playerPed);
			while (Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed))
			{
				this_coro::wait(0ms);
			}
		}
		mission->removeMissionEntity();
		missionActors.clear();
		missionVehicle.clear();
		missionObject.clear();
		missionParticle.clear();
		missionPickup.clear();
		missionExplosion.clear();
		missionAudio.clear();
		playerPed->m_fCurrentRotation = static_cast<float>(rad(mission->player.angle));
		playerPed->SetPosn(mission->player.pos[0], mission->player.pos[1], mission->player.pos[2]);
		Command<Commands::REQUEST_COLLISION>(mission->player.pos[0], mission->player.pos[1]);
		CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(0);
		CWorld::Players[0].m_PlayerData.m_pWanted->SetMaximumWantedLevel(0);
		CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevel(0);
		CWorld::ClearExcitingStuffFromArea(playerPed->GetPosition(), 1000, 1);
		playerPed->m_fMaxHealth = 100.0f;
		playerPed->m_fHealth = 100.0f;
		Command<Commands::SET_CHAR_AREA_VISIBLE>(playerPed, mission->player.interiorID);
		Command<Commands::SET_PLAYER_MODEL>(0, 0);
		playerPed->ClearWeapons();
		Command<Commands::SET_LA_RIOTS>(false);
		Command<Commands::SET_PED_DENSITY_MULTIPLIER>(0);
		Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(0);
		mission->updateEditorEntity();
	}
}


void start_storyline_mission(std::string& mission_name)
{
	const std::string path = "LDYOM//Missions_packs//";
	std::stringstream full_path;
	full_path << path << replace_symb(UTF8_to_CP1251(currentStorylinePtr->missPack)) << "//" << replace_symb(UTF8_to_CP1251(mission_name)) << ".bin";
	Mission* mission = Manager::LoadMission(full_path.str());

	mission_started = true;
	defeat = false;

	while (mission_started || defeat)
	{
		mission_started = true;
		defeat = false;
		instance.add_to_queue(std::bind(MissionPlayer::start_mission, mission, false));
		while (!defeat && mission_started)
		{
			this_coro::wait(0ms);
		}
	}
	delete mission;
}

void MissionPlayer::start_storyline()
{
	storyline_started = true;
	int pack_idx = 0;
	int last_mission = -1;
	
	for (int i = 0; i < namesMissionPacks.size(); ++i)
	{
		if (namesMissionPacks[i] == currentStorylinePtr->missPack)
		{
			pack_idx = i;
			break;
		}
	}

	auto& missions_name_list = namesMissions[pack_idx];

	if (currentStorylinePtr->startMission > 0)
	{
		auto& start_mission_name = namesMissions[pack_idx][currentStorylinePtr->startMission - 1];
		start_storyline_mission(start_mission_name);
		last_mission = currentStorylinePtr->startMission - 1;
	}

	while (storyline_started)
	{
		this_coro::wait(0ms);

		for (auto check : currentStorylinePtr->list_checkpoints)
		{
			if (check->useMission)
			{
				if (check->startC == last_mission)
				{
					if (check->markerPlay == NULL)
					{
						Command<Commands::ADD_SPRITE_BLIP_FOR_CONTACT_POINT>(check->pos[0], check->pos[1], check->pos[2], check->iconMarker, &check->markerPlay);
						Command<Commands::CHANGE_BLIP_SCALE>(check->markerPlay, 3);
						if (check->colorBlip > 0)
						{
							Command<Commands::CHANGE_BLIP_DISPLAY>(check->markerPlay, 3);
							CRadar::ChangeBlipColour(check->markerPlay, check->colorBlip - 1);
						}
						else
							Command<Commands::CHANGE_BLIP_DISPLAY>(check->markerPlay, 1);
					}
					else {
						if (Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(playerPed, check->pos[0], check->pos[1], check->pos[2], 2.0f, 2.0f, 2.0f, false))
						{
							unsigned char times[6][2] = { {20,6},{12,5},{9,17},{7,18},{22,6},{12,20} };
							auto hours = CClock::ms_Stored_nGameClockHours;
							auto minutes = CClock::ms_Stored_nGameClockMinutes;
							if (check->timeStart > 0 && !(hours >= times[check->timeStart][0] && hours < times[check->timeStart][1]))
							{
								std::string gxt = "MTIME";
								gxt.append(std::to_string(check->timeStart));
								unsigned int textPtr;
								Command<0x0ADE>(gxt.c_str(), &textPtr);
								CHud::SetHelpMessage(reinterpret_cast<char*>(textPtr), false, false, false);
								CHud::DrawHelpText();
							}
							else
							{
								for (auto blip_remove : currentStorylinePtr->list_checkpoints) {
									if (blip_remove->markerPlay != NULL)
									{
										CRadar::ClearBlip(blip_remove->markerPlay);
										blip_remove->markerPlay = NULL;
									}
								}
								auto& mission_name = namesMissions[pack_idx][check->gotoMission];
								start_storyline_mission(mission_name);
								last_mission = check->gotoMission;
							}
						}
					}
				}
			}
		}

		if (currentStorylinePtr->endMission > 0)
		{
			if (currentStorylinePtr->endMission-1 == last_mission)
			{
				storyline_started = false;
			}
		}
		
		if (Command<0x0ADC>("help"))
		{
			CHud::SetHelpMessage("HELPING!", false, false, false);
			CHud::DrawHelpText();
			storyline_started = false;
			mission_started = false;
		}
	}

	if (Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed))
	{
		Command<Commands::TASK_LEAVE_ANY_CAR>(playerPed);
		while (Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed))
		{
			this_coro::wait(0ms);
		}
	}
	for (auto actor : missionActors)
	{
		CWorld::Remove(actor);
		actor->Remove();
	}
	missionActors.clear();
	for (auto vehicle : missionVehicle)
	{
		CWorld::Remove(vehicle);
		vehicle->Remove();
	}
	missionVehicle.clear();
	for (auto object : missionObject)
	{
		CWorld::Remove(object);
		object->Remove();
	}
	missionObject.clear();
	for (auto particle : missionParticle)
	{
		Command<Commands::KILL_FX_SYSTEM>(particle.first);
		CWorld::Remove(particle.second);
		particle.second->Remove();
	}
	missionParticle.clear();
	for (auto pickup : missionPickup)
	{
		CPickups::RemovePickUp(pickup);
	}
	missionPickup.clear();
	for (auto explosion : missionExplosion)
	{
		Command<Commands::REMOVE_SCRIPT_FIRE>(explosion);
	}
	missionExplosion.clear();
	for (auto audio : missionAudio)
	{
		Command<0x0AAE>(audio);
	}
	missionAudio.clear();
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(0);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetMaximumWantedLevel(0);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevel(0);
	CWorld::ClearExcitingStuffFromArea(playerPed->GetPosition(), 1000, 1);
	playerPed->m_fMaxHealth = 100.0f;
	playerPed->m_fHealth = 100.0f;
	Command<Commands::SET_PLAYER_MODEL>(0, 0);
	playerPed->ClearWeapons();
	Command<Commands::SET_LA_RIOTS>(false);
	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(0);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(0);
}