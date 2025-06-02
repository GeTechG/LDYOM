#pragma once
#include "modal_popup_window.h"
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class CreateNewComponent : public ModalPopupWindow {
  private:
	std::array<char, 256> m_searchQuery;
	std::string m_selectedType;

	// For the category-based organization
	struct CategoryNode {
		std::string name;
		std::unordered_map<std::string, CategoryNode> subcategories;
		std::vector<std::string> componentTypes;
	};

	CategoryNode m_rootCategory;

	// Initialize category structure from ComponentsManager
	void initializeCategories();
	// Helper functions for UI
	void renderSearchBox();
	void renderComponentTree();
	void renderCategoryNode(const CategoryNode& node, int depth, const std::string& searchFilter);
	void renderComponentsInNode(const CategoryNode& node, const std::string& searchFilter);
	void renderDescription();

	// Check if category or its children match the search filter
	bool categoryMatchesFilter(const CategoryNode& node, const std::string& searchFilter);

	std::vector<std::string> parseCategoryPath(const std::string& categoryPath);

	static void renderContent(CreateNewComponent* window);

  public:
	CreateNewComponent();
	~CreateNewComponent() override = default;

	void open() override;
};
