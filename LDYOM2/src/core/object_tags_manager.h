#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct ObjectModelTag {
	int id;
	std::string name;
};

struct ObjectModelCategory {
	int id;
	std::string name;
	int parentId = -1;
};

class ObjectTagsManager {
  private:
	ObjectTagsManager() = default;

	std::unique_ptr<SQLite::Database>      m_modelsDb;
	std::unique_ptr<SQLite::Database>      m_iconsDb;
	std::vector<ObjectModelTag>            m_tags;
	std::unordered_map<int, std::set<int>> m_modelTags;
	std::vector<ObjectModelCategory>       m_categories;
	std::unordered_map<int, std::set<int>> m_modelCategories;

	static const std::string MODELS_DB_FILE;
	static const std::string ICONS_DB_FILE;

  public:
	ObjectTagsManager(const ObjectTagsManager&) = delete;
	void operator=(const ObjectTagsManager&) = delete;

	static ObjectTagsManager& instance();

	void initialize();

	std::optional<int> addTag(const std::string& name);
	bool deleteTag(int tagId);
	void setModelTag(int modelId, int tagId);
	void clearModelTag(int modelId, int tagId);
	std::set<int> getModelTags(int modelId) const;
	const std::vector<ObjectModelTag>& getTags() const { return m_tags; }

	std::optional<int> addCategory(const std::string& name, int parentId = -1);
	bool deleteCategory(int categoryId);
	void setModelCategory(int modelId, int categoryId);
	void clearModelCategory(int modelId, int categoryId);
	std::set<int> getModelCategories(int modelId) const;
	const std::vector<ObjectModelCategory>& getCategories() const { return m_categories; }
	std::set<int> expandCategories(const std::set<int>& selected) const;

	void        setTitleName(int modelId, const std::string& name);
	std::string getTitleName(int modelId) const;

	// Query methods for object models (no caching, direct SQL queries)
	struct ObjectModelInfo {
		int modelId;
		std::string modelName;
		std::string titleName;
		bool hasCollision;
		bool isDestroyByHit;
		bool seeByTime;
	};
	std::vector<ObjectModelInfo> queryObjectModels(
		const std::string& searchText,
		int hasCollision, int destroyByHit, int seeByTime,
		int tagId,
		const std::set<int>& categoryIds
	) const;
	std::string getModelName(int modelId) const;
	std::vector<unsigned char> getModelIcon(int modelId) const; // BLOB from icons_models.sqlite3
};
