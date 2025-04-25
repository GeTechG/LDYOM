#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "node_editor/imgui_node_editor.h"
#include "node_editor/blueprint/utilities/drawing.h"
#include "node_editor/blueprint/utilities/widgets.h"

namespace ed = ax::NodeEditor;

#include <sol.hpp>


void nodeEditorWrapper(sol::state &state) {
	auto table = state.create_table("NodeEditor");
	table["SetCurrentEditor"] = [](const sol::object &context) {
		if (context.get_type() == sol::type::lightuserdata) {
			SetCurrentEditor(static_cast<ed::EditorContext*>(context.as<void*>()));
		} else {
			ed::SetCurrentEditor(nullptr);
		}
	};
	table["GetCurrentEditor"] = []() {
		return reinterpret_cast<void*>(ed::GetCurrentEditor());
	};
	table["CreateEditor"] = [](ed::Config *config) {
		return reinterpret_cast<void*>(CreateEditor(config));
	};
	table["DestroyEditor"] = [](void *context) {
		DestroyEditor(static_cast<ed::EditorContext*>(context));
	};
	table["GetConfig"] = [](void *context) {
		return std::cref(GetConfig(static_cast<ed::EditorContext*>(context)));
	};
	table["GetStyle"] = &ed::GetStyle;
	table["GetStyleColorName"] = &ed::GetStyleColorName;
	table["PushStyleColor"] = &ed::PushStyleColor;
	table["PopStyleColor"] = &ed::PopStyleColor;
	table["PushStyleVar"] = sol::overload(
		sol::resolve<void(ed::StyleVar, float)>(&ed::PushStyleVar),
		sol::resolve<void(ed::StyleVar, const ImVec2 &)>(&ed::PushStyleVar),
		sol::resolve<void(ed::StyleVar, const ImVec4 &)>(&ed::PushStyleVar)
	);
	table["PopStyleVar"] = &ed::PopStyleVar;
	table["Begin"] = &ed::Begin;
	table["End"] = &ed::End;
	table["BeginNode"] = &ed::BeginNode;
	table["BeginPin"] = &ed::BeginPin;
	table["PinRect"] = &ed::PinRect;
	table["PinPivotRect"] = &ed::PinPivotRect;
	table["PinPivotSize"] = &ed::PinPivotSize;
	table["PinPivotScale"] = &ed::PinPivotScale;
	table["PinPivotAlignment"] = &ed::PinPivotAlignment;
	table["EndPin"] = &ed::EndPin;
	table["Group"] = &ed::Group;
	table["EndNode"] = &ed::EndNode;
	table["BeginGroupHint"] = &ed::BeginGroupHint;
	table["GetGroupMin"] = &ed::GetGroupMin;
	table["GetGroupMax"] = &ed::GetGroupMax;
	table["GetHintForegroundDrawList"] = &ed::GetHintForegroundDrawList;
	table["GetHintBackgroundDrawList"] = &ed::GetHintBackgroundDrawList;
	table["EndGroupHint"] = &ed::EndGroupHint;
	table["GetNodeBackgroundDrawList"] = &ed::GetNodeBackgroundDrawList;
	table["Link"] = &ed::Link;
	table["Flow"] = &ed::Flow;
	table["BeginCreate"] = &ed::BeginCreate;
	table["QueryNewLink"] = sol::overload(
		sol::resolve<bool(ed::PinId *, ed::PinId *)>(&ed::QueryNewLink),
		sol::resolve<bool(ed::PinId *, ed::PinId *, const ImVec4 &, float)>(&ed::QueryNewLink)
	);
	table["QueryNewNode"] = sol::overload(
		sol::resolve<bool(ed::PinId *)>(&ed::QueryNewNode),
		sol::resolve<bool(ed::PinId *, const ImVec4 &, float)>(&ed::QueryNewNode)
	);
	table["AcceptNewItem"] = sol::overload(
		sol::resolve<bool()>(&ed::AcceptNewItem),
		sol::resolve<bool(const ImVec4 &, float)>(&ed::AcceptNewItem)
	);
	table["RejectNewItem"] = sol::overload(
		sol::resolve<void()>(&ed::RejectNewItem),
		sol::resolve<void(const ImVec4 &, float)>(&ed::RejectNewItem)
	);
	table["EndCreate"] = &ed::EndCreate;
	table["BeginDelete"] = &ed::BeginDelete;
	table["QueryDeletedLink"] = [](ed::LinkId *linkId, const sol::object &startPinId, const sol::object &endPinId) {
		return QueryDeletedLink(linkId, startPinId.get_type() == sol::type::nil ? nullptr : startPinId.as<ed::PinId*>(),
		                        endPinId.get_type() == sol::type::nil ? nullptr : endPinId.as<ed::PinId*>());
	};
	table["QueryDeletedNode"] = &ed::QueryDeletedNode;
	table["AcceptDeletedItem"] = &ed::AcceptDeletedItem;
	table["RejectDeletedItem"] = &ed::RejectDeletedItem;
	table["EndDelete"] = &ed::EndDelete;
	table["SetNodePosition"] = &ed::SetNodePosition;
	table["SetGroupSize"] = &ed::SetGroupSize;
	table["GetNodePosition"] = [](const ed::NodeId id, ImVec2 &pos) {
		pos = GetNodePosition(id);
	};
	table["GetNodeSize"] = &ed::GetNodeSize;
	table["CenterNodeOnScreen"] = &ed::CenterNodeOnScreen;
	table["SetNodeZPosition"] = &ed::SetNodeZPosition;
	table["GetNodeZPosition"] = &ed::GetNodeZPosition;
	table["RestoreNodeState"] = &ed::RestoreNodeState;
	table["Suspend"] = &ed::Suspend;
	table["Resume"] = &ed::Resume;
	table["IsSuspended"] = &ed::IsSuspended;
	table["IsActive"] = &ed::IsActive;
	table["HasSelectionChanged"] = &ed::HasSelectionChanged;
	table["GetSelectedObjectCount"] = &ed::GetSelectedObjectCount;
	table["GetSelectedNodes"] = &ed::GetSelectedNodes;
	table["GetSelectedLinks"] = &ed::GetSelectedLinks;
	table["IsNodeSelected"] = &ed::IsNodeSelected;
	table["IsLinkSelected"] = &ed::IsLinkSelected;
	table["ClearSelection"] = &ed::ClearSelection;
	table["SelectNode"] = &ed::SelectNode;
	table["SelectLink"] = &ed::SelectLink;
	table["DeselectNode"] = &ed::DeselectNode;
	table["DeselectLink"] = &ed::DeselectLink;
	table["DeleteNode"] = &ed::DeleteNode;
	table["DeleteLink"] = &ed::DeleteLink;
	table["HasAnyLinks"] = sol::overload(
		sol::resolve<bool(ed::NodeId)>(&ed::HasAnyLinks),
		sol::resolve<bool(ed::PinId)>(&ed::HasAnyLinks)
	);
	table["BreakLinks"] = sol::overload(
		sol::resolve<int(ed::NodeId)>(&ed::BreakLinks),
		sol::resolve<int(ed::PinId)>(&ed::BreakLinks)
	);
	table["NavigateToContent"] = &ed::NavigateToContent;
	table["NavigateToSelection"] = &ed::NavigateToSelection;
	table["ShowNodeContextMenu"] = &ed::ShowNodeContextMenu;
	table["ShowPinContextMenu"] = &ed::ShowPinContextMenu;
	table["ShowLinkContextMenu"] = &ed::ShowLinkContextMenu;
	table["ShowBackgroundContextMenu"] = &ed::ShowBackgroundContextMenu;
	table["EnableShortcuts"] = &ed::EnableShortcuts;
	table["AreShortcutsEnabled"] = &ed::AreShortcutsEnabled;
	table["BeginShortcut"] = &ed::BeginShortcut;
	table["AcceptCut"] = &ed::AcceptCut;
	table["AcceptCopy"] = &ed::AcceptCopy;
	table["AcceptPaste"] = &ed::AcceptPaste;
	table["AcceptDuplicate"] = &ed::AcceptDuplicate;
	table["AcceptCreateNode"] = &ed::AcceptCreateNode;
	table["GetActionContextSize"] = &ed::GetActionContextSize;
	table["GetActionContextNodes"] = &ed::GetActionContextNodes;
	table["GetActionContextLinks"] = &ed::GetActionContextLinks;
	table["EndShortcut"] = &ed::EndShortcut;
	table["GetCurrentZoom"] = &ed::GetCurrentZoom;
	table["GetHoveredNode"] = &ed::GetHoveredNode;
	table["GetHoveredPin"] = &ed::GetHoveredPin;
	table["GetHoveredLink"] = &ed::GetHoveredLink;
	table["GetDoubleClickedNode"] = &ed::GetDoubleClickedNode;
	table["GetDoubleClickedPin"] = &ed::GetDoubleClickedPin;
	table["GetDoubleClickedLink"] = &ed::GetDoubleClickedLink;
	table["IsBackgroundClicked"] = &ed::IsBackgroundClicked;
	table["IsBackgroundDoubleClicked"] = &ed::IsBackgroundDoubleClicked;
	table["GetBackgroundClickButtonIndex"] = &ed::GetBackgroundClickButtonIndex;
	table["GetBackgroundDoubleClickButtonIndex"] = &ed::GetBackgroundDoubleClickButtonIndex;
	table["GetLinkPins"] = &ed::GetLinkPins;
	table["PinHadAnyLinks"] = &ed::PinHadAnyLinks;
	table["GetScreenSize"] = &ed::GetScreenSize;
	table["ScreenToCanvas"] = &ed::ScreenToCanvas;
	table["CanvasToScreen"] = &ed::CanvasToScreen;
	table["GetNodeCount"] = &ed::GetNodeCount;
	table["GetOrderedNodeIds"] = &ed::GetOrderedNodeIds;

	table["LinkId"] = [](const sol::object &id) {
		if (id.get_type() == sol::type::number) {
			return ed::LinkId(id.as<unsigned>());
		}
		return ed::LinkId();
	};
	table["NodeId"] = [](const sol::object &id) {
		if (id.get_type() == sol::type::number) {
			return ed::NodeId(id.as<unsigned>());
		}
		return ed::NodeId();
	};
	table["PinId"] = [](const sol::object &id) {
		if (id.get_type() == sol::type::number) {
			return ed::PinId(id.as<unsigned>());
		}
		return ed::PinId();
	};

	table["DrawIcon"] = &ax::Drawing::DrawIcon;
	table["Icon"] = &ax::Widgets::Icon;

	state.new_enum<ed::PinKind>("NodeEditorPinKind",
	                            {
		                            {"Input", ed::PinKind::Input},
		                            {"Output", ed::PinKind::Output}
	                            }
	);

	state.new_enum<ed::FlowDirection>("NodeEditorFlowDirection",
	                                  {
		                                  {"Backward", ed::FlowDirection::Backward},
		                                  {"Forward", ed::FlowDirection::Forward},
	                                  }
	);

	state.new_enum<ax::Drawing::IconType>("NodeEditorIconType",
	                                      {
		                                      {"Flow", ax::Drawing::IconType::Flow},
		                                      {"Circle", ax::Drawing::IconType::Circle},
		                                      {"Square", ax::Drawing::IconType::Square},
		                                      {"Grid", ax::Drawing::IconType::Grid},
		                                      {"RoundSquare", ax::Drawing::IconType::RoundSquare},
		                                      {"Diamond", ax::Drawing::IconType::Diamond}
	                                      }
	);

	state.new_enum<ed::StyleColor>("NodeEditorStyleColor",
	                               {
		                               {"Bg", ed::StyleColor::StyleColor_Bg},
		                               {"Grid", ed::StyleColor::StyleColor_Grid},
		                               {"NodeBg", ed::StyleColor::StyleColor_NodeBg},
		                               {"NodeBorder", ed::StyleColor::StyleColor_NodeBorder},
		                               {"HovNodeBorder", ed::StyleColor::StyleColor_HovNodeBorder},
		                               {"SelNodeBorder", ed::StyleColor::StyleColor_SelNodeBorder},
		                               {"NodeSelRect", ed::StyleColor::StyleColor_NodeSelRect},
		                               {"NodeSelRectBorder", ed::StyleColor::StyleColor_NodeSelRectBorder},
		                               {"HovLinkBorder", ed::StyleColor::StyleColor_HovLinkBorder},
		                               {"SelLinkBorder", ed::StyleColor::StyleColor_SelLinkBorder},
		                               {"HighlightLinkBorder", ed::StyleColor::StyleColor_HighlightLinkBorder},
		                               {"LinkSelRect", ed::StyleColor::StyleColor_LinkSelRect},
		                               {"LinkSelRectBorder", ed::StyleColor::StyleColor_LinkSelRectBorder},
		                               {"PinRect", ed::StyleColor::StyleColor_PinRect},
		                               {"PinRectBorder", ed::StyleColor::StyleColor_PinRectBorder},
		                               {"Flow", ed::StyleColor::StyleColor_Flow},
		                               {"FlowMarker", ed::StyleColor::StyleColor_FlowMarker},
		                               {"GroupBg", ed::StyleColor::StyleColor_GroupBg},
		                               {"GroupBorder", ed::StyleColor::StyleColor_GroupBorder}
	                               }
	);

	state.new_enum<ed::StyleVar>("NodeEditorStyleVar",
	                             {
		                             {"NodePadding", ed::StyleVar::StyleVar_NodePadding},
		                             {"NodeRounding", ed::StyleVar::StyleVar_NodeRounding},
		                             {"NodeBorderWidth", ed::StyleVar::StyleVar_NodeBorderWidth},
		                             {"HoveredNodeBorderWidth", ed::StyleVar::StyleVar_HoveredNodeBorderWidth},
		                             {"SelectedNodeBorderWidth", ed::StyleVar::StyleVar_SelectedNodeBorderWidth},
		                             {"PinRounding", ed::StyleVar::StyleVar_PinRounding},
		                             {"PinBorderWidth", ed::StyleVar::StyleVar_PinBorderWidth},
		                             {"LinkStrength", ed::StyleVar::StyleVar_LinkStrength},
		                             {"SourceDirection", ed::StyleVar::StyleVar_SourceDirection},
		                             {"TargetDirection", ed::StyleVar::StyleVar_TargetDirection},
		                             {"ScrollDuration", ed::StyleVar::StyleVar_ScrollDuration},
		                             {"FlowMarkerDistance", ed::StyleVar::StyleVar_FlowMarkerDistance},
		                             {"FlowSpeed", ed::StyleVar::StyleVar_FlowSpeed},
		                             {"FlowDuration", ed::StyleVar::StyleVar_FlowDuration},
		                             {"PivotAlignment", ed::StyleVar::StyleVar_PivotAlignment},
		                             {"PivotSize", ed::StyleVar::StyleVar_PivotSize},
		                             {"PivotScale", ed::StyleVar::StyleVar_PivotScale},
		                             {"PinCorners", ed::StyleVar::StyleVar_PinCorners},
		                             {"PinRadius", ed::StyleVar::StyleVar_PinRadius},
		                             {"PinArrowSize", ed::StyleVar::StyleVar_PinArrowSize},
		                             {"PinArrowWidth", ed::StyleVar::StyleVar_PinArrowWidth},
		                             {"GroupRounding", ed::StyleVar::StyleVar_GroupRounding},
		                             {"GroupBorderWidth", ed::StyleVar::StyleVar_GroupBorderWidth},
		                             {"HighlightConnectedLinks", ed::StyleVar::StyleVar_HighlightConnectedLinks},
		                             {"SnapLinkToPinDir", ed::StyleVar::StyleVar_SnapLinkToPinDir},
		                             {"HoveredNodeBorderOffset", ed::StyleVar::StyleVar_HoveredNodeBorderOffset},
		                             {"SelectedNodeBorderOffset", ed::StyleVar::StyleVar_SelectedNodeBorderOffset}
	                             }
	);

	state.new_usertype<ed::Config>("NodeEditorConfig",
	                               sol::constructors<ed::Config()>(),
	                               "SettingsFile", &ed::Config::SettingsFile,
	                               "BeginSaveSession", &ed::Config::BeginSaveSession,
	                               "EndSaveSession", &ed::Config::EndSaveSession,
	                               "SaveSettings", &ed::Config::SaveSettings,
	                               "LoadSettings", &ed::Config::LoadSettings,
	                               "SaveNodeSettings", &ed::Config::SaveNodeSettings,
	                               "LoadNodeSettings", &ed::Config::LoadNodeSettings,
	                               "UserPointer", &ed::Config::UserPointer,
	                               "CustomZoomLevels", &ed::Config::CustomZoomLevels,
	                               "CanvasSizeMode", &ed::Config::CanvasSizeMode,
	                               "DragButtonIndex", &ed::Config::DragButtonIndex,
	                               "SelectButtonIndex", &ed::Config::SelectButtonIndex,
	                               "NavigateButtonIndex", &ed::Config::NavigateButtonIndex,
	                               "ContextMenuButtonIndex", &ed::Config::ContextMenuButtonIndex,
	                               "EnableSmoothZoom", &ed::Config::EnableSmoothZoom,
	                               "SmoothZoomPower", &ed::Config::SmoothZoomPower
	);

	auto linkIdClass = state.new_usertype<ed::LinkId>("NodeEditorLinkId", sol::constructors<ed::LinkId(unsigned)>(),
	                                                  "Get", &ed::LinkId::Get,
	                                                  "bool", sol::property(&ed::LinkId::operator bool)
	);
	auto nodeIdClass = state.new_usertype<ed::NodeId>("NodeEditorNodeId", sol::constructors<ed::NodeId(unsigned)>(),
	                                                  "Get", &ed::NodeId::Get,
	                                                  "bool", sol::property(&ed::NodeId::operator bool)
	);
	auto pinIdClass = state.new_usertype<ed::PinId>("NodeEditorPinId", sol::constructors<ed::PinId(unsigned)>(),
	                                                "Get", &ed::PinId::Get,
	                                                "bool", sol::property(&ed::PinId::operator bool)
	);

	auto styleClass = state.new_usertype<ed::Style>("NodeEditorStyle", sol::no_constructor);
	styleClass["NodePadding"] = &ed::Style::NodePadding;
	styleClass["NodeRounding"] = &ed::Style::NodeRounding;
	styleClass["NodeBorderWidth"] = &ed::Style::NodeBorderWidth;
	styleClass["HoveredNodeBorderWidth"] = &ed::Style::HoveredNodeBorderWidth;
	styleClass["HoverNodeBorderOffset"] = &ed::Style::HoverNodeBorderOffset;
	styleClass["SelectedNodeBorderWidth"] = &ed::Style::SelectedNodeBorderWidth;
	styleClass["SelectedNodeBorderOffset"] = &ed::Style::SelectedNodeBorderOffset;
	styleClass["PinRounding"] = &ed::Style::PinRounding;
	styleClass["PinBorderWidth"] = &ed::Style::PinBorderWidth;
	styleClass["LinkStrength"] = &ed::Style::LinkStrength;
	styleClass["SourceDirection"] = &ed::Style::SourceDirection;
	styleClass["TargetDirection"] = &ed::Style::TargetDirection;
	styleClass["ScrollDuration"] = &ed::Style::ScrollDuration;
	styleClass["FlowMarkerDistance"] = &ed::Style::FlowMarkerDistance;
	styleClass["FlowSpeed"] = &ed::Style::FlowSpeed;
	styleClass["FlowDuration"] = &ed::Style::FlowDuration;
	styleClass["PivotAlignment"] = &ed::Style::PivotAlignment;
	styleClass["PivotSize"] = &ed::Style::PivotSize;
	styleClass["PivotScale"] = &ed::Style::PivotScale;
	styleClass["PinCorners"] = &ed::Style::PinCorners;
	styleClass["PinRadius"] = &ed::Style::PinRadius;
	styleClass["PinArrowSize"] = &ed::Style::PinArrowSize;
	styleClass["PinArrowWidth"] = &ed::Style::PinArrowWidth;
	styleClass["GroupRounding"] = &ed::Style::GroupRounding;
	styleClass["GroupBorderWidth"] = &ed::Style::GroupBorderWidth;
	styleClass["HighlightConnectedLinks"] = &ed::Style::HighlightConnectedLinks;
	styleClass["SnapLinkToPinDir"] = &ed::Style::SnapLinkToPinDir;
	styleClass["Colors"] = &ed::Style::Colors;
}
