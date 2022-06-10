#pragma once
#include <optional>
#include <set>
#include <unordered_map>

#include "ModelRenderer.h"
#include "ModelsService.h"
#include "plugin.h"

#include "../Windows/AbstractWindow.h"
#include "../Data/Texture.h"


struct ObjectModelCategory;

class IconObject {
	std::unique_ptr<Texture> baseTexture_ = nullptr;
	RwTexture* textureRW = nullptr;
public:
	IconObject() = default;
	IconObject(std::unique_ptr<Texture> texture);
	IconObject(RwTexture* texture);

	IconObject(const IconObject& other)
		: baseTexture_{other.baseTexture_.get()},
		  textureRW{other.textureRW} {
	}

	IconObject(IconObject&& other) noexcept
		: baseTexture_{std::move(other.baseTexture_)},
		  textureRW{other.textureRW} {
	}

	IconObject& operator=(const IconObject& other) {
		if (this == &other)
			return *this;
		baseTexture_ = std::unique_ptr<Texture>(other.baseTexture_.get());
		textureRW = other.textureRW;
		return *this;
	}

	IconObject& operator=(IconObject&& other) noexcept {
		if (this == &other)
			return *this;
		baseTexture_ = std::move(other.baseTexture_);
		textureRW = other.textureRW;
		return *this;
	}

	IDirect3DTexture9* getTexture() const;
	RwTexture*& getTextureRw();
	~IconObject();
};

class ObjectSelectorPopup: public Windows::AbstractWindow {
private:
	std::unordered_map<int, IconObject> icons_;
	static std::optional<ModelRenderer> renderer_;
	std::optional<std::unique_ptr<Texture>> preview_;
	RwTexture* previewObjectModel = nullptr;

	int selectorState = 0;
	char searchText_[256] = "";
	char newTagName_[64] = "";
	int page_ = 1;
	int countOnPage_ = 100;
	int allCount = 0;
	int objectBehaviourHasCollision = -1;
	int objectBehaviourDestroyByHit = -1;
	int objectBehaviourSeeByTime = -1;
	int selectedTag = -1;
	int selectedObjectModel = 0;
	bool renderedPreviewModel = false;

	std::function<void(int)> callbackSelect_ = [](int i) {};

	std::set<int> selectedCategories_;

	void drawNode(const std::vector<ObjectModelCategory>& models, int parent_id, bool isParentSelect);

	void drawTags(float blockWidth, std::map<int, ObjectInfo>& models);
	void drawCategories(float blockWidth);
	void drawBehaviors(float blockWidth);

	void drawPreviewObject(ObjectInfo& info);

	void makePreview(int modelId, const std::string& iconUrl);
	void clearPreviews();

	void updateModels(std::map<int, ObjectInfo>& models);
public:
	void open() override;
	void draw() override;
	void setCallbackSelect(const std::function<void(int)>& callbackSelect);
};
