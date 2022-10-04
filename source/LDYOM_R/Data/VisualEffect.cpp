#include "VisualEffect.h"

#include <array>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <Events.h>

#include "CCoronas.h"
#include "LuaEngine.h"
#include "ProjectPlayerService.h"
#include "strUtils.h"
#include "utils.h"
#include "easylogging/easylogging++.h"
#include "Scene.h"

using namespace plugin;

void VisualEffect::drawVisualEffect() {
	const auto red = static_cast<unsigned char>(color_[0] * 255);
	const auto green = static_cast<unsigned char>(color_[1] * 255);
	const auto blue = static_cast<unsigned char>(color_[2] * 255);
	const auto alpha = static_cast<unsigned char>(color_[3] * 255);

	const auto position = CVector(pos_[0], pos_[1], pos_[2]);
	if (this->type_ == 0) {
		const auto coronaType = static_cast<eCoronaType>(this->effectType_);
		const auto coronaFlare = static_cast<eCoronaFlareType>(this->flare_);
		CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(this), nullptr, red, green, blue, alpha, position, this->size_, 150.0f, coronaType, coronaFlare, true, false, 0, 0.0f, false, 0.2f, 0, 15.0f, false, false);
	} else {
		const auto shadowTextureType = static_cast<unsigned char>(this->effectType_);
		Command<Commands::DRAW_SHADOW>(shadowTextureType, pos_[0], pos_[1], pos_[2], RAD(this->angle_), this->size_, alpha, red, green, blue);
	}
}

VisualEffect::VisualEffect(const char* name, const CVector& pos): ObjectiveDependent(nullptr),
													  uuid_(boost::uuids::random_generator()()) {
	strlcpy(this->name_.data(), name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

VisualEffect::VisualEffect(const VisualEffect& other): ObjectiveDependent{other},
                                                       INameable{other},
                                                       IPositionable{other},
                                                       IUuidable{other},
                                                       uuid_{ boost::uuids::random_generator()() },
                                                       name_{other.name_},
                                                       pos_{other.pos_},
                                                       type_{other.type_},
                                                       size_{other.size_},
                                                       effectType_{other.effectType_},
                                                       flare_{other.flare_},
                                                       color_{other.color_},
                                                       drawing_{other.drawing_},
													   angle_(other.angle_) {
}

VisualEffect& VisualEffect::operator=(const VisualEffect& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	name_ = other.name_;
	pos_ = other.pos_;
	type_ = other.type_;
	size_ = other.size_;
	effectType_ = other.effectType_;
	flare_ = other.flare_;
	color_ = other.color_;
	drawing_ = other.drawing_;
	angle_ = other.angle_;
	return *this;
}

VisualEffect::~VisualEffect() {
	this->deleteEditorVisualEffect();
	this->deleteProjectEntity();
}

boost::uuids::uuid& VisualEffect::getUuid() {
	return uuid_;
}

int& VisualEffect::getType() {
	return type_;
}

float& VisualEffect::getSize() {
	return size_;
}

int& VisualEffect::getEffectType() {
	return effectType_;
}

int& VisualEffect::getFlare() {
	return flare_;
}

std::array<float, 4>& VisualEffect::getColor() {
	return color_;
}

bool& VisualEffect::isDrawing() {
	return drawing_;
}

float& VisualEffect::getAngle() {
	return angle_;
}

char* VisualEffect::getName() {
	return this->name_.data();
}

float* VisualEffect::getPosition() {
	return pos_.data();
}

void VisualEffect::spawnEditorVisualEffect() {
	if (editorVisualEffect_.has_value())
		this->deleteEditorVisualEffect();

	auto slot = reinterpret_cast<unsigned>(&this->editorVisualEffect_);

	Events::processScriptsEvent.AddAtId(slot, [this] {
		this->drawVisualEffect();
	});

	this->editorVisualEffect_ = slot;
}

extern bool restart;
void VisualEffect::deleteEditorVisualEffect() {
	if (this->editorVisualEffect_.has_value()) {
		Events::processScriptsEvent.RemoveById(this->editorVisualEffect_.value());
		this->editorVisualEffect_ = std::nullopt;
	}
}

void VisualEffect::spawnProjectEntity() {
	if (projectVisualEffect_.has_value())
		this->deleteProjectEntity();

	auto slot = reinterpret_cast<unsigned>(&this->projectVisualEffect_);

	Events::processScriptsEvent.AddAtId(slot, [this] {
		this->drawVisualEffect();
	});

	this->projectVisualEffect_ = slot;

	auto& scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		const auto onVisualEffectSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onVisualEffectSpawn"].get_or_create<sol::table>();
		for (auto [_, func] : onVisualEffectSpawn) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}
	}
}

void VisualEffect::deleteProjectEntity() {
	if (this->projectVisualEffect_.has_value()) {
		Events::processScriptsEvent.RemoveById(this->projectVisualEffect_.value());
		this->projectVisualEffect_ = std::nullopt;

		auto scene = ProjectPlayerService::getInstance().getCurrentScene();
		auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

		if (scene.has_value() && tasklist != nullptr) {
			const auto onVisualEffectDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onVisualEffectDelete"].get_or_create<sol::table>();
			for (auto [_, func] : onVisualEffectDelete) {
				if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
					const sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
				}
			}
		}
	}
}
