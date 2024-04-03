#pragma once
#include <CCamera.h>


class CCameraExtend {
private:
	CCamera *camera;

	bool extendMode = false;

	void Process();

public:
	CMatrix matrix;
	CMatrix localMatrix;
	CEntity *attachEntity = nullptr;
	CEntity *lookEntity = nullptr;

	CCameraExtend(CCamera *camera);

	CCameraExtend() = delete;

	void InitHook();

	void setExtendMode(bool mode);
	void attachToEntity(CEntity *attachEntity, CEntity *lookEntity, const CQuaternion &rotationOffset,
	                    const CVector &posOffset);
};

extern CCameraExtend TheCameraExtend;
