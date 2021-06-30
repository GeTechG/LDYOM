#pragma once

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
#include "fa.h"

bool pressed_keys[122];

bool KeyJustPressed(unsigned int key) {
	bool now = KeyPressed(key);
	if (!pressed_keys[key] && now) {
		pressed_keys[key] = true;
		return true;
	}
	pressed_keys[key] = KeyPressed(key);
	return false;
}

void startLog() {
	if (boost::filesystem::exists("LDYOM/ldyom.log")) {
		remove("LDYOM/ldyom.log");
	}
}

bool is_utf8(const char* string)
{
	if (!string)
		return false;

	const unsigned char* bytes = (const unsigned char*)string;
	while (*bytes)
	{
		if ((bytes[0] == 0x09 || bytes[0] == 0x0A || bytes[0] == 0x0D ||
			(0x20 <= bytes[0] && bytes[0] <= 0x7E)
			)
			)
		{
			bytes += 1;
			continue;
		}

		if (( // non-overlong 2-byte
			(0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF)
			)
			)
		{
			bytes += 2;
			continue;
		}

		if (( // excluding overlongs
			bytes[0] == 0xE0 &&
			(0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
			( // straight 3-byte
			((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
				bytes[0] == 0xEE ||
				bytes[0] == 0xEF) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
				) ||
				( // excluding surrogates
					bytes[0] == 0xED &&
					(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
					(0x80 <= bytes[2] && bytes[2] <= 0xBF)
					)
			)
		{
			bytes += 3;
			continue;
		}

		if (( // planes 1-3
			bytes[0] == 0xF0 &&
			(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
			( // planes 4-15
			(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
				) ||
				( // plane 16
					bytes[0] == 0xF4 &&
					(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
					(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
					(0x80 <= bytes[3] && bytes[3] <= 0xBF)
					)
			)
		{
			bytes += 4;
			continue;
		}

		return false;
	}

	return true;
}

std::string UTF8_to_CP1251(std::string const& utf8)
{
	if (!utf8.empty())
	{
		if (!is_utf8(utf8.c_str()))
		{
			return utf8;
		}
		int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
		if (wchlen > 0 && wchlen != 0xFFFD)
		{
			std::vector<wchar_t> wbuf(wchlen);
			MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
			std::vector<char> buf(wchlen);
			WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, 0, 0);

			return std::string(&buf[0], wchlen);
		}
	}
	return std::string();
}

void printLog(std::string print_text) {
	ofstream logg("LDYOM/ldyom.log", ios_base::app);
	print_text.append("\n");
	//logg << UTF8_to_CP1251(print_text);
	logg << print_text;
	logg.close();

}

std::string cp1251_to_utf8(const char *str)
{
	std::string res;
	WCHAR *ures = NULL;
	char *cres = NULL;

	int result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
	if (result_u != 0)
	{
		ures = new WCHAR[result_u];
		if (MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
		{
			int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
			if (result_c != 0)
			{
				cres = new char[result_c];
				if (WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
				{
					res = cres;
				}
			}
		}
	}

	delete[] ures;
	delete[] cres;

	return res;
}

std::vector<std::string> get_filename_list(const std::string& path) {
	std::vector <std::string> m_file_list;
	if (!path.empty()) {
		boost::filesystem::path apk_path(path);
		boost::filesystem::directory_iterator iter(apk_path), eod;

		BOOST_FOREACH(boost::filesystem::path const& i, make_pair(iter, eod)) {
			if (is_regular_file(i)) {
				m_file_list.push_back(cp1251_to_utf8(i.stem().string().c_str()));
			}
		}
	}
	return m_file_list;
}

std::vector<std::string> get_filename_list(const std::string& path, const std::string& extension) {
	std::vector <std::string> m_file_list;
	if (!path.empty()) {
		boost::filesystem::path apk_path(path);
		boost::filesystem::directory_iterator iter(apk_path), eod;

		BOOST_FOREACH(boost::filesystem::path const& i, make_pair(iter, eod)) {
			if (is_regular_file(i)) {
				if (i.extension() == extension) {
					m_file_list.push_back(cp1251_to_utf8(i.stem().string().c_str()));
				}
			}
		}
	}
	return m_file_list;
}

const char* langt(const std::string& key) {
	if (lang_file.size() == 0) {
		std::string pathh = "LDYOM/Language/";
		pathh.append(curr_lang_string);
		pathh.append(".ini");
		mINI::INIFile file(pathh);
		assert(file.read(lang_file));
	}
	return lang_file["Keys"][key].c_str();
}

template <typename T>
std::vector<T> parseJsonArray(std::string data) {
	std::vector<T> arr;
	boost::property_tree::ptree data_ptree;
	std::stringstream data_strsream;
	data_strsream << data;
	boost::property_tree::read_json(data_strsream, data_ptree);
	for (boost::property_tree::ptree::value_type &v : data_ptree) {
		arr.push_back(static_cast<T>(v.second.data()));
	}
	return arr;
}

const char sym_ru[67] = "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ";
const char sym_sl[67] = "A€‹‚ƒEE„ˆ…†K‡–­OŒPCYX‰Š‘’“”•a—¢™šee›Ÿœk¯®o£pc¦y˜x ¤¥¡§¨©ª«¬";
void GXTEncode(std::string& str) {
	for (int i = 0; i < 67; i++) {
		std::replace(str.begin(), str.end(), sym_ru[i], sym_sl[i]);
	}
}



template <typename T>
void moveCellVector(std::vector<T>* vec, int* in, int* out) {
	std::vector<T> new_vec(vec->size());
	int  j = 0;
	for (int i = 0; i < vec->size(); i++) {
		if (i != *in) {
			new_vec.at(j) = vec->at(i);
			j++;
		}
		if (i == *out) {
			new_vec.at(j) = vec->at(*in);
			j++;
		}
	}
	vec->assign(new_vec.begin(), new_vec.end());
}

bool Combo(const char* label, int& current_item, vector<std::string>* items) {
	current_item = clamp(current_item, 0, (int)items->size());
	if (ImGui::BeginCombo(label, items->empty() ? "" : items->at(current_item).c_str())) {
		for (int i = 0; i < items->size(); i++) {
			const bool is_selected = (i == current_item);
			ImGui::PushID(i);
			if (ImGui::Selectable(items->at(i).c_str(), is_selected)) {
				current_item = i;
				ImGui::PopID();
				ImGui::EndCombo();
				return true;
			}
			ImGui::PopID();
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	return false;
}

bool Combo(const char* label, int& current_item, vector<const char*>* items) {
	current_item = clamp(current_item, 0, (int)items->size());
	if (ImGui::BeginCombo(label, items->empty() ? "" : items->at(current_item))) {
		for (int i = 0; i < items->size(); i++) {
			const bool is_selected = (i == current_item);
			ImGui::PushID(i);
			if (ImGui::Selectable(items->at(i), is_selected)) {
				current_item = i;
				ImGui::PopID();
				ImGui::EndCombo();
				return true;
			}
			ImGui::PopID();
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	return false;
}

bool ListBox(const char* label, int& current_item, vector<const char*>* items) {
	if (ImGui::ListBoxHeader("", items->size(), 15)) {
		for (unsigned int i = 0; i < items->size(); i++) {
			ImGui::PushID(i);
			if (ImGui::Selectable(items->at(i), i == current_item)) {
				current_item = i;
				ImGui::PopID();
				ImGui::ListBoxFooter();
				return true;
			}
			ImGui::PopID();
		}
		ImGui::ListBoxFooter();
	}
	return false;
}

bool ListBox(const char* label, int& current_item, vector<std::string>* items) {
	if (ImGui::ListBoxHeader("", items->size(), 15)) {
		for (unsigned int i = 0; i < items->size(); i++) {
			ImGui::PushID(i);
			if (ImGui::Selectable(items->at(i).c_str(), i == current_item)) {
				current_item = i;
				ImGui::PopID();
				ImGui::ListBoxFooter();
				return true;
			}
			ImGui::PopID();
		}
		ImGui::ListBoxFooter();
	}
	return false;
}

inline double rad(float degrees) {
	static const double pi_on_180 = 4.0 * atan(1.0) / 180.0;
	return degrees * pi_on_180;
}

inline double deg(float radians) {
	static const double v180_on_pi = 180.0 / (4.0 * atan(1.0));
	return radians * v180_on_pi;
}

bool ToggleButton(const char* str_id, bool* v) {
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetTextLineHeightWithSpacing();
	float width = height * 1.2f;
	float radius = height * 0.5f;

	ImGui::InvisibleButton(str_id, ImVec2(width + radius, height));
	bool click = false;
	if (ImGui::IsItemClicked()) {
		*v = !*v;
		click = true;
	}

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	float ANIM_SPEED = 0.08f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.392f, 0.392f, 0.392f, 0.706f), ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered], t));
	ImU32 col_circ = ImGui::GetColorU32(ImLerp(ImVec4(0.588f, 0.588f, 0.588f, 1.0f), ImGui::GetStyle().Colors[ImGuiCol_ButtonActive], t));

	draw_list->AddRectFilled(ImVec2(p.x + (radius * 0.65f), p.y + (height / 6.0f)), ImVec2(p.x + (radius * 0.65f) + width, p.y + (height - (height / 6.0f))), col_bg, 10.0f);
	draw_list->AddCircleFilled(ImVec2(p.x + (radius * 1.3f) + t * (width - (radius * 1.3f)), p.y + radius), radius - 1.0f, col_circ);

	ImGui::SameLine();
	ImGui::Text(str_id);
	return click;
}

bool LoadTextureFromResource(WORD IdResource, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromResourceA(GetD3DDevice(), hhModule, MAKEINTRESOURCE(IdResource), &texture);
	printLog(std::to_string(hr));
	if (hr != S_OK)
		return false;
	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	if (out_width != nullptr)
		*out_width = (int)my_image_desc.Width;
	if (out_width != nullptr)
		*out_height = (int)my_image_desc.Height;
	return true;
}

bool LoadTextureFromFile(std::string path, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(GetD3DDevice(), path.c_str(), &texture);
	printLog(std::to_string(hr));
	if (hr != S_OK)
		return false;
	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	if (out_width != nullptr)
		*out_width = (int)my_image_desc.Width;
	if (out_width != nullptr)
		*out_height = (int)my_image_desc.Height;
	return true;
}

unsigned int rgba_to_int(int r, int g, int b, int a) {
	int argb = b;  // b
	argb = argb | g << 8;  //g
	argb = argb | r << 16; //r
	argb = argb | a << 24; //a
	return argb;
}

unsigned int get_car_color_rgba(unsigned char id) {
	unsigned int col_table = memsafe::read<unsigned int>((void*)0x4C8390);
	unsigned int clr = memsafe::read<unsigned int>((void*)(col_table + (id * 4)));
	return clr;
}

std::string replace_symb(std::string& str) {
	char symb[9] = { '\\','/',':','*','?','\"','<','>','|' };
	for (char ch : symb) {
		std::replace(str.begin(), str.end(), ch, ' ');
	}
	return str;
}

struct InputTextCallback_UserData
{
	std::string*            Str;
	ImGuiInputTextCallback  ChainCallback;
	void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
		std::string* str = user_data->Str;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
	}
	else if (user_data->ChainCallback)
	{
		// Forward to user callback, if any
		data->UserData = user_data->ChainCallbackUserData;
		return user_data->ChainCallback(data);
	}
	return 0;
}

bool InputText(const char* label, size_t length, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
	flags |= ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = str;
	cb_user_data.ChainCallback = callback;
	cb_user_data.ChainCallbackUserData = user_data;
	return ImGui::InputText(label, (char*)str->c_str(), length == 0? str->capacity() + 1 : length, flags, InputTextCallback, &cb_user_data);
}

void rotateVec2(float& x, float& y, float angle) {
	float x1, y1;
	x1 = x * static_cast<float>(cos(rad(angle))) - y * static_cast<float>(sin(rad(angle)));
	y1 = x * static_cast<float>(sin(rad(angle))) + y * static_cast<float>(cos(rad(angle)));
	x = x1;
	y = y1;
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled(ICON_FA_QUESTION_CIRCLE);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::Text(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
