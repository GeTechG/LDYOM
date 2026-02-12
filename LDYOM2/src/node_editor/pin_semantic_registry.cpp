#include "pin_semantic_registry.h"

PinSemanticRegistry& PinSemanticRegistry::instance() {
    static PinSemanticRegistry inst;
    return inst;
}

void PinSemanticRegistry::registerPin(ImFlow::PinUID uid, const std::string& semanticType) {
    std::lock_guard lock(m_mutex);
    m_map[uid] = semanticType;
}

std::string PinSemanticRegistry::getType(ImFlow::PinUID uid) const {
    std::lock_guard lock(m_mutex);
    auto it = m_map.find(uid);
    if (it == m_map.end()) return {};
    return it->second;
}

void PinSemanticRegistry::unregisterPin(ImFlow::PinUID uid) {
    std::lock_guard lock(m_mutex);
    m_map.erase(uid);
}

bool PinSemanticRegistry::sameType(ImFlow::Pin* out, ImFlow::Pin* in) {
    if (!out || !in) return false;
    auto& reg = instance();
    std::string outType = reg.getType(out->getUid());
    std::string inType  = reg.getType(in->getUid());
    if (outType.empty() || inType.empty()) return false;
    return outType == inType;
}
