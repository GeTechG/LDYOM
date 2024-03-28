#pragma once
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include "../Data/CVehicleRecording.h"
#include "../Data/Vehicle.h"

class CarrecPath : public IUuidable {
private:
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	std::string name;
	std::vector<CVehicleStateEachFrame> path;
	Vehicle vehicle;

public:
	CarrecPath() = default;
	CarrecPath(const CarrecPath &) = default;
	CarrecPath& operator=(const CarrecPath &) = default;
	~CarrecPath() override = default;

	std::vector<CVehicleStateEachFrame>& getPath();
	Vehicle& getVehicle();
	std::string& getName();
	boost::uuids::uuid& getUuid() override;
};

class CarrecPathsService {
public:
	CarrecPathsService(const CarrecPathsService &) = delete;
	CarrecPathsService& operator=(const CarrecPathsService &) = delete;
	~CarrecPathsService() = default;

	static CarrecPathsService& getInstance() {
		static CarrecPathsService instance{};
		return instance;
	}

private:
	std::vector<CarrecPath> paths;

	explicit CarrecPathsService() = default;

public:
	void savePaths(const std::filesystem::path &path) const;
	void loadPaths(const std::filesystem::path &path);

	CarrecPath* createNewPath();
	std::vector<CarrecPath>& getPaths();


	static void startPlaybackRecordedCar(CVehicle *vehicle, std::vector<CVehicleStateEachFrame> &frames, bool useCarAI,
	                                     bool looped);
	static void stopPlaybackRecordedCar(CVehicle *vehicle);
};
