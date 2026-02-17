#pragma once

#include "window.h"
#include <ImNodeFlow.h>
#include <memory>
#include <string>

class NodeEditorWindow : public Window {
  private:
    std::unique_ptr<ImFlow::ImNodeFlow> m_nodeFlow;

    void renderContextMenu();
    void initNodeFlow();

    static void renderContent(NodeEditorWindow* window);

  public:
    NodeEditorWindow();
    ~NodeEditorWindow() override = default;

    void open() override;
    void close() override;

    ImFlow::ImNodeFlow* getNodeFlow() { return m_nodeFlow.get(); }
    void refreshTheme();

    void saveGraph(const std::string& filePath);
    void loadGraph(const std::string& filePath);
    void clearGraph();
};
