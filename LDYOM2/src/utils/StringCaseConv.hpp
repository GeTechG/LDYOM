#pragma once

////////////////////////////////////////////////////////////////////////////////
//
// Conversion Helpers to Convert Unicode UTF-16 Strings to Upper and Lower Case
// ============================================================================
//
//                          by Giovanni Dicanio
//                  <giovanni.dicanio AT gmail DOT com>
//
// Last update: 2024, October 9th
//
// Compiler: Visual C++ 2019 in C++17 mode
// The code compiles cleanly at /W4 in both 64-bit and 32-bit builds.
//
// This header-only library exports two functions: ToStringUpper() and
// ToStringLower(), to convert a Unicode UTF-16 string to upper and lower
// case, respectively.
// Errors are signaled throwing exceptions (like StringCaseConversionException).
//
//
// =============================================================================
//
// The MIT License(MIT)
//
// Copyright(c) 2017-2024 by Giovanni Dicanio
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

//==============================================================================
//                              Includes
//==============================================================================

#include <Windows.h> // Win32 APIs (e.g. LCMapStringEx)
#include <crtdbg.h>  // _ASSERTE


#include <stdexcept>    // std::invalid_argument, std::overflow_error
#include <string>       // std::wstring, std::string
#include <string_view>  // std::wstring_view
#include <system_error> // std::system_error

namespace StringCaseConv {

//==============================================================================
// Forward Declarations and Prototypes
//==============================================================================

// Exception representing an error during the conversion
class StringCaseConversionException;

// Convert a string to uppercase
[[nodiscard]] std::wstring ToStringUpper(std::wstring_view source);

// Convert a string to lowercase
[[nodiscard]] std::wstring ToStringLower(std::wstring_view source);

// Convert a string to uppercase, using a specific locale
[[nodiscard]] std::wstring ToStringUpper(std::wstring_view source, const wchar_t* locale);

// Convert a string to lowercase, using a specific locale
[[nodiscard]] std::wstring ToStringLower(std::wstring_view source, const wchar_t* locale);

//==============================================================================
// Exception representing an error during the conversion.
//==============================================================================
class StringCaseConversionException : public std::system_error {
  public:
	StringCaseConversionException(DWORD errorCode, const char* message)
		: std::system_error{(int)errorCode, std::system_category(), message} {}

	StringCaseConversionException(DWORD errorCode, const std::string& message)
		: std::system_error{(int)errorCode, std::system_category(), message} {}
};

//==============================================================================
//                           Helper Functions
//==============================================================================

namespace detail {

//------------------------------------------------------------------------------
//                      Helper Function Prototypes
//------------------------------------------------------------------------------

[[nodiscard]] int SafeSizeToInt(size_t sizeValue);

[[nodiscard]] std::wstring LCMapStringEx(const wchar_t* locale, std::wstring_view source, DWORD conversionFlags);

enum class StringCaseConversionType { ToUpperCase, ToLowerCase };

[[nodiscard]] std::wstring ToStringCase(std::wstring_view source, StringCaseConversionType conversionType,
                                        const wchar_t* locale);

//------------------------------------------------------------------------------
//                      Helper Function Implementations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Safely convert from size_t to int.
// See this blog post for more details:
// https://giodicanio.com/2023/10/20/beware-of-unsafe-conversions-from-size_t-to-int/
//
// Throws a std::overflow_error exception if the conversion is impossible.
//------------------------------------------------------------------------------
inline int SafeSizeToInt(const size_t sizeValue) {
	constexpr int kIntMax = (std::numeric_limits<int>::max)();
	if (sizeValue > static_cast<size_t>(kIntMax)) {
		throw std::overflow_error("size_t value is too big to fit into an int.");
	}

	return static_cast<int>(sizeValue);
}

//------------------------------------------------------------------------------
// Wraps the LCMapStringEx Win32 API.
// Throws a StringCaseConversionException on error.
//------------------------------------------------------------------------------
inline std::wstring LCMapStringEx(const wchar_t* const locale, const std::wstring_view source,
                                  const DWORD conversionFlags) {
	// Special case of empty input string
	if (source.empty()) {
		return std::wstring{};
	}

	// Compute source length (in wchar_t's) with a safe check when converting from size_t to int.
	// This will be used in the following LCMapStringEx function calls.
	const int sourceLength = detail::SafeSizeToInt(source.size());

	// Assume the output string is the same size as the input string;
	// if the input size is insufficient, we will resize the output
	// string buffer later
	std::wstring resultString;
	resultString.resize(source.size());
	int resultLength = ::LCMapStringEx(locale, conversionFlags, source.data(), sourceLength, resultString.data(),
	                                   detail::SafeSizeToInt(resultString.size()), nullptr, nullptr, 0);
	// If the function succeeded, just return the result converted string
	if (resultLength != 0) {
		// On success, LCMapStringEx returns the length in wchar_t's
		// of the result string.
		//
		// In case the output result string is shorter than the input string,
		// properly resize the result string object
		if (resultLength < static_cast<int>(resultString.size())) {
			resultString.resize(resultLength);
		}

		return resultString;
	}

	// The previous call to LCMapStringEx failed.
	// Let's see the reason why.

	DWORD errorCode = ::GetLastError();
	if (errorCode != ERROR_INSUFFICIENT_BUFFER) {
		// Throw an exception as this is an unexpected error
		throw StringCaseConversionException{errorCode, "LCMapStringEx failed when converting the string."};
	}

	// The LCMapStringEx function call failed with ERROR_INSUFFICIENT_BUFFER,
	// because it needs a larger destination buffer.
	// To figure out the required destination buffer size, call LCMapStringEx
	// with cchDest set to 0.
	const int requiredBufferSize = ::LCMapStringEx(locale, conversionFlags, source.data(), sourceLength,
	                                               nullptr, // no destination buffer pointer required now
	                                               0, // Pass 0 for cchDest to query the required output buffer size
	                                               nullptr, nullptr, 0);

	if (requiredBufferSize == 0) {
		// The function failed again.
		// Get the error code.
		errorCode = ::GetLastError();

		// Throw an exception as this is an unexpected error
		throw StringCaseConversionException{errorCode,
		                                    "LCMapStringEx failed when querying the destination buffer size."};
	}

	// Resize the destination string with proper buffer size
	resultString.resize(requiredBufferSize);

	// Call the LCMapStringEx again with the newly allocated larger buffer
	resultLength = ::LCMapStringEx(locale, conversionFlags, source.data(), sourceLength, resultString.data(),
	                               detail::SafeSizeToInt(resultString.size()), nullptr, nullptr, 0);

	// If the function succeeded, just return the result converted string
	if (resultLength != 0) {
		// Resize result string object in case of shorter result
		if (resultLength < static_cast<int>(resultString.size())) {
			resultString.resize(resultLength);
		}

		return resultString;
	} else {
		// The function returned 0, meaning failure.

		// Retrieve the error code
		errorCode = ::GetLastError();

		// Throw an exception as this is an unexpected error
		throw StringCaseConversionException{errorCode, "LCMapStringEx failed when converting the string."};
	}
}

inline std::wstring ToStringCase(const std::wstring_view source, const StringCaseConversionType conversionType,
                                 const wchar_t* const locale) {
	// Special case of empty input string
	if (source.empty()) {
		return std::wstring{};
	}

	// Convert to upper or lower case, using linguistic rules
	const DWORD conversionFlags = [=]() {
		DWORD flag = LCMAP_LINGUISTIC_CASING;

		if (conversionType == StringCaseConversionType::ToUpperCase) {
			flag |= LCMAP_UPPERCASE;
		} else if (conversionType == StringCaseConversionType::ToLowerCase) {
			flag |= LCMAP_LOWERCASE;
		} else {
			// No other values are possible for the string case
			_ASSERTE(FALSE);
			throw std::invalid_argument{"Invalid string conversion enum value."};
		}

		return flag;
	}();

	// Use this library's higher-level C++ wrapper around the ::LCMapStringEx Win32 API
	return detail::LCMapStringEx(locale, source, conversionFlags);
}

} // namespace detail

//==============================================================================
//                         Function Implementations
//==============================================================================

inline std::wstring ToStringUpper(const std::wstring_view source) {
	return ToStringUpper(source, LOCALE_NAME_INVARIANT);
}

inline std::wstring ToStringLower(const std::wstring_view source) {
	return ToStringLower(source, LOCALE_NAME_INVARIANT);
}

inline std::wstring ToStringUpper(const std::wstring_view source, const wchar_t* locale) {
	return detail::ToStringCase(source, detail::StringCaseConversionType::ToUpperCase, locale);
}

inline std::wstring ToStringLower(const std::wstring_view source, const wchar_t* locale) {
	return detail::ToStringCase(source, detail::StringCaseConversionType::ToLowerCase, locale);
}

} // namespace StringCaseConv
