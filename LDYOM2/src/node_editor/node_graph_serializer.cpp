#include "node_graph_serializer.h"
#include "lua_node.h"
#include "node_registry.h"
#include <fstream>
#include <functional>
#include <logger.h>
#include <unordered_map>

void NodeGraphSerializer::save(ImFlow::ImNodeFlow* graph, const std::string& filePath) {
    if (!graph) return;
    try {
        nlohmann::json j = toJson(graph);
        std::ofstream file(filePath);
        if (!file.is_open()) {
            LDYOM_ERROR("NodeGraphSerializer: cannot open '{}' for writing", filePath);
            return;
        }
        file << j.dump(2);
        LDYOM_INFO("NodeGraphSerializer: saved graph to '{}'", filePath);
    } catch (const std::exception& e) {
        LDYOM_ERROR("NodeGraphSerializer::save exception: {}", e.what());
    }
}

void NodeGraphSerializer::load(ImFlow::ImNodeFlow* graph, const std::string& filePath) {
    if (!graph) return;
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            LDYOM_INFO("NodeGraphSerializer: no saved graph at '{}'", filePath);
            return;
        }
        nlohmann::json j;
        file >> j;
        fromJson(graph, j);
        LDYOM_INFO("NodeGraphSerializer: loaded graph from '{}'", filePath);
    } catch (const std::exception& e) {
        LDYOM_ERROR("NodeGraphSerializer::load exception: {}", e.what());
    }
}

nlohmann::json NodeGraphSerializer::toJson(ImFlow::ImNodeFlow* graph) {
    nlohmann::json j;
    j["nodes"] = nlohmann::json::array();
    j["links"] = nlohmann::json::array();

    for (auto& [uid, nodePtr] : graph->getNodes()) {
        auto* luaNode = dynamic_cast<LuaNode*>(nodePtr.get());
        if (!luaNode) continue;

        nlohmann::json nodeJ;
        nodeJ["uid"]    = uid;
        nodeJ["type"]   = luaNode->getNodeType();
        nodeJ["pos_x"]  = luaNode->getPos().x;
        nodeJ["pos_y"]  = luaNode->getPos().y;
        nodeJ["data"]         = luaNode->serializeData();
        nodeJ["pin_defaults"] = luaNode->serializePinDefaults();
        j["nodes"].push_back(nodeJ);
    }

    for (auto& weakLink : graph->getLinks()) {
        auto link = weakLink.lock();
        if (!link) continue;

        ImFlow::Pin* left  = link->left();   // output pin
        ImFlow::Pin* right = link->right();  // input pin
        if (!left || !right) continue;

        auto* outNode = dynamic_cast<LuaNode*>(left->getParent());
        auto* inNode  = dynamic_cast<LuaNode*>(right->getParent());
        if (!outNode || !inNode) continue;

        // Find pin indices (more reliable than name-based lookup)
        int outPinIdx = -1;
        {
            const auto& outs = outNode->getOuts();
            for (int i = 0; i < static_cast<int>(outs.size()); ++i) {
                if (outs[i].get() == left) { outPinIdx = i; break; }
            }
        }
        int inPinIdx = -1;
        {
            const auto& ins = inNode->getIns();
            for (int i = 0; i < static_cast<int>(ins.size()); ++i) {
                if (ins[i].get() == right) { inPinIdx = i; break; }
            }
        }
        if (outPinIdx < 0 || inPinIdx < 0) continue;

        nlohmann::json linkJ;
        linkJ["out_node_uid"] = outNode->getUID();
        linkJ["out_pin_idx"]  = outPinIdx;
        linkJ["in_node_uid"]  = inNode->getUID();
        linkJ["in_pin_idx"]   = inPinIdx;
        j["links"].push_back(linkJ);
    }

    return j;
}

void NodeGraphSerializer::fromJson(ImFlow::ImNodeFlow* graph, const nlohmann::json& j) {
    if (!j.contains("nodes")) return;

    // uid → shared_ptr mapping built during node creation
    std::unordered_map<ImFlow::NodeUID, std::shared_ptr<LuaNode>> uidMap;

    for (const auto& nodeJ : j["nodes"]) {
        std::string type = nodeJ.value("type", "");
        if (type.empty()) continue;

        NodeRegistry::instance().ensureLoaded(type);

        float posX = nodeJ.value("pos_x", 0.0f);
        float posY = nodeJ.value("pos_y", 0.0f);

        auto node = graph->addNode<LuaNode>(ImVec2(posX, posY), type);
        if (!node) continue;

        if (nodeJ.contains("data") && !nodeJ["data"].is_null()) {
            node->deserializeData(nodeJ["data"]);
        }
        if (nodeJ.contains("pin_defaults") && nodeJ["pin_defaults"].is_array()) {
            node->deserializePinDefaults(nodeJ["pin_defaults"]);
        }

        // Use the serialized uid for link reconnection
        ImFlow::NodeUID storedUid = nodeJ.value("uid", static_cast<ImFlow::NodeUID>(0));
        uidMap[storedUid] = node;
    }

    // Reconnect links
    if (!j.contains("links")) return;
    for (const auto& linkJ : j["links"]) {
        ImFlow::NodeUID outUid = linkJ.value("out_node_uid", static_cast<ImFlow::NodeUID>(0));
        ImFlow::NodeUID inUid  = linkJ.value("in_node_uid",  static_cast<ImFlow::NodeUID>(0));
        int outPinIdx = linkJ.value("out_pin_idx", -1);
        int inPinIdx  = linkJ.value("in_pin_idx",  -1);

        auto outIt = uidMap.find(outUid);
        auto inIt  = uidMap.find(inUid);
        if (outIt == uidMap.end() || inIt == uidMap.end()) continue;

        const auto& outs = outIt->second->getOuts();
        const auto& ins  = inIt->second->getIns();
        if (outPinIdx < 0 || outPinIdx >= static_cast<int>(outs.size())) continue;
        if (inPinIdx  < 0 || inPinIdx  >= static_cast<int>(ins.size()))  continue;

        ImFlow::Pin* outPin = outs[outPinIdx].get();
        ImFlow::Pin* inPin  = ins[inPinIdx].get();
        if (!outPin || !inPin) continue;

        // createLink handles m_link assignment, outPin registration, and graph addLink internally
        inPin->createLink(outPin);
    }
}
