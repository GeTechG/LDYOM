#pragma once
#include <functional>

#include "SaveService.h"
#include "SceneObjective.h"

class SaveObjective final : public virtual SceneObjective {
private:
	bool confirmSave_ = false;

	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<SceneObjective>(*this);
		ar & confirmSave_;
	}

public:

	SaveObjective() = default;
	explicit SaveObjective(void* _new);
	~SaveObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
	static void saveGame(bool confirm, int nodeSave, Scene* scene, ktcoro_tasklist& tasklist, std::function<void()> callback);
};
