#pragma once
#include <string>
#include <vector>
#include <optional>

#define NOMINMAX

#include "../Data/Texture.h"

#define PI 3.14159265358979323846f
#define	DEG(rad) ((rad)*180.0f/PI)
#define RAD(deg) ((deg)*PI/180.0f) 

#if defined(__cpp_char8_t)

template<typename T>
const char* u8Cpp20(T&& t) noexcept
{
#pragma warning (disable: 26490)
	return reinterpret_cast<const char*>(t);
#pragma warning (default: 26490)
}
#define U8(x) u8Cpp20(u8##x)
#else
#define U8(x) u8##x
#endif

#define CLASS_VERSION(T, N)                                            \
namespace boost {                                                      \
namespace serialization {                                              \
template<>                                                             \
struct version<T >                                                     \
{                                                                      \
    typedef mpl::int_<N> type;                                         \
    typedef mpl::integral_c_tag tag;                                   \
    BOOST_STATIC_CONSTANT(int, value = version::type::value);          \
    /*                                                                 \
    BOOST_MPL_ASSERT((                                                 \
        mpl::equal_to<                                                 \
            :implementation_level<T >,                                 \
            mpl::int_<object_class_info>                               \
        >::value                                                       \
    ));                                                                \
    */                                                                 \
};                                                                     \
}                                                                      \
}

class utils
{
public:
	static std::vector<std::string> split(const std::string& s, const std::string& delimiter);
	static bool Combo(const char* label, int* currentItem, const std::vector<std::string>& items);
    static bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
	static std::optional<Texture> LoadTextureRequiredFromFile(const char* filename);
};

inline std::vector<std::string> utils::split(const std::string& s, const std::string& delimiter) {
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
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::ranges::find_if(s, [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}