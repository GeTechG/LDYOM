#pragma once

#include <ImNodeFlow.h>
#include <ktcoro_wait.hpp>
#include <string>

// Traverses the node graph and runs all entry-point LuaNodes via ktwait coroutines.
// Entry nodes are LuaNode instances whose flow-input pins are unconnected.
class NodeEditorExecutor {
  public:
    // Launch execution of all entry nodes in the graph.
    // Each entry node is started as a separate TaskManager task.
    static void run(ImFlow::ImNodeFlow* graph, const std::string& graphId);

    // Stop all running tasks associated with graphId
    static void stop(const std::string& graphId);

  private:
    // Coroutine that traverses the flow chain starting from a given node
    static ktwait traverseChain(ImFlow::ImNodeFlow* graph, ImFlow::BaseNode* startNode,
                                const std::string& graphId);
};
