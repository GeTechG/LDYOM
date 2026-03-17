#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <variant>

enum class GlobalVarType { Number = 0, String = 1, Boolean = 2 };

NLOHMANN_JSON_SERIALIZE_ENUM(GlobalVarType, {
												{GlobalVarType::Number, "number"},
												{GlobalVarType::String, "string"},
												{GlobalVarType::Boolean, "boolean"},
											})

using VarValue = std::variant<double, std::string, bool>;

struct GlobalVariable {
	std::string name;
	VarValue defaultValue = 0.0;

	GlobalVarType type() const { return static_cast<GlobalVarType>(defaultValue.index()); }

	friend void to_json(nlohmann::json& j, const GlobalVariable& v);
	friend void from_json(const nlohmann::json& j, GlobalVariable& v);
};
