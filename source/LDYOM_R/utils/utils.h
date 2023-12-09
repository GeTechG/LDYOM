#pragma once
#include <optional>
#include <string>
#include <vector>

#define NOMINMAX

#include <array>
#include <CMatrix.h>
#include <CQuaternion.h>
#include <CRGBA.h>
#include <CVector.h>
#include <filesystem>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "../Data/Texture.h"

#include "../Data/IUuidable.h"

#define PI 3.14159265358979323846f
#define	DEG(rad) ((rad)*180.0f/PI)
#define RAD(deg) ((deg)*PI/180.0f)

#if defined(__cpp_char8_t)

template <typename T>
const char* u8Cpp20(T &&t) noexcept {
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

class utils {
public:
	static bool Combo(const char *label, int *currentItem, const std::vector<std::string> &items, int count = 0);
	static bool Combo(const char *label, boost::uuids::uuid *currentItem, int currentElement, int size,
	                  const std::function<std::string&(int)>
	                  &getName, const std::function<boost::uuids::uuid(int)> &getUuid);
	static bool ToggleButton(const char *strId, bool *v);
	static bool LoadTextureFromFile(std::filesystem::path filename, PDIRECT3DTEXTURE9 *out_texture, int *out_width,
	                                int *out_height);
	static std::optional<std::unique_ptr<Texture>> LoadTextureRequiredFromFile(std::filesystem::path filename);
	static bool LoadTextureFromMemory(const void *pointer, unsigned size, PDIRECT3DTEXTURE9 *out_texture,
	                                  int *out_width,
	                                  int *out_height);
	static std::optional<std::unique_ptr<Texture>> LoadTextureRequiredFromMemory(const void *pointer, unsigned size);
	static std::optional<std::unique_ptr<Texture>> LoadTextureRequiredFromURL(const std::string &url);
	static void controlCamera(const CVector &position);
	static bool controlCameraWithMove(float *pos, bool withZ = true, float zOffset = 0.0f);
	static std::wstring stringToPathString(const std::string &string);
	static unsigned int getCarColorRgba(unsigned char id);
	template <class T>
	static int indexByUuid(const std::vector<std::unique_ptr<T>> &vector, boost::uuids::uuid uuid);
	static int createBlip(float *pos, int blipType, int blipColor, int blipSprite = 0);
	static std::vector<std::string> getFilenameList(const std::filesystem::path &path,
	                                                const std::vector<std::string> &extensions);
	template <typename... Args>
	static std::string stringFormat(const std::string &format, Args... args);
	static std::string floatArrayColorToHex(const std::array<float, 4> &color);
	static std::array<float, 4> hexToFloatArrayColor(const std::string &hexColor);
	static bool getScreenPositionFromGamePosition(const CVector &position, ImVec2 &out);
};

template <class T>
int utils::indexByUuid(const std::vector<std::unique_ptr<T>> &vector, boost::uuids::uuid uuid) {
	if (uuid.is_nil())
		return -1;

	const auto it = std::ranges::find_if(vector, [&uuid](const std::unique_ptr<T> &ptr) {
		IUuidable *uuidable = ptr.get();
		return uuid == uuidable->getUuid();
	});

	if (it != vector.end())
		return it - vector.begin();
	return -1;
}

inline void rotateVec2(float &x, float &y, const float angle) {
	const float x1 = x * cos(RAD(angle)) - y * sin(RAD(angle));
	const float y1 = x * sin(RAD(angle)) + y * cos(RAD(angle));
	x = x1;
	y = y1;
}

inline CRGBA floatColorToCRGBA(const std::array<float, 4> &color) {
	return CRGBA{
		static_cast<unsigned char>(color[0] * 255.0f),
		static_cast<unsigned char>(color[1] * 255.0f),
		static_cast<unsigned char>(color[2] * 255.0f),
		static_cast<unsigned char>(color[3] * 255.0f)
	};
}

template <typename... Args>
std::string utils::stringFormat(const std::string &format, Args... args) {
	const int sizeS = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
	if (sizeS <= 0) { throw std::runtime_error("Error during formatting."); }
	const auto size = static_cast<size_t>(sizeS);
	const std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
