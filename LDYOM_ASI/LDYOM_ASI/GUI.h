#pragma once

#include <CRadar.h>

#include "Manager.h"
#include "MissionPlayer.h"
#include "ScriptManager.h"
#include "NodeGraph.h"

//bool frames
bool bMainMenu = false;
bool bTargets = false;
bool bActors = false;
bool bCars = false;
bool bObjects = false;
bool bGroupRelations = false;
bool bParticles = false;
bool bPickups = false;
bool bExplosions = false;
bool bAudios = false;
bool bPlayer = false;
bool bNodeEditor = false;
bool bMissionPacks = false;
bool bMissionSettings = false;
bool bStorylines = false;
bool bTools = false;
bool bInfo = false;
bool bSettings = false;
bool bStorylineMainMenu = false;
bool bStorylineCheckpoints = false;
bool bScriptsSettings = false;

ImVec2 resolution;
int currentTarget;
int currentActor;
int currentCar;
int currentObject;
int currentParticle;
int currentPickup;
int currentExplosion;
int currentAudio;
int currentMissionPack;
int currentMission;
int currentStoryline;
int currentStorylineCheckpoint;
vector<const char*> namesTargets;
vector<const char*> namesCars;
vector<const char*> namesActors;
vector<const char*> namesObjects;
vector<const char*> namesParticles;
vector<const char*> namesPickups;
vector<const char*> namesExplosions;
vector<const char*> namesAudios;
vector<const char*> namesStorylineCheckpoints;
vector<std::string> namesMissionPacks;
vector<vector<std::string>> namesMissions;
vector<std::string> namesStorylines;
vector<std::string> namesAudioFiles;
char temp_nameMissionPack[65] = {};
char temp_nameStoryline[65] = {};
int moveTarget = -1;
int moveMission = -1;
int selTarget = 0;
int selTypeTarget = 0;
extern bool storylineMode;
int selMissPack;
bool enexMarker = false;
int tp_actor;
int tp_car;
int tp_object;

extern bool editmodeTimemiss;
extern void loadArrayMenu();
extern bool load_theme;
extern std::map<int, const char*> namesVars;

void fMainMenu()
{
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 100, resolution.y / 2 - 200), ImGuiCond_Appearing);
	ImGui::Begin(langt("mainMenu"), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

	const ImVec2 size_b(160, 0);
	std::string name;
	name.clear();
	name.append(ICON_FA_RUNNING);
	name.append(" ");
	name.append(langt("targets"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bTargets = true;
	}
	name.clear();
	name.append(ICON_FA_MALE);
	name.append(" ");
	name.append(langt("actors"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bActors = true;
	}
	name.clear();
	name.append(ICON_FA_HANDSHAKE);
	name.append(" ");
	name.append(langt("groupRelations"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bGroupRelations = true;
	}
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("cars"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bCars = true;
	}
	name.clear();
	name.append(ICON_FA_ARCHIVE);
	name.append(" ");
	name.append(langt("objects"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bObjects = true;
	}
	name.clear();
	name.append(ICON_FA_STARS);
	name.append(" ");
	name.append(langt("particles"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bParticles = true;
	}
	name.clear();
	name.append(ICON_FA_HEART);
	name.append(" ");
	name.append(langt("pickups"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bPickups = true;
	}
	name.clear();
	name.append(ICON_FA_BOMB);
	name.append(" ");
	name.append(langt("explosions"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bExplosions = true;
	}
	name.clear();
	name.append(ICON_FA_VOLUME_UP);
	name.append(" ");
	name.append(langt("audios"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bAudios = true;
	}
	name.clear();
	name.append(ICON_FA_PROJECT_DIAGRAM);
	name.append(" ");
	name.append(langt("nodeEditor"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bNodeEditor = true;
	}
	ImGui::Separator();
	name.clear();
	name.append(ICON_FA_THEATER_MASKS);
	name.append(" ");
	name.append(langt("missionPacks"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bMissionPacks = true;
	}
	name.clear();
	name.append(ICON_FA_SLIDERS_H);
	name.append(" ");
	name.append(langt("settingsMiss"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bMissionSettings = true;
	}
	name.clear();
	name.append(ICON_FA_BROOM);
	name.append(" ");
	name.append(langt("clearAll"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		name.clear();
		name.append(ICON_FA_EXCLAMATION_TRIANGLE);
		name.append(" ");
		name.append(langt("clearAll"));
		ImGui::OpenPopup(name.c_str());
	}
	name.clear();
	name.append(ICON_FA_SCROLL);
	name.append(" ");
	name.append(langt("storylines"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bStorylines = true;
	}
	ImGui::Separator();
	name.clear();
	name.append(ICON_FA_PLAY);
	name.append(" ");
	name.append(langt("missionStart"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		// open mission
		currentMissionPtr->removeEditorEntity();
		instance.add_to_queue(std::bind(MissionPlayer::start_mission, currentMissionPtr, currentNodeGraphPtr, true));
	}
	name.clear();
	name.append(ICON_FA_TOOLS);
	name.append(" ");
	name.append(langt("tools"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bTools = true;
	}
	name.clear();
	name.append(ICON_FA_FILE_CODE);
	name.append(" ");
	name.append(langt("scriptsSettings"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bScriptsSettings = true;
	}
	ImGui::Separator();
	name.clear();
	name.append(ICON_FA_INFO);
	name.append(" ");
	name.append(langt("info"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bInfo = true;
	}
	name.clear();
	name.append(ICON_FA_CONGS);
	name.append(" ");
	name.append(langt("settings"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bMainMenu = false;
		bSettings = true;
	}

	name.clear();
	name.append(ICON_FA_EXCLAMATION_TRIANGLE);
	name.append(" ");
	name.append(langt("clearAll"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text(langMenu["reloadQues"][2].c_str());

		if (ImGui::Button(langt("yes"), size_b)) {
			currentMissionPtr->removeEditorEntity();

			delete currentMissionPtr;
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

			currentMissionPtr = new Mission;
			currentMissionPtr->player.updateEditorPed();

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b)) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	
	ImGui::End();
}

void fTargets()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_RUNNING);
	name.append(" ");
	name.append(langt("targets"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentTarget, &namesTargets);

	if (ImGui::Button(langt("add")))
	{
		ImGui::OpenPopup("addT");
	}

	if (!currentMissionPtr->list_targets.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_target = currentMissionPtr->list_targets.size();
			Target* target;
			switch (currentMissionPtr->list_targets.at(currentTarget)->type)
			{
			case 0:
				target = new TargetCheckpoint(
					*static_cast<TargetCheckpoint*>(new_target, currentMissionPtr->list_targets.at(currentTarget)));
				break;
			case 1:
				target = new TargetCar(*static_cast<TargetCar*>(currentMissionPtr->list_targets.at(currentTarget)));
				break;
			case 2:
				target = new TargetActor(*static_cast<TargetActor*>(currentMissionPtr->list_targets.at(currentTarget)));
				break;
			case 3:
				switch (currentMissionPtr->list_targets.at(currentTarget)->targetType)
				{
				case 0:
					target = new TargetCutscene(
						*static_cast<TargetCutscene*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 1:
					target = new TargetCountdown(
						*static_cast<TargetCountdown*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 2:
					target = new TargetTimeout(
						*static_cast<TargetTimeout*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 3:
					target = new TargetWeather(
						*static_cast<TargetWeather*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 4:
					target = new TargetTime(*static_cast<TargetTime*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 5:
					target = new TargetTraffic(
						*static_cast<TargetTraffic*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				default:
					break;
				}
				break;
			case 4:
				target = new TargetObject(*static_cast<TargetObject*>(currentMissionPtr->list_targets.at(currentTarget)));
				break;
			case 5:
				target = new TargetPickup(*static_cast<TargetPickup*>(currentMissionPtr->list_targets.at(currentTarget)));
				break;
			case 6:
				switch (currentMissionPtr->list_targets.at(currentTarget)->targetType)
				{
				case 0:
					target = new TargetTeleport(
						*static_cast<TargetTeleport*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 1:
					target = new TargetAnimation(
						*static_cast<TargetAnimation*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 2:
					target = new TargetTeleportToVehicle(
						*static_cast<TargetTeleportToVehicle*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 3:
					target = new TargetLevelWanted(
						*static_cast<TargetLevelWanted*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 4:
					target = new TargetRemoveWeapons(
						*static_cast<TargetRemoveWeapons*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 5:
					target = new TargetDialog(
						*static_cast<TargetDialog*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				case 6:
					target = new
						TargetMoney(*static_cast<TargetMoney*>(currentMissionPtr->list_targets.at(currentTarget)));
					break;
				default:
					break;
				}
				break;
			case 7:
				target = new TargetWaitSignal(
					*static_cast<TargetWaitSignal*>(currentMissionPtr->list_targets.at(currentTarget)));
				break;
			default:
				break;
			}
			currentMissionPtr->list_targets.push_back(target);
			string str;
			str.append(currentMissionPtr->list_targets[new_target]->name);
			str.append("c");
			strcpy(target->name, str.c_str());

			namesTargets.push_back(target->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("cut")))
		{
			moveTarget = currentTarget;
		}
		if (moveTarget != -1)
		{
			ImGui::SameLine();
			if (ImGui::Button(langt("paste")))
			{
				moveCellVector(&currentMissionPtr->list_targets, &moveTarget, &currentTarget);
				moveCellVector(&namesTargets, &moveTarget, &currentTarget);
				moveTarget = -1;
			}
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	if (ImGui::BeginPopup("addT"))
	{
		vector<std::string>* type_targets_name = &langMenu["type_targets_name"];
		if (Combo(langt("typeTarget"), selTarget, type_targets_name))
		{
			selTypeTarget = 0;
			if (selTarget == 3)
				langMenu["target_type"] = parseJsonArray<std::string>(langt("target_type"));
			else if (selTarget == 6)
				langMenu["target_type"] = parseJsonArray<std::string>(langt("target_type_ev"));
		}
		vector<std::string>* target_type = &langMenu["target_type"];

		if (selTarget == 3 || selTarget == 6)
			Combo(langt("type"), selTypeTarget, target_type);
		if (ImGui::Button(langt("create")))
		{
			name.clear();
			name = langt("target");
			name.append(" #");
			name.append(std::to_string(currentMissionPtr->list_targets.size()));
			name.append(" : ");
			CVector playerPos = playerPed->GetPosition();
			switch (selTarget)
			{
			case 0:
				name.append(type_targets_name->at(selTarget));
				currentMissionPtr->list_targets.push_back(
					new TargetCheckpoint(name.c_str(), playerPos.x, playerPos.y, playerPos.z));
				break;
			case 1:
				name.append(type_targets_name->at(selTarget));
				currentMissionPtr->list_targets.push_back(new TargetCar(name.c_str()));
				break;
			case 2:
				name.append(type_targets_name->at(selTarget));
				currentMissionPtr->list_targets.push_back(new TargetActor(name.c_str()));
				break;
			case 3:
				name.append(target_type->at(selTypeTarget));
				switch (selTypeTarget)
				{
				case 0:
					currentMissionPtr->list_targets.push_back(
						new TargetCutscene(name.c_str(), playerPos.x, playerPos.y, playerPos.z));
					break;
				case 1:
					currentMissionPtr->list_targets.push_back(new TargetCountdown(name.c_str()));
					break;
				case 2:
					currentMissionPtr->list_targets.push_back(new TargetTimeout(name.c_str()));
					break;
				case 3:
					currentMissionPtr->list_targets.push_back(new TargetWeather(name.c_str()));
					break;
				case 4:
					currentMissionPtr->list_targets.push_back(new TargetTime(name.c_str()));
					break;
				case 5:
					currentMissionPtr->list_targets.push_back(new TargetTraffic(name.c_str()));
					break;
				default:
					break;
				}
				break;
			case 4:
				name.append(type_targets_name->at(selTarget));
				currentMissionPtr->list_targets.push_back(new TargetObject(name.c_str()));
				break;
			case 5:
				name.append(type_targets_name->at(selTarget));
				currentMissionPtr->list_targets.push_back(new TargetPickup(name.c_str()));
				break;
			case 6:
				name.append(target_type->at(selTypeTarget));
				switch (selTypeTarget)
				{
				case 0:
					{
						float angle = static_cast<float>(deg(playerPed->GetHeading()));
						int interiorID = CGame::currArea;
						currentMissionPtr->list_targets.push_back(
							new TargetTeleport(name.c_str(), playerPos.x, playerPos.y, playerPos.z, angle, interiorID));
						break;
					}
				case 1:
					currentMissionPtr->list_targets.push_back(new TargetAnimation(name.c_str()));
					break;
				case 2:
					currentMissionPtr->list_targets.push_back(new TargetTeleportToVehicle(name.c_str()));
					break;
				case 3:
					currentMissionPtr->list_targets.push_back(new TargetLevelWanted(name.c_str()));
					break;
				case 4:
					currentMissionPtr->list_targets.push_back(new TargetRemoveWeapons(name.c_str()));
					break;
				case 5:
					currentMissionPtr->list_targets.push_back(new TargetDialog(name.c_str()));
					break;
				case 6:
					currentMissionPtr->list_targets.push_back(new TargetMoney(name.c_str()));
					break;
				}
				break;
			case 7:
				name.append(type_targets_name->at(selTarget));
				currentMissionPtr->list_targets.push_back(new TargetWaitSignal(name.c_str()));
				break;
			default:
				break;
			}
			currentTarget = currentMissionPtr->list_targets.size() - 1;
			namesTargets.push_back(currentMissionPtr->list_targets[currentTarget]->name);
			selTarget = 0;
			selTypeTarget = 0;
			printLog(name);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			delete currentMissionPtr->list_targets[currentTarget];
			currentMissionPtr->list_targets.erase(currentMissionPtr->list_targets.begin() + currentTarget);
			namesTargets.erase(namesTargets.begin() + currentTarget);
			if (currentTarget > 0)
			{
				currentTarget -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_targets[currentTarget]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_targets[currentTarget]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//target render
	if (!currentMissionPtr->list_targets.empty())
	{
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_RUNNING);
		name.append(" ");
		name.append(langt("target"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);
		switch (currentMissionPtr->list_targets[currentTarget]->type)
		{
			//Checkpoint
		case 0:
			{
				TargetCheckpoint* targetPtr = static_cast<TargetCheckpoint*>(currentMissionPtr->list_targets[currentTarget]);
				if (ImGui::Button(ICON_FA_STREET_VIEW))
				{
					CVector playerPos = playerPed->GetPosition();
					targetPtr->pos[0] = playerPos.x;
					targetPtr->pos[1] = playerPos.y;
					targetPtr->pos[2] = playerPos.z;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(langt("playerCoordinates"));
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(270);
				ImGui::InputFloat3(langt("position"), targetPtr->pos, "%.6f");
				ImGui::PushItemWidth(-130);
				if (ImGui::DragFloat(langt("radiusCheckpoint"), &targetPtr->radius, 0.05f, 0.0f, 100.0f))
				{
					updateSphere = true;
				}
				Combo(langt("colorMarker"), targetPtr->colorBlip, &langMenu["targets_marker_color"]);
				ImGui::InputText(langt("textTarget"), &targetPtr->text[0], IM_ARRAYSIZE(targetPtr->text));
				ImGui::InputFloat(langt("timeText"), &targetPtr->textTime);


				ImGui::SetNextWindowBgAlpha(0.50);
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
				ImGui::Begin("info", nullptr,
				             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
				             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

				ImGui::Text(langMenu["infoOverlay"][0].c_str());
				ImGui::Text(langMenu["infoOverlay"][1].c_str());
				ImGui::Text(langMenu["infoOverlay"][2].c_str());

				ImGui::End();

				isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

				//edit
				float cx = targetPtr->pos[0], cy = targetPtr->pos[1], cz = targetPtr->pos[2];
				cx = cx + (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
					static_cast<float>(rad(camera_angle[1]))));
				cy = cy + (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
					static_cast<float>(rad(camera_angle[1]))));
				cz = cz + (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
				Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
				Command<Commands::POINT_CAMERA_AT_POINT>(targetPtr->pos[0], targetPtr->pos[1], targetPtr->pos[2], 2);

				if (!isWindow)
				{
					if (ImGui::IsMouseDragging(2))
					{
						ImVec2 dt = ImGui::GetIO().MouseDelta;
						camera_angle[0] += dt.x;
						camera_angle[1] += dt.y;
					}
					if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
					{
						camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4;
						if (camera_zoom < 1)
							camera_zoom = 1;
					}
				}
				if (KeyPressed(VK_UP))
				{
					targetPtr->pos[0] -= 0.01f * camera_zoom * static_cast<float>(sin(
						static_cast<float>(rad(camera_angle[0]))));
					targetPtr->pos[1] -= 0.01f * camera_zoom * static_cast<float>(cos(
						static_cast<float>(rad(camera_angle[0]))));
				}
				else if (KeyPressed(VK_DOWN))
				{
					targetPtr->pos[0] += 0.01f * camera_zoom * static_cast<float>(sin(
						static_cast<float>(rad(camera_angle[0]))));
					targetPtr->pos[1] += 0.01f * camera_zoom * static_cast<float>(cos(
						static_cast<float>(rad(camera_angle[0]))));
				}
				if (KeyPressed(VK_LEFT))
				{
					targetPtr->pos[0] += 0.01f * camera_zoom * static_cast<float>(sin(
						static_cast<float>(rad(camera_angle[0] + 90))));
					targetPtr->pos[1] += 0.01f * camera_zoom * static_cast<float>(cos(
						static_cast<float>(rad(camera_angle[0] + 90))));
				}
				else if (KeyPressed(VK_RIGHT))
				{
					targetPtr->pos[0] += 0.01f * camera_zoom * static_cast<float>(sin(
						static_cast<float>(rad(camera_angle[0] - 90))));
					targetPtr->pos[1] += 0.01f * camera_zoom * static_cast<float>(cos(
						static_cast<float>(rad(camera_angle[0] - 90))));
				}
				break;
			}
		case 1:
			{
				TargetCar* targetPtr = static_cast<TargetCar*>(currentMissionPtr->list_targets[currentTarget]);
				Combo(langt("car"), targetPtr->car, &namesCars);
				Combo(langt("colorMarker"), targetPtr->colorBlip, &langMenu["targets_marker_color"]);
				ImGui::InputText(langt("textTarget"), &targetPtr->text[0], IM_ARRAYSIZE(targetPtr->text));
				ImGui::InputFloat(langt("timeText"), &targetPtr->textTime);
				break;
			}
		case 2:
			{
				TargetActor* targetPtr = static_cast<TargetActor*>(currentMissionPtr->list_targets[currentTarget]);
				Combo(langt("actor"), targetPtr->actor, &namesActors);
				if (ToggleButton(langt("hitActor"), &targetPtr->hit))
				{
					targetPtr->killGroup = false;
				}
				if (ToggleButton(langt("killGroup"), &targetPtr->killGroup))
				{
					targetPtr->hit = false;
				}
				Combo(langt("colorMarker"), targetPtr->colorBlip, &langMenu["targets_marker_color"]);
				ImGui::InputText(langt("textTarget"), &targetPtr->text[0], IM_ARRAYSIZE(targetPtr->text));
				ImGui::InputFloat(langt("timeText"), &targetPtr->textTime);
				break;
			}
		case 3:
			{
				switch (currentMissionPtr->list_targets[currentTarget]->targetType)
				{
				case 0:
					{
						TargetCutscene* targetPtr = static_cast<TargetCutscene*>(currentMissionPtr->list_targets[
							currentTarget]);
						if (ImGui::Button(ICON_FA_STREET_VIEW))
						{
							CVector playerPos = playerPed->GetPosition();
							targetPtr->pos[0] = playerPos.x;
							targetPtr->pos[1] = playerPos.y;
							targetPtr->pos[2] = playerPos.z;
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::SetTooltip(langt("playerCoordinates"));
						}
						ImGui::SameLine();
						ImGui::PushItemWidth(270.0f);
						ImGui::InputFloat3(langt("position"), targetPtr->pos, "%.6f");
						ImGui::PushItemWidth(-130.0f);
						ImGui::DragInt3(langt("rotate"), targetPtr->rotate, 1, -360, 360, u8"%d°");

						if (ImGui::Button(langt("edithand")))
						{
							std::string HOBJ = UTF8_to_CP1251(langt("HOBJ"));
							GXTEncode(HOBJ);
							CHud::SetHelpMessage(HOBJ.c_str(), false, true, false);
							editmodeCamera = true;
							bTargets = false;
						}
						if (ImGui::SliderInt(langt("tied"), &targetPtr->tied, 0, 3,
						                     langMenu["CutscenePos"][targetPtr->tied].c_str()))
							targetPtr->tiedID = 0;

						if (targetPtr->tied == 1) {
							ImGui::PushID("tieda");
							Combo(langt("actor"), targetPtr->tiedID, &namesActors);
							ImGui::PopID();
						}
						else if (targetPtr->tied == 2) {
							ImGui::PushID("tiedc");
							Combo(langt("car"), targetPtr->tiedID, &namesCars);
							ImGui::PopID();
						}
						else if (targetPtr->tied == 3) {
							ImGui::PushID("tiedo");
							Combo(langt("object"), targetPtr->tiedID, &namesObjects);
							ImGui::PopID();
						}

						if (ImGui::SliderInt(langt("follow"), &targetPtr->follow, 0, 3,
						                     langMenu["CutsceneLook"][targetPtr->follow].c_str()))
							targetPtr->followID = 0;

						if (targetPtr->follow == 1) {
							ImGui::PushID("followa");
							Combo(langt("actor"), targetPtr->followID, &namesActors);
							ImGui::PopID();
						}
						else if (targetPtr->follow == 2) {
							ImGui::PushID("followc");
							Combo(langt("car"), targetPtr->followID, &namesCars);
							ImGui::PopID();
						}
						else if (targetPtr->follow == 3) {
							ImGui::PushID("followo");
							Combo(langt("object"), targetPtr->followID, &namesObjects);
							ImGui::PopID();
						}

						ToggleButton(langt("movecam"), &targetPtr->moveCam);
						ImGui::InputText(langt("text"), targetPtr->text, IM_ARRAYSIZE(targetPtr->text));
						ImGui::PushItemWidth(-180);
						ImGui::InputFloat(langt("timecutscene"), &targetPtr->time);
						break;
					}
				case 1:
					{
						TargetCountdown* targetPtr = static_cast<TargetCountdown*>(currentMissionPtr->list_targets[
							currentTarget]);
						ImGui::InputInt(langt("time"), &targetPtr->time);
						break;
					}
				case 2:
					{
						TargetTimeout* targetPtr = static_cast<TargetTimeout*>(currentMissionPtr->list_targets[
							currentTarget]);
						ImGui::InputText(langt("text"), targetPtr->text, IM_ARRAYSIZE(targetPtr->text));
						ImGui::InputFloat(langt("timeout"), &targetPtr->time);
						break;
					}
				case 3:
					{
						TargetWeather* targetPtr = static_cast<TargetWeather*>(currentMissionPtr->list_targets[
							currentTarget]);
						Combo(langt("weather"), targetPtr->weather, &langMenu["Weather_arr"]);
						if (ImGui::Button(langt("preview")))
							CWeather::ForceWeatherNow(targetPtr->weather);
						if (ImGui::Button(langt("unPreview")))
							CWeather::ForceWeatherNow(0);
						break;
					}
				case 4:
					{
						TargetTime* targetPtr = static_cast<TargetTime*>(currentMissionPtr->list_targets[currentTarget]);
						if (ImGui::Button(langt("timeGameMiss")))
						{
							std::string HMTIM = UTF8_to_CP1251(langt("HMTIM"));
							GXTEncode(HMTIM);
							CHud::SetHelpMessage(HMTIM.c_str(), false, true, false);
							editmodeTimetarg = true;
							bTargets = false;
						}
						break;
					}
				case 5:
					{
						TargetTraffic* targetPtr = static_cast<TargetTraffic*>(currentMissionPtr->list_targets[
							currentTarget]);
						unsigned char minS = 0;
						unsigned char maxS = 2;
						ImGui::SliderScalar(langt("countPed"), ImGuiDataType_U8, &targetPtr->peds, &minS, &maxS,
						                    langMenu["countTraffic"][targetPtr->peds].c_str());
						ImGui::SliderScalar(langt("countCar"), ImGuiDataType_U8, &targetPtr->cars, &minS, &maxS,
						                    langMenu["countTraffic"][targetPtr->cars].c_str());
						break;
					}
				default:
					break;
				}
				break;
			}
		case 4:
			{
				TargetObject* targetPtr = static_cast<TargetObject*>(currentMissionPtr->list_targets[currentTarget]);
				Combo(langt("object"), targetPtr->object, &namesObjects);
				ImGui::SliderInt(langt("type"), &targetPtr->typeUse, 0, 3,
				                 langMenu["targetTypeObj"][targetPtr->typeUse].c_str());
				if (targetPtr->typeUse == 3)
				{
					if (ImGui::ImageButton(static_cast<void*>(weaponsAtlas), ImVec2(52, 52),
					                       ImVec2(targetPtr->weapon * 0.02272727272f, 0),
					                       ImVec2((targetPtr->weapon + 1) * 0.02272727272f, 1)))
					{
						ImGui::OpenPopup("weapon");
					}

					ImGui::SameLine();
					ImGui::Text(langt("weapon"));

					//weapon popup
					if (ImGui::BeginPopup("weapon"))
					{
						ImGui::BeginChild("weapon", ImVec2(200, 450));

						for (int i = 0; i < IM_ARRAYSIZE(ID_Weapons); i++)
						{
							ImGui::PushID(i);
							if (ImGui::ImageButton(weaponsAtlas, ImVec2(52, 52), ImVec2(i * 0.02272727272f, 0),
							                       ImVec2((i + 1) * 0.02272727272f, 1)))
							{
								targetPtr->weapon = i;
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::SetTooltip(langMenu["weap_names"][i].c_str());
							}
							ImGui::PopID();
							if ((i + 1) % 3 != 0)
							{
								ImGui::SameLine();
							}
						}

						ImGui::EndChild();
						ImGui::EndPopup();
					}
				}
				Combo(langt("colorMarker"), targetPtr->colorBlip, &langMenu["targets_marker_color"]);
				ImGui::InputText(langt("textTarget"), &targetPtr->text[0], IM_ARRAYSIZE(targetPtr->text));
				ImGui::InputFloat(langt("timeText"), &targetPtr->textTime);
				break;
			}
		case 5:
			{
				TargetPickup* targetPtr = static_cast<TargetPickup*>(currentMissionPtr->list_targets[currentTarget]);
				Combo(langt("pickup"), targetPtr->pickup, &namesPickups);
				Combo(langt("colorMarker"), targetPtr->colorBlip, &langMenu["targets_marker_color"]);
				ImGui::InputText(langt("textTarget"), &targetPtr->text[0], IM_ARRAYSIZE(targetPtr->text));
				ImGui::InputFloat(langt("timeText"), &targetPtr->textTime);
				break;
			}
		case 6:
			{
				switch (currentMissionPtr->list_targets[currentTarget]->targetType)
				{
				case 0:
					{
						TargetTeleport* targetPtr = static_cast<TargetTeleport*>(currentMissionPtr->list_targets[
							currentTarget]);
						if (ImGui::Button(ICON_FA_STREET_VIEW))
						{
							CVector playerPos = playerPed->GetPosition();
							targetPtr->pos[0] = playerPos.x;
							targetPtr->pos[1] = playerPos.y;
							targetPtr->pos[2] = playerPos.z;
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::SetTooltip(langt("playerCoordinates"));
						}
						ImGui::SameLine();
						ImGui::PushItemWidth(270);
						ImGui::InputFloat3(langt("position"), targetPtr->pos, "%.6f");
						ImGui::PushID("angle");
						if (ImGui::Button(ICON_FA_STREET_VIEW))
						{
							float angle = static_cast<float>(deg(playerPed->GetHeading()));
							targetPtr->angle = angle;
						}
						ImGui::PopID();
						if (ImGui::IsItemHovered())
						{
							ImGui::SetTooltip(langt("playerAngle"));
						}

						ImGui::SameLine();
						ImGui::DragFloat(langt("angle"), &targetPtr->angle, 1, -360, 360);

						ImGui::Separator();

						unsigned char minS = 0;
						unsigned char maxS = 1;
						if (ImGui::SliderScalar("", ImGuiDataType_U8, &targetPtr->modelType, &minS, &maxS,
						                        (targetPtr->modelType == 0) ? langt("ped") : langt("specActor")))
						{
							targetPtr->modelID = 0;
						}


						//skin popup open
						if (targetPtr->modelType == 0)
						{
							if (ImGui::Button(ICON_FA_TSHIRT))
								ImGui::OpenPopup("skins");


							ImGui::SameLine();

							if (ImGui::InputInt(langt("model"), &targetPtr->modelID))
							{
								int id_a = 0;
								for (int v = 0; v < ID_Actors.size(); v++)
								{
									if (targetPtr->modelID <= ID_Actors[v])
									{
										id_a = ID_Actors[v];
										break;
									}
								}
								targetPtr->modelID = id_a;
							}
						}
						else
							Combo(langt("model"), targetPtr->modelID, &ID_Spec_Actors);

						ImGui::Separator();

						if (ImGui::TreeNode(langt("chartics")))
						{
							ImGui::InputInt(langt("countlive"), &targetPtr->health);

							if (ImGui::ImageButton(static_cast<void*>(weaponsAtlas), ImVec2(52, 52),
							                       ImVec2(targetPtr->weapon * 0.02272727272f, 0),
							                       ImVec2((targetPtr->weapon + 1) * 0.02272727272f, 1)))
							{
								ImGui::OpenPopup("weapon");
							}

							ImGui::SameLine();
							ImGui::Text(langt("weapon"));

							if (ImGui::InputInt(langt("ammo"), &targetPtr->ammo))
							{
								if (targetPtr->ammo < 0)
									targetPtr->ammo = 0;
							}


							//weapon popup
							if (ImGui::BeginPopup("weapon"))
							{
								ImGui::BeginChild("weapon", ImVec2(200, 450));

								for (int i = 0; i < IM_ARRAYSIZE(ID_Weapons); i++)
								{
									ImGui::PushID(i);
									if (ImGui::ImageButton(weaponsAtlas, ImVec2(52, 52), ImVec2(i * 0.02272727272f, 0),
									                       ImVec2((i + 1) * 0.02272727272f, 1)))
									{
										targetPtr->weapon = i;
									}
									if (ImGui::IsItemHovered())
									{
										ImGui::SetTooltip(langMenu["weap_names"][i].c_str());
									}
									ImGui::PopID();
									if ((i + 1) % 3 != 0)
									{
										ImGui::SameLine();
									}
								}

								ImGui::EndChild();
								ImGui::EndPopup();
							}

							ImGui::TreePop();
						}

						ImGui::Separator();

						if (ImGui::Button(langt("edithand")))
						{
							editmodeTeleportPlayer = true;
							bTargets = false;
							Command<Commands::FREEZE_CHAR_POSITION>(playerPed, false);
						}

						//skin popup
						if (ImGui::BeginPopup("skins"))
						{
							ImGui::BeginChild("skins", ImVec2(340, 450));


							for (int i = 0; i < ID_Actors.size(); i++)
							{
								ImGui::PushID(i);
								if (ImGui::ImageButton(pedsSkinAtlas, ImVec2(55, 100), ImVec2((i * 55) / 14630.0f, 0),
								                       ImVec2(((i + 1) * 55) / 14630.0f, 1)))
								{
									targetPtr->modelID = ID_Actors[i];
								}
								ImGui::PopID();
								if ((i + 1) % 5 != 0)
								{
									ImGui::SameLine();
								}
							}

							ImGui::EndChild();
							ImGui::EndPopup();
						}
						break;
					}
				case 1:
					{
						TargetAnimation* targetPtr = static_cast<TargetAnimation*>(currentMissionPtr->list_targets[
							currentTarget]);
						if (Combo(langt("pack"), targetPtr->pack, &Anim_name))
							targetPtr->anim = 0;

						Combo(langt("anim"), targetPtr->anim, &Anim_list[targetPtr->pack]);
						ToggleButton(langt("looped"), &targetPtr->looped);
						if (ImGui::Button(langt("preview")))
						{
							if (!Command<Commands::HAS_ANIMATION_LOADED>(Anim_name[targetPtr->pack].c_str()))
								Command<Commands::REQUEST_ANIMATION>(Anim_name[targetPtr->pack].c_str());
							vector<std::string> anims = Anim_list[targetPtr->pack];
							Command<Commands::TASK_PLAY_ANIM>(playerPed, anims[targetPtr->anim].c_str(),
							                                  Anim_name[targetPtr->pack].c_str(), 1.0f,
							                                  targetPtr->looped, false, false, false, -1);
						}
						break;
					}
				case 2:
					{
						TargetTeleportToVehicle* targetPtr = static_cast<TargetTeleportToVehicle*>(currentMissionPtr->
							list_targets[currentTarget]);
						Combo(langt("car"), targetPtr->car, &namesCars);
						ImGui::SliderInt(langt("place"), &targetPtr->carPlace, 0, 3,
						                 langMenu["place_car"][targetPtr->carPlace].c_str());
						break;
					}
				case 3:
					{
						TargetLevelWanted* targetPtr = static_cast<TargetLevelWanted*>(currentMissionPtr->list_targets[
							currentTarget]);
						ImGui::SliderInt(langt("levelp"), &targetPtr->levelWanted, 0, 6, "%d stars");
						break;
					}
				case 5:
					{
						TargetDialog* targetPtr = static_cast<TargetDialog*>(currentMissionPtr->list_targets[currentTarget]
						);
						if (ImGui::Button(langt("add")))
							targetPtr->dialogs.push_back(Dialog());
						ImGui::SameLine();
						vector<std::string> rang;
						for (int i = 0; i < targetPtr->dialogs.size(); i++)
						{
							rang.push_back(std::to_string(i));
						}
						ImGui::SetNextItemWidth(120);
						Combo(langt("number"), targetPtr->dialog, &rang);
						ImGui::SameLine();
						if (!targetPtr->dialogs.empty())
						{
							if (ImGui::Button(langt("delete")))
							{
								targetPtr->dialogs.erase(targetPtr->dialogs.begin() + targetPtr->dialog);
								if (targetPtr->dialog > 0)
									targetPtr->dialog--;
							}
						}
						for (int d = 0; d < targetPtr->dialogs.size(); d++)
						{
							if (ImGui::TreeNode(std::to_string(d).c_str()))
							{
								ImGui::InputText(langt("text"), &targetPtr->dialogs[d].text[0],
								                 IM_ARRAYSIZE(targetPtr->dialogs[d].text));
								ImGui::InputFloat(langt("time"), &targetPtr->dialogs[d].textTime);
								ImGui::TreePop();
							}
						}
						break;
					}
				case 6:
					{
						TargetMoney* targetPtr = static_cast<TargetMoney*>(currentMissionPtr->list_targets[currentTarget]);
						ImGui::DragInt(langt("add"), &targetPtr->money, 100, NULL, NULL, u8"%d$");
						break;
					}
				default:
					break;
				}
				break;
			}
		default:
			break;
		}

		ImGui::End();
	}
}

void fActors()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_MALE);
	name.append(" ");
	name.append(langt("actors"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentActor, &namesActors);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("actor"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_actors.size()));
		currentMissionPtr->list_actors.push_back(new Actor(name.c_str(), playerPos.x, playerPos.y, playerPos.z, angle,
		                                                currentMissionPtr->list_targets.empty()
			                                                ? 0
			                                                : currentMissionPtr->list_targets.size() - 1));
		currentActor = currentMissionPtr->list_actors.size() - 1;
		namesActors.push_back(currentMissionPtr->list_actors[currentActor]->name);
		currentMissionPtr->list_actors[currentMissionPtr->list_actors.size() - 1]->updateEditorPed();
	}
	if (!currentMissionPtr->list_actors.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_actor = currentMissionPtr->list_actors.size();
			Actor* actor = new Actor(*currentMissionPtr->list_actors[currentActor]);
			actor->updateEditorPed();
			currentMissionPtr->list_actors.push_back(actor);
			string str;
			str.append(currentMissionPtr->list_actors[new_actor]->name);
			str.append("c");
			strcpy(actor->name, str.c_str());

			namesActors.push_back(actor->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_actors[currentActor]->removeEditorPed();
			delete currentMissionPtr->list_actors[currentActor];
			currentMissionPtr->list_actors.erase(currentMissionPtr->list_actors.begin() + currentActor);
			namesActors.erase(namesActors.begin() + currentActor);
			if (currentActor > 0)
			{
				currentActor -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_actors[currentActor]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_actors[currentActor]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//actor render
	if (!currentMissionPtr->list_actors.empty())
	{
		Actor* actorPtr = currentMissionPtr->list_actors[currentActor];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("actor"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			actorPtr->pos[0] = playerPos.x;
			actorPtr->pos[1] = playerPos.y;
			actorPtr->pos[2] = playerPos.z;
			actorPtr->editorPed->SetPosn(playerPos);
			Command<Commands::GET_AREA_VISIBLE>(&actorPtr->interiorID);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		ImGui::InputFloat3(langt("position"), actorPtr->pos, "%.6f");
		ImGui::PushID("angle");
		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			float angle = static_cast<float>(deg(playerPed->GetHeading()));
			actorPtr->angle = angle;
			actorPtr->editorPed->SetHeading(static_cast<float>(rad(actorPtr->angle)));
		}
		ImGui::PopID();
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerAngle"));
		}

		ImGui::SameLine();
		if (ImGui::DragFloat(langt("angle"), &actorPtr->angle, 1, -360, 360))
		{
			actorPtr->editorPed->SetHeading(static_cast<float>(rad(actorPtr->angle)));
		}

		ImGui::Separator();

		unsigned char minS = 0;
		unsigned char maxS = 1;
		if (ImGui::SliderScalar("", ImGuiDataType_U8, &actorPtr->modelType, &minS, &maxS,
		                        (actorPtr->modelType == 0) ? langt("ped") : langt("specActor")))
		{
			actorPtr->modelID = 0;
			actorPtr->updateEditorPed();
		}

		//skin popup open
		if (actorPtr->modelType == 0)
		{
			if (ImGui::Button(ICON_FA_TSHIRT))
				ImGui::OpenPopup("skins");


			ImGui::SameLine();

			if (ImGui::InputInt(langt("model"), &actorPtr->modelID))
			{
				int id_a = 0;
				for (int v = 0; v < ID_Actors.size(); v++)
				{
					if (actorPtr->modelID <= ID_Actors[v])
					{
						id_a = ID_Actors[v];
						break;
					}
				}
				actorPtr->modelID = id_a;
				actorPtr->updateEditorPed();
			}
		}
		else if (ImGui::SliderInt(langt("slot"), &actorPtr->slotSkin, 1, 10) || Combo(
			langt("model"), actorPtr->modelID, &ID_Spec_Actors))
		{
			actorPtr->updateEditorPed();
		}

		ImGui::Separator();

		if (ImGui::TreeNode(langt("chartics")))
		{
			ImGui::InputInt(langt("countlive"), &actorPtr->health);

			if (ImGui::ImageButton(static_cast<void*>(weaponsAtlas), ImVec2(52, 52),
			                       ImVec2(actorPtr->weapon * 0.02272727272f, 0),
			                       ImVec2((actorPtr->weapon + 1) * 0.02272727272f, 1)))
			{
				ImGui::OpenPopup("weapon");
			}

			ImGui::SameLine();
			ImGui::Text(langt("weapon"));

			if (ImGui::InputInt(langt("ammo"), &actorPtr->ammo))
			{
				if (actorPtr->ammo < 0)
					actorPtr->ammo = 0;
			}

			ImGui::DragInt(langt("accuracy"), &actorPtr->accuracy, 0.5f, 0, 100, "%d %%");

			//weapon popup
			if (ImGui::BeginPopup("weapon"))
			{
				ImGui::BeginChild("weapon", ImVec2(200, 450));

				for (int i = 0; i < IM_ARRAYSIZE(ID_Weapons); i++)
				{
					ImGui::PushID(i);
					if (ImGui::ImageButton(weaponsAtlas, ImVec2(52, 52), ImVec2(i * 0.02272727272f, 0),
					                       ImVec2((i + 1) * 0.02272727272f, 1)))
					{
						actorPtr->weapon = i;
						actorPtr->updateEditorPed();
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip(langMenu["weap_names"][i].c_str());
					}
					ImGui::PopID();
					if ((i + 1) % 3 != 0)
					{
						ImGui::SameLine();
					}
				}

				ImGui::EndChild();
				ImGui::EndPopup();
			}

			ImGui::SliderInt(langt("group"), &actorPtr->group, 0, 8, langMenu["name_groups"][actorPtr->group].c_str());

			ImGui::TreePop();
		}

		ToggleButton(langt("randomSpawnW"), &actorPtr->randomSpawn);
		ToggleButton(langt("should_live"), &actorPtr->shouldNotDie);


		ImGui::Separator();

		if (ImGui::Button(langt("edithand")))
		{
			editActor = true;
			bActors = false;
			actorPtr->removeEditorPed();

			int modelID = actorPtr->modelID;
			int weapon = actorPtr->weapon;
			if (actorPtr->modelType == 0)
			{
				CStreaming::RequestModel(modelID, 0);
				CStreaming::LoadAllRequestedModels(false);
			}
			else
			{
				std::string modell_n = ID_Spec_Actors[modelID];
				CStreaming::RequestSpecialChar(actorPtr->slotSkin - 1, modell_n.c_str(), 0);
				CStreaming::LoadAllRequestedModels(false);
				modelID = 290 + actorPtr->slotSkin - 1;
			}
			Command<Commands::SET_PLAYER_MODEL>(0, modelID);
			CStreaming::SetSpecialCharIsDeletable(actorPtr->slotSkin - 1);
			Command<Commands::FREEZE_CHAR_POSITION>(playerPed, false);
			playerPed->SetPosn(CVector(actorPtr->pos[0], actorPtr->pos[1], actorPtr->pos[2]));
			Command<Commands::SET_CHAR_HEADING>(playerPed, actorPtr->angle);
			Command<Commands::SET_AREA_VISIBLE>(actorPtr->interiorID);
		}

		ImGui::PushItemWidth(150);
		ImGui::Separator();

		ToggleButton(langt("useTarget"), &actorPtr->useTarget);

		if (actorPtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), actorPtr->startC, &namesTargets);
			Combo(langt("dis_after"), actorPtr->endC, &list_tg_m);
		}

		//edit
		ImGui::SetNextWindowBgAlpha(0.50f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
		             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
		             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();


		float cx = actorPtr->pos[0], cy = actorPtr->pos[1], cz = actorPtr->pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(actorPtr->pos[0], actorPtr->pos[1], actorPtr->pos[2] + 1, 2);
		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
				if (camera_zoom < 1.0f)
					camera_zoom = 1.0f;
			}
		}

		//skin popup
		if (ImGui::BeginPopup("skins"))
		{
			ImGui::BeginChild("skins", ImVec2(340, 450));


			for (int i = 0; i < ID_Actors.size(); i++)
			{
				ImGui::PushID(i);
				if (ImGui::ImageButton(pedsSkinAtlas, ImVec2(55, 100), ImVec2((i * 55) / 14630.0f, 0),
				                       ImVec2(((i + 1) * 55) / 14630.0f, 1)))
				{
					actorPtr->modelID = ID_Actors[i];
					actorPtr->updateEditorPed();
				}
				ImGui::PopID();
				if ((i + 1) % 5 != 0)
				{
					ImGui::SameLine();
				}
			}

			ImGui::EndChild();
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

void fGroupRelations()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 125, resolution.y / 2 - 150), ImGuiCond_Always);
	name.append(ICON_FA_HANDSHAKE);
	name.append(" ");
	name.append(langt("groupRelations"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PushItemWidth(125);
	for (int i = 0; i < 8; i++)
	{
		std::stringstream group;
		group << langt("group") << i + 1;
		if (ImGui::TreeNode(group.str().c_str()))
		{
			for (int j = 0; j < 8; j++)
			{
				if (currentMissionPtr->groupRelations[i][j] != -1)
				{
					ImS8 min = 0;
					ImS8 max = 4;
					ImGui::SliderScalar(langMenu["name_groups"][j].c_str(), ImGuiDataType_S8,
					                    &currentMissionPtr->groupRelations[i][j], &min, &max,
					                    langMenu["Relationship_types"][currentMissionPtr->groupRelations[i][j]].c_str());
				}
			}
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void fCars()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("cars"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentCar, &namesCars);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("car"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_cars.size()));
		currentMissionPtr->list_cars.push_back(new Car(name.c_str(), playerPos.x, playerPos.y, playerPos.z, angle,
		                                            currentMissionPtr->list_targets.empty()
			                                            ? 0
			                                            : currentMissionPtr->list_targets.size() - 1));
		currentCar = currentMissionPtr->list_cars.size() - 1;
		namesCars.push_back(currentMissionPtr->list_cars[currentCar]->name);
		currentMissionPtr->list_cars[currentMissionPtr->list_cars.size() - 1]->updateEditorCar(true);
	}
	if (currentMissionPtr->list_cars.size() > 0)
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_car = currentMissionPtr->list_cars.size();
			Car* car = new Car(*currentMissionPtr->list_cars[currentCar]);
			car->updateEditorCar(false);
			currentMissionPtr->list_cars.push_back(car);
			string str;
			str.append(currentMissionPtr->list_cars[new_car]->name);
			str.append("c");
			strcpy(car->name, str.c_str());

			namesCars.push_back(car->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_cars[currentCar]->removeEditorCar();
			delete currentMissionPtr->list_cars[currentCar];
			currentMissionPtr->list_cars.erase(currentMissionPtr->list_cars.begin() + currentCar);
			namesCars.erase(namesCars.begin() + currentCar);
			if (currentCar > 0)
			{
				currentCar -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_cars[currentCar]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_cars[currentCar]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//car render
	if (!currentMissionPtr->list_cars.empty())
	{
		Car* carPtr = currentMissionPtr->list_cars[currentCar];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("car"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			carPtr->pos[0] = playerPos.x;
			carPtr->pos[1] = playerPos.y;
			carPtr->pos[2] = playerPos.z;
			carPtr->editorCar->SetPosn(playerPos);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		ImGui::InputFloat3(langt("position"), carPtr->pos, "%.6f");
		ImGui::PushID("angle");
		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			float angle = static_cast<float>(deg(playerPed->GetHeading()));
			carPtr->angle = angle;
			carPtr->editorCar->SetHeading(static_cast<float>(rad(carPtr->angle)));
		}
		ImGui::PopID();
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerAngle"));
		}

		ImGui::SameLine();
		if (ImGui::DragFloat(langt("angle"), &carPtr->angle, 1, -360, 360))
		{
			carPtr->editorCar->SetHeading(static_cast<float>(rad(carPtr->angle)));
		}

		if (ImGui::Button(langt("selectCar")))
		{
			carSelector::bShow = true;
			bCars = false;
		}

		ImGui::Separator();

		if (ImGui::TreeNode(langt("chartics")))
		{
			ToggleButton(langt("extendedColor"), &carPtr->extendedColor);
			if (carPtr->extendedColor)
			{
				if (ImGui::TreeNode(langt("colors")))
				{
					int i = 0;
					for (auto comp : components::get_all_vehicle_components(carPtr->editorCar))
					{
						for (auto obj : comp.getObjects())
						{
							for (auto mat : obj.getMaterials())
							{
								ImGui::PushID(i);
								ImGui::PushItemWidth(200);
								if (ImGui::ColorEdit4(std::to_string(i).c_str(), carPtr->colors[i].second.data(),
								                      ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs +
								                      ImGuiColorEditFlags_NoLabel))
								{
									float new_r = carPtr->colors[i].second[0], new_g = carPtr->colors[i].second[1],
									      new_b = carPtr->colors[i].second[2], a = carPtr->colors[i].second[3];
									mat.setColor(CRGBA(new_r * 255, new_g * 255, new_b * 255, a * 255));
								}
								ImGui::SameLine();
								std::stringstream name_c;
								name_c << comp.getName() << ":" << (!mat._material->texture
									                                    ? "[no texture]"
									                                    : mat._material->texture->name) << ":" << mat.
									getRawPointer();
								ImGui::Text(name_c.str().c_str());
								ImGui::PopItemWidth();
								ImGui::PopID();
								i++;
							}
						}
					}
					ImGui::TreePop();
				}
			}
			else
			{
				if (ImGui::ColorEdit4("primaryColorE", carPtr->primaryColor,
				                      ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs +
				                      ImGuiColorEditFlags_NoLabel))
				{
					int i = 0;
					for (auto comp : components::get_all_vehicle_components(carPtr->editorCar))
					{
						for (auto obj : comp.getObjects())
						{
							for (auto mat : obj.getMaterials())
							{
								if (carPtr->colors[i].first == 0)
								{
									float r = carPtr->primaryColor[0], g = carPtr->primaryColor[1], b = carPtr->
										      primaryColor[2], a = carPtr->primaryColor[3];
									carPtr->colors[i].second[0] = r;
									carPtr->colors[i].second[1] = g;
									carPtr->colors[i].second[2] = b;
									carPtr->colors[i].second[3] = a;
									mat.setColor(CRGBA(r * 255, g * 255, b * 255, a * 255));
								}
								i++;
							}
						}
					}
				}
				ImGui::SameLine();
				ImGui::Text(langt("primaryColor"));

				if (ImGui::ColorEdit4("secondaryColorE", carPtr->secondaryColor,
				                      ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs +
				                      ImGuiColorEditFlags_NoLabel))
				{
					int i = 0;
					for (auto comp : components::get_all_vehicle_components(carPtr->editorCar))
					{
						for (auto obj : comp.getObjects())
						{
							for (auto mat : obj.getMaterials())
							{
								if (carPtr->colors[i].first == 1)
								{
									float r = carPtr->secondaryColor[0], g = carPtr->secondaryColor[1], b = carPtr->
										      secondaryColor[2], a = carPtr->secondaryColor[3];
									carPtr->colors[i].second[0] = r;
									carPtr->colors[i].second[1] = g;
									carPtr->colors[i].second[2] = b;
									carPtr->colors[i].second[3] = a;
									mat.setColor(CRGBA(r * 255, g * 255, b * 255, a * 255));
								}
								i++;
							}
						}
					}
				}
				ImGui::SameLine();
				ImGui::Text(langt("secondaryColor"));
			}

			ImGui::Separator();

			ImGui::InputInt(langt("countlive"), &carPtr->health);

			ImGui::Separator();

			std::string varia;
			varia.append(langt("variation"));
			varia.append(" 1");
			if (ImGui::SliderInt(varia.c_str(), &carPtr->component_type_A, -1, 5))
			{
				carPtr->updateEditorCar(true);
			}
			varia.clear();
			varia.append(langt("variation"));
			varia.append(" 2");
			if (ImGui::SliderInt(varia.c_str(), &carPtr->component_type_B, -1, 5))
			{
				carPtr->updateEditorCar(true);
			}

			ImGui::Separator();

			ToggleButton(langMenu["car_unbreak"][0].c_str(), &carPtr->bulletproof);
			ToggleButton(langMenu["car_unbreak"][1].c_str(), &carPtr->fireproof);
			ToggleButton(langMenu["car_unbreak"][2].c_str(), &carPtr->explosionproof);
			ToggleButton(langMenu["car_unbreak"][3].c_str(), &carPtr->collisionproof);
			ToggleButton(langMenu["car_unbreak"][4].c_str(), &carPtr->meleeproof);
			ToggleButton(langMenu["car_unbreak"][5].c_str(), &carPtr->tiresVulnerability);

			ImGui::TreePop();
		}
		ToggleButton(langt("CoreNodeLockDoorsCar"), &carPtr->locked);
		ToggleButton(langt("should_live"), &carPtr->shouldNotDie);


		ImGui::Separator();

		if (ImGui::Button(langt("edithand")))
		{
			editmodeCar = true;
			bCars = false;
		}

		ImGui::PushItemWidth(150);
		ImGui::Separator();

		ToggleButton(langt("useTarget"), &carPtr->useTarget);
		if (carPtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), carPtr->startC, &namesTargets);
			Combo(langt("dis_after"), carPtr->endC, &list_tg_m);
		}

		ImGui::End();

		//edit
		ImGui::SetNextWindowBgAlpha(0.50f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
		             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
		             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

		float cx = carPtr->pos[0], cy = carPtr->pos[1], cz = carPtr->pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(carPtr->pos[0], carPtr->pos[1], carPtr->pos[2] + 1, 2);
		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0f;
				if (camera_zoom < 1.0f)
					camera_zoom = 1.0f;
			}
		}
	}
}

void fObjects()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("objects"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentObject, &namesObjects);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("object"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_objects.size()));
		currentMissionPtr->list_objects.push_back(new Object(name.c_str(), playerPos.x, playerPos.y, playerPos.z,
		                                                  currentMissionPtr->list_targets.empty()
			                                                  ? 0
			                                                  : currentMissionPtr->list_targets.size() - 1));
		currentObject = currentMissionPtr->list_objects.size() - 1;
		namesObjects.push_back(currentMissionPtr->list_objects[currentObject]->name);
		currentMissionPtr->list_objects[currentMissionPtr->list_objects.size() - 1]->updateEditorObject();
	}
	if (currentMissionPtr->list_objects.size() > 0)
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_object = currentMissionPtr->list_objects.size();
			Object* object = new Object(*currentMissionPtr->list_objects[currentObject]);
			object->updateEditorObject();
			currentMissionPtr->list_objects.push_back(object);
			string str;
			str.append(currentMissionPtr->list_objects[new_object]->name);
			str.append("c");
			strcpy(object->name, str.c_str());

			namesObjects.push_back(object->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_objects[currentObject]->removeEditorObject();
			delete currentMissionPtr->list_objects[currentObject];
			currentMissionPtr->list_objects.erase(currentMissionPtr->list_objects.begin() + currentObject);
			namesObjects.erase(namesObjects.begin() + currentObject);
			if (currentObject > 0)
			{
				currentObject -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_objects[currentObject]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_objects[currentObject]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//object render
	if (!currentMissionPtr->list_objects.empty())
	{
		Object* objectPtr = currentMissionPtr->list_objects[currentObject];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("object"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			objectPtr->pos[0] = playerPos.x;
			objectPtr->pos[1] = playerPos.y;
			objectPtr->pos[2] = playerPos.z;
			Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
			                                          objectPtr->pos[2]);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		if (ImGui::InputFloat3(langt("position"), objectPtr->pos, "%.6f"))
		{
			Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
			                                          objectPtr->pos[2]);
		}

		if (ImGui::DragInt3(langt("rotate"), objectPtr->rotation, 0.3, -360.0, 360.0, u8"%d°"))
		{
			Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject, static_cast<float>(objectPtr->rotation[0]),
			                                       static_cast<float>(objectPtr->rotation[1]),
			                                       static_cast<float>(objectPtr->rotation[2]));
		}
		ImGui::InputInt(langt("model"), &objectPtr->modelID);
		if (ImGui::Button(langt("apply")))
		{
			objectPtr->updateEditorObject();
		}

		ToggleButton(langt("useTarget"), &objectPtr->useTarget);
		if (objectPtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), objectPtr->startC, &namesTargets);
			Combo(langt("dis_after"), objectPtr->endC, &list_tg_m);
		}

		ImGui::End();

		//edit
		ImGui::SetNextWindowBgAlpha(0.50f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
		             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
		             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());
		ImGui::Text(langMenu["infoOverlay"][2].c_str());
		ImGui::Text(langMenu["infoOverlay"][3].c_str());
		ImGui::Text(langMenu["infoOverlay"][4].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

		float cx = objectPtr->pos[0], cy = objectPtr->pos[1], cz = objectPtr->pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(objectPtr->pos[0], objectPtr->pos[1], objectPtr->pos[2], 2);
		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
				if (camera_zoom < 1.0f)
					camera_zoom = 1.0f;
			}
		}
		if (KeyPressed(VK_SHIFT))
		{
			if (KeyPressed(VK_UP))
			{
				objectPtr->rotation[0]++;
				Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject,
				                                       static_cast<float>(objectPtr->rotation[0]),
				                                       static_cast<float>(objectPtr->rotation[1]),
				                                       static_cast<float>(objectPtr->rotation[2]));
			}
			else if (KeyPressed(VK_DOWN))
			{
				objectPtr->rotation[0]--;
				Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject,
				                                       static_cast<float>(objectPtr->rotation[0]),
				                                       static_cast<float>(objectPtr->rotation[1]),
				                                       static_cast<float>(objectPtr->rotation[2]));
			}
			if (KeyPressed(VK_LEFT))
			{
				objectPtr->rotation[1]++;
				Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject,
				                                       static_cast<float>(objectPtr->rotation[0]),
				                                       static_cast<float>(objectPtr->rotation[1]),
				                                       static_cast<float>(objectPtr->rotation[2]));
			}
			else if (KeyPressed(VK_RIGHT))
			{
				objectPtr->rotation[1]--;
				Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject,
				                                       static_cast<float>(objectPtr->rotation[0]),
				                                       static_cast<float>(objectPtr->rotation[1]),
				                                       static_cast<float>(objectPtr->rotation[2]));
			}
			if (KeyPressed(VK_Q))
			{
				objectPtr->rotation[2]++;
				Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject,
				                                       static_cast<float>(objectPtr->rotation[0]),
				                                       static_cast<float>(objectPtr->rotation[1]),
				                                       static_cast<float>(objectPtr->rotation[2]));
			}
			else if (KeyPressed(VK_E))
			{
				objectPtr->rotation[2]--;
				Command<Commands::SET_OBJECT_ROTATION>(objectPtr->editorObject,
				                                       static_cast<float>(objectPtr->rotation[0]),
				                                       static_cast<float>(objectPtr->rotation[1]),
				                                       static_cast<float>(objectPtr->rotation[2]));
			}
		}
		else
		{
			if (KeyPressed(VK_UP))
			{
				objectPtr->pos[0] -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
				objectPtr->pos[1] -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
				Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
				                                          objectPtr->pos[2]);
			}
			else if (KeyPressed(VK_DOWN))
			{
				objectPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
				objectPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
				Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
				                                          objectPtr->pos[2]);
			}
			if (KeyPressed(VK_LEFT))
			{
				objectPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
				objectPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
				Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
				                                          objectPtr->pos[2]);
			}
			else if (KeyPressed(VK_RIGHT))
			{
				objectPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
				objectPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
				Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
				                                          objectPtr->pos[2]);
			}
			if (KeyPressed(VK_Q))
			{
				objectPtr->pos[2] += 0.01 * camera_zoom;
				Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
				                                          objectPtr->pos[2]);
			}
			else if (KeyPressed(VK_E))
			{
				objectPtr->pos[2] -= 0.01 * camera_zoom;
				Command<Commands::SET_OBJECT_COORDINATES>(objectPtr->editorObject, objectPtr->pos[0], objectPtr->pos[1],
				                                          objectPtr->pos[2]);
			}
		}
	}
}

void fParticles()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("particles"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentParticle, &namesParticles);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("particle"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_particles.size()));
		currentMissionPtr->list_particles.push_back(new Particle(name.c_str(), playerPos.x, playerPos .y, playerPos.z,
		                                                      currentMissionPtr->list_targets.empty()
			                                                      ? 0
			                                                      : currentMissionPtr->list_targets.size() - 1));
		currentParticle = currentMissionPtr->list_particles.size() - 1;
		namesParticles.push_back(currentMissionPtr->list_particles[currentParticle]->name);
		currentMissionPtr->list_particles[currentMissionPtr->list_particles.size() - 1]->updateEditorParticle();
	}
	if (!currentMissionPtr->list_particles.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_particle = currentMissionPtr->list_particles.size();
			Particle* particle = new Particle(*currentMissionPtr->list_particles[currentParticle]);
			particle->updateEditorParticle();
			currentMissionPtr->list_particles.push_back(particle);
			string str;
			str.append(currentMissionPtr->list_particles[new_particle]->name);
			str.append("c");
			strcpy(particle->name, str.c_str());

			namesParticles.push_back(particle->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_particles[currentParticle]->removeEditorParticle();
			delete currentMissionPtr->list_particles[currentParticle];
			currentMissionPtr->list_particles.erase(currentMissionPtr->list_particles.begin() + currentParticle);
			namesParticles.erase(namesParticles.begin() + currentParticle);
			if (currentParticle > 0)
			{
				currentParticle -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_particles[currentParticle]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_particles[currentParticle]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//particle render
	if (!currentMissionPtr->list_particles.empty())
	{
		Particle* particlePtr = currentMissionPtr->list_particles[currentParticle];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("particle"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			particlePtr->pos[0] = playerPos.x;
			particlePtr->pos[1] = playerPos.y;
			particlePtr->pos[2] = playerPos.z;
			particlePtr->updateEditorParticle();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		if (ImGui::InputFloat3(langt("position"), particlePtr->pos, "%.6f"))
		{
			particlePtr->updateEditorParticle();
		}

		if (ImGui::DragInt3(langt("rotate"), particlePtr->rotation, 0.3, -360.0, 360.0, u8"%d°"))
		{
			particlePtr->updateEditorParticle();
		}
		if (Combo(langt("particle"), particlePtr->modelID, &Particle_name))
		{
			particlePtr->updateEditorParticle();
		}
		if (ImGui::SliderInt(langt("tied"), &particlePtr->tied, 0, 3,
		                     langMenu["CutscenePos"][particlePtr->tied].c_str()))
		{
			particlePtr->tiedID = 0;
		}
		if (particlePtr->tied == 1)
		{
			Combo(langt("actor"), particlePtr->tiedID, &namesActors);
		}
		else if (particlePtr->tied == 2)
		{
			Combo(langt("car"), particlePtr->tiedID, &namesCars);
		}
		else if (particlePtr->tied == 3)
		{
			Combo(langt("object"), particlePtr->tiedID, &namesObjects);
		}

		ToggleButton(langt("useTarget"), &particlePtr->useTarget);
		if (particlePtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), particlePtr->startC, &namesTargets);
			Combo(langt("dis_after"), particlePtr->endC, &list_tg_m);
		}

		ImGui::End();

		//edit
		ImGui::SetNextWindowBgAlpha(0.50f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
		             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
		             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());
		ImGui::Text(langMenu["infoOverlay"][2].c_str());
		ImGui::Text(langMenu["infoOverlay"][3].c_str());
		ImGui::Text(langMenu["infoOverlay"][4].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

		float cx = particlePtr->pos[0], cy = particlePtr->pos[1], cz = particlePtr->pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(particlePtr->pos[0], particlePtr->pos[1], particlePtr->pos[2], 2);
		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
				if (camera_zoom < 1.0f)
					camera_zoom = 1.0f;
			}
		}
		if (KeyPressed(VK_SHIFT))
		{
			if (KeyPressed(VK_UP))
			{
				particlePtr->rotation[0]++;
				particlePtr->updateEditorParticle();
			}
			else if (KeyPressed(VK_DOWN))
			{
				particlePtr->rotation[0]--;
				particlePtr->updateEditorParticle();
			}
			if (KeyPressed(VK_LEFT))
			{
				particlePtr->rotation[1]++;
				particlePtr->updateEditorParticle();
			}
			else if (KeyPressed(VK_RIGHT))
			{
				particlePtr->rotation[1]--;
				particlePtr->updateEditorParticle();
			}
			if (KeyPressed(VK_Q))
			{
				particlePtr->rotation[2]++;
				particlePtr->updateEditorParticle();
			}
			else if (KeyPressed(VK_E))
			{
				particlePtr->rotation[2]--;
				particlePtr->updateEditorParticle();
			}
		}
		else
		{
			if (KeyPressed(VK_UP))
			{
				particlePtr->pos[0] -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
				particlePtr->pos[1] -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
				particlePtr->updateEditorParticle();
			}
			else if (KeyPressed(VK_DOWN))
			{
				particlePtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
				particlePtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
				particlePtr->updateEditorParticle();
			}
			if (KeyPressed(VK_LEFT))
			{
				particlePtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
				particlePtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
				particlePtr->updateEditorParticle();
			}
			else if (KeyPressed(VK_RIGHT))
			{
				particlePtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
				particlePtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
				particlePtr->updateEditorParticle();
			}
			if (KeyPressed(VK_Q))
			{
				particlePtr->pos[2] += 0.01 * camera_zoom;
				particlePtr->updateEditorParticle();
			}
			else if (KeyPressed(VK_E))
			{
				particlePtr->pos[2] -= 0.01 * camera_zoom;
				particlePtr->updateEditorParticle();
			}
		}
	}
}

void fPickups()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("pickups"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentPickup, &namesPickups);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("pickup"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_pickups.size()));
		currentMissionPtr->list_pickups.push_back(new Pickup(name.c_str(), playerPos.x, playerPos.y, playerPos.z,
		                                                  currentMissionPtr->list_targets.empty()
			                                                  ? 0
			                                                  : currentMissionPtr->list_targets.size() - 1));
		currentPickup = currentMissionPtr->list_pickups.size() - 1;
		namesPickups.push_back(currentMissionPtr->list_pickups[currentPickup]->name);
		currentMissionPtr->list_pickups[currentMissionPtr->list_pickups.size() - 1]->updateEditorPickup();
	}
	if (currentMissionPtr->list_pickups.size() > 0)
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_pickup = currentMissionPtr->list_pickups.size();
			Pickup* pickup = new Pickup(*currentMissionPtr->list_pickups[currentPickup]);
			pickup->updateEditorPickup();
			currentMissionPtr->list_pickups.push_back(pickup);
			string str;
			str.append(currentMissionPtr->list_pickups[new_pickup]->name);
			str.append("c");
			strcpy(pickup->name, str.c_str());

			namesPickups.push_back(pickup->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_pickups[currentPickup]->removeEditorPickup();
			delete currentMissionPtr->list_pickups[currentPickup];
			currentMissionPtr->list_pickups.erase(currentMissionPtr->list_pickups.begin() + currentPickup);
			namesPickups.erase(namesPickups.begin() + currentPickup);
			if (currentPickup > 0)
			{
				currentPickup -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_pickups[currentPickup]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_pickups[currentPickup]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//pickup render
	if (!currentMissionPtr->list_pickups.empty())
	{
		Pickup* pickupPtr = currentMissionPtr->list_pickups[currentPickup];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("pickup"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			pickupPtr->pos[0] = playerPos.x;
			pickupPtr->pos[1] = playerPos.y;
			pickupPtr->pos[2] = playerPos.z;
			pickupPtr->updateEditorPickup();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		if (ImGui::InputFloat3(langt("position"), pickupPtr->pos, "%.6f"))
		{
			pickupPtr->updateEditorPickup();
		}

		ImGui::SliderInt(langt("spawn"), &pickupPtr->spawnType, 0, 2,
		                 langMenu["spawn_type_pickup"][pickupPtr->spawnType].c_str());
		if (Combo(langt("type"), pickupPtr->type, &langMenu["type_pickup"]))
		{
			pickupPtr->updateEditorPickup();
		}
		if (pickupPtr->type == 0)
		{
			ImGui::Separator();
			if (ImGui::ImageButton(static_cast<void*>(weaponsAtlas), ImVec2(52, 52),
			                       ImVec2(pickupPtr->weapon * 0.02272727272f, 0),
			                       ImVec2((pickupPtr->weapon + 1) * 0.02272727272f, 1)))
			{
				ImGui::OpenPopup("weapon");
			}

			ImGui::SameLine();
			ImGui::Text(langt("weapon"));

			if (ImGui::InputInt(langt("ammo"), &pickupPtr->ammo))
			{
				if (pickupPtr->ammo < 0)
					pickupPtr->ammo = 0;
			}

			//weapon popup
			if (ImGui::BeginPopup("weapon"))
			{
				ImGui::BeginChild("weapon", ImVec2(200, 450));

				for (int i = 0; i < IM_ARRAYSIZE(ID_Weapons); i++)
				{
					ImGui::PushID(i);
					if (ImGui::ImageButton(weaponsAtlas, ImVec2(52, 52), ImVec2(i * 0.02272727272f, 0),
					                       ImVec2((i + 1) * 0.02272727272f, 1)))
					{
						pickupPtr->weapon = i;
						pickupPtr->updateEditorPickup();
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip(langMenu["weap_names"][i].c_str());
					}
					ImGui::PopID();
					if ((i + 1) % 3 != 0)
					{
						ImGui::SameLine();
					}
				}

				ImGui::EndChild();
				ImGui::EndPopup();
			}
		}
		else if (pickupPtr->type == 5)
		{
			ImGui::InputInt(langt("model"), &pickupPtr->modelID);
			if (ImGui::Button(langt("apply")))
			{
				pickupPtr->updateEditorPickup();
			}
		}
		ImGui::Separator();

		ToggleButton(langt("useTarget"), &pickupPtr->useTarget);
		if (pickupPtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), pickupPtr->startC, &namesTargets);
			Combo(langt("dis_after"), pickupPtr->endC, &list_tg_m);
		}

		ImGui::End();

		//edit
		ImGui::SetNextWindowBgAlpha(0.50f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
		             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
		             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());
		ImGui::Text(langMenu["infoOverlay"][2].c_str());
		ImGui::Text(langMenu["infoOverlay"][3].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

		float cx = pickupPtr->pos[0], cy = pickupPtr->pos[1], cz = pickupPtr->pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(pickupPtr->pos[0], pickupPtr->pos[1], pickupPtr->pos[2], 2);
		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
				if (camera_zoom < 1.0f)
					camera_zoom = 1.0f;
			}
		}
		if (KeyPressed(VK_UP))
		{
			pickupPtr->pos[0] -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
			pickupPtr->pos[1] -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
			pickupPtr->updateEditorPickup();
		}
		else if (KeyPressed(VK_DOWN))
		{
			pickupPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
			pickupPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
			pickupPtr->updateEditorPickup();
		}
		if (KeyPressed(VK_LEFT))
		{
			pickupPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
			pickupPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
			pickupPtr->updateEditorPickup();
		}
		else if (KeyPressed(VK_RIGHT))
		{
			pickupPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
			pickupPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
			pickupPtr->updateEditorPickup();
		}
		if (KeyPressed(VK_Q))
		{
			pickupPtr->pos[2] += 0.01 * camera_zoom;
			pickupPtr->updateEditorPickup();
		}
		else if (KeyPressed(VK_E))
		{
			pickupPtr->pos[2] -= 0.01 * camera_zoom;
			pickupPtr->updateEditorPickup();
		}
	}
}

void fExplosions()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("explosions"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentExplosion, &namesExplosions);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("explosion"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_explosions.size()));
		currentMissionPtr->list_explosions.push_back(new Explosion(name.c_str(), playerPos.x, playerPos.y, playerPos.z,
		                                                        currentMissionPtr->list_targets.empty()
			                                                        ? 0
			                                                        : currentMissionPtr->list_targets.size() - 1));
		currentExplosion = currentMissionPtr->list_explosions.size() - 1;
		namesExplosions.push_back(currentMissionPtr->list_explosions[currentExplosion]->name);
		currentMissionPtr->list_explosions[currentMissionPtr->list_explosions.size() - 1]->updateEditorExplosion();
	}
	if (currentMissionPtr->list_explosions.size() > 0)
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_explosion = currentMissionPtr->list_explosions.size();
			Explosion* explosion = new Explosion(*currentMissionPtr->list_explosions[currentExplosion]);
			explosion->updateEditorExplosion();
			currentMissionPtr->list_explosions.push_back(explosion);
			string str;
			str.append(currentMissionPtr->list_explosions[new_explosion]->name);
			str.append("c");
			strcpy(explosion->name, str.c_str());

			namesExplosions.push_back(explosion->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_explosions[currentExplosion]->removeEditorExplosion();
			delete currentMissionPtr->list_explosions[currentExplosion];
			currentMissionPtr->list_explosions.erase(currentMissionPtr->list_explosions.begin() + currentExplosion);
			namesExplosions.erase(namesExplosions.begin() + currentExplosion);
			if (currentExplosion > 0)
			{
				currentExplosion -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_explosions[currentExplosion]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_explosions[currentExplosion]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//explosion render
	if (!currentMissionPtr->list_explosions.empty())
	{
		Explosion* explosionPtr = currentMissionPtr->list_explosions[currentExplosion];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("explosion"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			explosionPtr->pos[0] = playerPos.x;
			explosionPtr->pos[1] = playerPos.y;
			explosionPtr->pos[2] = playerPos.z;
			explosionPtr->updateEditorExplosion();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		if (ImGui::InputFloat3(langt("position"), explosionPtr->pos, "%.6f"))
		{
			explosionPtr->updateEditorExplosion();
		}

		if (ImGui::SliderInt(langt("type"), &explosionPtr->type, 0, 1,
		                     langMenu["type_expl"][explosionPtr->type].c_str()))
		{
			explosionPtr->updateEditorExplosion();
		}
		if (explosionPtr->type == 0)
		{
			ImU8 min_fire = 0;
			ImU8 max_fire = 5;
			if (ImGui::SliderScalar(langt("size_fire"), ImGuiDataType_U8, &explosionPtr->sizeFire, &min_fire, &max_fire))
			{
				explosionPtr->updateEditorExplosion();
			}
			ImGui::SetNextItemWidth(120);
			ImGui::DragScalar(langt("spread_fire"), ImGuiDataType_U8, &explosionPtr->propagationFire, 0.1f);
		}
		else
		{
			ImGui::DragScalar(langt("type_explosion"), ImGuiDataType_U8, &explosionPtr->typeExplosion, 0.1f);
		}

		ToggleButton(langt("useTarget"), &explosionPtr->useTarget);
		if (explosionPtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), explosionPtr->startC, &namesTargets);
			Combo(langt("dis_after"), explosionPtr->endC, &list_tg_m);
		}

		ImGui::End();

		//edit
		ImGui::SetNextWindowBgAlpha(0.50f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
		             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
		             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());
		ImGui::Text(langMenu["infoOverlay"][2].c_str());
		ImGui::Text(langMenu["infoOverlay"][3].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

		float cx = explosionPtr->pos[0], cy = explosionPtr->pos[1], cz = explosionPtr->pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(explosionPtr->pos[0], explosionPtr->pos[1], explosionPtr->pos[2], 2);
		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
				if (camera_zoom < 1.0f)
					camera_zoom = 1.0f;
			}
		}
		if (KeyPressed(VK_UP))
		{
			explosionPtr->pos[0] -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
			explosionPtr->pos[1] -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
			explosionPtr->updateEditorExplosion();
		}
		else if (KeyPressed(VK_DOWN))
		{
			explosionPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
			explosionPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
			explosionPtr->updateEditorExplosion();
		}
		if (KeyPressed(VK_LEFT))
		{
			explosionPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
			explosionPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
			explosionPtr->updateEditorExplosion();
		}
		else if (KeyPressed(VK_RIGHT))
		{
			explosionPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
			explosionPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
			explosionPtr->updateEditorExplosion();
		}
		if (KeyPressed(VK_Q))
		{
			explosionPtr->pos[2] += 0.01 * camera_zoom;
			explosionPtr->updateEditorExplosion();
		}
		else if (KeyPressed(VK_E))
		{
			explosionPtr->pos[2] -= 0.01 * camera_zoom;
			explosionPtr->updateEditorExplosion();
		}
	}
}

void fAudios()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_CARS);
	name.append(" ");
	name.append(langt("audios"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentAudio, &namesAudios);

	if (ImGui::Button(langt("create")))
	{
		CVector playerPos = playerPed->GetPosition();
		float angle = deg(playerPed->GetHeading());

		std::string name(langt("audio"));
		name.append(" #");
		name.append(std::to_string(currentMissionPtr->list_audios.size()));
		currentMissionPtr->list_audios.push_back(new Audio(name.c_str(), playerPos.x, playerPos.y, playerPos.z,
		                                                currentMissionPtr->list_targets.empty()
			                                                ? 0
			                                                : currentMissionPtr->list_targets.size() - 1));
		currentAudio = currentMissionPtr->list_audios.size() - 1;
		namesAudios.push_back(currentMissionPtr->list_audios[currentAudio]->name);
		currentMissionPtr->list_audios[currentMissionPtr->list_audios.size() - 1]->updateEditorAudio(true);
	}
	if (!currentMissionPtr->list_audios.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_audio = currentMissionPtr->list_audios.size();
			Audio* audio = new Audio(*currentMissionPtr->list_audios[currentAudio]);
			audio->updateEditorAudio();
			currentMissionPtr->list_audios.push_back(audio);
			string str;
			str.append(currentMissionPtr->list_audios[new_audio]->name);
			str.append("c");
			strcpy(audio->name, str.c_str());

			namesAudios.push_back(audio->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentMissionPtr->list_audios[currentAudio]->removeEditorAudio();
			delete currentMissionPtr->list_audios[currentAudio];
			currentMissionPtr->list_audios.erase(currentMissionPtr->list_audios.begin() + currentAudio);
			namesAudios.erase(namesAudios.begin() + currentAudio);
			if (currentAudio > 0)
			{
				currentAudio -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentMissionPtr->list_audios[currentAudio]->name[0],
		                 IM_ARRAYSIZE(currentMissionPtr->list_audios[currentAudio]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//audio render
	if (!currentMissionPtr->list_audios.empty())
	{
		Audio* audioPtr = currentMissionPtr->list_audios[currentAudio];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("audio"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		Combo(langt("sound"), audioPtr->sound, &namesAudioFiles);

		if (ImGui::Button(langt("update")))
		{
			audioPtr->loadAudiosList();
		}

		if (ToggleButton(langt("audio3d"), &audioPtr->audio3D))
		{
			audioPtr->updateEditorAudio(true);
			audioPtr->audio3DAttach = 0;
		}
		if (audioPtr->audio3D)
		{
			ImU8 min_p = 0;
			ImU8 max_p = 3;
			if (ImGui::SliderScalar(langt("place"), ImGuiDataType_U8, &audioPtr->audio3DType, &min_p, &max_p,
			                        langMenu["place_type"][audioPtr->audio3DType].c_str()))
			{
				audioPtr->updateEditorAudio();
			}

			if (audioPtr->audio3DType == 0)
			{
				if (ImGui::Button(ICON_FA_STREET_VIEW))
				{
					CVector playerPos = playerPed->GetPosition();
					audioPtr->pos[0] = playerPos.x;
					audioPtr->pos[1] = playerPos.y;
					audioPtr->pos[2] = playerPos.z;
					audioPtr->updateEditorAudio();
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(langt("playerCoordinates"));
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(270);
				if (ImGui::InputFloat3(langt("position"), audioPtr->pos, "%.6f"))
				{
					audioPtr->updateEditorAudio();
				}

				//edit
				ImGui::SetNextWindowBgAlpha(0.50f);
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
				ImGui::Begin("info", nullptr,
				             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
				             ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

				ImGui::Text(langMenu["infoOverlay"][0].c_str());
				ImGui::Text(langMenu["infoOverlay"][1].c_str());
				ImGui::Text(langMenu["infoOverlay"][2].c_str());
				ImGui::Text(langMenu["infoOverlay"][3].c_str());

				ImGui::End();

				isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

				float cx = audioPtr->pos[0], cy = audioPtr->pos[1], cz = audioPtr->pos[2];
				cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
					static_cast<float>(rad(camera_angle[1]))));
				cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
					static_cast<float>(rad(camera_angle[1]))));
				cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
				Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
				Command<Commands::POINT_CAMERA_AT_POINT>(audioPtr->pos[0], audioPtr->pos[1], audioPtr->pos[2], 2);
				if (!isWindow)
				{
					if (ImGui::IsMouseDragging(2))
					{
						ImVec2 dt = ImGui::GetIO().MouseDelta;
						camera_angle[0] += dt.x;
						camera_angle[1] += dt.y;
					}
					if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
					{
						camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
						if (camera_zoom < 1.0f)
							camera_zoom = 1.0f;
					}
				}
				if (KeyPressed(VK_UP))
				{
					audioPtr->pos[0] -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
					audioPtr->pos[1] -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
					audioPtr->updateEditorAudio();
				}
				else if (KeyPressed(VK_DOWN))
				{
					audioPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
					audioPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
					audioPtr->updateEditorAudio();
				}
				if (KeyPressed(VK_LEFT))
				{
					audioPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
					audioPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
					audioPtr->updateEditorAudio();
				}
				else if (KeyPressed(VK_RIGHT))
				{
					audioPtr->pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
					audioPtr->pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
					audioPtr->updateEditorAudio();
				}
				if (KeyPressed(VK_Q))
				{
					audioPtr->pos[2] += 0.01 * camera_zoom;
					audioPtr->updateEditorAudio();
				}
				else if (KeyPressed(VK_E))
				{
					audioPtr->pos[2] -= 0.01 * camera_zoom;
					audioPtr->updateEditorAudio();
				}
			}
			else if (audioPtr->audio3DType == 1)
			{
				Combo(langt("actor"), audioPtr->audio3DAttach, &namesActors);
			}
			else if (audioPtr->audio3DType == 2)
			{
				Combo(langt("car"), audioPtr->audio3DAttach, &namesCars);
			}
			else if (audioPtr->audio3DType == 3)
			{
				Combo(langt("object"), audioPtr->audio3DAttach, &namesObjects);
			}
		}

		ToggleButton(langt("repeat"), &audioPtr->repeat);

		ToggleButton(langt("useTarget"), &audioPtr->useTarget);
		if (audioPtr->useTarget)
		{
			vector<const char*> list_tg_m = namesTargets;
			list_tg_m.insert(list_tg_m.begin(), langt("toend"));
			Combo(langt("app_on"), audioPtr->startC, &namesTargets);
			Combo(langt("dis_after"), audioPtr->endC, &list_tg_m);
		}

		ImGui::End();
	}
}

void fPlayer()
{
	bool isWindow = false;
	std::string name;

	//player render
	Player* playerPtr = &currentMissionPtr->player;
	ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 400, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_PARKING);
	name.append(" ");
	name.append(langt("player"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

	if (ImGui::Button(ICON_FA_STREET_VIEW))
	{
		CVector playerPos = playerPed->GetPosition();
		playerPtr->pos[0] = playerPos.x;
		playerPtr->pos[1] = playerPos.y;
		playerPtr->pos[2] = playerPos.z;
		playerPtr->editorPed->SetPosn(playerPos);
		Command<Commands::GET_AREA_VISIBLE>(&playerPtr->interiorID);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(langt("playerCoordinates"));
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(270);
	ImGui::InputFloat3(langt("position"), playerPtr->pos, "%.6f");
	ImGui::PushID("angle");
	if (ImGui::Button(ICON_FA_STREET_VIEW))
	{
		float angle = static_cast<float>(deg(playerPed->GetHeading()));
		playerPtr->angle = angle;
		playerPtr->editorPed->SetHeading(static_cast<float>(rad(playerPtr->angle)));
	}
	ImGui::PopID();
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(langt("playerAngle"));
	}

	ImGui::SameLine();
	if (ImGui::DragFloat(langt("angle"), &playerPtr->angle, 1, -360, 360))
	{
		playerPtr->editorPed->SetHeading(static_cast<float>(rad(playerPtr->angle)));
	}

	ImGui::Separator();

	unsigned char minS = 0;
	unsigned char maxS = 1;
	if (ImGui::SliderScalar("", ImGuiDataType_U8, &playerPtr->modelType, &minS, &maxS,(playerPtr->modelType == 0) ? langt("ped") : langt("specActor")))
	{
		playerPtr->modelID = 0;
		playerPtr->updateEditorPed();
	}

	//skin popup open
	if (playerPtr->modelType == 0)
	{
		if (ImGui::Button(ICON_FA_TSHIRT))
			ImGui::OpenPopup("skins");


		ImGui::SameLine();

		if (ImGui::InputInt(langt("model"), &playerPtr->modelID))
		{
			int id_a = 0;
			for (int v = 0; v < ID_Actors.size(); v++)
			{
				if (playerPtr->modelID <= ID_Actors[v])
				{
					id_a = ID_Actors[v];
					break;
				}
			}
			playerPtr->modelID = id_a;
			playerPtr->updateEditorPed();
		}
	}
	else if (Combo(langt("model"), playerPtr->modelID, &ID_Spec_Actors))
	{
		playerPtr->updateEditorPed();
	}

	ImGui::Separator();

	if (ImGui::TreeNode(langt("chartics")))
	{
		ImGui::InputInt(langt("countlive"), &playerPtr->health);

		if (ImGui::ImageButton(static_cast<void*>(weaponsAtlas), ImVec2(52, 52),
		                       ImVec2(playerPtr->weapon * 0.02272727272f, 0),
		                       ImVec2((playerPtr->weapon + 1) * 0.02272727272f, 1)))
		{
			ImGui::OpenPopup("weapon");
		}

		ImGui::SameLine();
		ImGui::Text(langt("weapon"));

		if (ImGui::InputInt(langt("ammo"), &playerPtr->ammo))
		{
			if (playerPtr->ammo < 0)
				playerPtr->ammo = 0;
		}

		//weapon popup
		if (ImGui::BeginPopup("weapon"))
		{
			ImGui::BeginChild("weapon", ImVec2(200, 450));

			for (int i = 0; i < IM_ARRAYSIZE(ID_Weapons); i++)
			{
				ImGui::PushID(i);
				if (ImGui::ImageButton(weaponsAtlas, ImVec2(52, 52), ImVec2(i * 0.02272727272f, 0),
				                       ImVec2((i + 1) * 0.02272727272f, 1)))
				{
					playerPtr->weapon = i;
					playerPtr->updateEditorPed();
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(langMenu["weap_names"][i].c_str());
				}
				ImGui::PopID();
				if ((i + 1) % 3 != 0)
				{
					ImGui::SameLine();
				}
			}

			ImGui::EndChild();
			ImGui::EndPopup();
		}

		ImGui::TreePop();
	}

	ImGui::Separator();

	if (ImGui::Button(langt("edithand")))
	{
		editPlayer = true;
		bPlayer = false;
		playerPtr->removeEditorPed();

		int modelID = playerPtr->modelID;
		int weapon = playerPtr->weapon;
		if (playerPtr->modelType == 0)
		{
			CStreaming::RequestModel(modelID, 0);
			CStreaming::LoadAllRequestedModels(false);
		}
		else
		{
			std::string modell_n = ID_Spec_Actors[modelID];
			CStreaming::RequestSpecialChar(9, modell_n.c_str(), 0);
			CStreaming::LoadAllRequestedModels(false);
			modelID = 290 + 10 - 1;
		}
		Command<Commands::SET_PLAYER_MODEL>(0, modelID);
		CStreaming::SetSpecialCharIsDeletable(9);
		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, false);
		playerPed->SetPosn(CVector(playerPtr->pos[0], playerPtr->pos[1], playerPtr->pos[2]));
		Command<Commands::SET_CHAR_HEADING>(playerPed, playerPtr->angle);
		Command<Commands::SET_AREA_VISIBLE>(playerPtr->interiorID);
	}

	ImGui::PushItemWidth(150);
	ImGui::Separator();

	//edit
	ImGui::SetNextWindowBgAlpha(0.50f);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
	ImGui::Begin("info", nullptr,
	             ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoSavedSettings +
	             ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

	ImGui::Text(langMenu["infoOverlay"][0].c_str());
	ImGui::Text(langMenu["infoOverlay"][1].c_str());

	ImGui::End();

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();


	float cx = playerPtr->pos[0], cy = playerPtr->pos[1], cz = playerPtr->pos[2];
	cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(static_cast<float>(rad(camera_angle[1]))));
	cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(static_cast<float>(rad(camera_angle[1]))));
	cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
	Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
	Command<Commands::POINT_CAMERA_AT_POINT>(playerPtr->pos[0], playerPtr->pos[1], playerPtr->pos[2] + 1, 2);
	if (!isWindow)
	{
		if (ImGui::IsMouseDragging(2))
		{
			ImVec2 dt = ImGui::GetIO().MouseDelta;
			camera_angle[0] += dt.x;
			camera_angle[1] += dt.y;
		}
		if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
		{
			camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4.0;
			if (camera_zoom < 1.0f)
				camera_zoom = 1.0f;
		}
	}

	//skin popup
	if (ImGui::BeginPopup("skins"))
	{
		ImGui::BeginChild("skins", ImVec2(340, 450));


		for (int i = 0; i < ID_Actors.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::ImageButton(pedsSkinAtlas, ImVec2(55, 100), ImVec2((i * 55) / 14630.0f, 0),
			                       ImVec2(((i + 1) * 55) / 14630.0f, 1)))
			{
				playerPtr->modelID = ID_Actors
					[i];
				playerPtr->updateEditorPed();
			}
			ImGui::PopID();
			if ((i + 1) % 5 != 0)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndChild();
		ImGui::EndPopup();
	}

	ImGui::End();
}

void fMissionPacks()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_THEATER_MASKS);
	name.append(" ");
	name.append(langt("missionPacks"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(260);
	ListBox("", currentMissionPack, &namesMissionPacks);

	if (ImGui::Button(langt("create")))
	{
		name.clear();
		name.append(langt("missionPack"));
		name.append(" #");
		name.append(std::to_string(namesMissionPacks.size()));
		char* name_c = new char[65];
		strcpy(name_c, name.c_str());
		namesMissionPacks.push_back(name_c);
		namesMissions.emplace_back();
		name.clear();
		name.append("LDYOM//Missions_packs//");
		name.append(replace_symb(std::string(name_c)));
		name = UTF8_to_CP1251(name);
		boost::filesystem::create_directory(name.c_str());
		name.append("//audio");
		boost::filesystem::create_directory(name.c_str());
		currentMissionPack = namesMissionPacks.size() - 1;
	}

	if (!namesMissionPacks.empty()) {
		ImGui::SameLine();
		if (ImGui::Button(langt("rename"))) {
			strcpy(temp_nameMissionPack, namesMissionPacks[currentMissionPack].c_str());
			ImGui::OpenPopup("rename");
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("delete"))) {
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	//delete
	const ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		const ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			std::stringstream path;
			path << "LDYOM//Missions_packs//" << replace_symb(namesMissionPacks[currentMissionPack]);
			boost::filesystem::remove_all(UTF8_to_CP1251(path.str()));
			namesMissionPacks.erase(namesMissionPacks.begin() + currentMissionPack);
			namesMissions.erase(namesMissions.begin() + currentMissionPack);
			if (currentMissionPack > 0)
			{
				currentMissionPack -= 1;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", temp_nameMissionPack,IM_ARRAYSIZE(temp_nameMissionPack));

		if (ImGui::Button(langt("apply"))) {
			std::stringstream old_path;
			old_path << "LDYOM//Missions_packs//" << replace_symb(namesMissionPacks[currentMissionPack]);
			std::stringstream new_path;
			new_path << "LDYOM//Missions_packs//" << replace_symb(string(temp_nameMissionPack));
			boost::filesystem::rename(UTF8_to_CP1251(old_path.str()), UTF8_to_CP1251(new_path.str()));
			namesMissionPacks[currentMissionPack] = temp_nameMissionPack;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
	
	//mission render
	if (!namesMissionPacks.empty()) {
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 540, 0), ImGuiCond_Appearing);
		ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_THEATER_MASKS);
		name.append(" ");
		name.append(langt("missions"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		//List
		ImGui::SetNextItemWidth(255.0f);
		ListBox("", currentMission, &namesMissions[currentMissionPack]);

		if (ImGui::Button(langt("create")))
		{
			currentMission = namesMissions[currentMissionPack].size();
			std::stringstream path;
			path << "LDYOM//Missions_packs//" << UTF8_to_CP1251(replace_symb(namesMissionPacks[currentMissionPack])) << "//" << replace_symb(currentMissionPtr->name);
			while (boost::filesystem::exists(UTF8_to_CP1251(path.str()))) {
				currentMissionPtr->name += "c";
			}
			namesMissions.at(currentMissionPack).push_back(currentMissionPtr->name);
			Manager::SaveMission(currentMissionPack, currentMission);
			Manager::SaveListMission(currentMissionPack);
			nameCurrPack = &namesMissionPacks[currentMissionPack];

			Audio::loadAudiosList();

			fastdata_pack = currentMissionPack;
			fastdata_miss = currentMission;
		}
		if (!namesMissions[currentMissionPack].empty())
		{
			ImGui::SameLine();

			if (ImGui::Button(langt("save")))
			{
				const std::string path = "LDYOM//Missions_packs//";
				std::string path_pack = replace_symb(UTF8_to_CP1251(namesMissionPacks[currentMissionPack])) + "//";
				std::string name_miss = replace_symb(UTF8_to_CP1251(namesMissions[currentMissionPack][currentMission]));
				boost::filesystem::rename(path + path_pack + name_miss + ".bin", path + path_pack + replace_symb(UTF8_to_CP1251(currentMissionPtr->name)) + ".bin");
				namesMissions[currentMissionPack][currentMission] = currentMissionPtr->name;
				Manager::SaveMission(currentMissionPack, currentMission);
				Manager::SaveListMission(currentMissionPack);
				nameCurrPack = &namesMissionPacks[currentMissionPack];

				Audio::loadAudiosList();

				fastdata_pack = currentMissionPack;
				fastdata_miss = currentMission;
				std::string saved = UTF8_to_CP1251(langt("saved"));
				GXTEncode(saved);
				CHud::SetHelpMessage(saved.c_str(), true, false, false);
			}
			ImGui::SameLine();
			if (ImGui::Button(langt("load")))
			{
				name.clear();
				name.append(ICON_FA_EXCLAMATION_TRIANGLE);
				name.append(" ");
				name.append(langt("load"));
				ImGui::OpenPopup(name.c_str());
			}
			if (ImGui::Button(langt("cut")))
			{
				moveMission = currentMission;
			}
			if (moveMission != -1)
			{
				ImGui::SameLine();
				if (ImGui::Button(langt("paste")))
				{
					moveCellVector<std::string>(&namesMissions[currentMissionPack], &moveMission, &currentMission);
					moveMission = -1;
				}
			}
			ImGui::SameLine();

			if (ImGui::Button(langt("delete")))
			{
				name.clear();
				name.append(ICON_FA_TRASH_ALT);
				name.append(" ");
				name.append(langt("delete"));
				ImGui::OpenPopup(name.c_str());
			}

		}

		name.clear();
		name.append(ICON_FA_TRASH_ALT);
		name.append(" ");
		name.append(langt("delete"));
		if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(langt("deleteQues"));

			const ImVec2 size_b = ImVec2(100, 0);

			if (ImGui::Button(langt("yes"), size_b)) {
				const std::string path = "LDYOM//Missions_packs//";
				std::string path_pack = replace_symb(UTF8_to_CP1251(namesMissionPacks[currentMissionPack])) + "//";
				std::string name_miss = replace_symb(UTF8_to_CP1251(namesMissions[currentMissionPack][currentMission]));
				boost::filesystem::remove_all(path + path_pack + name_miss + ".bin");
				namesMissions[currentMissionPack].erase(namesMissions[currentMissionPack].begin() + currentMission);
				Manager::SaveListMission(currentMissionPack);
				if (currentMission > 0) {
					currentMission--;
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button(langt("no"), size_b))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		name.clear();
		name.append(ICON_FA_EXCLAMATION_TRIANGLE);
		name.append(" ");
		name.append(langt("load"));
		if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

			ImGui::Text(langMenu["reloadQues"][1].c_str());

			const ImVec2 size_b = ImVec2(160, 0);

			if (ImGui::Button(langt("yes"), size_b)) {
				currentMissionPtr->removeEditorEntity();
				Manager::LoadMission(currentMissionPack, currentMission);
				currentMissionPtr->updateEditorEntity();

				playerPed->SetPosn(currentMissionPtr->player.pos[0], currentMissionPtr->player.pos[1], currentMissionPtr->player.pos[2]);
				Command<Commands::SET_CHAR_AREA_VISIBLE>(currentMissionPtr->player.editorPed, currentMissionPtr->player.interiorID);
				Command<Commands::SET_AREA_VISIBLE>(currentMissionPtr->player.interiorID);

				namesTargets.clear();
				for (auto& list_target : currentMissionPtr->list_targets)
				{
					namesTargets.push_back(list_target->name);
				}
				namesActors.clear();
				for (auto& list_actor : currentMissionPtr->list_actors)
				{
					namesActors.push_back(list_actor->name);
				}
				namesCars.clear();
				for (auto& list_car : currentMissionPtr->list_cars)
				{
					namesCars.push_back(list_car->name);
				}
				namesObjects.clear();
				for (auto& list_object : currentMissionPtr->list_objects)
				{
					namesObjects.push_back(list_object->name);
				}
				namesPickups.clear();
				for (auto& list_pickup : currentMissionPtr->list_pickups)
				{
					namesPickups.push_back(list_pickup->name);
				}
				namesParticles.clear();
				for (auto& list_particle : currentMissionPtr->list_particles)
				{
					namesParticles.push_back(list_particle->name);
				}
				namesExplosions.clear();
				for (auto& list_explosion : currentMissionPtr->list_explosions)
				{
					namesExplosions.push_back(list_explosion->name);
				}
				namesAudios.clear();
				for (auto& list_audio : currentMissionPtr->list_audios)
				{
					namesAudios.push_back(list_audio->name);
				}

				nameCurrPack = &namesMissionPacks[currentMissionPack];

				Audio::loadAudiosList();

				fastdata_pack = currentMissionPack;
				fastdata_miss = currentMission;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button(langt("no"), size_b)) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

}

void fMissionSettings()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 100, resolution.y / 2 - 200), ImGuiCond_Always);
	ImGui::Begin(langt("settingsMiss"), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImVec2 size_b = ImVec2(160, 0);

	InputText(langt("nameMission"),65, &currentMissionPtr->name,0, nullptr,nullptr);
	ImGui::Separator();
	Combo(langt("weather"), currentMissionPtr->weather, &langMenu["Weather_arr"]);
	if (ImGui::Button(langt("preview"))) {
		CWeather::ForceWeatherNow(currentMissionPtr->weather);
	}
	if (ImGui::Button(langt("unPreview"))) {
		CWeather::ForceWeatherNow(0);
	}
	ImGui::Separator();
	if (ImGui::Button(langt("timeGameMiss"))) {
		std::string HMTIM = UTF8_to_CP1251(langt("HMTIM"));
		GXTEncode(HMTIM);
		CHud::SetHelpMessage(HMTIM.c_str(), false, true, false);
		editmodeTimemiss = true;
		bMissionSettings = false;
	}
	ImGui::Separator();
	unsigned char minS = 0;
	unsigned char maxS = 2;
	ImGui::SliderScalar(langt("countPed"), ImGuiDataType_U8, &currentMissionPtr->traffic_ped, &minS, &maxS, langMenu["countTraffic"][currentMissionPtr->traffic_ped].c_str());
	ImGui::SliderScalar(langt("countCar"), ImGuiDataType_U8, &currentMissionPtr->traffic_car, &minS, &maxS,langMenu["countTraffic"][currentMissionPtr->traffic_car].c_str());
	ImGui::Separator();
	unsigned char maxW = 6;
	ImGui::SliderScalar(langt("minWanted"), ImGuiDataType_U8, &currentMissionPtr->wanted_min, &minS, &maxW);
	ImGui::SliderScalar(langt("maxWanted"), ImGuiDataType_U8, &currentMissionPtr->wanted_max, &minS, &maxW);
	ImGui::Separator();
	ToggleButton(langt("modeRiot"), &currentMissionPtr->riot);
	ImGui::Separator();
	name.clear();
	name.append(ICON_FA_PARKING);
	name.append(" ");
	name.append(langt("player"));
	if (ImGui::Button(name.c_str())) {
		bPlayer = true;
		bMissionSettings = false;
	}

	ImGui::End();
}

void fStorylineList()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_SCROLL);
	name.append(" ");
	name.append(langt("storylines"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(260);
	ListBox("", currentStoryline, &namesStorylines);

	if (ImGui::Button(langt("add")))
	{
		ImGui::OpenPopup("addT");
	}

	if (!namesStorylines.empty()) {
		if (storylineMode)
		{
			if (ImGui::Button(langt("save"))) {
				Manager::SaveStoryline(currentStoryline);
				CHud::SetHelpMessage("Saved!", false, false, false);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("load"))) {
			name.clear();
			name.append(ICON_FA_EXCLAMATION_TRIANGLE);
			name.append(" ");
			name.append(langt("load"));
			ImGui::OpenPopup(name.c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename"))) {
			strcpy(temp_nameStoryline, namesStorylines[currentStoryline].c_str());
			ImGui::OpenPopup("rename");
		}
		if (namesStorylines.size() > 1 || !storylineMode)
		{
			if (ImGui::Button(langt("delete"))) {
				name.clear();
				name.append(ICON_FA_TRASH_ALT);
				name.append(" ");
				name.append(langt("delete"));
				ImGui::OpenPopup(name.c_str());
			}
		}
	}

	if (ImGui::BeginPopup("addT")) {
		Combo(langt("missionPack"), selMissPack, &namesMissionPacks);
		if (!namesMissionPacks.empty()) {
			if (ImGui::Button(langt("create"))) {
				name.clear();
				name.append(ICON_FA_EXCLAMATION_TRIANGLE);
				name.append(" ");
				name.append(langt("create"));
				ImGui::OpenPopup(name.c_str());
			}
		}

		name.clear();
		name.append(ICON_FA_EXCLAMATION_TRIANGLE);
		name.append(" ");
		name.append(langt("create"));
		if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(langMenu["reloadQues"][1].c_str());

			ImVec2 size_b = ImVec2(160, 0);

			if (ImGui::Button(langt("yes"), size_b))
			{
				currentMissionPtr->removeEditorEntity();
				if (currentMissionPtr != nullptr) {
					delete currentMissionPtr;
					currentMissionPtr = nullptr;
				}

				std::string name = namesMissionPacks[selMissPack];
				std::stringstream path;
				path << "LDYOM//Storylines//" << UTF8_to_CP1251(replace_symb(name)) << ".bin";
				while (boost::filesystem::exists(path.str()))
				{
					path.str("");
					name += "c";
					path << "LDYOM//Storylines//" << UTF8_to_CP1251(replace_symb(name)) << ".bin";
				}
				currentStoryline = namesStorylines.size();
				namesStorylines.push_back(name);
				if (currentStorylinePtr == nullptr) {
					delete currentStorylinePtr;
					currentStorylinePtr = nullptr;
				}
				currentStorylinePtr = new Storyline(namesMissionPacks[selMissPack]);
				Manager::SaveStoryline(currentStoryline);
				storylineMode = true;
				ImGui::CloseCurrentPopup();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button(langt("no"), size_b))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		
		ImGui::EndPopup();
	}

	name.clear();
	name.append(ICON_FA_EXCLAMATION_TRIANGLE);
	name.append(" ");
	name.append(langt("load"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(langMenu["reloadQues"][1].c_str());

		ImVec2 size_b = ImVec2(160, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			if (currentMissionPtr != nullptr) {
				currentMissionPtr->removeEditorEntity();
				delete currentMissionPtr;
				currentMissionPtr = nullptr;
			}
			if (currentStorylinePtr != nullptr) {
				delete currentStorylinePtr;
				currentStorylinePtr = nullptr;
			}
			Manager::LoadStoryline(currentStoryline);

			namesStorylineCheckpoints.clear();
			for (auto checkpoint : currentStorylinePtr->list_checkpoints)
			{
				namesStorylineCheckpoints.push_back(checkpoint->name);
			}
			
			currentStorylinePtr->updateEditorEntity();
			storylineMode = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//delete
	const ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		const ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			std::stringstream path;
			path << "LDYOM//Storylines//" << replace_symb(UTF8_to_CP1251(namesStorylines[currentStoryline])) << ".bin";
			boost::filesystem::remove_all(path.str());
			namesStorylines.erase(namesStorylines.begin() + currentStoryline);
			if (currentStoryline > 0)
				currentStoryline--;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", temp_nameStoryline, IM_ARRAYSIZE(temp_nameStoryline));

		if (ImGui::Button(langt("apply"))) {
			std::stringstream old_path;
			old_path << "LDYOM//Storylines//" << replace_symb(UTF8_to_CP1251(namesStorylines[currentStoryline])) << ".bin";
			std::stringstream new_path;
			new_path << "LDYOM//Storylines//" << replace_symb(UTF8_to_CP1251(string(temp_nameStoryline))) << ".bin";
			boost::filesystem::rename(old_path.str(), new_path.str());
			namesStorylines[currentStoryline] = temp_nameStoryline;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::End();

}

void fTools()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(420, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 210, resolution.y / 2 - 100), ImGuiCond_Always);
	name.clear();
	name.append(ICON_FA_TOOLS);
	name.append(" ");
	name.append(langt("tools"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button(langt("tool_tp_marker"))) {
		float x, y, z;
		Command<0x0AB6>(&x, &y, &z);
		z = CWorld::FindGroundZForCoord(x, y);
		playerPed->SetPosn(x, y, z);
	}
	if (ImGui::Button(langt("jetpack"))) {
		Command<Commands::TASK_JETPACK>(playerPed);
	}
	if (ImGui::Button(langt("enexMarker"))) {
		enexMarker = !enexMarker;
		Command<Commands::DISABLE_ALL_ENTRY_EXITS>((int)enexMarker);
	}
	ImGui::PushItemWidth(170);
	Combo(langt("tool_tp_actor"), tp_actor, &namesActors);
	ImGui::SameLine();
	ImGui::PushID(langt("tool_tp_actor"));
	if (ImGui::Button(langt("teleport"))) {
		Command<Commands::SET_AREA_VISIBLE>(currentMissionPtr->list_actors[tp_actor]->interiorID);
		playerPed->SetPosn(currentMissionPtr->list_actors[tp_actor]->pos[0], currentMissionPtr->list_actors[tp_actor]->pos[1], currentMissionPtr->list_actors[tp_actor]->pos[2]);
	}
	ImGui::PopID();
	Combo(langt("tool_tp_car"), tp_actor, &namesCars);
	ImGui::SameLine();
	ImGui::PushID(langt("tool_tp_car"));
	if (ImGui::Button(langt("teleport"))) {
		playerPed->SetPosn(currentMissionPtr->list_cars[tp_car]->pos[0], currentMissionPtr->list_cars[tp_car]->pos[1], currentMissionPtr->list_cars[tp_car]->pos[2]);
	}
	ImGui::PopID();
	Combo(langt("tool_tp_object"), tp_object, &namesObjects);
	ImGui::SameLine();
	ImGui::PushID(langt("tool_tp_object"));
	if (ImGui::Button(langt("teleport"))) {
		playerPed->SetPosn(currentMissionPtr->list_objects[tp_object]->pos[0], currentMissionPtr->list_objects[tp_object]->pos[1], currentMissionPtr->list_objects[tp_object]->pos[2]);
	}
	ImGui::PopID();

	if (ImGui::Button(langt("antifreez"))) {
		Command<Commands::SET_PLAYER_CONTROL>(0, 1);
		playerPed->SetPosn(playerPed->GetPosition() + CVector(0, 1, 1));
	}
	ImGui::End();
}

void fSettings()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 100, resolution.y / 2 - 200), ImGuiCond_Always);
	name.clear();
	name.append(ICON_FA_CONGS);
	name.append(" ");
	name.append(langt("settings"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PushItemWidth(125);
	if (Combo(langt("style"), curr_theme, &names_themes)) {
		boost::property_tree::ptree pt;
		read_ini("LDYOM/LDYOM_data.ini", pt);
		assert(!pt.empty());
		pt.put<std::string>("Settings.curr_theme", names_themes[curr_theme]);
		boost::property_tree::write_ini("LDYOM/LDYOM_data.ini", pt);
		curr_theme_string = names_themes[curr_theme];
		
		name.clear();
		name.append(ICON_FA_EXCLAMATION_TRIANGLE);
		name.append(" ");
		name.append(langt("reload"));
		load_theme = false;
		//ImGui::OpenPopup(name.c_str());
	}

	if (Combo(langt("lang"), curr_lang, &names_langs)) {
		boost::property_tree::ptree pt;
		read_ini("LDYOM/LDYOM_data.ini", pt);
		assert(!pt.empty());
		pt.put<std::string>("Settings.curr_lang", names_langs[curr_lang]);
		boost::property_tree::write_ini("LDYOM/LDYOM_data.ini", pt);
		curr_lang_string = names_langs[curr_lang];
		lang_file.clear();
		loadArrayMenu();
	}

	/*name.clear();
	name.append(ICON_FA_EXCLAMATION_TRIANGLE);
	name.append(" ");
	name.append(langt("reload"));
				if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

					ImGui::Text(langMenu["reloadQues"][1].c_str());
					ImGui::Text(langMenu["reloadQues"][2].c_str());
					ImGui::Text(langMenu["reloadQues"][3].c_str());

					ImVec2 size_b = ImVec2(160, 0);

					if (ImGui::Button(langt("yes"], size_b)) {
						for i = 1, #vr.list_actors do
							deleteChar(vr.list_actors[i]["data"]["char"])
							end
							for c = 1, #vr.list_cars do
								deleteCar(vr.list_cars[c]["data"]["car"])
								end
								for o = 1, #vr.list_objects do
									deleteObject(vr.list_objects[o]["data"]["obj"])
									end
									for p = 1, #vr.list_pickups do
										removePickup(vr.list_pickups[p]["data"]["pick"])
										end
										for p = 1, #vr.list_particles do
											killFxSystem(vr.list_particles[p]["data"]["prtcl"][1])
											deleteObject(vr.list_particles[p]["data"]["prtcl"][2])
											end
											for p = 1, #vr.list_explosions do
												if vr.list_explosions[p]["data"]["fire"] then
													removeScriptFire(vr.list_explosions[p]["data"]["fire"])
													end
													if vr.list_explosions[p]["data"]["explosion"] then
														deleteObject(vr.list_explosions[p]["data"]["explosion"])
														end
														end
														for a = 1, #vr.list_audios do
															if vr.list_audios[a]["data"]["obj"] then
																deleteObject(vr.list_audios[a]["data"]["obj"])
																end
																end
																deleteChar(vr.player_ped)
																thisScript() :reload()
																end
																ImGui::SameLine()
																if (ImGui::Button(langt("no"], size_b) then
																	ImGui::CloseCurrentPopup()
																	end

																	ImGui::EndPopup()
																	}*/

	ImGui::End();
}

void fInfo()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(210, 400), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 105, resolution.y / 2 - 200), ImGuiCond_Always);
	name.clear();
	name.append(ICON_FA_INFO);
	name.append(" ");
	name.append(langt("info"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	name.clear();
	name.append(langMenu["info_t"][2]);
	name.append(" ");
	name.append(VERSION_STR);
	ImGui::Text(name.c_str());
	ImGui::Spacing();
	
	name.clear();
	name.append(langMenu["info_t"][0]);
	name.append(" ");
	name.append(authors[0]);
	ImGui::Text(name.c_str());
	name.clear();
	name.append(langMenu["info_t"][1]);
	name.append(" ");
	name.append(authors[1]);
	ImGui::Text(name.c_str());
	name.clear();
	name.append(langMenu["info_t"][5]);
	name.append(" ");
	name.append(langt("nameLoc"));
	ImGui::Text(name.c_str());
	ImGui::Spacing();
	ImGui::Text(langMenu["info_t"][6].c_str());
	std::string namesHelpers = "Jasmijn Wellner (gvx), Um Geek, Alexey Generalov, Ivan Kogotko";
	ImGui::TextWrapped(namesHelpers.c_str());
	ImGui::Text("");
	name.clear();
	name.append(ICON_FA_BOOK);
	name.append(" ");
	name.append(langMenu["info_t"][7].c_str());
	if (ImGui::Button(name.c_str())) {
		//os.execute('start https://mgeymer.github.io/LDYOM_DOC/')
		system("explorer https://mgeymer.github.io/LDYOM_DOC/");
	}
	
	ImGui::End();
}

void fStorylineMainMenu()
{
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 100, resolution.y / 2 - 200), ImGuiCond_Appearing);
	ImGui::Begin(langt("menuStoryline"), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

	const ImVec2 size_b(250, 0);
	std::string name;

	name.clear();
	name.append(ICON_FA_FLAG);
	name.append(" ");
	name.append(langt("initEndMiss"));
	if (ImGui::TreeNode(name.c_str()))
	{
		int pack_idx = 0;
		for (int i = 0; i < namesMissionPacks.size(); ++i)
		{
			if (namesMissionPacks[i] == currentStorylinePtr->missPack)
			{
				pack_idx = i;
				break;
			}
		}
		vector<std::string> list{ langt("nothing") };
		for (auto mission : namesMissions[pack_idx])
		{
			list.push_back(mission);
		}
		ImGui::Text(langt("startMiss"));
		ImGui::PushID(langt("startMiss"));
		Combo("", currentStorylinePtr->startMission, &list);
		ImGui::PopID();
		ImGui::Text(langt("endMiss"));
		ImGui::PushID(langt("endMiss"));
		Combo("", currentStorylinePtr->endMission, &list);
		ImGui::PopID();
		ImGui::TreePop();
	}
	ImGui::Separator();

	name.clear();
	name.append(ICON_FA_MAP_MARKER_ALT);
	name.append(" ");
	name.append(langt("checkpoints"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bStorylineMainMenu = false;
		bStorylineCheckpoints = true;
	}
	name.clear();
	name.append(ICON_FA_PROJECT_DIAGRAM);
	name.append(" ");
	name.append(langt("nodeEditor"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bStorylineMainMenu = false;
		bNodeEditor = true;
	}
	name.clear();
	name.append(ICON_FA_PLAY);
	name.append(" ");
	name.append(langt("storylineStart"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bStorylineMainMenu = false;
		instance.add_to_queue(MissionPlayer::start_storyline);
	}
	name.clear();
	name.append(ICON_FA_SCROLL);
	name.append(" ");
	name.append(langt("storylines"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		bStorylineMainMenu = false;
		bStorylines = true;
	}
	name.clear();
	name.append(ICON_FA_DOOR_OPEN);
	name.append(" ");
	name.append(langt("exitStoryline"));
	if (ImGui::Button(name.c_str(), size_b))
	{
		name.clear();
		name.append(ICON_FA_EXCLAMATION_TRIANGLE);
		name.append(" ");
		name.append(langt("exitStoryline"));
		ImGui::OpenPopup(name.c_str());
	}

	name.clear();
	name.append(ICON_FA_EXCLAMATION_TRIANGLE);
	name.append(" ");
	name.append(langt("exitStoryline"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(langMenu["reloadQues"][1].c_str());

		ImVec2 size_b = ImVec2(160, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentStorylinePtr->removeEditorEntity();
			if (currentStorylinePtr != nullptr) {
				delete currentStorylinePtr;
				currentStorylinePtr = nullptr;
			}
			currentMissionPtr = new Mission;
			currentMissionPtr->player.updateEditorPed();
			bStorylineMainMenu = false;
			storylineMode = false;
			fastdata_pack = -1;

			currentNodeGraphPtr->nodes.clear();
			currentNodeGraphPtr->links.clear();
			currentNodeGraphPtr->vars.clear();
			namesVars.clear();
			
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	
	
	ImGui::End();
}

void fStorylineCheckpoints()
{
	bool isWindow = false;
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(270, 410), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x - 270, 0), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_MAP_MARKER_ALT);
	name.append(" ");
	name.append(langt("checkpoints"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	//List
	ImGui::SetNextItemWidth(255.0f);
	ListBox("", currentStoryline, &namesStorylineCheckpoints);

	if (ImGui::Button(langt("add")))
	{
		CVector playerPos = playerPed->GetPosition();

		std::string name(langt("checkpoint"));
		name.append(" #");
		name.append(std::to_string(currentStorylinePtr->list_checkpoints.size()));
		currentStorylinePtr->list_checkpoints.push_back(new CheckpointStoryline(name.c_str(), playerPos.x, playerPos.y, playerPos.z));
		currentStorylineCheckpoint = currentStorylinePtr->list_checkpoints.size() - 1;
		namesStorylineCheckpoints.push_back(currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint]->name);
		currentStorylinePtr->list_checkpoints[currentStorylinePtr->list_checkpoints.size() - 1]->updateEditorCheckpoint();
	}
	if (!currentStorylinePtr->list_checkpoints.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button(langt("duplicate")))
		{
			int new_audio = currentStorylinePtr->list_checkpoints.size();
			CheckpointStoryline* checkpointStoryline = new CheckpointStoryline(*currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint]);
			checkpointStoryline->updateEditorCheckpoint();
			currentStorylinePtr->list_checkpoints.push_back(checkpointStoryline);
			string str;
			str.append(currentStorylinePtr->list_checkpoints[new_audio]->name);
			str.append("c");
			strcpy(checkpointStoryline->name, str.c_str());

			namesStorylineCheckpoints.push_back(checkpointStoryline->name);
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("rename")))
		{
			ImGui::OpenPopup("rename");
		}
		if (ImGui::Button(langt("delete")))
		{
			name.clear();
			name.append(ICON_FA_TRASH_ALT);
			name.append(" ");
			name.append(langt("delete"));
			ImGui::OpenPopup(name.c_str());
		}
	}

	isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	//delete
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	name.clear();
	name.append(ICON_FA_TRASH_ALT);
	name.append(" ");
	name.append(langt("delete"));
	if (ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(langt("deleteQues"));

		ImVec2 size_b = ImVec2(100, 0);

		if (ImGui::Button(langt("yes"), size_b))
		{
			currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint]->removeEditorCheckpoint();
			delete currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint];
			currentStorylinePtr->list_checkpoints.erase(currentStorylinePtr->list_checkpoints.begin() + currentStorylineCheckpoint);
			namesStorylineCheckpoints.erase(namesStorylineCheckpoints.begin() + currentStorylineCheckpoint);
			if (currentStorylineCheckpoint > 0)
				currentStorylineCheckpoint--;

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langt("no"), size_b))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//Rename popup
	if (ImGui::BeginPopup("rename"))
	{
		ImGui::InputText("", &currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint]->name[0],
			IM_ARRAYSIZE(currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint]->name));

		if (ImGui::Button(langt("close")))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	//checkpoint render
	if (!currentStorylinePtr->list_checkpoints.empty())
	{
		CheckpointStoryline* checkPtr = currentStorylinePtr->list_checkpoints[currentStorylineCheckpoint];
		ImGui::SetNextWindowSize(ImVec2(400, 360), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(resolution.x - 670, 0), ImGuiCond_Appearing);
		name.clear();
		name.append(ICON_FA_MALE);
		name.append(" ");
		name.append(langt("audio"));
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		Command<Commands::FREEZE_CHAR_POSITION>(playerPed, true);

		if (ImGui::Button(ICON_FA_STREET_VIEW))
		{
			CVector playerPos = playerPed->GetPosition();
			checkPtr->pos[0] = playerPos.x;
			checkPtr->pos[1] = playerPos.y;
			checkPtr->pos[2] = playerPos.z;
			checkPtr->updateEditorCheckpoint();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(langt("playerCoordinates"));
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(270);
		ImGui::InputFloat3(langt("position"), checkPtr->pos, "%.6f");
		ImGui::PushItemWidth(-130);
		auto& colorsL = langMenu["targets_marker_color"];
		vector<std::string> cut_colors(colorsL.begin(), colorsL.end() - 1);
		if (Combo(langt("colorMarker"), checkPtr->colorBlip, &cut_colors))
		{
			if (checkPtr->colorBlip > 0)
			{
				Command<Commands::CHANGE_BLIP_DISPLAY>(checkPtr->marker, 3);
				Command<Commands::CHANGE_BLIP_COLOUR>(checkPtr->marker, checkPtr->colorBlip - 1);
			} else
				Command<Commands::CHANGE_BLIP_DISPLAY>(checkPtr->marker, 1);
		}

		if (ImGui::ImageButton(blipsAtlas, ImVec2(32, 32), ImVec2(checkPtr->iconMarker * 0.015625f, 0),ImVec2((checkPtr->iconMarker + 1) * 0.015625f, 1)))
		{
			ImGui::OpenPopup("blips");
		}

		//blip popup
		if (ImGui::BeginPopup("blips"))
		{
			ImGui::BeginChild("blipsc", ImVec2(180, 450));


			for (int i = 0; i < 64; i++)
			{
				ImGui::PushID(i);
				if (ImGui::ImageButton(blipsAtlas, ImVec2(32, 32), ImVec2(i * 0.015625f, 0),ImVec2((i + 1) * 0.015625f, 1)))
				{
					checkPtr->iconMarker = i;
					checkPtr->updateEditorCheckpoint();
				}
				ImGui::PopID();
				if ((i + 1) % 4 != 0)
				{
					ImGui::SameLine();
				}
			}

			ImGui::EndChild();
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetItemRectSize().y / 2 - 10);
		ImGui::Text(langt("icon"));
		Combo(langt("startTime"), checkPtr->timeStart, &langMenu["timeForStart"]);
		ToggleButton(langt("startEndCheck"), &checkPtr->useMission);
		int pack_idx = 0;
		for (int i = 0; i < namesMissionPacks.size(); ++i)
		{
			if (namesMissionPacks[i] == currentStorylinePtr->missPack)
			{
				pack_idx = i;
				break;
			}
		}
		if (checkPtr->useMission)
		{
			ImGui::Text(langt("app_after"));
			ImGui::PushID(langt("app_after"));
			Combo("", checkPtr->startC, &namesMissions[pack_idx]);
			ImGui::PopID();
		}
		ImGui::Text(langt("startCheckMiss"));
		ImGui::PushID(langt("startCheckMiss"));
		Combo("", checkPtr->gotoMission, &namesMissions[pack_idx]);
		ImGui::PopID();

		ImGui::End();
		
		ImGui::SetNextWindowBgAlpha(0.50);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, 60), ImGuiCond_Always);
		ImGui::Begin("info", nullptr,
			ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_AlwaysAutoResize +
			ImGuiWindowFlags_NoSavedSettings + ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoInputs);

		ImGui::Text(langMenu["infoOverlay"][0].c_str());
		ImGui::Text(langMenu["infoOverlay"][1].c_str());
		ImGui::Text(langMenu["infoOverlay"][2].c_str());

		ImGui::End();

		isWindow |= ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

		//edit
		float cx = checkPtr->pos[0], cy = checkPtr->pos[1], cz = checkPtr->pos[2];
		cx = cx + (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy = cy + (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz = cz + (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(checkPtr->pos[0], checkPtr->pos[1], checkPtr->pos[2], 2);

		if (!isWindow)
		{
			if (ImGui::IsMouseDragging(2))
			{
				ImVec2 dt = ImGui::GetIO().MouseDelta;
				camera_angle[0] += dt.x;
				camera_angle[1] += dt.y;
			}
			if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT))
			{
				camera_zoom += (camera_zoom * ImGui::GetIO().MouseWheel) / 4;
				if (camera_zoom < 1)
					camera_zoom = 1;
			}
		}
		if (KeyPressed(VK_UP))
		{
			checkPtr->pos[0] -= 0.01f * camera_zoom * static_cast<float>(sin(
				static_cast<float>(rad(camera_angle[0]))));
			checkPtr->pos[1] -= 0.01f * camera_zoom * static_cast<float>(cos(
				static_cast<float>(rad(camera_angle[0]))));
			checkPtr->updateEditorCheckpoint();
		}
		else if (KeyPressed(VK_DOWN))
		{
			checkPtr->pos[0] += 0.01f * camera_zoom * static_cast<float>(sin(
				static_cast<float>(rad(camera_angle[0]))));
			checkPtr->pos[1] += 0.01f * camera_zoom * static_cast<float>(cos(
				static_cast<float>(rad(camera_angle[0]))));
			checkPtr->updateEditorCheckpoint();
		}
		if (KeyPressed(VK_LEFT))
		{
			checkPtr->pos[0] += 0.01f * camera_zoom * static_cast<float>(sin(
				static_cast<float>(rad(camera_angle[0] + 90))));
			checkPtr->pos[1] += 0.01f * camera_zoom * static_cast<float>(cos(
				static_cast<float>(rad(camera_angle[0] + 90))));
			checkPtr->updateEditorCheckpoint();
		}
		else if (KeyPressed(VK_RIGHT))
		{
			checkPtr->pos[0] += 0.01f * camera_zoom * static_cast<float>(sin(
				static_cast<float>(rad(camera_angle[0] - 90))));
			checkPtr->pos[1] += 0.01f * camera_zoom * static_cast<float>(cos(
				static_cast<float>(rad(camera_angle[0] - 90))));
			checkPtr->updateEditorCheckpoint();
		}
	}
}

extern void clearScripts();

void fScriptsSettings()
{
	std::string name;
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(resolution.x / 2 - 100, resolution.y / 2 - 200), ImGuiCond_Appearing);
	name.clear();
	name.append(ICON_FA_FILE_CODE);
	name.append(" ");
	name.append(langt("scriptsSettings"));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);

	static bool update_scripts = false;
	
	if (ImGui::BeginChild("list_scripts", ImVec2(0,200),NULL,ImGuiWindowFlags_NoBackground)) {
		for (unsigned int i = 0; i < ScriptManager::lua_scripts.size(); i++) {
			ImGui::PushID(i);
			sol::optional<std::string> name = ScriptManager::lua_scripts.at(i).second["info"]["name"];
			if (name.has_value()) {
				if (ToggleButton(name.value().c_str(), &ScriptManager::lua_scripts.at(i).first))
				{
					mINI::INIStructure config;
					mINI::INIFile file("LDYOM//Scripts//config.ini");
					file.read(config);
					config["Scripts"][name.value()] = ScriptManager::lua_scripts.at(i).first? "true" : "false";
					file.write(config);
					update_scripts = true;
				}
			}
			ImGui::PopID();
		}
		ImGui::EndChild();
	}

	if (ImGui::Button(langt("apply")))
	{
		if (update_scripts)
		{
			clearScripts();
			ScriptManager::loadScripts();
			bScriptsSettings = false;
		}
	}
	
	ImGui::End();
}