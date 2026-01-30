#pragma once

#include "vehicle_state_each_frame.h"
#include <nlohmann/json.hpp>
#include <string>
#include <uuid_wrap.h>
#include <vector>

/**
 * Vehicle recording path (CarRec Path)
 * Contains a sequence of vehicle states captured during recording
 * Can be played back on any vehicle entity
 */
class CarrecPath {
  public:
	CarrecPath();
	explicit CarrecPath(const std::string& name, int vehicleModel = 0);

	// Accessors
	const uuids::uuid& getId() const { return m_id; }
	const std::string& getName() const { return m_name; }
	void setName(const std::string& name) { m_name = name; }

	int getVehicleModel() const { return m_vehicleModel; }
	void setVehicleModel(int model) { m_vehicleModel = model; }

	const std::vector<CVehicleStateEachFrame>& getFrames() const { return m_frames; }
	std::vector<CVehicleStateEachFrame>& getFrames() { return m_frames; }

	// Frame management
	void addFrame(const CVehicleStateEachFrame& frame);
	void clearFrames();
	size_t getFrameCount() const;
	float getDuration() const; // Duration in seconds

	// Serialization
	nlohmann::json to_json() const;
	static void from_json(const nlohmann::json& j, CarrecPath& path);

  private:
	uuids::uuid m_id;
	std::string m_name;
	int m_vehicleModel;                           // Reference vehicle model ID
	std::vector<CVehicleStateEachFrame> m_frames; // Recorded frames
};
