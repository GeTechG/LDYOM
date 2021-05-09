#include "ScriptManager.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

#include "imgui.h"
#include "NodeGraph.h"
#include "libs/ini.h"
#include "imnodes.h"
//#include "Init.h"
#include <CCamera.h>
#include <CHud.h>
#include <CMessages.h>

#include "libs/coro_wait.h"
#include "libs/ScriptCommands.h"
#include <boost/property_tree/json_parser.hpp>


extern void printLog(std::string print_text);
extern bool Combo(const char* label, int& current_item, std::vector<std::string>* items);
extern bool Combo(const char* label, int& current_item, std::vector<const char*>* items);
extern bool ListBox(const char* label, int& current_item, std::vector<const char*>* items);
extern bool ListBox(const char* label, int& current_item, std::vector<std::string>* items);
extern const char* langt(const std::string& key);
extern std::string curr_lang_string;
extern mINI::INIStructure lang_file;
extern std::vector<std::string> getStorylineMissionsNames();

std::vector<std::pair<bool, sol::state&>> ScriptManager::lua_scripts = {};

bool ComboVecStr(std::string label, sol::object current_item, std::vector<std::string>& items)
{
	int* value_p = (int*)current_item.pointer();
	bool result = Combo(label.c_str(), *value_p, &items);
	return result;
}

bool ComboVecChars(std::string label, sol::object current_item, std::vector<const char*>& items)
{
	int* value_p = (int*)current_item.pointer();
	bool result = Combo(label.c_str(), *value_p, &items);
	return result;
}

bool InputText(const char* name, sol::object buffer, unsigned int buffer_size)
{
	return ImGui::InputText(name, (char*)buffer.pointer(), buffer_size);
}

bool InputFloat(const char* name, sol::object value)
{
	return ImGui::InputFloat(name, (float*)value.pointer());
}

bool InputFloat3(const char* name, sol::object value)
{
	return ImGui::InputFloat3(name, (float*)value.pointer());
}

bool InputInt(const char* name, sol::object value)
{
	return ImGui::InputInt(name, (int*)value.pointer());
}

bool SliderInt(const char* label, sol::object value, int v_min, int v_max, const char* format)
{
	return ImGui::SliderInt(label, (int*)value.pointer(), v_min, v_max, format);
}

extern coro_wait instance;
void luaThreadFunc(sol::function func) {
	auto result = func();
	if (!ScriptManager::checkProtected(result))
		CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
}

void addThread(sol::function func)
{
	instance.add_to_queue(std::bind(luaThreadFunc, func));
}

void luaThreadFuncObj(sol::function func, sol::table obj) {
	auto result = func(obj);
	if (!ScriptManager::checkProtected(result))
		CMessages::AddMessageJumpQ("~r~Node Graph error! The mission will be unstable, dial the cheat code: LDSTOP, to end the mission prematurely.", 5000, 0, false);
}

void addThreadObj(sol::function func, sol::table obj)
{
	instance.add_to_queue(std::bind(luaThreadFuncObj, func, obj));
}

const char* getStrPtr(std::string& str)
{
	return str.c_str();
}

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
extern NodeGraph* currentNodeGraphPtr;
extern bool ToggleButton(const char* str_id, bool* v);
extern void stopStoryline();
extern void start_storyline_mission(std::string& mission_name);
extern int last_mission;
extern map<unsigned, sol::object> realVariable;
extern map <std::string, std::vector<std::string>> langMenu;
extern CPed *playerPed;
extern bool off_gui;
extern bool KeyJustPressed(unsigned int key);
extern std::string UTF8_to_CP1251(std::string const& utf8);
extern Mission* currentMissionPtr;
extern inline double rad(float degrees);
extern bool bNodeEditor;
extern inline double deg(float radians);

bool ToggleButtonLua(const char* str_id, sol::object v) {
	return ToggleButton(str_id, (bool*)v.pointer());
}

void wait (int mills)
{
	this_coro::wait(mills);
}

void callNodeThread(sol::table& node, NodeGraph* data, Mission* mission)
{
	instance.add_to_queue(std::bind(NodeGraph::callNode, node, data, mission));
}

void callNodeThreadS(sol::table& node, NodeGraph* data, Storyline* storyline)
{
	instance.add_to_queue(std::bind(NodeGraph::callNodeS, node, data, storyline));
}

int getCurrTarget()
{
	return current_mission_target;
}

void setCurrTarget(int new_target)
{
	current_mission_target = new_target;
}

char* getPointer(sol::object obj)
{
	return (char*)obj.pointer();
}

void addR(sol::table clas)
{
	sol::protected_function_result result = currentNodeGraphPtr->luaGraph["addR"](clas);
	if (!result.valid())
	{
		sol::error error = result;
		printLog(error.what());
	}
}

void setLastNode(int node_id)
{
	currentNodeGraphPtr->lastNode = node_id;
}

int getLastNode()
{
	return currentNodeGraphPtr->lastNode;
}

void set_last_mission(int last)
{
	last_mission = last;
}

void set_off_gui(bool off)
{
	off_gui = off;
;}

std::vector<std::string> parseJsonArray(std::string data) {
	std::vector<std::string> arr;
	boost::property_tree::ptree data_ptree;
	std::stringstream data_strsream;
	data_strsream << data;
	boost::property_tree::read_json(data_strsream, data_ptree);
	for (boost::property_tree::ptree::value_type &v : data_ptree) {
		arr.push_back(static_cast<std::string>(v.second.data()));
	}
	return arr;
}

std::string GXTEncode(std::string& str) {
	static const char sym_ru[67] = "¿¡¬√ƒ≈®∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ‡·‚„‰Â∏ÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ";
	static const char sym_sl[67] = "AÄãÇÉEEÑàÖÜKáñ≠OåPCèYÅXâçéäêëíìîïaó¢ôöeeõüúùkûØÆo£pc¶yòx†§•°ß®©™´¨";
	for (int i = 0; i < 67; i++) {
		std::replace(str.begin(), str.end(), sym_ru[i], sym_sl[i]);
	}
	return str;
}

void selectWeapon(const char* str_id, sol::object v)
{
	int* weap = (int*)v.pointer();
	if (ImGui::ImageButton(static_cast<void*>(weaponsAtlas), ImVec2(52, 52), ImVec2(*weap * 0.02272727272f, 0), ImVec2((*weap + 1) * 0.02272727272f, 1)))
	{
		ImGui::OpenPopup("weapon");
	}
	ImGui::SameLine();
	ImGui::Text(str_id);

	//weapon popup
	if (ImGui::BeginPopup("weapon"))
	{
		ImGui::BeginChild("weapon", ImVec2(200, 450));

		for (int i = 1; i < IM_ARRAYSIZE(ID_Weapons); i++)
		{
			ImGui::PushID(i);
			if (ImGui::ImageButton(weaponsAtlas, ImVec2(52, 52), ImVec2(i * 0.02272727272f, 0),
				ImVec2((i + 1) * 0.02272727272f, 1)))
			{
				*weap = i;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(langMenu["weap_names"][i].c_str());
			}
			ImGui::PopID();
			if (i % 3 != 0)
			{
				ImGui::SameLine();
			}
		}

		ImGui::EndChild();
		ImGui::EndPopup();
	}
	
}

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

void SetPedIndexModel(int ped, unsigned modelID)
{
	CPools::GetPed(ped)->SetModelIndex(modelID);
}

void LoadSpecialChar(int slot, const char* name)
{
	CStreaming::RequestSpecialChar(slot, name, 0);
	CStreaming::LoadAllRequestedModels(false);
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

void setbNodeEditor(bool boolean)
{
	bNodeEditor = boolean;
}

long getTime()
{
	return clock();
}

void getOrientationObject(CObject* obj,sol::object x_obj, sol::object y_obj, sol::object z_obj)
{
	float* x = (float*)x_obj.pointer();
	float* y = (float*)y_obj.pointer();
	float* z = (float*)z_obj.pointer();
	float rad_x, rad_y, rad_z;
	obj->GetOrientation(rad_x, rad_y, rad_z);
	rad_z = obj->GetHeading();
	*x = deg(rad_x);
	*y = deg(rad_y);
	*z = deg(rad_z);
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


std::tuple<float, float, float> getPos(float* pos)
{
	return std::make_tuple(pos[0], pos[1], pos[2]);
}

void setCameraLook(float x, float y, float z)
{
	TheCamera.m_vecAttachedCamLookAt.Set(x, y, z);
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
				Command<Commands::TASK_CAR_DRIVE_TO_COORD>(pedHandle, carHandle, point[0], point[1], point[2], speed, 0, 0, (!agressive)? 0 : 2);
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
			} else
			{
				Command<Commands::TASK_ENTER_CAR_AS_DRIVER>(pedHandle, carHandle, -1);
			}
		} else
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

void setNamespaceLua(sol::state& lua)
{
	lua.set_function("print", &printLog);
	
	lua["callOpcode"] = &pluginmy::scripting::CallCommandByIdPtr;
	lua["getPtrStr"] = &getStrPtr;
	lua["addThread"] = &addThread;
	lua["addThreadObj"] = &addThreadObj;
	lua["callNodeThread"] = &callNodeThread;
	lua["callNodeThreadS"] = &callNodeThreadS;
	lua["PLAYER_PED"] = playerPed;
	lua.set_function("wait", &wait);
	lua.set_function("getPointer", &getPointer);
	lua.set_function("getPedRef", CPools::GetPedRef);
	lua.set_function("getCarRef", CPools::GetVehicleRef);
	lua.set_function("getCarRef", CPools::GetVehicleRef);
	lua.set_function("getObjectRef", CPools::GetObjectRef);
	lua.set_function("isKeyJustPressed", &KeyJustPressed);
	lua.set_function("UTF8_to_CP1251", &UTF8_to_CP1251);
	lua.set_function("GXTEncode", &GXTEncode);
	lua.set_function("time_ms", &getTime);
	lua.set_function("getOrientationObject", getOrientationObject);

	auto nodeGraph_lua = lua.new_usertype<NodeGraph>("NodeGraph");
	nodeGraph_lua.set("nodes", &NodeGraph::nodes);
	nodeGraph_lua.set("links", &NodeGraph::links);
	nodeGraph_lua.set("vars", &NodeGraph::vars);
	
	//ldyom
	sol::table t_ldyom = lua["ldyom"].get_or_create<sol::table>();
	t_ldyom.set_function("langt", langt);
	t_ldyom.set_function("getStorylineMissionsNames", getStorylineMissionsNames);
	t_ldyom["namesStorylineCheckpoints"] = &namesStorylineCheckpoints;
	t_ldyom["namesTargets"] = &namesTargets;
	t_ldyom["namesCars"] = &namesCars;
	t_ldyom["namesTrains"] = &namesTrains;
	t_ldyom["namesActors"] = &namesActors;
	t_ldyom["namesObjects"] = &namesObjects;
	t_ldyom["namesParticles"] = &namesParticles;
	t_ldyom["namesPickups"] = &namesPickups;
	t_ldyom["namesExplosions"] = &namesExplosions;
	t_ldyom["namesAudios"] = &namesAudios;
	t_ldyom.set_function("setNodeSignal", &setNodeSignal);
	t_ldyom.set_function("getMissionStarted", &getMissionStarted);
	t_ldyom.set_function("getCurrTarget", &getCurrTarget);
	t_ldyom.set_function("setCurrTarget", &setCurrTarget);
	t_ldyom.set("setLastNode", &setLastNode);
	t_ldyom.set("getLastNode", &getLastNode);
	t_ldyom.set("currentNodeGraph", currentNodeGraphPtr);
	t_ldyom.set_function("stopStoryline", &stopStoryline);
	t_ldyom.set_function("start_storyline_mission", &start_storyline_mission);
	t_ldyom.set_function("set_last_mission", &set_last_mission);
	t_ldyom.set("realVariable", &realVariable);
	t_ldyom.set_function("parseJsonArray", &parseJsonArray);
	t_ldyom.set("langMenu", &langMenu);
	t_ldyom.set("Anim_name", &Anim_name);
	t_ldyom.set("Anim_list", &Anim_list);
	t_ldyom.set_function("set_off_gui", &set_off_gui);
	t_ldyom.set_function("selectWeapon", &selectWeapon);
	t_ldyom.set("ID_Weapons", &ID_Weapons);
	t_ldyom.set("ID_Spec_Actors", &ID_Spec_Actors);
	t_ldyom.set_function("SkinSelector", &SkinSelector);
	t_ldyom.set_function("SetPedIndexModel", &SetPedIndexModel);
	t_ldyom.set_function("LoadSpecialChar", &LoadSpecialChar);
	t_ldyom.set("currentMissionPtr", currentMissionPtr);
	t_ldyom.set("moveObject", moveObject);
	t_ldyom.set("slideObject", slideObject);
	t_ldyom.set("setbNodeEditor", setbNodeEditor);
	t_ldyom.set("staticCutscene", staticCutscene);
	t_ldyom.set("getPos", &getPos);
	t_ldyom.set("setCameraLook", setCameraLook);
	t_ldyom.set("attachCameraToEntity", &attachCameraToEntity);
	t_ldyom.set("walkPointsNode", &walkPointsNode);
	t_ldyom.set("driveCarPointsNode", &driveCarPointsNode);
	t_ldyom.set("nodeEnterCar", &nodeEnterCar);
	t_ldyom.set("mainCycle", &mainCycle);
	t_ldyom.set("waitUtilNode", &waitUtilNode);

	addLDYOMClasses(lua);
	
	//ldyom.nodeEditor
	t_ldyom["nodeEditor"].get_or_create<sol::table>();
	sol::function func = currentNodeGraphPtr->luaGraph["addRegister"];
	lua.set_function("addR", func);
	t_ldyom["nodeEditor"]["addNodeClass"] = &currentNodeGraphPtr->addNodeClass;
	t_ldyom["nodeEditor"]["getID"] = NodeGraph::getID;
	
	//ImGui
	sol::table t_imgui = lua["imgui"].get_or_create<sol::table>();

	//ImVec2
	sol::usertype<ImVec2> imguiImVec2 = t_imgui.new_usertype<ImVec2>("ImVec2",
		sol::constructors<ImVec2(),ImVec2(float,float)>()
		);
	imguiImVec2.set("x", &ImVec2::x);
	imguiImVec2.set("y", &ImVec2::y);
	
	//ImVec4
	sol::usertype<ImVec4> imguiImVec4 = t_imgui.new_usertype<ImVec4>("ImVec4",
		sol::constructors<ImVec4(), ImVec4(float, float, float, float)>()
		);
	imguiImVec4.set("x", &ImVec4::x);
	imguiImVec4.set("y", &ImVec4::y);
	imguiImVec4.set("z", &ImVec4::z);
	imguiImVec4.set("w", &ImVec4::w);

	t_imgui.set_function("Begin", ImGui::Begin);
	t_imgui.set_function("End", ImGui::End);

	t_imgui.set_function("BeginChildStr", sol::resolve<const char*, const ImVec2&, bool, ImGuiWindowFlags>(ImGui::BeginChild));
	t_imgui.set_function("BeginChild", sol::resolve<ImGuiID, const ImVec2&, bool, ImGuiWindowFlags>(ImGui::BeginChild));
	t_imgui.set_function("EndChild", ImGui::EndChild);

	t_imgui.set_function("IsWindowAppearing", ImGui::IsWindowAppearing);
	t_imgui.set_function("IsWindowCollapsed", ImGui::IsWindowCollapsed);
	t_imgui.set_function("IsWindowFocused", ImGui::IsWindowFocused);
	t_imgui.set_function("IsWindowHovered", ImGui::IsWindowHovered);
	t_imgui.set_function("GetWindowDrawList", ImGui::GetWindowDrawList);
	t_imgui.set_function("GetWindowPos", ImGui::GetWindowPos);
	t_imgui.set_function("GetWindowSize", ImGui::GetWindowSize);
	t_imgui.set_function("GetWindowWidth", ImGui::GetWindowWidth);
	t_imgui.set_function("GetWindowHeight", ImGui::GetWindowHeight);

	t_imgui.set_function("SetNextWindowPos", ImGui::SetNextWindowPos);
	t_imgui.set_function("SetNextWindowSize", ImGui::SetNextWindowSize);
	t_imgui.set_function("SetNextWindowSizeConstraints", ImGui::SetNextWindowSizeConstraints);
	t_imgui.set_function("SetNextWindowContentSize", ImGui::SetNextWindowContentSize);
	t_imgui.set_function("SetNextWindowCollapsed", ImGui::SetNextWindowCollapsed);
	t_imgui.set_function("SetNextWindowFocus", ImGui::SetNextWindowFocus);
	t_imgui.set_function("SetNextWindowBgAlpha", ImGui::SetNextWindowBgAlpha);
	t_imgui.set_function("SetWindowPos", sol::resolve<const ImVec2&, ImGuiCond>(ImGui::SetWindowPos));
	t_imgui.set_function("SetWindowSize", sol::resolve<const ImVec2&, ImGuiCond>(ImGui::SetWindowSize));
	t_imgui.set_function("SetWindowCollapsed", sol::resolve<bool, ImGuiCond>(ImGui::SetWindowCollapsed));
	t_imgui.set_function("SetWindowFocus", sol::resolve<void()>(ImGui::SetWindowFocus));
	t_imgui.set_function("SetWindowFontScale", ImGui::SetWindowFontScale);
	t_imgui.set_function("SetWindowPosStr", sol::resolve<const char*, const ImVec2&, ImGuiCond>(ImGui::SetWindowPos));
	t_imgui.set_function("SetWindowSizeStr", sol::resolve<const char*, const ImVec2&, ImGuiCond>(ImGui::SetWindowSize));
	t_imgui.set_function("SetWindowCollapsedStr", sol::resolve<const char*, bool, ImGuiCond>(ImGui::SetWindowCollapsed));
	t_imgui.set_function("SetWindowFocusStr", sol::resolve<const char*>(ImGui::SetWindowFocus));

	t_imgui.set_function("GetContentRegionMax", ImGui::GetContentRegionMax);
	t_imgui.set_function("GetContentRegionAvail", ImGui::GetContentRegionAvail);
	t_imgui.set_function("GetWindowContentRegionMin", ImGui::GetWindowContentRegionMin);
	t_imgui.set_function("GetWindowContentRegionMax", ImGui::GetWindowContentRegionMax);
	t_imgui.set_function("GetWindowContentRegionWidth", ImGui::GetWindowContentRegionWidth);

	t_imgui.set_function("GetScrollX", ImGui::GetScrollX);
	t_imgui.set_function("GetScrollY", ImGui::GetScrollY);
	t_imgui.set_function("GetScrollMaxX", ImGui::GetScrollMaxX);
	t_imgui.set_function("GetScrollMaxY", ImGui::GetScrollMaxY);
	//t_imgui.set_function("SetScrollX", ImGui::SetScrollX);
	//t_imgui.set_function("SetScrollY", ImGui::SetScrollY);
	t_imgui.set_function("SetScrollHereX", ImGui::SetScrollHereX);
	t_imgui.set_function("SetScrollHereY", ImGui::SetScrollHereY);
	//t_imgui.set_function("SetScrollFromPosX", ImGui::SetScrollFromPosX);
	//t_imgui.set_function("SetScrollFromPosY", ImGui::SetScrollFromPosY);

	t_imgui.set_function("PushFont", ImGui::PushFont);
	t_imgui.set_function("PopFont", ImGui::PopFont);
	t_imgui.set_function("PushStyleColor", sol::resolve<ImGuiCol, ImU32>(ImGui::PushStyleColor));
	t_imgui.set_function("PushStyleColorImVec4", sol::resolve<ImGuiCol, const ImVec4&>(ImGui::PushStyleColor));
	t_imgui.set_function("PopStyleColor", ImGui::PopStyleColor);
	t_imgui.set_function("PushStyleVar", sol::resolve<ImGuiStyleVar, float>(ImGui::PushStyleVar));
	t_imgui.set_function("PushStyleVarImVec4", sol::resolve<ImGuiStyleVar, const ImVec2&>(ImGui::PushStyleVar));
	t_imgui.set_function("PopStyleVar", ImGui::PopStyleVar);
	t_imgui.set_function("GetStyleColorVec4", ImGui::GetStyleColorVec4);
	t_imgui.set_function("GetFont", ImGui::GetFont);
	t_imgui.set_function("GetFontSize", ImGui::GetFontSize);
	t_imgui.set_function("GetFontTexUvWhitePixel", ImGui::GetFontTexUvWhitePixel);
	t_imgui.set_function("GetColorU32ImGuiCol", sol::resolve<ImGuiCol, float>(ImGui::GetColorU32));
	t_imgui.set_function("GetColorU32Vec4", sol::resolve<const ImVec4&>(ImGui::GetColorU32));
	t_imgui.set_function("GetColorU32", sol::resolve<ImU32>(ImGui::GetColorU32));

	t_imgui.set_function("PushItemWidth", ImGui::PushItemWidth);
	t_imgui.set_function("PopItemWidth", ImGui::PopItemWidth);
	t_imgui.set_function("SetNextItemWidth", ImGui::SetNextItemWidth);
	t_imgui.set_function("CalcItemWidth", ImGui::CalcItemWidth);
	t_imgui.set_function("PushTextWrapPos", ImGui::PushTextWrapPos);
	t_imgui.set_function("PopTextWrapPos", ImGui::PopTextWrapPos);
	t_imgui.set_function("PushAllowKeyboardFocus", ImGui::PushAllowKeyboardFocus);
	t_imgui.set_function("PopAllowKeyboardFocus", ImGui::PopAllowKeyboardFocus);
	t_imgui.set_function("PushButtonRepeat", ImGui::PushButtonRepeat);
	t_imgui.set_function("PopButtonRepeat", ImGui::PopButtonRepeat);

	t_imgui.set_function("Separator", ImGui::Separator);
	t_imgui.set_function("SameLine", ImGui::SameLine);
	t_imgui.set_function("NewLine", ImGui::NewLine);
	t_imgui.set_function("Spacing", ImGui::Spacing);
	t_imgui.set_function("Dummy", ImGui::Dummy);
	t_imgui.set_function("Indent", ImGui::Indent);
	t_imgui.set_function("Unindent", ImGui::Unindent);
	t_imgui.set_function("BeginGroup", ImGui::BeginGroup);
	t_imgui.set_function("EndGroup", ImGui::EndGroup);
	t_imgui.set_function("GetCursorPos", ImGui::GetCursorPos);
	t_imgui.set_function("GetCursorPosX", ImGui::GetCursorPosX);
	t_imgui.set_function("GetCursorPosY", ImGui::GetCursorPosY);
	t_imgui.set_function("SetCursorPos", ImGui::SetCursorPos);
	t_imgui.set_function("SetCursorPosX", ImGui::SetCursorPosX);
	t_imgui.set_function("SetCursorPosY", ImGui::SetCursorPosY);
	t_imgui.set_function("GetCursorStartPos", ImGui::GetCursorStartPos);
	t_imgui.set_function("GetCursorScreenPos", ImGui::GetCursorScreenPos);
	t_imgui.set_function("SetCursorScreenPos", ImGui::SetCursorScreenPos);
	t_imgui.set_function("AlignTextToFramePadding", ImGui::AlignTextToFramePadding);
	t_imgui.set_function("GetTextLineHeight", ImGui::GetTextLineHeight);
	t_imgui.set_function("GetTextLineHeightWithSpacing", ImGui::GetTextLineHeightWithSpacing);
	t_imgui.set_function("GetFrameHeight", ImGui::GetFrameHeight);
	t_imgui.set_function("GetFrameHeightWithSpacing", ImGui::GetFrameHeightWithSpacing);

	t_imgui.set_function("PushID", sol::resolve<void(const char*)>(ImGui::PushID));
	t_imgui.set_function("PushID", sol::resolve<const char*, const char*>(ImGui::PushID));
	t_imgui.set_function("PushID", sol::resolve<const void*>(ImGui::PushID));
	t_imgui.set_function("PushID", sol::resolve<int>(ImGui::PushID));
	t_imgui.set_function("PopID", ImGui::PopID);
	t_imgui.set_function("GetID", sol::resolve<ImGuiID(const char*)>(ImGui::GetID));
	t_imgui.set_function("GetID", sol::resolve<const char*, const char*>(ImGui::GetID));
	t_imgui.set_function("GetID", sol::resolve<const void*>(ImGui::GetID));

	t_imgui.set_function("TextUnformatted", ImGui::TextUnformatted);
	t_imgui.set_function("Text", ImGui::Text);
	t_imgui.set_function("TextV", ImGui::TextV);
	t_imgui.set_function("TextColored", ImGui::TextColored);
	t_imgui.set_function("TextColoredV", ImGui::TextColoredV);
	t_imgui.set_function("TextDisabled", ImGui::TextDisabled);
	t_imgui.set_function("TextDisabledV", ImGui::TextDisabledV);
	t_imgui.set_function("TextWrapped", ImGui::TextWrapped);
	t_imgui.set_function("TextWrappedV", ImGui::TextWrappedV);
	t_imgui.set_function("LabelText", ImGui::LabelText);
	t_imgui.set_function("LabelTextV", ImGui::LabelTextV);
	t_imgui.set_function("BulletText", ImGui::BulletText);
	t_imgui.set_function("BulletTextV", ImGui::BulletTextV);


	t_imgui.set_function("Button", ImGui::Button);
	t_imgui.set_function("SmallButton", ImGui::SmallButton);
	t_imgui.set_function("InvisibleButton", ImGui::InvisibleButton);
	t_imgui.set_function("ArrowButton", ImGui::ArrowButton);
	t_imgui.set_function("Image", ImGui::Image);
	t_imgui.set_function("ImageButton", ImGui::ImageButton);
	t_imgui.set_function("Checkbox", ImGui::Checkbox);
	//t_imgui.set_function("CheckboxFlags", ImGui::CheckboxFlags);
	t_imgui.set_function("RadioButton", sol::resolve<const char*, bool>(ImGui::RadioButton));
	t_imgui.set_function("RadioButtonP", sol::resolve<const char*, int*, int>(ImGui::RadioButton));
	t_imgui.set_function("ProgressBar", ImGui::ProgressBar);
	t_imgui.set_function("Bullet", ImGui::Bullet);

	t_imgui.set_function("BeginCombo", ImGui::BeginCombo);
	t_imgui.set_function("EndCombo", ImGui::EndCombo);
	//t_imgui.set_function("Combo", sol::resolve<const char*, int*, const char*, int>(ImGui::Combo));
	t_imgui.set_function("ComboVecStr", ComboVecStr);
	t_imgui.set_function("ComboVecChars", ComboVecChars);

	//t_imgui.set_function("DragFloat", ImGui::DragFloat);
	//t_imgui.set_function("DragFloat2", ImGui::DragFloat2);
	//t_imgui.set_function("DragFloat3", ImGui::DragFloat3);
	//t_imgui.set_function("DragFloat4", ImGui::DragFloat4);
	t_imgui.set_function("DragFloatRange2", ImGui::DragFloatRange2);
	t_imgui.set_function("DragInt", ImGui::DragInt);
	t_imgui.set_function("DragInt2", ImGui::DragInt2);
	t_imgui.set_function("DragInt3", ImGui::DragInt3);
	t_imgui.set_function("DragInt4", ImGui::DragInt4);
	t_imgui.set_function("DragIntRange2", ImGui::DragIntRange2);
	//t_imgui.set_function("DragScalar", ImGui::DragScalar);
	//t_imgui.set_function("DragScalarN", ImGui::DragScalarN);

	//t_imgui.set_function("SliderFloat", ImGui::SliderFloat);
	//t_imgui.set_function("SliderFloat2", ImGui::SliderFloat2);
	//t_imgui.set_function("SliderFloat3", ImGui::SliderFloat3);
	//t_imgui.set_function("SliderFloat4", ImGui::SliderFloat4);
	t_imgui.set_function("SliderAngle", ImGui::SliderAngle);
	t_imgui.set_function("SliderInt", SliderInt);
	t_imgui.set_function("SliderInt2", ImGui::SliderInt2);
	t_imgui.set_function("SliderInt3", ImGui::SliderInt3);
	t_imgui.set_function("SliderInt4", ImGui::SliderInt4);
	//t_imgui.set_function("SliderScalar", ImGui::SliderScalar);
	//t_imgui.set_function("SliderScalarN", ImGui::SliderScalarN);
	t_imgui.set_function("VSliderFloat", ImGui::VSliderFloat);
	t_imgui.set_function("VSliderInt", ImGui::VSliderInt);
	t_imgui.set_function("VSliderScalar", ImGui::VSliderScalar);

	t_imgui.set_function("InputText", InputText);
	t_imgui.set_function("InputTextMultiline", ImGui::InputTextMultiline);
	t_imgui.set_function("InputTextWithHint", ImGui::InputTextWithHint);
	t_imgui.set_function("InputFloat", InputFloat);
	//t_imgui.set_function("InputFloat2", sol::resolve<const char*, float[2], const char*, ImGuiInputTextFlags>(ImGui::InputFloat2));
	t_imgui.set_function("InputFloat3", InputFloat3);
	//t_imgui.set_function("InputFloat4", sol::resolve<const char*, float[4], const char*, ImGuiInputTextFlags>(ImGui::InputFloat4));
	t_imgui.set_function("InputInt", InputInt);
	t_imgui.set_function("InputInt2", ImGui::InputInt2);
	t_imgui.set_function("InputInt3", ImGui::InputInt3);
	t_imgui.set_function("InputInt4", ImGui::InputInt4);
	t_imgui.set_function("InputDouble", ImGui::InputDouble);
	t_imgui.set_function("InputScalar", ImGui::InputScalar);
	t_imgui.set_function("InputScalarN", ImGui::InputScalarN);

	//t_imgui.set_function("ListBox", sol::resolve<const char*, int*, const char* const[], int, int>(ImGui::ListBox));
	//t_imgui.set_function("ListBox", sol::resolve<const char*, int&, std::vector<const char*>*>(ListBox));
	//t_imgui.set_function("ListBox", sol::resolve<const char*, int&, std::vector<std::string>*>(ListBox));

	t_imgui.set_function("BeginTooltip", ImGui::BeginTooltip);
	t_imgui.set_function("EndTooltip", ImGui::EndTooltip);
	t_imgui.set_function("SetTooltip", ImGui::SetTooltip);
	t_imgui.set_function("SetTooltipV", ImGui::SetTooltipV);

	t_imgui.set_function("BeginPopup", ImGui::BeginPopup);
	t_imgui.set_function("BeginPopupModal", ImGui::BeginPopupModal);
	t_imgui.set_function("EndPopup", ImGui::EndPopup);

	t_imgui.set_function("OpenPopup", ImGui::OpenPopup);
	t_imgui.set_function("OpenPopupContextItem", ImGui::OpenPopupContextItem);
	t_imgui.set_function("CloseCurrentPopup", ImGui::CloseCurrentPopup);

	t_imgui.set_function("IsItemHovered", ImGui::IsItemHovered);
	t_imgui.set_function("IsItemActive", ImGui::IsItemActive);
	t_imgui.set_function("IsItemFocused", ImGui::IsItemFocused);
	t_imgui.set_function("IsItemClicked", ImGui::IsItemClicked);
	t_imgui.set_function("IsItemVisible", ImGui::IsItemVisible);
	t_imgui.set_function("IsItemEdited", ImGui::IsItemEdited);
	t_imgui.set_function("IsItemActivated", ImGui::IsItemActivated);
	t_imgui.set_function("IsItemDeactivated", ImGui::IsItemDeactivated);
	t_imgui.set_function("IsItemDeactivatedAfterEdit", ImGui::IsItemDeactivatedAfterEdit);
	t_imgui.set_function("IsItemToggledOpen", ImGui::IsItemToggledOpen);
	t_imgui.set_function("IsAnyItemHovered", ImGui::IsAnyItemHovered);
	t_imgui.set_function("IsAnyItemActive", ImGui::IsAnyItemActive);
	t_imgui.set_function("IsAnyItemFocused", ImGui::IsAnyItemFocused);
	t_imgui.set_function("GetItemRectMin", ImGui::GetItemRectMin);
	t_imgui.set_function("GetItemRectMax", ImGui::GetItemRectMax);
	t_imgui.set_function("GetItemRectSize", ImGui::GetItemRectSize);
	t_imgui.set_function("SetItemAllowOverlap", ImGui::SetItemAllowOverlap);

	t_imgui.set_function("IsMouseDown", ImGui::IsMouseDown);
	t_imgui.set_function("IsMouseClicked", ImGui::IsMouseClicked);
	t_imgui.set_function("IsMouseReleased", ImGui::IsMouseReleased);
	t_imgui.set_function("IsMouseDoubleClicked", ImGui::IsMouseDoubleClicked);
	t_imgui.set_function("IsMouseHoveringRect", ImGui::IsMouseHoveringRect);
	t_imgui.set_function("IsMousePosValid", ImGui::IsMousePosValid);
	t_imgui.set_function("IsAnyMouseDown", ImGui::IsAnyMouseDown);
	t_imgui.set_function("GetMousePos", ImGui::GetMousePos);
	t_imgui.set_function("GetMousePosOnOpeningCurrentPopup", ImGui::GetMousePosOnOpeningCurrentPopup);
	t_imgui.set_function("IsMouseDragging", ImGui::IsMouseDragging);
	t_imgui.set_function("GetMouseDragDelta", ImGui::GetMouseDragDelta);
	t_imgui.set_function("ResetMouseDragDelta", ImGui::ResetMouseDragDelta);
	t_imgui.set_function("GetMouseCursor", ImGui::GetMouseCursor);
	t_imgui.set_function("SetMouseCursor", ImGui::SetMouseCursor);
	t_imgui.set_function("CaptureMouseFromApp", ImGui::CaptureMouseFromApp);
	t_imgui.set_function("ToggleButton", ToggleButtonLua);

	//imnodes
	sol::table t_imnodes = t_imgui["imnodes"].get_or_create<sol::table>();

	t_imnodes.set_function("PushColorStyle", ImNodes::PushColorStyle);
	t_imnodes.set_function("PopColorStyle", ImNodes::PopColorStyle);
	t_imnodes.set_function("PushStyleVar", ImNodes::PushStyleVar);
	t_imnodes.set_function("PopStyleVar", ImNodes::PopStyleVar);
	
	t_imnodes.set_function("BeginNodeDef", ImNodes::BeginNode);
	t_imnodes.set_function("EndNodeDef", ImNodes::EndNode);
	t_imnodes.set_function("BeginNode", NodeGraph::BeginNode);
	t_imnodes.set_function("EndNode", NodeGraph::EndNode);
	
	t_imnodes.set_function("GetNodeDimensions", ImNodes::GetNodeDimensions);
	
	t_imnodes.set_function("BeginNodeTitleBar", ImNodes::BeginNodeTitleBar);
	t_imnodes.set_function("EndNodeTitleBar", ImNodes::EndNodeTitleBar);
	
	t_imnodes.set_function("BeginInputAttributeDef", ImNodes::BeginInputAttribute);
	t_imnodes.set_function("EndInputAttributeDef", ImNodes::EndInputAttribute);
	t_imnodes.set_function("BeginInputAttribute", NodeGraph::BeginInputAttribute);
	t_imnodes.set_function("EndInputAttribute", NodeGraph::EndInputAttribute);
	
	t_imnodes.set_function("BeginOutputAttributeDef", ImNodes::BeginOutputAttribute);
	t_imnodes.set_function("EndOutputAttributeDef", ImNodes::EndOutputAttribute);
	t_imnodes.set_function("BeginOutputAttribute", NodeGraph::BeginOutputAttribute);
	t_imnodes.set_function("EndOutputAttribute", NodeGraph::EndOutputAttribute);
	
	t_imnodes.set_function("BeginStaticAttribute", ImNodes::BeginStaticAttribute);
	t_imnodes.set_function("EndStaticAttribute", ImNodes::EndStaticAttribute);
	
	t_imnodes.set_function("PushAttributeFlag", ImNodes::PushAttributeFlag);
	t_imnodes.set_function("PopAttributeFlag", ImNodes::PopAttributeFlag);
	
	t_imnodes.set_function("Link", ImNodes::Link);
	
	t_imnodes.set_function("SetNodeDraggable", ImNodes::SetNodeDraggable);
	
	t_imnodes.set_function("SetNodeScreenSpacePos", ImNodes::SetNodeScreenSpacePos);
	t_imnodes.set_function("SetNodeEditorSpacePos", ImNodes::SetNodeEditorSpacePos);
	t_imnodes.set_function("SetNodeGridSpacePos", ImNodes::SetNodeGridSpacePos);
	
	t_imnodes.set_function("GetNodeScreenSpacePos", ImNodes::GetNodeScreenSpacePos);
	t_imnodes.set_function("GetNodeEditorSpacePos", ImNodes::GetNodeEditorSpacePos);
	t_imnodes.set_function("GetNodeGridSpacePos", ImNodes::GetNodeGridSpacePos);
	
	t_imnodes.set_function("IsEditorHovered", ImNodes::IsEditorHovered);
	
	t_imnodes.set_function("IsNodeHovered", ImNodes::IsNodeHovered);
	t_imnodes.set_function("IsLinkHovered", ImNodes::IsLinkHovered);
	t_imnodes.set_function("IsPinHovered", ImNodes::IsPinHovered);
	
	t_imnodes.set_function("NumSelectedNodes", ImNodes::NumSelectedNodes);
	t_imnodes.set_function("NumSelectedLinks", ImNodes::NumSelectedLinks);
	
	t_imnodes.set_function("GetSelectedNodes", ImNodes::GetSelectedNodes);
	t_imnodes.set_function("GetSelectedLinks", ImNodes::GetSelectedLinks);
	
	t_imnodes.set_function("ClearNodeSelection", ImNodes::ClearNodeSelection);
	t_imnodes.set_function("ClearLinkSelection", ImNodes::ClearLinkSelection);
	
	t_imnodes.set_function("IsAttributeActive", ImNodes::IsAttributeActive);
	t_imnodes.set_function("IsAnyAttributeActive", ImNodes::IsAnyAttributeActive);
	
	t_imnodes.set_function("IsLinkStarted", ImNodes::IsLinkStarted);
	t_imnodes.set_function("IsLinkDropped", ImNodes::IsLinkDropped);
	t_imnodes.set_function("IsLinkCreated", sol::resolve<int*, int*, bool*>(ImNodes::IsLinkCreated));
	
	t_imnodes.set_function("IsLinkDestroyed", ImNodes::IsLinkDestroyed);
	t_imnodes.set_function("IsLinkDestroyed", ImNodes::IsLinkDestroyed);

	t_imnodes.set_function("getNodeIcon", NodeGraph::getNodeIcon);
	
	t_imnodes.new_enum<ImNodesPinShape>("PinShape", {
		{"Circle",ImNodesPinShape_Circle},
		{"CircleFilled",ImNodesPinShape_CircleFilled},
		{"Triangle",ImNodesPinShape_Triangle},
		{"TriangleFilled",ImNodesPinShape_TriangleFilled},
		{"Quad",ImNodesPinShape_Quad},
		{"QuadFilled",ImNodesPinShape_QuadFilled},
		});

	t_imnodes["PinType"].get_or_create<sol::table>();
	t_imnodes["PinType"]["number"] = (int)NodeGraph::PinType::number;
	t_imnodes["PinType"]["boolean"] = (int)NodeGraph::PinType::boolean;
	t_imnodes["PinType"]["string"] = (int)NodeGraph::PinType::string;
	t_imnodes["PinType"]["void"] = (int)NodeGraph::PinType::void_;
	
	
	t_imnodes.new_enum<ImNodesCol>("ColorStyle", {
		{"NodeBackground",ImNodesCol_NodeBackground},
		{"NodeBackgroundHovered",ImNodesCol_NodeBackgroundHovered},
		{"NodeBackgroundSelected",ImNodesCol_NodeBackgroundSelected},
		{"NodeOutline",ImNodesCol_NodeOutline},
		{"TitleBar",ImNodesCol_TitleBar},
		{"TitleBarHovered",ImNodesCol_TitleBarHovered},
		{"TitleBarSelected",ImNodesCol_TitleBarSelected},
		{"Link",ImNodesCol_Link},
		{"LinkHovered",ImNodesCol_LinkHovered},
		{"LinkSelected",ImNodesCol_LinkSelected},
		{"Pin",ImNodesCol_Pin},
		{"PinHovered",ImNodesCol_PinHovered},
		{"BoxSelector",ImNodesCol_BoxSelector},
		{"BoxSelectorOutline",ImNodesCol_BoxSelectorOutline},
		{"GridBackground",ImNodesCol_GridBackground},
		{"GridLine",ImNodesCol_GridLine},
		{"Count",ImNodesCol_COUNT},
		});
}

extern std::map<std::string, std::vector<sol::table>> map_nodes_class;
extern NodeGraph* currentNodeGraphPtr;

void ScriptManager::loadScripts()
{
	std::string path = "LDYOM//Scripts";
	delete currentNodeGraphPtr;
	currentNodeGraphPtr = new NodeGraph();
	
	for (auto lua_script : lua_scripts)
		delete &lua_script.second;
	lua_scripts.clear();
	
	mINI::INIStructure config;

	std::string path_config = path+"//config.ini";
	mINI::INIFile file(path_config);
	file.read(config);
	
	if (!path.empty()) {
		boost::filesystem::path apk_path(path);
		boost::filesystem::directory_iterator iter(apk_path), eod;

		for (boost::filesystem::path i : boost::filesystem::directory_iterator(path))
		{
			if (boost::filesystem::is_directory(i)) {

				std::string init_file = i.string() + "//init.lua";
				
				if (boost::filesystem::exists(init_file))
				{
					sol::state& lua = *(new sol::state);
					
					lua.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::ffi, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::string);
					
					setNamespaceLua(lua);

					auto script = lua.script_file(init_file);
					
					if (!script.valid())
					{
						sol::error error = script;
						printLog(error.what());
					} else
					{
						bool active = false;
						sol::optional<std::string> name_script = lua["info"]["name"];
						if (name_script != sol::nullopt) {
							if (config["Scripts"][name_script.value()] != "false")
							{
								//load lang script
								std::string path_lang_folder = i.string() + "//Language";
								if (boost::filesystem::exists(path_lang_folder))
								{
									std::string english_lang = path_lang_folder + "//English.ini";
									std::string custom_lang = path_lang_folder + "//" + curr_lang_string + ".ini";

									mINI::INIStructure lang{};
									if (boost::filesystem::exists(custom_lang))
									{
										mINI::INIFile file(custom_lang);
										file.read(lang);
									}
									else if (boost::filesystem::exists(english_lang))
									{
										mINI::INIFile file(english_lang);
										file.read(lang);
									}
									if (lang.size() > 0)
									{
										for (auto const& it2 : lang["Keys"])
										{
											lang_file["Keys"][it2.first] = it2.second;
										}
									}

								}
								
								active = true;
								sol::protected_function fRun = lua["main"];
								if (fRun.valid()) {
									auto result = fRun();
									if (!result.valid()) {
										sol::error error = result;
										printLog(error.what());
									}
								}
								
							}
							lua_scripts.emplace_back(active,lua);
						} else
						{
							printLog(i.string() + ": The script was rejected. Reason: Poor information about.");
							delete &lua;
						}
					}
				}
			}
		}
	}
}

bool ScriptManager::checkProtected(sol::protected_function_result& result)
{
	if (!result.valid())
	{
		sol::error error = result;
		printLog(error.what());
		return false;
	}
	return true;
}