#pragma once

#include <CVehicle.h>
#include <data/carrec_path.h>
#include <data/vehicle_state_each_frame.h>
#include <functional>
#include <ktcoro_wait.hpp>
#include <map>
#include <memory>
#include <vector>

class ManualEditingSession;

/**
 * @brief Interactive editor for vehicle recording paths (CarRec)
 *
 * Provides two modes:
 * - Record mode (type 0): Player drives vehicle while preview paths play back with AI
 * - Re-record mode (type 1): Player drives vehicle to replace existing recording
 */
class CarrecPathEditor {
  public:
	using CallbackCarrecPath = std::function<void(bool isAccepted, std::vector<CVehicleStateEachFrame> recordedPath,
	                                              const std::map<int, std::vector<CVehicleStateEachFrame>>& previewPaths)>;

  private:
	static int m_type;
	static int m_state;
	static CarrecPath* m_playerPath;
	static std::vector<CarrecPath*> m_previewPaths;
	static std::vector<bool> m_useAIRerecord;
	static CallbackCarrecPath m_callback;

	static std::vector<CVehicleStateEachFrame> m_recordedFrames;
	static std::map<int, std::vector<CVehicleStateEachFrame>> m_previewRecordedFrames;
	static unsigned m_recordingStartTime;
	static unsigned m_lastFrameTime;

	static CVehicle* m_playerVehicle;
	static std::vector<CVehicle*> m_previewVehicles;

	static std::unique_ptr<ManualEditingSession> m_session;

	static void render() noexcept;
	static ktwait editByPlayerCarrecPathTask();

  public:
	/**
	 * @brief Open carrec path editor
	 * @param type Editor type (0 = Record, 1 = ReRecord)
	 * @param playerPath Path being edited/created
	 * @param previewPaths Other paths to play simultaneously (for syncing)
	 * @param useAIRerecord Which preview paths should re-record with AI
	 * @param callback Completion callback
	 */
	static void openEditor(int type, CarrecPath& playerPath, const std::vector<CarrecPath*>& previewPaths,
	                       const std::vector<bool>& useAIRerecord, CallbackCarrecPath callback) noexcept;

	/**
	 * @brief Close the editor
	 * @param accept Whether to accept changes or cancel
	 */
	static void closeEditor(bool accept) noexcept;
};
