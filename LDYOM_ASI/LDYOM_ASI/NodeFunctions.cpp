#include "NodeFunctions.h"



#include <CCamera.h>
#include <CHud.h>
#include <CMessages.h>
#include <CText.h>
#include "libs/ScriptCommands.h"

#include <CUserDisplay.h>
#include <map>

#include "imgui.h"
#include "Init.h"
#include "NodeGraph.h"
#include "ScriptManager.h"
#include "libs/coro_wait.h"

extern void printLog(std::string print_text);
extern bool Combo(const char* label, int& current_item, std::vector<std::string>* items);
extern bool Combo(const char* label, int& current_item, std::vector<const char*>* items);
extern bool ListBox(const char* label, int& current_item, std::vector<const char*>* items);
extern bool ListBox(const char* label, int& current_item, std::vector<std::string>* items);
extern const char* langt(const std::string& key);
extern std::string curr_lang_string;
extern std::vector<std::string> getStorylineMissionsNames();

extern std::vector<const char*> namesStorylineCheckpoints;
extern std::vector<const char*> namesTargets;
extern std::vector<const char*> namesCars;
extern std::vector<const char*> namesTrains;
extern std::vector<const char*> namesActors;
extern std::vector<const char*> namesObjects;
extern std::vector<const char*> namesParticles;
extern std::vector<const char*> namesPickups;
extern std::vector<const char*> namesExplosions;
extern std::vector<const char*> namesAudios;
extern std::vector<const char*> namesStorylineCheckpoints;
extern void addLDYOMClasses(sol::state& lua);
extern void setNodeSignal(bool value);
extern bool getMissionStarted();
extern int current_mission_target;
extern bool ToggleButton(const char* str_id, bool* v);
extern void stopStoryline();
extern void start_storyline_mission(std::string& mission_name);
extern int last_mission;
extern map<unsigned, sol::object> realVariable;
extern map<std::string, std::vector<std::string>> langMenu;
extern CPed *playerPed;
extern bool off_gui;
extern bool KeyJustPressed(unsigned int key);
extern std::string UTF8_to_CP1251(std::string const& utf8);
extern Mission* currentMissionPtr;
extern inline double rad(float degrees);
extern bool bNodeEditor;
extern inline double deg(float radians);
extern coro_wait instance;
extern void setLastNode(int node_id);
extern CPed *playerPed;

void SkinSelector(sol::object value)
{
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
				*(int*)value.pointer() = ID_Actors[i];
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
}

#define VK_I 0x49
#define VK_U 0x55
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Q 0x51
#define VK_E 0x45
#define VK_R 0x52
#define VK_F 0x46
#define VK_O 0x4F
#define VK_K 0x4B
#define VK_L 0x4C

void moveObject(int idObject, sol::object posObj, sol::object angleObj)
{
	float* pos = (float*)posObj.pointer();
	float* rotation = (float*)angleObj.pointer();
	CObject* editorObject = currentMissionPtr->list_objects[idObject]->editorObject;

	Command<Commands::SET_OBJECT_ROTATION>(editorObject,
		static_cast<float>(rotation[0]),
		static_cast<float>(rotation[1]),
		static_cast<float>(rotation[2]));
	Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
		pos[2]);

	while (true)
	{
		this_coro::wait(0);
		float cx = pos[0], cy = pos[1], cz = pos[2];
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(pos[0], pos[1], pos[2], 2);
		float left_right;
		float up_down;
		Command<Commands::GET_PC_MOUSE_MOVEMENT>(&left_right, &up_down);
		camera_angle[0] += left_right / 10;
		camera_angle[1] += up_down / 10;
		if (KeyPressed(VK_SHIFT))
		{
			if (KeyPressed(VK_UP))
			{
				rotation[0]++;
				Command<Commands::SET_OBJECT_ROTATION>(editorObject,
					static_cast<float>(rotation[0]),
					static_cast<float>(rotation[1]),
					static_cast<float>(rotation[2]));
			}
			else if (KeyPressed(VK_DOWN))
			{
				rotation[0]--;
				Command<Commands::SET_OBJECT_ROTATION>(editorObject,
					static_cast<float>(rotation[0]),
					static_cast<float>(rotation[1]),
					static_cast<float>(rotation[2]));
			}
			if (KeyPressed(VK_LEFT))
			{
				rotation[1]++;
				Command<Commands::SET_OBJECT_ROTATION>(editorObject,
					static_cast<float>(rotation[0]),
					static_cast<float>(rotation[1]),
					static_cast<float>(rotation[2]));
			}
			else if (KeyPressed(VK_RIGHT))
			{
				rotation[1]--;
				Command<Commands::SET_OBJECT_ROTATION>(editorObject,
					static_cast<float>(rotation[0]),
					static_cast<float>(rotation[1]),
					static_cast<float>(rotation[2]));
			}
			if (KeyPressed(VK_Q))
			{
				rotation[2]++;
				Command<Commands::SET_OBJECT_ROTATION>(editorObject,
					static_cast<float>(rotation[0]),
					static_cast<float>(rotation[1]),
					static_cast<float>(rotation[2]));
			}
			else if (KeyPressed(VK_E))
			{
				rotation[2]--;
				Command<Commands::SET_OBJECT_ROTATION>(editorObject,
					static_cast<float>(rotation[0]),
					static_cast<float>(rotation[1]),
					static_cast<float>(rotation[2]));
			}
		}
		else
		{
			if (KeyPressed(VK_UP))
			{
				pos[0] -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
				pos[1] -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
				Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
					pos[2]);
			}
			else if (KeyPressed(VK_DOWN))
			{
				pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
				pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
				Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
					pos[2]);
			}
			if (KeyPressed(VK_LEFT))
			{
				pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
				pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
				Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
					pos[2]);
			}
			else if (KeyPressed(VK_RIGHT))
			{
				pos[0] += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
				pos[1] += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
				Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
					pos[2]);
			}
			if (KeyPressed(VK_Q))
			{
				pos[2] += 0.01 * camera_zoom;
				Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
					pos[2]);
			}
			else if (KeyPressed(VK_E))
			{
				pos[2] -= 0.01 * camera_zoom;
				Command<Commands::SET_OBJECT_COORDINATES>(editorObject, pos[0], pos[1],
					pos[2]);
			}
		}

		if (KeyPressed(VK_F))
		{
			Command<Commands::SET_OBJECT_ROTATION>(editorObject,
				static_cast<float>(currentMissionPtr->list_objects[idObject]->rotation[0]),
				static_cast<float>(currentMissionPtr->list_objects[idObject]->rotation[1]),
				static_cast<float>(currentMissionPtr->list_objects[idObject]->rotation[2]));
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject,
				currentMissionPtr->list_objects[idObject]->pos[0],
				currentMissionPtr->list_objects[idObject]->pos[1],
				currentMissionPtr->list_objects[idObject]->pos[2]);

			off_gui = false;
			break;
		}
	}

}

void slideObject(int idObject, sol::object x_obj, sol::object y_obj, sol::object z_obj)
{
	float& x = *(float*)x_obj.pointer();
	float& y = *(float*)y_obj.pointer();
	float& z = *(float*)z_obj.pointer();
	CObject* editorObject = currentMissionPtr->list_objects[idObject]->editorObject;

	Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
		z);

	while (true)
	{
		this_coro::wait(0);
		float cx = x, cy = y, cz = z;
		cx += (camera_zoom * sin(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cy += (camera_zoom * cos(static_cast<float>(rad(camera_angle[0]))) * cos(
			static_cast<float>(rad(camera_angle[1]))));
		cz += (camera_zoom * sin(static_cast<float>(rad(camera_angle[1]))));
		Command<Commands::SET_FIXED_CAMERA_POSITION>(cx, cy, cz, 0.0f, 0.0f, 0.0f);
		Command<Commands::POINT_CAMERA_AT_POINT>(x, y, z, 2);
		float left_right;
		float up_down;
		Command<Commands::GET_PC_MOUSE_MOVEMENT>(&left_right, &up_down);
		camera_angle[0] += left_right / 10;
		camera_angle[1] += up_down / 10;


		if (KeyPressed(VK_UP))
		{
			x -= 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
			y -= 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
				z);
		}
		else if (KeyPressed(VK_DOWN))
		{
			x += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0])));
			y += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0])));
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
				z);
		}
		if (KeyPressed(VK_LEFT))
		{
			x += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] + 90)));
			y += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] + 90)));
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
				z);
		}
		else if (KeyPressed(VK_RIGHT))
		{
			x += 0.01 * camera_zoom * sin(static_cast<float>(rad(camera_angle[0] - 90)));
			y += 0.01 * camera_zoom * cos(static_cast<float>(rad(camera_angle[0] - 90)));
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
				z);
		}
		if (KeyPressed(VK_Q))
		{
			z += 0.01 * camera_zoom;
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
				z);
		}
		else if (KeyPressed(VK_E))
		{
			z -= 0.01 * camera_zoom;
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject, x, y,
				z);
		}

		if (KeyPressed(VK_F))
		{
			Command<Commands::SET_OBJECT_COORDINATES>(editorObject,
				currentMissionPtr->list_objects[idObject]->pos[0],
				currentMissionPtr->list_objects[idObject]->pos[1],
				currentMissionPtr->list_objects[idObject]->pos[2]);

			off_gui = false;
			break;
		}
	}

}

void staticCutscene(sol::object x_obj, sol::object y_obj, sol::object z_obj, sol::object rot_x_obj, sol::object rot_y_obj, sol::object rot_z_obj)
{

	float& x = *(float*)x_obj.pointer();
	float& y = *(float*)y_obj.pointer();
	float& z = *(float*)z_obj.pointer();

	int& rot_x = *(int*)rot_x_obj.pointer();
	int& rot_y = *(int*)rot_y_obj.pointer();
	int& rot_z = *(int*)rot_z_obj.pointer();

	CHud::bScriptDontDisplayRadar = true;
	CHud::m_Wants_To_Draw_Hud = false;

	while (true)
	{
		this_coro::wait(0ms);
		float multy = 1.0f;
		if (KeyPressed(VK_CONTROL))
			multy = 0.5f;

		if (KeyPressed(VK_A) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y + 90)));
			float coss = static_cast<float>(cos(rad(rot_y + 90)));
			x = x - 0.2 * sinn * multy;
			y = y - 0.2 * coss * multy;
		}
		if (KeyPressed(VK_D) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y - 90)));
			float coss = static_cast<float>(cos(rad(rot_y - 90)));
			x = x - 0.2 * sinn * multy;
			y = y - 0.2 * coss * multy;
		}
		if (KeyPressed(VK_W) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y)));
			float coss = static_cast<float>(cos(rad(rot_y)));
			x = x + 0.2 * sinn * multy;
			y = y + 0.2 * coss * multy;
		}
		if (KeyPressed(VK_S) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y)));
			float coss = static_cast<float>(cos(rad(rot_y)));
			x = x - 0.2 * sinn * multy;
			y = y - 0.2 * coss * multy;
		}
		if (KeyPressed(VK_Q) && !KeyPressed(VK_SHIFT))
		{
			z = z + 0.2 * multy;
		}
		if (KeyPressed(VK_E) && !KeyPressed(VK_SHIFT))
		{
			z = z - 0.2 * multy;
		}

		if (KeyPressed(VK_SHIFT))
		{
			if (KeyPressed(VK_A))
			{
				rot_y = rot_y - 2 * multy;
			}
			if (KeyPressed(VK_D))
			{
				rot_y = rot_y + 2 * multy;
			}
			if (KeyPressed(VK_W))
			{
				rot_x = rot_x - 2 * multy;
			}
			if (KeyPressed(VK_S))
			{
				rot_x = rot_x + 2 * multy;
			}
			if (KeyPressed(VK_E))
			{
				rot_z = rot_z - 2 * multy;
			}
			if (KeyPressed(VK_Q))
			{
				rot_z = rot_z + 2 * multy;
			}
		}

		float xx = x, xy = y, xz = z;
		float rxx = rot_x, rxy = rot_y, rxz = rot_z;
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

			rot_x = rot_x % 360;
			rot_y = rot_y % 360;
			rot_z = rot_z % 360;

			off_gui = false;
			CHud::bScriptDontDisplayRadar = false;
			CHud::m_Wants_To_Draw_Hud = true;
			break;
		}
	}
}

// 0 - attach_camera_to_vehicle
// 1 - attach_camera_to_char
// 2 - attach_camera_to_vehicle_look_at_vehicle
// 3 - attach_camera_to_vehicle_look_at_char
// 4 - attach_camera_to_char_look_at_char
void attachCameraToEntity(int opcode, int entity, int entity2, sol::object x_obj, sol::object y_obj, sol::object z_obj, sol::object rot_x_obj, sol::object rot_y_obj, sol::object rot_z_obj)
{

	float rot_x = 0, rot_y = 0, rot_z = 0;


	float& x = *(float*)x_obj.pointer();
	float& y = *(float*)y_obj.pointer();
	float& z = *(float*)z_obj.pointer();

	if (opcode <= 1) {
		rot_x = *(float*)rot_x_obj.pointer();
		rot_y = *(float*)rot_y_obj.pointer();
		rot_z = *(float*)rot_z_obj.pointer();
	}

	CHud::bScriptDontDisplayRadar = true;
	CHud::m_Wants_To_Draw_Hud = false;

	while (true)
	{
		this_coro::wait(0ms);
		float multy = 1.0f;
		if (KeyPressed(VK_CONTROL))
			multy = 0.5f;

		if (KeyPressed(VK_A) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y + 90)));
			float coss = static_cast<float>(cos(rad(rot_y + 90)));
			x = x - 0.2 * sinn * multy;
			y = y - 0.2 * coss * multy;
		}
		if (KeyPressed(VK_D) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y - 90)));
			float coss = static_cast<float>(cos(rad(rot_y - 90)));
			x = x - 0.2 * sinn * multy;
			y = y - 0.2 * coss * multy;
		}
		if (KeyPressed(VK_W) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y)));
			float coss = static_cast<float>(cos(rad(rot_y)));
			x = x + 0.2 * sinn * multy;
			y = y + 0.2 * coss * multy;
		}
		if (KeyPressed(VK_S) && !KeyPressed(VK_SHIFT))
		{
			float sinn = static_cast<float>(sin(rad(rot_y)));
			float coss = static_cast<float>(cos(rad(rot_y)));
			x = x - 0.2 * sinn * multy;
			y = y - 0.2 * coss * multy;
		}
		if (KeyPressed(VK_Q) && !KeyPressed(VK_SHIFT))
		{
			z = z + 0.2 * multy;
		}
		if (KeyPressed(VK_E) && !KeyPressed(VK_SHIFT))
		{
			z = z - 0.2 * multy;
		}

		if (KeyPressed(VK_SHIFT))
		{
			if (KeyPressed(VK_A))
			{
				rot_y = rot_y - 2 * multy;
			}
			if (KeyPressed(VK_D))
			{
				rot_y = rot_y + 2 * multy;
			}
			if (KeyPressed(VK_W))
			{
				rot_x = rot_x - 2 * multy;
			}
			if (KeyPressed(VK_S))
			{
				rot_x = rot_x + 2 * multy;
			}
			if (KeyPressed(VK_E))
			{
				rot_z = rot_z - 2 * multy;
			}
			if (KeyPressed(VK_Q))
			{
				rot_z = rot_z + 2 * multy;
			}
		}

		CVehicle *veh1, *veh2;
		CPed *ped1, *ped2;

		switch (opcode)
		{
		case 0:
			veh1 = currentMissionPtr->list_cars.at(entity)->editorCar;
			Command<Commands::ATTACH_CAMERA_TO_VEHICLE>(veh1, x, y, z, rot_x, rot_y, rot_z, 0.0f, 2);
			break;
		case 1:
			ped1 = currentMissionPtr->list_actors.at(entity)->editorPed;
			Command<Commands::ATTACH_CAMERA_TO_CHAR>(ped1, x, y, z, rot_x, rot_y, rot_z, 0.0f, 2);
			break;
		case 2:
			veh1 = currentMissionPtr->list_cars.at(entity)->editorCar;
			veh2 = currentMissionPtr->list_cars.at(entity2)->editorCar;
			Command<Commands::ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_VEHICLE>(veh1, x, y, z, veh2, 0.0f, 2);
			break;
		case 3:
			veh1 = currentMissionPtr->list_cars.at(entity)->editorCar;
			ped1 = currentMissionPtr->list_actors.at(entity2)->editorPed;
			Command<Commands::ATTACH_CAMERA_TO_VEHICLE_LOOK_AT_CHAR>(veh1, x, y, z, ped1, 0.0f, 2);
			break;
		case 4:
			ped1 = currentMissionPtr->list_actors.at(entity)->editorPed;
			ped2 = currentMissionPtr->list_actors.at(entity2)->editorPed;
			Command<Commands::ATTACH_CAMERA_TO_CHAR_LOOK_AT_CHAR>(ped1, x, y, z, ped2, 0.0f, 2);
			break;
		}

		//Close editor
		if (KeyPressed(VK_F))
		{
			CHud::m_bHelpMessagePermanent = false;
			TheCamera.Restore();
			editmodeCamera = false;

			rot_x = (int)rot_x % 360;
			rot_y = (int)rot_y % 360;
			rot_z = (int)rot_z % 360;

			if (opcode <= 1) {
				*(float*)rot_x_obj.pointer() = rot_x;
				*(float*)rot_y_obj.pointer() = rot_y;
				*(float*)rot_z_obj.pointer() = rot_z;
			}

			off_gui = false;
			CHud::bScriptDontDisplayRadar = false;
			CHud::m_Wants_To_Draw_Hud = true;
			break;
		}
	}
}

void walkPointsNode(int pedHandle, int type_move_ped, int type_route_ped, sol::table &path, sol::table &node, int id_out, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([pedHandle, type_move_ped, path, type_route_ped, data, mission, node, id_out]() {
		CPed* ped = CPools::GetPed(pedHandle);

		int type_walk = 4;
		if (type_move_ped == 2)
			type_walk = 6;
		else if (type_move_ped == 3)
			type_walk = 7;

		Command<Commands::TASK_TOGGLE_DUCK>(pedHandle, (int)(type_move_ped == 0));

		while (getMissionStarted())
		{
			for (int i = 1; i <= path.size(); ++i)
			{
				if (!Command<Commands::DOES_CHAR_EXIST>(pedHandle))
				{
					return;
				}
				float* point = (float*)((sol::object)path[i]).pointer();
				Command<Commands::TASK_GO_STRAIGHT_TO_COORD>(pedHandle, point[0], point[1], point[2], type_walk, -1);
				float dist = 99999;
				while (dist > 1.0f)
				{
					if (!Command<Commands::DOES_CHAR_EXIST>(pedHandle))
					{
						return;
					}
					dist = DistanceBetweenPoints(ped->GetPosition(), CVector(point[0], point[1], point[2]));
					this_coro::wait(0);
				}
			}

			if (type_route_ped == 0)
				break;

			this_coro::wait(0);
		}

		Command<Commands::TASK_TOGGLE_DUCK>(pedHandle, 0);

		const sol::protected_function play = node["callOutputLinks"];
		auto result = play(node, data, mission, id_out);
		if (!ScriptManager::checkProtected(result))
			CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);

	});
}


void driveCarPointsNode(int pedHandle, int carHandle, float speed, int type_route_ped, bool agressive, sol::table &path, sol::table &node, int id_out, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([pedHandle, carHandle, speed, agressive, path, type_route_ped, data, mission, node, id_out]() {
		CVehicle* car = CPools::GetVehicle(carHandle);

		Command<Commands::SET_CAR_STRONG>(carHandle, 1);

		while (getMissionStarted())
		{
			for (int i = 1; i <= path.size(); ++i)
			{
				if (!Command<Commands::DOES_CHAR_EXIST>(pedHandle))
				{
					return;
				}
				if (!Command<Commands::DOES_VEHICLE_EXIST>(carHandle))
				{
					return;
				}
				float* point = (float*)((sol::object)path[i]).pointer();
				Command<Commands::TASK_CAR_DRIVE_TO_COORD>(pedHandle, carHandle, point[0], point[1], point[2], speed, 0, 0, (!agressive) ? 0 : 2);
				float dist = 99999;
				while (dist > 5.0f)
				{
					if (!Command<Commands::DOES_CHAR_EXIST>(pedHandle))
					{
						return;
					}
					if (!Command<Commands::DOES_VEHICLE_EXIST>(carHandle))
					{
						return;
					}
					dist = DistanceBetweenPoints(car->GetPosition(), CVector(point[0], point[1], point[2]));
					this_coro::wait(0);
				}
			}

			if (type_route_ped == 0)
				break;

			this_coro::wait(0);
		}

		Command<Commands::SET_CAR_STRONG>(carHandle, 0);

		const sol::protected_function play = node["callOutputLinks"];
		auto result = play(node, data, mission, id_out);
		if (!ScriptManager::checkProtected(result))
			CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);

	});
}

void nodeEnterCar(int pedHandle, int carHandle, int speed_walk, int seat, bool teleport, sol::table &node, int id_out, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([pedHandle, carHandle, speed_walk, seat, teleport, data, mission, node, id_out]() {

		Command<Commands::SET_NEXT_DESIRED_MOVE_STATE>(speed_walk + 4);

		if (seat == 0)
		{
			if (teleport)
			{
				Command<Commands::TASK_ENTER_CAR_AS_DRIVER>(pedHandle, carHandle, 0);
			}
			else
			{
				Command<Commands::TASK_ENTER_CAR_AS_DRIVER>(pedHandle, carHandle, -1);
			}
		}
		else
		{
			if (teleport)
			{
				Command<Commands::TASK_ENTER_CAR_AS_PASSENGER>(pedHandle, carHandle, 0, seat - 1);
			}
			else
			{
				Command<Commands::TASK_ENTER_CAR_AS_PASSENGER>(pedHandle, carHandle, -1, seat - 1);
			}
		}

		while (true)
		{
			if (!Command<Commands::DOES_CHAR_EXIST>(pedHandle))
			{
				return;
			}
			if (Command<Commands::IS_CHAR_IN_CAR>(pedHandle, carHandle))
			{
				break;
			}
			this_coro::wait(0);
		}

		const sol::protected_function play = node["callOutputLinks"];
		auto result = play(node, data, mission, id_out);
		if (!ScriptManager::checkProtected(result))
			CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);

	});
}

void mainCycle(sol::table &node, int id_out, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([data, mission, node, id_out]() {
		while (getMissionStarted())
		{
			const sol::protected_function play = node["callOutputLinks"];
			auto result = play(node, data, mission, id_out);
			if (!ScriptManager::checkProtected(result))
				CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			this_coro::wait(0);
		}
	});
}

void waitUtilNode(sol::table &node, int id_in, int id_out, int id_out2, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([data, mission, node, id_out, id_in, id_out2]() {
		while (getMissionStarted()) {
			this_coro::wait(0);
			const sol::protected_function pin = node["getPinValue"];
			auto result_pin = pin(node, id_in, data, mission);
			if (!ScriptManager::checkProtected(result_pin))
				CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			else {
				bool* cond = (bool*)((sol::object)result_pin).pointer();

				if (*cond)
				{
					const sol::protected_function play = node["callOutputLinks"];
					auto result = play(node, data, mission, id_out);
					if (!ScriptManager::checkProtected(result))
						CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
					break;
				}
				const sol::protected_function play = node["callOutputLinks"];
				auto result = play(node, data, mission, id_out2);
				if (!ScriptManager::checkProtected(result))
					CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			}
		}
	});
}

void targetCycle(sol::table &node, int target_id, int id_out, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([data, mission, node, id_out, target_id]() {
		while (getMissionStarted() && current_mission_target == target_id) {
			const sol::protected_function play = node["callOutputLinks"];
			auto result = play(node, data, mission, id_out);
			if (!ScriptManager::checkProtected(result))
				CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			this_coro::wait(0);
		}
	});
}

void triggerMoveObject(sol::table &node, int id_on, int object, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z, float speed, float radius, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue([=]() {
		setLastNode(node.get<int>("id"));
		if (mission->list_objects.at(object)->missionObject == nullptr)
		{
			CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			return;
		}
		CObject* obj = mission->list_objects.at(object)->missionObject;

		CVector start_pos = obj->GetPosition();
		float start_rot_x, start_rot_y, start_rot_z;
		obj->GetOrientation(start_rot_x, start_rot_y, start_rot_z);
		start_rot_x = deg(start_rot_x);
		start_rot_y = deg(start_rot_y);
		start_rot_z = deg(obj->GetHeading());
		float delta = 0;
		CVector& playerPos = playerPed->GetPosition();

		while (getMissionStarted())
		{
			if (obj == nullptr)
				break;

			const sol::protected_function pin = node["getPinValue"];
			auto result_pin = pin(node, id_on, data, mission);
			if (!ScriptManager::checkProtected(result_pin))
				CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			bool* cond = (bool*)((sol::object)result_pin).pointer();

			if (!*cond)
			{
				break;
			}
			
			float dist = DistanceBetweenPoints(playerPos, start_pos);

			if (dist < radius)
			{
				delta += speed;
			} else
			{
				delta -= speed;
			}

			delta = clamp(delta, 0.0f, 1.0f);

			float delta_x = start_pos.x * (1 - delta) + pos_x * delta;
			float delta_y = start_pos.y * (1 - delta) + pos_y * delta;
			float delta_z = start_pos.z * (1 - delta) + pos_z * delta;
			Command<Commands::SET_OBJECT_COORDINATES>(obj, delta_x, delta_y, delta_z);

			float delta_rot_x = start_rot_x * (1 - delta) + rot_x * delta;
			float delta_rot_y = start_rot_y * (1 - delta) + rot_y * delta;
			float delta_rot_z = start_rot_z * (1 - delta) + rot_z * delta;
			Command<Commands::SET_OBJECT_ROTATION>(obj, delta_rot_x, delta_rot_y, delta_rot_z);

			this_coro::wait(0);
		}
	});
}


int spaceVarsPtr = 0xA49960;

bool timerOn = false;
int timer = 12640;
void addTimer(std::string text, bool direction, int value) {
	int& adress = *(int*)(spaceVarsPtr + timer);
	adress = value;
	
	CUserDisplay::OnscnTimer.AddClock(timer, (char*)text.c_str(), direction);
}

void removeTimer()
{
	CUserDisplay::OnscnTimer.ClearClock(timer);
}

int getTimerTime()
{
	return *(int*)(spaceVarsPtr + timer);
}

bool counterOn[4] = { false, false, false, false };
int counter = 12644;
void addCounter(int slot, int type, std::string text) {
	CUserDisplay::OnscnTimer.AddCounter(counter + slot * 4, type, (char*)text.c_str(), slot);
}

void removeCounter(int slot)
{
	CUserDisplay::OnscnTimer.ClearCounter(counter + slot * 4);
}

void removeCounterByNode(int slot)
{
	counterOn[slot] = false;
}

void addCounterByNode(int slot, bool type, std::string gxt, sol::table node, NodeGraph* data, Mission* mission)
{
	
	addCounter(slot, type ? 1 : 0, gxt);
	
	
	instance.add_to_queue([=]()
	{
		if (counterOn[slot])
		{
			counterOn[slot] = false;
			this_coro::wait(1);
		}
		counterOn[slot] = true;
		while (getMissionStarted() && counterOn[slot])
		{
			const sol::protected_function getValue = node["getValue"];
			auto result_getValue = getValue(node, data, mission);
			if (!ScriptManager::checkProtected(result_getValue))
				CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
			int value = (int)result_getValue.get<float>();

			int& adress = *(int*)(spaceVarsPtr + counter + slot * 4);
			adress = value;
			
			this_coro::wait(0);
		}
		removeCounter(slot);
	});
}

void connectNodesFunctions(sol::table& t_ldyom)
{
	t_ldyom.set_function("SkinSelector", &SkinSelector);
	t_ldyom.set("moveObject", moveObject);
	t_ldyom.set("slideObject", slideObject);
	t_ldyom.set("staticCutscene", staticCutscene);
	t_ldyom.set("attachCameraToEntity", &attachCameraToEntity);
	t_ldyom.set("walkPointsNode", &walkPointsNode);
	t_ldyom.set("driveCarPointsNode", &driveCarPointsNode);
	t_ldyom.set("nodeEnterCar", &nodeEnterCar);
	t_ldyom.set("mainCycle", &mainCycle);
	t_ldyom.set("waitUtilNode", &waitUtilNode);
	t_ldyom.set("targetCycle", &targetCycle);
	t_ldyom.set("triggerMoveObject", &triggerMoveObject);
	t_ldyom.set("addTimer", addTimer);
	t_ldyom.set("removeTimer", removeTimer);
	t_ldyom.set("getTimerTime", getTimerTime);
	t_ldyom.set("addCounterByNode", addCounterByNode);
	t_ldyom.set("removeCounterByNode", removeCounterByNode);
	t_ldyom.set("addCounter", addCounter);
	t_ldyom.set("removeCounter", removeCounter);
}
