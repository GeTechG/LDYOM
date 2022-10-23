#include "ImGuiLuaWrapper.h"

#include "imgui.h"
#include "utils.h"

void ImGuiLuaWrapper::wrap(sol::state& state) {
	state.set_function("IM_COL32", [](const int r, const int g, const int b, const int a) {
		return IM_COL32(r, g, b, a);
	});

	auto table = state.create_table("ImGui");
	table.set_function("text", [](const char* text) {
		ImGui::Text(text);
	});
	table.set_function("sameLine", sol::overload([]() {
		ImGui::SameLine();
	}, [](float offset) {
		ImGui::SameLine(offset);
	}));
	table.set_function("getItemRectSize", ImGui::GetItemRectSize);
	table.set_function("getFontSize", ImGui::GetFontSize);
	table.set_function("getMousePos", []() {
		auto mousePos = ImGui::GetMousePos();
		return std::make_tuple(mousePos.x, mousePos.y);
	});
	table.set_function("inputText", [](const char* name, const sol::object& buf, const int sizeBuf) {
		return ImGui::InputText(name, static_cast<char*>(const_cast<void*>(buf.pointer())), sizeBuf);
	});
	table.set_function("inputTextMultiline", [](const char* name, const sol::object& buf, const int sizeBuf, const float width, const float height) {
		return ImGui::InputTextMultiline(name, static_cast<char*>(const_cast<void*>(buf.pointer())), sizeBuf, ImVec2(width, height));
	});
	table.set_function("setNextItemWidth", [](const float itemWidth) {
		ImGui::SetNextItemWidth(itemWidth);
	});
	table.set_function("getStyleColorVec4", [](const int col) {
		return ImGui::GetStyleColorVec4(col);
	});
	table.set_function("beginGroup", []() {
		ImGui::BeginGroup();
	});
	table.set_function("endGroup", []() {
		ImGui::EndGroup();
	});
	table.set_function("inputFloat", [](const char* name, const sol::object& value) {
		ImGui::InputFloat(name, static_cast<float*>(const_cast<void*>(value.pointer())));
	});
	table.set_function("checkbox", [](const char* name, const sol::object& value) {
		ImGui::Checkbox(name, static_cast<bool*>(const_cast<void*>(value.pointer())));
	});
	table.set_function("combo", [](const char* name, const sol::object& value, const std::vector<std::string>* arr) {
		int v = static_cast<int>(*static_cast<float*>(const_cast<void*>(value.pointer())));
		if (utils::Combo(name, &v, *arr)) {
			*static_cast<float*>(const_cast<void*>(value.pointer())) = static_cast<float>(v);
		}
	});
	table.set_function("dummy", [](float x, float y) {
		ImGui::Dummy(ImVec2(x, y));
	});
	table.set_function("calcTextSize", [](const char* text) {
		return ImGui::CalcTextSize(text);
	});
	table.set_function("beginTooltip", []() {
		ImGui::BeginTooltip();
	});
	table.set_function("endTooltip", []() {
		ImGui::EndTooltip();
	});
	table.set_function("isItemHovered", []() {
		return ImGui::IsItemHovered();
	});
	table.set_function("openPopup", [](const char* name) {
		ImGui::OpenPopup(name);
	});


	auto ImVec4Lua = state.new_usertype<ImVec4>("ImVec4",
                                     sol::constructors<ImVec4(float, float, float, float)>());
	ImVec4Lua["x"] = &ImVec4::x;
	ImVec4Lua["y"] = &ImVec4::y;
	ImVec4Lua["z"] = &ImVec4::z;
	ImVec4Lua["w"] = &ImVec4::w;
	auto ImVec2Lua = state.new_usertype<ImVec2>("ImVec2",
		sol::constructors<ImVec2(float, float)>());
	ImVec2Lua["x"] = &ImVec2::x;
	ImVec2Lua["y"] = &ImVec2::y;
}
