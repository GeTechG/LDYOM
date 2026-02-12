#include "node_graph_serializer.h"
#include "lua_node.h"
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
        nodeJ["data"]   = luaNode->serializeData();
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

        nlohmann::json linkJ;
        linkJ["out_node_uid"] = outNode->getUID();
        linkJ["out_pin"]      = left->getName();
        linkJ["in_node_uid"]  = inNode->getUID();
        linkJ["in_pin"]       = right->getName();
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

        float posX = nodeJ.value("pos_x", 0.0f);
        float posY = nodeJ.value("pos_y", 0.0f);

        auto node = graph->addNode<LuaNode>(ImVec2(posX, posY), type);
        if (!node) continue;

        if (nodeJ.contains("data") && !nodeJ["data"].is_null()) {
            node->deserializeData(nodeJ["data"]);
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
        std::string outPinName = linkJ.value("out_pin", "");
        std::string inPinName  = linkJ.value("in_pin",  "");

        auto outIt = uidMap.find(outUid);
        auto inIt  = uidMap.find(inUid);
        if (outIt == uidMap.end() || inIt == uidMap.end()) continue;

        // Safely find pins without assert
        ImFlow::Pin* outPin = nullptr;
        ImFlow::Pin* inPin  = nullptr;
        {
            ImFlow::PinUID h = std::hash<std::string>{}(outPinName);
            for (auto& p : outIt->second->getOuts()) {
                if (p->getUid() == h) { outPin = p.get(); break; }
            }
        }
        {
            ImFlow::PinUID h = std::hash<std::string>{}(inPinName);
            for (auto& p : inIt->second->getIns()) {
                if (p->getUid() == h) { inPin = p.get(); break; }
            }
        }
        if (!outPin || !inPin) continue;

        // Create link via the graph
        auto link = std::make_shared<ImFlow::Link>(outPin, inPin, graph);
        outPin->setLink(link);
        inPin->setLink(link);
        graph->addLink(link);
    }
}
