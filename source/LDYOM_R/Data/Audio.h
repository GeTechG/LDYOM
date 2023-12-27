#pragma once
#include <array>
#include <CObject.h>
#include <optional>
#include <boost/signals2.hpp>
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"

class Audio final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid{};

	std::optional<CObject*> editorAudioObject_;
	std::optional<int> editorAudio_;
	std::optional<int> projectAudio_;

	std::string name{};
	std::array<float, 3> pos{};
	std::string audioName;
	bool audio3D = false;
	int attachType3d = 0;
	boost::uuids::uuid attachUuid{};
	bool isLooped = false;
	float volume = 1.f;

	static std::vector<std::string> audioFilesList_;

	std::optional<int> loadAudio();

public:
	Audio() = default;
	Audio(const char *name, const CVector &pos);
	Audio(const Audio &other) = default;
	Audio(Audio &&other) = default;

	Audio copy() const;

	Audio& operator=(Audio other) {
		using std::swap;
		swap(*this, other);
		return *this;
	}

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

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorAudio();
	void deleteEditorAudio();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	static std::vector<std::string>& getAudioFilesList();

	static void loadAudioFilesList();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Audio> {
		static void to_json(json &j, const Audio &obj) {
			auto &base = static_cast<const ObjectiveDependent&>(obj);
			j = base;
			auto &a = const_cast<Audio&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["audioName"] = a.getAudioName();
			j["audio3D"] = a.isAudio3D();
			j["attachType3d"] = a.getAttachType3d();
			j["attachUuid"] = a.getAttachUuid();
			j["isLooped"] = a.isIsLooped();
			j["volume"] = a.getVolume();
		}

		static void from_json(const json &j, Audio &obj) {
			auto &base = static_cast<ObjectiveDependent&>(obj);
			j.get_to(base);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("audioName").get_to(obj.getAudioName());
			j.at("audio3D").get_to(obj.isAudio3D());
			j.at("attachType3d").get_to(obj.getAttachType3d());
			j.at("attachUuid").get_to(obj.getAttachUuid());
			j.at("isLooped").get_to(obj.isIsLooped());
			j.at("volume").get_to(obj.getVolume());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
