#include "pin_semantic_registry.h"

PinSemanticRegistry& PinSemanticRegistry::instance() {
    static PinSemanticRegistry inst;
    return inst;
}

void PinSemanticRegistry::registerPin(ImFlow::Pin* pin, const std::string& semanticType) {
    std::lock_guard lock(m_mutex);
    m_map[pin] = semanticType;
}

std::string PinSemanticRegistry::getType(ImFlow::Pin* pin) const {
    std::lock_guard lock(m_mutex);
    auto it = m_map.find(pin);
    if (it == m_map.end()) return {};
    return it->second;
}

void PinSemanticRegistry::unregisterPin(ImFlow::Pin* pin) {
    std::lock_guard lock(m_mutex);
    m_map.erase(pin);
}

bool PinSemanticRegistry::sameType(ImFlow::Pin* out, ImFlow::Pin* in) {
    if (!out || !in) return false;
    auto& reg = instance();
    std::string outType = reg.getType(out);
    std::string inType  = reg.getType(in);
    if (outType.empty() || inType.empty()) return false;
    return outType == inType;
}
