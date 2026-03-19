#pragma once

#include <ImNodeFlow.h>
#include <mutex>
#include <string>
#include <unordered_map>

// Maps Pin* -> semantic type string ("flow", "number", "bool", "string")
// Uses Pin pointer as key (not PinUID) to avoid hash collisions when multiple
// nodes share pins with the same name.
class PinSemanticRegistry {
  public:
    static PinSemanticRegistry& instance();

    PinSemanticRegistry(const PinSemanticRegistry&) = delete;
    PinSemanticRegistry& operator=(const PinSemanticRegistry&) = delete;

    void registerPin(ImFlow::Pin* pin, const std::string& semanticType);
    std::string getType(ImFlow::Pin* pin) const;
    void unregisterPin(ImFlow::Pin* pin);

    // ConnectionFilter: returns true if out and in pins have the same semantic type
    static bool sameType(ImFlow::Pin* out, ImFlow::Pin* in);

  private:
    PinSemanticRegistry() = default;

    mutable std::mutex m_mutex;
    std::unordered_map<ImFlow::Pin*, std::string> m_map;
};
