#include "model_renderer.h"

#include "CObjectInfo.h"
#include <CModelInfo.h>
#include <CStreaming.h>
#include <CStreamingInfo.h>

plugin::CdeclEvent<plugin::AddressList<0x53EA12, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickNone, void()>
	beforeMainRender;

ModelRenderer::ModelRenderer(const CVector& rotateModel, const int sceneSize)
	: m_scene(sceneSize),
	  m_rotateModel(rotateModel) {
	m_scene.initialize();
}

ModelRenderer::ModelRenderer(const CVector& rotateModel, int sceneWidth, int sceneHeight)
	: m_scene(sceneWidth, sceneHeight),
	  m_rotateModel(rotateModel) {
	m_scene.initialize();
}

void ModelRenderer::init() {
	beforeMainRender += [&] {
		if (!this->listReRender_.empty()) {
			for (auto& reRender : this->listReRender_) {
				const auto info = CModelInfo::GetModelInfo(reRender.first);
				if (!info)
					continue;

				float mFRadius = info->m_pColModel->m_boundSphere.m_fRadius;

				const RwRGBA bg = {static_cast<RwUInt8>(this->backgroundColor_[0] * 255.f),
				                   static_cast<RwUInt8>(this->backgroundColor_[1] * 255.f),
				                   static_cast<RwUInt8>(this->backgroundColor_[2] * 255.f),
				                   static_cast<RwUInt8>(this->backgroundColor_[3] * 255.f)};

				RwV3d& posRw = *reinterpret_cast<RwV3d*>(this->pos.data());
				posRw.y = -mFRadius * 4.f + abs(this->zoomKoef);
				const auto rotate = this->m_rotateModel.ToRwV3d();
				switch (info->GetModelType()) {
				case MODEL_INFO_VEHICLE:
					reRender.second->setTexture(this->m_scene.renderVehicle(reRender.first, bg, posRw, rotate),
					                            this->m_scene.getWidth(), this->m_scene.getHeight());
					break;
				case MODEL_INFO_PED:
					reRender.second->setTexture(this->m_scene.renderPed(reRender.first, bg, posRw, rotate),
					                            this->m_scene.getWidth(), this->m_scene.getHeight());
					break;
				case MODEL_INFO_WEAPON:
				case MODEL_INFO_ATOMIC:
				case MODEL_INFO_CLUMP:
					reRender.second->setTexture(this->m_scene.renderObject(reRender.first, bg, posRw, rotate),
					                            this->m_scene.getWidth(), this->m_scene.getHeight());
					break;
				default:
					break;
				}
			}
			this->listReRender_.clear();
		}
		if (!this->listSpecialPedRender_.empty()) {
			for (const auto& reRender : this->listSpecialPedRender_) {
				CStreaming::RequestSpecialChar(0, reRender.first.c_str(), GAME_REQUIRED);
				CStreaming::LoadAllRequestedModels(false);

				const auto info = CModelInfo::GetModelInfo(290);
				if (!info)
					continue;

				auto size = info->m_pColModel->m_boundBox.m_vecMax - info->m_pColModel->m_boundBox.m_vecMin;
				const float maxSize = std::max(size.x, std::max(size.y, size.z));

				const RwRGBA bg = {static_cast<RwUInt8>(this->backgroundColor_[0] * 255.f),
				                   static_cast<RwUInt8>(this->backgroundColor_[1] * 255.f),
				                   static_cast<RwUInt8>(this->backgroundColor_[2] * 255.f),
				                   static_cast<RwUInt8>(this->backgroundColor_[3] * 255.f)};

				RwV3d& posRw = *reinterpret_cast<RwV3d*>(this->pos.data());
				posRw.y = -maxSize * 4.f + abs(this->zoomKoef);
				const auto rotate = this->m_rotateModel.ToRwV3d();
				reRender.second->setTexture(this->m_scene.renderPed(290, bg, posRw, rotate), this->m_scene.getWidth(),
				                            this->m_scene.getHeight());

				CStreaming::SetMissionDoesntRequireSpecialChar(0);
			}
			this->listSpecialPedRender_.clear();
		}
	};
}

std::array<float, 3>& ModelRenderer::getPos() { return pos; }

CVector& ModelRenderer::getRotateModel() { return m_rotateModel; }

float& ModelRenderer::getZoomKoef() { return zoomKoef; }

void ModelRenderer::render(const int model, RenderwareTexture* texture) {
	this->listReRender_.emplace_back(model, texture);
}

void ModelRenderer::renderSpecialPed(const std::string& modelName, RenderwareTexture* texture) {
	this->listSpecialPedRender_.emplace_back(modelName, texture);
}
