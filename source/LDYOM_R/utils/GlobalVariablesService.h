#pragma once
#include <memory>
#include <optional>
#include <string>
#include <nlohmann/json.hpp>

enum class GlobalVariableType {
	Float,
	Int,
	String,
	Bool
};

struct GlobalVariableView {
	class Value {
	private:
		GlobalVariableType type;

	public:
		void *value = nullptr;

		explicit Value(GlobalVariableType type);
		void deleteValue() const;

		//move semantics
		Value(Value &&other) noexcept;
		Value& operator=(Value &&other) noexcept;

		//copy semantics
		Value(const Value &other);
		Value& operator=(const Value &other);

		void setType(GlobalVariableType type);
		GlobalVariableType getType() const;

		~Value();
	};

	std::string name;
	std::string uuid;
	Value value{GlobalVariableType::Float};
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<GlobalVariableView::Value> {
		static void to_json(json &j, const GlobalVariableView::Value &obj) {
			j["type"] = obj.getType();
			switch (obj.getType()) {
				case GlobalVariableType::Float:
					j["value"] = *static_cast<float*>(obj.value);
					break;
				case GlobalVariableType::Int:
					j["value"] = *static_cast<int*>(obj.value);
					break;
				case GlobalVariableType::String:
					j["value"] = *static_cast<std::string*>(obj.value);
					break;
				case GlobalVariableType::Bool:
					j["value"] = *static_cast<bool*>(obj.value);
					break;
			}
		}

		static void from_json(const json &j, GlobalVariableView::Value &obj) {
			obj.setType(j["type"].get<GlobalVariableType>());
			switch (obj.getType()) {
				case GlobalVariableType::Float:
					*static_cast<float*>(obj.value) = j["value"].get<float>();
					break;
				case GlobalVariableType::Int:
					*static_cast<int*>(obj.value) = j["value"].get<int>();
					break;
				case GlobalVariableType::String:
					*static_cast<std::string*>(obj.value) = j["value"].get<std::string>();
					break;
				case GlobalVariableType::Bool:
					*static_cast<bool*>(obj.value) = j["value"].get<bool>();
					break;
			}
		}
	};

NLOHMANN_JSON_NAMESPACE_END

class GlobalVariablesService {
public:
	GlobalVariablesService(const GlobalVariablesService &) = delete;
	GlobalVariablesService& operator=(const GlobalVariablesService &) = delete;
	~GlobalVariablesService() = default;

	static GlobalVariablesService& getInstance() {
		static GlobalVariablesService instance{};
		return instance;
	}

	int create(const std::string &name, GlobalVariableType type);
	void remove(const std::string &uuid);
	void remove(int index);
	int getSize();
	std::pair<int, GlobalVariableView> get(int index);
	std::pair<int, GlobalVariableView> get(const std::string &uuid);
	void set(int index, GlobalVariableView &value);

	std::string toJson();
	void fromJson(const std::string &json);

private:
	explicit GlobalVariablesService() = default;
};
