#define IMGUI_DEFINE_MATH_OPERATORS
#include <tracy/Tracy.hpp>
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

// Split a lowercase string into non-empty whitespace-separated tokens.
static std::vector<std::string> splitTokens(const std::string& s) {
	std::vector<std::string> tokens;
	size_t i = 0;
	while (i < s.size()) {
		while (i < s.size() && s[i] == ' ')
			++i;
		size_t j = i;
		while (j < s.size() && s[j] != ' ')
			++j;
		if (j > i)
			tokens.push_back(s.substr(i, j - i));
		i = j;
	}
	return tokens;
}

// Draw text with every occurrence of every token highlighted.
// Overlapping/adjacent spans are merged before drawing.
static void DrawTextHighlighted(ImDrawList* dl, ImVec2 pos, const std::string& text,
                                const std::vector<std::string>& tokens, ImU32 normalColor, ImU32 highlightColor) {
	if (text.empty())
		return;
	if (tokens.empty()) {
		dl->AddText(pos, normalColor, text.c_str());
		return;
	}

	std::string textLower = text;
	std::transform(textLower.begin(), textLower.end(), textLower.begin(), ::tolower);

	// Collect all [start, end) highlight spans from every token
	std::vector<std::pair<size_t, size_t>> spans;
	for (const auto& token : tokens) {
		if (token.empty())
			continue;
		size_t p = 0;
		while ((p = textLower.find(token, p)) != std::string::npos) {
			spans.push_back({p, p + token.size()});
			p += token.size();
		}
	}

	// Sort then merge overlapping/adjacent spans
	std::sort(spans.begin(), spans.end());
	std::vector<std::pair<size_t, size_t>> merged;
	for (auto& s : spans) {
		if (!merged.empty() && s.first <= merged.back().second)
			merged.back().second = std::max(merged.back().second, s.second);
		else
			merged.push_back(s);
	}

	float xOff = 0.0f;
	size_t cur = 0;
	for (auto& [start, end] : merged) {
		if (start > cur) {
			const char* b = text.c_str() + cur;
			const char* e = text.c_str() + start;
			dl->AddText(ImVec2(pos.x + xOff, pos.y), normalColor, b, e);
			xOff += ImGui::CalcTextSize(b, e).x;
		}
		const char* b = text.c_str() + start;
		const char* e = text.c_str() + end;
		dl->AddText(ImVec2(pos.x + xOff, pos.y), highlightColor, b, e);
		xOff += ImGui::CalcTextSize(b, e).x;
		cur = end;
	}
	if (cur < text.size())
		dl->AddText(ImVec2(pos.x + xOff, pos.y), normalColor, text.c_str() + cur);
}

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

	auto* nodeFlowPtr = ws.nodeFlow.get();
	ws.nodeFlow->rightClickPopUpContent([this, nodeFlowPtr](ImFlow::BaseNode* node) {
		if (node) {
			if (ImGui::MenuItem(ICON_FA_COPY " Duplicate Node")) {
				auto* luaNode = dynamic_cast<LuaNode*>(node);
				if (luaNode) {
					constexpr float kOffset = 30.0f;
					ImVec2 newPos = ImVec2(node->getPos().x + kOffset, node->getPos().y + kOffset);
					NodeRegistry::instance().ensureLoaded(luaNode->getNodeType());
					auto newNode = nodeFlowPtr->addNode<LuaNode>(newPos, luaNode->getNodeType());
					if (newNode) {
						newNode->deserializeData(luaNode->serializeData());
						newNode->deserializePinDefaults(luaNode->serializePinDefaults());
						newNode->onLoad();
					}
				}
			}
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
	ZoneScopedN("NodeEditor::renderSidebar");
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
	ZoneScopedN("NodeEditor::renderContent");
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

void NodeEditorWindow::rebuildNodeCache() {
	ZoneScopedN("NodeEditor::rebuildNodeCache");
	m_nodeCache.clear();

	auto& registry = NodeRegistry::instance();
	auto& loc = Localization::instance().getI18N();

	for (const auto& category : registry.getCategories()) {
		for (const auto& type : registry.getTypesForCategory(category)) {
			NodeCacheEntry entry;
			entry.type = type;
			entry.category = category;

			auto title = _(fmt::format("nodes_titles.{}", type));
			entry.titleLower = title;
			std::transform(entry.titleLower.begin(), entry.titleLower.end(), entry.titleLower.begin(), ::tolower);

			const auto* descReg = registry.find(type);
			std::string icon;
			if (descReg)
				icon = NodeStyleRegistry::instance().getIcon(descReg->styleKey);
			entry.label = icon.empty() ? title : icon + " " + title;

			std::string descKey = fmt::format("nodes_descriptions.{}", type);
			if (loc.keyExists(descKey)) {
				entry.desc = _(descKey);
				entry.descLower = entry.desc;
				std::transform(entry.descLower.begin(), entry.descLower.end(), entry.descLower.begin(), ::tolower);
			}

			m_nodeCache.push_back(std::move(entry));
		}
	}
}

void NodeEditorWindow::rebuildFilteredResults(const std::string& searchLower) {
	ZoneScopedN("NodeEditor::rebuildFilteredResults");
	m_filteredResults.clear();
	m_searchTokens = splitTokens(searchLower);
	if (m_searchTokens.empty())
		return;

	const int n = static_cast<int>(m_searchTokens.size());

	for (const auto& entry : m_nodeCache) {
		int score = 0;
		for (int ti = 0; ti < n; ++ti) {
			const int tokenWeight = n - ti;
			const auto& token = m_searchTokens[ti];
			if (entry.titleLower.find(token) != std::string::npos)
				score += tokenWeight * 2;
			else if (entry.descLower.find(token) != std::string::npos)
				score += tokenWeight;
		}
		if (score == 0)
			continue;

		m_filteredResults.push_back({entry.type, entry.label, entry.desc, entry.category, score});
	}

	std::sort(m_filteredResults.begin(), m_filteredResults.end(),
	          [](const FilteredEntry& a, const FilteredEntry& b) { return a.score > b.score; });
}

void NodeEditorWindow::addToRecent(const std::string& type) {
	m_recentNodeTypes.erase(
	    std::remove(m_recentNodeTypes.begin(), m_recentNodeTypes.end(), type), m_recentNodeTypes.end());
	m_recentNodeTypes.push_front(type);
	if (m_recentNodeTypes.size() > 10)
		m_recentNodeTypes.pop_back();
}

void NodeEditorWindow::rebuildNodeTree() {
	ZoneScopedN("NodeEditor::rebuildNodeTree");
	m_rootCategory = CategoryNode{};
	auto& registry = NodeRegistry::instance();

	for (const auto& category : registry.getCategories()) {
		CategoryNode* cur = &m_rootCategory;
		std::string path;
		std::string remaining = category;
		while (true) {
			auto dotPos = remaining.find('.');
			std::string seg = (dotPos == std::string::npos) ? remaining : remaining.substr(0, dotPos);
			if (!path.empty())
				path += '.';
			path += seg;
			auto& child = cur->children[seg];
			child.name = seg;
			child.fullPath = path;
			if (dotPos == std::string::npos) {
				child.fullCategory = category;
				child.nodeTypes = registry.getTypesForCategory(category);
				break;
			}
			remaining = remaining.substr(dotPos + 1);
			cur = &child;
		}
	}
}

void NodeEditorWindow::renderNodeTreeNode(const CategoryNode& node, Workspace& ws) {
	auto& registry = NodeRegistry::instance();
	auto& loc = Localization::instance().getI18N();

	// Nodes directly in this category
	for (const auto& type : node.nodeTypes) {
		const auto* desc = registry.find(type);
		std::string icon;
		if (desc)
			icon = NodeStyleRegistry::instance().getIcon(desc->styleKey);
		std::string title = _(fmt::format("nodes_titles.{}", type));
		std::string label = (icon.empty() ? title : icon + " " + title) + "##nd_" + type;

		bool selected = (m_selectedNodeType == type);
		if (ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick)) {
			m_selectedNodeType = type;
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				registry.ensureLoaded(type);
				ws.nodeFlow->placeNode<LuaNode>(type);
				addToRecent(type);
				ImGui::CloseCurrentPopup();
			}
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
			std::string descKey = fmt::format("nodes_descriptions.{}", type);
			if (loc.keyExists(descKey))
				ImGui::SetTooltip("%s", _(descKey).c_str());
		}
	}

	// Subcategories as tree nodes (Unreal-style: plain arrow, no folder icon)
	for (const auto& [seg, child] : node.children) {
		std::string locKey = fmt::format("nodes_categories.{}", child.fullPath);
		std::string titleLocKey = locKey + ".title";
		std::string menuLabel = loc.keyExists(titleLocKey) ? _(titleLocKey) : _(locKey);
		std::string treeId = menuLabel + "##cat_" + child.fullPath;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		bool open = ImGui::TreeNodeEx(treeId.c_str(), flags, "%s", menuLabel.c_str());
		if (open) {
			renderNodeTreeNode(child, ws);
			ImGui::TreePop();
		}
	}
}

void NodeEditorWindow::renderNodeDescription() {
	auto& registry = NodeRegistry::instance();
	auto& loc = Localization::instance().getI18N();

	if (m_selectedNodeType.empty()) {
		ImGui::TextDisabled(_("create_new_objective.no_selection").c_str());
		return;
	}

	const auto* desc = registry.find(m_selectedNodeType);
	std::string icon;
	if (desc)
		icon = NodeStyleRegistry::instance().getIcon(desc->styleKey);
	std::string title = _(fmt::format("nodes_titles.{}", m_selectedNodeType));

	if (icon.empty())
		ImGui::TextUnformatted(title.c_str());
	else
		ImGui::Text("%s %s", icon.c_str(), title.c_str());

	ImGui::Separator();

	std::string descKey = fmt::format("nodes_descriptions.{}", m_selectedNodeType);
	if (loc.keyExists(descKey)) {
		ImGui::TextWrapped("%s", _(descKey).c_str());
	} else {
		ImGui::TextDisabled(_("create_new_objective.no_description").c_str());
	}

	if (desc && !desc->category.empty()) {
		ImGui::Spacing();
		ImGui::TextDisabled("%s: %s", _("create_new_component.category").c_str(),
		                    buildCategoryDisplayName(desc->category).c_str());
	}
}

void NodeEditorWindow::renderContextMenu() {
	ZoneScopedN("NodeEditor::renderContextMenu");
	auto& registry = NodeRegistry::instance();

	if (registry.getCategories().empty()) {
		ImGui::TextDisabled("(no nodes registered)");
		return;
	}

	if (ImGui::IsWindowAppearing()) {
		ImGui::SetKeyboardFocusHere();
		memset(m_searchBuf, 0, sizeof(m_searchBuf));
		m_lastSearch.clear();
		m_filteredResults.clear();
		m_selectedNodeType.clear();
		rebuildNodeTree();
		rebuildNodeCache();
	}

	constexpr float kPopupW = 320.0f;
	constexpr float kListH = 400.0f;

	// ── Search bar ──────────────────────────────────────────────────────────
	ImGui::SetNextItemWidth(kPopupW);
	ImGui::InputTextWithHint("##node_search", ICON_FA_MAGNIFYING_GLASS " Search...", m_searchBuf, sizeof(m_searchBuf));

	std::string searchLower(m_searchBuf);
	std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
	bool isSearching = !searchLower.empty();

	if (isSearching && searchLower != m_lastSearch) {
		m_lastSearch = searchLower;
		rebuildFilteredResults(searchLower);
	}

	auto& ws = activeWorkspace();

	// ── Node list ────────────────────────────────────────────────────────────
	ImGui::BeginChild("##node_list_panel", ImVec2(kPopupW, kListH), ImGuiChildFlags_FrameStyle);

	if (isSearching) {
		// Group results by category, preserving registry insertion order
		std::vector<std::string> catOrder;
		std::map<std::string, std::vector<const FilteredEntry*>> grouped;
		for (const auto& entry : m_filteredResults) {
			if (grouped.find(entry.category) == grouped.end())
				catOrder.push_back(entry.category);
			grouped[entry.category].push_back(&entry);
		}

		const ImU32 highlightColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.40f, 0.85f, 0.20f, 1.0f));
		const ImU32 normalColor = ImGui::GetColorU32(ImGuiCol_Text);
		const float itemH = ImGui::GetTextLineHeightWithSpacing(); // used as explicit selectable height

		for (const auto& catKey : catOrder) {
			const auto& entries = grouped[catKey];
			std::string catDisplay = catKey.empty() ? "" : buildCategoryDisplayName(catKey);

			bool catOpen = true;
			if (!catDisplay.empty()) {
				ImGuiTreeNodeFlags catFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
				catOpen = ImGui::TreeNodeEx(("##srchcat_" + catKey).c_str(), catFlags, "%s", catDisplay.c_str());
			}

			if (catOpen) {
				for (const auto* entry : entries) {
					bool selected = (m_selectedNodeType == entry->type);
					ImVec2 cursorPos = ImGui::GetCursorScreenPos();
					float availW = ImGui::GetContentRegionAvail().x;
					if (ImGui::Selectable(("##srch_" + entry->type).c_str(), selected,
					                      ImGuiSelectableFlags_AllowDoubleClick,
					                      ImVec2(availW, itemH))) {
						m_selectedNodeType = entry->type;
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							registry.ensureLoaded(entry->type);
							ws.nodeFlow->placeNode<LuaNode>(entry->type);
							addToRecent(entry->type);
							ImGui::CloseCurrentPopup();
						}
					}
					DrawTextHighlighted(ImGui::GetWindowDrawList(), cursorPos, entry->label, m_searchTokens,
					                    normalColor, highlightColor);
					if (!entry->desc.empty() && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
						ImGui::SetTooltip("%s", entry->desc.c_str());
				}
				if (!catDisplay.empty())
					ImGui::TreePop();
			}
		}
	} else {
		// ── Recent nodes ────────────────────────────────────────────────────────
		if (!m_recentNodeTypes.empty()) {
			ImGuiTreeNodeFlags recentFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::TreeNodeEx("##recent_nodes", recentFlags, "%s", _("node_editor.recent").c_str())) {
				for (const auto& type : m_recentNodeTypes) {
					if (registry.find(type) == nullptr)
						continue;
					const auto* desc = registry.find(type);
					std::string icon;
					if (desc)
						icon = NodeStyleRegistry::instance().getIcon(desc->styleKey);
					std::string title = _(fmt::format("nodes_titles.{}", type));
					std::string label = (icon.empty() ? title : icon + " " + title) + "##rec_" + type;
					bool selected = (m_selectedNodeType == type);
					if (ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick)) {
						m_selectedNodeType = type;
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							registry.ensureLoaded(type);
							ws.nodeFlow->placeNode<LuaNode>(type);
							addToRecent(type);
							ImGui::CloseCurrentPopup();
						}
					}
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		renderNodeTreeNode(m_rootCategory, ws);
	}

	ImGui::EndChild();

	// ── Place button ─────────────────────────────────────────────────────────
	ImGui::Separator();
	bool canPlace = !m_selectedNodeType.empty();
	if (!canPlace)
		ImGui::BeginDisabled();
	if (ImGui::Button(_("create").c_str(), ImVec2(kPopupW, 0))) {
		registry.ensureLoaded(m_selectedNodeType);
		ws.nodeFlow->placeNode<LuaNode>(m_selectedNodeType);
		addToRecent(m_selectedNodeType);
		ImGui::CloseCurrentPopup();
	}
	if (!canPlace)
		ImGui::EndDisabled();
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
	ZoneScopedN("NodeEditor::saveGraph");
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
	ZoneScopedN("NodeEditor::loadGraph");
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
	m_workspaces.clear();
	m_activeIdx = 0;
	addWorkspace("Workspace 1");
}
