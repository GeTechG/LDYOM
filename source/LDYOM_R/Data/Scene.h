#pragma once
#include "Actor.h"
#include <vector>
#include <boost/serialization/access.hpp>

#include "BaseObjective.h"
#include "CheckpointObjective.h"
#include "Object.h"
#include "Vehicle.h"

class Scene final : public INameable {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & this->name_;
		ar & this->actors_;
		ar & this->vehicles_;

		ar.template register_type<CheckpointObjective>();

		ar & this->objectives_;
	}

	char name_[NAME_SIZE]{};

	std::vector<std::unique_ptr<BaseObjective>> objectives_;
	std::vector<std::unique_ptr<Actor>> actors_;
	std::vector<std::unique_ptr<Vehicle>> vehicles_;
	std::vector<std::unique_ptr<Object>> objects_;
public:
	Scene() = default;
	Scene(const char* name);

	Scene& operator=(Scene&& other) noexcept;

	char* getName() override;

	~Scene() override = default;

	std::vector<std::unique_ptr<BaseObjective>>& getObjectives();
	std::vector<std::unique_ptr<Actor>>& getActors();
	std::vector<std::unique_ptr<Vehicle>>& getVehicles();
	std::vector<std::unique_ptr<Object>>& getObjects();

	template <class _Ty, class... _Types>
	void createNewObjectives(_Types&&... _Args);
	void createNewActor();
	void createNewVehicle();
	void createNewObject();

	template<typename T>
	void createNewObjectiveFrom(T& objective);
	void createNewActorFrom(Actor& actor);
	void createNewVehicleFrom(Vehicle& vehicle);
	void createNewObjectFrom(Object& object);

	void unloadEditorScene() const;
	void unloadProjectScene() const;
	void loadEditorScene() const;
};

template <class _Ty, class... _Types>
void Scene::createNewObjectives(_Types&&... _Args) {
	this->objectives_.emplace_back(std::make_unique<_Ty>(_STD forward<_Types>(_Args)...));
}

template <typename T>
void Scene::createNewObjectiveFrom(T& objective) {
	this->objectives_.emplace_back(std::make_unique<T>(objective));
}
