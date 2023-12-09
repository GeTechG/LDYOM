#include "utils.h"

#include <CRadar.h>
#include <filesystem>
#include <vector>
#include <extensions/ScriptCommands.h>

#include <CSprite.h>

#include "CCamera.h"
#include "d3dx9tex.h"
#include "HttpRequester.h"
#include "imgui_internal.h"
#include "KeyCodes.h"
#include "Logger.h"
#include "memsafe.h"
#include "plugin.h"
#include "strUtils.h"

bool utils::Combo(const char *label, int *currentItem, const std::vector<std::string> &items, int count) {
	const int size = count == 0 ? static_cast<int>(items.size()) : count;

	bool action = false;
	*currentItem = std::max(0, std::min(*currentItem, static_cast<const int&>(items.size())));
	const std::string previewText = items.empty() ? "" : items.at(*currentItem);
	if (ImGui::BeginCombo(label, previewText.c_str())) {
		for (int i = 0; i < size; ++i) {
			bool isSelected = i == *currentItem;
			if (ImGui::Selectable(items.at(i).c_str(), &isSelected)) {
				*currentItem = i;
				action = true;
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	return action;
}

bool utils::Combo(const char *label, boost::uuids::uuid *currentItem, int currentElement, int size,
                  const std::function<std::string&(int)> &getName,
                  const std::function<boost::uuids::uuid(int)> &getUuid) {
	bool action = false;

	if (currentElement == -1)
		*currentItem = boost::uuids::uuid{};

	const std::string previewText = currentItem->is_nil() ? "" : getName(currentElement);
	if (ImGui::BeginCombo(label, previewText.c_str())) {
		for (int i = 0; i < size; ++i) {
			bool isSelected = i == currentElement;
			if (ImGui::Selectable(getName(i).c_str(), &isSelected)) {
				*currentItem = getUuid(i);
				action = true;
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	return action;
}

bool utils::ToggleButton(const char *strId, bool *v) {
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList *drawList = ImGui::GetWindowDrawList();

	const float height = ImGui::GetTextLineHeightWithSpacing();
	const float width = height * 1.2f;
	const float RADius = height * 0.5f;

	ImGui::InvisibleButton(strId, ImVec2(width + RADius, height));
	bool click = false;
	if (ImGui::IsItemClicked()) {
		*v = !*v;
		click = true;
	}

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext &g = *GImGui;
	if (g.LastActiveId == g.CurrentWindow->GetID(strId)) // && g.LastActiveIdTimer < ANIM_SPEED)
	{
		constexpr float ANIM_SPEED = 0.08f;
		const float tAnim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (tAnim) : (1.0f - tAnim);
	}

	const ImU32 colBg = ImGui::GetColorU32(ImLerp(ImVec4(0.392f, 0.392f, 0.392f, 0.706f),
	                                              ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered], t));
	const ImU32 colCirc = ImGui::GetColorU32(ImLerp(ImVec4(0.588f, 0.588f, 0.588f, 1.0f),
	                                                ImGui::GetStyle().Colors[ImGuiCol_ButtonActive], t));

	drawList->AddRectFilled(ImVec2(p.x + (RADius * 0.65f), p.y + (height / 6.0f)),
	                        ImVec2(p.x + (RADius * 0.65f) + width, p.y + (height - (height / 6.0f))), colBg, 10.0f);
	drawList->AddCircleFilled(ImVec2(p.x + (RADius * 1.3f) + t * (width - (RADius * 1.3f)), p.y + RADius),
	                          RADius - 1.0f, colCirc);

	ImGui::SameLine();
	ImGui::Text(strId);
	return click;
}


bool utils::LoadTextureFromFile(const std::filesystem::path filename, PDIRECT3DTEXTURE9 *out_texture, int *out_width,
                                int *out_height) {
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	const HRESULT hr = D3DXCreateTextureFromFileExW(GetD3DDevice<IDirect3DDevice9>(), filename.wstring().c_str(),
	                                                D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0,
	                                                D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
	                                                nullptr, nullptr, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = static_cast<int>(my_image_desc.Width);
	*out_height = static_cast<int>(my_image_desc.Height);
	return true;
}

std::optional<std::unique_ptr<Texture>> utils::LoadTextureRequiredFromFile(const std::filesystem::path filename) {
	int imageWidth = 0;
	int imageHeight = 0;
	PDIRECT3DTEXTURE9 texture = nullptr;
	if (LoadTextureFromFile(filename, &texture, &imageWidth, &imageHeight)) {
		return std::make_unique<Texture>(texture, imageWidth, imageHeight);
	}
	return std::nullopt;
}

bool utils::LoadTextureFromMemory(const void *pointer, unsigned int size, PDIRECT3DTEXTURE9 *out_texture,
                                  int *out_width, int *out_height) {
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	const HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(GetD3DDevice<IDirect3DDevice9>(), pointer, size,
	                                                       D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0,
	                                                       D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
	                                                       0, nullptr, nullptr, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = static_cast<int>(my_image_desc.Width);
	*out_height = static_cast<int>(my_image_desc.Height);
	return true;
}

std::optional<std::unique_ptr<Texture>> utils::LoadTextureRequiredFromMemory(const void *pointer, unsigned int size) {
	int imageWidth = 0;
	int imageHeight = 0;
	PDIRECT3DTEXTURE9 texture = nullptr;
	if (LoadTextureFromMemory(pointer, size, &texture, &imageWidth, &imageHeight)) {
		return std::make_unique<Texture>(texture, imageWidth, imageHeight);
	}
	return std::nullopt;
}


std::optional<std::unique_ptr<Texture>> utils::LoadTextureRequiredFromURL(const std::string &url) {
	return std::nullopt;
}

float cameraZoom = 1.0f;
float cameraAngle[3] = {0.0f, 0.0f, 0.0f};

void utils::controlCamera(const CVector &position) {
	using namespace plugin;

	const bool isWindow = ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered();

	float cx = position.x, cy = position.y, cz = position.z;

	cx += cameraZoom * sin(RAD(cameraAngle[0])) * cos(RAD(cameraAngle[1]));
	cy += cameraZoom * cos(RAD(cameraAngle[0])) * cos(RAD(cameraAngle[1]));
	cz += cameraZoom * sin(RAD(cameraAngle[1]));

	const auto fixedModeSource = CVector(cx, cy, cz);
	const auto up = CVector(0.0f, 0.0f, 0.0f);
	TheCamera.SetCamPositionForFixedMode(&fixedModeSource, &up);
	TheCamera.TakeControlNoEntity(&position, 2, 1);

	if (!isWindow) {
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
			const ImVec2 dt = ImGui::GetIO().MouseDelta;
			cameraAngle[0] += dt.x;
			cameraAngle[1] += dt.y;
		}
		if (ImGui::GetIO().MouseWheel != 0.0f && KeyPressed(VK_SHIFT)) {
			cameraZoom += cameraZoom * ImGui::GetIO().MouseWheel / 4.0f;
			if (cameraZoom < 1.0f)
				cameraZoom = 1.0f;
		}
	}
}

bool utils::controlCameraWithMove(float *pos, bool withZ, float zOffset) {
	using namespace plugin;

	controlCamera({pos[0], pos[1], pos[2] + zOffset});

	bool changed = false;

	if (KeyPressed(VK_UP)) {
		pos[0] -= 0.01f * cameraZoom * sin(RAD(cameraAngle[0]));
		pos[1] -= 0.01f * cameraZoom * cos(RAD(cameraAngle[0]));
		changed = true;
	}
	if (KeyPressed(VK_DOWN)) {
		pos[0] += 0.01f * cameraZoom * sin(RAD(cameraAngle[0]));
		pos[1] += 0.01f * cameraZoom * cos(RAD(cameraAngle[0]));
		changed = true;
	}

	if (KeyPressed(VK_LEFT)) {
		pos[0] += 0.01f * cameraZoom * sin(RAD(cameraAngle[0] + 90.0f));
		pos[1] += 0.01f * cameraZoom * cos(RAD(cameraAngle[0] + 90.0f));
		changed = true;
	}
	if (KeyPressed(VK_RIGHT)) {
		pos[0] += 0.01f * cameraZoom * sin(RAD(cameraAngle[0] - 90.0f));
		pos[1] += 0.01f * cameraZoom * cos(RAD(cameraAngle[0] - 90.0f));
		changed = true;
	}

	if (withZ) {
		if (KeyPressed(VK_Q)) {
			pos[2] += 0.01f * cameraZoom;
			changed = true;
		}
		if (KeyPressed(VK_E)) {
			pos[2] -= 0.01f * cameraZoom;
			changed = true;
		}
	}

	return changed;
}

wchar_t canceledSymbols[] = {'<', '>', ':', '\"', '/', '\\', '|', '?', '*'};

std::wstring utils::stringToPathString(const std::string &string) {
	auto copy = utf16FromUtf8(string);
	for (wchar_t canceledSymbol : canceledSymbols)
		std::erase(copy, canceledSymbol);
	trim(copy);
	if (copy.empty())
		copy += utf16FromUtf8(std::to_string(time(nullptr)));

	return copy;
}

unsigned utils::getCarColorRgba(unsigned char id) {
	const unsigned int colTable = memsafe::read<unsigned int>(reinterpret_cast<void*>(0x4C8390));
	const unsigned int clr = memsafe::read<unsigned int>(reinterpret_cast<void*>(colTable + (id * 4)));
	return clr;
}

int utils::createBlip(float *pos, int blipType, int blipColor, int blipSprite) {
	using namespace plugin;

	int blip;
	if (blipType == 0) {
		Command<Commands::ADD_BLIP_FOR_COORD>(pos[0], pos[1], pos[2], &blip);
		if (blipColor != 6) {
			CRadar::ChangeBlipColour(blip, blipColor - 1);
		} else {
			CRadar::SetBlipFriendly(blip, 1);
		}
	} else {
		Command<Commands::ADD_SPRITE_BLIP_FOR_COORD>(pos[0], pos[1], pos[2], blipSprite, &blip);
	}

	return blip;
}

//extensions with dot - .mp3, .exe
std::vector<std::string> utils::getFilenameList(const std::filesystem::path &path,
                                                const std::vector<std::string> &extensions) {
	std::vector<std::string> filesNames;

	if (!path.empty() && exists(path)) {
		for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
			if (entry.is_regular_file()) {
				if (auto finedFile = entry.path(); std::ranges::find(extensions, finedFile.extension().string()) !=
					extensions.end()) {
					filesNames.emplace_back(wstrToUtf8Str(finedFile.filename().wstring()));
				}
			}
		}
	}

	return filesNames;
}

std::string utils::floatArrayColorToHex(const std::array<float, 4> &color) {
	int r = static_cast<int>(color[0] * 255);
	int g = static_cast<int>(color[1] * 255);
	int b = static_cast<int>(color[2] * 255);
	int a = static_cast<int>(color[3] * 255);

	r = std::clamp(r, 0, 255);
	g = std::clamp(g, 0, 255);
	b = std::clamp(b, 0, 255);
	a = std::clamp(a, 0, 255);

	std::stringstream stream;
	stream << std::hex << std::setw(2) << std::setfill('0') << r
		<< std::hex << std::setw(2) << std::setfill('0') << g
		<< std::hex << std::setw(2) << std::setfill('0') << b
		<< std::hex << std::setw(2) << std::setfill('0') << a;

	return "#" + stream.str();
}

std::array<float, 4> utils::hexToFloatArrayColor(const std::string &hexColor) {
	const std::string hexValue = (hexColor[0] == '#') ? hexColor.substr(1) : hexColor;

	if (hexValue.size() != 8) {
		throw std::invalid_argument("Invalid HEX color string");
	}

	const float r = static_cast<float>(std::stoul(hexValue.substr(0, 2), nullptr, 16)) / 255.0f;
	const float g = static_cast<float>(std::stoul(hexValue.substr(2, 2), nullptr, 16)) / 255.0f;
	const float b = static_cast<float>(std::stoul(hexValue.substr(4, 2), nullptr, 16)) / 255.0f;
	const float a = static_cast<float>(std::stoul(hexValue.substr(6, 2), nullptr, 16)) / 255.0f;

	return {r, g, b, a};
}

bool utils::getScreenPositionFromGamePosition(const CVector &position, ImVec2 &out) {
	RwV3d point;
	float w, h;
	const bool isVisible = CSprite::CalcScreenCoors(
		RwV3d{position.x, position.y, position.z}, &point, &w,
		&h, true, true);
	out = ImVec2(point.x, point.y);
	return isVisible;
}
