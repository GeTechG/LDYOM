#pragma once

#include <array>
#include <functional>
#include <vector>

typedef std::vector<std::array<float, 3>> PointsArray;

class ActorPathsEditing {
  private:
	static PointsArray m_points;
	static std::function<void(bool, const PointsArray&)> m_onCloseCallback;
	static size_t m_currentPointIndex;
	static bool giveJetpack;

	static void render() noexcept;

  public:
	static void addJetpack() noexcept { giveJetpack = true; }
	static void openPathEditor(const PointsArray& points,
	                           std::function<void(bool, const PointsArray&)> onClose) noexcept;
	static void closePathEditor(bool saveChanges) noexcept;
};