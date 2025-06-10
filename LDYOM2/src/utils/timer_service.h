#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

class TimerService {
  private:
	bool timerActive_ = false;
	static constexpr std::uintptr_t TIMER_OFFSET = 12640;

	class Timer {
	  public:
		explicit Timer(std::uintptr_t timerAddress) noexcept;
		~Timer() noexcept;

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;
		Timer(Timer&&) = delete;
		Timer& operator=(Timer&&) = delete;

		[[nodiscard]] std::uintptr_t getAddress() const noexcept { return timerAddress_; }

	  private:
		std::uintptr_t timerAddress_;
	};

	std::unique_ptr<Timer> currentTimer_;

	TimerService() = default;

	[[nodiscard]] static std::uintptr_t getScriptSpacePtr() noexcept;
	[[nodiscard]] static std::uintptr_t getTimerAddress() noexcept;

  public:
	TimerService(const TimerService&) = delete;
	TimerService& operator=(const TimerService&) = delete;
	TimerService(TimerService&&) = delete;
	TimerService& operator=(TimerService&&) = delete;
	~TimerService() = default;

	[[nodiscard]] static TimerService& instance() noexcept {
		static TimerService instance{};
		return instance;
	}
	[[nodiscard]] bool isTimerActive() const noexcept { return timerActive_; }

	void addTimer(std::string_view text, bool isDown, int value);
	void removeTimer() noexcept;
	[[nodiscard]] int getTimerTime() const noexcept;
};
