#pragma once

#include "lua/lua_task_manager.h"
#include "window.h"
#include <ImNodeFlow.h>
#include <deque>
#include <lua_node.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

class NodeEditorWindow : public Window {
  public:
    struct Workspace {
        std::string name;
        std::unique_ptr<ImFlow::ImNodeFlow> nodeFlow;
    };

  private:
    LuaTaskManager m_taskManager;

    std::vector<Workspace> m_workspaces;
    int m_activeIdx = 0;

    // Rename state
    int m_renamingIdx = -1;
    char m_renameBuffer[128]{};

    // Context-menu / node picker state
    char m_searchBuf[256]{};
    std::string m_selectedNodeType;

    struct FilteredEntry {
        std::string type;
        std::string label;    // icon + title
        std::string desc;
        std::string category; // raw registry category path (e.g. "flow.core")
        int score = 0;        // higher = better match, used for sorting
    };
    std::string m_lastSearch;
    std::vector<std::string> m_searchTokens;
    std::vector<FilteredEntry> m_filteredResults;

    // Category tree for the node picker
    struct CategoryNode {
        std::string name;
        std::string fullPath;
        std::string fullCategory; // non-empty when this level maps to a real registry category
        std::map<std::string, CategoryNode> children; // ordered by segment name
        std::vector<std::string> nodeTypes;
    };
    CategoryNode m_rootCategory;

    // Recently used nodes (up to 10)
    std::deque<std::string> m_recentNodeTypes;
    void addToRecent(const std::string& type);

    Workspace& activeWorkspace();
    void addWorkspace(const std::string& name);
    void initNodeFlowForWorkspace(Workspace& ws);

    void renderSidebar();
    void rebuildFilteredResults(const std::string& searchLower);
    void rebuildNodeTree();
    void renderNodeTreeNode(const CategoryNode& node, Workspace& ws);
    void renderNodeDescription();
    void renderContextMenu();

    static void renderContent(NodeEditorWindow* window);

  public:
    NodeEditorWindow();
    ~NodeEditorWindow() override = default;

    void open() override;
    void close() override;

    // Returns active workspace's node flow (for backward compat)
    ImFlow::ImNodeFlow* getNodeFlow();

    // Access all workspaces (for Lua bindings)
    const std::vector<Workspace>& getWorkspaces() const { return m_workspaces; }

    LuaTaskManager& taskManager() { return m_taskManager; }

    // Find a node by UID across all workspaces; optionally returns the owning graph
    LuaNode* findNodeByUID(ImFlow::NodeUID uid, ImFlow::ImNodeFlow** outGraph = nullptr) const;

    void refreshTheme();

    // Save/load all workspaces to a single JSON file
    void saveGraph(const std::string& filePath);
    void loadGraph(const std::string& filePath);

    // Clear the active workspace's graph only
    void clearGraph();
};
