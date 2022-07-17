#pragma once
#include <ostream>
#include <string>

enum ErrorCode {
	Ok,
	NotExists,
	NotSelected
};

class Result {
private:
	unsigned code_ = Ok;
	std::string errorMessage_;
public:
	Result() = default;
	explicit Result(unsigned code, std::string errorMessage): code_(code), errorMessage_(std::move(errorMessage)) {}

	void setError(unsigned code, const std::string& errorMessage);

	[[nodiscard]] bool isDone() const;

	friend std::ostream& operator<<(std::ostream& os, const Result& obj);

	[[nodiscard]] std::string str() const;
};
