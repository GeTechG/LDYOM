#pragma once
#include <CPed.h>

#include "BaseObjective.h"

class Actor;

class ActorObjective : virtual public BaseObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<BaseObjective>(*this);
		ar & actorUuid_;
		ar & boost::serialization::make_array(text_.data(), text_.size());
		ar & textTime_;
		ar & colorBlip_;
	}

	boost::uuids::uuid actorUuid_{};
	std::array<char, TEXT_SIZE> text_{ "" };
	float textTime_ = 1.f;
	int colorBlip_ = 0;

	std::array<char, TEXT_SIZE> gameText_ = { "" };
protected:
	ActorObjective() = default;

	std::optional<int> editorBlip_;
	std::optional<int> projectBlip_;

	int spawnBlip(CPed* ped);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	boost::uuids::uuid& getActorUuid();
	std::array<char, TEXT_SIZE>& getText();
	float& getTextTime();
	int& getColorBlip();
	std::optional<int>& getEditorBlip();
	std::optional<int>& getProjectBlip();

	virtual ktwait execute(Scene* scene, Actor* actor, Result& result, ktcoro_tasklist& tasklist) = 0;
public:
	~ActorObjective() override;

	int getCategory() override {
		return 1;
	}

	void draw(Localization& local) override;
	void open() override;
	void close() override;
	ktwait execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) override;
};
