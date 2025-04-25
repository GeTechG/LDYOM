#include "string_utils.h"
#include <Windows.h>
#include <cctype>
#include <codecvt>
#include <cwctype>
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
