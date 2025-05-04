#pragma once
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <typeindex>
#include <typeinfo>

class DataContainer {
  public:
	virtual ~DataContainer() = default;
	virtual void callRenderer() = 0;
	virtual nlohmann::json toJson() const = 0;
	virtual void fromJson(const nlohmann::json& j) = 0;
};

template <typename T> class TypedDataContainer : public DataContainer {
  public:
	T value;
	std::function<void(T&)> editorCallback;

	TypedDataContainer(T initialValue, std::function<void(T&)> callback)
		: value(initialValue),
		  editorCallback(callback) {}

	nlohmann::json toJson() const override { return nlohmann::json(value); }

	void fromJson(const nlohmann::json& j) override { this->value = j; }

	void callRenderer() override {
		if (editorCallback) {
			editorCallback(value);
		}
	}
};

class Objective {
  private:
	std::shared_ptr<DataContainer> content;
	std::type_index contentType;

  public:
	std::string type;
	std::string name;

	template <typename T>
	Objective(std::string_view type, std::string_view name, T initialValue, std::function<void(T&)> callback)
		: content(std::make_shared<TypedDataContainer<T>>(initialValue, callback)),
		  contentType(typeid(T)),
		  type(std::string(type)),
		  name(std::string(name)) {}

	template <typename T> T& getValue() {
		if (typeid(T) != contentType) {
			throw std::bad_cast();
		}

		auto derivedPtr = std::dynamic_pointer_cast<TypedDataContainer<T>>(content);
		if (!derivedPtr) {
			throw std::bad_cast();
		}

		return derivedPtr->value;
	}

	const std::type_index& getValueType() const { return contentType; }

	void renderEditor() {
		if (content) {
			content->callRenderer();
		}
	}

	friend void to_json(nlohmann::json& j, const Objective& p) {
		j = nlohmann::json{{"type", p.type}, {"name", p.name}, {"content", p.content->toJson()}};
	}

	friend void from_json(const nlohmann::json& j, Objective& p) {
		j.at("name").get_to(p.name);
		if (j.contains("content")) {
			p.content->fromJson(j.at("content"));
		}
	}
};