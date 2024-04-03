#include "CCameraExtend.h"

#include <common.h>

#include <CVector.h>
#include "CMatrixUtils.h"
#include "glm/gtx/quaternion.hpp"

CCameraExtend TheCameraExtend = CCameraExtend(
	reinterpret_cast<CCamera*>(GLOBAL_ADDRESS_BY_VERSION(0xB6F028, 0, 0, 0, 0, 0)));

plugin::ThiscallEvent<plugin::AddressList<0x0052C983, plugin::H_CALL>,
                      plugin::PRIORITY_BEFORE,
                      plugin::ArgPickN<CCamera*, 0>, void(CCamera *, char, char)> cameraProcessEvent;

void CCameraExtend::Process() {
	if (extendMode) {
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
