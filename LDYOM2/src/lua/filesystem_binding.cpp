#include "filesystem_binding.h"
#include <filesystem>
#include <fstream>
#include <paths.h>

namespace {

const std::filesystem::path BASE_PATH = LDYOM_PATH("");

bool isSubPath(const std::filesystem::path& path, const std::filesystem::path& base) {
	auto rel = std::filesystem::relative(path, base);
	return !rel.empty() && rel.native()[0] != '.';
}

std::optional<std::filesystem::path> resolveSafe(const char* luaPath) {
	auto path = std::filesystem::path(PLUGIN_PATH((char*)"")) / luaPath;
	std::error_code ec;
	path = std::filesystem::weakly_canonical(path, ec);
	if (ec || !isSubPath(path, BASE_PATH)) {
		return std::nullopt;
	}
	return path;
}

} // namespace

void register_filesystem_bindings(sol::state& state) {
	auto table = state.create_table("Dirs");

	table.set_function("entries", [](const char* luaPath, const bool recursive) {
		auto path = resolveSafe(luaPath);
		if (!path) return sol::as_table(std::vector<std::string>());

		std::error_code ec;
		if (!std::filesystem::is_directory(*path, ec) || ec)
			return sol::as_table(std::vector<std::string>());

		std::vector<std::string> entries;
		if (recursive) {
			for (auto& entry : std::filesystem::recursive_directory_iterator(*path, ec)) {
				entries.emplace_back(entry.path().lexically_relative(*path).string());
			}
		} else {
			for (auto& entry : std::filesystem::directory_iterator(*path, ec)) {
				entries.emplace_back(entry.path().filename().string());
			}
		}
		return sol::as_table(entries);
	});

	table.set_function("exists", [](const char* luaPath) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::error_code ec;
		return std::filesystem::exists(*path, ec) && !ec;
	});

	table.set_function("isFile", [](const char* luaPath) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::error_code ec;
		return std::filesystem::is_regular_file(*path, ec) && !ec;
	});

	table.set_function("isDir", [](const char* luaPath) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::error_code ec;
		return std::filesystem::is_directory(*path, ec) && !ec;
	});

	table.set_function("create", [](const char* luaPath) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::error_code ec;
		std::filesystem::create_directories(*path, ec);
		return !ec;
	});

	table.set_function("remove", [](const char* luaPath) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::error_code ec;
		std::filesystem::remove_all(*path, ec);
		return !ec;
	});

	table.set_function("rename", [](const char* fromPath, const char* toPath) {
		auto from = resolveSafe(fromPath);
		auto to   = resolveSafe(toPath);
		if (!from || !to) return false;
		std::error_code ec;
		std::filesystem::rename(*from, *to, ec);
		return !ec;
	});

	table.set_function("readFile", [](const char* luaPath) -> sol::optional<std::string> {
		auto path = resolveSafe(luaPath);
		if (!path) return sol::nullopt;
		std::ifstream f(*path, std::ios::binary);
		if (!f) return sol::nullopt;
		return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	});

	table.set_function("writeFile", [](const char* luaPath, const std::string& content) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::error_code ec;
		std::filesystem::create_directories(path->parent_path(), ec);
		std::ofstream f(*path, std::ios::binary | std::ios::trunc);
		if (!f) return false;
		f << content;
		return f.good();
	});

	table.set_function("appendFile", [](const char* luaPath, const std::string& content) {
		auto path = resolveSafe(luaPath);
		if (!path) return false;
		std::ofstream f(*path, std::ios::binary | std::ios::app);
		if (!f) return false;
		f << content;
		return f.good();
	});
}
