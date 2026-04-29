#pragma once

// Canonical settings paths used by the onboarding-flow change:
//   onboarding.completed        – written once when the onboarding wizard finishes
//   hints.shown.<key>           – written by markShown(); read by shouldShow()
//   hint.example.<id>           – demo key namespace used as a reference pattern

#include <string_view>

class Hints {
  public:
    static Hints& instance();

    Hints(const Hints&) = delete;
    Hints& operator=(const Hints&) = delete;
    Hints(Hints&&) = delete;
    Hints& operator=(Hints&&) = delete;

    bool shouldShow(std::string_view key) const;
    void markShown(std::string_view key);

  private:
    Hints() = default;
};
