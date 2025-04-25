#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

std::string to_snake_case(std::string_view text);
std::vector<std::string> split(std::string_view s, std::string_view delimiter);

inline std::string& ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	return s;
}

inline std::string& rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
	return s;
}

inline std::string& trim(std::string& s) { return ltrim(rtrim(s)); }

inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

inline std::string trim_copy(std::string s) {
	trim(s);
	return s;
}