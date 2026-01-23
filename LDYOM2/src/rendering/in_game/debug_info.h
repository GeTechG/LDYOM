#pragma once

class DebugInfo {
  private:
	static bool m_isVisible;
	static void render() noexcept;

  public:
	static void toggle() noexcept;
	static bool isVisible() noexcept { return m_isVisible; }
};
