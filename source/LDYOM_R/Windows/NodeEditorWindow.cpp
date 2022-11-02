#include "NodeEditorWindow.h"

#include <fmt/core.h>

#include "fa.h"
#include "imgui.h"
#include "LuaEngine.h"
#include "LuaLogger.h"
#include "NodeEditorUtils.h"
#include "ProjectsService.h"
#include "easylogging/easylogging++.h"
#include "imnodes/imnodes.h"
#include "Localization/Localization.h"
#include <boost/algorithm/string.hpp>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "Settings.h"

void Windows::NodeEditorWindow::newNodePopup(sol::table globalData, sol::table ed, bool* isCreateNode) {
	if (ImGui::BeginPopup("newNode")) {

		static char searchBuf[NAME_SIZE] = "";

		static std::map<std::string, std::vector<int>> searchedCache;

		std::optional<std::string> description;

		ImGui::BeginGroup();

		if (ImGui::InputText("##searchNodes", searchBuf, std::size(searchBuf))) {
			if (const size_t searchBufLen = std::strlen(searchBuf); searchBufLen > 0) {
				searchedCache.clear();
				const auto searchBufLower = boost::algorithm::to_lower_copy(std::string(searchBuf));
				for (auto& category : globalData["nodes_categories"].get_or_create<sol::table>()) {
					for (auto pair : category.second.as<sol::table>()) {
						const auto nameNode = Localization::getInstance().get(globalData["nodes"][pair.second.as<int>()]["name_"].get<const char*>());
						if (boost::algorithm::contains(boost::algorithm::to_lower_copy(nameNode), searchBufLower)) {
							if (!searchedCache.contains(category.first.as<std::string>())) {
								searchedCache[category.first.as<std::string>()] = std::vector<int>();
							}
							auto& vCategory = searchedCache[category.first.as<std::string>()];
							vCategory.emplace_back(pair.second.as<int>());
						}
					}
				}
			}
		}
		if (ImGui::BeginChild("##categories", ImVec2(ImGui::GetFontSize() * 16.f, ImGui::GetFontSize() * 25.f))) {
			static bool opened = false;
			const size_t searchBufLen = std::strlen(searchBuf);

			auto nodeClassMenuItem = [&globalData, &ed, &isCreateNode, &description, this](int nodeId) {
				auto nodeClass = globalData["nodes"][nodeId].get<sol::table>();
				const auto nameNode = Localization::getInstance().get(nodeClass.get<const char*>("name_"));
				ImGui::PushID(nodeId);
				if (ImGui::SmallButton(ICON_FA_STAR)) {
					if (this->favoritesNodes_.contains(nodeId)) {
						this->favoritesNodes_.erase(nodeId);
					}
					else {
						this->favoritesNodes_.emplace(nodeId);
					}
					saveFavoritesNodes();
				}
				ImGui::PopID();
				ImGui::SameLine();
				if (ImGui::MenuItem(fmt::format("{} {}", NodeEditorUtils::getIcon(static_cast<NodeType>(nodeClass["static"]["type"].get<int>())), nameNode).c_str())) {
					if (auto result = ed["newNode"](ed, nodeClass); !result.valid()) {
						sol::error err = result;
						CLOG(ERROR, "lua") << err.what();
						LuaLogger::getInstance().print(err.what());
					}
					else {
						*isCreateNode = true;
						ImGui::CloseCurrentPopup();
					}
				}
				if (ImGui::IsItemHovered()) {
					if (auto descr = nodeClass["static"]["description"]; descr.is<std::string>()) {
						description = descr.get<std::string>();
					}
				}
			};

			if (searchBufLen == 0) {
				if (ImGui::TreeNode(Localization::getInstance().get("core.categories.variables").c_str())) {
					if (ImGui::TreeNode(Localization::getInstance().get("core.nodes.variable_get").c_str())) {
						for (auto value : globalData["variables"].get_or_create<sol::table>()) {
							const auto name = static_cast<char*>(const_cast<void*>(value.second.as<sol::table>()["name"].get<sol::object>().pointer()));
							if (ImGui::MenuItem(name)) {
								if (auto result = ed["newNode"](ed, globalData["GetVariableNode"], value.first); !result.valid()) {
									sol::error err = result;
									CLOG(ERROR, "lua") << err.what();
									LuaLogger::getInstance().print(err.what());
								}
								else {
									*isCreateNode = true;
									ImGui::CloseCurrentPopup();
								}
							}
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNode(Localization::getInstance().get("core.nodes.variable_set").c_str())) {
						for (auto value : globalData["variables"].get_or_create<sol::table>()) {
							const auto name = static_cast<char*>(const_cast<void*>(value.second.as<sol::table>()["name"].get<sol::object>().pointer()));
							if (ImGui::MenuItem(name)) {
								if (auto result = ed["newNode"](ed, globalData["SetVariableNode"], value.first); !result.valid()) {
									sol::error err = result;
									CLOG(ERROR, "lua") << err.what();
									LuaLogger::getInstance().print(err.what());
								}
								else {
									*isCreateNode = true;
									ImGui::CloseCurrentPopup();
								}
							}
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode(Localization::getInstance().get("core.categories.favorites").c_str())) {
					std::set clone(this->favoritesNodes_);
					for (int favoriteNode : clone) {
						nodeClassMenuItem(favoriteNode);
					}
					ImGui::TreePop();
				}
				for (auto& category : globalData["sorted_nodes_categories_names"].get_or_create<sol::table>()) {
					if (opened)
						ImGui::SetNextItemOpen(false);
					if (ImGui::TreeNode(Localization::getInstance().get(category.second.as<std::string>()).c_str())) {
						for (auto pair : globalData["nodes_categories"][category.second].get<sol::table>()) {
							nodeClassMenuItem(pair.second.as<int>());
						}
						ImGui::TreePop();
					}
				}
				if (opened)
					opened = false;
			} else {
				for (auto& cache : searchedCache) {
					if (!opened)
						ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode(Localization::getInstance().get(cache.first).c_str())) {
						for (auto& node : cache.second) {
							nodeClassMenuItem(node);
						}
						ImGui::TreePop();
					}
				}
				if (!opened)
					opened = true;
			}
		}
		ImGui::EndChild();
		ImGui::EndGroup();

		if (description.has_value()) {
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 20.f);
			ImGui::Text(Localization::getInstance().get(description.value()).c_str());
		}

		ImGui::EndPopup();
	}
}

void Windows::NodeEditorWindow::saveFavoritesNodes() {
	std::vector<std::string> favoritesNodes{};
	const auto nodes = LuaEngine::getInstance().getLuaState()["global_data"]["nodes"].get_or_create<sol::table>();
	for (int favoriteNode : this->favoritesNodes_) {
		favoritesNodes.emplace_back(nodes[favoriteNode].get<sol::table>()["name"].get<std::string>());
	}
	Settings::getInstance().set("main.favoritesNodes", favoritesNodes);
	Settings::getInstance().Save();
}

Windows::NodeEditorWindow::NodeEditorWindow() {
	const auto listNodesName = Settings::getInstance().get<std::vector<std::string>>("main.favoritesNodes");
	if (listNodesName.has_value()) {
		const auto nodes = LuaEngine::getInstance().getLuaState()["global_data"]["nodes"].get_or_create<sol::table>();
		for (auto node : nodes) {
			for (auto value : listNodesName.value()) {
				if (strcmp(node.second.as<sol::table>()["name"].get<const char*>(), value.c_str()) == 0) {
					this->favoritesNodes_.emplace(node.first.as<int>());
				}
			}
		}
	}
}

void nodeContextPopup(sol::table ed, sol::table node) {
	auto&& local = Localization::getInstance();
	if (ImGui::BeginPopup("contextNode")) {

		if (ImGui::MenuItem(local.get("general.delete").c_str())) {
			if (const auto result = ed["deleteNode"](ed, node["id"].get<int>()); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}
		if (ImGui::BeginMenu(local.get("other.move_to").c_str())) {

			for (auto pair : ed["workspaces"].get<sol::table>()) {
				const auto workspaceName = static_cast<const char*>(const_cast<void*>(pair.second.as<sol::table>()["name"].get<sol::object>().pointer()));
				if (ImGui::MenuItem(workspaceName)) {
					auto result = ed["moveNodeToWorkspace"](ed, node["id"].get<int>(), pair.first);
					if (!result.valid()) {
						sol::error err = result;
						CLOG(ERROR, "lua") << err.what();
					}
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

void Windows::NodeEditorWindow::draw() {
	static auto& local = Localization::getInstance();
	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Once, ImVec2(.5f, .5f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 50.f, ImGui::GetFontSize() * 37.5f), ImGuiCond_Once);

	static std::optional<int> sDroppedPin;

	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_PROJECT_DIAGRAM, local.get("node_editor.title")).c_str(), nullptr, ImGuiWindowFlags_NoCollapse)) {
		int currentSceneIndex = ProjectsService::getInstance().getCurrentProject().getCurrentSceneIndex();
		auto globalData = LuaEngine::getInstance().getLuaState()["global_data"];
		auto ed = globalData["ed_contexts"][currentSceneIndex];

		if (ImGui::BeginChild("##workspacesChid", ImVec2(ImGui::GetFontSize() * 15.f, 0.f), true)) {
			if (ImGui::BeginListBox("##workspacesList", ImVec2(ImGui::GetFontSize() * 15.f - 4.f, -ImGui::GetFontSize() * 1.8f))) {

				for (auto pair : ed["workspaces"].get<sol::table>()) {
					if (ImGui::Selectable(static_cast<const char*>(const_cast<void*>(pair.second.as<sol::table>()["name"].get<sol::object>().pointer())), pair.first.as<int>() == ed["currentWorkspace"].get<int>())) {
						ed["currentWorkspace"] = pair.first;
					}
					if (ImGui::BeginPopupContextItem()) {
						ImGui::InputText("##nameWorkspace", static_cast<char*>(const_cast<void*>(pair.second.as<sol::table>()["name"].get<sol::object>().pointer())), TEXT_SIZE);
						ImGui::EndPopup();
					}
				}

				ImGui::EndListBox();
			}
			if (ImGui::Button(local.get("general.create").c_str(), ImVec2(ImGui::GetFontSize() * 5.f, 0.f))) {
				if (auto result = ed["newWorkspace"](ed); !result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
				}
			}
			ImGui::SameLine();
			ImGui::BeginDisabled(ed["workspaces"].get<sol::table>().size() <= 1);
			if (ImGui::Button(local.get("general.delete").c_str(), ImVec2(ImGui::GetFontSize() * 5.f, 0.f))) {
				ImGui::OpenPopup(local.get("warning_popup.title").c_str());
			}
			ImGui::EndDisabled();

			if (ImGui::BeginPopupModal(local.get("warning_popup.title").c_str())) {

				ImGui::Text(local.get("warning_popup.text").c_str());
				if (ImGui::Button(local.get("warning_popup.yes").c_str(), ImVec2(ImGui::GetFontSize() * 6.f, 0.f))) {
					ed["deleteWorkspace"](ed);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button(local.get("warning_popup.no").c_str(), ImVec2(ImGui::GetFontSize() * 6.f, 0.f))) {
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImNodesEditorContext* context = ed["workspaces"][ed["currentWorkspace"]]["context"];
		ImNodes::EditorContextSet(context);

		ImNodes::BeginNodeEditor();

		auto nodes = ed["nodes"].get<sol::table>();
		for (auto pair : nodes) {
			auto node = pair.second.as<sol::table>();
			if (node["workspace"].get<int>() == ed["currentWorkspace"].get<int>()) {
				auto result = node["draw"](pair.second);
				if (!result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
					LuaLogger::getInstance().print(err.what());
				}
				if (node["new_pos"].valid()) {
					ImVec2& pos = node["new_pos"];
					ImNodes::SetNodeEditorSpacePos(node["id"].get<int>(), pos);
					node["new_pos"] = sol::nil;
				}
			}
		}

		auto links = ed["links"].get<sol::table>();
		for (auto pair : links) {
			auto link = pair.second.as<sol::table>();
			if (link["workspace"].get<int>() == ed["currentWorkspace"].get<int>()) {
				auto pinIn = ed["getPin"](ed, link["pinInId"].get<int>());
				if (!pinIn.valid()) {
					sol::error err = pinIn;
					CLOG(ERROR, "lua") << err.what();
				}
				auto colorLink = NodeEditorUtils::getColorByType(static_cast<ValueType>(pinIn.get<sol::table>()["valueType"].get<int>()));
				const auto blend = ImGui::GetStyleColorVec4(ImGuiCol_Text) * ImVec4(255.f, 255.f, 255.f, 255.f);
				ImNodes::PushColorStyle(ImNodesCol_Link, colorLink);
				ImNodes::PushColorStyle(ImNodesCol_LinkHovered, ImAlphaBlendColors(colorLink, IM_COL32(static_cast<int>(blend.x), static_cast<int>(blend.y), static_cast<int>(blend.z), 20)));
				ImNodes::PushColorStyle(ImNodesCol_LinkSelected, ImAlphaBlendColors(colorLink, IM_COL32(static_cast<int>(blend.x), static_cast<int>(blend.y), static_cast<int>(blend.z), 20)));
				ImNodes::Link(pair.first.as<int>(), link["pinInId"].get<int>(), link["pinOutId"].get<int>());
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
			}
		}

		ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);

		bool openNewNodePopup = ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right);

		ImNodes::EndNodeEditor();

		int startedPinId;
		if (ImNodes::IsLinkStarted(&startedPinId)) {
			auto node = nodes[startedPinId / 100 * 100].get<sol::table>();
			const auto pinValueType = static_cast<ValueType>(node["pins"][startedPinId % 100].get<sol::table>()["valueType"].get<int>());
			const auto pinColor = NodeEditorUtils::getColorByType(pinValueType);
			const auto blend = ImGui::GetStyleColorVec4(ImGuiCol_Text) * ImVec4(255.f, 255.f, 255.f, 255.f);
			ImNodes::GetStyle().Colors[ImNodesCol_Link] = pinColor;
			ImNodes::GetStyle().Colors[ImNodesCol_LinkHovered] = ImAlphaBlendColors(pinColor, IM_COL32(static_cast<int>(blend.x), static_cast<int>(blend.y), static_cast<int>(blend.z), 20));
			ImNodes::GetStyle().Colors[ImNodesCol_LinkSelected] = ImAlphaBlendColors(pinColor, IM_COL32(static_cast<int>(blend.x), static_cast<int>(blend.y), static_cast<int>(blend.z), 20));
		}

		int droppedPin;
		if (ImNodes::IsLinkDropped(&droppedPin, false)) {
			auto result = ed["IsPinHaveValueType"](ed, droppedPin, static_cast<int>(Out), static_cast<int>(Void));
			if (!result.valid()) {
				sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
			if (result.get<bool>()) {
				sDroppedPin = droppedPin;
				ImGui::OpenPopup("newNode");
			}
		}

		int startedNode, startedPin, endNode, endPin;
		if (ImNodes::IsLinkCreated(&startedNode, &startedPin, &endNode, &endPin)) {
			if (const auto result = ed["createLink"](ed, startedNode, startedPin, endNode, endPin); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}

		int linkDestroyed;
		if (ImNodes::IsLinkDestroyed(&linkDestroyed)) {
			auto result = ed["deleteLink"](ed, linkDestroyed);
			if (!result.valid()) {
				sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}

		static int nodeContextSelected;

		int pinHovered;
		int nodeHovered;
		if (ImNodes::IsPinHovered(&pinHovered)) {
			auto node = nodes[pinHovered / 100 * 100].get<sol::table>();
			const auto pinValueType = static_cast<ValueType>(node["pins"][pinHovered % 100].get<sol::table>()["valueType"].get<int>());
			ImGui::SetTooltip(toString(pinValueType));
		} else if (ImNodes::IsNodeHovered(&nodeHovered)) {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				nodeContextSelected = nodeHovered;
				ImGui::OpenPopup("contextNode");
			}
		} else if (openNewNodePopup) {
			ImGui::OpenPopup("newNode");
		}
		static bool isCreatedNode = false;
		if (ImGui::IsPopupOpen("newNode"))
			newNodePopup(globalData, ed, &isCreatedNode);
		else {
			if (sDroppedPin.has_value() && isCreatedNode) {
				auto lastNodeId = ed["getLastNodeId"](ed);
				auto pin = ed["findPinByValueType"](ed, lastNodeId, static_cast<int>(PinType::In), static_cast<int>(ValueType::Void));
				if (!pin.valid()) {
					sol::error err = pin;
					CLOG(ERROR, "lua") << err.what();
				}
				else {
					if (pin.get_type() != sol::type::nil) {
						auto pinId = pin.get<sol::table>()["id"].get<int>();
						if (auto result = ed["createLink"](ed, sDroppedPin.value() / 100 * 100, sDroppedPin.value(), pinId / 100 * 100, pinId); !result.valid()) {
							const sol::error err = result;
							CLOG(ERROR, "lua") << err.what();
						}
					}
				}
			}
			isCreatedNode = false;
			sDroppedPin = std::nullopt;
		}
		
		if (ImGui::IsPopupOpen("contextNode"))
			nodeContextPopup(ed, ed["nodes"][nodeContextSelected].get<sol::table>());
		
	}
	ImGui::End();
}

void Windows::NodeEditorWindow::close() {
	AbstractWindow::close();
	FindPlayerPed()->m_pIntelligence->ClearTasks(true, true);
}
