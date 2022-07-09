#pragma once
#include <array>
#include <CObject.h>
#include <optional>
#include <boost/serialization/access.hpp>
#include "boost/uuid/uuid.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/signals2.hpp>

#include "constants.h"
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"
#include <boost/serialization/array.hpp>

class Audio final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar& boost::serialization::make_array(this->name_.data(), this->name_.size());
		ar& boost::serialization::make_array(this->pos_.data(), this->pos_.size());
		ar& this->audioName_;
		ar& this->audio3D_;
		ar& this->attachType3d_;
		ar& this->attachUuid_;
		ar& this->isLooped;
		ar& this->volume_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<CObject*> editorAudioObject_;
	std::optional<int> editorAudio_;
	std::optional<int> projectAudio_;

	std::array<char, NAME_SIZE> name_{};
	std::array<float, 3> pos_{};
	std::string audioName_;
	bool audio3D_ = false;
	int attachType3d_ = 0;
	boost::uuids::uuid attachUuid_{};
	bool isLooped = false;
	float volume_ = 1.f;

	std::optional<boost::signals2::connection> attachDeleterConnection_;

	static std::vector<std::string> audioFilesList_;

	std::optional<int> loadAudio();
public:
	Audio() = default;
	Audio(const char* name, const CVector& pos);

	Audio(const Audio& other);
	Audio& operator=(const Audio& other);

	~Audio() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<int>& getEditorAudio();
	std::optional<int>& getProjectAudio();

	std::string& getAudioName();
	bool& isAudio3D();
	int& getAttachType3d();
	boost::uuids::uuid& getAttachUuid();
	bool& isIsLooped();
	float& getVolume();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorAudio();
	void deleteEditorAudio();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	void preloadProjectAudio();

	static std::vector<std::string>& getAudioFilesList();

	static void loadAudioFilesList();
};
