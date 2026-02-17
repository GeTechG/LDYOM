#pragma once

#include <ImNodeFlow.h>
#include <nlohmann/json.hpp>
#include <string>

// Saves and loads the node graph (positions, connections, per-instance data)
// to/from a JSON file at {project_path}/scene_nodes/{sceneId}.json
class NodeGraphSerializer {
  public:
    // Save current graph state to file
    static void save(ImFlow::ImNodeFlow* graph, const std::string& filePath);

    // Load graph state from file, placing LuaNode instances into the graph
    static void load(ImFlow::ImNodeFlow* graph, const std::string& filePath);

    // Build the JSON representation without writing to disk
    static nlohmann::json toJson(ImFlow::ImNodeFlow* graph);

    // Restore graph from JSON
    static void fromJson(ImFlow::ImNodeFlow* graph, const nlohmann::json& j);
};
