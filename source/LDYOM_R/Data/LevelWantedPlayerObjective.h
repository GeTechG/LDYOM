#pragma once
#include "PlayerObjective.h"

class LevelWantedPlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & levelWanted_;
	}

	int levelWanted_ = 0;
public:
	LevelWantedPlayerObjective() = default;
	explicit LevelWantedPlayerObjective(void* _new);
	~LevelWantedPlayerObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
