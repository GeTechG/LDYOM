#include <plugin.h>
#include <rwcore.h>
#include <rpworld.h>

#include "imgui.h"
#include "imgui_impl_rw.h"
#include <CTimer.h>

using namespace plugin;

static RwIm2DVertex* g_vertbuf     = nullptr;
static int           g_vertbufSize = 0;

// --------------------------------------------------------------------------
// Texture management (ImGui 1.92+ ImGuiBackendFlags_RendererHasTextures API)
// --------------------------------------------------------------------------

static void ImGui_ImplRW_UpdateTexture(ImTextureData* tex)
{
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        IM_ASSERT(tex->TexID == ImTextureID_Invalid);

        RwImage* image = RwImageCreate(tex->Width, tex->Height, 32);
        RwImageAllocatePixels(image);
        for (int y = 0; y < tex->Height; y++)
            memcpy(image->cpPixels + image->stride * y,
                   (const unsigned char*)tex->GetPixels() + tex->Width * 4 * y,
                   tex->Width * 4);

        RwRaster* raster = RwRasterCreate(tex->Width, tex->Height, 32, rwRASTERTYPETEXTURE);
        RwRasterSetFromImage(raster, image);
        RwImageDestroy(image);

        RwTexture* rwTex = RwTextureCreate(raster);
        RwTextureSetFilterMode(rwTex, rwFILTERLINEAR);

        tex->SetTexID((ImTextureID)rwTex);
        tex->SetStatus(ImTextureStatus_OK);
    }
    else if (tex->Status == ImTextureStatus_WantUpdates)
    {
        // Recreate the whole texture — RW doesn't expose partial raster writes easily
        RwTexture* old = (RwTexture*)tex->TexID;
        if (old) RwTextureDestroy(old);

        RwImage* image = RwImageCreate(tex->Width, tex->Height, 32);
        RwImageAllocatePixels(image);
        for (int y = 0; y < tex->Height; y++)
            memcpy(image->cpPixels + image->stride * y,
                   (const unsigned char*)tex->GetPixels() + tex->Width * 4 * y,
                   tex->Width * 4);

        RwRaster* raster = RwRasterCreate(tex->Width, tex->Height, 32, rwRASTERTYPETEXTURE);
        RwRasterSetFromImage(raster, image);
        RwImageDestroy(image);

        RwTexture* rwTex = RwTextureCreate(raster);
        RwTextureSetFilterMode(rwTex, rwFILTERLINEAR);

        tex->SetTexID((ImTextureID)rwTex);
        tex->SetStatus(ImTextureStatus_OK);
    }
    else if (tex->Status == ImTextureStatus_WantDestroy)
    {
        RwTexture* rwTex = (RwTexture*)tex->TexID;
        if (rwTex) RwTextureDestroy(rwTex);
        tex->SetTexID(ImTextureID_Invalid);
        tex->SetStatus(ImTextureStatus_Destroyed);
    }
}

// --------------------------------------------------------------------------
// Rendering
// --------------------------------------------------------------------------

void ImGui_ImplRW_RenderDrawData(ImDrawData* draw_data)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f)
        return;

    // Catch up with pending texture create/update/destroy requests
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                ImGui_ImplRW_UpdateTexture(tex);

    // Grow vertex buffer if needed
    if (g_vertbuf == nullptr || g_vertbufSize < draw_data->TotalVtxCount) {
        if (g_vertbuf) RwFree(g_vertbuf);
        g_vertbufSize = draw_data->TotalVtxCount + 5000;
        g_vertbuf = (RwIm2DVertex*)RwMalloc(sizeof(RwIm2DVertex) * g_vertbufSize, 0);
    }

    float xoff = 0.0f, yoff = 0.0f;
#ifdef RWHALFPIXEL
    xoff = -0.5f; yoff = 0.5f;
#endif

    RwCamera* cam  = (RwCamera*)RwCameraGetCurrentCamera();
    float recipZ   = 1.0f / RwCameraGetNearClipPlane(cam);
    RwIm2DVertex* vtx_dst = g_vertbuf;

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_src  = cmd_list->VtxBuffer.Data;
        for (int i = 0; i < cmd_list->VtxBuffer.Size; i++) {
            RwIm2DVertexSetScreenX(&vtx_dst[i], vtx_src[i].pos.x + xoff);
            RwIm2DVertexSetScreenY(&vtx_dst[i], vtx_src[i].pos.y + yoff);
            RwIm2DVertexSetScreenZ(&vtx_dst[i], RwIm2DGetNearScreenZ());
            RwIm2DVertexSetCameraZ(&vtx_dst[i], RwCameraGetNearClipPlane(cam));
            RwIm2DVertexSetRecipCameraZ(&vtx_dst[i], recipZ);

            // Premultiplied alpha fix
            unsigned int col = vtx_src[i].col;
            float alpha = ((col >> 24) & 0xFF) / 255.0f;
            RwIm2DVertexSetIntRGBA(&vtx_dst[i],
                (int)((col & 0xFF)        * alpha),
                (int)(((col >> 8)  & 0xFF) * alpha),
                (int)(((col >> 16) & 0xFF) * alpha),
                (col >> 24) & 0xFF);

            RwIm2DVertexSetU(&vtx_dst[i], vtx_src[i].uv.x, recipZ);
            RwIm2DVertexSetV(&vtx_dst[i], vtx_src[i].uv.y, recipZ);
        }
        vtx_dst += cmd_list->VtxBuffer.Size;
    }

    // Save render states
    void* savedTex;
    int vertexAlpha, srcBlend, dstBlend, ztest, addrU, addrV, filter, cullmode, shadeMode;
    RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &vertexAlpha);
    RwRenderStateGet(rwRENDERSTATESRCBLEND,          &srcBlend);
    RwRenderStateGet(rwRENDERSTATEDESTBLEND,         &dstBlend);
    RwRenderStateGet(rwRENDERSTATEZTESTENABLE,       &ztest);
    RwRenderStateGet(rwRENDERSTATETEXTURERASTER,     &savedTex);
    RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSU,   &addrU);
    RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSV,   &addrV);
    RwRenderStateGet(rwRENDERSTATETEXTUREFILTER,     &filter);
    RwRenderStateGet(rwRENDERSTATECULLMODE,          &cullmode);
    RwRenderStateGet(rwRENDERSTATESHADEMODE,         &shadeMode);

    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND,          (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,         (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE,       (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATECULLMODE,          (void*)rwCULLMODECULLNONE);
    RwRenderStateSet(rwRENDERSTATESHADEMODE,         (void*)rwSHADEMODEGOURAUD);

    int vtx_offset = 0;
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        int idx_offset = 0;
        for (int i = 0; i < cmd_list->CmdBuffer.Size; i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                RwTexture* tex = (RwTexture*)pcmd->GetTexID();
                if (tex && tex->raster) {
                    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, tex->raster);
                } else {
                    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
                }
                RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST,
                    g_vertbuf + vtx_offset, cmd_list->VtxBuffer.Size,
                    cmd_list->IdxBuffer.Data + idx_offset, pcmd->ElemCount);
            }
            idx_offset += pcmd->ElemCount;
        }
        vtx_offset += cmd_list->VtxBuffer.Size;
    }

    // Restore render states
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)vertexAlpha);
    RwRenderStateSet(rwRENDERSTATESRCBLEND,          (void*)srcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,         (void*)dstBlend);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE,       (void*)ztest);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER,     savedTex);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSU,   (void*)addrU);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSV,   (void*)addrV);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER,     (void*)filter);
    RwRenderStateSet(rwRENDERSTATECULLMODE,          (void*)cullmode);
    RwRenderStateSet(rwRENDERSTATESHADEMODE,         (void*)shadeMode);
}

// --------------------------------------------------------------------------
// Init / Shutdown / NewFrame
// --------------------------------------------------------------------------

bool ImGui_ImplRW_Init()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_rw";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Renderer_TextureMaxWidth  = 4096;
    platform_io.Renderer_TextureMaxHeight = 4096;

    return true;
}

void ImGui_ImplRW_Shutdown()
{
    // Destroy all managed textures
    for (ImTextureData* tex : ImGui::GetPlatformIO().Textures)
        if (tex->RefCount == 1) {
            tex->SetStatus(ImTextureStatus_WantDestroy);
            ImGui_ImplRW_UpdateTexture(tex);
        }

    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = nullptr;
    io.BackendFlags &= ~ImGuiBackendFlags_RendererHasTextures;
    ImGui::GetPlatformIO().ClearRendererHandlers();

    if (g_vertbuf) {
        RwFree(g_vertbuf);
        g_vertbuf     = nullptr;
        g_vertbufSize = 0;
    }
}

void ImGui_ImplRW_InvalidateDeviceObjects()
{
    for (ImTextureData* tex : ImGui::GetPlatformIO().Textures)
        if (tex->RefCount == 1) {
            tex->SetStatus(ImTextureStatus_WantDestroy);
            ImGui_ImplRW_UpdateTexture(tex);
        }
}

bool ImGui_ImplRW_CreateDeviceObjects()
{
    // Textures are created lazily via UpdateTexture — nothing to do here
    return true;
}

void ImGui_ImplRW_NewFrame()
{
    // Override display size and delta time with game values
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)plugin::screen::GetScreenWidth(),
                            (float)plugin::screen::GetScreenHeight());
    io.DeltaTime   = CTimer::ms_fTimeStepNonClipped;
}
