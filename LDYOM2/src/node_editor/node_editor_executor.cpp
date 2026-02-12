#include "node_editor_executor.h"
#include "lua_node.h"
#include <functional>
#include <logger.h>
#include <task_manager.h>

namespace {

// Check whether a node's flow-input pins are all unconnected (entry node heuristic)
bool isEntryNode(ImFlow::BaseNode* node) {
    for (auto& pin : node->getIns()) {
        if (pin->getDataType() == typeid(FlowToken) && pin->isConnected()) {
            return false;
        }
    }
    // Must have at least one flow input to be considered a flow node
    for (auto& pin : node->getIns()) {
        if (pin->getDataType() == typeid(FlowToken)) {
            return true;
        }
    }
    return false;
}

// Find the first output flow pin name that has a connected node
ImFlow::BaseNode* followFlowPin(ImFlow::ImNodeFlow* graph, ImFlow::BaseNode* current,
                                const std::string& outPinName) {
    // Safe pin lookup without assert
    ImFlow::Pin* pin = nullptr;
    {
        ImFlow::PinUID h = std::hash<std::string>{}(outPinName);
        for (auto& p : current->getOuts()) {
            if (p->getUid() == h) { pin = p.get(); break; }
        }
    }
    if (!pin || !pin->isConnected()) return nullptr;

    auto link = pin->getLink().lock();
    if (!link) return nullptr;

    ImFlow::Pin* inPin = link->right();
    if (!inPin) return nullptr;

    return inPin->getParent();
}

} // namespace

ktwait NodeEditorExecutor::traverseChain(ImFlow::ImNodeFlow* graph, ImFlow::BaseNode* startNode,
                                          const std::string& /*graphId*/) {
    ImFlow::BaseNode* current = startNode;
    while (current) {
        auto* luaNode = dynamic_cast<LuaNode*>(current);
        if (!luaNode) break;

        std::string outFlowPin;
        co_await luaNode->execute(outFlowPin);

        if (outFlowPin.empty()) break;

        current = followFlowPin(graph, current, outFlowPin);
    }
}

void NodeEditorExecutor::run(ImFlow::ImNodeFlow* graph, const std::string& graphId) {
    if (!graph) return;

    int chainIndex = 0;
    for (auto& [uid, nodePtr] : graph->getNodes()) {
        auto* luaNode = dynamic_cast<LuaNode*>(nodePtr.get());
        if (!luaNode) continue;
        if (!isEntryNode(luaNode)) continue;

        std::string taskKey = graphId + "_chain_" + std::to_string(chainIndex++);
        TaskManager::instance().addTask(
            taskKey,
            traverseChain,
            graph,
            static_cast<ImFlow::BaseNode*>(luaNode),
            graphId
        );
    }
}

void NodeEditorExecutor::stop(const std::string& graphId) {
    // Remove all tasks whose keys start with graphId
    auto states = TaskManager::instance().getAllTaskStates();
    for (auto& [key, state] : states) {
        if (key.rfind(graphId + "_chain_", 0) == 0) {
            TaskManager::instance().removeTask(key);
        }
    }
}
