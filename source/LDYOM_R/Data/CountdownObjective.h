#pragma once
#include "WorldObjective.h"

class CountdownObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive &ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & time_;
		ar & boost::serialization::make_array(textGo_.data(), textGo_.size());
	}

	int time_ = 3;
	std::array<char, TEXT_SIZE> textGo_{"Go!"};

	std::array<char, TEXT_SIZE> gameTextGo_{""};

public:
	CountdownObjective() = default;
	explicit CountdownObjective(void *_new);
	~CountdownObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};
