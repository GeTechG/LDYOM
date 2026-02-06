#include "object_tags_manager.h"
#include <algorithm>
#include <filesystem>
#include <logger.h>
#include <paths.h>

const std::string ObjectTagsManager::MODELS_DB_FILE = "models.sqlite3";
const std::string ObjectTagsManager::ICONS_DB_FILE = "icons_models.sqlite3";

ObjectTagsManager& ObjectTagsManager::instance() {
	static ObjectTagsManager instance;
	return instance;
}

void ObjectTagsManager::initialize() {
	if (m_modelsDb)
		return;

	// Open models.sqlite3 (read-write for tags/categories/title_names)
	try {
		const std::string modelsDbPath = LDYOM_PATH(MODELS_DB_FILE);
		if (!std::filesystem::exists(modelsDbPath)) {
			LDYOM_ERROR("ObjectTagsManager: models.sqlite3 not found at {}", modelsDbPath);
			return;
		}

		m_modelsDb = std::make_unique<SQLite::Database>(modelsDbPath, SQLite::OPEN_READWRITE);
		LDYOM_INFO("ObjectTagsManager: opened models.sqlite3");

		// Load tags from models.sqlite3
		{
			SQLite::Statement stmt(*m_modelsDb, "SELECT id, name FROM tags ORDER BY id");
			while (stmt.executeStep())
				m_tags.push_back({stmt.getColumn(0).getInt(), stmt.getColumn(1).getString()});
		}
		{
			SQLite::Statement stmt(*m_modelsDb, "SELECT tag_id, model_id FROM tag_to_models");
			while (stmt.executeStep())
				m_modelTags[stmt.getColumn(1).getInt()].insert(stmt.getColumn(0).getInt());
		}

		// Load categories from models.sqlite3
		{
			SQLite::Statement stmt(*m_modelsDb, "SELECT id, category_name, parent_id FROM categories ORDER BY id");
			while (stmt.executeStep())
				m_categories.push_back(
					{stmt.getColumn(0).getInt(), stmt.getColumn(1).getString(), stmt.getColumn(2).getInt()});
		}
		{
			SQLite::Statement stmt(*m_modelsDb, "SELECT category_id, model_id FROM category_to_models");
			while (stmt.executeStep())
				m_modelCategories[stmt.getColumn(1).getInt()].insert(stmt.getColumn(0).getInt());
		}

		LDYOM_INFO("ObjectTagsManager: loaded {} tags, {} categories", m_tags.size(), m_categories.size());
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: failed to open models database: {}", e.what());
	}

	// Open icons_models.sqlite3 (optional, for icon BLOBs)
	try {
		const std::string iconsDbPath = LDYOM_PATH(ICONS_DB_FILE);
		if (std::filesystem::exists(iconsDbPath)) {
			m_iconsDb = std::make_unique<SQLite::Database>(iconsDbPath, SQLite::OPEN_READONLY);
			LDYOM_INFO("ObjectTagsManager: opened icons_models.sqlite3");
		}
	} catch (const SQLite::Exception& e) {
		LDYOM_WARN("ObjectTagsManager: failed to open icons database: {}", e.what());
	}
}

std::optional<int> ObjectTagsManager::addTag(const std::string& name) {
	if (!m_modelsDb)
		return std::nullopt;

	for (const auto& tag : m_tags)
		if (tag.name == name)
			return std::nullopt;

	try {
		SQLite::Statement stmt(*m_modelsDb, "INSERT INTO tags (name) VALUES (?)");
		stmt.bind(1, name);
		stmt.executeStep();

		int newId = static_cast<int>(m_modelsDb->getLastInsertRowid());
		m_tags.push_back({newId, name});
		return newId;
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: addTag failed: {}", e.what());
		return std::nullopt;
	}
}

bool ObjectTagsManager::deleteTag(int tagId) {
	if (!m_modelsDb)
		return false;

	auto it = std::find_if(m_tags.begin(), m_tags.end(), [tagId](const ObjectModelTag& t) { return t.id == tagId; });
	if (it == m_tags.end())
		return false;

	try {
		SQLite::Transaction txn(*m_modelsDb);
		{
			SQLite::Statement stmt(*m_modelsDb, "DELETE FROM tag_to_models WHERE tag_id = ?");
			stmt.bind(1, tagId);
			stmt.executeStep();
		}
		{
			SQLite::Statement stmt(*m_modelsDb, "DELETE FROM tags WHERE id = ?");
			stmt.bind(1, tagId);
			stmt.executeStep();
		}
		txn.commit();
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: deleteTag failed: {}", e.what());
		return false;
	}

	m_tags.erase(it);
	for (auto& [modelId, tagIds] : m_modelTags)
		tagIds.erase(tagId);

	return true;
}

void ObjectTagsManager::setModelTag(int modelId, int tagId) {
	if (!m_modelsDb)
		return;

	try {
		SQLite::Statement stmt(*m_modelsDb, "INSERT OR IGNORE INTO tag_to_models (tag_id, model_id) VALUES (?, ?)");
		stmt.bind(1, tagId);
		stmt.bind(2, modelId);
		stmt.executeStep();

		m_modelTags[modelId].insert(tagId);
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: setModelTag failed: {}", e.what());
	}
}

void ObjectTagsManager::clearModelTag(int modelId, int tagId) {
	if (!m_modelsDb)
		return;

	try {
		SQLite::Statement stmt(*m_modelsDb, "DELETE FROM tag_to_models WHERE tag_id = ? AND model_id = ?");
		stmt.bind(1, tagId);
		stmt.bind(2, modelId);
		stmt.executeStep();
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: clearModelTag failed: {}", e.what());
		return;
	}

	auto it = m_modelTags.find(modelId);
	if (it != m_modelTags.end()) {
		it->second.erase(tagId);
		if (it->second.empty())
			m_modelTags.erase(it);
	}
}

std::set<int> ObjectTagsManager::getModelTags(int modelId) const {
	auto it = m_modelTags.find(modelId);
	if (it != m_modelTags.end())
		return it->second;
	return {};
}

std::optional<int> ObjectTagsManager::addCategory(const std::string& name, int parentId) {
	if (!m_modelsDb)
		return std::nullopt;

	for (const auto& cat : m_categories)
		if (cat.name == name && cat.parentId == parentId)
			return std::nullopt;

	try {
		SQLite::Statement stmt(*m_modelsDb, "INSERT INTO categories (category_name, parent_id) VALUES (?, ?)");
		stmt.bind(1, name);
		stmt.bind(2, parentId);
		stmt.executeStep();

		int newId = static_cast<int>(m_modelsDb->getLastInsertRowid());
		m_categories.push_back({newId, name, parentId});
		return newId;
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: addCategory failed: {}", e.what());
		return std::nullopt;
	}
}

bool ObjectTagsManager::deleteCategory(int categoryId) {
	if (!m_modelsDb)
		return false;

	// Collect this category + all descendants
	std::set<int> toDelete = {categoryId};
	bool changed = true;
	while (changed) {
		changed = false;
		for (const auto& cat : m_categories) {
			if (toDelete.count(cat.parentId) && !toDelete.count(cat.id)) {
				toDelete.insert(cat.id);
				changed = true;
			}
		}
	}

	try {
		SQLite::Transaction txn(*m_modelsDb);
		for (int id : toDelete) {
			{
				SQLite::Statement stmt(*m_modelsDb, "DELETE FROM category_to_models WHERE category_id = ?");
				stmt.bind(1, id);
				stmt.executeStep();
			}
			{
				SQLite::Statement stmt(*m_modelsDb, "DELETE FROM categories WHERE id = ?");
				stmt.bind(1, id);
				stmt.executeStep();
			}
		}
		txn.commit();
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: deleteCategory failed: {}", e.what());
		return false;
	}

	std::erase_if(m_categories, [&toDelete](const ObjectModelCategory& c) { return toDelete.count(c.id); });
	for (auto& [modelId, catIds] : m_modelCategories)
		for (int id : toDelete)
			catIds.erase(id);
	std::erase_if(m_modelCategories, [](const auto& p) { return p.second.empty(); });

	return true;
}

void ObjectTagsManager::setModelCategory(int modelId, int categoryId) {
	if (!m_modelsDb)
		return;

	try {
		SQLite::Statement stmt(*m_modelsDb,
		                       "INSERT OR IGNORE INTO category_to_models (category_id, model_id) VALUES (?, ?)");
		stmt.bind(1, categoryId);
		stmt.bind(2, modelId);
		stmt.executeStep();

		m_modelCategories[modelId].insert(categoryId);
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: setModelCategory failed: {}", e.what());
	}
}

void ObjectTagsManager::clearModelCategory(int modelId, int categoryId) {
	if (!m_modelsDb)
		return;

	try {
		SQLite::Statement stmt(*m_modelsDb, "DELETE FROM category_to_models WHERE category_id = ? AND model_id = ?");
		stmt.bind(1, categoryId);
		stmt.bind(2, modelId);
		stmt.executeStep();
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: clearModelCategory failed: {}", e.what());
		return;
	}

	auto it = m_modelCategories.find(modelId);
	if (it != m_modelCategories.end()) {
		it->second.erase(categoryId);
		if (it->second.empty())
			m_modelCategories.erase(it);
	}
}

std::set<int> ObjectTagsManager::getModelCategories(int modelId) const {
	auto it = m_modelCategories.find(modelId);
	if (it != m_modelCategories.end())
		return it->second;
	return {};
}

std::set<int> ObjectTagsManager::expandCategories(const std::set<int>& selected) const {
	std::set<int> expanded = selected;
	bool changed = true;
	while (changed) {
		changed = false;
		for (const auto& cat : m_categories) {
			if (expanded.count(cat.parentId) && !expanded.count(cat.id)) {
				expanded.insert(cat.id);
				changed = true;
			}
		}
	}
	return expanded;
}

void ObjectTagsManager::setTitleName(int modelId, const std::string& name) {
	if (!m_modelsDb)
		return;

	try {
		SQLite::Statement stmt(*m_modelsDb, "UPDATE models SET title_name = ? WHERE model_id = ?");
		stmt.bind(1, name);
		stmt.bind(2, modelId);
		stmt.executeStep();
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: setTitleName failed: {}", e.what());
	}
}

std::string ObjectTagsManager::getTitleName(int modelId) const {
	if (!m_modelsDb)
		return "";

	try {
		SQLite::Statement stmt(*m_modelsDb, "SELECT title_name FROM models WHERE model_id = ? LIMIT 1");
		stmt.bind(1, modelId);
		if (stmt.executeStep()) {
			const char* title = stmt.getColumn(0);
			return title ? title : "";
		}
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: getTitleName failed: {}", e.what());
	}
	return "";
}

std::string ObjectTagsManager::getModelName(int modelId) const {
	if (!m_modelsDb)
		return "";

	try {
		SQLite::Statement stmt(*m_modelsDb, "SELECT model_name FROM models WHERE model_id = ? LIMIT 1");
		stmt.bind(1, modelId);
		if (stmt.executeStep())
			return stmt.getColumn(0).getString();
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: getModelName failed: {}", e.what());
	}
	return "";
}

std::vector<ObjectTagsManager::ObjectModelInfo>
ObjectTagsManager::queryObjectModels(const std::string& searchText, int hasCollision, int destroyByHit, int seeByTime,
                                     int tagId, const std::set<int>& categoryIds) const {
	std::vector<ObjectModelInfo> results;

	if (!m_modelsDb)
		return results;

	try {
		std::string sql = "SELECT model_id, model_name, title_name, has_collision, is_destroy_by_hit, see_by_time FROM "
		                  "models WHERE 1=1";

		// Search filter (case-insensitive)
		if (!searchText.empty()) {
			sql += " AND (CAST(model_id AS TEXT) LIKE ?1 OR LOWER(model_name) LIKE ?1 OR LOWER(COALESCE(title_name, "
			       "'')) LIKE ?1)";
		}

		// Behaviors filter (from database columns)
		if (hasCollision != -1) {
			sql += " AND has_collision = " + std::to_string(hasCollision);
		}
		if (destroyByHit != -1) {
			sql += " AND is_destroy_by_hit = " + std::to_string(destroyByHit);
		}
		if (seeByTime != -1) {
			sql += " AND see_by_time = " + std::to_string(seeByTime);
		}

		// Categories filter (expand to include children)
		if (!categoryIds.empty()) {
			const auto expanded = expandCategories(categoryIds);
			std::string catList;
			for (int catId : expanded) {
				if (!catList.empty())
					catList += ",";
				catList += std::to_string(catId);
			}
			sql += " AND model_id IN (SELECT model_id FROM category_to_models WHERE category_id IN (" + catList + "))";
		}

		// Tags filter
		if (tagId != -1) {
			sql += " AND model_id IN (SELECT model_id FROM tag_to_models WHERE tag_id = " + std::to_string(tagId) + ")";
		}

		sql += " ORDER BY model_id";

		SQLite::Statement stmt(*m_modelsDb, sql);

		if (!searchText.empty()) {
			std::string lowerSearch = searchText;
			std::ranges::transform(lowerSearch, lowerSearch.begin(), [](unsigned char c) { return std::tolower(c); });
			std::string pattern = "%" + lowerSearch + "%";
			stmt.bind(1, pattern);
		}

		while (stmt.executeStep()) {
			ObjectModelInfo info;
			info.modelId = stmt.getColumn(0).getInt();
			info.modelName = stmt.getColumn(1).getString();
			const char* titleName = stmt.getColumn(2);
			info.titleName = titleName ? titleName : "";
			info.hasCollision = stmt.getColumn(3).getInt() != 0;
			info.isDestroyByHit = stmt.getColumn(4).getInt() != 0;
			info.seeByTime = stmt.getColumn(5).getInt() != 0;
			results.push_back(std::move(info));
		}
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: queryObjectModels failed: {}", e.what());
	}

	return results;
}
