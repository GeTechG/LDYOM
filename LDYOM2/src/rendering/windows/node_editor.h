#pragma once

#include "window.h"
#include <ImNodeFlow.h>
#include <lua_node.h>
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
    std::vector<Workspace> m_workspaces;
    int m_activeIdx = 0;

    // Rename state
    int m_renamingIdx = -1;
    char m_renameBuffer[128]{};

    // Context-menu search
    char m_searchBuf[128]{};

    struct FilteredEntry {
        std::string type;
        std::string label;
        std::string desc;
    };
    std::string m_lastSearch;
    std::vector<FilteredEntry> m_filteredResults;

    Workspace& activeWorkspace();
    void addWorkspace(const std::string& name);
    void initNodeFlowForWorkspace(Workspace& ws);

    void renderSidebar();
    void rebuildFilteredResults(const std::string& searchLower);
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

    // Find a node by UID across all workspaces; optionally returns the owning graph
    LuaNode* findNodeByUID(ImFlow::NodeUID uid, ImFlow::ImNodeFlow** outGraph = nullptr) const;

    void refreshTheme();

    // Save/load all workspaces to a single JSON file
    void saveGraph(const std::string& filePath);
    void loadGraph(const std::string& filePath);

    // Clear the active workspace's graph only
    void clearGraph();
};
