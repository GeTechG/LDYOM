#include "create_new_component.h"
#include "components_manager.h"
#include "entities.h"
#include "entities_manager.h"
#include "fa_icons.h"
#include "localization.h"
#include "scenes_manager.h"
#include "utils/string_utils.h"
#include "window_manager.h"
#include <algorithm>
#include <imgui.h>
#include <imgui_widgets/imgui_widgets.h>
#include <sstream>
#include <unordered_set>

CreateNewComponent::CreateNewComponent()
	: ModalPopupWindow(_("create_new_component.title")) {
	this->m_searchQuery.fill(0);
	setSize(600, 500);
	setSizeMin(500, 400);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setFlags(ImGuiWindowFlags_NoCollapse);
	setRenderCallback<CreateNewComponent>(renderContent);
}

void CreateNewComponent::open() {
	ModalPopupWindow::open();
	initializeCategories();
	m_selectedType.clear();
	m_searchQuery.fill(0);
	// Invalidate cache when opening
	m_dependenciesCacheValid = false;
}

void CreateNewComponent::renderContent(CreateNewComponent* window) {
	// Check if selected entity changed to invalidate cache
	auto entitiesWindow = WindowManager::instance().getWindowAs<EntitiesWindow>("entities");
	int currentEntityIndex = (entitiesWindow.has_value()) ? entitiesWindow.value()->getSelectedEntityIndex() : -1;
	if (window->m_cachedEntityIndex != currentEntityIndex) {
		window->invalidateDependenciesCache();
	}

	window->renderSearchBox();

	window->renderComponentTree();

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
			// Get selected entity from EntitiesWindow and add component
			auto entitiesWindow = WindowManager::instance().getWindowAs<EntitiesWindow>("entities");
			if (entitiesWindow.has_value() && entitiesWindow.value()->getSelectedEntityIndex() >= 0) {
				auto& entity =
					EntitiesManager::instance().getUnsafeEntity(entitiesWindow.value()->getSelectedEntityIndex());
				auto component = ComponentsManager::instance().createComponent(window->m_selectedType);
				entity.addComponent(component);
			}
			window->close();
		}
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(center + (availableRegion.x - buttonsWidth) * 0.1f);
	if (ImGui::Button(cancelText.c_str())) {
		window->close();
	}
}

void CreateNewComponent::renderSearchBox() {
	ImGui::Text(_("create_new_component.search").c_str());
	ImGui::SameLine();

	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##SearchInput", this->m_searchQuery.data(), this->m_searchQuery.size());
	ImGui::SameLine();
	const float inputX = ImGui::GetCursorPosX();
	const auto iconSearchSize = ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS);
	ImGui::SetCursorPosX(inputX - iconSearchSize.x - ImGui::GetStyle().ItemSpacing.x * 2);
	ImGui::TextDisabled(ICON_FA_MAGNIFYING_GLASS);
}

void CreateNewComponent::renderComponentTree() {
	ImGui::Text(_("create_new_component.components").c_str());

	ImVec2 treeSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.7f);

	ImGui::BeginChild("ComponentTree", treeSize, ImGuiChildFlags_FrameStyle);

	// Get search query
	std::string searchStr(m_searchQuery.data());
	// StringUtils::toLower(searchStr);

	// Always render category tree, but filter based on search
	renderCategoryNode(m_rootCategory, 0, searchStr);

	ImGui::EndChild();
}

void CreateNewComponent::renderCategoryNode(const CategoryNode& node, int depth, const std::string& searchFilter) {
	// Check if this category or any of its children have available components that match the search filter
	bool shouldShowCategory = categoryMatchesFilter(node, searchFilter);

	if (!shouldShowCategory) {
		return; // Don't render this category if it has no available components
	}

	// Don't render the root node itself, but render its components and subcategories
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
			// Render components in this category
			renderComponentsInNode(node, searchFilter);

			// Render subcategories
			for (const auto& [subcategoryName, subcategoryNode] : node.subcategories) {
				renderCategoryNode(subcategoryNode, depth + 1, searchFilter);
			}

			ImGui::TreePop();
		}
	} else {
		// Root level - render components without category and subcategories
		renderComponentsInNode(node, searchFilter);

		for (const auto& [subcategoryName, subcategoryNode] : node.subcategories) {
			renderCategoryNode(subcategoryNode, depth + 1, searchFilter);
		}
	}
}

void CreateNewComponent::renderComponentsInNode(const CategoryNode& node, const std::string& searchFilter) {
	for (const std::string& componentType : node.componentTypes) {
		// Filter components based on search query
		if (!searchFilter.empty()) {
			std::string lowerComponentType = componentType;
			// StringUtils::toLower(lowerComponentType);

			if (lowerComponentType.find(searchFilter) == std::string::npos) {
				continue; // Skip this component if it doesn't match search
			}
		}

		// Use cached dependency check
		if (!isComponentAvailable(componentType)) {
			continue;
		}

		const bool isSelected = (m_selectedType == componentType);
		const std::string selectableId = fmt::format("##component_{}", componentType);

		if (ImGui::Selectable(selectableId.c_str(), isSelected, 0, ImVec2(0, 0))) {
			m_selectedType = componentType;
		}

		ImGui::SameLine();
		const auto componentTypeLocalized = _(fmt::format("components.{}.name", componentType));
		ImGui::Text("%s %s", ICON_FA_PUZZLE_PIECE, componentTypeLocalized.c_str());
	}
}

void CreateNewComponent::renderDescription() {
	ImGui::Text(_("create_new_component.description").c_str());

	ImVec2 descSize =
		ImVec2(ImGui::GetContentRegionAvail().x,
	           ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()); // Remaining space minus button

	ImGui::BeginChild("Description", descSize, true);

	if (m_selectedType.empty()) {
		ImGui::TextWrapped("%s", _("create_new_component.no_selection").c_str());
	} else {
		// Display component information
		const auto componentTypeLocalized = _(fmt::format("components.{}.name", m_selectedType));
		ImGui::Text("%s %s (%s)", ICON_FA_PUZZLE_PIECE, componentTypeLocalized.c_str(), m_selectedType.c_str());
		ImGui::Separator();

		// Try to get localized description
		std::string descriptionKey = "components." + m_selectedType + ".description";
		std::string description = _(descriptionKey);

		// If no localized description found, show default text
		if (description == descriptionKey) {
			description = _("create_new_component.no_description");
		}

		ImGui::TextWrapped("%s", description.c_str());

		ImGui::Spacing();

		// Show component category if available
		auto& componentBuilders = ComponentsManager::instance().getComponentBuilders();
		auto it = componentBuilders.find(m_selectedType);
		if (it != componentBuilders.end() && !it->second.category.empty()) {
			std::vector<std::string> categories = parseCategoryPath(it->second.category);
			ImGui::Text("%s: ", _("category").c_str());
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

std::vector<std::string> CreateNewComponent::parseCategoryPath(const std::string& categoryPath) {
	std::vector<std::string> categories;
	std::stringstream ss(categoryPath);
	std::string category;

	while (std::getline(ss, category, '.')) {
		if (!category.empty()) {
			categories.push_back(category);
		}
	}

	return categories;
}

void CreateNewComponent::initializeCategories() {
	m_rootCategory = CategoryNode{.name = "root"};

	auto& componentBuilders = ComponentsManager::instance().getComponentBuilders();

	for (auto& [typeName, builderData] : componentBuilders) {
		// Check if the component type has a category
		if (builderData.category.empty()) {
			// If no category, add to root
			m_rootCategory.componentTypes.push_back(typeName);
		} else {
			// Parse category path and add to proper node
			std::vector<std::string> categoryPath = parseCategoryPath(builderData.category);

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

			// Add the component type to the leaf category node
			currentNode->componentTypes.push_back(typeName);
		}
	}
}

bool CreateNewComponent::categoryMatchesFilter(const CategoryNode& node, const std::string& searchFilter) {
	// Check if any component in this category matches the filter AND is available
	for (const std::string& componentType : node.componentTypes) {
		// First check if component is available (dependencies satisfied)
		if (!isComponentAvailable(componentType)) {
			continue;
		}

		// Then check search filter if provided
		if (!searchFilter.empty()) {
			std::string lowerComponentType = componentType;
			// StringUtils::toLower(lowerComponentType);

			if (lowerComponentType.find(searchFilter) != std::string::npos) {
				return true;
			}
		} else {
			// No search filter, component is available
			return true;
		}
	}

	// Check if any subcategory has matching components
	for (const auto& [subcategoryName, subcategoryNode] : node.subcategories) {
		if (categoryMatchesFilter(subcategoryNode, searchFilter)) {
			return true;
		}
	}

	return false;
}

void CreateNewComponent::updateAvailableComponents() const {
	auto entitiesWindow = WindowManager::instance().getWindowAs<EntitiesWindow>("entities");
	if (!entitiesWindow.has_value() || entitiesWindow.value()->getSelectedEntityIndex() < 0) {
		m_availableComponentsSet.clear();
		m_dependenciesCacheValid = true;
		m_cachedEntityIndex = -1;
		return;
	}

	int currentEntityIndex = entitiesWindow.value()->getSelectedEntityIndex();

	// Check if cache is still valid
	if (m_dependenciesCacheValid && m_cachedEntityIndex == currentEntityIndex) {
		return;
	}

	// Clear and rebuild cache
	m_availableComponentsSet.clear();
	m_cachedEntityIndex = currentEntityIndex;

	auto& entity = EntitiesManager::instance().getUnsafeEntity(currentEntityIndex);

	// Get current entity's component types for faster lookup
	std::unordered_set<std::string> entityComponentTypes;
	for (const auto& component : entity.getComponents()) {
		entityComponentTypes.insert(component->getType());
	}

	auto& componentBuilders = ComponentsManager::instance().getComponentBuilders();

	// Check all component types for availability
	for (const auto& [componentType, builderData] : componentBuilders) {
		bool isAvailable = true;

		if (!builderData.dependencies.components.empty()) {
			if (builderData.dependencies.oneOf) {
				// Check if at least one dependency is satisfied
				bool oneOfSatisfied =
					std::any_of(builderData.dependencies.components.begin(), builderData.dependencies.components.end(),
				                [&entityComponentTypes](const std::string& dep) {
									return entityComponentTypes.find(dep) != entityComponentTypes.end();
								});
				isAvailable = oneOfSatisfied;
			} else {
				// Check if all dependencies are satisfied
				bool allSatisfied =
					std::all_of(builderData.dependencies.components.begin(), builderData.dependencies.components.end(),
				                [&entityComponentTypes](const std::string& dep) {
									return entityComponentTypes.find(dep) != entityComponentTypes.end();
								});
				isAvailable = allSatisfied;
			}
		}

		if (isAvailable) {
			m_availableComponentsSet.insert(componentType);
		}
	}

	m_dependenciesCacheValid = true;
}

bool CreateNewComponent::isComponentAvailable(const std::string& componentType) const {
	updateAvailableComponents();

	return m_availableComponentsSet.find(componentType) != m_availableComponentsSet.end();
}

void CreateNewComponent::invalidateDependenciesCache() const { m_dependenciesCacheValid = false; }
