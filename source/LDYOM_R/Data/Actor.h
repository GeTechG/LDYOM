#pragma once
#include "plugin.h"
#include <optional>
#include "constants.h"
#include "INameable.h"
#include "IPositionable.h"
#include "Weapon.h"

class Actor final : public INameable, IPositionable {
private:
	std::optional<CPed*> editorPed_;
	std::optional<CPed*> projectPed_;

	char name_[NAME_SIZE]{};
	float pos_[3]{};
	float headingAngle_;
	int group_;
	unsigned char modelType_;
	int slot_;
	int modelId_;
	std::vector<Weapon> weapons_;
	int defaultWeapon_;

	CPed* spawnPed();
public:
	Actor(const char* name, const CVector& pos, float headingAngle);
	Actor(const Actor& other);

	Actor& operator=(Actor other);

	~Actor() override;

	char* getName() override;
	float* getPosition() override;

	std::optional<CPed*>& getEditorPed();
	std::optional<CPed*>& getProjectPed();

	float& getHeadingAngle();
	int& getGroup();
	int& getModelId();
	std::vector<Weapon>& getWeapons();
	int& getDefaultWeapon();
	unsigned char& getModelType();
	int& getSlot();

	void updateLocation() const;

	void spawnEditorPed();
	void spawnProjectPed();
	void deleteEditorPed();
	void deleteProjectPed();
};
