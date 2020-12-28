#include "ScriptManager.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

#include "imgui.h"
#include "NodeGraph.h"
#include "libs/ini.h"
#include "imnodes.h"
//#include "Init.h"
#include "libs/coro_wait.h"
#include "libs/ScriptCommands.h"


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

extern coro_wait instance;
void addThread(sol::function func)
{
	instance.add_to_queue(func);
}

const char* getStrPtr(std::string& str)
{
	return str.c_str();
}

extern std::vector<const char*> namesStorylineCheckpoints;
extern std::vector<const char*> namesTargets;
extern std::vector<const char*> namesCars;
extern std::vector<const char*> namesActors;
extern std::vector<const char*> namesObjects;
extern std::vector<const char*> namesParticles;
extern std::vector<const char*> namesPickups;
extern std::vector<const char*> namesExplosions;
extern std::vector<const char*> namesAudios;
extern std::vector<const char*> namesStorylineCheckpoints;
extern void addLDYOMClasses(sol::state& lua);

void setNamespaceLua(sol::state& lua)
{
	lua.set_function("print", &printLog);

	lua["callOpcode"] = &pluginmy::scripting::CallCommandByIdPtr;
	lua["getPtrStr"] = &getStrPtr;
	lua["addThread"] = &addThread;

	auto nodeGraph_lua = lua.new_usertype<NodeGraph>("NodeGraph");
	nodeGraph_lua.set("nodes", &NodeGraph::nodes);
	nodeGraph_lua.set("links", &NodeGraph::links);
	
	//ldyom
	sol::table t_ldyom = lua["ldyom"].get_or_create<sol::table>();
	t_ldyom.set_function("langt", langt);
	t_ldyom.set_function("getStorylineMissionsNames", getStorylineMissionsNames);
	t_ldyom["namesStorylineCheckpoints"] = &namesStorylineCheckpoints;
	t_ldyom["namesTargets"] = &namesTargets;
	t_ldyom["namesCars"] = &namesCars;
	t_ldyom["namesActors"] = &namesActors;
	t_ldyom["namesObjects"] = &namesObjects;
	t_ldyom["namesParticles"] = &namesParticles;
	t_ldyom["namesPickups"] = &namesPickups;
	t_ldyom["namesExplosions"] = &namesExplosions;
	t_ldyom["namesAudios"] = &namesAudios;
	t_ldyom["namesStorylineCheckpoints"] = &namesStorylineCheckpoints;

	addLDYOMClasses(lua);
	
	//ldyom.nodeEditor
	t_ldyom["nodeEditor"].get_or_create<sol::table>();
	t_ldyom["nodeEditor"]["addNodeClass"] = NodeGraph::addNodeClass;
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

	t_imgui.set_function("BeginHorizontalStr", sol::resolve<const char*, const ImVec2&, float>(ImGui::BeginHorizontal));
	t_imgui.set_function("BeginHorizontalVoid", sol::resolve<const void*, const ImVec2&, float>(ImGui::BeginHorizontal));
	t_imgui.set_function("BeginHorizontal", sol::resolve<int, const ImVec2&, float>(ImGui::BeginHorizontal));
	t_imgui.set_function("EndHorizontal", ImGui::EndHorizontal);
	t_imgui.set_function("BeginVerticalStr", sol::resolve<const char*, const ImVec2&, float>(ImGui::BeginVertical));
	t_imgui.set_function("BeginVerticalVoid", sol::resolve<const void*, const ImVec2&, float>(ImGui::BeginVertical));
	t_imgui.set_function("BeginVertical", sol::resolve<int, const ImVec2&, float>(ImGui::BeginVertical));
	t_imgui.set_function("EndVertical", ImGui::EndVertical);
	t_imgui.set_function("Spring", ImGui::Spring);
	t_imgui.set_function("SuspendLayout", ImGui::SuspendLayout);
	t_imgui.set_function("ResumeLayout", ImGui::ResumeLayout);

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
	t_imgui.set_function("CheckboxFlags", ImGui::CheckboxFlags);
	t_imgui.set_function("RadioButton", sol::resolve<const char*, bool>(ImGui::RadioButton));
	t_imgui.set_function("RadioButtonP", sol::resolve<const char*, int*, int>(ImGui::RadioButton));
	t_imgui.set_function("ProgressBar", ImGui::ProgressBar);
	t_imgui.set_function("Bullet", ImGui::Bullet);

	t_imgui.set_function("BeginCombo", ImGui::BeginCombo);
	t_imgui.set_function("EndCombo", ImGui::EndCombo);
	//t_imgui.set_function("Combo", sol::resolve<const char*, int*, const char*, int>(ImGui::Combo));
	t_imgui.set_function("ComboVecStr", ComboVecStr);
	t_imgui.set_function("ComboVecChars", ComboVecChars);

	t_imgui.set_function("DragFloat", ImGui::DragFloat);
	t_imgui.set_function("DragFloat2", ImGui::DragFloat2);
	t_imgui.set_function("DragFloat3", ImGui::DragFloat3);
	t_imgui.set_function("DragFloat4", ImGui::DragFloat4);
	t_imgui.set_function("DragFloatRange2", ImGui::DragFloatRange2);
	t_imgui.set_function("DragInt", ImGui::DragInt);
	t_imgui.set_function("DragInt2", ImGui::DragInt2);
	t_imgui.set_function("DragInt3", ImGui::DragInt3);
	t_imgui.set_function("DragInt4", ImGui::DragInt4);
	t_imgui.set_function("DragIntRange2", ImGui::DragIntRange2);
	t_imgui.set_function("DragScalar", ImGui::DragScalar);
	t_imgui.set_function("DragScalarN", ImGui::DragScalarN);

	t_imgui.set_function("SliderFloat", ImGui::SliderFloat);
	t_imgui.set_function("SliderFloat2", ImGui::SliderFloat2);
	t_imgui.set_function("SliderFloat3", ImGui::SliderFloat3);
	t_imgui.set_function("SliderFloat4", ImGui::SliderFloat4);
	t_imgui.set_function("SliderAngle", ImGui::SliderAngle);
	t_imgui.set_function("SliderInt", ImGui::SliderInt);
	t_imgui.set_function("SliderInt2", ImGui::SliderInt2);
	t_imgui.set_function("SliderInt3", ImGui::SliderInt3);
	t_imgui.set_function("SliderInt4", ImGui::SliderInt4);
	t_imgui.set_function("SliderScalar", ImGui::SliderScalar);
	t_imgui.set_function("SliderScalarN", ImGui::SliderScalarN);
	t_imgui.set_function("VSliderFloat", ImGui::VSliderFloat);
	t_imgui.set_function("VSliderInt", ImGui::VSliderInt);
	t_imgui.set_function("VSliderScalar", ImGui::VSliderScalar);

	t_imgui.set_function("InputText", ImGui::InputText);
	t_imgui.set_function("InputTextMultiline", ImGui::InputTextMultiline);
	t_imgui.set_function("InputTextWithHint", ImGui::InputTextWithHint);
	//t_imgui.set_function("InputFloat", sol::resolve<const char*, float*, float, float, const char*, ImGuiInputTextFlags>(ImGui::InputFloat));
	//t_imgui.set_function("InputFloat2", sol::resolve<const char*, float[2], const char*, ImGuiInputTextFlags>(ImGui::InputFloat2));
	//t_imgui.set_function("InputFloat3", sol::resolve<const char*, float[3], const char*, ImGuiInputTextFlags>(ImGui::InputFloat3));
	//t_imgui.set_function("InputFloat4", sol::resolve<const char*, float[4], const char*, ImGuiInputTextFlags>(ImGui::InputFloat4));
	t_imgui.set_function("InputInt", ImGui::InputInt);
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

	//imnodes
	sol::table t_imnodes = t_imgui["imnodes"].get_or_create<sol::table>();

	t_imnodes.set_function("PushColorStyle", imnodes::PushColorStyle);
	t_imnodes.set_function("PopColorStyle", imnodes::PopColorStyle);
	t_imnodes.set_function("PushStyleVar", imnodes::PushStyleVar);
	t_imnodes.set_function("PopStyleVar", imnodes::PopStyleVar);
	
	t_imnodes.set_function("BeginNodeDef", imnodes::BeginNode);
	t_imnodes.set_function("EndNodeDef", imnodes::EndNode);
	t_imnodes.set_function("BeginNode", NodeGraph::BeginNode);
	t_imnodes.set_function("EndNode", NodeGraph::EndNode);
	
	t_imnodes.set_function("GetNodeDimensions", imnodes::GetNodeDimensions);
	
	t_imnodes.set_function("BeginNodeTitleBar", imnodes::BeginNodeTitleBar);
	t_imnodes.set_function("EndNodeTitleBar", imnodes::EndNodeTitleBar);
	
	t_imnodes.set_function("BeginInputAttributeDef", imnodes::BeginInputAttribute);
	t_imnodes.set_function("EndInputAttributeDef", imnodes::EndInputAttribute);
	t_imnodes.set_function("BeginInputAttribute", NodeGraph::BeginInputAttribute);
	t_imnodes.set_function("EndInputAttribute", NodeGraph::EndInputAttribute);
	
	t_imnodes.set_function("BeginOutputAttributeDef", imnodes::BeginOutputAttribute);
	t_imnodes.set_function("EndOutputAttributeDef", imnodes::EndOutputAttribute);
	t_imnodes.set_function("BeginOutputAttribute", NodeGraph::BeginOutputAttribute);
	t_imnodes.set_function("EndOutputAttribute", NodeGraph::EndOutputAttribute);
	
	t_imnodes.set_function("BeginStaticAttribute", imnodes::BeginStaticAttribute);
	t_imnodes.set_function("EndStaticAttribute", imnodes::EndStaticAttribute);
	
	t_imnodes.set_function("PushAttributeFlag", imnodes::PushAttributeFlag);
	t_imnodes.set_function("PopAttributeFlag", imnodes::PopAttributeFlag);
	
	t_imnodes.set_function("Link", imnodes::Link);
	
	t_imnodes.set_function("SetNodeDraggable", imnodes::SetNodeDraggable);
	
	t_imnodes.set_function("SetNodeScreenSpacePos", imnodes::SetNodeScreenSpacePos);
	t_imnodes.set_function("SetNodeEditorSpacePos", imnodes::SetNodeEditorSpacePos);
	t_imnodes.set_function("SetNodeGridSpacePos", imnodes::SetNodeGridSpacePos);
	
	t_imnodes.set_function("GetNodeScreenSpacePos", imnodes::GetNodeScreenSpacePos);
	t_imnodes.set_function("GetNodeEditorSpacePos", imnodes::GetNodeEditorSpacePos);
	t_imnodes.set_function("GetNodeGridSpacePos", imnodes::GetNodeGridSpacePos);
	
	t_imnodes.set_function("IsEditorHovered", imnodes::IsEditorHovered);
	
	t_imnodes.set_function("IsNodeHovered", imnodes::IsNodeHovered);
	t_imnodes.set_function("IsLinkHovered", imnodes::IsLinkHovered);
	t_imnodes.set_function("IsPinHovered", imnodes::IsPinHovered);
	
	t_imnodes.set_function("NumSelectedNodes", imnodes::NumSelectedNodes);
	t_imnodes.set_function("NumSelectedLinks", imnodes::NumSelectedLinks);
	
	t_imnodes.set_function("GetSelectedNodes", imnodes::GetSelectedNodes);
	t_imnodes.set_function("GetSelectedLinks", imnodes::GetSelectedLinks);
	
	t_imnodes.set_function("ClearNodeSelection", imnodes::ClearNodeSelection);
	t_imnodes.set_function("ClearLinkSelection", imnodes::ClearLinkSelection);
	
	t_imnodes.set_function("IsAttributeActive", imnodes::IsAttributeActive);
	t_imnodes.set_function("IsAnyAttributeActive", imnodes::IsAnyAttributeActive);
	
	t_imnodes.set_function("IsLinkStarted", imnodes::IsLinkStarted);
	t_imnodes.set_function("IsLinkDropped", imnodes::IsLinkDropped);
	t_imnodes.set_function("IsLinkCreated", sol::resolve<int*, int*, bool*>(imnodes::IsLinkCreated));
	
	t_imnodes.set_function("IsLinkDestroyed", imnodes::IsLinkDestroyed);
	t_imnodes.set_function("IsLinkDestroyed", imnodes::IsLinkDestroyed);

	t_imnodes.set_function("getNodeIcon", NodeGraph::getNodeIcon);
	
	t_imnodes.new_enum<imnodes::PinShape>("PinShape", {
		{"Circle",imnodes::PinShape_Circle},
		{"CircleFilled",imnodes::PinShape_CircleFilled},
		{"Triangle",imnodes::PinShape_Triangle},
		{"TriangleFilled",imnodes::PinShape_TriangleFilled},
		{"Quad",imnodes::PinShape_Quad},
		{"QuadFilled",imnodes::PinShape_QuadFilled},
		});

	t_imnodes["PinType"].get_or_create<sol::table>();
	t_imnodes["PinType"]["number"] = (int)NodeGraph::PinType::number;
	t_imnodes["PinType"]["boolean"] = (int)NodeGraph::PinType::boolean;
	t_imnodes["PinType"]["string"] = (int)NodeGraph::PinType::string;
	t_imnodes["PinType"]["void"] = (int)NodeGraph::PinType::void_;
	
	
	t_imnodes.new_enum<imnodes::ColorStyle>("ColorStyle", {
		{"NodeBackground",imnodes::ColorStyle_NodeBackground},
		{"NodeBackgroundHovered",imnodes::ColorStyle_NodeBackgroundHovered},
		{"NodeBackgroundSelected",imnodes::ColorStyle_NodeBackgroundSelected},
		{"NodeOutline",imnodes::ColorStyle_NodeOutline},
		{"TitleBar",imnodes::ColorStyle_TitleBar},
		{"TitleBarHovered",imnodes::ColorStyle_TitleBarHovered},
		{"TitleBarSelected",imnodes::ColorStyle_TitleBarSelected},
		{"Link",imnodes::ColorStyle_Link},
		{"LinkHovered",imnodes::ColorStyle_LinkHovered},
		{"LinkSelected",imnodes::ColorStyle_LinkSelected},
		{"Pin",imnodes::ColorStyle_Pin},
		{"PinHovered",imnodes::ColorStyle_PinHovered},
		{"BoxSelector",imnodes::ColorStyle_BoxSelector},
		{"BoxSelectorOutline",imnodes::ColorStyle_BoxSelectorOutline},
		{"GridBackground",imnodes::ColorStyle_GridBackground},
		{"GridLine",imnodes::ColorStyle_GridLine},
		{"Count",imnodes::ColorStyle_Count},
		});
}

extern std::map<std::string, std::vector<sol::table>> map_nodes_class;
extern NodeGraph* currentNodeGraphPtr;

void ScriptManager::loadScripts()
{
	std::string path = "LDYOM//Scripts";
	delete currentNodeGraphPtr;
	currentNodeGraphPtr = new NodeGraph;
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
					
					lua.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::utf8, sol::lib::ffi, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::string);
					
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