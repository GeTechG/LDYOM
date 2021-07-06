#include "MissionPlayer.h"


#include <CClothes.h>
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
#include "ScriptManager.h"

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
extern NodeGraph* currentNodeGraphPtr;

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
map<unsigned, sol::object> realVariable;
bool loadedNodes = false;
bool useSkip = false;

void stopStoryline()
{
	storyline_started = false;
}

void setNodeSignal(bool value)
{
	nodeSignal = value;
}

bool getMissionStarted()
{
	return mission_started;
}

void setMissionStarted(bool value)
{
	mission_started = value;
}

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
			playerPed->SetModelIndex(0);
			while (CWorld::Players[0].m_nPlayerState == ePlayerState::PLAYERSTATE_PLAYING)
			{
				this_coro::wait(0ms);
			}
			this_coro::wait(3s);
			defeat = true;
			mission_started = false;
			Command<Commands::SET_CHAR_PROOFS>(playerPed, 1, 1, 1, 1, 1);
			CMessages::ClearMessages(true);
			failMission();
		}
		if (Command<0x0ADC>("ldstop"))
		{
			defeat = true;
			mission_started = false;
			Command<Commands::SET_CHAR_PROOFS>(playerPed, 1, 1, 1, 1, 1);
			CMessages::ClearMessages(true);
		}
	}
}

int current_mission_target = 0;
bool break_target = false;

extern void addTimer(std::string text, bool direction, int value);
extern void removeTimer();
extern int getTimerTime();
extern int spaceVarsPtr;
extern int timer;
extern bool timerOn;

void MissionPlayer::start_mission(Mission* mission, NodeGraph* graph, bool clearSelf)
{
	assert(CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] == 0);
	CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 1;
	graph->lastNode = -1;

	//start
	timerOn = false;
	defeat = false;
	mission_started = true;
	TheCamera.Fade(0.2f, 0);
	this_coro::wait(200ms);

	//set player
	Command<Commands::SET_CHAR_HEADING>(playerPed, mission->player.angle);
	playerPed->SetPosn(mission->player.pos[0], mission->player.pos[1], mission->player.pos[2]);
	Command<Commands::REQUEST_COLLISION>(mission->player.pos[0], mission->player.pos[1]);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(mission->wanted_min);
	CWanted::SetMaximumWantedLevel(mission->wanted_max);
	Command<Commands::SET_CHAR_PROOFS>(playerPed, 0, 0, 0, 0, 0);
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
		CStreaming::RequestSpecialChar(8, modell_n.c_str(), 0);
		CStreaming::LoadAllRequestedModels(false);
		modell = 290 + 9 - 1;
	}
	Command<Commands::SET_PLAYER_MODEL>(0, modell);
	int weap_modell;
	Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[mission->player.weapon], &weap_modell);
	if (weap_modell != -1) {
		CStreaming::RequestModel(weap_modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	playerPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[mission->player.weapon]), mission->player.ammo, false);
	playerPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[mission->player.weapon]));
	CStreaming::RemoveAllUnusedModels();

	if (mission->player.modelType == 0 && mission->player.modelID == 0)
	{
		auto playerClothers = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothers->m_anTextureKeys, mission->player.clotherM_anTextureKeys, sizeof(playerClothers->m_anTextureKeys));
		std::memcpy(playerClothers->m_anModelKeys, mission->player.clotherM_anModelKeys, sizeof(playerClothers->m_anModelKeys));
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}
	
	//show name mission.
	
	std::string name_miss = is_utf8(mission->name.c_str())? UTF8_to_CP1251(mission->name) : mission->name;
	GXTEncode(name_miss);
	CMessages::AddBigMessage(const_cast<char*>(name_miss.c_str()), 2000, 1);

	//set world
	CWeather::ForceWeatherNow(mission->weather);
	Command<Commands::SET_AREA_VISIBLE>(mission->player.interiorID);
	CClock::SetGameClock(mission->time[0], mission->time[1], CClock::CurrentDay);
	Command<Commands::SET_LA_RIOTS>(mission->riot);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (mission->groupRelations[i][j] != -1)
			{
				if (j == 0)
				{
					Command<Commands::SET_RELATIONSHIP>((int)mission->groupRelations[i][j], 24 + i, 0);
					Command<Commands::SET_RELATIONSHIP>((int)mission->groupRelations[i][j], 24 + i, 23);
					Command<Commands::SET_RELATIONSHIP>((int)mission->groupRelations[i][j], 23, 24 + i);
				} else
				{
					if (i != j - 1) {
						Command<Commands::SET_RELATIONSHIP>((int)mission->groupRelations[i][j], 24 + i, 24 + j-1);
					}
				}
			}
		}
	}

	//load audio
	for (auto audio : mission->list_audios)
	{
		audio->loadAudio();
	}
	
	this_coro::wait(300ms);
	TheCamera.Fade(0.2f, 1);

	float ped_density = mission->traffic_ped;
	float car_density = mission->traffic_car;
	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(ped_density);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(car_density);
	
	instance.add_to_queue(testDefeat);

	if (clearSelf) {
		realVariable.clear();
		for (auto lua_script : ScriptManager::lua_scripts) {
			if (lua_script.first)
			{
				sol::optional<std::string> name = lua_script.second["info"]["name"];
				if (name.value()._Equal("Main nodes")) {
					for (auto var_pair : graph->vars) {
						sol::object type_var = var_pair.second["typeValue"];
						sol::object value_var = var_pair.second["value"];
						if (*(unsigned char*)type_var.pointer() == 0) {
							sol::protected_function func = lua_script.second["ffi"]["new"];
							auto result = func("float[1]", *(float*)value_var.pointer());
							ScriptManager::checkProtected(result);
							realVariable[var_pair.first] = result;
						} else if (*(unsigned char*)type_var.pointer() == 1) {
							sol::protected_function func = lua_script.second["ffi"]["new"];
							realVariable[var_pair.first] = func("char[128]", (char*)value_var.pointer());
						} else if (*(unsigned char*)type_var.pointer() == 2) {
							sol::protected_function func = lua_script.second["ffi"]["new"];
							realVariable[var_pair.first] = func("bool[1]", *(bool*)value_var.pointer());
						}
					}
				}
			}
		}
	}

	
	unsigned char skip = 1;


	if (!mission->list_targets.empty()) {
		vector<pair<vector<int>, vector<int>>> arr_target_actor(mission->list_targets.size());
		for (int i = 0; i < mission->list_actors.size(); ++i)
		{
			if (mission->list_actors[i]->useTarget) {
				int startC = min(mission->list_actors[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_actors[i]->endC, mission->list_targets.size());
				arr_target_actor[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_actor[endC-1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_car(mission->list_targets.size());
		for (int i = 0; i < mission->list_cars.size(); ++i)
		{
			if (mission->list_cars[i]->useTarget) {
				int startC = min(mission->list_cars[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_cars[i]->endC, mission->list_targets.size());
				arr_target_car[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_car[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_train(mission->list_targets.size());
		for (int i = 0; i < mission->list_trains.size(); ++i)
		{
			if (mission->list_trains[i]->useTarget) {
				int startC = min(mission->list_trains[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_trains[i]->endC, mission->list_targets.size());
				arr_target_train[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_train[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_object(mission->list_targets.size());
		for (int i = 0; i < mission->list_objects.size(); ++i)
		{
			if (mission->list_objects[i]->useTarget) {
				int startC = min(mission->list_objects[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_objects[i]->endC, mission->list_targets.size());
				arr_target_object[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_object[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_particle(mission->list_targets.size());
		for (int i = 0; i < mission->list_particles.size(); ++i)
		{
			if (mission->list_particles[i]->useTarget) {
				int startC = min(mission->list_particles[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_particles[i]->endC, mission->list_targets.size());
				arr_target_particle[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_particle[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_pickup(mission->list_targets.size());
		for (int i = 0; i < mission->list_pickups.size(); ++i)
		{
			if (mission->list_pickups[i]->useTarget) {
				int startC = min(mission->list_pickups[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_pickups[i]->endC, mission->list_targets.size());
				arr_target_pickup[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_pickup[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_explosion(mission->list_targets.size());
		for (int i = 0; i < mission->list_explosions.size(); ++i)
		{
			if (mission->list_explosions[i]->useTarget) {
				int startC = min(mission->list_explosions[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_explosions[i]->endC, mission->list_targets.size());
				arr_target_explosion[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_explosion[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_audio(mission->list_targets.size());
		for (int i = 0; i < mission->list_audios.size(); ++i)
		{
			if (mission->list_audios[i]->useTarget) {
				int startC = min(mission->list_audios[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_audios[i]->endC, mission->list_targets.size());
				arr_target_audio[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_audio[endC - 1].second.emplace_back(i);
			}
		}

		vector<pair<vector<int>, vector<int>>> arr_target_visualEffect(mission->list_targets.size());
		for (int i = 0; i < mission->list_visualEffects.size(); ++i)
		{
			mission->list_visualEffects[i]->drawing = false;
			if (mission->list_visualEffects[i]->useTarget) {
				int startC = min(mission->list_visualEffects[i]->startC, mission->list_targets.size() - 1);
				int endC = min(mission->list_visualEffects[i]->endC, mission->list_targets.size());
				arr_target_visualEffect[startC].first.emplace_back(i);
				if (endC != 0)
					arr_target_visualEffect[endC - 1].second.emplace_back(i);
			}
		}

		loadedNodes = false;

		
		
		std::vector<std::string> labels;

		instance.add_to_queue([&labels,&mission]()
		{
			this_coro::wait(100ms);
			for (auto start_label : mission->startLabels)
			{
				std::string& str_text = labels.emplace_back(start_label.first);
				str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
				GXTEncode(str_text);
				CHud::SetHelpMessage(str_text.c_str(), true, false, false);
				this_coro::wait(start_label.second * 1000);
			}
		});
		
		
		//targets loop
		while (current_mission_target < mission->list_targets.size() && mission_started)
		{
			for (int actor_targ : arr_target_actor[current_mission_target].first)
			{
				instance.add_to_queue([&mission,actor_targ, &graph]{
					mission->list_actors[actor_targ]->updateMissionPed();
					missionActors.push_back(mission->list_actors[actor_targ]->missionPed);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperActor");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == actor_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_actor = lua_script.second["apper_actor"];
					if (apper_actor.valid())
					{
						auto result = apper_actor(arr_target_actor[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int car_targ : arr_target_car[current_mission_target].first)
			{
				instance.add_to_queue([&mission, car_targ, &graph] {
					mission->list_cars[car_targ]->updateMissionCar();
					missionVehicle.push_back(mission->list_cars[car_targ]->missionCar);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperCar");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == car_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_car = lua_script.second["apper_car"];
					if (apper_car.valid())
					{
						auto result = apper_car(arr_target_car[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}

			for (int train_targ : arr_target_train[current_mission_target].first)
			{
				instance.add_to_queue([&mission, train_targ, &graph] {
					mission->list_trains[train_targ]->updateMissionTrain();
					missionVehicle.push_back(mission->list_trains[train_targ]->missionTrain);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperTrain");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == train_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_train = lua_script.second["apper_train"];
					if (apper_train.valid())
					{
						auto result = apper_train(arr_target_train[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}

			for (int object_targ : arr_target_object[current_mission_target].first)
			{
				instance.add_to_queue([&mission, object_targ, &graph] {
					mission->list_objects[object_targ]->updateMissionObject();
					missionObject.push_back(mission->list_objects[object_targ]->missionObject);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperObject");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == object_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_object = lua_script.second["apper_object"];
					if (apper_object.valid())
					{
						auto result = apper_object(arr_target_object[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int particle_targ : arr_target_particle[current_mission_target].first)
			{
				instance.add_to_queue([&mission, particle_targ, &graph] {
					mission->list_particles[particle_targ]->updateMissionParticle(mission);
					missionParticle.push_back(mission->list_particles[particle_targ]->missionParticle);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperParticle");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == particle_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_particle = lua_script.second["apper_particle"];
					if (apper_particle.valid())
					{
						auto result = apper_particle(arr_target_particle[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int pickup_targ : arr_target_pickup[current_mission_target].first)
			{
				instance.add_to_queue([&mission, pickup_targ, &graph] {
					mission->list_pickups[pickup_targ]->updateMissionPickup();
					missionPickup.push_back(mission->list_pickups[pickup_targ]->missionPickup);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperPickup");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == pickup_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_pickup = lua_script.second["apper_pickup"];
					if (apper_pickup.valid())
					{
						auto result = apper_pickup(arr_target_pickup[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}

			for (int explosion_targ : arr_target_explosion[current_mission_target].first)
			{
				instance.add_to_queue([&mission, explosion_targ, &graph] {
					mission->list_explosions[explosion_targ]->updateMissionExplosion();
					missionExplosion.push_back(mission->list_explosions[explosion_targ]->missionFire);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperExplosion");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == explosion_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_explosion = lua_script.second["apper_explosion"];
					if (apper_explosion.valid())
					{
						auto result = apper_explosion(arr_target_explosion[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int audio_targ : arr_target_audio[current_mission_target].first)
			{
				instance.add_to_queue([&mission, audio_targ, &graph] {
					mission->list_audios[audio_targ]->play(mission);
					missionAudio.push_back(mission->list_audios[audio_targ]->missionAudio);
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperAudio");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == audio_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_audio = lua_script.second["apper_audio"];
					if (apper_audio.valid())
					{
						auto result = apper_audio(arr_target_audio[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}

			for (int visualEffect_targ : arr_target_visualEffect[current_mission_target].first)
			{
				instance.add_to_queue([&mission, visualEffect_targ, &graph] {
					mission->list_visualEffects[visualEffect_targ]->drawing = true;
					for (auto node : graph->nodes)
					{
						std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeApperVisualEffect");
						sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
						if (node_name.has_value())
						{
							if (node_name.value()._Equal(name))
							{
								sol::object value = node.second["Pins"][node.first + 1]["value"];
								if ((*(int*)value.pointer()) == visualEffect_targ)
									instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
							}
						}
					}
				});
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function apper_visualEffect = lua_script.second["apper_visualEffect"];
					if (apper_visualEffect.valid())
					{
						auto result = apper_visualEffect(arr_target_visualEffect[current_mission_target].first);
						ScriptManager::checkProtected(result);
					}
				}
			}

			this_coro::wait(1ms);

			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function target_ended = lua_script.second["target_started"];
					if (target_ended.valid())
					{
						auto result = target_ended(current_mission_target);
						ScriptManager::checkProtected(result);
					}
				}
			}
			for (auto node : graph->nodes)
			{
				std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeStartedTarget");
				sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
				if (node_name.has_value())
				{
					if (node_name.value()._Equal(name))
					{
						sol::object value = node.second["Pins"][node.first + 1]["value"];
						if ((*(int*)value.pointer()) == current_mission_target)
							instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
					}
				}
			}
			for (auto node : graph->nodes)
			{
				std::string name = NodeGraph::getNodeIcon("loop") + " " + langt("CoreNodeTargetCycle");
				sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
				if (node_name.has_value())
				{
					if (node_name.value()._Equal(name))
					{
						sol::object value = node.second["Pins"][node.first + 1]["value"];
						if ((*(int*)value.pointer()) == current_mission_target)
							instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
					}
				}
			}
			
			if (!loadedNodes)
			{
				//call start node
				std::string name_node = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeStart");
				NodeGraph::callAllNodeName(name_node, graph, mission);
				//call main loop
				name_node = NodeGraph::getNodeIcon("loop") + " " + langt("CoreNodeMainCycle");
				NodeGraph::callAllNodeName(name_node, graph, mission);

				loadedNodes = true;
			}
			
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
					unsigned sphere = CTheScripts::AddScriptSphere(600, CVector(targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2]), targetPtr->radius);
					switch (targetPtr->onWhat)
					{
					case 0:
						{
							while (!Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(playerPed, targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], targetPtr->radius, targetPtr->radius, targetPtr->radius, false) && mission_started)
							{
								if (break_target)
								{
									break_target = false;
									break;
								}
								this_coro::wait(0ms);
								CTheScripts::DrawScriptSpheres();
							}
							break;
						}
					case 1:
						{
							while (!Command<Commands::LOCATE_CHAR_ON_FOOT_3D>(playerPed, targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], targetPtr->radius, targetPtr->radius, targetPtr->radius, false) && mission_started)
							{
								if (break_target)
								{
									break_target = false;
									break;
								}
								this_coro::wait(0ms);
								CTheScripts::DrawScriptSpheres();
							}
							break;
						}
					case 2:
						{
							while (!Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], targetPtr->radius, targetPtr->radius, targetPtr->radius, false) && mission_started)
							{
								if (break_target)
								{
									break_target = false;
									break;
								}
								this_coro::wait(0ms);
								CTheScripts::DrawScriptSpheres();
							}
							break;
						}
					case 3:
						{
							while (mission_started)
							{
								if (break_target)
								{
									break_target = false;
									break;
								}
								if (!Command<Commands::IS_CHAR_IN_CAR>(playerPed, mission->list_cars[targetPtr->comeBackVehicle]->missionCar))
								{
									std::string str_textComeBack = targetPtr->textComeBackVehicle;
									str_textComeBack = is_utf8(str_textComeBack.c_str()) ? UTF8_to_CP1251(str_textComeBack) : str_textComeBack;
									GXTEncode(str_textComeBack);
									CMessages::AddMessage(const_cast<char*>(str_textComeBack.c_str()), 2000.0f, 0, false);
									int blipComeBack = NULL;
									if (targetPtr->colorBlipComeBackVehicle > 0)
									{
										Command<Commands::ADD_BLIP_FOR_CAR>(mission->list_cars[targetPtr->comeBackVehicle]->missionCar, &blipComeBack);
										if (targetPtr->colorBlipComeBackVehicle != 6)
										{
											CRadar::ChangeBlipColour(blipComeBack, targetPtr->colorBlipComeBackVehicle - 1);
										}
										else
										{
											CRadar::SetBlipFriendly(blipComeBack, 1);
										}
									}
									while (!Command<Commands::IS_CHAR_IN_CAR>(playerPed, mission->list_cars[targetPtr->comeBackVehicle]->missionCar) && mission_started)
									{
										if (break_target)
										{
											break;
										}
										this_coro::wait(0ms);
									}
									if (blipComeBack != NULL)
									{
										CRadar::ClearBlip(blipComeBack);
									}
									CMessages::ClearMessages(true);
								}
								CTheScripts::DrawScriptSpheres();
								if (Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], targetPtr->radius, targetPtr->radius, targetPtr->radius, false))
								{
									break;
								}
								this_coro::wait(0ms);
							}
							break;
						}
					}
						
					CTheScripts::RemoveScriptSphere(sphere);
					if (blip != NULL)
					{
						CRadar::ClearBlip(blip);
					}
					CMessages::ClearMessages(true);
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
						if (break_target)
						{
							break_target = false;
							break;
						}
						this_coro::wait(0ms);
					}
					if (blip != NULL)
					{
						CRadar::ClearBlip(blip);
					}
					CMessages::ClearMessages(true);
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
										instance.add_to_queue(std::bind([](Actor *actor, int blip)
										{
											while (!CTheScripts::pActiveScripts->IsPedDead(actor->missionPed) && mission_started)
											{
												if (break_target)
												{
													break;
												}
												this_coro::wait(0);
											}
											CRadar::ClearBlip(blip);
										}, actor, blip));
									}
								}
							}
						}
						int count_dead = 0;
						while (count_dead < group_actors.size() && mission_started)
						{
							if (break_target)
							{
								break_target = false;
								break;
							}
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
								if (break_target)
								{
									break_target = false;
									break;
								}
								this_coro::wait(0ms);
							}
						}
						else {
							while (!CTheScripts::pActiveScripts->IsPedDead(mission->list_actors[targetPtr->actor]->missionPed) && mission_started)
							{
								if (break_target)
								{
									break_target = false;
									break;
								}
								this_coro::wait(0ms);
							}
						}
						if (blip != NULL)
						{
							CRadar::ClearBlip(blip);
						}
					}
					CMessages::ClearMessages(true);
					break;
				}
				case 3: {
					switch (mission->list_targets[current_mission_target]->targetType)
					{
						case 0: {
							TargetCutscene* targetPtr = static_cast<TargetCutscene*>(mission->list_targets[current_mission_target]);

							CHud::bScriptDontDisplayRadar = true;
							CHud::m_Wants_To_Draw_Hud = false;
							Command<Commands::SWITCH_WIDESCREEN>((int)targetPtr->widescreen);
								

							std::function start_cutscene = [&skip]() {
								TheCamera.Fade(0.5f, 0);
								this_coro::wait(1s);
								TheCamera.Fade(0.5f, 1);
								skip = 1;
								useSkip = true;

								instance.add_to_queue([&skip]
								{
									while (useSkip && mission_started)
									{
										if (KeyJustPressed(VK_SPACE))
										{
											skip = 0;
											break;
										}
										this_coro::wait(0ms);
									}
								});
							};
								
							if (current_mission_target > 0)
							{
								bool last_cutscene = mission->list_targets[current_mission_target - 1]->type == 3 && mission->list_targets[current_mission_target - 1]->targetType == 0;
								bool last_playerAnim = mission->list_targets[current_mission_target - 1]->type == 6 && mission->list_targets[current_mission_target - 1]->targetType == 1;
								
								if (!(last_cutscene || last_playerAnim))
								{
									start_cutscene();
								}
								if (!(mission->list_targets[current_mission_target - 1]->type == 6 && mission->list_targets[current_mission_target - 1]->targetType == 1))
								{
									Command<Commands::SET_PLAYER_CONTROL>(0, 0);
								}
							} else
							{
								start_cutscene();
								Command<Commands::SET_PLAYER_CONTROL>(0, 0);
							}

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
										CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);
										this_coro::wait(targetPtr->time*1000.0*skip);
										CMessages::ClearMessages(true);
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
										CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);
										this_coro::wait(targetPtr->time*1000*skip);
										CMessages::ClearMessages(true);
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
										CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);
										this_coro::wait(targetPtr->time*1000*skip);
										CMessages::ClearMessages(true);
										break;
									}
									case 3: {
										float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
										CVector& pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();

										std::string str_text = targetPtr->text;
										str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
										GXTEncode(str_text);
										CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

										unsigned int timer = 0;
										auto last_time = clock();
										if (targetPtr->moveCam)
										{

											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);

											while (timer < targetPtr->time * 1000 * skip && mission_started)
											{
												if (break_target)
												{
													break_target = false;
													break;
												}
												Command<Commands::POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, 1);
												timer += clock() - last_time;
												last_time = clock();
												this_coro::wait(0ms);
											}
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);

											while (timer < targetPtr->time * 1000 * skip && mission_started)
											{
												if (break_target)
												{
													break_target = false;
													break;
												}
												Command<Commands::POINT_CAMERA_AT_POINT>(pos.x, pos.y, pos.z, 2);
												timer += clock() - last_time;
												last_time = clock();
												this_coro::wait(0ms);
											}
										}
										CMessages::ClearMessages(true);
										break;
									}
									case 4: {
										float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
										if (targetPtr->moveCam)
										{
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(xx, xy, xz, 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 2);
										}

										std::string str_text = targetPtr->text;
										str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
										GXTEncode(str_text);
										CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);
										this_coro::wait(targetPtr->time * 1000 * skip);
										CMessages::ClearMessages(true);
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
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										
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
									CMessages::ClearMessages(true);
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
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
									CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
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
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
									CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
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
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
									CVector& obj_pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
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
									CMessages::ClearMessages(true);
									break;
								}
								case 4: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->list_actors[targetPtr->tiedID]->pos[0], y1 = mission->list_actors[targetPtr->tiedID]->pos[1], z1 = mission->list_actors[targetPtr->tiedID]->pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = mission->list_actors[targetPtr->tiedID]->missionPed->GetPosition();
									CVector& actor_pos = playerPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
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
									float x1 = mission->list_cars[targetPtr->tiedID]->pos[0], y1 = mission->list_cars[targetPtr->tiedID]->pos[1], z1 = mission->list_cars[targetPtr->tiedID]->pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
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
									CMessages::ClearMessages(true);
									break;
								}
								case 1: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->list_cars[targetPtr->tiedID]->pos[0], y1 = mission->list_cars[targetPtr->tiedID]->pos[1], z1 = mission->list_cars[targetPtr->tiedID]->pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
									CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
									break;
								}
								case 2: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->list_cars[targetPtr->tiedID]->pos[0], y1 = mission->list_cars[targetPtr->tiedID]->pos[1], z1 = mission->list_cars[targetPtr->tiedID]->pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
									CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
									break;
								}
								case 3: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->list_cars[targetPtr->tiedID]->pos[0], y1 = mission->list_cars[targetPtr->tiedID]->pos[1], z1 = mission->list_cars[targetPtr->tiedID]->pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

									CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
									CVector& obj_pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
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
									CMessages::ClearMessages(true);
									break;
								}
								case 4: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->list_cars[targetPtr->tiedID]->pos[0], y1 = mission->list_cars[targetPtr->tiedID]->pos[1], z1 = mission->list_cars[targetPtr->tiedID]->pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = mission->list_cars[targetPtr->tiedID]->missionCar->GetPosition();
									CVector& actor_pos = playerPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
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
										float x1 = mission->list_objects[targetPtr->tiedID]->pos[0], y1 = mission->list_objects[targetPtr->tiedID]->pos[1], z1 = mission->list_objects[targetPtr->tiedID]->pos[2];

										unsigned int timer = 0;
										auto last_time = clock();

										std::string str_text = targetPtr->text;
										str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
										GXTEncode(str_text);
										CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

										CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
										while (timer < targetPtr->time * 1000 * skip && mission_started)
										{
											if (break_target)
											{
												break_target = false;
												break;
											}
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
										CMessages::ClearMessages(true);
										break;
									}
									case 1: {
										float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
										float x1 = mission->list_objects[targetPtr->tiedID]->pos[0], y1 = mission->list_objects[targetPtr->tiedID]->pos[1], z1 = mission->list_objects[targetPtr->tiedID]->pos[2];

										unsigned int timer = 0;
										auto last_time = clock();

										std::string str_text = targetPtr->text;
										str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
										GXTEncode(str_text);
										CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

										CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
										CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
										while (timer < targetPtr->time * 1000 * skip && mission_started)
										{
											if (break_target)
											{
												break_target = false;
												break;
											}
											if (targetPtr->moveCam) {
												Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
												Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
												Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 1);
											}
											else
											{
												Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
												Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 2);
											}

											timer += clock() - last_time;
											last_time = clock();
											this_coro::wait(0ms);
										}
										CMessages::ClearMessages(true);
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
										CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

										CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
										CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
										while (timer < targetPtr->time * 1000 * skip && mission_started)
										{
											if (break_target)
											{
												break_target = false;
												break;
											}
											if (targetPtr->moveCam) {
												Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
												Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
												Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 1);
											}
											else
											{
												Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
												Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 2);
											}

											timer += clock() - last_time;
											last_time = clock();
											this_coro::wait(0ms);
										}
										CMessages::ClearMessages(true);
										break;
									}
									case 3: {
										float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
										float x1 = mission->list_objects[targetPtr->tiedID]->pos[0], y1 = mission->list_objects[targetPtr->tiedID]->pos[1], z1 = mission->list_objects[targetPtr->tiedID]->pos[2];

										unsigned int timer = 0;
										auto last_time = clock();

										std::string str_text = targetPtr->text;
										str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
										GXTEncode(str_text);
										CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);

										CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
										CVector& obj_pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();
										while (timer < targetPtr->time * 1000 * skip && mission_started)
										{
											if (break_target)
											{
												break_target = false;
												break;
											}
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
										CMessages::ClearMessages(true);
										break;
									}
									case 4: {
										float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
										float x1 = mission->list_objects[targetPtr->tiedID]->pos[0], y1 = mission->list_objects[targetPtr->tiedID]->pos[1], z1 = mission->list_objects[targetPtr->tiedID]->pos[2];

										unsigned int timer = 0;
										auto last_time = clock();

										std::string str_text = targetPtr->text;
										str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
										GXTEncode(str_text);
										CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

										CVector& curr_pos = mission->list_objects[targetPtr->tiedID]->missionObject->GetPosition();
										CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
										while (timer < targetPtr->time * 1000 * skip && mission_started)
										{
											if (break_target)
											{
												break_target = false;
												break;
											}
											if (targetPtr->moveCam) {
												Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
												Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
												Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 1);
											}
											else
											{
												Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
												Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 2);
											}

											timer += clock() - last_time;
											last_time = clock();
											this_coro::wait(0ms);
										}
										CMessages::ClearMessages(true);
										break;
									}
									break;
								}
							}
							case 4: {
								switch (targetPtr->follow) {
								case 0: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float rxx = targetPtr->rotate[0], rxy = targetPtr->rotate[1], rxz = targetPtr->rotate[2];
									float x1 = mission->player.pos[0], y1 = mission->player.pos[1], z1 = mission->player.pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = playerPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
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
									CMessages::ClearMessages(true);
									break;
								}
								case 1: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->player.pos[0], y1 = mission->player.pos[1], z1 = mission->player.pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = playerPed->GetPosition();
									CVector& actor_pos = mission->list_actors[targetPtr->followID]->missionPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(mission->list_actors[targetPtr->followID]->missionPed, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
									break;
								}
								case 2: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->player.pos[0], y1 = mission->player.pos[1], z1 = mission->player.pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = playerPed->GetPosition();
									CVector& car_pos = mission->list_cars[targetPtr->followID]->missionCar->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CAR>(mission->list_cars[targetPtr->followID]->missionCar, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
									break;
								}
								case 3: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->player.pos[0], y1 = mission->player.pos[1], z1 = mission->player.pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = playerPed->GetPosition();
									CVector& obj_pos = mission->list_objects[targetPtr->followID]->missionObject->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
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
									CMessages::ClearMessages(true);
									break;
								}
								case 4: {
									float xx = targetPtr->pos[0], xy = targetPtr->pos[1], xz = targetPtr->pos[2];
									float x1 = mission->player.pos[0], y1 = mission->player.pos[1], z1 = mission->player.pos[2];

									unsigned int timer = 0;
									auto last_time = clock();

									std::string str_text = targetPtr->text;
									str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
									GXTEncode(str_text);
									CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), targetPtr->time * 1000, 0, false);

									CVector& curr_pos = playerPed->GetPosition();
									CVector& actor_pos = playerPed->GetPosition();
									while (timer < targetPtr->time * 1000 * skip && mission_started)
									{
										if (break_target)
										{
											break_target = false;
											break;
										}
										if (targetPtr->moveCam) {
											Command<Commands::SET_INTERPOLATION_PARAMETERS>(0.0f, (int)(targetPtr->time * 1000));
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 1);
										}
										else
										{
											Command<Commands::SET_FIXED_CAMERA_POSITION>(curr_pos.x + (xx - x1), curr_pos.y + (xy - y1), curr_pos.z + (xz - z1), 0, 0, 0);
											Command<Commands::POINT_CAMERA_AT_CHAR>(playerPed, 15, 2);
										}

										timer += clock() - last_time;
										last_time = clock();
										this_coro::wait(0ms);
									}
									CMessages::ClearMessages(true);
									break;
								}
								}
								break;
							}
							break;
							}

							std::function endCutscene = []()
							{
								useSkip = false;
								TheCamera.Fade(0.5f, 0);
								this_coro::wait(0.5s);
								CHud::bScriptDontDisplayRadar = false;
								CHud::m_Wants_To_Draw_Hud = true;
								TheCamera.RestoreWithJumpCut();
								TheCamera.Fade(0.5f, 1);
								this_coro::wait(0.5s);
								Command<Commands::SET_PLAYER_CONTROL>(0, 1);
							};
								
							if (current_mission_target >= mission->list_targets.size() - 1) {
								endCutscene();
							} else
							{
								bool next_cutscene = mission->list_targets[current_mission_target + 1]->type == 3 && mission->list_targets[current_mission_target + 1]->targetType == 0;
								bool next_playerAnim = mission->list_targets[current_mission_target + 1]->type == 6 && mission->list_targets[current_mission_target + 1]->targetType == 1;
								if (!(next_cutscene || next_playerAnim))
								{
									endCutscene();
								}
							}

							Command<Commands::SWITCH_WIDESCREEN>(0);
								
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
							CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->time*1000, 0, false);
							this_coro::wait(targetPtr->time*1000);
							CMessages::ClearMessages(true);
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
						case 6: {
							TargetAddTimer* targetPtr = static_cast<TargetAddTimer*>(mission->list_targets[current_mission_target]);

							if (timerOn)
							{
								break;
							}
							
							timerOn = true;

							addTimer("LDTIMER0", targetPtr->backward, targetPtr->startTime * 1000);
								
							if (targetPtr->typeTimer == 0)
							{
								[=]()
								{
									while (mission_started && timerOn)
									{
										switch (targetPtr->compareType)
										{
										//Равно
										case 0:
											if (getTimerTime() == targetPtr->compareValue * 1000)
												return;
											break;
										//Не равно
										case 1:
											if (getTimerTime() != targetPtr->compareValue * 1000)
												return;
											break;
										//Больше
										case 2:
											if (getTimerTime() > targetPtr->compareValue * 1000)
												return;
											break;
										//Больше, или равно
										case 3:
											if (getTimerTime() >= targetPtr->compareValue * 1000)
												return;
											break;
										//Меньше
										case 4:
											if (getTimerTime() < targetPtr->compareValue * 1000)
												return;
											break;
										//Меньше, или ровно
										case 5:
											if (getTimerTime() <= targetPtr->compareValue * 1000)
												return;
											break;
										default:
											break;
										}
										this_coro::wait(0);
									}
									timerOn = false;
									removeTimer();
								}();
							} else
							{
								instance.add_to_queue([=]()
								{
									auto defeatFunc = [=]()
									{
										removeTimer();
										timerOn = false;
										defeat = true;
										mission_started = false;
										Command<Commands::SET_CHAR_PROOFS>(playerPed, 1, 1, 1, 1, 1);
										CMessages::ClearMessages(true);
										failMission();
									};
									
									while (mission_started && timerOn)
									{
										switch (targetPtr->compareType)
										{
											//Равно
										case 0:
											if (getTimerTime() == targetPtr->compareValue * 1000)
												defeatFunc();
											break;
											//Не равно
										case 1:
											if (getTimerTime() != targetPtr->compareValue * 1000)
												defeatFunc();
											break;
											//Больше
										case 2:
											if (getTimerTime() > targetPtr->compareValue * 1000)
												defeatFunc();
											break;
											//Больше, или равно
										case 3:
											if (getTimerTime() >= targetPtr->compareValue * 1000)
												defeatFunc();
											break;
											//Меньше
										case 4:
											if (getTimerTime() < targetPtr->compareValue * 1000)
												defeatFunc();
											break;
											//Меньше, или ровно
										case 5:
											if (getTimerTime() <= targetPtr->compareValue * 1000)
												defeatFunc();
											break;
										default:
											break;
										}
										this_coro::wait(0);
									}
									removeTimer();
								});
							}

							break;
						}
						case 7: {
							TargetRemoveTimer* targetPtr = static_cast<TargetRemoveTimer*>(mission->list_targets[current_mission_target]);

							timerOn = false;
							removeTimer();
							

							break;
						}
						default:
							break;
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
							if (break_target)
							{
								break_target = false;
								break;
							}
							this_coro::wait(0ms);
						}

						break;
					case 1:
						while (mission->list_objects[targetPtr->object]->missionObject->m_nLastWeaponDamage == 255 && mission_started)
						{
							if (break_target)
							{
								break_target = false;
								break;
							}
							this_coro::wait(0ms);
						}
						mission->list_objects[targetPtr->object]->missionObject->m_nLastWeaponDamage = 255;
						break;
					case 2:

						while (!Command<Commands::HAS_OBJECT_BEEN_PHOTOGRAPHED>(mission->list_objects[targetPtr->object]->missionObject) && mission_started)
						{
							if (break_target)
							{
								break_target = false;
								break;
							}
							this_coro::wait(0ms);
						}

						break;
					case 3:

						while (!Command<Commands::HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON>(mission->list_objects[targetPtr->object]->missionObject,targetPtr->weapon) && mission_started)
						{
							if (break_target)
							{
								break_target = false;
								break;
							}
							this_coro::wait(0ms);
						}

						break;
					}
					if (blip != NULL)
					{
						CRadar::ClearBlip(blip);
					}

					CMessages::ClearMessages(true);
						
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
						if (break_target)
						{
							break_target = false;
							break;
						}
						this_coro::wait(0ms);
					}
					if (blip != NULL)
					{
						CRadar::ClearBlip(blip);
					}
					CMessages::ClearMessages(true);
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
							TheCamera.Fade(0.5f, 0);
							this_coro::wait(0.5s);
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
							playerPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[targetPtr->weapon]), targetPtr->ammo, false);
							playerPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[targetPtr->weapon]));
							CStreaming::RemoveAllUnusedModels();
							Command<Commands::SET_CHAR_HEADING>(playerPed, targetPtr->angle);
							Command<Commands::REQUEST_COLLISION>(targetPtr->pos[0], targetPtr->pos[1]);

							if (targetPtr->modelType == 0 && targetPtr->modelID == 0)
							{
								auto playerClothers = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
								std::memcpy(playerClothers->m_anTextureKeys, targetPtr->clotherM_anTextureKeys, sizeof(playerClothers->m_anTextureKeys));
								std::memcpy(playerClothers->m_anModelKeys, targetPtr->clotherM_anModelKeys, sizeof(playerClothers->m_anModelKeys));
								CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
							}
								
							TheCamera.Fade(0.5f, 1);
							this_coro::wait(0.5s);
							break;
						}
						case 1: {
							TargetAnimation* targetPtr = static_cast<TargetAnimation*>(mission->list_targets[current_mission_target]);

							Command<Commands::SET_PLAYER_CONTROL>(0, 0);

							if (!Command<Commands::HAS_ANIMATION_LOADED>(Anim_name[targetPtr->pack].c_str()))
								Command<Commands::REQUEST_ANIMATION>(Anim_name[targetPtr->pack].c_str());
								
							vector<std::string> anims = Anim_list[targetPtr->pack];
								
							Command<Commands::TASK_PLAY_ANIM>(playerPed, anims[targetPtr->anim].c_str(), Anim_name[targetPtr->pack].c_str(), 4.0, targetPtr->looped, 0, 0, 0, -1);
								
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
							useSkip = true;

							instance.add_to_queue([&skip]
							{
								while (useSkip && mission_started)
								{
									if (KeyJustPressed(VK_SPACE))
									{
										skip = 0;
										break;
									}
									this_coro::wait(0ms);
								}
							});

							for (auto dialog : targetPtr->dialogs)
							{
								if (break_target)
								{
									break_target = false;
									break;
								}
								std::string str_text = dialog.text;
								str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
								GXTEncode(str_text);
								CMessages::AddMessageJumpQ(const_cast<char*>(str_text.c_str()), dialog.textTime*1000*skip, 0, false);
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

							CMessages::ClearMessages(true);
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
						if (break_target)
						{
							break_target = false;
							break;
						}
						this_coro::wait(0ms);
					}
					nodeSignal = false;
				}
				case 8:
					{
						TargetDestroyVehicle* targetPtr = static_cast<TargetDestroyVehicle*>(mission->list_targets[current_mission_target]);

						std::string str_text = targetPtr->text;
						str_text = is_utf8(str_text.c_str()) ? UTF8_to_CP1251(str_text) : str_text;
						GXTEncode(str_text);
						CMessages::AddMessage(const_cast<char*>(str_text.c_str()), targetPtr->textTime*1000.0f, 0, false);
						int blip = NULL;
						if (targetPtr->colorBlip > 0)
						{
							Command<Commands::ADD_BLIP_FOR_CAR>(mission->list_cars[targetPtr->vehicle]->missionCar, &blip);
							if (targetPtr->colorBlip != 6)
							{
								CRadar::ChangeBlipColour(blip, targetPtr->colorBlip - 1);
							}
							else
							{
								CRadar::SetBlipFriendly(blip, 1);
							}
						}
						while (mission_started)
						{
							if (break_target)
							{
								break_target = false;
								break;
							}
							if (Command<Commands::IS_CAR_DEAD>(mission->list_cars[targetPtr->vehicle]->missionCar) && targetPtr->typeDamage == 0)
							{
								break;
							}
							if (Command<Commands::IS_CAR_VISIBLY_DAMAGED>(mission->list_cars[targetPtr->vehicle]->missionCar) && targetPtr->typeDamage == 1)
							{
								break;
							}
							if (Command<Commands::IS_CAR_IN_WATER>(mission->list_cars[targetPtr->vehicle]->missionCar) && targetPtr->typeDamage == 2)
							{
								break;
							}
							this_coro::wait(0);
						}
						if (blip != NULL)
						{
							CRadar::ClearBlip(blip);
						}
						CMessages::ClearMessages(true);
						break;
					}
			}

			for (int actor_targ : arr_target_actor[current_mission_target].second)
			{
				mission->list_actors[actor_targ]->removeMissionPed();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperActor");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == actor_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_actor = lua_script.second["disapper_actor"];
					if (disapper_actor.valid())
					{
						auto result = disapper_actor(arr_target_actor[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int car_targ : arr_target_car[current_mission_target].second)
			{
				mission->list_cars[car_targ]->removeMissionCar();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperCar");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == car_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_car = lua_script.second["disapper_car"];
					if (disapper_car.valid())
					{
						auto result = disapper_car(arr_target_car[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}

			for (int train_targ : arr_target_train[current_mission_target].second)
			{
				mission->list_trains[train_targ]->removeMissionTrain();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperTrain");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == train_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_train = lua_script.second["disapper_train"];
					if (disapper_train.valid())
					{
						auto result = disapper_train(arr_target_train[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int object_targ : arr_target_object[current_mission_target].second)
			{
				mission->list_objects[object_targ]->removeMissionObject();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperObject");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == object_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_object = lua_script.second["disapper_object"];
					if (disapper_object.valid())
					{
						auto result = disapper_object(arr_target_object[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int particle_targ : arr_target_particle[current_mission_target].second)
			{
				mission->list_particles[particle_targ]->removeMissionParticle();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperParticle");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == particle_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_particle = lua_script.second["disapper_particle"];
					if (disapper_particle.valid())
					{
						auto result = disapper_particle(arr_target_particle[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int pickup_targ : arr_target_pickup[current_mission_target].second)
			{
				mission->list_pickups[pickup_targ]->removeMissionPickup();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperPickup");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == pickup_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_pickup = lua_script.second["disapper_pickup"];
					if (disapper_pickup.valid())
					{
						auto result = disapper_pickup(arr_target_pickup[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int explosion_targ : arr_target_explosion[current_mission_target].second)
			{
				mission->list_explosions[explosion_targ]->removeMissionExplosion();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperExplosion");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == explosion_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_explosion = lua_script.second["disapper_explosion"];
					if (disapper_explosion.valid())
					{
						auto result = disapper_explosion(arr_target_explosion[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			for (int audio_targ : arr_target_audio[current_mission_target].second)
			{
				mission->list_audios[audio_targ]->stop();
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperAudio");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == audio_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_audio = lua_script.second["disapper_audio"];
					if (disapper_audio.valid())
					{
						auto result = disapper_audio(arr_target_audio[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}

			for (int visualEffect_targ : arr_target_visualEffect[current_mission_target].second)
			{
				mission->list_visualEffects[visualEffect_targ]->drawing = false;
				for (auto node : graph->nodes)
				{
					std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeDisapperVisualEffect");
					sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
					if (node_name.has_value())
					{
						if (node_name.value()._Equal(name))
						{
							sol::object value = node.second["Pins"][node.first + 1]["value"];
							if ((*(int*)value.pointer()) == visualEffect_targ)
								instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
						}
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function disapper_visualEffect = lua_script.second["disapper_visualEffect"];
					if (disapper_visualEffect.valid())
					{
						auto result = disapper_visualEffect(arr_target_visualEffect[current_mission_target].second);
						ScriptManager::checkProtected(result);
					}
				}
			}


			for (auto node : graph->nodes)
			{
				std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeEndedTarget");
				sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
				if (node_name.has_value())
				{
					if (node_name.value()._Equal(name))
					{
						sol::object value = node.second["Pins"][node.first + 1]["value"];
						if ((*(int*)value.pointer()) == current_mission_target)
							instance.add_to_queue(std::bind(NodeGraph::callNode, node.second, graph, mission));
					}
				}
			}
			for (auto lua_script : ScriptManager::lua_scripts)
			{
				if (lua_script.first)
				{
					sol::protected_function target_ended = lua_script.second["target_ended"];
					if (target_ended.valid())
					{
						auto result = target_ended(current_mission_target);
						ScriptManager::checkProtected(result);
					}
				}
			}
			
			current_mission_target++;
			
			this_coro::wait(0);
		}
	}

	//unload audio
	for (auto audio : mission->list_audios)
	{
		audio->unloadAudio();
	}

	if (!defeat)
	{
		CMessages::ClearMessages(true);
		respectMission();
	}
	printLog(std::to_string(graph->lastNode));
	mission_started = false;
	Command<Commands::SET_CHAR_PROOFS>(playerPed, 1, 1, 1, 1, 1);
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
		Command<Commands::SET_PED_DENSITY_MULTIPLIER>(0.0f);
		Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(0.0f);
		TheCamera.Restore();
		TheCamera.LerpFOV(80.0f, 70.0f, 0.1f, false);
		Command<Commands::SWITCH_WIDESCREEN>(0);
		mission->updateEditorEntity();
	}
}


void start_storyline_mission(std::string& mission_name)
{
	const std::string path = "LDYOM//Missions_packs//";
	std::stringstream full_path;
	full_path << path << replace_symb(UTF8_to_CP1251(currentStorylinePtr->missPack)) << "//" << replace_symb(UTF8_to_CP1251(mission_name)) << ".bin";

	std::pair<Mission*, NodeGraph*> mission = Manager::LoadMission(full_path.str());

	mission_started = true;
	defeat = false;

	while (mission_started || defeat)
	{
		mission_started = true;
		defeat = false;
		instance.add_to_queue(std::bind(MissionPlayer::start_mission, mission.first, mission.second, false));
		while (!defeat && mission_started)
		{
			this_coro::wait(0ms);
		}
	}
	delete mission.first;
	delete mission.second;
}

int last_mission;

void MissionPlayer::start_storyline()
{
	storyline_started = true;
	int pack_idx = 0;
	last_mission = -1;
	
	for (int i = 0; i < namesMissionPacks.size(); ++i)
	{
		if (namesMissionPacks[i] == currentStorylinePtr->missPack)
		{
			pack_idx = i;
			break;
		}
	}

	auto& missions_name_list = namesMissions[pack_idx];

	std::string name_node;

	realVariable.clear();
	for (auto lua_script : ScriptManager::lua_scripts) {
		if (lua_script.first)
		{
			sol::optional<std::string> name = lua_script.second["info"]["name"];
			if (name.value()._Equal("Main nodes")) {
				for (auto var_pair : currentNodeGraphPtr->vars) {
					sol::object type_var = var_pair.second["typeValue"];
					sol::object value_var = var_pair.second["value"];
					if (*(unsigned char*)type_var.pointer() == 0) {
						sol::protected_function func = lua_script.second["ffi"]["new"];
						auto result = func("float[1]", *(float*)value_var.pointer());
						ScriptManager::checkProtected(result);
						realVariable[var_pair.first] = result;
					}
					else if (*(unsigned char*)type_var.pointer() == 1) {
						sol::protected_function func = lua_script.second["ffi"]["new"];
						realVariable[var_pair.first] = func("char[128]", (char*)value_var.pointer());
					}
					else if (*(unsigned char*)type_var.pointer() == 2) {
						sol::protected_function func = lua_script.second["ffi"]["new"];
						realVariable[var_pair.first] = func("bool[1]", *(bool*)value_var.pointer());
					}
				}
			}
		}
	}
	
	if (currentStorylinePtr->startMission > 0)
	{
		auto& start_mission_name = namesMissions[pack_idx][currentStorylinePtr->startMission - 1];
		start_storyline_mission(start_mission_name);
		last_mission = currentStorylinePtr->startMission - 1;
		for (auto node : currentNodeGraphPtr->nodes)
		{
			std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeMissionComplete");
			sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
			if (node_name.has_value())
			{
				if (node_name.value()._Equal(name))
				{
					sol::object value = node.second["Pins"][node.first + 1]["value"];
					if ((*(int*)value.pointer()) == last_mission)
						instance.add_to_queue(std::bind(NodeGraph::callNodeS, node.second, currentNodeGraphPtr, currentStorylinePtr));
				}
			}
		}
	}

	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(1.0f);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(1.0f);

	//call start node
	name_node = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeStart");
	NodeGraph::callAllNodeNameS(name_node, currentNodeGraphPtr, currentStorylinePtr);
	//call main loop
	name_node = NodeGraph::getNodeIcon("loop") + " " + langt("CoreNodeMainCycle");
	NodeGraph::callAllNodeNameS(name_node, currentNodeGraphPtr, currentStorylinePtr);
	
	while (storyline_started)
	{
		this_coro::wait(0ms);

		for (auto check : currentStorylinePtr->list_checkpoints)
		{
			if (check->useMission || check->activate)
			{
				if (check->startC == last_mission || check->activate)
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
										Command<Commands::REMOVE_BLIP>(blip_remove->markerPlay);
										blip_remove->markerPlay = NULL;
									}
								}
								auto& mission_name = namesMissions[pack_idx][check->gotoMission];
								start_storyline_mission(mission_name);
								last_mission = check->gotoMission;
								check->activate = false;
								for (auto node : currentNodeGraphPtr->nodes)
								{
									std::string name = NodeGraph::getNodeIcon("event") + " " + langt("CoreNodeMissionComplete");
									sol::optional<std::string> node_name = node.second["class"]["static"]["name"];
									if (node_name.has_value())
									{
										if (node_name.value()._Equal(name))
										{
											sol::object value = node.second["Pins"][node.first + 1]["value"];
											if ((*(int*)value.pointer()) == last_mission)
												instance.add_to_queue(std::bind(NodeGraph::callNodeS, node.second, currentNodeGraphPtr, currentStorylinePtr));
										}
									}
								}
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
		
		if (Command<0x0ADC>("ldhelp"))
		{
			CHud::SetHelpMessage("HELPING!", false, false, false);
			CHud::DrawHelpText();
			storyline_started = false;
			mission_started = false;
			this_coro::wait(0ms);
		}
	}

	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(0.0f);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(0.0f);
	
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
		Command<Commands::DELETE_CHAR>(actor);
	}
	missionActors.clear();
	for (auto vehicle : missionVehicle)
	{
		Command<Commands::DELETE_CAR>(vehicle);
	}
	missionVehicle.clear();
	for (auto object : missionObject)
	{
		Command<Commands::DELETE_OBJECT>(object);
		object->Remove();
	}
	missionObject.clear();
	for (auto particle : missionParticle)
	{
		Command<Commands::KILL_FX_SYSTEM>(particle.first);
		Command<Commands::DELETE_OBJECT>(particle.second);
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
	currentStorylinePtr->updateEditorEntity();
}
