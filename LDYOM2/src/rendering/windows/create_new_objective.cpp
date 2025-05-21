#define IMGUI_DEFINE_MATH_OPERATORS
#include "create_new_objective.h"
#include <fa_icons.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <localization.h>
#include <objectives_manager.h>
#include <ranges>
#include <utils/imgui_configurate.h>

void CreateNewObjective::renderSearchBox() {
	ImGui::Text(_("create_new_objective.search").c_str());
	ImGui::SameLine();

	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##SearchInput", this->m_searchQuery.data(), this->m_searchQuery.size());
	ImGui::SameLine();
	const float inputX = ImGui::GetCursorPosX();
	const auto iconSearchSize = ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS);
	ImGui::SetCursorPosX(inputX - iconSearchSize.x - ImGui::GetStyle().ItemSpacing.x * 2);
	ImGui::TextDisabled(ICON_FA_MAGNIFYING_GLASS);
}

void CreateNewObjective::renderCategoryNode(const CategoryNode& node, int depth) {
	auto& objectiveTypes = ObjectivesManager::instance().getObjectiveBuilders();
	std::string searchQuery = this->m_searchQuery.data();
	bool hasSearchQuery = !searchQuery.empty();

	// First render subcategories
	for (const auto& [subCategoryName, subCategory] : node.subcategories) {
		// For categories other than root, create a tree node
		if (depth > 0) {
			// Check if any of this category's children (subcategories or objectives) match the search query
			bool shouldShow = !hasSearchQuery;

			if (hasSearchQuery) {
				// Check category name itself
				if (subCategoryName.find(searchQuery) != std::string::npos) {
					shouldShow = true;
				} else {
					// Check all objectives in this category and subcategories
					for (const auto& objType : subCategory.objectiveTypes) {
						auto objectiveTypeName = _("objectives." + objType + ".name");
						if (std::string(objectiveTypeName).find(searchQuery) != std::string::npos) {
							shouldShow = true;
							break;
						}
					}

					// Recursively check subcategories (simplified check - we don't check deep into the tree)
					if (!shouldShow) {
						for (const auto& [_, subSubCategory] : subCategory.subcategories) {
							for (const auto& objType : subSubCategory.objectiveTypes) {
								auto objectiveTypeName = _("objectives." + objType + ".name");
								if (std::string(objectiveTypeName).find(searchQuery) != std::string::npos) {
									shouldShow = true;
									break;
								}
							}
							if (shouldShow)
								break;
						}
					}
				}
			}

			if (!shouldShow)
				continue;

			// Set tree node flags
			ImGuiTreeNodeFlags nodeFlags = 0;

			if (hasSearchQuery) {
				nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
			}

			// Create a unique ID for the tree node to ensure proper state tracking
			std::string nodeId = subCategoryName + "##" + std::to_string(depth);

			// If it's a category that should be shown, create a tree node
			if (ImGui::TreeNodeEx(nodeId.c_str(), nodeFlags, "%s", subCategoryName.c_str())) {
				renderCategoryNode(subCategory, depth + 1);
				ImGui::TreePop();
			}
		} else {
			// For root, just recurse without creating a node
			renderCategoryNode(subCategory, depth + 1);
		}
	}

	// Then render objectives in this category
	for (const auto& objType : node.objectiveTypes) {
		// Skip if it doesn't match the search query
		auto objectiveTypeName = _("objectives." + objType + ".name");
		if (hasSearchQuery && std::string(objectiveTypeName).find(searchQuery) == std::string::npos) {
			continue;
		}

		// Add indent for objectives
		if (depth > 0) {
			ImGui::Indent();
		}

		bool isSelected = (this->m_selectedType == objType);
		if (ImGui::Selectable(objectiveTypeName.c_str(), isSelected)) {
			this->m_selectedType = objType;
		}

		if (isSelected) {
			ImGui::SetItemDefaultFocus();
		}

		// Remove indent
		if (depth > 0) {
			ImGui::Unindent();
		}
	}
}

void CreateNewObjective::renderObjectiveTree() {
	ImGui::Text(_("create_new_objective.objectives").c_str());

	ImVec2 treeSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.7f);

	ImGui::BeginChild("ObjectiveTree", treeSize, ImGuiChildFlags_FrameStyle);

	// Render the category tree starting from the root
	renderCategoryNode(m_rootCategory, 0);

	ImGui::EndChild();
}

void CreateNewObjective::renderDescription() {
	ImGui::Text(_("create_new_objective.description").c_str());

	ImVec2 descSize =
		ImVec2(ImGui::GetContentRegionAvail().x,
	           ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()); // Remaining space minus button

	ImGui::BeginChild("Description", descSize, true);

	auto& objectiveTypes = ObjectivesManager::instance().getObjectiveBuilders();
	if (objectiveTypes.contains(this->m_selectedType)) {
		const auto& objectiveType = objectiveTypes[this->m_selectedType];
		ImGui::Text(_("create_new_objective.type", _("objectives." + objectiveType.type + ".name").c_str()).c_str());
		ImGui::Separator();
		ImGui::TextWrapped(_("objectives." + objectiveType.type + ".description").c_str());
	}

	ImGui::EndChild();
}

void CreateNewObjective::renderContent(CreateNewObjective* window) {

	window->renderSearchBox();

	window->renderObjectiveTree();

	window->renderDescription();

	auto availableRegion = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + availableRegion.y - ImGui::GetFrameHeight());

	const auto createText = _("create");
	const auto cancelText = _("cancel");
	const auto center = availableRegion.x / 2.f;
	const auto buttonsWidth = ImGui::CalcTextSize(createText.c_str()).x + ImGui::CalcTextSize(cancelText.c_str()).x +
	                          ImGui::GetStyle().FramePadding.x * 4;

	ImGui::SetCursorPosX(center - ImGui::CalcTextSize(createText.c_str()).x - ImGui::GetStyle().FramePadding.x * 2 -
	                     (availableRegion.x - buttonsWidth) * 0.1f);
	if (ImGui::Button(createText.c_str())) {
		if (!window->m_selectedType.empty()) {
			ObjectivesManager::instance().addNewObjective(window->m_selectedType);
			window->close();
		}
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(center + (availableRegion.x - buttonsWidth) * 0.1f);
	if (ImGui::Button(cancelText.c_str())) {
		window->close();
	}
}

void CreateNewObjective::initializeCategories() {
	m_rootCategory = CategoryNode{.name = "root"};

	auto& objectiveTypes = ObjectivesManager::instance().getObjectiveBuilders();

	for (auto& [typeName, objectiveType] : objectiveTypes) {
		// Check if the objective type has a category
		if (objectiveType.category.empty()) {
			// If no category, add to root
			m_rootCategory.objectiveTypes.push_back(typeName);
		} else {
			// Parse category path and add to proper node
			std::vector<std::string> categoryPath = parseCategoryPath(objectiveType.category);

			CategoryNode* currentNode = &m_rootCategory;

			// Build the category tree
			for (size_t i = 0; i < categoryPath.size(); i++) {
				const std::string& categoryName = categoryPath[i];

				// If category doesn't exist yet, create it
				if (currentNode->subcategories.find(categoryName) == currentNode->subcategories.end()) {
					currentNode->subcategories[categoryName] = CategoryNode{.name = categoryName};
				}

				currentNode = &currentNode->subcategories[categoryName];
			}

			// Add the objective type to the leaf category node
			currentNode->objectiveTypes.push_back(typeName);
		}
	}
}

std::vector<std::string> CreateNewObjective::parseCategoryPath(const std::string& categoryPath) {
	std::vector<std::string> result;
	std::string currentCategory;

	for (char c : categoryPath) {
		if (c == '.') {
			if (!currentCategory.empty()) {
				result.push_back(currentCategory);
				currentCategory.clear();
			}
		} else {
			currentCategory += c;
		}
	}

	// Add the last category if not empty
	if (!currentCategory.empty()) {
		result.push_back(currentCategory);
	}

	return result;
}

void CreateNewObjective::open() {
	ModalPopupWindow::open();
	this->m_searchQuery.fill(0);
	initializeCategories();
}

CreateNewObjective::CreateNewObjective()
	: ModalPopupWindow(_("create_new_objective.title").c_str()) {
	this->m_searchQuery.fill(0);
	setSize(600, 500);
	setSizeMin(500, 400);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_NoCollapse);
	setRenderCallback<CreateNewObjective>(renderContent);
}
