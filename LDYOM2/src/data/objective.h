#pragma once
#include <functional>
#include <ktcoro_wait.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <uuid_wrap.h>

class ObjectiveDataContainer {
  public:
	virtual ~ObjectiveDataContainer() = default;
	virtual void callRenderer() = 0;
	virtual ktwait callExecutor() = 0;
	virtual void callOnStart() = 0;
	virtual void callOnReset() = 0;
	virtual void callOnSelected() = 0;
	virtual void callOnDeselected() = 0;
	virtual nlohmann::json toJson() const = 0;
	virtual void fromJson(const nlohmann::json& j) = 0;
};

template <typename T> class TypedObjectiveDataContainer : public ObjectiveDataContainer {
  public:
	T value;
	std::function<void(T&)> editorCallback;
	std::function<ktwait(T&)> executorCallback;
	std::function<void(T&)> onStartCallback;
	std::function<void(T&)> onResetCallback;
	std::function<void(T&)> onSelectedCallback;
	std::function<void(T&)> onDeselectedCallback;

	TypedObjectiveDataContainer(T initialValue, std::function<void(T&)> editorCallback,
	                            std::function<ktwait(T&)> executorCallback,
	                            std::function<void(T&)> onStartCallback = nullptr,
	                            std::function<void(T&)> onResetCallback = nullptr,
	                            std::function<void(T&)> onSelectedCallback = nullptr,
	                            std::function<void(T&)> onDeselectedCallback = nullptr)
		: value(initialValue),
		  editorCallback(editorCallback),
		  executorCallback(executorCallback),
		  onStartCallback(onStartCallback),
		  onResetCallback(onResetCallback),
		  onSelectedCallback(onSelectedCallback),
		  onDeselectedCallback(onDeselectedCallback) {}

	nlohmann::json toJson() const override { return nlohmann::json(value); }

	void fromJson(const nlohmann::json& j) override { this->value = j; }

	void callRenderer() override { editorCallback(value); }

	ktwait callExecutor() override { return executorCallback(value); }

	void callOnStart() override {
		if (onStartCallback) {
			onStartCallback(value);
		}
	}

	void callOnReset() override {
		if (onResetCallback) {
			onResetCallback(value);
		}
	}

	void callOnSelected() override {
		if (onSelectedCallback) {
			onSelectedCallback(value);
		}
	}

	void callOnDeselected() override {
		if (onDeselectedCallback) {
			onDeselectedCallback(value);
		}
	}
};

class Objective {
  private:
	std::shared_ptr<ObjectiveDataContainer> content;
	std::type_index contentType;

  public:
	bool isInitialized = false;
	std::string type;
	std::string name;
	uuids::uuid id = uuids::uuid_system_generator{}();

	template <typename T>
	Objective(std::string_view type, std::string_view name, T initialValue, std::function<void(T&)> editorCallback,
	          std::function<ktwait(T&)> executorCallback = nullptr, std::function<void(T&)> onStartCallback = nullptr,
	          std::function<void(T&)> onResetCallback = nullptr, std::function<void(T&)> onSelectedCallback = nullptr,
	          std::function<void(T&)> onDeselectedCallback = nullptr)
		: content(std::make_shared<TypedObjectiveDataContainer<T>>(initialValue, editorCallback, executorCallback,
		                                                           onStartCallback, onResetCallback, onSelectedCallback,
		                                                           onDeselectedCallback)),
		  contentType(typeid(T)),
		  type(std::string(type)),
		  name(std::string(name)) {}

	template <typename T> T& getValue() {
		if (typeid(T) != contentType) {
			throw std::bad_cast();
		}

		auto derivedPtr = std::dynamic_pointer_cast<TypedObjectiveDataContainer<T>>(content);
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

	ktwait execute() {
		if (!content) {
			throw std::runtime_error("Content is not initialized");
		}
		return content->callExecutor();
	}

	void onStart() {
		if (content) {
			content->callOnStart();
		}
		isInitialized = true;
	}

	void onReset() {
		if (content) {
			content->callOnReset();
		}
		isInitialized = false;
	}

	void onSelected() {
		if (content) {
			content->callOnSelected();
		}
	}

	void onDeselected() {
		if (content) {
			content->callOnDeselected();
		}
	}

	friend void to_json(nlohmann::json& j, const Objective& p) {
		j = nlohmann::json{{"type", p.type}, {"name", p.name}, {"content", p.content->toJson()}, {"id", p.id}};
	}

	friend void from_json(const nlohmann::json& j, Objective& p) {
		j.at("name").get_to(p.name);
		if (j.contains("content")) {
			p.content->fromJson(j.at("content"));
		}
		j.at("id").get_to(p.id);
	}
};