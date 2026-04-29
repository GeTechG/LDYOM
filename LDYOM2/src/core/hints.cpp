#include "hints.h"
#include "settings.h"
#include <string>

Hints& Hints::instance() {
    static Hints s_instance;
    return s_instance;
}

bool Hints::shouldShow(std::string_view key) const {
    return !Settings::instance().getSetting<bool>("hints.shown." + std::string(key), false);
}

void Hints::markShown(std::string_view key) {
    Settings::instance().setSetting<bool>("hints.shown." + std::string(key), true);
}
