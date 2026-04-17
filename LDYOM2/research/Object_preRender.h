void __thiscall CEntity::PreRender(#39 * this) {
	CBaseModelInfo* v1;           // ebx
	CBaseModelInfo* p_base;       // edi
	char m_n2dfxCount;            // al
	unsigned __int16 m_nMdlFlags; // ax
	RpAtomic* m_pRwObject;        // ebp
	RpGeometry* geometry;         // ebp
	char m_nAlpha;                // al
	__int16 v9;                   // ax
	RwV3d* v10;                   // eax
	RwV3d* v11;                   // edx
	RwV3d* p_m_SimpleCoors;       // ebx
	RwV3d* v13;                   // ebp
	RwV3d* v14;                   // edi
	RwV3d* v15;                   // edx
	RwV3d* v16;                   // ecx
	RwV3d* v17;                   // eax
	long double v18;              // st7
	__int16 v19;                  // cx
	int v20;                      // eax
	RwV3d* v21;                   // eax
	RwV3d* v22;                   // eax
	RwV3d* v23;                   // eax
	RwV3d* v24;                   // eax
	double v25;                   // st7
	__int16 v26;                  // ax
	CVector* v27;                 // ebx
	CVector* v28;                 // edi
	float fX;                     // ebx
	float fY;                     // ebp
	CVector* v31;                 // eax
	RwV3d* v32;                   // eax
	CVector* v33;                 // edi
	RwV3d* pPosition2;            // ecx
	CVector* v35;                 // eax
	CVector* v36;                 // ebx
	CVector* v37;                 // edi
	CVector* v38;                 // ebx
	CVector* v39;                 // eax
	RwV3d* v40;                   // eax
	float x;                      // ecx
	float y;                      // edx
	float z;                      // eax
	float v44;                    // ebx
	float v45;                    // ebp
	CVector* v46;                 // eax
	CVector* v47;                 // eax
	double v48;                   // st6
	double v49;                   // st4
	float* p_x;                   // ecx
	float* v51;                   // eax
	float* v52;                   // edi
	int v53;                      // eax
	float* v54;                   // eax
	CVector* v55;                 // ecx
	CVector* v56;                 // edi
	CSimpleTransform* v57;        // eax
	CVector* v58;                 // eax
	RwV3d* v59;                   // eax
	CVector* v60;                 // ecx
	CVector* v61;                 // edi
	CSimpleTransform* v62;        // eax
	CVector* v63;                 // eax
	RwV3d* v64;                   // eax
	int modelID;                  // eax
	float v66;                    // [esp+0h] [ebp-E4h]
	float v67;                    // [esp+0h] [ebp-E4h]
	float v68;                    // [esp+24h] [ebp-C0h]
	float v69;                    // [esp+28h] [ebp-BCh]
	float v70;                    // [esp+30h] [ebp-B4h]
	RwV3d* p_pos;                 // [esp+48h] [ebp-9Ch]
	float v72;                    // [esp+48h] [ebp-9Ch]
	float v73;                    // [esp+48h] [ebp-9Ch]
	RwV3d v74;                    // [esp+4Ch] [ebp-98h] BYREF
	int v75;                      // [esp+58h] [ebp-8Ch] BYREF
	RwV3d v76;                    // [esp+5Ch] [ebp-88h] BYREF
	RwV3d* v77;                   // [esp+68h] [ebp-7Ch]
	CVector endX;                 // [esp+6Ch] [ebp-78h] BYREF
	CVector v79;                  // [esp+78h] [ebp-6Ch] BYREF
	CVector out;                  // [esp+84h] [ebp-60h] BYREF
	CVector v81;                  // [esp+90h] [ebp-54h] BYREF
	CVector a1;                   // [esp+9Ch] [ebp-48h] BYREF
	CVector v83;                  // [esp+A8h] [ebp-3Ch] BYREF
	RwV3d outVec;                 // [esp+B4h] [ebp-30h] BYREF
	CVector v85;                  // [esp+C0h] [ebp-24h] BYREF
	CVector v86;                  // [esp+CCh] [ebp-18h] BYREF
	CVector v87;                  // [esp+D8h] [ebp-Ch] BYREF

	p_base = &CModelInfo::ms_modelInfoPtrs[*(this + 17)]->clump.base;
	v1 = p_base->vmt->AsAtomicModel(p_base);
	m_n2dfxCount = p_base->m_n2dfxCount;
	v75 = v1;
	if (m_n2dfxCount)
		CEntity::ProcessLightsForEntity(this);
	m_nMdlFlags = p_base->m_nMdlFlags;
	if ((m_nMdlFlags & 1) == 0) {
		p_base->m_nMdlFlags = m_nMdlFlags | 1;
		if (v1) {
			m_pRwObject = v1->m_pRwObject;
			if (m_pRwObject) {
				if (RpMatFXAtomicQueryEffects(v1->m_pRwObject)) {
					geometry = m_pRwObject->geometry;
					if (geometry)
						RpGeometryForAllMaterials(geometry, MaterialUpdateUVAnimCB, 0);
				}
			}
		}
		m_nAlpha = p_base->m_nAlpha;
		if (m_nAlpha >= 239u)
			p_base->m_nAlpha = -1;
		else
			p_base->m_nAlpha = m_nAlpha + 16;
		if (v1) {
			sub_5D7200(v1->m_pRwObject, 0);
		} else if (p_base->vmt->GetModelType(p_base) == OBJECT_TYPE_HAND) {
			sub_5D72C0(p_base->m_pRwObject, 0);
		}
	}
	if ((*(this + 7) & 0x200000) == 0)
		return;
	if (v1) {
		v9 = v1->m_nMdlFlags & 0x7800;
		if ((v9 == 0x800 || v9 == 0x1000) && ((*(this + 54) & 7) != 4 || (*(this + 320) & 0x40) == 0)) {
			if (TheCamera.placeable.m_pMatrix)
				p_pos = &TheCamera.placeable.m_pMatrix->mat.pos;
			else
				p_pos = &TheCamera.placeable.m_SimpleCoors;
			v10 = *(this + 5);
			v11 = v10 + 4;
			if (!v10)
				v11 = (this + 4);
			v77 = v11;
			p_m_SimpleCoors = &TheCamera.placeable.m_pMatrix->mat.pos;
			if (!TheCamera.placeable.m_pMatrix)
				p_m_SimpleCoors = &TheCamera.placeable.m_SimpleCoors;
			v13 = v10 + 4;
			if (!v10)
				v13 = (this + 4);
			v14 = &TheCamera.placeable.m_pMatrix->mat.pos;
			if (!TheCamera.placeable.m_pMatrix)
				v14 = &TheCamera.placeable.m_SimpleCoors;
			v15 = v10 + 4;
			if (!v10)
				v15 = (this + 4);
			if (TheCamera.placeable.m_pMatrix)
				v16 = &TheCamera.placeable.m_pMatrix->mat.pos;
			else
				v16 = &TheCamera.placeable.m_SimpleCoors;
			if (v10)
				v17 = v10 + 4;
			else
				v17 = (this + 4);
			v18 = sqrt((v16->y - v17->y) * (v14->y - v15->y) + (p_m_SimpleCoors->x - v13->x) * (p_pos->x - v77->x));
			if (CObject::fDistToNearestTree >= v18)
				CObject::fDistToNearestTree = v18;
			CEntity::ModifyMatrixForTreeInWind(this);
			v1 = v75;
		}
	}
	if ((*(this + 54) & 7) == 1) {
		if (v1 && (v1->m_nMdlFlags & 0x7800) == 0x4800)
			CEntity::ModifyMatrixForCrane(this);
		return;
	}
	if ((*(this + 54) & 7) != 4) {
		if ((*(this + 54) & 7) != 5)
			return;
		goto LABEL_126;
	}
	v19 = *(this + 17);
	if (v19 == MI_COLLECTABLE1) {
		CPickups::DoCollectableEffects(this);
		CEntity::UpdateRW_(this);
		v20 = *(this + 6);
		if (v20)
			RwFrameUpdateObjects(*(v20 + 4));
		goto LABEL_126;
	}
	if (v19 == MI_MONEY) {
		CPickups::DoMoneyEffects(this);
	LABEL_125:
		CEntity::UpdateRW_(this);
		CEntity::UpdateRwFrame(this);
		goto LABEL_126;
	}
	if (v19 == MI_CARMINE || v19 == MI_NAUTICALMINE || v19 == MI_BRIEFCASE) {
		if ((*(this + 320) & 1) != 0) {
			CPickups::DoMineEffects(this);
			goto LABEL_125;
		}
	} else {
		if (v19 == MODEL_MISSILE) {
			if (CReplay::Mode != 1) {
				v21 = *(this + 5);
				if (v21)
					v22 = v21 + 4;
				else
					v22 = (this + 4);
				v76 = *v22;
				v75 = rand() & 0xF;
				v72 = v75 * 0.0625;
				CShadows::StoreShadowToBeRendered(2, gpShadowExplosionTex, &v76, 8.0, 0.0, 0.0, -8.0, 255,
				                                  (v72 * 200.0), (v72 * 160.0), (v72 * 120.0), 20.0, 0, 1.0, 0, 0);
				v69 = v72 * 0.60000002;
				memset(&v74, 0, sizeof(v74));
				v68 = v72 * 0.80000001;
				CPointLights::AddLight(0, v76.x, v76.y, v76.z, 0.0, 0.0, 0.0, 8.0, v72, v68, v69, 0, 1, 0);
				v66 = v72 * 6.0;
				CCoronas::RegisterCorona(this, 0, (v72 * 255.0), (v72 * 220.0), (v72 * 190.0), 0xFFu, &v76, v66, 300.0,
				                         gpCoronaTexture, 0, 1, 0, 0, 0.0, 0, 1.5, 0, 15.0, 0, 0);
			}
			goto LABEL_126;
		}
		if (v19 == miFlare) {
			v23 = *(this + 5);
			if (v23)
				v24 = v23 + 4;
			else
				v24 = (this + 4);
			v76 = *v24;
			v75 = rand() & 0xF;
			v70 = v75 * 0.0625;
			v25 = max(v70, 0.5);
			v73 = v25;
			CShadows::StoreShadowToBeRendered(2, gpShadowExplosionTex, &v76, 8.0, 0.0, 0.0, -8.0, 255, (v25 * 200.0),
			                                  (v25 * 200.0), (v25 * 200.0), 20.0, 0, 1.0, 0, 0);
			memset(&v74, 0, sizeof(v74));
			CPointLights::AddLight(0, v76.x, v76.y, v76.z, 0.0, 0.0, 0.0, 32.0, v73, v73, v73, 0, 1, 0);
			v67 = v73 * 6.0;
			CCoronas::RegisterCorona(this, 0, (v73 * 255.0), (v73 * 255.0), (v73 * 255.0), 0xFFu, &v76, v67, 300.0,
			                         gpCoronaTexture, 0, 1, 0, 0, 0.0, 0, 1.5, 0, 15.0, 0, 0);
			goto LABEL_126;
		}
		if (!IsGlassModel(this)) {
			if ((*(this + 0x140) & 1) == 0) {
				v26 = *(this + 17);
				if (v26 == MODEL_GRENADE) {
					v28 = *(this + 5);
					v27 = v28 + 4;
					if (!v28)
						v27 = (this + 4);
					v31 = VectorScale(&outVec, &TheCamera.m_mCameraMatrix.right, 0.07);
					v32 = vectorSub(&out, v27, v31);
					fX = v32->x;
					fY = v32->y;
					v74.z = v32->z;
					if (v28)
						v33 = v28 + 4;
					else
						v33 = (this + 4);
					v35 = VectorScale(&endX, &TheCamera.m_mCameraMatrix.right, 0.07);
					pPosition2 = vectorAdd(&a1, v33, v35);
					if (sqrt(*(this + 0x11) * *(this + 0x11) + *(this + 0x12) * *(this + 0x12)) > 0.029999999)
						CMotionBlurStreaks::RegisterStreak(this, 100u, 100u, 100u, 255u, fX, fY, v74.z, pPosition2->x,
						                                   pPosition2->y, pPosition2->z);
				} else if (v26 == 344) {
					v37 = *(this + 5);
					v36 = v37 + 4;
					if (!v37)
						v36 = (this + 4);
					v39 = VectorScale(&v86, &TheCamera.m_mCameraMatrix.right, 0.07);
					v40 = vectorSub(&v79, v36, v39);
					x = v40->x;
					y = v40->y;
					z = v40->z;
					v74.x = x;
					v74.y = y;
					v74.z = z;
					v38 = v37 + 4;
					if (!v37)
						v38 = (this + 4);
					v46 = VectorScale(&v81, &TheCamera.m_mCameraMatrix.right, 0.07);
					v47 = vectorAdd(&v83, v38, v46);
					v48 = *(this + 17);
					v44 = v47->x;
					v45 = v47->y;
					v49 = *(this + 18);
					v76.z = v47->z;
					if (sqrt(v48 * v48 + v49 * v49) > 0.029999999) {
						p_x = &v37[4].x;
						if (v37) {
							v51 = &v37[4].x;
						} else {
							p_x = (this + 4);
							v51 = (this + 4);
						}
						v52 = v37 ? &v37[4] : (this + 4);
						if (!CWaterLevel::GetWaterLevelNoWaves(*v52, v51[1], p_x[2], &v75, 0i64) ||
						    ((v53 = *(this + 5)) == 0 ? (v54 = (this + 4)) : (v54 = (v53 + 48)), v54[2] > *&v75)) {
							CMotionBlurStreaks::RegisterStreak(this, 0xFFu, 0xA0u, 0x64u, 0xFFu, v74.x, v74.y, v74.z,
							                                   v44, v45, v76.z);
						}
					}
				} else if (v26 == meBeachball) {
					v56 = *(this + 5);
					v55 = v56 + 4;
					if (!v56)
						v55 = (this + 4);
					if (TheCamera.placeable.m_pMatrix)
						v57 = &TheCamera.placeable.m_pMatrix->mat.pos;
					else
						v57 = &TheCamera.placeable.m_SimpleCoors;
					v58 = vectorSub(&v85, &v57->m_vPosn, v55);
					if (CVector::Magnitude(v58) < 50.0) {
						v59 = &v56[4];
						if (!v56)
							v59 = (this + 4);
						v74 = *v59;
						CShadows::StoreShadowToBeRendered(1, gpShadowPedTex, &v74, 0.40000001, 0.0, 0.0, -0.40000001,
						                                  SLOWORD(CTimeCycle::m_nCurrentShadowStrength),
						                                  SLOBYTE(CTimeCycle::m_nCurrentShadowStrength),
						                                  SLOBYTE(CTimeCycle::m_nCurrentShadowStrength),
						                                  SLOBYTE(CTimeCycle::m_nCurrentShadowStrength), 20.0, 0, 1.0,
						                                  0, 0);
					}
				} else if (v26 == miMagnocrane5SFS || v26 == mimDynWrekingBall || v26 == miIndusMagnet ||
				           v26 == miMiniMagnet || v26 == miCjPlayerHarness) {
					v61 = *(this + 5);
					v60 = v61 + 4;
					if (!v61)
						v60 = (this + 4);
					if (TheCamera.placeable.m_pMatrix)
						v62 = &TheCamera.placeable.m_pMatrix->mat.pos;
					else
						v62 = &TheCamera.placeable.m_SimpleCoors;
					v63 = vectorSub(&v87, &v62->m_vPosn, v60);
					if (CVector::Magnitude(v63) < 110.0) {
						v64 = &v61[4];
						if (!v61)
							v64 = (this + 4);
						v74 = *v64;
						CShadows::StoreShadowToBeRendered(1, gpShadowPedTex, &v74, 2.0, 0.0, 0.0, -2.0, 128, 128, 128,
						                                  128, 50.0, 0, 1.0, 0, 0);
					}
				} else if (v26 == miBonyrdwindsock) {
					CEntity::ModifyMatrixForPoleInWind(this);
				}
				goto LABEL_126;
			}
			CPickups::DoPickUpEffects(this);
			goto LABEL_125;
		}
		CEntity::PreRenderForGlassWindow(this);
	}
LABEL_126:
	modelID = *(this + 0x11);
	if (modelID == miTrafficlight1) {
		CTrafficLights::DisplayActualLight(this);
		CShadows::StoreShadowForPole(this, 2.957, 0.147, 0.0, 16.0, 0.40000001, 0);
		return;
	}
	if (modelID == miMTraffic4)
		goto LABEL_137;
	if (modelID == miMTraffic1) {
		CTrafficLights::DisplayActualLight(this);
		CShadows::StoreShadowForPole(this, 4.8099999, 0.0, 0.0, 16.0, 0.40000001, 0);
		return;
	}
	if (modelID == miMtraffic2) {
		CTrafficLights::DisplayActualLight(this);
		CShadows::StoreShadowForPole(this, 7.5029998, 0.0, 0.0, 16.0, 0.40000001, 0);
		return;
	}
	if (modelID == miCjTrafficLight3 || modelID == miCjTrafficLight4 || modelID == miGayTrafficLight ||
	    modelID == miCjTrafficLight5) {
	LABEL_137:
		CTrafficLights::DisplayActualLight(this);
	} else if (modelID == miLamppost1) {
		CShadows::StoreShadowForPole(this, 0.74400002, 0.0, 0.0, 16.0, 0.40000001, 0);
	} else if (modelID == miLamppost2) {
		CShadows::StoreShadowForPole(this, 0.043000001, 0.0, 0.0, 16.0, 0.40000001, 0);
	} else if (modelID == miLamppost3) {
		CShadows::StoreShadowForPole(this, 1.143, 0.145, 0.0, 16.0, 0.40000001, 0);
	} else if (modelID == miDoublestreetlight1) {
		CShadows::StoreShadowForPole(this, 0.0, -0.048, 0.0, 16.0, 0.40000001, 0);
	} else if (modelID == miVgsstriplights1) {
		CTrafficLights::DisplayActualLight(this);
		CShadows::StoreShadowForPole(this, 7.5, 0.2, 0.0, 16.0, 0.40000001, 0);
	}
}