#pragma once
#include <vector>
#include <boost/uuid/random_generator.hpp>

#include "Actor.h"

#include "Audio.h"
#include "BaseObjective.h"
#include "Checkpoint.h"
#include "CheckpointObjective.h"
#include "Object.h"
#include "Particle.h"
#include "Pickup.h"
#include "Pyrotechnics.h"
#include "SceneSettings.h"
#include "Train.h"
#include "Vehicle.h"
#include "VisualEffect.h"


struct SceneSettings;

class Scene final : public INameable {
	std::string name;
	int id_ = 0;

	std::vector<std::unique_ptr<BaseObjective>> objectives_;
	std::vector<std::unique_ptr<Actor>> actors_;
	std::vector<std::unique_ptr<Vehicle>> vehicles_;
	std::vector<std::unique_ptr<Object>> objects_;
	std::vector<std::unique_ptr<Particle>> particles_;
	std::vector<std::unique_ptr<Train>> trains_;
	std::vector<std::unique_ptr<Pickup>> pickups_;
	std::vector<std::unique_ptr<Pyrotechnics>> pyrotechnics_;
	std::vector<std::unique_ptr<Audio>> audio_;
	std::vector<std::unique_ptr<VisualEffect>> visualEffects_;
	std::vector<std::unique_ptr<Checkpoint>> checkpoints_;

	SceneSettings sceneSettings_;
	bool toggleSceneSettings_ = true;

	void initGroupRelations();

public:
	Scene();
	Scene(const char *name);

	Scene(std::string name, int id, std::vector<std::unique_ptr<BaseObjective>> objectives,
	      std::vector<std::unique_ptr<Actor>> actors, std::vector<std::unique_ptr<Vehicle>> vehicles,
	      std::vector<std::unique_ptr<Object>> objects, std::vector<std::unique_ptr<Particle>> particles,
	      std::vector<std::unique_ptr<Train>> trains, std::vector<std::unique_ptr<Pickup>> pickups,
	      std::vector<std::unique_ptr<Pyrotechnics>> pyrotechnics, std::vector<std::unique_ptr<Audio>> audio,
	      std::vector<std::unique_ptr<VisualEffect>> visualEffects,
	      std::vector<std::unique_ptr<Checkpoint>> checkpoints,
	      SceneSettings sceneSettings, bool toggleSceneSettings)
		: name(std::move(name)),
		  id_(id),
		  objectives_(std::move(objectives)),
		  actors_(std::move(actors)),
		  vehicles_(std::move(vehicles)),
		  objects_(std::move(objects)),
		  particles_(std::move(particles)),
		  trains_(std::move(trains)),
		  pickups_(std::move(pickups)),
		  pyrotechnics_(std::move(pyrotechnics)),
		  audio_(std::move(audio)),
		  visualEffects_(std::move(visualEffects)),
		  checkpoints_(std::move(checkpoints)),
		  sceneSettings_(std::move(sceneSettings)),
		  toggleSceneSettings_(toggleSceneSettings) {}

	Scene& operator=(Scene &&other) noexcept;

	std::string& getName() override;
	int& getId();

	~Scene() override = default;

	std::vector<std::unique_ptr<BaseObjective>>& getObjectives();
	std::vector<std::unique_ptr<Actor>>& getActors();
	std::vector<std::unique_ptr<Vehicle>>& getVehicles();
	std::vector<std::unique_ptr<Object>>& getObjects();
	std::vector<std::unique_ptr<Particle>>& getParticles();
	std::vector<std::unique_ptr<Train>>& getTrains();
	std::vector<std::unique_ptr<Pickup>>& getPickups();
	std::vector<std::unique_ptr<Pyrotechnics>>& getPyrotechnics();
	std::vector<std::unique_ptr<Audio>>& getAudio();
	std::vector<std::unique_ptr<VisualEffect>>& getVisualEffects();
	std::vector<std::unique_ptr<Checkpoint>>& getCheckpoints();

	SceneSettings& getSceneSettings();
	bool& isToggleSceneSettings();

	template <class _Ty, class... _Types>
	void createNewObjectives(_Types &&... _Args);
	void createNewActor();
	void createNewVehicle();
	void createNewObject();
	void createNewParticle();
	void createNewTrain();
	void createNewPickup();
	void createNewPyrotechnics();
	void createNewAudio();
	void createNewVisualEffect();
	void createNewCheckpoint();

	template <typename T,
	          typename = std::enable_if_t<std::is_base_of_v<BaseObjective, T>>>
	void createNewObjectiveFrom(T &objective);
	void createNewActorFrom(Actor &actor);
	void createNewVehicleFrom(Vehicle &vehicle);
	void createNewObjectFrom(Object &object);
	void createNewParticleFrom(Particle &particle);
	void createNewTrainFrom(Train &train);
	void createNewPickupFrom(Pickup &pickup);
	void createNewPyrotechnicsFrom(Pyrotechnics &pyrotechnics);
	void createNewAudioFrom(Audio &a);
	void createNewVisualEffectFrom(VisualEffect &visualEffect);
	void createNewCheckpointFrom(Checkpoint &checkpoint);

	void unloadEditorScene() const;
	void unloadProjectScene() const;
	void loadEditorScene() const;
};

template <class _Ty, class... _Types>
void Scene::createNewObjectives(_Types &&... _Args) {
	this->objectives_.emplace_back(std::make_unique<_Ty>(_STD forward<_Types>(_Args)...));
}

template <typename T, typename>
void Scene::createNewObjectiveFrom(T &objective) {
	auto unique = std::make_unique<T>(objective);
	unique.get()->getUuid() = boost::uuids::random_generator()();
	unique.get()->getName() = fmt::format("{}_copy", unique.get()->getName());
	this->objectives_.emplace_back(std::move(unique));
}
