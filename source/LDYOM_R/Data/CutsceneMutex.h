#pragma once

class CutsceneMutex {
	friend class CutsceneMutexGuard;

private:
	inline static bool lock_ = false;

public:
	static void free();
	static bool isLocked();
};

class CutsceneMutexGuard {
public:
	CutsceneMutexGuard() {
		CutsceneMutex::lock_ = true;
	}

	~CutsceneMutexGuard() {
		CutsceneMutex::lock_ = false;
	}

	CutsceneMutexGuard(const CutsceneMutexGuard &other) = delete;
	CutsceneMutexGuard(CutsceneMutexGuard &&other) noexcept = delete;
	CutsceneMutexGuard& operator=(const CutsceneMutexGuard &other) = delete;
	CutsceneMutexGuard& operator=(CutsceneMutexGuard &&other) noexcept = delete;
};
