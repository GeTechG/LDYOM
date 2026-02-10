#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor.h"
#include "window_manager.h"
#include <fa_icons.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <localization.h>
#include <logger.h>
#include <utils/imgui_configurate.h>

// Helper to apply ImGui theme colors to ImNodeFlow
namespace {
void InitializeNodeFlowTheme() {
	// Set global node colors based on ImGui theme
	ImFlow::NodeStyle::s_default_bg = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
	ImFlow::NodeStyle::s_default_border_color =
		ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Border));
	ImFlow::NodeStyle::s_default_border_selected_color =
		ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	// Set global grid colors based on ImGui theme
	ImFlow::InfColors::s_default_background =
		ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
	ImFlow::InfColors::s_default_grid = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Separator));
	ImVec4 gridColor = ImGui::GetStyleColorVec4(ImGuiCol_Separator);
	gridColor.w *= 0.25f; // Make subgrid more transparent
	ImFlow::InfColors::s_default_subGrid = ImGui::ColorConvertFloat4ToU32(gridColor);
}
} // namespace

// NumberNode implementation
NumberNode::NumberNode() {
	setTitle("Number");
	setStyle(ImFlow::NodeStyle::cyan());

	auto out = addOUT<float>("Value", ImFlow::PinStyle::cyan());
	out->behaviour([this]() { return m_value; });
}

void NumberNode::draw() {
	ImGui::PushItemWidth(100.0f);
	ImGui::DragFloat("##value", &m_value, 0.1f);
	ImGui::PopItemWidth();
}

// AddNode implementation
AddNode::AddNode() {
	setTitle("Add");
	setStyle(ImFlow::NodeStyle::green());

	addIN<float>("A", 0.0f, ImFlow::ConnectionFilter::SameType(), ImFlow::PinStyle::cyan());
	addIN<float>("B", 0.0f, ImFlow::ConnectionFilter::SameType(), ImFlow::PinStyle::cyan());

	auto out = addOUT<float>("Result", ImFlow::PinStyle::cyan());
	out->behaviour([this]() { return getInVal<float>("A") + getInVal<float>("B"); });
}

void AddNode::draw() { ImGui::Text("A + B"); }

// MultiplyNode implementation
MultiplyNode::MultiplyNode() {
	setTitle("Multiply");
	setStyle(ImFlow::NodeStyle::brown());

	addIN<float>("A", 1.0f, ImFlow::ConnectionFilter::SameType(), ImFlow::PinStyle::cyan());
	addIN<float>("B", 1.0f, ImFlow::ConnectionFilter::SameType(), ImFlow::PinStyle::cyan());

	auto out = addOUT<float>("Result", ImFlow::PinStyle::cyan());
	out->behaviour([this]() { return getInVal<float>("A") * getInVal<float>("B"); });
}

void MultiplyNode::draw() { ImGui::Text("A * B"); }

// OutputNode implementation
OutputNode::OutputNode() {
	setTitle("Output");
	setStyle(ImFlow::NodeStyle::red());

	addIN<float>("Value", 0.0f, ImFlow::ConnectionFilter::SameType(), ImFlow::PinStyle::cyan());
}

void OutputNode::draw() {
	float value = getInVal<float>("Value");
	ImGui::Text("Result: %.2f", value);
}

// TextNode implementation
TextNode::TextNode() {
	setTitle("Text");
	setStyle(ImFlow::NodeStyle::cyan());

	auto out = addOUT<std::string>("Text", ImFlow::PinStyle::white());
	out->behaviour([this]() { return std::string(m_buffer); });
}

void TextNode::draw() {
	ImGui::PushItemWidth(150.0f);
	ImGui::InputText("##text", m_buffer, sizeof(m_buffer));
	ImGui::PopItemWidth();
}

// NodeEditorWindow implementation
NodeEditorWindow::NodeEditorWindow()
	: Window("Node Editor") {
	m_nodeFlow = std::make_unique<ImFlow::ImNodeFlow>("LDYOM_NodeEditor");

	setSize(1000, 600);
	setSizeMin(400, 300);
	setPosition(100, 100);
	setNeedLockPlayerControl(true);

	setRenderCallback<NodeEditorWindow>(renderContent);

	// Setup right-click context menu
	m_nodeFlow->rightClickPopUpContent([this](ImFlow::BaseNode* node) {
		if (node) {
			if (ImGui::MenuItem(ICON_FA_TRASH " Delete Node")) {
				node->destroy();
			}
		} else {
			renderContextMenu();
		}
	});

	// Initialize theme colors
	refreshTheme();
}

void NodeEditorWindow::open() { Window::open(); }

void NodeEditorWindow::close() { Window::close(); }

void NodeEditorWindow::renderContent(NodeEditorWindow* window) {
	if (!window || !window->m_nodeFlow) {
		return;
	}

	// Set node flow size to fill the window
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	window->m_nodeFlow->setSize(windowSize);

	// Update node flow
	window->m_nodeFlow->update();
}

void NodeEditorWindow::renderContextMenu() {
	ImGui::SeparatorText("Add Node");

	if (ImGui::MenuItem(ICON_FA_HASHTAG " Number")) {
		m_nodeFlow->placeNode<NumberNode>();
	}
	if (ImGui::MenuItem(ICON_FA_PLUS " Add")) {
		m_nodeFlow->placeNode<AddNode>();
	}
	if (ImGui::MenuItem(ICON_FA_XMARK " Multiply")) {
		m_nodeFlow->placeNode<MultiplyNode>();
	}
	if (ImGui::MenuItem(ICON_FA_ARROW_RIGHT_TO_BRACKET " Output")) {
		m_nodeFlow->placeNode<OutputNode>();
	}
	if (ImGui::MenuItem(ICON_FA_FONT " Text")) {
		m_nodeFlow->placeNode<TextNode>();
	}
}

void NodeEditorWindow::refreshTheme() { InitializeNodeFlowTheme(); }
