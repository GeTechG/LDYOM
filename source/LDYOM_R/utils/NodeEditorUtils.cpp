#include "NodeEditorUtils.h"

#include "imnodes/imnodes.h"
#include "fmt/core.h"
#include "fa.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "Localization/Localization.h"

void NodeEditorUtils::beginNode(const int id, const char* name, const NodeType& nodeType, const std::optional<std::string> description) {
	const char* icon = getIcon(nodeType);
	unsigned colorTitlebar = 0;
	switch (nodeType) {
	case Function:
		colorTitlebar = IM_COL32(119, 13, 255, 255);
		break;
	case Loops:
		colorTitlebar = IM_COL32(186, 214, 56, 255);
		break;
	case Event:
		colorTitlebar = IM_COL32(11, 115, 36, 255);
		break;
	case Condition:
		colorTitlebar = IM_COL32(255, 26, 0, 255);
		break;
	case Variable:
		colorTitlebar = IM_COL32(71, 116, 115, 255);
		break;
	case Constant:
		colorTitlebar = IM_COL32(176, 83, 2, 255);
		break;
	default:
		break;
	}
	const auto blend = ImGui::GetStyleColorVec4(ImGuiCol_Text) * ImVec4(255.f, 255.f, 255.f, 255.f);
	const auto selectedColor = ImAlphaBlendColors(colorTitlebar, IM_COL32(static_cast<int>(blend.x), static_cast<int>(blend.y), static_cast<int>(blend.z), 20));
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, colorTitlebar);
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, selectedColor);
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, selectedColor);
	ImNodes::BeginNode(id);
	ImNodes::BeginNodeTitleBar();
	ImGui::Text(fmt::format("{} {}", icon, Localization::getInstance().get(name)).c_str());
	if (description.has_value()) {
		ImGui::SameLine();
		ImGui::Text(ICON_FA_INFO_CIRCLE);
		if (ImGui::IsItemHovered()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 15.f);
			ImGui::SetTooltip(Localization::getInstance().get(description.value()).c_str());
			ImGui::PopTextWrapPos();
		}
	}
	ImNodes::EndNodeTitleBar();
}

ImNodesPinShape stylePin(const ValueType& valueType) {

	ImNodesPinShape_ shape;
	switch (valueType) {
	case Void:
		shape = ImNodesPinShape_Triangle;
		break;
	case Number:
		shape = ImNodesPinShape_Circle;
		break;
	case Boolean:
		shape = ImNodesPinShape_Circle;
		break;
	case String:
		shape = ImNodesPinShape_Circle;
		break;
	default:
		shape = ImNodesPinShape_Circle;
		break;
	}
	const auto pinColor = NodeEditorUtils::getColorByType(valueType);
	const auto blend = ImGui::GetStyleColorVec4(ImGuiCol_Text) * ImVec4(255.f, 255.f, 255.f, 255.f);
	ImNodes::PushColorStyle(ImNodesCol_Pin, pinColor);
	ImNodes::PushColorStyle(ImNodesCol_PinHovered, ImAlphaBlendColors(pinColor, IM_COL32(static_cast<int>(blend.x), static_cast<int>(blend.y), static_cast<int>(blend.z), 20)));

	return shape;
}

void NodeEditorUtils::beginInputAttribute(sol::table pin) {
	const ImNodesPinShape shape = stylePin(static_cast<ValueType>(pin["valueType"].get<int>())) + (pin["connected"].get<bool>() ? 1 : 0);
	ImNodes::BeginInputAttribute(pin["id"].get<int>(), shape);
}

void NodeEditorUtils::beginOutputAttribute(sol::table pin) {
	const ImNodesPinShape shape = stylePin(static_cast<ValueType>(pin["valueType"].get<int>())) + (pin["connected"].get<bool>() ? 1 : 0);
	ImNodes::BeginOutputAttribute(pin["id"].get<int>(), shape);
}

void NodeEditorUtils::endInputAttribute() {
	ImNodes::EndInputAttribute();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void NodeEditorUtils::endOutputAttribute() {
	ImNodes::EndOutputAttribute();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void NodeEditorUtils::endNode() {
	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

unsigned NodeEditorUtils::getColorByType(const ValueType& valueType) {
	unsigned pinColor = 0;
	switch (valueType) {
	case Void:
		pinColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Text));
		break;
	case Number:
		pinColor = IM_COL32(36, 159, 222, 255);
		break;
	case Boolean:
		pinColor = IM_COL32(255, 26, 0, 255);
		break;
	case String:
		pinColor = IM_COL32(120, 176, 107, 255);
		break;
	default:
		break;
	}

	return pinColor;
}

const char* NodeEditorUtils::getIcon(const NodeType& nodeType) {
	switch (nodeType) {
	case Function:
		return ICON_FA_FUNCTION;
	case Loops:
		return ICON_FA_REPEAT_ALT;
	case Event:
		return ICON_FA_ARROW_ALT_FROM_LEFT;
	case Condition:
		return ICON_FA_UTENSIL_FORK;
	case Variable:
		return ICON_FA_BOX;
	case Constant:
		return ICON_FA_LOCK;
	default:
		return nullptr;
	}
}

