﻿#include "Scene.h"

#include "strUtils.h"
#include "Localization/Localization.h"
#include "fmt/core.h"

const char* names[200] = { "Douglas", "Aaron", "Brandon", "Dorothy", "Eric", "Carlos", "Shirley", "Carol", "Ralph", "Jeremy", "Lisa", "Kathleen", "Martin", "Teresa", "Donna", "Connie", "Sarah", "Cynthia", "Monica", "Michael", "Bruce", "Craig", "Mildred", "Timothy", "Evelyn", "Nicholas", "Angela", "Mary", "Andrea", "Dennis", "Samuel", "Stephanie", "Alice", "Amber", "Mark", "Rebecca", "Tina", "Joyce", "Laura", "Ernest", "Matthew", "Justin", "Marilyn", "Heather", "Donald", "Maria", "Christopher", "Ruby", "Susan", "Harold", "Victor", "Gerald", "Elizabeth", "Andrew", "Patricia", "Thomas", "Rachel", "Sherry", "Larry", "Brian", "Harry", "Eugene", "Lori", "Joe", "Ronald", "William", "Tiffany", "Randy", "Sylvia", "Frances", "Russell", "Cheryl", "April", "Kelly", "Brenda", "Alan", "Shawn", "Amanda", "Willie", "Karen", "Peter", "Sandra", "Stanley", "Debra", "Ryan", "Joseph", "Earl", "Danny", "Richard", "Norma", "Frank", "Amy", "Terry", "Antonio", "Ashley", "Ruth", "Helen", "Jose", "Kimberly", "Joan", "Ann", "Philip", "Martha", "Lois", "Judith", "Steve", "Tony", "Steven", "Nicole", "Bonnie", "Tracy", "Linda", "Paul", "Stephen", "Kenneth", "Charles", "John", "Jerry", "George", "Lauren", "Gregory", "Jesse", "Louis", "Louise", "Virginia", "Janet", "Gloria", "Jacqueline", "Henry", "Jimmy", "Carl", "Juan", "Johnny", "Jane", "Jennifer", "Walter", "Tammy", "Emma", "Denise", "Chris", "Catherine", "Melissa", "Patrick", "Todd", "Jonathan", "Irene", "Scott", "Grace", "Anthony", "Margaret", "Edward", "Keith", "Arthur", "Christine", "Albert", "Roy", "Wanda", "Bobby", "Doris", "Beverly", "Sharon", "Paula", "Pamela", "Jack", "Kevin", "Jason", "Lillian", "Howard", "Janice", "Benjamin", "Julia", "Daniel", "Robert", "David", "Betty", "Adam", "Lawrence", "Wendy", "Raymond", "Cindy", "Michelle", "Clarence", "Carolyn", "Judy", "Emily", "James", "Wayne", "Billy", "Nancy", "Barbara", "Jean", "Phyllis", "Rose", "Fred", "Jeffrey", "Kathy", "Joshua", "Jessica", "Roger", "Julie" };

Scene::Scene(const char* name) {
	strcpy_s(this->name_, sizeof this->name_, name);
}

Scene& Scene::operator=(Scene&& other) noexcept {
	if (this == &other)
		return *this;
	strlcpy(this->name_, other.name_, sizeof this->name_);
	objectives_ = std::move(other.objectives_);
	actors_ = std::move(other.actors_);
	vehicles_ = std::move(other.vehicles_);
	objects_ = std::move(other.objects_);
	return *this;
}

char* Scene::getName() {
	return this->name_;
}

std::vector<std::unique_ptr<Actor>>& Scene::getActors() {
	return actors_;
}

std::vector<std::unique_ptr<Vehicle>>& Scene::getVehicles() {
	return vehicles_;
}

std::vector<std::unique_ptr<Object>>& Scene::getObjects() {
	return objects_;
}

std::vector<std::unique_ptr<Particle>>& Scene::getParticles() {
	return particles_;
}

std::vector<std::unique_ptr<Train>>& Scene::getTrains() {
	return trains_;
}

std::vector<std::unique_ptr<Pickup>>& Scene::getPickups() {
	return pickups_;
}


std::vector<std::unique_ptr<BaseObjective>>& Scene::getObjectives() {
	return objectives_;
}

void Scene::createNewActor() {
	srand(static_cast<unsigned>(time(nullptr)));
	const auto player = FindPlayerPed();
	const auto defaultName = names[rand() % 200];
	this->actors_.emplace_back(std::make_unique<Actor>(defaultName, player->GetPosition(), player->GetHeading()));
	this->actors_.back()->spawnEditorPed();
	this->actors_.back()->updateLocation();
}

void Scene::createNewVehicle() {
	const auto player = FindPlayerPed();
	const auto defaultName = fmt::format("{} #{}", Localization::getInstance().get("entities.vehicle"), this->vehicles_.size());
	this->vehicles_.emplace_back(std::make_unique<Vehicle>(defaultName.c_str(), player->GetPosition(), player->GetHeading()));
	this->vehicles_.back()->spawnEditorVehicle(true);
	this->vehicles_.back()->updateLocation();
}

void Scene::createNewObject() {
	const auto player = FindPlayerPed();
	const auto defaultName = fmt::format("{} #{}", Localization::getInstance().get("entities.object"), this->objects_.size());
	this->objects_.emplace_back(std::make_unique<Object>(defaultName.c_str(), player->GetPosition()));
	this->objects_.back()->spawnEditorObject();
	this->objects_.back()->updateLocation();
}

void Scene::createNewParticle() {
	const auto player = FindPlayerPed();
	const auto defaultName = fmt::format("{} #{}", Localization::getInstance().get("entities.particle"), this->particles_.size());
	this->particles_.emplace_back(std::make_unique<Particle>(defaultName.c_str(), player->GetPosition()));
	this->particles_.back()->spawnEditorParticle();
	this->particles_.back()->updateLocation();
}

void Scene::createNewTrain() {
	const auto player = FindPlayerPed();
	const auto defaultName = fmt::format("{} #{}", Localization::getInstance().get("entities.train"), this->trains_.size());
	this->trains_.emplace_back(std::make_unique<Train>(defaultName.c_str(), player->GetPosition()));
	this->trains_.back()->spawnEditorTrain();
}

void Scene::createNewPickup() {
	const auto player = FindPlayerPed();
	const auto defaultName = fmt::format("{} #{}", Localization::getInstance().get("entities.pickup"), this->pickups_.size());
	this->pickups_.emplace_back(std::make_unique<Pickup>(defaultName.c_str(), player->GetPosition()));
	this->pickups_.back()->spawnEditorPickup();
}

void Scene::createNewActorFrom(Actor& actor) {
	this->actors_.emplace_back(std::make_unique<Actor>(actor));
}

void Scene::createNewVehicleFrom(Vehicle& vehicle) {
	this->vehicles_.emplace_back(std::make_unique<Vehicle>(vehicle));
}

void Scene::createNewObjectFrom(Object& object) {
	this->objects_.emplace_back(std::make_unique<Object>(object));
}

void Scene::createNewParticleFrom(Particle& particle) {
	this->particles_.emplace_back(std::make_unique<Particle>(particle));
}

void Scene::createNewTrainFrom(Train& train) {
	this->trains_.emplace_back(std::make_unique<Train>(train));
}

void Scene::createNewPickupFrom(Pickup& pickup) {
	this->pickups_.emplace_back(std::make_unique<Pickup>(pickup));
}

void Scene::unloadEditorScene() const {
	for (const auto & actor : this->actors_)
		actor->deleteEditorPed();
	for (const auto& vehicle : this->vehicles_)
		vehicle->deleteEditorVehicle();
	for (const auto& object : this->objects_)
		object->deleteEditorObject();
	for (const auto& train : this->trains_)
		train->deleteEditorTrain();
	for (const auto& particle : this->particles_)
		particle->deleteEditorParticle();
	for (const auto& pickup : this->pickups_)
		pickup->deleteEditorPickup();
	for (const auto& objective : this->objectives_) {
		if (auto* checkpoint = dynamic_cast<CheckpointObjective*>(objective.get())) {
			checkpoint->removeEditorBlip();
		}
	}
}

void Scene::unloadProjectScene() const {
	for (const auto& entity : this->actors_)
		entity->deleteProjectEntity();
	for (const auto& entity : this->vehicles_)
		entity->deleteProjectEntity();
	for (const auto& entity : this->objects_)
		entity->deleteProjectEntity();
	for (const auto& entity : this->trains_)
		entity->deleteProjectEntity();
	for (const auto& entity : this->particles_)
		entity->deleteProjectEntity();
	for (const auto& entity : this->pickups_)
		entity->deleteProjectEntity();
	for (const auto& objective : this->objectives_) {
		if (auto* checkpoint = dynamic_cast<CheckpointObjective*>(objective.get())) {
			checkpoint->removeProjectBlip();
			checkpoint->removeProjectComeBackBlip();
		}
	}
}

void Scene::loadEditorScene() const {
	for (const auto& actor : this->actors_)
		actor->spawnEditorPed();
	for (const auto& vehicle : this->vehicles_)
		vehicle->spawnEditorVehicle();
	for (const auto& object : this->objects_)
		object->spawnEditorObject();
	for (const auto& object : this->trains_)
		object->spawnEditorTrain();
	for (const auto& particle : this->particles_)
		particle->spawnEditorParticle();
	for (const auto& pickup : this->pickups_)
		pickup->spawnEditorPickup();
	for (const auto& objective : this->objectives_) {
		if (auto* checkpoint = dynamic_cast<CheckpointObjective*>(objective.get())) {
			checkpoint->createEditorBlip();
		}
	}
}
