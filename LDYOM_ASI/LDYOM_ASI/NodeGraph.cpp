#include "NodeGraph.h"

#include <map>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS 1
#include "imgui_internal.h"
#include "ScriptManager.h"
#include "libs/coro_wait.h"

extern std::map<std::string, std::vector<sol::table>> map_nodes_class;
extern void printLog(std::string print_text);
extern const char* langt(const std::string& key);
extern bool storylineMode;
extern NodeGraph* currentNodeGraphPtr;
sol::state NodeGraph::baseNode{};

NodeGraph::NodeGraph()
{
	this->nodes = std::map<unsigned, sol::table>();
	this->links = std::map<int, sol::table>();
}

void NodeGraph::addNodeClass(const std::string& category, sol::table& class_)
{
	map_nodes_class[category].emplace_back(class_);
}

unsigned NodeGraph::getID()
{
	static unsigned id = 0;
	id++;
	return id;
}

ImColor getPinColor(NodeGraph::PinType type)
{
	switch (type)
	{
	case NodeGraph::number:
		return ImColor(0.09f, 0.52f, 0.82f, 1.0f);
		break;
	case NodeGraph::boolean:
		return ImColor(0.79f, 0.05f, 0.06f, 1.0f);
		break;
	case NodeGraph::void_:
		return ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]);
		break;
	default:
		return ImColor();
		break;
	}
}

void NodeGraph::BeginInputAttribute(int id, sol::object shape = sol::nil)
{
	sol::optional<sol::table> pin = currentNodeGraphPtr->nodes[(id / 100) * 100]["Pins"][id];
	if (pin) {
		imnodes::PinShape c_shape = imnodes::PinShape_CircleFilled;
		if (shape.get_type() == sol::type::userdata)
		{
			c_shape = shape.as<imnodes::PinShape>();
		}
		auto color = getPinColor(static_cast<PinType>(pin.value()["type"]));
		ImColor hover_color = color.Value - ImColor(30, 30, 30, 0).Value;
		imnodes::PushColorStyle(imnodes::ColorStyle_Pin, ImGui::ColorConvertFloat4ToU32(color.Value));
		imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, ImGui::ColorConvertFloat4ToU32(hover_color.Value));
		imnodes::BeginInputAttribute(id, c_shape);
	}
}

void NodeGraph::EndInputAttribute()
{
	imnodes::EndInputAttribute();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

void NodeGraph::BeginOutputAttribute(int id, sol::object shape)
{
	sol::optional<sol::table> pin = currentNodeGraphPtr->nodes[(id / 100) * 100]["Pins"][id];
	if (pin) {
		imnodes::PinShape c_shape = imnodes::PinShape_CircleFilled;
		if (shape.get_type() == sol::type::userdata)
		{
			c_shape = shape.as<imnodes::PinShape>();
		}
		auto color = getPinColor(static_cast<PinType>(pin.value()["type"]));
		ImColor hover_color = color.Value - ImColor(10, 10, 10, 0).Value;
		imnodes::PushColorStyle(imnodes::ColorStyle_Pin, ImGui::ColorConvertFloat4ToU32(color.Value));
		imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, ImGui::ColorConvertFloat4ToU32(hover_color.Value));
		imnodes::BeginOutputAttribute(id, c_shape);
	}
}

void NodeGraph::EndOutputAttribute()
{
	imnodes::EndOutputAttribute();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

ImColor getNodeColor(unsigned short type_)
{
	switch (type_)
	{
	case 0: //event
		return  ImColor(0.18f, 0.55f, .0f, 1.0f);
		break;
	case 1: //fork
		return  ImColor(1.00f, 0.26f, 0.12f, 1.0f);
		break;
	case 2: //loop
		return  ImColor(0.95f, 0.64f, 0.05f, 1.0f);
		break;
	case 3: //var
		return  ImColor(0.00f, 0.38f, 0.65f, 1.0f);
		break;
	case 4: //func
		return  ImColor(0.37f, 0.21f, 0.69f, 1.0f);
		break;
	case 5: //exit
		return  ImColor(0.85f, 0.05f, 0.12f, 1.0f);
		break;
	default:
		return ImColor();
		break;
	}
}

void NodeGraph::BeginNode(int id, unsigned short type_)
{
	ImColor color = getNodeColor(type_);
	ImColor color_hover = color + ImColor(15, 15, 15, 0).Value;
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, ImGui::ColorConvertFloat4ToU32(color.Value));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, ImGui::ColorConvertFloat4ToU32(color_hover.Value));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, ImGui::ColorConvertFloat4ToU32(color_hover.Value));
	imnodes::BeginNode(id);
}

void NodeGraph::EndNode()
{
	imnodes::EndNode();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

std::string NodeGraph::getNodeIcon(std::string icon)
{
	if (icon == "event")
		return u8"\uf2f6";
	else if (icon == "fork")
		return u8"\uf2e3";
	else if (icon == "loop")
		return u8"\uf363";
	else if (icon == "var")
		return u8"\uf187";
	else if (icon == "func")
		return u8"\uf661";
	else if (icon == "exit")
		return u8"\uf2f5";
}
extern coro_wait instance;
void NodeGraph::callAllNodeName(std::string& name, NodeGraph* data, Mission* mission)
{
	std::map<unsigned, sol::table>& nodes_l = data->nodes;
	for (auto node : nodes_l)
	{
		std::string node_name = node.second["class"]["static"]["name"];
		if (node_name._Equal(name))
		{
			instance.add_to_queue(std::bind(callNode, node.second, data, mission));
		}
	}
}

void removeLink(int link_id)
{
	int id_in_p = currentNodeGraphPtr->links.at(link_id)["id_in"];
	int id_out_p = currentNodeGraphPtr->links.at(link_id)["id_out"];
	int node_id_in = (id_in_p / 100) * 100;
	int node_id_out = (id_out_p / 100) * 100;
	currentNodeGraphPtr->nodes[node_id_in]["Pins"][id_in_p]["link"] = sol::nil;
	sol::table old_out_links = currentNodeGraphPtr->nodes[node_id_out]["Pins"][id_out_p]["links"];
	for (const auto& old_out_link : old_out_links)
	{
		if (old_out_link.second.as<int>() == link_id)
		{
			old_out_links[old_out_link.first] = sol::nil;
			break;
		}
	}
	currentNodeGraphPtr->links.erase(link_id);
}

void NodeGraph::render()
{
	bool editor_hover = false;
	ImGui::Begin("my first node graph");
	ImGui::Text(std::to_string(ImGui::GetIO().Framerate).c_str());

	imnodes::PushColorStyle(imnodes::ColorStyle_GridBackground, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ChildBg]));
	imnodes::PushColorStyle(imnodes::ColorStyle_GridLine, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text] + ImVec4(.0f, .0f, .0f, -0.84f)));
	imnodes::PushColorStyle(imnodes::ColorStyle_NodeBackground, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg] + ImVec4(0.07f, 0.07f, 0.07f, .0f)));
	imnodes::PushColorStyle(imnodes::ColorStyle_NodeBackgroundHovered, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg] + ImVec4(0.07f, 0.09f, 0.09f, .0f)));
	imnodes::PushColorStyle(imnodes::ColorStyle_NodeBackgroundSelected, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_WindowBg] + ImVec4(0.09f, 0.09f, 0.09f, .0f)));
	imnodes::PushStyleVar(imnodes::StyleVar_NodeCornerRounding, ImGui::GetStyle().FrameRounding);
	imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);
	imnodes::BeginNodeEditor();

	for (auto node : currentNodeGraphPtr->nodes)
	{
		sol::protected_function fDraw = node.second["draw"];
		if (fDraw.valid())
		{
			auto result = fDraw(node.second);
			if (!result.valid())
			{
				sol::error error = result;
				printLog(error.what());
			}
		}
	}
	for (auto link : currentNodeGraphPtr->links)
	{
		int id_in = link.second["id_in"];
		int id_out = link.second["id_out"];
		int node_id = (id_in / 100) * 100;
		int pin_type = currentNodeGraphPtr->nodes[node_id]["Pins"][id_in]["type"];
		ImColor color = getPinColor((PinType)pin_type);
		ImColor color_hover = color.Value + ImColor(20, 20, 20, 0).Value;
		ImColor color_selected = color.Value + ImColor(35, 35, 35, 0).Value;
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color_hover);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color_selected);
		imnodes::Link(link.first, id_in, id_out);
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
	}
	editor_hover = imnodes::IsEditorHovered();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopStyleVar();
	imnodes::PopAttributeFlag();
	imnodes::EndNodeEditor();

	int id_in, id_out;

	if (imnodes::IsLinkStarted(&id_in))
	{
		sol::object type_pin = currentNodeGraphPtr->nodes[(id_in / 100) * 100]["Pins"][id_in]["type"];
		if (type_pin.get_type() == sol::type::number)
		{
			ImColor color = getPinColor((PinType)type_pin.as<int>());
			ImColor color_hover = color.Value + ImColor(10, 10, 10, 0).Value;
			ImColor color_selected = color.Value + ImColor(15, 15, 15, 0).Value;
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color_hover);
			imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color_selected);
		}
	}

	int link_id;
	if (imnodes::IsLinkDestroyed(&link_id))
	{
		removeLink(link_id);
	}
	if (imnodes::IsPinHovered(&id_in))
	{
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			int id_node = (id_in / 100) * 100;
			if (currentNodeGraphPtr->nodes.at(id_node)["Pins"][id_in]["pin_type"] == 0) {
				sol::optional<int> link_id_in = currentNodeGraphPtr->nodes.at(id_node)["Pins"][id_in]["link"];
				if (link_id_in.has_value())
					removeLink(link_id_in.value());
			}
		}
	}

	int id_node;
	if (imnodes::IsNodeHovered(&id_node)) {}
	else
	{
		if (editor_hover)
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				ImGui::OpenPopup("CreateMenu");
		}
	}

	ImGui::SetNextWindowSize(ImVec2(200, 0));
	if (ImGui::BeginPopup("CreateMenu"))
	{
		for (auto categories : map_nodes_class)
		{
			std::string nameC = langt(categories.first);
			if (ImGui::TreeNode(nameC.empty() ? categories.first.c_str() : nameC.c_str()))
			{
				for (auto node_class : categories.second)
				{
					std::string name_node = node_class["static"]["name"];
					sol::optional<bool> storyline_node = node_class["static"]["storyline"];
					if (storyline_node.has_value())
					{
						if (storyline_node.value() && !storylineMode)
						{
							continue;
						}
					}
					sol::optional<bool> mission_node = node_class["static"]["mission"];
					if (mission_node.has_value())
					{
						if (mission_node.value() && storylineMode)
						{
							continue;
						}
					}
					if (ImGui::Selectable(name_node.c_str()))
					{
						int id_node = currentNodeGraphPtr->nodes.empty()? 0 : (--currentNodeGraphPtr->nodes.end())->first + 100;
						sol::protected_function fNew = node_class["new"];
						auto result = fNew(node_class, id_node);
						if (ScriptManager::checkProtected(result)) {
							currentNodeGraphPtr->nodes[id_node] = result;
							imnodes::SetNodeScreenSpacePos(id_node, ImGui::GetWindowPos());
						}
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::EndPopup();
	}
	
	if (imnodes::IsLinkCreated(&id_in, &id_out))
	{
		int node_id_in = (id_in / 100) * 100;
		int node_id_out = (id_out / 100) * 100;
		if (currentNodeGraphPtr->nodes[node_id_in]["Pins"][id_in]["pin_type"] == 1)
		{
			std::swap(id_in, id_out);
			std::swap(node_id_in, node_id_out);
		}
		int in_type = currentNodeGraphPtr->nodes[node_id_in]["Pins"][id_in]["type"];
		int out_type = currentNodeGraphPtr->nodes[node_id_out]["Pins"][id_out]["type"];
		if (in_type == out_type) {
			sol::object link_in = currentNodeGraphPtr->nodes[node_id_in]["Pins"][id_in]["link"];
			if (link_in.get_type() != sol::type::nil)
			{
				removeLink(link_in.as<int>());
			}
			int link_id = currentNodeGraphPtr->links.empty() ? 0 : (--currentNodeGraphPtr->links.end())->first + 1;
			currentNodeGraphPtr->nodes[node_id_in]["Pins"][id_in]["link"] = link_id;
			sol::table links_out = currentNodeGraphPtr->nodes[node_id_out]["Pins"][id_out]["links"];
			links_out.add(link_id);
			sol::protected_function funcNew = baseNode["BaseLink"]["new"];
			auto result = funcNew(baseNode["BaseLink"], id_in, id_out);
			if (ScriptManager::checkProtected(result))
			{
				currentNodeGraphPtr->links.emplace(link_id, result);
			}
		}
	}
	
	ImGui::End();
}

void NodeGraph::callNode(sol::table& node, NodeGraph* data, Mission* mission)
{
	const sol::protected_function play = node["play"];
	auto result = play(node, data, mission);
	ScriptManager::checkProtected(result);
}
