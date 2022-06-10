#pragma once
#include <map>
#include <filesystem>
#include <functional>

#define UNICODE
#include <Windows.h>

struct WatcherTask {
	std::filesystem::path path;
	std::function<void()> callback;
	HANDLE handle;
};

class FileWatcher {
private:
	inline static bool work = false;
	inline static std::map<std::filesystem::path, WatcherTask> watches_;
	inline static std::vector<std::function<void()>*> tasks_;
	static void process();
public:
	static void start();
	static void stop();

	static void addWatch(const std::filesystem::path& path, BOOL subtree, DWORD filter, std::function<void()> callback);
	static void removeWatch(const std::filesystem::path& path);

	static void runTasks();
};
