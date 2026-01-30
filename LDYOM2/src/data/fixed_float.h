#pragma once

#include <CVector.h>
#include <concepts>
#include <nlohmann/json.hpp>

//! Fixed point number (With implicit conversion to float)
template <std::integral T, float CompressValue> class FixedFloat {
	friend struct nlohmann::adl_serializer<FixedFloat<T, CompressValue>>;

  public:
	constexpr FixedFloat() = default;
	constexpr FixedFloat(float v)
		: value(static_cast<T>(v * CompressValue)) {}

	template <std::integral Y>
	constexpr FixedFloat(Y x)
		: value(x) {}

	constexpr operator float() const { return static_cast<float>(value) / CompressValue; }

	void Set(float v, bool round) {
		value = round ? static_cast<T>(v * CompressValue + 0.5f) : static_cast<T>(v * CompressValue);
	}

  private:
	T value{};
};

NLOHMANN_JSON_NAMESPACE_BEGIN
template <std::integral T, float CompressValue> struct adl_serializer<FixedFloat<T, CompressValue>> {
	static void to_json(json& j, const FixedFloat<T, CompressValue>& obj) { j = obj.value; }

	static void from_json(const json& j, FixedFloat<T, CompressValue>& obj) { obj.value = j.get<T>(); }
};

NLOHMANN_JSON_NAMESPACE_END

template <typename T, float CompressValue> struct FixedVector {
	constexpr FixedVector() = default;
	constexpr FixedVector(CVector v3d)
		: x(v3d.x),
		  y(v3d.y),
		  z(v3d.z) {}
	constexpr FixedVector(T X, T Y, T Z)
		: x(X),
		  y(Y),
		  z(Z) {}
	constexpr FixedVector(float X, float Y, float Z)
		: x(X),
		  y(Y),
		  z(Z) {}

	constexpr operator CVector() const { return CVector{x, y, z}; }

  public: // Public, because in some cases it might be useful to only access 1
	FixedFloat<T, CompressValue> x{}, y{}, z{};
};

NLOHMANN_JSON_NAMESPACE_BEGIN
template <typename T, float CompressValue> struct adl_serializer<FixedVector<T, CompressValue>> {
	static void to_json(json& j, const FixedVector<T, CompressValue>& obj) {
		j = json::array({json(obj.x), json(obj.y), json(obj.z)});
	}

	static void from_json(const json& j, FixedVector<T, CompressValue>& obj) {
		obj.x = j.at(0).get<FixedFloat<T, CompressValue>>();
		obj.y = j.at(1).get<FixedFloat<T, CompressValue>>();
		obj.z = j.at(2).get<FixedFloat<T, CompressValue>>();
	}
};
NLOHMANN_JSON_NAMESPACE_END