#pragma once
#include "WorldObjective.h"

class TimeoutObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar& boost::serialization::base_object<WorldObjective>(*this);
		ar& time_;
		ar& boost::serialization::make_array(text_.data(), text_.size());
	}

	float time_ = 1.f;
	std::array<char, TEXT_SIZE> text_{""};

	std::array<char, TEXT_SIZE> gameText_{};
public:
	TimeoutObjective() = default;
	explicit TimeoutObjective(void* _new);
	~TimeoutObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
