#include "Result.h"

#include <sstream>

void Result::setError(unsigned code, const std::string& errorMessage) {
	errorMessage_ = errorMessage;
	code_ = code;
}

bool Result::isDone() const {
	return code_ == Ok;
}

std::ostream& operator<<(std::ostream& os, const Result& obj) {
	return os
		<< "Error code: " << obj.code_ << ", "
		<< "\nMessage: " << obj.errorMessage_ << ", "
		<< "\nSee more about the error in the FAQ.";
}

std::string Result::str() const {
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}
