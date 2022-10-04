---@meta
---@class Scene
Scene = {
    ---operator=(other: Scene) -> Scene
    ---@return Scene
    ["operator="] = function(self, other) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---~Scene() -> void
    ---@return void
    ["~Scene"] = function(self) end,
	---getObjectives() -> std::vector<std::unique_ptr<BaseObjective>>
    ---@return std::vector<std::unique_ptr<BaseObjective>>
    ["getObjectives"] = function(self) end,
	---getActors() -> std::vector<std::unique_ptr<Actor>>
    ---@return std::vector<std::unique_ptr<Actor>>
    ["getActors"] = function(self) end,
	---getVehicles() -> std::vector<std::unique_ptr<Vehicle>>
    ---@return std::vector<std::unique_ptr<Vehicle>>
    ["getVehicles"] = function(self) end,
	---getObjects() -> std::vector<std::unique_ptr<Object>>
    ---@return std::vector<std::unique_ptr<Object>>
    ["getObjects"] = function(self) end,
	---getParticles() -> std::vector<std::unique_ptr<Particle>>
    ---@return std::vector<std::unique_ptr<Particle>>
    ["getParticles"] = function(self) end,
	---getTrains() -> std::vector<std::unique_ptr<Train>>
    ---@return std::vector<std::unique_ptr<Train>>
    ["getTrains"] = function(self) end,
	---getPickups() -> std::vector<std::unique_ptr<Pickup>>
    ---@return std::vector<std::unique_ptr<Pickup>>
    ["getPickups"] = function(self) end,
	---getPyrotechnics() -> std::vector<std::unique_ptr<Pyrotechnics>>
    ---@return std::vector<std::unique_ptr<Pyrotechnics>>
    ["getPyrotechnics"] = function(self) end,
	---getAudio() -> std::vector<std::unique_ptr<Audio>>
    ---@return std::vector<std::unique_ptr<Audio>>
    ["getAudio"] = function(self) end,
	---getVisualEffects() -> std::vector<std::unique_ptr<VisualEffect>>
    ---@return std::vector<std::unique_ptr<VisualEffect>>
    ["getVisualEffects"] = function(self) end,
	---getCheckpoints() -> std::vector<std::unique_ptr<Checkpoint>>
    ---@return std::vector<std::unique_ptr<Checkpoint>>
    ["getCheckpoints"] = function(self) end,
	---getSceneSettings() -> SceneSettings
    ---@return SceneSettings
    ["getSceneSettings"] = function(self) end,
	---isToggleSceneSettings() -> bool
    ---@return bool
    ["isToggleSceneSettings"] = function(self) end,
	---createNewObjectives(_Args: _Types) -> void
    ---@return void
    ["createNewObjectives"] = function(self, _Args) end,
	---createNewActor() -> void
    ---@return void
    ["createNewActor"] = function(self) end,
	---createNewVehicle() -> void
    ---@return void
    ["createNewVehicle"] = function(self) end,
	---createNewObject() -> void
    ---@return void
    ["createNewObject"] = function(self) end,
	---createNewParticle() -> void
    ---@return void
    ["createNewParticle"] = function(self) end,
	---createNewTrain() -> void
    ---@return void
    ["createNewTrain"] = function(self) end,
	---createNewPickup() -> void
    ---@return void
    ["createNewPickup"] = function(self) end,
	---createNewPyrotechnics() -> void
    ---@return void
    ["createNewPyrotechnics"] = function(self) end,
	---createNewAudio() -> void
    ---@return void
    ["createNewAudio"] = function(self) end,
	---createNewVisualEffect() -> void
    ---@return void
    ["createNewVisualEffect"] = function(self) end,
	---createNewCheckpoint() -> void
    ---@return void
    ["createNewCheckpoint"] = function(self) end,
	---createNewObjectiveFrom(objective: T) -> void
    ---@return void
    ["createNewObjectiveFrom"] = function(self, objective) end,
	---createNewActorFrom(actor: Actor) -> void
    ---@return void
    ["createNewActorFrom"] = function(self, actor) end,
	---createNewVehicleFrom(vehicle: Vehicle) -> void
    ---@return void
    ["createNewVehicleFrom"] = function(self, vehicle) end,
	---createNewObjectFrom(object: Object) -> void
    ---@return void
    ["createNewObjectFrom"] = function(self, object) end,
	---createNewParticleFrom(particle: Particle) -> void
    ---@return void
    ["createNewParticleFrom"] = function(self, particle) end,
	---createNewTrainFrom(train: Train) -> void
    ---@return void
    ["createNewTrainFrom"] = function(self, train) end,
	---createNewPickupFrom(pickup: Pickup) -> void
    ---@return void
    ["createNewPickupFrom"] = function(self, pickup) end,
	---createNewPyrotechnicsFrom(pyrotechnics: Pyrotechnics) -> void
    ---@return void
    ["createNewPyrotechnicsFrom"] = function(self, pyrotechnics) end,
	---createNewAudioFrom(a: Audio) -> void
    ---@return void
    ["createNewAudioFrom"] = function(self, a) end,
	---createNewVisualEffectFrom(visualEffect: VisualEffect) -> void
    ---@return void
    ["createNewVisualEffectFrom"] = function(self, visualEffect) end,
	---createNewCheckpointFrom(checkpoint: Checkpoint) -> void
    ---@return void
    ["createNewCheckpointFrom"] = function(self, checkpoint) end,
	---unloadEditorScene() -> void
    ---@return void
    ["unloadEditorScene"] = function(self) end,
	---unloadProjectScene() -> void
    ---@return void
    ["unloadProjectScene"] = function(self) end,
	---loadEditorScene() -> void
    ---@return void
    ["loadEditorScene"] = function(self) end
}

