#include "virtual_scene.h"
#include "CObject.h"
#include "CScene.h"
#include "CStreaming.h"
#include "CVehicle.h"
#include "CVisibilityPlugins.h"
#include "common.h"
#include "extensions/ScriptCommands.h"

using namespace plugin;

RwV3d Xaxis{1.0f, 0.0f, 0.0f};
RwV3d Yaxis{0.0f, 1.0f, 0.0f};
RwV3d Zaxis{0.0f, 0.0f, 1.0f};

void add_camera_to_gta_scene(RwCamera* camera) {
	if (Scene.m_pWorld)
		RpWorldAddCamera(Scene.m_pWorld, camera);
}

void add_light_to_gta_scene(RpLight* light) {
	if (Scene.m_pWorld)
		RpWorldAddLight(Scene.m_pWorld, light);
}

void remove_light_from_gta_scene(RpLight* light) {
	if (Scene.m_pWorld)
		RpWorldRemoveLight(Scene.m_pWorld, light);
}

RpLight* create_light() {
	RpLight* light = RpLightCreate(rpLIGHTAMBIENT);
	if (light) {
		const RwRGBAReal col{1.f, 1.f, 1.f, 1.f};
		RpLightSetColor(light, &col);
	}
	return light;
}

RwFrame* create_frame() {
	RwFrame* frame = RwFrameCreate();
	if (frame) {
		const RwV3d pos{0.f, 0.f, 0.f};
		RwFrameTranslate(frame, &pos, rwCOMBINEPRECONCAT);
		RwFrameRotate(frame, &Xaxis, 90.f, rwCOMBINEPRECONCAT);
	}
	return frame;
}

bool VirtualScene::create_texture(RwRaster*& buffer, RwTexture*& texture) const {
	buffer = RwRasterCreate(frameWidth_, frameHeight_, 0, rwRASTERTYPECAMERATEXTURE);
	if (!buffer)
		return false;
	texture = RwTextureCreate(buffer);
	return texture != nullptr;
}

void update_matrix(CEntity* entity, const RwMatrix& mat) {
	if (!entity || !entity->m_matrix)
		return;

	auto* frame = static_cast<RwFrame*>(entity->m_pRwObject->parent);
	entity->Remove();
	std::memcpy(entity->m_matrix, &mat, sizeof(RwMatrix));
	entity->GetMatrix()->UpdateRW(&frame->modelling);
	entity->UpdateRwFrame();
	entity->Add();
}

void rotate_entity(CEntity* entity, const RwV3d& rotate) {
	RwMatrix mat{};
	entity->GetMatrix()->UpdateRW(&mat);
	if (rotate.x != 0.f)
		RwMatrixRotate(&mat, &Xaxis, rotate.x, rwCOMBINEPRECONCAT);
	if (rotate.y != 0.f)
		RwMatrixRotate(&mat, &Yaxis, rotate.y, rwCOMBINEPRECONCAT);
	if (rotate.z != 0.f)
		RwMatrixRotate(&mat, &Zaxis, rotate.z, rwCOMBINEPRECONCAT);
	update_matrix(entity, mat);
}

VirtualScene::VirtualScene(const int frameSize)
	: frameWidth_(frameSize),
	  frameHeight_(frameSize) {}

VirtualScene::VirtualScene(const int frameWidth, const int frameHeight)
	: frameWidth_(frameWidth),
	  frameHeight_(frameHeight) {}

bool VirtualScene::initialize() {
	light_ = create_light();
	if (!light_)
		return false;

	zraster_ = RwRasterCreate(frameWidth_, frameHeight_, 0, rwRASTERTYPEZBUFFER);
	if (!create_texture(raster_, texture_) || !zraster_)
		return false;

	camera_ = RwCameraCreate();
	frame_ = create_frame();
	if (!camera_ || !frame_)
		return false;
	camera_->frameBuffer = raster_;
	camera_->zBuffer = zraster_;
	rwObjectHasFrameSetFrame(camera_, frame_);
	RwCameraSetFarClipPlane(camera_, 300.f);
	RwCameraSetNearClipPlane(camera_, 0.01f);

	RwV2d calculatedViewWin;
	if (frameHeight_ > 0) { // Check for valid frameHeight to prevent division by zero
		const float aspectRatio = static_cast<float>(frameWidth_) / static_cast<float>(frameHeight_);
		// Adjust the horizontal component of the view window by the aspect ratio,
		// keeping the vertical component fixed. This maintains the vertical field of view.
		calculatedViewWin = {0.5f * aspectRatio, 0.5f};
	} else {
		// Fallback to a square view window if frameHeight is invalid.
		// Ideally, frameWidth_ and frameHeight_ should always be positive.
		calculatedViewWin = {0.5f, 0.5f};
	}
	RwCameraSetViewWindow(camera_, &calculatedViewWin); // Apply the calculated view window

	RwCameraSetProjection(camera_, rwPERSPECTIVE);
	add_camera_to_gta_scene(camera_);

	return true;
}

#define RWRSTATE(a) (reinterpret_cast<void*>(a))

void VirtualScene::begin_camera(RwRaster* buffer, RwRGBA bgr) const {
	camera_->frameBuffer = buffer;
	camera_->zBuffer = zraster_;
	CVisibilityPlugins::SetRenderWareCamera(camera_);
	RwCameraClear(camera_, &bgr, rwCAMERACLEARIMAGE | rwCAMERACLEARZ);
	RwCameraBeginUpdate(camera_);
	add_light_to_gta_scene(light_);

	RwRenderStateSet(rwRENDERSTATECULLMODE, RWRSTATE(rwCULLMODECULLBACK));
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, RWRSTATE(TRUE));
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, RWRSTATE(TRUE));
	RwRenderStateSet(rwRENDERSTATESHADEMODE, RWRSTATE(rwSHADEMODEGOURAUD));
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, RWRSTATE(FALSE));
}

void VirtualScene::end_camera() const {
	RwCameraEndUpdate(camera_);
	remove_light_from_gta_scene(light_);
}

RwTexture* VirtualScene::renderEntity(CEntity* entity, const RwRGBA bgr) const {
	if (!entity)
		return nullptr;
	RpClump* clump = entity->m_pRwClump;
	if (!clump)
		return nullptr;

	RwRaster* buffer;
	RwTexture* tex;
	if (!create_texture(buffer, tex))
		return nullptr;

	begin_camera(buffer, bgr);
	entity->Add();
	entity->PreRender();
	CVisibilityPlugins::RenderEntity(entity, false, 999.f);
	entity->Remove();
	end_camera();

	return tex;
}

RwTexture* VirtualScene::renderVehicle(int model, const RwRGBA bgr, const RwV3d& pos, const RwV3d& rotate,
                                       const int primary, const int secondary) {
	if (model == 570)
		model = 538;
	else if (model == 569)
		model = 537;

	CVehicle* veh;
	CStreaming::RequestModel(model, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);
	Command<COMMAND_CREATE_CAR>(model, 0.f, 0.f, 50.f, &veh);
	if (!veh)
		return nullptr;
	veh->m_bCollisionProcessed = false;
	veh->m_bUsesCollision = false;
	veh->Teleport(pos, false);
	rotate_entity(veh, rotate);

	if (primary != -1 && secondary != -1) {
		veh->m_nPrimaryColor = static_cast<unsigned char>(primary);
		veh->m_nSecondaryColor = static_cast<unsigned char>(secondary);
	}

	RwTexture* tex = renderEntity(veh, bgr);
	Command<COMMAND_DELETE_CAR>(veh);
	return tex;
}

RwTexture* VirtualScene::renderPed(int model, RwRGBA bgr, const RwV3d& pos, const RwV3d& rotate) {
	CPed* ped;
	CStreaming::RequestModel(model, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);
	Command<COMMAND_CREATE_CHAR>(0, model, 0.f, 0.f, 50.f, &ped);
	if (!ped)
		return nullptr;
	ped->SetIdle();
	ped->Teleport(pos, false);
	ped->m_bCollisionProcessed = false;
	ped->m_bUsesCollision = false;
	rotate_entity(ped, rotate);
	RpAnimBlendClumpUpdateAnimations(ped->m_pRwClump, 100.f, true); // UpdateAnim

	RwTexture* tex = renderEntity(ped, bgr);
	Command<COMMAND_DELETE_CHAR>(ped);
	return tex;
}

RwTexture* VirtualScene::renderObject(int model, const RwRGBA bgr, const RwV3d& pos, const RwV3d& rotate) {
	CObject* object;
	CStreaming::RequestModel(model, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);
	Command<COMMAND_CREATE_OBJECT>(model, 0.f, 0.f, 50.f, &object);
	if (!object)
		return nullptr;
	object->CreateRwObject();
	object->Teleport(pos, false);
	rotate_entity(object, rotate);

	RwTexture* tex = renderEntity(object, bgr);
	Command<COMMAND_DELETE_OBJECT>(object);
	return tex;
}

int VirtualScene::getWidth() const { return frameWidth_; }

int VirtualScene::getHeight() const { return frameHeight_; }
