#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor.h"
#include "node_graph_serializer.h"
#include "node_registry.h"
#include "window_manager.h"
#include <algorithm>
#include <fa_icons.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <localization.h>
#include <logger.h>
#include <lua_node.h>
#include <utils/imgui_configurate.h>
#include <vector>

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

void NodeEditorWindow::rebuildFilteredResults(const std::string& searchLower) {
	m_filteredResults.clear();
	auto& registry = NodeRegistry::instance();
	auto& loc = Localization::instance().getI18N();

	for (const auto& category : registry.getCategories()) {
		std::string categoryName = _(fmt::format("nodes_categories.{}", category));
		std::string catLower = categoryName;
		std::transform(catLower.begin(), catLower.end(), catLower.begin(), ::tolower);

		for (const auto& type : registry.getTypesForCategory(category)) {
			auto title = _(fmt::format("nodes_titles.{}", type));
			std::string titleLower = title;
			std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);

			std::string desc;
			std::string descLower;
			std::string descKey = fmt::format("nodes_descriptions.{}", type);
			if (loc.keyExists(descKey)) {
				desc = _(descKey);
				descLower = desc;
				std::transform(descLower.begin(), descLower.end(), descLower.begin(), ::tolower);
			}

			if (titleLower.find(searchLower) == std::string::npos &&
			    catLower.find(searchLower) == std::string::npos &&
			    descLower.find(searchLower) == std::string::npos) {
				continue;
			}

			const auto* descReg = registry.find(type);
			std::string icon;
			if (descReg) {
				icon = NodeStyleRegistry::instance().getIcon(descReg->styleKey);
			}
			std::string label = categoryName + " / " + (icon.empty() ? title : icon + " " + title);

			m_filteredResults.push_back({type, std::move(label), std::move(desc)});
		}
	}
}

namespace {
void showNodeDescriptionTooltip(const std::string& desc) {
	if (!desc.empty()) {
		ImGui::BeginTooltip();
		ImGui::TextUnformatted(desc.c_str());
		ImGui::EndTooltip();
	}
}
} // namespace

void NodeEditorWindow::renderContextMenu() {
	auto& registry = NodeRegistry::instance();
	auto categories = registry.getCategories();

	if (categories.empty()) {
		ImGui::TextDisabled("(no nodes registered)");
		return;
	}

	if (ImGui::IsWindowAppearing()) {
		ImGui::SetKeyboardFocusHere();
		memset(m_searchBuf, 0, sizeof(m_searchBuf));
		m_lastSearch.clear();
		m_filteredResults.clear();
	}

	ImGui::SetNextItemWidth(260.0f);
	ImGui::InputTextWithHint("##node_search", ICON_FA_MAGNIFYING_GLASS " Search...", m_searchBuf, sizeof(m_searchBuf));

	std::string searchLower(m_searchBuf);
	std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
	bool isSearching = !searchLower.empty();

	if (isSearching) {
		if (searchLower != m_lastSearch) {
			m_lastSearch = searchLower;
			rebuildFilteredResults(searchLower);
		}

		ImGui::BeginChild("##node_search_list", ImVec2(260.0f, 320.0f), false);

		for (const auto& entry : m_filteredResults) {
			if (ImGui::Selectable(entry.label.c_str())) {
				registry.ensureLoaded(entry.type);
				m_nodeFlow->placeNode<LuaNode>(entry.type);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
				showNodeDescriptionTooltip(entry.desc);
			}
		}

		ImGui::EndChild();
	} else {
		ImGui::BeginChild("##cat_scroll", ImVec2(ImGui::GetContentRegionAvail().x, 320.0f), false);

		for (const auto& category : categories) {
			if (ImGui::BeginMenu(_(fmt::format("nodes_categories.{}", category)).c_str())) {
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
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
						std::string descKey = fmt::format("nodes_descriptions.{}", type);
						if (Localization::instance().getI18N().keyExists(descKey)) {
							showNodeDescriptionTooltip(_(descKey));
						}
					}
				}
				ImGui::EndMenu();
			}
		}

		ImGui::EndChild();
	}
}

void NodeEditorWindow::refreshTheme() { InitializeNodeFlowTheme(); }

void NodeEditorWindow::saveGraph(const std::string& filePath) { NodeGraphSerializer::save(m_nodeFlow.get(), filePath); }

void NodeEditorWindow::loadGraph(const std::string& filePath) { NodeGraphSerializer::load(m_nodeFlow.get(), filePath); }

void NodeEditorWindow::clearGraph() { initNodeFlow(); }
