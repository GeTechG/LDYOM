#pragma once
#include <array>
#include <string>


class CounterService {
public:
	CounterService(const CounterService &) = delete;
	CounterService& operator=(const CounterService &) = delete;
	~CounterService() = default;

	static CounterService& getInstance() {
		static CounterService instance{};
		return instance;
	}

	int addNumberCounter(const std::string &title, int initialValue);
	int addNumberCounterAtLine(const std::string &title, int initialValue, unsigned short lineId);

	int addBarCounter(const std::string &title, int initialValue);
	int addBarCounterAtLine(const std::string &title, int initialValue, unsigned short lineId);

	int addStagesCounter(const std::string &title, int initialValue, unsigned short maxCount,
	                     unsigned char colorId);
	int addStagesCounterAtLine(const std::string &title, int initialValue, unsigned short maxCount,
	                           unsigned char colorId, unsigned short lineId);

	void updateCounter(int counterId, int value) const;

	void clearCounter(int counterId);

private:
	std::array<bool, 4> usedCounters;

	explicit CounterService() = default;
};
