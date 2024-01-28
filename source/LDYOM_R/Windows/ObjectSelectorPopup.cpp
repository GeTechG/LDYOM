#include "ObjectSelectorPopup.h"

#include "fa.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ModelsService.h"
#include "utils.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

std::optional<ModelRenderer> ObjectSelectorPopup::renderer_{};

IconObject::IconObject(std::unique_ptr<Texture> texture) : baseTexture_(std::move(texture)) {}

IconObject::IconObject(RwTexture *texture) : textureRW(texture) {}

IDirect3DTexture9* IconObject::getTexture() const {
	if (this->baseTexture_)
		return this->baseTexture_->getTexture();
	if (this->textureRW)
		return *reinterpret_cast<IDirect3DTexture9**>(this->textureRW->raster + 1);
	return nullptr;
}

RwTexture*& IconObject::getTextureRw() {
	return textureRW;
}

IconObject::~IconObject() {
	if (this->textureRW) {
		RwTextureDestroy(this->textureRW);
		this->textureRW = nullptr;
	}
}

void ObjectSelectorPopup::open() {
	AbstractWindow::open();
	ImGui::OpenPopup(Localization::getInstance().get("object_selector.title").c_str());
}

void ObjectSelectorPopup::drawTags(const float blockWidth, std::map<int, ObjectInfo> &models) {
	auto &local = Localization::getInstance();

	ImGui::BeginGroup();
	ImGui::Text(local.get("object_selector.tags").c_str());
	if (ImGui::BeginChild("##ObjectsTags",
	                      ImVec2(blockWidth, -(ImGui::GetFontSize() + 4.f + ImGui::GetStyle().ItemSpacing.y * 2.0f)))) {
		const float maxBlockX = ImGui::GetCursorScreenPos().x + blockWidth - ImGui::GetStyle().ItemSpacing.x * 4.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ImGui::GetFontSize() / 4.0f);

		const auto &objectModelTags = ModelsService::getInstance().getObjectModelTags();
		for (int i = 0; i < static_cast<int>(objectModelTags.size()); ++i) {
			const auto &objectModelTag = objectModelTags[i];

			auto str = fmt::format(" {} {}", ICON_FA_TAG, local.get(objectModelTag.name));

			const float lastButtonX2 = ImGui::GetItemRectMax().x;
			const float nextButtonX2 = lastButtonX2 + ImGui::GetStyle().ItemSpacing.x + ImGui::CalcTextSize(str.c_str())
				.x + ImGui::GetStyle().FramePadding.x * 2.0f; // Expected position if next button was on same line
			if (i > 0 && nextButtonX2 < maxBlockX)
				ImGui::SameLine();

			ImGui::PushID(objectModelTag.id);
			if (ImGui::Button(str.c_str())) {
				this->selectedTag = objectModelTag.id;
				this->selectorState = 2;
				this->page_ = 1;
				updateModels(models);
			}
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem(local.get("general.delete").c_str()))
					ModelsService::getInstance().deleteTag(objectModelTag.id);
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		ImGui::PopStyleVar();

		ImGui::EndChild();
	}

	ImGui::SetNextItemWidth(blockWidth - 25.0f - ImGui::GetStyle().ItemSpacing.x);
	ImGui::InputTextWithHint("##newTagInput", local.get("object_selector.name_new_tag_hint").c_str(), this->newTagName_,
	                         sizeof this->newTagName_);
	ImGui::SameLine();
	ImGui::BeginDisabled(strlen(this->newTagName_) == 0);
	const auto fontSize = ImGui::GetFontSize();
	if (ImGui::Button(ICON_FA_PLUS, ImVec2(fontSize * 1.5, .0f))) {
		if (const auto newTagID = ModelsService::getInstance().addNewTag(this->newTagName_); newTagID.has_value()) {
			strcpy(this->newTagName_, "");
		}
	}
	ImGui::EndDisabled();

	ImGui::EndGroup();
}

ImGuiTreeNodeFlags baseFlagsTreeNode = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
	ImGuiTreeNodeFlags_SpanAvailWidth;

void ObjectSelectorPopup::drawNode(const std::vector<ObjectModelCategory> &models, int parent_id, bool isParentSelect) {
	auto &local = Localization::getInstance();

	for (const auto &modelCategory : models) {
		if (modelCategory.parentId == parent_id) {
			auto iterator = std::ranges::find_if(models, [&modelCategory](const ObjectModelCategory &model) {
				return model.parentId == modelCategory.id;
			});

			auto flags = baseFlagsTreeNode;

			bool isSelectedNode = this->selectedCategories_.contains(modelCategory.id);
			if (isSelectedNode || isParentSelect)
				flags |= ImGuiTreeNodeFlags_Selected;

			const bool emptyNode = iterator == models.end();
			if (emptyNode)
				flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			const bool isTreeNodeOpen = ImGui::TreeNodeEx(local.get(modelCategory.name).c_str(), flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				if (isSelectedNode) {
					this->selectedCategories_.erase(modelCategory.id);
				} else {
					this->selectedCategories_.emplace(modelCategory.id);
				}
				isSelectedNode = !isSelectedNode;
			}

			if (isTreeNodeOpen && !emptyNode) {
				drawNode(models, modelCategory.id, isSelectedNode);
				ImGui::TreePop();
			}
		}
	}
}

void ObjectSelectorPopup::drawCategories(const float blockWidth) {
	auto &local = Localization::getInstance();

	ImGui::BeginGroup();
	ImGui::Text(local.get("object_selector.categories").c_str());
	if (ImGui::BeginChild("##ObjectsCategories", ImVec2(blockWidth, 0.f))) {
		const auto &objectModelCategories = ModelsService::getInstance().getObjectModelCategories();

		drawNode(objectModelCategories, -1, false);

		ImGui::EndChild();
	}
	ImGui::EndGroup();
}

void ObjectSelectorPopup::drawBehaviors(const float blockWidth) {
	auto &local = Localization::getInstance();

	ImGui::BeginGroup();
	ImGui::Text(local.get("object_selector.behaviors").c_str());
	if (ImGui::BeginChild("##ObjectsBehaviour", ImVec2(blockWidth, 0.f))) {
		auto drawBehaviourRadio = [&local](const char *icon, const char *description, int *v) {
			const float dummy = (25.0f - ImGui::CalcTextSize(icon).x) / 2.0f;
			ImGui::PushID(icon);
			ImGui::Dummy(ImVec2(dummy, .0f));
			ImGui::SameLine();
			ImGui::Text(icon);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip(description);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(dummy, .0f));
			ImGui::SameLine();
			ImGui::RadioButton(local.get("object_selector.beh_whatever").c_str(), v, -1);
			ImGui::SameLine();
			ImGui::RadioButton(local.get("object_selector.beh_no").c_str(), v, 0);
			ImGui::SameLine();
			ImGui::RadioButton(local.get("object_selector.beh_yes").c_str(), v, 1);
			ImGui::PopID();
		};

		drawBehaviourRadio(ICON_FA_OBJECT_UNGROUP, local.get("object_selector.collision").c_str(),
		                   &this->objectBehaviourHasCollision);
		drawBehaviourRadio(ICON_FA_FRAGILE, local.get("object_selector.destroy_by_hit").c_str(),
		                   &this->objectBehaviourDestroyByHit);
		drawBehaviourRadio(ICON_FA_CLOCK, local.get("object_selector.see_by_time").c_str(),
		                   &this->objectBehaviourSeeByTime);

		ImGui::EndChild();
	}
	ImGui::EndGroup();
}

std::optional<ModelRenderer> rendererPreviewModel;

void renderPreviewObject(int model, RwTexture **texture) {
	if (!rendererPreviewModel.has_value()) {
		rendererPreviewModel = ModelRenderer(CVector(-10.0f, 0, -135.0f),
		                                     static_cast<int>(ImGui::GetFontSize() * 16.f));
		rendererPreviewModel.value().init();
	}
	rendererPreviewModel.value().render(model, texture);
}

std::set<int> modelTags;

void updateModelTag(int model) {
	modelTags = ModelsService::getInstance().getModelTags(model);
}

void ObjectSelectorPopup::drawPreviewObject(ObjectInfo &info) {
	auto &local = Localization::getInstance();

	static std::string newTitleName;

	if (ImGui::BeginPopupModal("##previewModel", nullptr,
	                           ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		if (!this->renderedPreviewModel) {
			renderPreviewObject(this->selectedObjectModel, &this->previewObjectModel);

			this->renderedPreviewModel = true;
		}

		if (this->previewObjectModel) {
			const auto texture9 = *reinterpret_cast<IDirect3DTexture9**>(this->previewObjectModel->raster + 1);
			ImGui::ImageButton(texture9, ImVec2(256.f, 256.f));
			if (ImGui::IsItemHovered()) {
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
					this->renderedPreviewModel = false;
					rendererPreviewModel.value().getRotateModel().z += ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).
						x;
					rendererPreviewModel.value().getRotateModel().x -= ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).
						y;
					ImGui::ResetMouseDragDelta();
				}
				if (abs(ImGui::GetIO().MouseWheel) > FLT_EPSILON) {
					this->renderedPreviewModel = false;
					rendererPreviewModel.value().getZoomKoef() += ImGui::GetIO().MouseWheel * 0.1f;
				}
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 32.f);
			ImGui::Text(
				fmt::format("{}: {}", local.get("object_selector.title_name"), local.get(info.titleName)).c_str());
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_EDIT, ImVec2(ImGui::GetFontSize() * 1.2f, ImGui::GetFontSize() * 1.2f))) {
				newTitleName = local.get(info.titleName);
				ImGui::OpenPopup("##editTitleName");
			}


			ImGui::Text(fmt::format("{}: {}", local.get("object_selector.model_id"), info.modelId).c_str());
			ImGui::Text(fmt::format("{}: {}", local.get("object_selector.model_name"), info.modelName).c_str());

			ImGui::Text(fmt::format("{}: {}", local.get("object_selector.collision"),
			                        info.hasCollision
				                        ? local.get("object_selector.beh_yes")
				                        : local.get("object_selector.beh_no")).c_str());
			ImGui::Text(fmt::format("{}: {}", local.get("object_selector.destroy_by_hit"),
			                        info.isDestroyByHit
				                        ? local.get("object_selector.beh_yes")
				                        : local.get("object_selector.beh_no")).c_str());
			ImGui::Text(fmt::format("{}: {}", local.get("object_selector.see_by_time"),
			                        info.seeByTime
				                        ? local.get("object_selector.beh_yes")
				                        : local.get("object_selector.beh_no")).c_str());

			ImGui::PopTextWrapPos();

			if (ImGui::BeginChild("##SelectedTags", ImVec2(ImGui::GetFontSize() * 16.f, ImGui::GetFontSize() * 6.5f),
			                      true)) {
				const auto &tags = ModelsService::getInstance().getObjectModelTags();
				for (const auto &tag : tags) {
					bool v = modelTags.contains(tag.id);
					if (ImGui::Checkbox(local.get(tag.name).c_str(), &v)) {
						if (v) {
							ModelsService::getInstance().setModelToTag(info.modelId, tag.id);
						} else {
							ModelsService::getInstance().clearModelToTag(info.modelId, tag.id);
						}
						modelTags = ModelsService::getInstance().getModelTags(info.modelId);
					}
				}
			}
			ImGui::EndChild();
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 16.f - 25.0f - ImGui::GetStyle().ItemSpacing.x);
			ImGui::InputTextWithHint("##newTagInput", local.get("object_selector.name_new_tag_hint").c_str(),
			                         this->newTagName_, sizeof this->newTagName_);
			ImGui::SameLine();
			ImGui::BeginDisabled(strlen(this->newTagName_) == 0);
			const auto fontSize = ImGui::GetFontSize();
			if (ImGui::Button(ICON_FA_PLUS, ImVec2(fontSize * 1.5, .0f))) {
				if (const auto newTagID = ModelsService::getInstance().addNewTag(this->newTagName_); newTagID.
					has_value()) {
					ModelsService::getInstance().setModelToTag(info.modelId, newTagID.value());
					strcpy(this->newTagName_, "");
				}
			}
			ImGui::EndDisabled();

			ImGui::EndGroup();
		}

		if (ImGui::BeginPopup("##editTitleName")) {
			ImGui::InputText("##newNameInput", &newTitleName);
			ImGui::SameLine();
			if (ImGui::Button(local.get("general.apply").c_str())) {
				ModelsService::getInstance().newTitleNameModel(info.modelId, newTitleName);
				info.titleName = newTitleName;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button(local.get("general.close").c_str())) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("general.create").c_str())) {
			callbackSelect_(this->selectedObjectModel);
			ImGui::CloseCurrentPopup();
			this->isShow() = false;
		}

		ImGui::EndPopup();
	}
}

void ObjectSelectorPopup::makePreview(const int modelId, const std::string &iconUrl) {
	if (!ModelsService::getInstance().validObjectModel(modelId))
		return;
	std::optional<std::unique_ptr<Texture>> texture = ModelsService::getInstance().getModelIcon(modelId);
	if (!texture.has_value())
		texture = utils::LoadTextureRequiredFromURL(iconUrl);
	if (texture.has_value()) {
		this->icons_[modelId] = IconObject(std::move(texture.value()));
	} else {
		if (!renderer_.has_value()) {
			renderer_ = ModelRenderer(CVector(-10.0f, 0, -135.0f), static_cast<int>(ImGui::GetFontSize() * 8.f));
			renderer_.value().init();
		}
		this->icons_[modelId] = IconObject(nullptr);
		renderer_.value().render(modelId, &this->icons_[modelId].getTextureRw());
	}
}

void ObjectSelectorPopup::clearPreviews() {
	this->icons_.clear();
}

void ObjectSelectorPopup::updateModels(std::map<int, ObjectInfo> &models) {
	if (this->selectorState == 1)
		models = ModelsService::getInstance().getObjectModelsBySearch(this->searchText_,
		                                                              this->selectedCategories_,
		                                                              this->objectBehaviourHasCollision,
		                                                              this->objectBehaviourDestroyByHit,
		                                                              this->objectBehaviourSeeByTime,
		                                                              this->countOnPage_, this->page_, this->allCount);
	if (this->selectorState == 2)
		models = ModelsService::getInstance().getObjectModelsByTag(this->selectedTag,
		                                                           this->page_, this->countOnPage_, this->allCount);
	selectedObjectModel = 0;
	clearPreviews();
	for (const auto &model : models)
		makePreview(model.first, model.second.iconUrl);
}

void ObjectSelectorPopup::draw() {
	auto &local = Localization::getInstance();

	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 64.0f, ImGui::GetFontSize() * 24.0f), ImGuiCond_Appearing);
	if (ImGui::BeginPopupModal(local.get("object_selector.title").c_str(), &this->isShow())) {
		static std::map<int, ObjectInfo> models;

		const auto backWidth = ImGui::CalcTextSize(local.get("general.back").c_str()).x + 5;
		const auto searchWidth = ImGui::CalcTextSize(local.get("object_selector.search").c_str()).x + 5;

		ImGui::BeginDisabled(this->selectorState == 0);
		if (ImGui::Button(local.get("general.back").c_str(), ImVec2(backWidth, 0))) {
			this->selectorState = 0;
		}
		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::BeginDisabled(this->selectorState == 2);
		ImGui::SetNextItemWidth(
			ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - backWidth * 1.1f - searchWidth
			* 1.1f -
			ImGui::GetStyle().ItemSpacing.x * 3.f);
		ImGui::InputTextWithHint("##searchInput", local.get("object_selector.search_hint").c_str(), this->searchText_,
		                         sizeof this->searchText_);
		ImGui::SameLine();
		if (ImGui::Button(local.get("object_selector.search").c_str(), ImVec2(searchWidth, .0f))) {
			this->selectorState = 1;
			this->page_ = 1;
			updateModels(models);
		}
		ImGui::EndDisabled();

		if (this->selectorState == 0) {
			const float blockWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x / 3.0f;
			drawTags(blockWidth, models);

			ImGui::SameLine();

			drawBehaviors(blockWidth);

			ImGui::SameLine();

			drawCategories(blockWidth);
		} else {
			if (ImGui::BeginChild("##objectModels",
			                      ImVec2(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x,
			                             0.f))) {
				const float windowStartX = ImGui::GetCursorScreenPos().x;

				const auto scaleFont = ImGui::GetFontSize() / 16.f;
				ImGui::SetNextItemWidth(scaleFont * 100.0f);
				if (ImGui::InputInt(local.get("object_selector.count_on_page").c_str(), &this->countOnPage_, 10, 100)) {
					updateModels(models);
				}
				ImGui::SameLine(0, 20.f);
				const int allCountPage = (this->allCount + this->countOnPage_ - 1) / this->countOnPage_;
				ImGui::Text("%s: %d/%d", local.get("general.page").c_str(), this->page_, allCountPage);

				ImGui::SameLine();
				ImGui::BeginDisabled(this->page_ <= 1);
				if (ImGui::Button(local.get("object_selector.previous_page").c_str())) {
					this->page_--;
					updateModels(models);
				}
				ImGui::EndDisabled();

				ImGui::SameLine();
				ImGui::BeginDisabled(this->page_ >= allCountPage);
				if (ImGui::Button(local.get("object_selector.next_page").c_str())) {
					this->page_++;
					updateModels(models);
				}
				ImGui::EndDisabled();


				int i = 0;
				for (const auto &pair : models) {
					if (ModelsService::getInstance().validObjectModel(pair.first)) {
						IDirect3DTexture9 *texture;
						if (this->icons_.contains(pair.second.modelId)) {
							texture = this->icons_.at(pair.second.modelId).getTexture();
						} else {
							if (!this->preview_.has_value()) {
								this->preview_ =
									utils::LoadTextureRequiredFromFile("LDYOM/Resources/preloadObject.jpg");
							}
							texture = this->preview_.value()->getTexture();
						}

						const float lastButtonX2 = ImGui::GetItemRectMax().x;
						const float nextButtonX2 = lastButtonX2 + ImGui::GetStyle().ItemSpacing.x + ImGui::GetFontSize()
							* 8.f + ImGui::GetStyle().FramePadding.x * 2.0f;
						// Expected position if next button was on same line
						if (i > 0 && nextButtonX2 < windowStartX + ImGui::GetWindowContentRegionMax().x -
							ImGui::GetWindowContentRegionMin().x)
							ImGui::SameLine();

						const float size = ImGui::GetFontSize() * 8.f;
						if (ImGui::ImageButton(texture, ImVec2(size, size))) {
							this->selectedObjectModel = pair.first;
							this->renderedPreviewModel = false;
							updateModelTag(pair.first);
							ImGui::OpenPopup("##previewModel");
						}

						i++;
					}
				}

				if (this->selectedObjectModel != 0)
					drawPreviewObject(models.at(this->selectedObjectModel));

				ImGui::EndChild();
			}
		}

		ImGui::EndPopup();
	}
}

void ObjectSelectorPopup::setCallbackSelect(const std::function<void(int)> &callbackSelect) {
	callbackSelect_ = callbackSelect;
}
