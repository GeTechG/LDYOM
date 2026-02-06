#pragma once
#include <functional>
#include <model_renderer.h>
#include <object_tags_manager.h>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class PopupObjectSelector {
  private:
	static bool s_isOpen;
	static bool s_filterApplied;
	static std::optional<ModelRenderer> s_iconRenderer;
	static std::optional<ModelRenderer> s_previewRenderer;

	static std::vector<ObjectTagsManager::ObjectModelInfo> s_queryResults;
	static std::unordered_map<int, std::string> s_modelNames;
	static std::set<std::string> s_loadedIcons;

	static int s_selectedModel;
	static bool s_previewNeedRender;

	// 0 = filter selection screen, 1 = search results, 2 = tag results
	static int s_selectorState;

	struct FilterBehavior {
		int hasCollision = -1; // -1 = any, 0 = no, 1 = yes
		int destroyByHit = -1;
		int seeByTime    = -1;
	};
	static FilterBehavior s_filterBehavior;
	static int s_selectedTagId;
	static std::set<int> s_selectedCategories;

	static Texture* getModelIcon(int model);
	static void clearLoadedIcons();
	static void applyFilter(const char* search);
	static void drawPreviewModal(const std::function<void(int)>& onSelectCallback);
	static bool drawCategoryNode(const std::vector<ObjectModelCategory>& cats, int parentId, bool parentSelected);

	static void drawTagsPanel(float blockWidth);
	static void drawBehaviorsPanel(float blockWidth);
	static void drawCategoriesPanel(float blockWidth);
	static void drawResultsView(const char* searchText, const std::function<void(int)>& onSelectCallback);

  public:
	static void showPopup();
	static void renderPopup(const std::function<void(int)>& onSelectCallback);
};
