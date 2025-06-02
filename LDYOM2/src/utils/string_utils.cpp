#include "string_utils.h"
#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <codecvt>
#include <cwctype>
#include <functional>
#include <settings.h>
#include <string>

std::wstring utf8_to_wstring(const std::string_view& utf8_str) {
	try {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(utf8_str.data(), utf8_str.data() + utf8_str.size());
	} catch (const std::exception&) {
		return std::wstring();
	}
}

std::string wstring_to_utf8(const std::wstring& wstr) {
	try {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(wstr);
	} catch (const std::exception&) {
		return std::string();
	}
}

wchar_t wchar_to_lower(wchar_t c) {
	if (c <= 127) {
		return towlower(c);
	}

	wchar_t input[2] = {c, L'\0'};
	wchar_t output[2] = {0, 0};

	int result = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING, input, 1, output, 2,
	                           NULL, NULL, 0);

	if (result > 0) {
		return output[0];
	}

	return c;
}

bool is_wchar_lower(wchar_t c) {
	if (c <= 127) {
		return iswlower(c);
	}

	wchar_t input[2] = {c, L'\0'};
	wchar_t lower[2] = {0, 0};
	wchar_t upper[2] = {0, 0};

	LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING, input, 1, lower, 2, NULL, NULL,
	              0);

	LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING, input, 1, upper, 2, NULL, NULL,
	              0);

	return (lower[0] != upper[0] && c == lower[0]);
}

bool is_wchar_upper(wchar_t c) {
	if (c <= 127) {
		return iswupper(c);
	}

	wchar_t input[2] = {c, L'\0'};
	wchar_t lower[2] = {0, 0};
	wchar_t upper[2] = {0, 0};

	LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING, input, 1, lower, 2, NULL, NULL,
	              0);

	LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING, input, 1, upper, 2, NULL, NULL,
	              0);

	return (lower[0] != upper[0] && c == upper[0]);
}

bool is_wchar_digit(wchar_t c) {
	if (c <= 127) {
		return iswdigit(c);
	}

	if (c >= 0x0030 && c <= 0x0039)
		return true;
	if (c >= 0x0660 && c <= 0x0669)
		return true;
	if (c >= 0x06F0 && c <= 0x06F9)
		return true;
	if (c >= 0x0966 && c <= 0x096F)
		return true;
	if (c >= 0x09E6 && c <= 0x09EF)
		return true;
	return false;
}

bool is_wchar_space(wchar_t c) {
	if (c <= 127) {
		return iswspace(c);
	}

	switch (c) {
		case 0x0020: // Space
		case 0x00A0: // No-Break Space
		case 0x2000: // En Quad
		case 0x2001: // Em Quad
		case 0x2002: // En Space
		case 0x2003: // Em Space
		case 0x2004: // Three-Per-Em Space
		case 0x2005: // Four-Per-Em Space
		case 0x2006: // Six-Per-Em Space
		case 0x2007: // Figure Space
		case 0x2008: // Punctuation Space
		case 0x2009: // Thin Space
		case 0x200A: // Hair Space
		case 0x202F: // Narrow No-Break Space
		case 0x205F: // Medium Mathematical Space
		case 0x3000: // Ideographic Space
			return true;
	}

	return false;
}

std::string to_snake_case(std::string_view text) {
	std::wstring wtext = utf8_to_wstring(text);
	std::wstring result;
	result.reserve(wtext.size() * 2);

	std::locale loc("");

	for (size_t i = 0; i < wtext.size(); ++i) {
		wchar_t c = wtext[i];

		if (is_wchar_space(c) || c == L'-' || c == L'_') {
			result += L'_';
		} else if (c == L'<' || c == L'>' || c == L':' || c == L'"' || c == L'/' || c == L'\\' || c == L'|' ||
		           c == L'?' || c == L'*') {
			result += L'_';
		} else {
			if (is_wchar_upper(c)) {
				if (i > 0 && (is_wchar_lower(wtext[i - 1]) || is_wchar_digit(wtext[i - 1]))) {
					result += L'_';
				}
				c = wchar_to_lower(c);
			}
			result += c;
		}
	}

	return wstring_to_utf8(result);
}

std::vector<std::string> split(std::string_view s, std::string_view delimiter) {
	std::vector<std::string> result;
	size_t pos = 0;
	size_t last = 0;

	while ((pos = s.find(delimiter, last)) != std::string_view::npos) {
		result.emplace_back(s.substr(last, pos - last));
		last = pos + delimiter.length();
	}

	// Add the last part
	if (last < s.length()) {
		result.emplace_back(s.substr(last));
	}

	return result;
}

// Validates if a string is valid UTF-8
bool is_valid_utf8(const std::string& str) noexcept {
	try {
		const auto* data = reinterpret_cast<const unsigned char*>(str.data());
		const auto* end = data + str.size();

		while (data < end) {
			unsigned char byte = *data++;

			// Single-byte character (ASCII)
			if (byte < 0x80) {
				continue;
			}

			// Multi-byte character
			int extra_bytes = 0;
			if ((byte & 0xE0) == 0xC0) {
				extra_bytes = 1;
			} else if ((byte & 0xF0) == 0xE0) {
				extra_bytes = 2;
			} else if ((byte & 0xF8) == 0xF0) {
				extra_bytes = 3;
			} else {
				return false; // Invalid UTF-8 start byte
			}

			// Check if we have enough bytes
			if (data + extra_bytes > end) {
				return false;
			}

			// Check continuation bytes
			for (int i = 0; i < extra_bytes; ++i) {
				if ((*data++ & 0xC0) != 0x80) {
					return false;
				}
			}
		}
		return true;
	} catch (...) {
		return false;
	}
}

std::string utf8_to_cp1251(std::string_view utf8_str) {
	if (utf8_str.empty()) {
		return {};
	}

	// Check if the input is valid UTF-8
	if (!is_valid_utf8(std::string(utf8_str))) {
		return std::string(utf8_str); // Return as-is if not UTF-8
	}

	constexpr UINT cp_utf8 = CP_UTF8;
	constexpr UINT cp_1251 = 1251;

	// Convert UTF-8 to wide char
	const int wide_char_count =
		MultiByteToWideChar(cp_utf8, 0, utf8_str.data(), static_cast<int>(utf8_str.size()), nullptr, 0);

	if (wide_char_count <= 0) {
		return std::string(utf8_str); // Return original on conversion error
	}

	std::vector<wchar_t> wide_buffer(static_cast<size_t>(wide_char_count));

	const int wide_result = MultiByteToWideChar(cp_utf8, 0, utf8_str.data(), static_cast<int>(utf8_str.size()),
	                                            wide_buffer.data(), wide_char_count);

	if (wide_result != wide_char_count) {
		return std::string(utf8_str); // Return original on conversion error
	}

	// Convert wide char to CP1251
	const int cp1251_count =
		WideCharToMultiByte(cp_1251, 0, wide_buffer.data(), wide_char_count, nullptr, 0, nullptr, nullptr);

	if (cp1251_count <= 0) {
		return std::string(utf8_str); // Return original on conversion error
	}

	std::vector<char> cp1251_buffer(static_cast<size_t>(cp1251_count));

	const int cp1251_result = WideCharToMultiByte(cp_1251, 0, wide_buffer.data(), wide_char_count, cp1251_buffer.data(),
	                                              cp1251_count, nullptr, nullptr);

	if (cp1251_result != cp1251_count) {
		return std::string(utf8_str); // Return original on conversion error
	}

	return std::string(cp1251_buffer.data(), static_cast<size_t>(cp1251_count));
}

void gxt_encode(std::string& str1251, GxtEncoding encoding) {
	static const std::string from_cp1251 = utf8_to_cp1251(Settings::instance().getSetting<std::string>("gxt_from", ""));
	static const std::string to_cp1251 = utf8_to_cp1251(Settings::instance().getSetting<std::string>("gxt_to", ""));

	if (from_cp1251.size() != to_cp1251.size()) {
		return;
	}

	const auto& source_chars = (encoding == GxtEncoding::ToSanAndreas) ? from_cp1251 : to_cp1251;
	const auto& target_chars = (encoding == GxtEncoding::ToSanAndreas) ? to_cp1251 : from_cp1251;

	std::transform(str1251.begin(), str1251.end(), str1251.begin(), [&source_chars, &target_chars](char ch) -> char {
		if (const auto pos = source_chars.find(ch); pos != std::string::npos) {
			return target_chars[pos];
		}
		return ch;
	});
}