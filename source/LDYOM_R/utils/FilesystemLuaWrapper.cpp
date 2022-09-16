#include "FilesystemLuaWrapper.h"

#include <filesystem>

void FilesystemLuaWrapper::wrap(sol::state& state) {
	auto table = state.create_table("Dirs");
	table.set_function("entries", [](const char* luaPath) {
		const auto path = std::filesystem::path("LDYOM/Scripts") / luaPath;
		std::vector<std::string> entries;
		for (auto& entry : std::filesystem::directory_iterator(path)) {
			entries.emplace_back(entry.path().filename().string());
		}
		return sol::as_table(entries);
	});
	table.set_function("exists", [](const char* luaPath) {
		const auto path = std::filesystem::path("LDYOM/Scripts") / luaPath;
		return exists(path);
	});
}
