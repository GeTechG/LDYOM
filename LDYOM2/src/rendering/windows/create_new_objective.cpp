#define IMGUI_DEFINE_MATH_OPERATORS
#include "create_new_objective.h"
#include <algorithm>
#include <fa_icons.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <localization.h>
#include <objectives_manager.h>
#include <ranges>
#include <sstream>
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

void CreateNewObjective::renderCategoryNode(const CategoryNode& node, int depth, const std::string& searchFilter) {
	// Check if this category or any of its children match the search filter
	bool shouldShowCategory = searchFilter.empty() || categoryMatchesFilter(node, searchFilter);

	if (!shouldShowCategory) {
		return; // Don't render this category if it doesn't match filter
	}

	// Don't render the root node itself, but render its objectives and subcategories
	if (depth > 0) {
		const std::string nodeId = fmt::format("category_{}_{}", node.name, depth);

		// If searching, force all categories to be open
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (!searchFilter.empty()) {
			flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			ImGui::SetNextItemOpen(true);
		}

		const auto categoryLocalized = _(fmt::format("categories.{}", node.name));
		if (ImGui::TreeNodeEx(nodeId.c_str(), flags, "%s %s", ICON_FA_FOLDER, categoryLocalized.c_str())) {
			// Render objectives in this category
			renderObjectivesInNode(node, searchFilter);

			// Render subcategories
			for (const auto& [subcategoryName, subcategoryNode] : node.subcategories) {
				renderCategoryNode(subcategoryNode, depth + 1, searchFilter);
			}

			ImGui::TreePop();
		}
	} else {
		// Root level - render objectives without category and subcategories
		renderObjectivesInNode(node, searchFilter);

		for (const auto& [subcategoryName, subcategoryNode] : node.subcategories) {
			renderCategoryNode(subcategoryNode, depth + 1, searchFilter);
		}
	}
}

void CreateNewObjective::renderObjectivesInNode(const CategoryNode& node, const std::string& searchFilter) {
	for (const std::string& objectiveType : node.objectiveTypes) {
		// Filter objectives based on search query
		if (!searchFilter.empty()) {
			std::string lowerObjectiveType = objectiveType;
			// StringUtils::toLower(lowerObjectiveType);

			if (lowerObjectiveType.find(searchFilter) == std::string::npos) {
				continue; // Skip this objective if it doesn't match search
			}
		}

		const bool isSelected = (m_selectedType == objectiveType);
		const std::string selectableId = fmt::format("##objective_{}", objectiveType);

		if (ImGui::Selectable(selectableId.c_str(), isSelected, 0, ImVec2(0, 0))) {
			m_selectedType = objectiveType;
		}
		ImGui::SameLine();
		const auto objectiveTypeLocalized = _(fmt::format("objectives.{}.name", objectiveType));
		ImGui::Text("%s %s", ICON_FA_BULLSEYE_ARROW, objectiveTypeLocalized.c_str());
	}
}

void CreateNewObjective::renderObjectiveTree() {
	ImGui::Text(_("create_new_objective.objectives").c_str());

	ImVec2 treeSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.7f);

	ImGui::BeginChild("ObjectiveTree", treeSize, ImGuiChildFlags_FrameStyle);

	// Get search query
	std::string searchStr(m_searchQuery.data());
	// StringUtils::toLower(searchStr);

	// Always render category tree, but filter based on search
	renderCategoryNode(m_rootCategory, 0, searchStr);

	ImGui::EndChild();
}

void CreateNewObjective::renderDescription() {
	ImGui::Text(_("create_new_objective.description").c_str());

	ImVec2 descSize =
		ImVec2(ImGui::GetContentRegionAvail().x,
	           ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()); // Remaining space minus button

	ImGui::BeginChild("Description", descSize, true);

	if (m_selectedType.empty()) {
		ImGui::TextWrapped("%s", _("create_new_objective.no_selection").c_str());
	} else { // Display objective information
		const auto objectiveTypeLocalized = _(fmt::format("objectives.{}.name", m_selectedType));
		ImGui::Text("%s %s (%s)", ICON_FA_BULLSEYE_ARROW, objectiveTypeLocalized.c_str(), m_selectedType.c_str());
		ImGui::Separator();

		// Try to get localized description
		std::string descriptionKey = "objectives." + m_selectedType + ".description";
		std::string description = _(descriptionKey);

		// If no localized description found, show default text
		if (description == descriptionKey) {
			description = _("create_new_objective.no_description");
		}

		ImGui::TextWrapped("%s", description.c_str());

		ImGui::Spacing();

		// Show objective category if available
		auto& objectiveBuilders = ObjectivesManager::instance().getObjectiveBuilders();
		auto it = objectiveBuilders.find(m_selectedType);
		if (it != objectiveBuilders.end() && !it->second.category.empty()) {
			std::vector<std::string> categories = parseCategoryPath(it->second.category);
			ImGui::Text("%s: ", _("create_new_component.category").c_str());
			for (size_t i = 0; i < categories.size(); ++i) {
				const std::string& category = categories[i];
				std::string localizedCategory = _(fmt::format("categories.{}", category));
				ImGui::Text("%s%s", localizedCategory.c_str(), (i < categories.size() - 1) ? " -> " : "");
				if (i < categories.size() - 1) {
					ImGui::SameLine(0, 0);
				}
			}
		}
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

bool CreateNewObjective::categoryMatchesFilter(const CategoryNode& node, const std::string& searchFilter) {
	// Check if any objective in this category matches the filter
	for (const std::string& objectiveType : node.objectiveTypes) {
		std::string lowerObjectiveType = objectiveType;
		// StringUtils::toLower(lowerObjectiveType);

		if (lowerObjectiveType.find(searchFilter) != std::string::npos) {
			return true;
		}
	}

	// Check if any subcategory has matching objectives
	for (const auto& [subcategoryName, subcategoryNode] : node.subcategories) {
		if (categoryMatchesFilter(subcategoryNode, searchFilter)) {
			return true;
		}
	}

	return false;
}

void CreateNewObjective::open() {
	ModalPopupWindow::open();
	initializeCategories();
	m_selectedType.clear();
	m_searchQuery.fill(0);
}

CreateNewObjective::CreateNewObjective()
	: ModalPopupWindow(_("create_new_objective.title")) {
	this->m_searchQuery.fill(0);
	setSize(600, 500);
	setSizeMin(500, 400);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_NoCollapse);
	setRenderCallback<CreateNewObjective>(renderContent);
}
