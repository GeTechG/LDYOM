#include "ImNodesLuaWrapper.h"

#include <utility>

#include "NodeEditorUtils.h"
#include "imnodes/imnodes.h"
#include "imnodes/imnodes_internal.h"

void ImNodesLuaWrapper::wrap(sol::state& state) {
	auto table = state.create_table("ImNodes");
	table.set_function("beginNode", [](const int id, const char* name, int nodeType, const sol::object& description) {
		std::optional<std::string> descriptionOpt;
		if (description.is<std::string>())
			descriptionOpt = description.as<std::string>();
		NodeEditorUtils::beginNode(id, name, static_cast<NodeType>(nodeType), descriptionOpt);
	});
	table.set_function("beginNodeNoTitle", ImNodes::BeginNode);
	table.set_function("beginInputAttribute", NodeEditorUtils::beginInputAttribute);
	table.set_function("endInputAttribute", NodeEditorUtils::endInputAttribute);
	table.set_function("beginOutputAttribute", NodeEditorUtils::beginOutputAttribute);
	table.set_function("endOutputAttribute", NodeEditorUtils::endOutputAttribute);
	table.set_function("beginStaticAttribute", ImNodes::BeginStaticAttribute);
	table.set_function("endStaticAttribute", ImNodes::EndStaticAttribute);
	table.set_function("endNode", NodeEditorUtils::endNode);
	table.set_function("endNodeNoTitle", ImNodes::EndNode);
	table.set_function("setNodeScreenSpacePos", [](const int nodeId, const float x, const float y) {
		ImNodes::SetNodeScreenSpacePos(nodeId, ImVec2(x, y));
	});
	table.set_function("getNodeDimensions", ImNodes::GetNodeDimensions);
	table.set_function("enum", [](const char* text, const sol::object& pV, std::vector<std::string>* names, const sol::table ids) {
		auto v = static_cast<float*>(const_cast<void*>(pV.pointer()));
		auto previewName = "";
		for (auto id : ids) {
			if (id.second.as<int>() == static_cast<int>(*v)) {
				previewName = names->at(id.first.as<int>() - 1).c_str();
			}
		}
		if (ImGui::BeginCombo(text, previewName)) {

			for (auto pairIds : ids) {
				if (ImGui::Selectable(names->at(pairIds.first.as<int>() - 1).c_str(), pairIds.second.as<int>() == static_cast<int>(*v))) {
					*v = pairIds.second.as<float>();
				}
			}

			ImGui::EndCombo();
		}
	});
	table.set_function("editorContextCreate", []() {
		return std::unique_ptr<ImNodesEditorContext, void(*)(ImNodesEditorContext*)>(ImNodes::EditorContextCreate(), [](ImNodesEditorContext* context) {
			ImNodes::EditorContextFree(context);
		});
	});
	table.set_function("getNodeEditorSpacePos", ImNodes::GetNodeEditorSpacePos);
	table.set_function("setNodeEditorSpacePos", ImNodes::SetNodeEditorSpacePos);
	table.set_function("saveEditorStateToIniString", [](const ImNodesEditorContext* context) {
		size_t size;
		const auto data = ImNodes::SaveEditorStateToIniString(context, &size);
		return std::make_tuple(size, std::string(data, size));
	});
	table.set_function("loadEditorStateFromIniString", [](ImNodesEditorContext* context, const char* data, const unsigned size) {
		ImNodes::LoadEditorStateFromIniString(context, data, size);
	});
	table.set_function("editorContextSet", ImNodes::EditorContextSet);
}
