#pragma once
#include "modal_popup_window.h"
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class CreateNewObjective : public ModalPopupWindow {
  private:
	std::array<char, 256> m_searchQuery;
	std::string m_selectedType;

	// For the category-based organization
	struct CategoryNode {
		std::string name;
		std::unordered_map<std::string, CategoryNode> subcategories;
		std::vector<std::string> objectiveTypes;
	};

	CategoryNode m_rootCategory;

	// Initialize category structure from ObjectivesManager
	void initializeCategories();
	// Helper functions for UI
	void renderSearchBox();
	void renderObjectiveTree();
	void renderCategoryNode(const CategoryNode& node, int depth, const std::string& searchFilter);
	void renderObjectivesInNode(const CategoryNode& node, const std::string& searchFilter);
	void renderDescription();

	std::vector<std::string> parseCategoryPath(const std::string& categoryPath);
	bool categoryMatchesFilter(const CategoryNode& node, const std::string& searchFilter);

	static void renderContent(CreateNewObjective* window);

  public:
	CreateNewObjective();
	~CreateNewObjective() override = default;

	void open() override;
};