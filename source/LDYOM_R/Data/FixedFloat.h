#pragma once

#include <concepts>

//! Fixed point number (With implicit conversion to float)
template <std::integral T, float CompressValue>
class FixedFloat {
	friend struct nlohmann::adl_serializer<FixedFloat<T, CompressValue>>;

public:
	constexpr FixedFloat() = default;
	constexpr FixedFloat(float v) : value(static_cast<T>(v * CompressValue)) {}

	template <std::integral Y>
	constexpr FixedFloat(Y x) : value(x) {}

	constexpr operator float() const { return static_cast<float>(value) / CompressValue; }

	void Set(float v, bool round) {
		value = round ? static_cast<T>(v * CompressValue + 0.5f) : static_cast<T>(v * CompressValue);
	}

	//float Get(bool round) const { return round ? (float)v * CompressValue + 0.5f; }

	// I'm not ready for this
	//friend FixedFloat operator+(const FixedFloat& a, const FixedFloat& b) { return a.value + b.value; }
	//friend FixedFloat operator-(const FixedFloat& a, const FixedFloat& b) { return a.value - b.value; }
	//friend FixedFloat operator*(const FixedFloat& a, const FixedFloat& b) { return a.value * b.value; }
	//friend FixedFloat operator/(const FixedFloat& a, const FixedFloat& b) { return a.value / b.value; }
private:
	T value{};
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <std::integral T, float CompressValue>
	struct adl_serializer<FixedFloat<T, CompressValue>> {
		static void to_json(json &j, const FixedFloat<T, CompressValue> &obj) {
			j = obj.value;
		}

		static void from_json(const json &j, FixedFloat<T, CompressValue> &obj) {
			obj.value = j.get<T>();
		}
	};

NLOHMANN_JSON_NAMESPACE_END
