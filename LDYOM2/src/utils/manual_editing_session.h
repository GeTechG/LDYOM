#pragma once

#include <functional>

class Entity;

/**
 * @brief RAII-based session manager for manual editing modes
 *
 * Automatically handles:
 * - UI rendering disable/enable
 * - EntityOrbitCamera deactivation/activation
 * - Player controls management
 * - Callback invocation on completion
 *
 * Usage:
 * @code
 * ManualEditingSession session({
 *     .entity = this->entity,
 *     .disableUI = true,
 *     .disableCamera = true,
 *     .onComplete = []() { // Restore state }
 * });
 * // ... do manual editing ...
 * session.complete(); // Call when done, or let destructor auto-complete
 * @endcode
 */
class ManualEditingSession {
  public:
	struct Options {
		Entity* entity = nullptr;           // Entity being edited (for camera restoration)
		bool disableUI = true;              // Disable WindowManager rendering
		bool disableCamera = true;          // Deactivate EntityOrbitCamera
		bool showInfoPanel = true;          // Show entity info panel after completion
		std::function<void()> onComplete = nullptr; // Callback on completion
	};

  private:
	Options m_options;
	bool m_completed = false;

  public:
	/**
	 * @brief Start manual editing session
	 * @param options Configuration options
	 */
	explicit ManualEditingSession(const Options& options) noexcept;

	/**
	 * @brief Destructor - automatically completes session if not already completed
	 */
	~ManualEditingSession() noexcept;

	/**
	 * @brief Manually complete the session
	 * Safe to call multiple times - subsequent calls are no-op
	 */
	void complete() noexcept;

	// Disable copying
	ManualEditingSession(const ManualEditingSession&) = delete;
	ManualEditingSession& operator=(const ManualEditingSession&) = delete;

	// Enable moving
	ManualEditingSession(ManualEditingSession&&) noexcept = default;
	ManualEditingSession& operator=(ManualEditingSession&&) noexcept = default;
};
