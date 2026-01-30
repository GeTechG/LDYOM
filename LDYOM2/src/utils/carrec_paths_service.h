#pragma once
#include "data/CVehicleRecording.h"
#include <data/carrec_path.h>
#include <uuid_wrap.h>

class CarrecPathsService {
  public:
	CarrecPathsService(const CarrecPathsService&) = delete;
	CarrecPathsService& operator=(const CarrecPathsService&) = delete;
	~CarrecPathsService() = default;

	static CarrecPathsService& instance() {
		static CarrecPathsService instance{};
		return instance;
	}

  private:
	std::vector<CarrecPath> paths;

	explicit CarrecPathsService() = default;

  public:
	void savePaths() const;
	void loadPaths();

	CarrecPath* createNewPath();
	std::vector<CarrecPath>& getPaths();

	bool pathNameExists(const std::string& name) const;
	void deletePath(const uuids::uuid& id);

	CarrecPath* getPathByUuidString(const std::string& uuidStr);

	static void startPlaybackRecordedCar(CVehicle* vehicle, std::vector<CVehicleStateEachFrame>& frames, bool useCarAI,
	                                     bool looped);
	static void stopPlaybackRecordedCar(CVehicle* vehicle);
	static bool isPlaybackActive(CVehicle* vehicle);
};
