#pragma once
#include <string>


class TimerService {
private:
	bool timerOn_ = false;

	TimerService () = default;
	TimerService (const TimerService & root) = delete;
	TimerService & operator=(const TimerService &) = delete;
public:
	static TimerService & getInstance() {
		static TimerService instance;
		return instance;
	}

	bool& isTimerOn();
	void addTimer(const char* text, const bool direction, const int value);
	void removeTimer();
	int getTimerTime();
};

