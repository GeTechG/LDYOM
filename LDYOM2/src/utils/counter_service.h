#pragma once
#include <array>
#include <string>

class CounterService {
  private:
	std::array<bool, 4> usedCounters{};

	CounterService() = default;

  public:
	CounterService(const CounterService&) = delete;
	CounterService& operator=(const CounterService&) = delete;
	CounterService(CounterService&&) = delete;
	CounterService& operator=(CounterService&&) = delete;
	~CounterService() = default;

	[[nodiscard]] static CounterService& instance() noexcept {
		static CounterService instance{};
		return instance;
	}

	[[nodiscard]] int addNumberCounter(const std::string& title, int initialValue);
	[[nodiscard]] int addNumberCounterAtLine(const std::string& title, int initialValue, unsigned short lineId);

	[[nodiscard]] int addBarCounter(const std::string& title, int initialValue);
	[[nodiscard]] int addBarCounterAtLine(const std::string& title, int initialValue, unsigned short lineId);

	[[nodiscard]] int addStagesCounter(const std::string& title, int initialValue, unsigned short maxCount,
	                                   unsigned char colorId);
	[[nodiscard]] int addStagesCounterAtLine(const std::string& title, int initialValue, unsigned short maxCount,
	                                         unsigned char colorId, unsigned short lineId);

	void updateCounter(int counterId, int value) const;
	void clearCounter(int counterId);
	void clearAllCounters() {
		for (size_t i = 0; i < usedCounters.size(); ++i) {
			if (usedCounters[i]) {
				clearCounter(static_cast<int>(i));
			}
		}
	}

  private:
};
