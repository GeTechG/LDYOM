#include "carSelector.h"

char carSelector::search[65] = "";
bool carSelector::filter_car = true;
bool carSelector::filter_bike = true;
bool carSelector::filter_mtrack = true;
bool carSelector::filter_quad = true;
bool carSelector::filter_plane = true;
bool carSelector::filter_heli = true;
bool carSelector::filter_bmx = true;
bool carSelector::filter_trailer = true;
bool carSelector::filter_boat = true;
int carSelector::typeSearch = 0;
bool carSelector::bInitVehicles = false;
bool carSelector::bShow = false;
vector<VehicleInfo> carSelector::vehicles = {};
float carSelector::scale = 0.5f;
int* carSelector::modelID = nullptr;


extern const char* langt(const std::string& key);
extern map <std::string, std::vector<std::string>> langMenu;
extern bool Combo(const char* label, int& current_item, vector<std::string>* items);
extern bool Combo(const char* label, int& current_item, vector<const char*>* items);
extern bool LoadTextureFromFile(std::string path, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
extern bool bCars;
extern Mission* currentMissionPtr;
extern int currentCar;
extern bool bCars;

void carSelector::fShow() {
	if (!bInitVehicles) {
		fInitVehicles();
		bInitVehicles = true;
	}

	ImGui::Begin(langt("carSelector"), NULL);

	ImGui::SetNextItemWidth(120);
	ImGui::PushID("searchType");
	Combo("", typeSearch, &langMenu["searchType"]);
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120);
	ImGui::InputText(langt("search"), search, IM_ARRAYSIZE(search));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120);
	if (ImGui::Button(langt("filter"))) {
		ImGui::OpenPopup("filter");
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120);
	ImGui::DragFloat("scale", &scale, 0.01f, 0.0001f);

	if (ImGui::BeginPopup("filter")) {
		
		if (ImGui::MenuItem(langMenu["filterVeh"][0].c_str(), "", filter_car))
			filter_car = !filter_car;
		if (ImGui::MenuItem(langMenu["filterVeh"][1].c_str(), "", filter_bike))
			filter_bike = !filter_bike;
		if (ImGui::MenuItem(langMenu["filterVeh"][2].c_str(), "", filter_mtrack))
			filter_mtrack = !filter_mtrack;
		if (ImGui::MenuItem(langMenu["filterVeh"][3].c_str(), "", filter_quad))
			filter_quad = !filter_quad;
		if (ImGui::MenuItem(langMenu["filterVeh"][4].c_str(), "", filter_heli))
			filter_heli = !filter_heli;
		if (ImGui::MenuItem(langMenu["filterVeh"][5].c_str(), "", filter_plane))
			filter_plane = !filter_plane;
		if (ImGui::MenuItem(langMenu["filterVeh"][6].c_str(), "", filter_bmx))
			filter_bmx = !filter_bmx;
		if (ImGui::MenuItem(langMenu["filterVeh"][7].c_str(), "", filter_trailer))
			filter_trailer = !filter_trailer;
		if (ImGui::MenuItem(langMenu["filterVeh"][8].c_str(), "", filter_boat))
			filter_boat = !filter_boat;
		
		ImGui::EndPopup();
	}

	ImGui::BeginChild("vehs", ImVec2(ImGui::GetWindowWidth()-5, ImGui::GetWindowHeight() - ImGui::GetCursorPosY()-5), true);
	
	int countWidth = (int)(ImGui::GetWindowWidth() / (224 * scale));
	int i = 1;
	for (VehicleInfo veh : vehicles) {
		if (search[0] != '\0') {
			if (typeSearch == 0) {
				if (std::string(veh.name).find(search) == std::string::npos)
					continue;
			} else {
				if (std::to_string(veh.id).find(search) == std::string::npos)
					continue;
			}
		}
		switch (reinterpret_cast<CVehicleModelInfo *>(CModelInfo::ms_modelInfoPtrs[veh.id])->m_nVehicleType) {
			case VEHICLE_MTRUCK:
				if (!filter_mtrack) {
					continue;
				}
				break;
			case VEHICLE_QUAD:
				if (!filter_quad) {
					continue;
				}
				break;
			case VEHICLE_HELI:
				if (!filter_heli) {
					continue;
				}
				break;
			case VEHICLE_PLANE:
				if (!filter_plane) {
					continue;
				}
				break;
			case VEHICLE_BIKE:
				if (!filter_bike) {
					continue;
				}
				break;
			case VEHICLE_BMX:
				if (!filter_bmx) {
					continue;
				}
				break;
			case VEHICLE_TRAILER:
				if (!filter_trailer) {
					continue;
				}
				break;
			case VEHICLE_BOAT:
				if (!filter_boat) {
					continue;
				}
				break;
			case VEHICLE_TRAIN:
				continue;
				break;
			default:
				if (!filter_car) {
					continue;
				}
				break;
		}

		if (ImGui::ImageButton((void*)veh.pic, ImVec2(204 * scale, 125 * scale), ImVec2(0, 0), ImVec2(1, 1))) {
			currentMissionPtr->list_cars[currentCar]->modelID = veh.id;
			currentMissionPtr->list_cars[currentCar]->updateEditorCar(true);
			carSelector::bShow = false;
			bCars = true;
		}

		if (ImGui::IsItemHovered()) {
			std::string info;
			info.append("ModelID: ");
			info.append(std::to_string(veh.id));
			info.append("\n");

			info.append("Name: ");
			info.append(veh.name);

			ImGui::SetTooltip(info.c_str());
		}

		if (i % countWidth != 0) {
			ImGui::SameLine();
		}
		
		i++;
	}

	ImGui::EndChild();

	ImGui::End();
}

void carSelector::fInitVehicles() {
	for (int i = 400; i <= 611; i++) {
		VehicleInfo veh = VehicleInfo();
		veh.id = i;

		char nameGXT[8];
		strcpy(nameGXT, &reinterpret_cast<CVehicleModelInfo *>(CModelInfo::ms_modelInfoPtrs[i])->m_szGameName[0]);
		intptr_t ptr_name;
		Command<0x10ADE>(nameGXT, &ptr_name);
		strcpy(veh.name, (char*)ptr_name);

		std::string path;
		path.append("LDYOM//resources//Vehicle_");
		path.append(std::to_string(i));
		path.append(".jpg");
		LoadTextureFromFile(path, &veh.pic, nullptr, nullptr);

		vehicles.push_back(veh);
	}
}
