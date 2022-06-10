#pragma once
#include <string>
#include "CMessages.h"
#include "ShlObj.h"

inline bool isUtf8(const char* string)
{
    if (!string)
        return false;

    const auto* bytes = reinterpret_cast<const unsigned char*>(string);
    while (*bytes)
    {
        if ((bytes[0] == 0x09 || bytes[0] == 0x0A || bytes[0] == 0x0D ||
            (0x20 <= bytes[0] && bytes[0] <= 0x7E)
            )
            )
        {
            bytes += 1;
            continue;
        }

        if (( // non-overlong 2-byte
            (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
            (0x80 <= bytes[1] && bytes[1] <= 0xBF)
            )
            )
        {
            bytes += 2;
            continue;
        }

        if (( // excluding overlongs
            bytes[0] == 0xE0 &&
            (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
            (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            ( // straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                ) ||
            ( // excluding surrogates
                bytes[0] == 0xED &&
                (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                )
            )
        {
            bytes += 3;
            continue;
        }

        if (( // planes 1-3
            bytes[0] == 0xF0 &&
            (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
            (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
            (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            ( // planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                ) ||
            ( // plane 16
                bytes[0] == 0xF4 &&
                (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                )
            )
        {
            bytes += 4;
            continue;
        }

        return false;
    }

    return true;
}

inline std::string utf8ToCp1251(std::string const& utf8) {
    if (!utf8.empty())
    {
        if (!isUtf8(utf8.c_str()))
        {
            return utf8;
        }
        const int wchlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), nullptr, 0);
        if (wchlen > 0 && wchlen != 0xFFFD)
        {
            std::vector<wchar_t> wbuf(wchlen);
            MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), &wbuf[0], wchlen);
            std::vector<char> buf(wchlen);
            WideCharToMultiByte(1251, 0, &wbuf[0], wchlen, &buf[0], wchlen, nullptr, nullptr);

            return { &buf[0], static_cast<unsigned>(wchlen) };
        }
    }
    return {};
}

inline std::string cp1251ToUtf8(const char* str) {
    std::string res;
    WCHAR* ures = nullptr;
    char* cres = nullptr;

    const int resultU = MultiByteToWideChar(1251, 0, str, -1, nullptr, 0);
    if (resultU != 0)
    {
        ures = new WCHAR[resultU];
        if (MultiByteToWideChar(1251, 0, str, -1, ures, resultU))
        {
            const int resultC = WideCharToMultiByte(CP_UTF8, 0, ures, -1, nullptr, 0, nullptr, nullptr);
            if (resultC != 0)
            {
                cres = new char[resultC];
                if (WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, resultC, nullptr, nullptr))
                {
                    res = cres;
                }
            }
        }
    }

    delete[] ures;
    delete[] cres;

    return res;
}


inline void gxtEncode(std::string& str1251, const bool toSl = true) {
    static std::string symRu = utf8ToCp1251("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя");
        static std::string symSl = utf8ToCp1251("AЂ‹‚ѓEE„€…†K‡–­OЊPCЏYЃX‰ЌЋЉђ‘’“”•a—ў™љee›џњќkћЇ®oЈpc¦yx ¤ҐЎ§Ё©Є«¬");

            for (int i = 0; i < 66; i++) {
                if (toSl)
                    std::ranges::replace(str1251, symRu[i], symSl[i]);
                else
                    std::ranges::replace(str1251, symSl[i], symRu[i]);
            }
}


inline size_t strlcpy(char* dest, const char* src, const size_t dest_size) {
    const size_t srcLen = strlen(src);
    if (srcLen < dest_size) {
        memmove(dest, src, srcLen + 1);  // handles overlap without UB
    }
    else if (dest_size > 0) {
        // Not enough room
        memmove(dest, src, dest_size - 1);  // handles overlap without UB
        dest[dest_size - 1] = '\0';
    }
    return srcLen;  // I do not think OP's return value is correct. S/B src length.
}


inline size_t strlcat(char* destination, const char* source, size_t size)
{
    size_t len;

    for (len = 0; len < size; len++) {
        if ('\0' == destination[len]) {
            break;
        }
    }

    return len + strlcpy(destination + len, source, size - len);
}

inline std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end;
    const size_t delimLen = delimiter.length();
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        std::string token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delimLen;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}


// trim from start (in place)
template <typename T>
void ltrim(std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s) {
    s.erase(s.begin(), std::ranges::find_if(s, [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
template <typename T>
void rtrim(std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
template <typename T>
void trim(std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s) {
    ltrim(s);
    rtrim(s);
}

inline std::wstring utf16FromUtf8(const std::string& utf8) {
    if (utf8.empty())
        return {};

    const int utf16Length = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.data(),
        static_cast<int>(utf8.length()),
        nullptr,
        0
    );
    if (utf16Length == 0)
    {
        // Error
        DWORD error = GetLastError();
        throw;
    }


    std::wstring utf16;
    utf16.resize(utf16Length);

    if (!MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.data(),
        static_cast<int>(utf8.length()),
        &utf16[0],
        static_cast<int>(utf16.length())
    ))
    {
        const DWORD error = GetLastError();
        throw std::exception(std::to_string(error).c_str());
    }

    return utf16;
}

inline std::string wstrToUtf8Str(const std::wstring& wstr) {
    std::string retStr;
    if (!wstr.empty()) {
        const int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (sizeRequired > 0) {
            std::vector<char> utf8String(sizeRequired);
            const int bytesConverted = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8String[0], utf8String.size(),
                nullptr, nullptr);
            if (bytesConverted != 0) { retStr = &utf8String[0]; }
            else {
                throw std::exception();
            }
        }
    }
    return retStr;
}