#pragma once
#include "ActorObjective.h"

class FollowPathActorObjective final: public virtual ActorObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ActorObjective>(*this);
		ar & moveType_;
		ar & pathType_;
		ar & executeTime_;
		ar & path_;
		ar & waitEnd;
	}

	int moveType_ = 0;
	int pathType_ = 0;
	float executeTime_ = 1.f;
	std::vector<std::array<float, 3>> path_;
	bool waitEnd = false;
public:
	FollowPathActorObjective() = default;
	explicit FollowPathActorObjective(void* _new);
	~FollowPathActorObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Actor * actor, Result & result, ktcoro_tasklist & tasklist) override;

	int& getMoveType();
	int& getPathType();
	float& getExecuteTime();
	std::vector<std::array<float, 3>>& getPath();
};
