#include "object_selector.h"
#include <imgui_hook/utils/imgui_configurate.h>
#include <utils/ui_scale.h>
#include <CModelInfo.h>
#include <CObjectData.h>
#include <CStreaming.h>
#include <fa_icons.h>
#include <fmt/format.h>
#include <imgui.h>
#include <localization.h>
#include <logger.h>
#include <models_manager.h>
#include <object_tags_manager.h>
#include <textures_manager.h>

bool PopupObjectSelector::s_isOpen = false;
bool PopupObjectSelector::s_filterApplied = false;
std::optional<ModelRenderer> PopupObjectSelector::s_iconRenderer = std::nullopt;
std::optional<ModelRenderer> PopupObjectSelector::s_previewRenderer = std::nullopt;

std::vector<ObjectTagsManager::ObjectModelInfo> PopupObjectSelector::s_queryResults;
std::unordered_map<int, std::string> PopupObjectSelector::s_modelNames;
std::set<std::string> PopupObjectSelector::s_loadedIcons;

int PopupObjectSelector::s_selectedModel = 0;
bool PopupObjectSelector::s_previewNeedRender = false;
int PopupObjectSelector::s_selectorState = 0;
PopupObjectSelector::FilterBehavior PopupObjectSelector::s_filterBehavior;
int PopupObjectSelector::s_selectedTagId = -1;
std::set<int> PopupObjectSelector::s_selectedCategories;

static bool s_addingCategory = false;
static int s_addCategoryParentId = -1;
static char s_addCategoryBuf[64] = "";

static constexpr const char* PREVIEW_TEX_NAME = "ObjectSelectorPreview";

void PopupObjectSelector::applyFilter(const char* search) {
	// Query database with all filters applied
	s_queryResults = ObjectTagsManager::instance().queryObjectModels(
		search, s_filterBehavior.hasCollision, s_filterBehavior.destroyByHit, s_filterBehavior.seeByTime,
		s_selectedTagId, s_selectedCategories);

	// Cache model names from results for tooltips
	s_modelNames.clear();
	for (const auto& info : s_queryResults) {
		s_modelNames[info.modelId] = info.modelName;
	}
}

Texture* PopupObjectSelector::getModelIcon(int model) {
	const auto texName = fmt::format("ObjIcon_{}", model);
	if (TexturesManager::instance().hasTexture(texName))
		return TexturesManager::instance().getTexture(texName).value();

	// Try to load icon from database
	auto iconData = ObjectTagsManager::instance().getModelIcon(model);
	if (!iconData.empty()) {
		auto result = TexturesManager::instance().loadTextureFromMemory(iconData, texName);
		if (result) {
			s_loadedIcons.insert(texName);
			return TexturesManager::instance().getTexture(texName).value();
		} else {
			LDYOM_ERROR("Failed to load icon texture for model {} from database", model);
		}
	}

	// Fallback: render model if no icon in database
	if (!s_iconRenderer.has_value()) {
		s_iconRenderer = ModelRenderer(CVector(0.f, 0.f, 0.f), 128);
		s_iconRenderer.value().init();
		s_iconRenderer.value().getZoomKoef() = 3.0f;
	}

	TexturesManager::instance().addTexture(texName, std::make_unique<RenderwareTexture>());
	auto newTexture = TexturesManager::instance().getTexture(texName).value();

	s_iconRenderer.value().render(model, dynamic_cast<RenderwareTexture*>(newTexture));
	s_loadedIcons.insert(texName);

	return newTexture;
}

void PopupObjectSelector::clearLoadedIcons() {
	for (const auto& iconName : s_loadedIcons) {
		TexturesManager::instance().removeTexture(iconName);
	}
	s_loadedIcons.clear();
}

void PopupObjectSelector::showPopup() {
	const auto popupName = _("object_selector.title", ICON_FA_CUBE);
	s_isOpen = true;
	s_selectorState = 0;
	s_filterApplied = false;
	s_addingCategory = false;
	ImGui::OpenPopup(popupName.c_str());
}

void PopupObjectSelector::drawPreviewModal(const std::function<void(int)>& onSelectCallback) {
	static bool s_editTitleInit = false;
	static char s_editTitleBuf[256] = "";

	ImGui::SetNextWindowSize(ui::em(37.5f, 31.25f), ImGuiCond_Appearing);
	if (!ImGui::BeginPopupModal("##objectPreview", nullptr, ImGuiWindowFlags_NoTitleBar))
		return;

	if (!s_previewRenderer.has_value()) {
		s_previewRenderer = ModelRenderer(CVector(-10.0f, 0.f, -135.0f), 256);
		s_previewRenderer.value().init();
	}

	if (s_previewNeedRender) {
		if (!TexturesManager::instance().hasTexture(PREVIEW_TEX_NAME)) {
			TexturesManager::instance().addTexture(PREVIEW_TEX_NAME, std::make_unique<RenderwareTexture>());
		}
		auto previewTex = TexturesManager::instance().getTexture(PREVIEW_TEX_NAME).value();
		auto rwTex = dynamic_cast<RenderwareTexture*>(previewTex);
		if (rwTex) {
			s_previewRenderer.value().render(s_selectedModel, rwTex);
		}
		s_previewNeedRender = false;
	}

	// --- Top section: Preview + Info side by side ---
	const auto previewTex = TexturesManager::instance().getTexture(PREVIEW_TEX_NAME);
	if (previewTex.has_value()) {
		// Left: Preview image
		ImGui::BeginChild("##previewLeft", ImVec2(256.f, 256.f), false);
		ImGui::ImageButton("##previewImg", (ImTextureID)previewTex.value()->getTexture(), ImVec2(256.f, 256.f));

		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
				s_previewRenderer.value().getRotateModel().z += ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).x;
				s_previewRenderer.value().getRotateModel().x -= ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).y;
				ImGui::ResetMouseDragDelta();
				s_previewNeedRender = true;
			}
			if (std::abs(ImGui::GetIO().MouseWheel) > FLT_EPSILON) {
				s_previewRenderer.value().getZoomKoef() += ImGui::GetIO().MouseWheel * 0.1f;
				s_previewNeedRender = true;
			}
		}
		ImGui::EndChild();

		// Right: Info panel
		ImGui::SameLine();
		ImGui::BeginChild("##previewRight", ImVec2(0.f, 256.f), false);

		ImGui::Text("%s: %d", _("object_selector.model_id").c_str(), s_selectedModel);
		if (const auto nameIt = s_modelNames.find(s_selectedModel); nameIt != s_modelNames.end())
			ImGui::Text("%s: %s", _("object_selector.model_name").c_str(), nameIt->second.c_str());

		ImGui::Separator();

		// title_name
		{
			std::string title = ObjectTagsManager::instance().getTitleName(s_selectedModel);
			std::string displayName =
				title.empty() ? (s_modelNames.count(s_selectedModel) ? s_modelNames[s_selectedModel] : std::string{})
							  : std::move(title);
			ImGui::Text("%s:", _("object_selector.title_name").c_str());
			ImGui::SameLine();
			ImGui::TextWrapped("%s", displayName.c_str());
			if (ImGui::Button(ICON_FA_PENCIL " Edit Title")) {
				s_editTitleInit = true;
				ImGui::OpenPopup("##editTitleName");
			}
		}

		ImGui::Separator();

		// behaviors info
		{
			auto info = CModelInfo::GetModelInfo(s_selectedModel);
			bool hasCol = info->m_pColModel->m_bHasCollisionVolumes > 0;
			bool destroyHit = info->m_nObjectInfoIndex != -1 &&
			                  CObjectData::ms_aObjectInfo[info->m_nObjectInfoIndex].m_nColDamageEffect != 0;
			bool seeTime = info && info->GetModelType() == MODEL_INFO_TIME;

			ImGui::Text("%s: %s", _("object_selector.collision").c_str(),
			            hasCol ? _("object_selector.beh_yes").c_str() : _("object_selector.beh_no").c_str());
			ImGui::Text("%s: %s", _("object_selector.destroy_by_hit").c_str(),
			            destroyHit ? _("object_selector.beh_yes").c_str() : _("object_selector.beh_no").c_str());
			ImGui::Text("%s: %s", _("object_selector.see_by_time").c_str(),
			            seeTime ? _("object_selector.beh_yes").c_str() : _("object_selector.beh_no").c_str());
		}

		ImGui::EndChild();
	}

	// --- Tags section ---
	ImGui::Separator();
	ImGui::Text("%s:", _("object_selector.tags").c_str());

	if (ImGui::BeginChild("##tagsSection", ImVec2(0.f, -30.f), true)) {
		static char previewNewTagBuf[64] = "";
		const auto& tags = ObjectTagsManager::instance().getTags();
		auto modelTags = ObjectTagsManager::instance().getModelTags(s_selectedModel);

		// Display tags in columns for better layout
		const int columns = 3;
		if (ImGui::BeginTable("##tagsTable", columns)) {
			int col = 0;
			for (const auto& tag : tags) {
				ImGui::TableNextColumn();
				bool checked = modelTags.count(tag.id) > 0;
				if (ImGui::Checkbox(_(tag.name).c_str(), &checked)) {
					if (checked)
						ObjectTagsManager::instance().setModelTag(s_selectedModel, tag.id);
					else
						ObjectTagsManager::instance().clearModelTag(s_selectedModel, tag.id);
				}
			}
			ImGui::EndTable();
		}

		ImGui::Separator();
		ImGui::SetNextItemWidth(-50.f);
		ImGui::InputTextWithHint("##previewNewTag", _("object_selector.tag_input_hint").c_str(), previewNewTagBuf,
		                         sizeof(previewNewTagBuf));
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PLUS "##addTagPreview")) {
			if (strlen(previewNewTagBuf) > 0) {
				auto newId = ObjectTagsManager::instance().addTag(previewNewTagBuf);
				if (newId.has_value())
					ObjectTagsManager::instance().setModelTag(s_selectedModel, newId.value());
				previewNewTagBuf[0] = '\0';
			}
		}
	}
	ImGui::EndChild();

	// --- Bottom buttons ---
	if (ImGui::Button(_("object_selector.close").c_str(), ImVec2(120.f, 0.f))) {
		ImGui::CloseCurrentPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button(_("object_selector.select").c_str(), ImVec2(120.f, 0.f))) {
		onSelectCallback(s_selectedModel);
		ImGui::CloseCurrentPopup();
		s_isOpen = false;
	}

	// title_name edit popup (rendered outside but opened from within)
	if (ImGui::BeginPopup("##editTitleName")) {
		if (s_editTitleInit) {
			std::string title = ObjectTagsManager::instance().getTitleName(s_selectedModel);
			std::string displayName =
				title.empty() ? (s_modelNames.count(s_selectedModel) ? s_modelNames[s_selectedModel] : std::string{})
							  : std::move(title);
			strncpy(s_editTitleBuf, displayName.c_str(), sizeof(s_editTitleBuf) - 1);
			s_editTitleBuf[sizeof(s_editTitleBuf) - 1] = '\0';
			s_editTitleInit = false;
		}
		ImGui::SetNextItemWidth(250.f);
		ImGui::InputText("##titleInput", s_editTitleBuf, sizeof(s_editTitleBuf));
		ImGui::SameLine();
		if (ImGui::Button(_("general.apply").c_str())) {
			ObjectTagsManager::instance().setTitleName(s_selectedModel, s_editTitleBuf);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndPopup();
}

bool PopupObjectSelector::drawCategoryNode(const std::vector<ObjectModelCategory>& cats, int parentId,
                                           bool parentSelected) {
	bool changed = false;
	for (const auto& cat : cats) {
		if (cat.parentId != parentId)
			continue;

		bool hasChildren = false;
		for (const auto& other : cats)
			if (other.parentId == cat.id) {
				hasChildren = true;
				break;
			}

		bool isSelected = s_selectedCategories.count(cat.id) > 0 || parentSelected;
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (isSelected)
			flags |= ImGuiTreeNodeFlags_Selected;
		if (!hasChildren)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::PushID(cat.id);
		bool nodeOpen = ImGui::TreeNodeEx(_(cat.name).c_str(), flags);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen()) {
			if (s_selectedCategories.count(cat.id))
				s_selectedCategories.erase(cat.id);
			else
				s_selectedCategories.insert(cat.id);
			changed = true;
		}

		bool deleteRequested = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem(_("object_selector.category_add_sub").c_str())) {
				s_addingCategory = true;
				s_addCategoryParentId = cat.id;
				s_addCategoryBuf[0] = '\0';
			}
			if (ImGui::MenuItem(_("general.delete").c_str())) {
				deleteRequested = true;
			}
			ImGui::EndPopup();
		}

		if (deleteRequested) {
			if (nodeOpen && hasChildren)
				ImGui::TreePop();
			ImGui::PopID();
			ObjectTagsManager::instance().deleteCategory(cat.id);
			s_selectedCategories.erase(cat.id);
			return true;
		}

		if (nodeOpen && hasChildren) {
			bool childSelected = s_selectedCategories.count(cat.id) > 0;
			if (drawCategoryNode(cats, cat.id, parentSelected || childSelected)) {
				ImGui::TreePop();
				ImGui::PopID();
				return true;
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
	return changed;
}

void PopupObjectSelector::drawTagsPanel(float blockWidth) {
	static char newTagBuf[64] = "";

	ImGui::BeginGroup();
	{
		ImGui::Text("%s:", _("object_selector.tags").c_str());

		if (ImGui::BeginChild("##ObjectsTags", ImVec2(blockWidth, -(ImGui::GetFontSize() + 4.f +
		                                                            ImGui::GetStyle().ItemSpacing.y * 2.0f)))) {
			const float maxBlockX = ImGui::GetCursorScreenPos().x + blockWidth - ImGui::GetStyle().ItemSpacing.x * 4.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ImGui::GetFontSize() / 4.0f);

			const auto& tags = ObjectTagsManager::instance().getTags();
			for (int i = 0; i < static_cast<int>(tags.size()); ++i) {
				const auto& tag = tags[i];

				auto str = fmt::format("{} {}", ICON_FA_TAG, _(tag.name));

				const float lastButtonX2 = ImGui::GetItemRectMax().x;
				const float nextButtonX2 = lastButtonX2 + ImGui::GetStyle().ItemSpacing.x +
				                           ImGui::CalcTextSize(str.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;

				if (i > 0 && nextButtonX2 < maxBlockX)
					ImGui::SameLine();

				ImGui::PushID(tag.id);
				if (ImGui::Button(str.c_str())) {
					s_selectedTagId = tag.id;
					s_selectorState = 2;
					s_filterApplied = false;
				}

				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem(_("general.delete").c_str())) {
						ObjectTagsManager::instance().deleteTag(tag.id);
					}
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();
		}

		ImGui::SetNextItemWidth(blockWidth - 25.0f - ImGui::GetStyle().ItemSpacing.x);
		ImGui::InputTextWithHint("##newTagInput", _("object_selector.tag_input_hint").c_str(), newTagBuf,
		                         sizeof(newTagBuf));
		ImGui::SameLine();
		ImGui::BeginDisabled(strlen(newTagBuf) == 0);
		const auto fontSize = ImGui::GetFontSize();
		if (ImGui::Button(ICON_FA_PLUS, ImVec2(fontSize * 1.5f, 0.0f))) {
			ObjectTagsManager::instance().addTag(newTagBuf);
			newTagBuf[0] = '\0';
		}
		ImGui::EndDisabled();
	}
	ImGui::EndGroup();
}

void PopupObjectSelector::drawBehaviorsPanel(float blockWidth) {
	ImGui::BeginGroup();
	ImGui::Text("%s:", _("object_selector.behaviors").c_str());

	if (ImGui::BeginChild("##ObjectsBehaviourChild", ImVec2(blockWidth, 0.0f))) {
		auto drawBehaviourRadio = [](const char* icon, const char* description, int* v) {
			const float dummy = (25.0f - ImGui::CalcTextSize(icon).x) / 2.0f;
			ImGui::PushID(icon);
			ImGui::Dummy(ImVec2(dummy, 0.0f));
			ImGui::SameLine();
			ImGui::Text("%s", icon);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", description);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(dummy, 0.0f));
			ImGui::SameLine();
			ImGui::RadioButton(_("object_selector.beh_any").c_str(), v, -1);
			ImGui::SameLine();
			ImGui::RadioButton(_("object_selector.beh_no").c_str(), v, 0);
			ImGui::SameLine();
			ImGui::RadioButton(_("object_selector.beh_yes").c_str(), v, 1);
			ImGui::PopID();
		};

		drawBehaviourRadio(ICON_FA_OBJECT_UNGROUP, _("object_selector.collision").c_str(),
		                   &s_filterBehavior.hasCollision);
		drawBehaviourRadio(ICON_FA_BOMB, _("object_selector.destroy_by_hit").c_str(), &s_filterBehavior.destroyByHit);
		drawBehaviourRadio(ICON_FA_CLOCK, _("object_selector.see_by_time").c_str(), &s_filterBehavior.seeByTime);
	}
	ImGui::EndChild();
	ImGui::EndGroup();
}

void PopupObjectSelector::drawCategoriesPanel(float blockWidth) {
	ImGui::BeginGroup();
	ImGui::Text("%s:", _("object_selector.categories").c_str());

	if (ImGui::BeginChild("##ObjectsCategories", ImVec2(blockWidth, 0.0f))) {
		const auto& cats = ObjectTagsManager::instance().getCategories();
		drawCategoryNode(cats, -1, false);
	}
	ImGui::EndChild();
	ImGui::EndGroup();
}

void PopupObjectSelector::drawResultsView(const char* searchText, const std::function<void(int)>& onSelectCallback) {
	static float scale = 1.0f;
	static int page = 1;
	static int prevPage = 1;
	static int countOnPage = 100;
	static bool requestPreview = false;

	if (!s_filterApplied) {
		applyFilter(searchText);
		s_filterApplied = true;
		page = 1;
		prevPage = 1;
		clearLoadedIcons();
	}

	// Pagination bar
	const int totalPages =
		s_queryResults.empty() ? 1 : (static_cast<int>(s_queryResults.size()) + countOnPage - 1) / countOnPage;
	page = std::clamp(page, 1, totalPages);

	// Clear icons if page changed
	if (page != prevPage) {
		clearLoadedIcons();
		prevPage = page;
	}

	ImGui::Text("Scale:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(80.f);
	ImGui::DragFloat("##scale", &scale, 0.01f, 0.1f, 5.0f, "%.2f");

	ImGui::SameLine();
	const auto scaleFont = ImGui::GetFontSize() / 16.f;
	ImGui::SetNextItemWidth(scaleFont * 100.0f);
	if (ImGui::InputInt(_("object_selector.count_on_page").c_str(), &countOnPage, 10, 100)) {
		countOnPage = std::max(10, countOnPage);
	}

	ImGui::SameLine(0, 20.f);
	ImGui::Text("%s: %d/%d (%d)", _("object_selector.page").c_str(), page, totalPages,
	            static_cast<int>(s_queryResults.size()));

	ImGui::SameLine();
	ImGui::BeginDisabled(page <= 1);
	if (ImGui::Button("<<"))
		--page;
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::BeginDisabled(page >= totalPages);
	if (ImGui::Button(">>"))
		++page;
	ImGui::EndDisabled();

	// Icon grid
	if (ImGui::BeginChild("##objectModels", ImVec2(0.0f, 0.0f), true)) {
		const int startIdx = (page - 1) * countOnPage;
		const int endIdx = std::min(startIdx + countOnPage, static_cast<int>(s_queryResults.size()));

		if (startIdx >= endIdx) {
			ImGui::Text("%s", _("object_selector.no_objects").c_str());
		}

		const float availableWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		const float contentMinScreenX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x;
		const float iconSize = 128.f * scale * ImGuiConfigurate::getUiScale();
		const ImVec2 iconSizeVec(iconSize, iconSize);

		for (int i = startIdx; i < endIdx; ++i) {
			const int model = s_queryResults[i].modelId;

			const auto info = CModelInfo::GetModelInfo(model);
			if (!info)
				continue;

			ImGui::PushID(i);

			if (i > startIdx) {
				float lastItemMaxScreenX = ImGui::GetItemRectMax().x;
				float currentLineWidth = lastItemMaxScreenX - contentMinScreenX;
				if (currentLineWidth + ImGui::GetStyle().ItemSpacing.x + iconSize < availableWidth)
					ImGui::SameLine();
			}

			auto icon = getModelIcon(model);
			bool clicked = false;

			if (icon && icon->getTexture()) {
				clicked = ImGui::ImageButton("", (ImTextureID)icon->getTexture(), iconSizeVec);
			} else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
				const auto label = fmt::format("{}", model);
				clicked = ImGui::Button(label.c_str(), iconSizeVec);
				ImGui::PopStyleColor();
			}

			if (clicked) {
				s_selectedModel = model;
				s_previewNeedRender = true;
				requestPreview = true;
				if (s_previewRenderer.has_value()) {
					s_previewRenderer.value().getRotateModel() = CVector(-10.0f, 0.f, -135.0f);
					s_previewRenderer.value().getZoomKoef() = 0.f;
				}
			}

			if (ImGui::IsItemHovered()) {
				if (const auto nameIt = s_modelNames.find(model); nameIt != s_modelNames.end())
					ImGui::SetTooltip("%s\nID: %d", nameIt->second.c_str(), model);
				else
					ImGui::SetTooltip("ID: %d", model);
			}

			ImGui::PopID();
		}
	}
	ImGui::EndChild();

	// Open preview modal after EndChild to ensure proper context
	if (requestPreview) {
		ImGui::OpenPopup("##objectPreview");
		requestPreview = false;
	}

	// Preview modal
	drawPreviewModal(onSelectCallback);
}

void PopupObjectSelector::renderPopup(const std::function<void(int)>& onSelectCallback) {
	static char searchText[256] = "";
	static bool wasOpen = false;

	const auto popupName = _("object_selector.title", ICON_FA_CUBE);
	const auto& displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowSize(ImVec2(displaySize.x * 0.6f, displaySize.y * 0.6f), ImGuiCond_Appearing);

	if (!ImGui::BeginPopupModal(popupName.c_str(), &s_isOpen, ImGuiWindowFlags_NoCollapse)) {
		// Popup closed - clear icons
		if (wasOpen) {
			clearLoadedIcons();
			wasOpen = false;
		}
		return;
	}

	wasOpen = true;

	// --- Top bar: Back button + Search ---
	const float backBtnW = ImGui::CalcTextSize(_("general.back").c_str()).x + ImGui::GetStyle().FramePadding.x * 2.f;
	const float searchBtnW =
		ImGui::CalcTextSize(_("object_selector.search").c_str()).x + ImGui::GetStyle().FramePadding.x * 2.f;

	ImGui::BeginDisabled(s_selectorState == 0);
	if (ImGui::Button(_("general.back").c_str(), ImVec2(backBtnW, 0.0f))) {
		s_selectedTagId = -1;
		s_selectorState = 0;
		clearLoadedIcons();
	}
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::BeginDisabled(s_selectorState == 2); // Disable search in tag mode
	{
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - searchBtnW - ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::InputTextWithHint("##search", _("object_selector.search_hint").c_str(), searchText,
		                             sizeof(searchText))) {
			// Auto-apply on typing
		}
		ImGui::SameLine();
		if (ImGui::Button(_("object_selector.search").c_str(), ImVec2(searchBtnW, 0.0f))) {
			s_selectorState = 1;
			s_filterApplied = false;
		}
	}
	ImGui::EndDisabled();

	ImGui::Separator();

	// --- Main content area ---
	if (s_selectorState == 0) {
		// Filter selection screen: 3 columns (Tags | Behaviors | Categories)
		const float blockWidth = (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) / 3.0f;

		drawTagsPanel(blockWidth);
		ImGui::SameLine();
		drawBehaviorsPanel(blockWidth);
		ImGui::SameLine();
		drawCategoriesPanel(blockWidth);
	} else {
		// Results view (search or tag mode)
		drawResultsView(searchText, onSelectCallback);
	}

	ImGui::EndPopup();
}
