#pragma once

#include <ImNodeFlow.h>
#include <mutex>
#include <string>
#include <unordered_map>

// Maps PinUID -> semantic type string ("flow", "float", "int", "bool", "string")
// Used by the custom ConnectionFilter to enforce type safety at the semantic level
// rather than at the C++ type level (since all data pins use sol::object).
class PinSemanticRegistry {
  public:
    static PinSemanticRegistry& instance();

    PinSemanticRegistry(const PinSemanticRegistry&) = delete;
    PinSemanticRegistry& operator=(const PinSemanticRegistry&) = delete;

    void registerPin(ImFlow::PinUID uid, const std::string& semanticType);
    std::string getType(ImFlow::PinUID uid) const;
    void unregisterPin(ImFlow::PinUID uid);

    // ConnectionFilter: returns true if out and in pins have the same semantic type
    static bool sameType(ImFlow::Pin* out, ImFlow::Pin* in);

  private:
    PinSemanticRegistry() = default;

    mutable std::mutex m_mutex;
    std::unordered_map<ImFlow::PinUID, std::string> m_map;
};
