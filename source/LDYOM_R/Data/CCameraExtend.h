#pragma once
#include <CCamera.h>

#include "CameraPath.h"


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
	CameraPath *playingPath = nullptr;
	float pathTimeProgress = 0.0f;

	CCameraExtend(CCamera *camera);

	CCameraExtend() = delete;

	void InitHook();

	void setExtendMode(bool mode);
	void attachToEntity(CEntity *attachEntity, CEntity *lookEntity, const CQuaternion &rotationOffset,
	                    const CVector &posOffset);
	bool playCameraPath(CameraPath *path);
	void stopCameraPath();
	bool isPlayingPath() const;
	float getPathProgress() const;
};

extern CCameraExtend TheCameraExtend;
