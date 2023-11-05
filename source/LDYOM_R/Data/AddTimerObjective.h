#pragma once
#include "MathCondition.h"
#include "WorldObjective.h"

class AddTimerObjective final: public virtual WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & typeTimer_;
		ar & backward_;
		ar & compareType_;
		ar & compareValue_;
		ar & startTime_;
		ar & boost::serialization::make_array(text_.data(), text_.size());
	}

	int typeTimer_ = 0;
	bool backward_ = false;
	MathCondition compareType_ = Equal;
	int compareValue_ = 0;
	int startTime_ = 0;
	std::array<char, TEXT_SIZE> text_ = {""};

	std::array<char, TEXT_SIZE> gameText_{};
public:
	AddTimerObjective() = default;
	explicit AddTimerObjective(void* _new);
	~AddTimerObjective() override = default;

	int getTypeCategory() override {
		return 7;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;

	int& getTypeTimer();
	MathCondition& getCompareType();
	int& getCompareValue();
};
