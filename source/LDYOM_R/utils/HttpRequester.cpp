#include "HttpRequester.h"
#define WIN32_LEAN_AND_MEAN
#include "HTTPRequest.hpp"
#include "Logger.h"

std::optional<std::vector<uint8_t>> HttpRequester::get(const std::string& url) {
	try {
		http::Request request{ url };
		const auto response = request.send("GET");
		return response.body;
	} catch (const std::exception& e) {
		Logger::getInstance().log(e.what());
	}
	return std::nullopt;
}
