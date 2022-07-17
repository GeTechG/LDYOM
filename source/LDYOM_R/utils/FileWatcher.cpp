#include "FileWatcher.h"

#include <thread>
#include <utility>

void FileWatcher::process() {
	while (work) {
		std::vector<std::filesystem::path> filepaths;
		std::vector<HANDLE> handles;
		for (const auto &[fst, snd] : watches_) {
			filepaths.emplace_back(fst);
			handles.emplace_back(snd.handle);
		}

		const DWORD dwWaitStatus = WaitForMultipleObjects(watches_.size(),
			handles.data(),
			FALSE,
			1000);

		if (const unsigned long result = dwWaitStatus - (WAIT_OBJECT_0); result < handles.size()) {
			std::lock_guard guard(tasks_mutex);
			tasks_.emplace_back(&watches_.at(filepaths.at(result)).callback);

			FindNextChangeNotification(handles[result]);
		}
	}
}

void FileWatcher::start() {
	if (!work) {
		work = true;
		auto watcherThread = std::thread(process);
		watcherThread.detach();
	}
}

void FileWatcher::stop() {
	work = false;
}

void FileWatcher::addWatch(const std::filesystem::path& path, const BOOL subtree, const DWORD filter, std::function<void()> callback) {
	watches_.emplace(path, WatcherTask{
		path,
		std::move(callback), 
		FindFirstChangeNotification(path.wstring().c_str(), subtree, filter)
	});
}

void FileWatcher::removeWatch(const std::filesystem::path& path) {
	watches_.erase(path);
}

void FileWatcher::runTasks() {
	std::lock_guard guard(tasks_mutex);
	for (const auto task : tasks_)
		(*task)();
	tasks_.clear();
}
