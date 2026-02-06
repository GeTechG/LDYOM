#include "object_tags_manager.h"
#include <logger.h>

std::vector<unsigned char> ObjectTagsManager::getModelIcon(int modelId) const {
	if (!m_iconsDb)
		return {};

	try {
		SQLite::Statement stmt(*m_iconsDb, "SELECT icon FROM icons WHERE model_id = ? LIMIT 1");
		stmt.bind(1, modelId);
		if (stmt.executeStep()) {
			const void* blob = stmt.getColumn(0).getBlob();
			int size = stmt.getColumn(0).getBytes();
			if (blob && size > 0) {
				const unsigned char* data = static_cast<const unsigned char*>(blob);
				return std::vector<unsigned char>(data, data + size);
			}
		}
	} catch (const SQLite::Exception& e) {
		LDYOM_ERROR("ObjectTagsManager: getModelIcon failed: {}", e.what());
	}
	return {};
}
