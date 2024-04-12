#include "CCameraExtend.h"

#include <common.h>

#include <CVector.h>
#include <tweeny.h>

#include "CMatrixUtils.h"
#include "CQuaternionUtils.h"
#include "glm/gtx/quaternion.hpp"

CCameraExtend TheCameraExtend = CCameraExtend(
	reinterpret_cast<CCamera*>(GLOBAL_ADDRESS_BY_VERSION(0xB6F028, 0, 0, 0, 0, 0)));

plugin::ThiscallEvent<plugin::AddressList<0x0052C983, plugin::H_CALL>,
                      plugin::PRIORITY_BEFORE,
                      plugin::ArgPickN<CCamera*, 0>, void(CCamera *, char, char)> cameraProcessEvent;


struct CachedQuaterninonData {
	CQuaternion *from, *to;
	float halftheta, sintheta_inv;
} quatData;

auto tween = tweeny::from(0.f).to(1.f);

void CCameraExtend::Process() {
	const auto deltaTime = static_cast<float>(CTimer::m_snTimeInMilliseconds - CTimer::m_snPreviousTimeInMilliseconds) /
		1000.0f;
	if (extendMode) {
		if (this->playingPath) {
			if (this->pathTimeProgress < this->playingPath->getTime()) {
				const auto t = std::min(this->pathTimeProgress / this->playingPath->getTime(), 1.f);
				const auto point = curvePoint(this->playingPath->getPositionCurve(), t);
				this->matrix.pos = CVector(point.x, point.y, point.z);

				auto rotationsCurve = this->playingPath->getRotationsCurve();

				CQuaternion interpolatedRot;

				if (this->playingPath->isCatmullRomRotations()) {
					interpolatedRot = catmullRomSpline(rotationsCurve, t);
				} else {
					const float rotT = static_cast<float>(rotationsCurve.size() - 1) * t;
					auto &rotFrom = rotationsCurve.at(static_cast<size_t>(rotT));
					auto &rotTo = rotationsCurve.at(
						std::min(static_cast<size_t>(rotT + 1), rotationsCurve.size() - 1));

					if (quatData.from != &rotFrom || quatData.to != &rotTo) {
						const float dot = glm::dot(
							glm::quat(rotFrom.real, rotFrom.imag.x, rotFrom.imag.y, rotFrom.imag.z),
							glm::quat(rotTo.real, rotTo.imag.x, rotTo.imag.y, rotTo.imag.z));
						if (dot > 0.99995f) {
							quatData.halftheta = 0.0f;
							quatData.sintheta_inv = 1.0f;
						} else {
							quatData.halftheta = acos(dot);
							quatData.sintheta_inv = 1.0f / sin(quatData.halftheta);
						}
						quatData.from = &rotFrom;
						quatData.to = &rotTo;
					}

					tween = tween.via(
						static_cast<tweeny::easing::enumerated>(this->playingPath->getRotationsEasing()[static_cast<
							size_t>(
							rotT)]));
					tween = tween.during(static_cast<unsigned>(this->playingPath->getTime() * 1000.f));
					const float rotP = tween.seek(rotT - floor(rotT));

					interpolatedRot.Slerp(rotFrom, rotTo, quatData.halftheta, quatData.sintheta_inv, rotP);
				}


				this->matrix.SetRotate(interpolatedRot);

				this->pathTimeProgress += deltaTime;
			} else {
				this->playingPath = nullptr;
				this->pathTimeProgress = 0.0f;
			}
		} else {
			if (this->attachEntity != nullptr) {
				const CMatrix *matrix = this->attachEntity->m_matrix;

				glm::quat q = quat_cast(CMatrixToGlmMat4(*matrix));
				auto multPos = q * glm::vec3(this->localMatrix.pos.x, this->localMatrix.pos.y, this->localMatrix.pos.z);
				this->matrix.pos = matrix->pos + CVector(multPos.x, multPos.y, multPos.z);
				auto rotated = quat_cast(
					CMatrixToGlmMat4(*matrix) * CMatrixToGlmMat4(this->localMatrix));
				this->matrix.SetRotate({{rotated.x, rotated.y, rotated.z}, rotated.w});
			}
			if (this->lookEntity != nullptr) {
				CVector up = {0.0f, 0.0f, 1.0f};
				this->camera->SetCamPositionForFixedMode(&this->matrix.pos, &up);
				this->camera->TakeControl(lookEntity, 15, 2, 1);
				this->matrix.right = this->camera->m_mCameraMatrix.up;
				this->matrix.up = this->camera->m_mCameraMatrix.right;
				this->matrix.at = this->camera->m_mCameraMatrix.at;
			}
		}

		this->camera->m_mCameraMatrix.right = matrix.up;
		this->camera->m_mCameraMatrix.up = matrix.right;
		this->camera->m_mCameraMatrix.at = matrix.at;
		this->camera->m_mCameraMatrix.pos = matrix.pos;
	} else {
		this->matrix.right = this->camera->m_mCameraMatrix.up;
		this->matrix.up = this->camera->m_mCameraMatrix.right;
		this->matrix.at = this->camera->m_mCameraMatrix.at;
		this->matrix.pos = this->camera->m_mCameraMatrix.pos;
	}
}

CCameraExtend::CCameraExtend(CCamera *camera) {
	this->camera = camera;
}

void CCameraExtend::InitHook() {
	cameraProcessEvent += [this](const CCamera *currentCamera) {
		if (this->camera == currentCamera) {
			this->Process();
		}
	};
}

void CCameraExtend::setExtendMode(const bool mode) {
	extendMode = mode;
	this->attachEntity = nullptr;
	this->lookEntity = nullptr;
}

void CCameraExtend::attachToEntity(CEntity *attachEntity, CEntity *lookEntity, const CQuaternion &rotationOffset,
                                   const CVector &posOffset) {
	this->attachEntity = attachEntity;
	if (lookEntity != nullptr) {
		this->lookEntity = lookEntity;
	}
	this->localMatrix.SetRotate(rotationOffset);
	this->localMatrix.pos = posOffset;
}

bool CCameraExtend::playCameraPath(CameraPath *path) {
	const bool isValid = path->isValid();
	if (isValid) {
		this->playingPath = path;
		this->pathTimeProgress = 0.0f;
		ensureShortestPath(path->getRotationsCurve());
	}
	return isValid;
}

void CCameraExtend::stopCameraPath() {
	if (this->playingPath) {
		this->playingPath = nullptr;
		this->pathTimeProgress = 0.0f;
	}
}

bool CCameraExtend::isPlayingPath() const {
	return this->playingPath != nullptr && this->pathTimeProgress < this->playingPath->getTime();
}

float CCameraExtend::getPathProgress() const {
	if (this->playingPath) {
		return this->pathTimeProgress / this->playingPath->getTime();
	}
	return 0.0f;
}
