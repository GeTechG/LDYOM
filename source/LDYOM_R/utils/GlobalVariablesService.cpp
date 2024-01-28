#include "GlobalVariablesService.h"
#include "LuaEngine.h"

void GlobalVariableView::Value::setType(const GlobalVariableType type) {
	deleteValue();
	this->type = type;
	switch (type) {
		case GlobalVariableType::Float: {
			value = new float(0.0f);
			break;
		}
		case GlobalVariableType::Int: {
			value = new int(0);
			break;
		}
		case GlobalVariableType::String: {
			value = new std::string("");
			break;
		}
		case GlobalVariableType::Bool: {
			value = new bool(false);
			break;
		}
		default:
			break;
	}
}

GlobalVariableType GlobalVariableView::Value::getType() const { return type; }

GlobalVariableView::Value::Value(const GlobalVariableType type) {
	setType(type);
}

void GlobalVariableView::Value::deleteValue() const {
	if (value == nullptr) {
		return;
	}
	switch (type) {
		case GlobalVariableType::Float: {
			delete static_cast<float*>(value);
			break;
		}
		case GlobalVariableType::Int: {
			delete static_cast<int*>(value);
			break;
		}
		case GlobalVariableType::String: {
			delete static_cast<std::string*>(value);
			break;
		}
		case GlobalVariableType::Bool: {
			delete static_cast<bool*>(value);
			break;
		}
		default:
			break;
	}
}

GlobalVariableView::Value::Value(Value &&other) noexcept: type{other.type},
                                                          value{other.value} {
	other.value = nullptr;
}

GlobalVariableView::Value& GlobalVariableView::Value::operator=(Value &&other) noexcept {
	if (this == &other)
		return *this;
	type = other.type;
	value = other.value;
	other.value = nullptr;
	return *this;
}

GlobalVariableView::Value::Value(const Value &other) {
	setType(other.getType());
	switch (other.getType()) {
		case GlobalVariableType::Float:
			*static_cast<float*>(value) = *static_cast<float*>(other.value);
			break;
		case GlobalVariableType::Int:
			*static_cast<int*>(value) = *static_cast<int*>(other.value);
			break;
		case GlobalVariableType::String:
			*static_cast<std::string*>(value) = *static_cast<std::string*>(other.value);
			break;
		case GlobalVariableType::Bool:
			*static_cast<bool*>(value) = *static_cast<bool*>(other.value);
			break;
	}
}

GlobalVariableView::Value& GlobalVariableView::Value::operator=(const Value &other) {
	if (this == &other)
		return *this;
	this->setType(other.getType());
	switch (other.getType()) {
		case GlobalVariableType::Float:
			*static_cast<float*>(value) = *static_cast<float*>(other.value);
			break;
		case GlobalVariableType::Int:
			*static_cast<int*>(value) = *static_cast<int*>(other.value);
			break;
		case GlobalVariableType::String:
			*static_cast<std::string*>(value) = *static_cast<std::string*>(other.value);
			break;
		case GlobalVariableType::Bool:
			*static_cast<bool*>(value) = *static_cast<bool*>(other.value);
			break;
	}
	return *this;
}

GlobalVariableView::Value::~Value() {
	deleteValue();
}

sol::table getGlobalVariablesTable() {
	auto ld = LuaEngine::getInstance().getLuaState()["ld"].get<sol::table>();
	auto data = ld["data"].get_or_create<sol::table>();
	return data["globalVariables"].get_or_create<sol::table>();
}

int GlobalVariablesService::create(const std::string &name, GlobalVariableType type) {
	auto gvTable = getGlobalVariablesTable();
	auto t = LuaEngine::getInstance().getLuaState().create_table();
	t["type"] = static_cast<int>(type);
	t["name"] = name;
	t["uuid"] = LuaEngine::getInstance().getLuaState()["uuid"]();
	switch (type) {
		case GlobalVariableType::Float: {
			t["value"] = 0.0f;
			break;
		}
		case GlobalVariableType::Int: {
			t["value"] = 0;
			break;
		}
		case GlobalVariableType::String: {
			t["value"] = "";
			break;
		}
		case GlobalVariableType::Bool: {
			t["value"] = false;
			break;
		}
		default:
			break;
	}
	gvTable.add(t);
	return 0;
}

void GlobalVariablesService::remove(const std::string &uuid) {
	auto &state = LuaEngine::getInstance().getLuaState();
	auto gvTable = state[sol::create_if_nil]["ld"]["data"]["globalVariables"].
		get_or_create<sol::table>();
	auto f = -1;
	for (int i = 1; i <= getSize(); ++i) {
		if (gvTable[i]["uuid"].get<std::string>() == uuid) {
			f = i;
			break;
		}
	}
	if (f != -1) {
		if (const auto functionResult = state["table"]["remove"](gvTable, f); !functionResult.valid()) {
			LuaEngine::errorHandler(functionResult);
		}
	}
}

void GlobalVariablesService::remove(int index) {
	auto &state = LuaEngine::getInstance().getLuaState();
	auto gvTable = getGlobalVariablesTable();
	if (gvTable[index + 1].get_type() == sol::type::none) {
		return;
	}
	if (const auto functionResult = state["table"]["remove"](gvTable, index + 1); !functionResult.valid()) {
		LuaEngine::errorHandler(functionResult);
	}
}

int GlobalVariablesService::getSize() {
	const auto gvTable = getGlobalVariablesTable();
	return static_cast<int>(gvTable.size());
}

std::pair<int, GlobalVariableView> GlobalVariablesService::get(int index) {
	auto gvTable = getGlobalVariablesTable();
	if (gvTable[index + 1].get_type() == sol::type::none) {
		return std::make_pair(1, GlobalVariableView{});
	}
	auto t = gvTable[index + 1];
	GlobalVariableView gv{};
	gv.name = t["name"];
	gv.uuid = t["uuid"];
	gv.value = GlobalVariableView::Value(static_cast<GlobalVariableType>(t["type"].get<int>()));
	switch (gv.value.getType()) {
		case GlobalVariableType::Float: {
			*static_cast<float*>(gv.value.value) = t["value"].get<float>();
			break;
		}
		case GlobalVariableType::Int: {
			*static_cast<int*>(gv.value.value) = t["value"].get<int>();
			break;
		}
		case GlobalVariableType::String: {
			*static_cast<std::string*>(gv.value.value) = t["value"].get<std::string>();
			break;
		}
		case GlobalVariableType::Bool: {
			*static_cast<bool*>(gv.value.value) = t["value"].get<bool>();
			break;
		}
		default:
			break;
	}
	return std::make_pair(0, std::move(gv));
}

std::pair<int, GlobalVariableView> GlobalVariablesService::get(const std::string &uuid) {
	auto gvTable = getGlobalVariablesTable();
	for (int i = 1; i <= getSize(); ++i) {
		if (auto var = gvTable[i].get<sol::table>(); var["uuid"].get<std::string>() == uuid) {
			GlobalVariableView gv{};
			gv.name = var["name"];
			gv.uuid = var["uuid"];
			gv.value = GlobalVariableView::Value(static_cast<GlobalVariableType>(var["type"].get<int>()));
			switch (gv.value.getType()) {
				case GlobalVariableType::Float: {
					*static_cast<float*>(gv.value.value) = var["value"].get<float>();
					break;
				}
				case GlobalVariableType::Int: {
					*static_cast<int*>(gv.value.value) = var["value"].get<int>();
					break;
				}
				case GlobalVariableType::String: {
					*static_cast<std::string*>(gv.value.value) = var["value"].get<std::string>();
					break;
				}
				case GlobalVariableType::Bool: {
					*static_cast<bool*>(gv.value.value) = var["value"].get<bool>();
					break;
				}
				default:
					break;
			}
			return std::make_pair(0, std::move(gv));
		}
	}
	return std::make_pair(1, GlobalVariableView{});
}

void GlobalVariablesService::set(const int index, GlobalVariableView &value) {
	auto gvTable = getGlobalVariablesTable();
	auto t = gvTable[index + 1];
	t["name"] = value.name;
	t["type"] = static_cast<int>(value.value.getType());
	switch (value.value.getType()) {
		case GlobalVariableType::Float: {
			t["value"] = *static_cast<float*>(value.value.value);
			break;
		}
		case GlobalVariableType::Int: {
			t["value"] = *static_cast<int*>(value.value.value);
			break;
		}
		case GlobalVariableType::String: {
			t["value"] = *static_cast<std::string*>(value.value.value);
			break;
		}
		case GlobalVariableType::Bool: {
			t["value"] = *static_cast<bool*>(value.value.value);
			break;
		}
		default:
			break;
	}
}

std::string GlobalVariablesService::toJson() {
	auto gvTable = getGlobalVariablesTable();
	const auto result = LuaEngine::getInstance().getLuaState()["json"]["encode"](gvTable);
	if (!result.valid()) {
		LuaEngine::errorHandler(result);
		return "";
	}
	return result.get<std::string>();
}

void GlobalVariablesService::fromJson(const std::string &json) {
	auto ld = LuaEngine::getInstance().getLuaState()["ld"].get_or_create<sol::table>();
	auto data = ld["data"].get_or_create<sol::table>();
	const auto result = LuaEngine::getInstance().getLuaState()["json"]["decode"](json);
	if (!result.valid()) {
		LuaEngine::errorHandler(result);
		return;
	}
	data["globalVariables"] = result.get<sol::table>();
}
