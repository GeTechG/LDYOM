#include "carrec_path.h"
#include <logger.h>

CarrecPath::CarrecPath()
	: m_id(uuids::uuid_system_generator{}()),
	  m_name("Unnamed Path"),
	  m_vehicleModel(0) {}

CarrecPath::CarrecPath(const std::string& name, int vehicleModel)
	: m_id(uuids::uuid_system_generator{}()),
	  m_name(name),
	  m_vehicleModel(vehicleModel) {}

void CarrecPath::addFrame(const CVehicleStateEachFrame& frame) { m_frames.push_back(frame); }

void CarrecPath::clearFrames() { m_frames.clear(); }

size_t CarrecPath::getFrameCount() const { return m_frames.size(); }

float CarrecPath::getDuration() const {
	if (m_frames.empty()) {
		return 0.0f;
	}
	// Duration is the difference between last and first frame timestamps
	UINT32 startTime = m_frames.front().m_nTime;
	UINT32 endTime = m_frames.back().m_nTime;
	return static_cast<float>(endTime - startTime) / 1000.0f; // Convert ms to seconds
}

nlohmann::json CarrecPath::to_json() const {
	nlohmann::json j;
	j["uuid"] = uuids::to_string(m_id);
	j["name"] = m_name;
	j["vehicleModel"] = m_vehicleModel;
	j["frames"] = m_frames;
	return j;
}

void CarrecPath::from_json(const nlohmann::json& j, CarrecPath& path) {
	try {
		std::string uuid_str = j.at("uuid").get<std::string>();
		auto uuid_opt = uuids::uuid::from_string(uuid_str);
		if (uuid_opt.has_value()) {
			path.m_id = uuid_opt.value();
		} else {
			LDYOM_WARN("Invalid UUID in CarrecPath JSON, generating new one");
		}

		path.m_name = j.at("name").get<std::string>();
		path.m_vehicleModel = j.at("vehicleModel").get<int>();
		path.m_frames = j.at("frames").get<std::vector<CVehicleStateEachFrame>>();

		LDYOM_DEBUG("Loaded CarrecPath '{}' with {} frames", path.m_name, path.m_frames.size());
	} catch (const nlohmann::json::exception& e) {
		LDYOM_ERROR("Failed to deserialize CarrecPath: {}", e.what());
		throw;
	}
}
