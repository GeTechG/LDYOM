#include "carrec_path_editor.h"
#include <CCamera.h>
#include <CGame.h>
#include <CPlayerPed.h>
#include <CPools.h>
#include <CStreaming.h>
#include <CTimer.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <logger.h>
#include <utils/carrec_paths_service.h>
#include <utils/manual_editing_session.h>
#include <utils/task_manager.h>
#include <window_manager.h>

using namespace plugin;

// Static member initialization
int CarrecPathEditor::m_type = 0;
int CarrecPathEditor::m_state = 0;
CarrecPath* CarrecPathEditor::m_playerPath = nullptr;
std::vector<CarrecPath*> CarrecPathEditor::m_previewPaths;
std::vector<bool> CarrecPathEditor::m_useAIRerecord;
CarrecPathEditor::CallbackCarrecPath CarrecPathEditor::m_callback = nullptr;

std::vector<CVehicleStateEachFrame> CarrecPathEditor::m_recordedFrames;
std::map<int, std::vector<CVehicleStateEachFrame>> CarrecPathEditor::m_previewRecordedFrames;
unsigned CarrecPathEditor::m_recordingStartTime = 0;
unsigned CarrecPathEditor::m_lastFrameTime = 0;

CVehicle* CarrecPathEditor::m_playerVehicle = nullptr;
std::vector<CVehicle*> CarrecPathEditor::m_previewVehicles;

std::unique_ptr<ManualEditingSession> CarrecPathEditor::m_session = nullptr;

void CarrecPathEditor::render() noexcept {
	// Draw overlay with instructions
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		if (m_state >= 0 && m_state < 3) {
			ImGui::Text("%s %s", ICON_FA_VIDEO, _(fmt::format("carrec_paths.overlay_states.{}", m_state)).c_str());
		}
		ImGui::PopTextWrapPos();
	}
	ImGui::End();
}

ktwait CarrecPathEditor::editByPlayerCarrecPathTask() {
	// Initialize state based on type
	m_state = (m_type == 0) ? 0 : 2;

	// Restore camera
	TheCamera.Restore();

	// Spawn player vehicle
	int vehicleModel = m_playerPath->getVehicleModel();
	CStreaming::RequestModel(vehicleModel, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);

	CVector playerPos = FindPlayerPed()->GetPosition();
	m_playerVehicle = nullptr;
	Command<Commands::CREATE_CAR>(vehicleModel, playerPos.x, playerPos.y, playerPos.z, &m_playerVehicle);
	Command<Commands::SET_VEHICLE_AREA_VISIBLE>(m_playerVehicle, CGame::currArea);
	Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(static_cast<CPed*>(FindPlayerPed()), m_playerVehicle);

	// Spawn preview vehicles
	for (auto* previewPath : m_previewPaths) {
		int previewModel = previewPath->getVehicleModel();
		CStreaming::RequestModel(previewModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		CVehicle* previewVehicle = nullptr;
		Command<Commands::CREATE_CAR>(previewModel, playerPos.x + 5.0f, playerPos.y, playerPos.z, &previewVehicle);

		// Position at first frame
		auto& frames = previewPath->getFrames();
		if (!frames.empty()) {
			auto& firstFrame = frames[0];
			previewVehicle->SetPosn(firstFrame.m_vecPosn);
			previewVehicle->GetMatrix()->right = firstFrame.m_bRight;
			previewVehicle->GetMatrix()->up = firstFrame.m_bTop;
			CVector at;
			at.Cross(firstFrame.m_bRight, firstFrame.m_bTop);
			previewVehicle->GetMatrix()->at = at;
			previewVehicle->UpdateRwMatrix();
		}

		previewVehicle->m_bUsesCollision = 0;
		Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(previewVehicle, 1);
		int handle;
		Command<Commands::CREATE_RANDOM_CHAR_AS_DRIVER>(previewVehicle, &handle);
		Command<Commands::SET_VEHICLE_AREA_VISIBLE>(previewVehicle, CGame::currArea);
		m_previewVehicles.push_back(previewVehicle);
	}

	co_await 100;

	// State 0: Wait for I key to start recording
	while (m_state == 0) {
		if (ImGui::IsKeyPressed(ImGuiKey_I, false)) {
			m_state = 1;
		}

		if (!Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
		                                       CPools::GetVehicleRef(m_playerVehicle))) {
			m_state = -1;
		}

		co_await 1;
	}

	if (m_type == 1) {
		// Load existing path for re-record
		m_recordedFrames = m_playerPath->getFrames();
		m_previewRecordedFrames.clear();
	} else if (m_type == 0) {
		// Initialize AI re-record storage
		for (size_t i = 0; i < m_previewPaths.size(); ++i) {
			if (m_useAIRerecord[i]) {
				m_previewRecordedFrames[static_cast<int>(i)] = {};
			}
		}
		m_recordedFrames.clear();
	}

	// State 1: Recording
	if (m_state == 1) {
		unsigned totalTime = 0;
		auto lastTime = CTimer::m_snTimeInMilliseconds;

		// Start playback for preview vehicles
		for (size_t i = 0; i < m_previewVehicles.size(); ++i) {
			m_previewVehicles[i]->m_bUsesCollision = 1;
			Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(m_previewVehicles[i], 0);
			CarrecPathsService::startPlaybackRecordedCar(m_previewVehicles[i], m_previewPaths[i]->getFrames(),
			                                             m_useAIRerecord[i], false);
		}

		while (m_state == 1) {
			if (ImGui::IsKeyPressed(ImGuiKey_O, false)) {
				m_state = 2;
			}

			if (!Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
			                                       CPools::GetVehicleRef(m_playerVehicle))) {
				m_state = -1;
			}

			// Record frame every 100ms
			if (CTimer::m_snTimeInMilliseconds - lastTime >= 100) {
				// Record player vehicle
				m_recordedFrames.emplace_back(totalTime, m_playerVehicle->m_vecMoveSpeed, m_playerVehicle->GetRight(),
				                              m_playerVehicle->GetForward(), m_playerVehicle->m_fSteerAngle,
				                              m_playerVehicle->m_fGasPedal, m_playerVehicle->m_fBreakPedal,
				                              static_cast<bool>(m_playerVehicle->m_nVehicleFlags.bIsHandbrakeOn),
				                              m_playerVehicle->GetPosition());

				// Record preview vehicles if AI re-record is enabled
				for (size_t i = 0; i < m_previewVehicles.size(); ++i) {
					if (m_useAIRerecord[i]) {
						CVehicle* previewVehicle = m_previewVehicles[i];
						m_previewRecordedFrames[static_cast<int>(i)].emplace_back(
							totalTime, previewVehicle->m_vecMoveSpeed, previewVehicle->GetRight(),
							previewVehicle->GetForward(), previewVehicle->m_fSteerAngle, previewVehicle->m_fGasPedal,
							previewVehicle->m_fBreakPedal,
							static_cast<bool>(previewVehicle->m_nVehicleFlags.bIsHandbrakeOn),
							previewVehicle->GetPosition());
					}
				}

				totalTime += CTimer::m_snTimeInMilliseconds - lastTime;
				lastTime = CTimer::m_snTimeInMilliseconds;
			}

			co_await 1;
		}
	}

	// State 2: Preview
	if (m_state == 2 && !m_recordedFrames.empty()) {
		// Start playback for player vehicle
		CarrecPathsService::startPlaybackRecordedCar(m_playerVehicle, m_recordedFrames, false, false);

		// Start playback for preview vehicles
		for (size_t i = 0; i < m_previewVehicles.size(); ++i) {
			if (m_useAIRerecord[i]) {
				CarrecPathsService::stopPlaybackRecordedCar(m_previewVehicles[i]);
				CarrecPathsService::startPlaybackRecordedCar(
					m_previewVehicles[i], m_previewRecordedFrames[static_cast<int>(i)], false, false);
			} else {
				CarrecPathsService::startPlaybackRecordedCar(m_previewVehicles[i], m_previewPaths[i]->getFrames(),
				                                             false, false);
			}
		}

		while (m_state == 2) {
			// P to accept
			if (ImGui::IsKeyPressed(ImGuiKey_P, false)) {
				m_state = 3;
			}

			// O to replay
			if (ImGui::IsKeyPressed(ImGuiKey_O, false)) {
				// Stop all playback
				CarrecPathsService::stopPlaybackRecordedCar(m_playerVehicle);
				for (auto* previewVehicle : m_previewVehicles) {
					CarrecPathsService::stopPlaybackRecordedCar(previewVehicle);
				}

				// Restart playback
				CarrecPathsService::startPlaybackRecordedCar(m_playerVehicle, m_recordedFrames, false, false);
				for (size_t i = 0; i < m_previewVehicles.size(); ++i) {
					if (m_useAIRerecord[i]) {
						CarrecPathsService::startPlaybackRecordedCar(
							m_previewVehicles[i], m_previewRecordedFrames[static_cast<int>(i)], false, false);
					} else {
						CarrecPathsService::startPlaybackRecordedCar(m_previewVehicles[i],
						                                             m_previewPaths[i]->getFrames(), false, false);
					}
				}
			}

			if (!Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
			                                       CPools::GetVehicleRef(m_playerVehicle))) {
				m_state = -1;
			}

			co_await 1;
		}

		// Stop all playback
		CarrecPathsService::stopPlaybackRecordedCar(m_playerVehicle);
		for (auto* previewVehicle : m_previewVehicles) {
			CarrecPathsService::stopPlaybackRecordedCar(previewVehicle);
		}
	}

	// Make player exit vehicle
	if (Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()), CPools::GetVehicleRef(m_playerVehicle))) {
		Command<Commands::TASK_LEAVE_CAR>(static_cast<CPed*>(FindPlayerPed()), m_playerVehicle);
	}
	while (
		Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()), CPools::GetVehicleRef(m_playerVehicle))) {
		co_await 1;
	}

	// Trigger closeEditor with the final state
	closeEditor(m_state != -1);

	// Remove this task
	TaskManager::instance().removeTask("editByPlayerCarrecPathTask");
}

void CarrecPathEditor::openEditor(int type, CarrecPath& playerPath, const std::vector<CarrecPath*>& previewPaths,
                                  const std::vector<bool>& useAIRerecord, CallbackCarrecPath callback) noexcept {
	// Store parameters
	m_type = type;
	m_playerPath = &playerPath;
	m_previewPaths = previewPaths;
	m_useAIRerecord = useAIRerecord;
	m_callback = callback;

	// Create RAII session
	m_session = std::make_unique<ManualEditingSession>(ManualEditingSession::Options{.entity = nullptr,
	                                                                                 .disableUI = true,
	                                                                                 .disableCamera = true,
	                                                                                 .showInfoPanel = false,
	                                                                                 .onComplete = nullptr});

	// Enable player control
	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	// Register render callback
	WindowManager::instance().addBackgroundRenderCallback("CarrecPathEditor", render);

	// Start coroutine task
	TaskManager::instance().addTask("editByPlayerCarrecPathTask", editByPlayerCarrecPathTask);

	LDYOM_INFO("Opened carrec path editor (type: {})", type);
}

void CarrecPathEditor::closeEditor(bool accept) noexcept {
	if (!m_session) {
		return;
	}

	bool wasAccepted = accept && (m_state == 3);

	LDYOM_INFO("Closing carrec path editor (accepted: {})", wasAccepted);

	// Delete vehicles
	if (m_playerVehicle) {
		Command<Commands::DELETE_CAR>(m_playerVehicle);
		m_playerVehicle = nullptr;
	}
	for (auto* previewVehicle : m_previewVehicles) {
		if (previewVehicle) {
			Command<Commands::DELETE_CAR>(previewVehicle);
		}
	}
	m_previewVehicles.clear();

	// Call callback with results
	if (m_callback) {
		m_callback(wasAccepted, m_recordedFrames, m_previewRecordedFrames);
		m_callback = nullptr;
	}

	// Remove render callback
	WindowManager::instance().removeBackgroundRenderCallback("CarrecPathEditor");

	// Destroy session (RAII cleanup)
	m_session.reset();

	// Disable player control
	Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	// Reset state
	m_playerPath = nullptr;
	m_previewPaths.clear();
	m_useAIRerecord.clear();
	m_recordedFrames.clear();
	m_previewRecordedFrames.clear();
}
