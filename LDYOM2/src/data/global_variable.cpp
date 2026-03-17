#include "global_variable.h"

static VarValue makeDefault(GlobalVarType t) {
    switch (t) {
        case GlobalVarType::Number:  return 0.0;
        case GlobalVarType::String:  return std::string{};
        case GlobalVarType::Boolean: return false;
    }
    return 0.0;
}

void to_json(nlohmann::json& j, const GlobalVariable& v) {
    j["name"] = v.name;
    j["type"] = v.type();
    std::visit([&](const auto& val) { j["defaultValue"] = val; }, v.defaultValue);
}

void from_json(const nlohmann::json& j, GlobalVariable& v) {
    v.name       = j.value("name", "");
    auto t       = j.value("type", GlobalVarType::Number);
    if (!j.contains("defaultValue")) {
        v.defaultValue = makeDefault(t);
        return;
    }
    const auto& jv = j["defaultValue"];
    switch (t) {
        case GlobalVarType::Number:  v.defaultValue = jv.get<double>();      break;
        case GlobalVarType::String:  v.defaultValue = jv.get<std::string>(); break;
        case GlobalVarType::Boolean: v.defaultValue = jv.get<bool>();        break;
    }
}
