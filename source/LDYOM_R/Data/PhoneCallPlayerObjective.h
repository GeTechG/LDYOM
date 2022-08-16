#pragma once
#include "PlayerObjective.h"


struct Dialog {
	std::array<char, TEXT_SIZE> text = {""};
	float textTime = 2.f;
	bool moveMouth = true;
	std::array<char, TEXT_SIZE> gameText = { "" };
};

namespace boost::serialization {

	template<class Archive>
	void serialize(Archive& ar, Dialog& p, const unsigned int version) {
		ar & make_array(p.text.data(), p.text.size());
		ar & p.moveMouth;
		ar & p.textTime;
	}
}

class PhoneCallPlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & dialogs_;
	}

	std::vector<Dialog> dialogs_;
public:
	PhoneCallPlayerObjective() = default;
	explicit PhoneCallPlayerObjective(void* _new);
	~PhoneCallPlayerObjective() override = default;

	int getTypeCategory() override {
		return 6;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
