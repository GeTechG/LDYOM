#pragma once
#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>

class DataContainer {
  public:
	virtual ~DataContainer() = default;
	virtual void callRenderer() = 0;
};

template <typename T> class TypedDataContainer : public DataContainer {
  public:
	T value;
	std::function<void(T&)> editorCallback;

	TypedDataContainer(T initialValue, std::function<void(T&)> callback)
		: value(initialValue),
		  editorCallback(callback) {}

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
		: type(std::string(type)),
		  name(std::string(name)),
		  content(std::make_shared<TypedDataContainer<T>>(initialValue, callback)),
		  contentType(typeid(T)) {}

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
};