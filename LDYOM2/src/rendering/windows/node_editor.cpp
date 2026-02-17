#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor.h"
#include "node_graph_serializer.h"
#include "node_registry.h"
#include "window_manager.h"
#include <fa_icons.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <localization.h>
#include <logger.h>
#include <lua_node.h>
#include <utils/imgui_configurate.h>

// Helper to apply ImGui theme colors to ImNodeFlow
namespace {
void InitializeNodeFlowTheme() {
	auto nodeBg = ImGui::GetStyleColorVec4(ImGuiCol_PopupBg);
	ImFlow::NodeStyle::s_default_bg = ImGui::ColorConvertFloat4ToU32(nodeBg);
	ImFlow::NodeStyle::s_default_border_color =
		ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Border));
	ImFlow::NodeStyle::s_default_border_selected_color =
		ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImFlow::InfColors::s_default_background =
		ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
	ImFlow::InfColors::s_default_grid = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Separator));
	ImVec4 gridColor = ImGui::GetStyleColorVec4(ImGuiCol_Separator);
	gridColor.w *= 0.25f;
	ImFlow::InfColors::s_default_subGrid = ImGui::ColorConvertFloat4ToU32(gridColor);
}
} // namespace

// NodeEditorWindow implementation
NodeEditorWindow::NodeEditorWindow()
	: Window("Node Editor") {
	setSize(1000, 600);
	setSizeMin(400, 300);
	setPosition(100, 100);
	setNeedLockPlayerControl(true);

	setRenderCallback<NodeEditorWindow>(renderContent);

	initNodeFlow();
}

void NodeEditorWindow::initNodeFlow() {
	m_nodeFlow = std::make_unique<ImFlow::ImNodeFlow>("LDYOM_NodeEditor");

	m_nodeFlow->rightClickPopUpContent([this](ImFlow::BaseNode* node) {
		if (node) {
			if (ImGui::MenuItem(ICON_FA_TRASH " Delete Node")) {
				node->destroy();
			}
		} else {
			renderContextMenu();
		}
	});

	refreshTheme();
}

void NodeEditorWindow::open() { Window::open(); }

void NodeEditorWindow::close() { Window::close(); }

void NodeEditorWindow::renderContent(NodeEditorWindow* window) {
	if (!window || !window->m_nodeFlow) {
		return;
	}

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	window->m_nodeFlow->setSize(windowSize);
	window->m_nodeFlow->update();
}

void NodeEditorWindow::renderContextMenu() {
	ImGui::SeparatorText("Add Node");

	auto& registry = NodeRegistry::instance();
	auto categories = registry.getCategories();

	if (categories.empty()) {
		ImGui::TextDisabled("(no nodes registered)");
		return;
	}

	for (const auto& category : categories) {
		if (ImGui::BeginMenu(category.c_str())) {
			auto types = registry.getTypesForCategory(category);
			for (const auto& type : types) {
				const auto* desc = registry.find(type);

				auto title = _(fmt::format("nodes_titles.{}", type));
				std::string label;
				if (desc) {
					const std::string& icon = NodeStyleRegistry::instance().getIcon(desc->styleKey);
					label = icon.empty() ? title : icon + " " + title;
				} else {
					label = title;
				}
				if (ImGui::MenuItem(label.c_str())) {
					registry.ensureLoaded(type);
					m_nodeFlow->placeNode<LuaNode>(type);
				}
			}
			ImGui::EndMenu();
		}
	}
}

void NodeEditorWindow::refreshTheme() { InitializeNodeFlowTheme(); }

void NodeEditorWindow::saveGraph(const std::string& filePath) { NodeGraphSerializer::save(m_nodeFlow.get(), filePath); }

void NodeEditorWindow::loadGraph(const std::string& filePath) { NodeGraphSerializer::load(m_nodeFlow.get(), filePath); }

void NodeEditorWindow::clearGraph() { initNodeFlow(); }
