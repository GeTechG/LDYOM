#define IMGUI_DEFINE_MATH_OPERATORS
#include "node_editor.h"
#include "node_graph_serializer.h"
#include "node_registry.h"
#include "window_manager.h"
#include <algorithm>
#include <fa_icons.h>
#include <fstream>
#include <functional>
#include <imgui.h>
#include <imgui_internal.h>
#include <localization.h>
#include <logger.h>
#include <lua_node.h>
#include <map>
#include <nlohmann/json.hpp>
#include <utils/imgui_configurate.h>
#include <vector>

namespace {
void ApplyNodeFlowTheme() {
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

// ─── Constructor ──────────────────────────────────────────────────────────────

NodeEditorWindow::NodeEditorWindow()
	: Window("Node Editor") {
	setSize(1000, 600);
	setSizeMin(400, 300);
	setPosition(100, 100);
	setNeedLockPlayerControl(true);
	setRenderCallback<NodeEditorWindow>(renderContent);

	addWorkspace("Workspace 1");
}

// ─── Workspace management ─────────────────────────────────────────────────────

void NodeEditorWindow::initNodeFlowForWorkspace(Workspace& ws) {
	// Each workspace gets a unique ImNodeFlow ID so their internal state is separate.
	ws.nodeFlow = std::make_unique<ImFlow::ImNodeFlow>("LDYOM_NE_" + ws.name);

	ws.nodeFlow->rightClickPopUpContent([this](ImFlow::BaseNode* node) {
		if (node) {
			if (ImGui::MenuItem(ICON_FA_TRASH " Delete Node")) {
				node->destroy();
			}
		} else {
			renderContextMenu();
		}
	});

	ApplyNodeFlowTheme();
}

void NodeEditorWindow::addWorkspace(const std::string& name) {
	m_workspaces.emplace_back();
	initNodeFlowForWorkspace(m_workspaces.back());
	m_workspaces.back().name = name;
}

NodeEditorWindow::Workspace& NodeEditorWindow::activeWorkspace() {
	if (m_activeIdx < 0 || m_activeIdx >= static_cast<int>(m_workspaces.size())) {
		m_activeIdx = 0;
	}
	return m_workspaces[m_activeIdx];
}

LuaNode* NodeEditorWindow::findNodeByUID(ImFlow::NodeUID uid, ImFlow::ImNodeFlow** outGraph) const {
	for (const auto& ws : m_workspaces) {
		if (!ws.nodeFlow)
			continue;
		auto& nodes = ws.nodeFlow->getNodes();
		auto it = nodes.find(uid);
		if (it != nodes.end()) {
			auto* luaNode = dynamic_cast<LuaNode*>(it->second.get());
			if (luaNode) {
				if (outGraph)
					*outGraph = ws.nodeFlow.get();
				return luaNode;
			}
		}
	}
	return nullptr;
}

// ─── Open / close ─────────────────────────────────────────────────────────────

void NodeEditorWindow::open() { Window::open(); }
void NodeEditorWindow::close() { Window::close(); }

// ─── Sidebar ─────────────────────────────────────────────────────────────────

void NodeEditorWindow::renderSidebar() {
	constexpr float kSidebarWidth = 160.0f;
	const float fullHeight = ImGui::GetContentRegionAvail().y;

	ImGui::BeginChild("##ws_sidebar", ImVec2(kSidebarWidth, fullHeight), true);

	// ── Toolbar ──
	if (ImGui::SmallButton(ICON_FA_PLUS)) {
		std::string base = "Workspace ";
		int idx = static_cast<int>(m_workspaces.size()) + 1;
		std::string newName = base + std::to_string(idx);
		while (std::any_of(m_workspaces.begin(), m_workspaces.end(),
		                   [&](const Workspace& w) { return w.name == newName; })) {
			++idx;
			newName = base + std::to_string(idx);
		}
		addWorkspace(newName);
		m_activeIdx = static_cast<int>(m_workspaces.size()) - 1;
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		ImGui::SetTooltip("Create workspace");

	ImGui::SameLine();

	const bool canDelete = m_workspaces.size() > 1;
	if (!canDelete)
		ImGui::BeginDisabled();
	if (ImGui::SmallButton(ICON_FA_TRASH)) {
		if (m_renamingIdx == m_activeIdx)
			m_renamingIdx = -1;
		m_workspaces.erase(m_workspaces.begin() + m_activeIdx);
		if (m_activeIdx >= static_cast<int>(m_workspaces.size()))
			m_activeIdx = static_cast<int>(m_workspaces.size()) - 1;
		if (m_renamingIdx > m_activeIdx)
			--m_renamingIdx;
	}
	if (!canDelete)
		ImGui::EndDisabled();
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		ImGui::SetTooltip("Delete workspace");

	ImGui::SameLine();

	if (ImGui::SmallButton(ICON_FA_PEN)) {
		if (m_renamingIdx != m_activeIdx) {
			m_renamingIdx = m_activeIdx;
			strncpy(m_renameBuffer, m_workspaces[m_activeIdx].name.c_str(), sizeof(m_renameBuffer) - 1);
			m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
		} else {
			m_renamingIdx = -1;
		}
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		ImGui::SetTooltip("Rename workspace");

	ImGui::Separator();

	// ── List ──
	for (int i = 0; i < static_cast<int>(m_workspaces.size()); ++i) {
		const bool isActive = (i == m_activeIdx);
		const bool isRenaming = (i == m_renamingIdx);

		if (isRenaming) {
			ImGui::SetNextItemWidth(kSidebarWidth - 16.0f);
			if (ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere();
			bool commit = ImGui::InputText("##rename_ws", m_renameBuffer, sizeof(m_renameBuffer),
			                               ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
			if (commit || (ImGui::IsItemDeactivated() && !ImGui::IsItemActive())) {
				std::string newName(m_renameBuffer);
				if (!newName.empty())
					m_workspaces[i].name = newName;
				m_renamingIdx = -1;
			}
		} else {
			if (isActive)
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
			bool selected =
				ImGui::Selectable(m_workspaces[i].name.c_str(), isActive, ImGuiSelectableFlags_AllowDoubleClick);
			if (isActive)
				ImGui::PopStyleColor();

			if (selected) {
				m_activeIdx = i;
				m_renamingIdx = -1;
				if (ImGui::IsMouseDoubleClicked(0)) {
					m_renamingIdx = i;
					strncpy(m_renameBuffer, m_workspaces[i].name.c_str(), sizeof(m_renameBuffer) - 1);
					m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
				}
			}
		}
	}

	ImGui::EndChild();
}

// ─── Main render ─────────────────────────────────────────────────────────────

void NodeEditorWindow::renderContent(NodeEditorWindow* window) {
	if (!window || window->m_workspaces.empty())
		return;

	window->renderSidebar();
	ImGui::SameLine();

	auto& ws = window->activeWorkspace();
	if (!ws.nodeFlow)
		return;

	ImVec2 canvasSize = ImGui::GetContentRegionAvail();
	ws.nodeFlow->setSize(canvasSize);
	ws.nodeFlow->update();
}

// ─── Context-menu / search ───────────────────────────────────────────────────

static std::string buildCategoryDisplayName(const std::string& category) {
	auto& loc = Localization::instance().getI18N();
	std::string display;
	std::string path;
	std::string remaining = category;
	while (true) {
		auto dotPos = remaining.find('.');
		std::string seg = (dotPos == std::string::npos) ? remaining : remaining.substr(0, dotPos);
		if (!path.empty())
			path += '.';
		path += seg;
		std::string locKey = fmt::format("nodes_categories.{}", path);
		std::string titleLocKey = fmt::format("{}.title", locKey);
		std::string segLabel = loc.keyExists(titleLocKey) ? _(titleLocKey) : _(locKey);
		if (!display.empty())
			display += " / ";
		display += segLabel;
		if (dotPos == std::string::npos)
			break;
		remaining = remaining.substr(dotPos + 1);
	}
	return display;
}

void NodeEditorWindow::rebuildFilteredResults(const std::string& searchLower) {
	m_filteredResults.clear();
	auto& registry = NodeRegistry::instance();
	auto& loc = Localization::instance().getI18N();

	for (const auto& category : registry.getCategories()) {
		std::string categoryDisplay = buildCategoryDisplayName(category);
		std::string catLower = categoryDisplay;
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

			if (titleLower.find(searchLower) == std::string::npos && catLower.find(searchLower) == std::string::npos &&
			    descLower.find(searchLower) == std::string::npos) {
				continue;
			}

			const auto* descReg = registry.find(type);
			std::string icon;
			if (descReg)
				icon = NodeStyleRegistry::instance().getIcon(descReg->styleKey);
			std::string label = categoryDisplay + " / " + (icon.empty() ? title : icon + " " + title);
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

	auto& ws = activeWorkspace();

	if (isSearching) {
		if (searchLower != m_lastSearch) {
			m_lastSearch = searchLower;
			rebuildFilteredResults(searchLower);
		}

		ImGui::BeginChild("##node_search_list", ImVec2(260.0f, 320.0f), false);
		for (const auto& entry : m_filteredResults) {
			if (ImGui::Selectable(entry.label.c_str())) {
				registry.ensureLoaded(entry.type);
				ws.nodeFlow->placeNode<LuaNode>(entry.type);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
				showNodeDescriptionTooltip(entry.desc);
		}
		ImGui::EndChild();
	} else {
		// Build multi-level category tree from dot-separated category strings.
		struct TreeNode {
			std::map<std::string, TreeNode> children; // segment -> child (ordered)
			std::string fullCategory;                 // non-empty when this level is an actual registry category
			std::string fullPath;                     // full dot-separated path
		};

		TreeNode root;
		for (const auto& category : categories) {
			TreeNode* cur = &root;
			std::string path;
			std::string remaining = category;
			while (true) {
				auto dotPos = remaining.find('.');
				std::string seg = (dotPos == std::string::npos) ? remaining : remaining.substr(0, dotPos);
				if (!path.empty())
					path += '.';
				path += seg;
				auto& child = cur->children[seg];
				child.fullPath = path;
				if (dotPos == std::string::npos) {
					child.fullCategory = category;
					break;
				}
				remaining = remaining.substr(dotPos + 1);
				cur = &child;
			}
		}

		auto& loc = Localization::instance().getI18N();

		std::function<void(const TreeNode&)> renderTree = [&](const TreeNode& node) {
			for (const auto& [seg, child] : node.children) {
				std::string locKey = fmt::format("nodes_categories.{}", child.fullPath);
				std::string titleLocKey = fmt::format("{}.title", locKey);
				std::string menuLabel = loc.keyExists(titleLocKey) ? _(titleLocKey) : _(locKey);

				if (ImGui::BeginMenu(menuLabel.c_str())) {
					// Render nodes directly in this category
					if (!child.fullCategory.empty()) {
						auto types = registry.getTypesForCategory(child.fullCategory);
						for (const auto& type : types) {
							const auto* desc = registry.find(type);
							auto title = _(fmt::format("nodes_titles.{}", type));
							std::string itemLabel;
							if (desc) {
								const std::string& icon = NodeStyleRegistry::instance().getIcon(desc->styleKey);
								itemLabel = icon.empty() ? title : icon + " " + title;
							} else {
								itemLabel = title;
							}
							if (ImGui::MenuItem(itemLabel.c_str())) {
								registry.ensureLoaded(type);
								ws.nodeFlow->placeNode<LuaNode>(type);
							}
							if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
								std::string descKey = fmt::format("nodes_descriptions.{}", type);
								if (loc.keyExists(descKey))
									showNodeDescriptionTooltip(_(descKey));
							}
						}
						if (!child.children.empty())
							ImGui::Separator();
					}
					renderTree(child);
					ImGui::EndMenu();
				}
			}
		};

		ImGui::BeginChild("##cat_scroll", ImVec2(ImGui::GetContentRegionAvail().x, 320.0f), false);
		renderTree(root);
		ImGui::EndChild();
	}
}

// ─── Public API ───────────────────────────────────────────────────────────────

void NodeEditorWindow::refreshTheme() {
	// ImNodeFlow theme colors are global statics — one call covers all workspaces.
	ApplyNodeFlowTheme();
}

ImFlow::ImNodeFlow* NodeEditorWindow::getNodeFlow() {
	if (m_workspaces.empty())
		return nullptr;
	return activeWorkspace().nodeFlow.get();
}

// ─── Save / load (all workspaces) ─────────────────────────────────────────────

void NodeEditorWindow::saveGraph(const std::string& filePath) {
	try {
		nlohmann::json root;
		root["active_index"] = m_activeIdx;
		root["workspaces"] = nlohmann::json::array();

		for (const auto& ws : m_workspaces) {
			nlohmann::json wsJ;
			wsJ["name"] = ws.name;
			wsJ["graph"] = ws.nodeFlow ? NodeGraphSerializer::toJson(ws.nodeFlow.get()) : nlohmann::json::object();
			root["workspaces"].push_back(wsJ);
		}

		std::ofstream file(filePath);
		if (!file.is_open()) {
			LDYOM_ERROR("NodeEditorWindow::saveGraph: cannot open '{}' for writing", filePath);
			return;
		}
		file << root.dump(2);
		LDYOM_INFO("NodeEditorWindow::saveGraph: saved {} workspace(s) to '{}'", m_workspaces.size(), filePath);
	} catch (const std::exception& e) {
		LDYOM_ERROR("NodeEditorWindow::saveGraph exception: {}", e.what());
	}
}

void NodeEditorWindow::loadGraph(const std::string& filePath) {
	try {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			LDYOM_INFO("NodeEditorWindow::loadGraph: no file at '{}'", filePath);
			return;
		}

		nlohmann::json root;
		file >> root;

		// ── Backward compat: old single-graph format has "nodes" at root level ──
		if (root.contains("nodes") && !root.contains("workspaces")) {
			// Load into the first (and only) workspace.
			m_workspaces.clear();
			addWorkspace("Workspace 1");
			NodeGraphSerializer::fromJson(m_workspaces[0].nodeFlow.get(), root);
			m_activeIdx = 0;
			LDYOM_INFO("NodeEditorWindow::loadGraph: loaded legacy single-graph from '{}'", filePath);
			return;
		}

		// ── Multi-workspace format ──
		if (!root.contains("workspaces"))
			return;

		m_workspaces.clear();
		m_renamingIdx = -1;

		for (const auto& wsJ : root["workspaces"]) {
			std::string name = wsJ.value("name", "Workspace");
			addWorkspace(name);
			auto& ws = m_workspaces.back();
			if (wsJ.contains("graph") && !wsJ["graph"].is_null()) {
				NodeGraphSerializer::fromJson(ws.nodeFlow.get(), wsJ["graph"]);
			}
		}

		if (m_workspaces.empty())
			addWorkspace("Workspace 1");

		m_activeIdx = root.value("active_index", 0);
		if (m_activeIdx >= static_cast<int>(m_workspaces.size()))
			m_activeIdx = 0;

		LDYOM_INFO("NodeEditorWindow::loadGraph: loaded {} workspace(s) from '{}'", m_workspaces.size(), filePath);
	} catch (const std::exception& e) {
		LDYOM_ERROR("NodeEditorWindow::loadGraph exception: {}", e.what());
	}
}

void NodeEditorWindow::clearGraph() {
	if (m_workspaces.empty())
		return;
	auto& ws = activeWorkspace();
	initNodeFlowForWorkspace(ws);
}
