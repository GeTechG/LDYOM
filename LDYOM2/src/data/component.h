#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Component {
public:
	virtual ~Component() = default;
	std::string type;
	std::string name;

	Component(const std::string_view type, const std::string_view name)
		: type(std::string(type)),
		  name(std::string(name)) {}

	[[nodiscard]] std::string getType() const { return type; }

	[[nodiscard]] virtual nlohmann::json to_json() const { return nlohmann::json{{"type", type}, {"name", name}}; }

	virtual void from_json(const nlohmann::json& j) { j.at("name").get_to(name); }

	virtual void editorRender() = 0;
	virtual void onStart() {}
	virtual void onUpdate(float deltaTime) {}
};