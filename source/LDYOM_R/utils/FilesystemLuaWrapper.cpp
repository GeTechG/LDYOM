#include <filesystem>
#include <sol.hpp>

const auto LDYOM_PATH = std::filesystem::path("LDYOM");

bool isSubPath(const std::filesystem::path &path, const std::filesystem::path &base) {
	const auto mismatchPair = std::mismatch(path.begin(), path.end(), base.begin(), base.end());
	return mismatchPair.second == base.end();
}

void filesystemWrapper(sol::state &state) {
	auto table = state.create_table("Dirs");
	table.set_function("entries", [](const char *luaPath, const bool recursive = false) {
		const auto path = std::filesystem::path(luaPath);
		if (!isSubPath(path, LDYOM_PATH)) {
			return sol::as_table(std::vector<std::string>());
		}
		std::vector<std::string> entries;
		if (recursive) {
			for (auto &entry : std::filesystem::recursive_directory_iterator(path)) {
				entries.emplace_back(entry.path().lexically_relative(path).string());
			}
		} else {
			for (auto &entry : std::filesystem::directory_iterator(path)) {
				entries.emplace_back(entry.path().filename().string());
			}
		}
		return sol::as_table(entries);
	});
	table.set_function("exists", [](const char *luaPath) {
		const auto path = std::filesystem::path(luaPath);
		if (!isSubPath(path, LDYOM_PATH)) {
			return false;
		}
		return exists(path);
	});
	table.set_function("create", [](const char *luaPath) {
		const auto path = std::filesystem::path(luaPath);
		if (!isSubPath(path, LDYOM_PATH)) {
			return false;
		}
		return create_directories(path);
	});
	table.set_function("remove", [](const char *luaPath) -> uintmax_t {
		const auto path = std::filesystem::path(luaPath);
		if (!isSubPath(path, LDYOM_PATH)) {
			return -1;
		}
		return remove_all(path);
	});
}
