#pragma once
#include <optional>
#include <string>
#include <vector>

class HttpRequester {
public:
	static std::optional<std::vector<uint8_t>> get(const std::string& url);
};
