#include <plugin.h>
#include <rwcore.h>
#include <rpworld.h>
#include <common.h>
#include <d3d9.h>

#include "imgui.h"
#include "imgui_impl_rw.h"
#include <CTimer.h>

using namespace plugin;

// Backend data
struct ImGui_ImplRW_Data
{
    RwIm2DVertex* VertexBuffer;
    int           VertexBufferSize;

    ImGui_ImplRW_Data() { memset((void*)this, 0, sizeof(*this)); }
};

static ImGui_ImplRW_Data* ImGui_ImplRW_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplRW_Data*)ImGui::GetIO().BackendRendererUserData : nullptr;
}

// --------------------------------------------------------------------------
// Texture management (ImGui 1.92+ ImGuiBackendFlags_RendererHasTextures API)
// --------------------------------------------------------------------------

static void ImGui_ImplRW_CreateOrUpdateTexture(ImTextureData* tex)
{
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

static void ImGui_ImplRW_UpdateTexture(ImTextureData* tex)
{
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        IM_ASSERT(tex->TexID == ImTextureID_Invalid);
        ImGui_ImplRW_CreateOrUpdateTexture(tex);
    }
    else if (tex->Status == ImTextureStatus_WantUpdates)
    {
        // Recreate the whole texture — RW doesn't expose partial raster writes easily
        RwTexture* old = (RwTexture*)tex->TexID;
        if (old) RwTextureDestroy(old);
        ImGui_ImplRW_CreateOrUpdateTexture(tex);
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
// Render state setup
// --------------------------------------------------------------------------

static void ImGui_ImplRW_SetupRenderState(IDirect3DDevice9* device)
{
    // All states managed by RW go through RwRenderStateSet to keep the RW cache in sync.
    // Only scissor test is set via D3D directly — RW doesn't manage it.
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND,          (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,         (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE,       (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,      (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE,         (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATECULLMODE,          (void*)rwCULLMODECULLNONE);
    RwRenderStateSet(rwRENDERSTATESHADEMODE,         (void*)rwSHADEMODEGOURAUD);
    RwRenderStateSet(rwRENDERSTATESTENCILENABLE,     (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSU,   (void*)rwTEXTUREADDRESSCLAMP);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSV,   (void*)rwTEXTUREADDRESSCLAMP);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER,     (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);

    // Scissor test — not managed by RW, must go through D3D directly
    if (device)
        device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
}

// --------------------------------------------------------------------------
// Rendering
// --------------------------------------------------------------------------

void ImGui_ImplRW_RenderDrawData(ImDrawData* draw_data)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f)
        return;

    ImGui_ImplRW_Data* bd = ImGui_ImplRW_GetBackendData();
    IM_ASSERT(bd != nullptr && "Backend not initialized!");

    // Catch up with pending texture create/update/destroy requests
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                ImGui_ImplRW_UpdateTexture(tex);

    if (draw_data->TotalVtxCount == 0)
        return;

    // Grow vertex buffer if needed
    if (bd->VertexBuffer == nullptr || bd->VertexBufferSize < draw_data->TotalVtxCount)
    {
        if (bd->VertexBuffer) RwFree(bd->VertexBuffer);
        bd->VertexBufferSize = draw_data->TotalVtxCount + 5000;
        bd->VertexBuffer = (RwIm2DVertex*)RwMalloc(sizeof(RwIm2DVertex) * bd->VertexBufferSize, 0);
    }

    float xoff = 0.0f, yoff = 0.0f;
#ifdef RWHALFPIXEL
    xoff = -0.5f; yoff = 0.5f;
#endif

    RwCamera* cam  = (RwCamera*)RwCameraGetCurrentCamera();
    float nearZ    = RwCameraGetNearClipPlane(cam);
    float recipZ   = 1.0f / nearZ;
    float screenZ  = RwIm2DGetNearScreenZ();
    RwIm2DVertex* vtx_dst = bd->VertexBuffer;

    // Convert all vertices into RW format
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_src  = cmd_list->VtxBuffer.Data;
        for (int i = 0; i < cmd_list->VtxBuffer.Size; i++)
        {
            RwIm2DVertexSetScreenX(&vtx_dst[i], vtx_src[i].pos.x + xoff);
            RwIm2DVertexSetScreenY(&vtx_dst[i], vtx_src[i].pos.y + yoff);
            RwIm2DVertexSetScreenZ(&vtx_dst[i], screenZ);
            RwIm2DVertexSetCameraZ(&vtx_dst[i], nearZ);
            RwIm2DVertexSetRecipCameraZ(&vtx_dst[i], recipZ);

            unsigned int col = vtx_src[i].col;
            RwIm2DVertexSetIntRGBA(&vtx_dst[i],
                (col      ) & 0xFF,
                (col >>  8) & 0xFF,
                (col >> 16) & 0xFF,
                (col >> 24) & 0xFF);

            RwIm2DVertexSetU(&vtx_dst[i], vtx_src[i].uv.x, recipZ);
            RwIm2DVertexSetV(&vtx_dst[i], vtx_src[i].uv.y, recipZ);
        }
        vtx_dst += cmd_list->VtxBuffer.Size;
    }

    // Get D3D device for scissor rect and state block
    IDirect3DDevice9* device = (IDirect3DDevice9*)GetD3DDevice();

    // Save RW render states
    int savedVertexAlpha, savedSrcBlend, savedDstBlend, savedZTest, savedZWrite;
    int savedFog, savedCullMode, savedShadeMode, savedStencil;
    int savedAddrU, savedAddrV, savedFilter, savedAlphaTestFunc;
    void* savedTexRaster;
    RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedVertexAlpha);
    RwRenderStateGet(rwRENDERSTATESRCBLEND,          &savedSrcBlend);
    RwRenderStateGet(rwRENDERSTATEDESTBLEND,         &savedDstBlend);
    RwRenderStateGet(rwRENDERSTATEZTESTENABLE,       &savedZTest);
    RwRenderStateGet(rwRENDERSTATEZWRITEENABLE,      &savedZWrite);
    RwRenderStateGet(rwRENDERSTATEFOGENABLE,         &savedFog);
    RwRenderStateGet(rwRENDERSTATECULLMODE,          &savedCullMode);
    RwRenderStateGet(rwRENDERSTATESHADEMODE,         &savedShadeMode);
    RwRenderStateGet(rwRENDERSTATESTENCILENABLE,     &savedStencil);
    RwRenderStateGet(rwRENDERSTATETEXTURERASTER,     &savedTexRaster);
    RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSU,   &savedAddrU);
    RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSV,   &savedAddrV);
    RwRenderStateGet(rwRENDERSTATETEXTUREFILTER,     &savedFilter);
    RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &savedAlphaTestFunc);

    // Save D3D scissor state (only D3D state we touch directly — RW doesn't manage it)
    DWORD savedScissorTest = FALSE;
    RECT  savedScissorRect = {};
    if (device)
    {
        device->GetRenderState(D3DRS_SCISSORTESTENABLE, &savedScissorTest);
        device->GetScissorRect(&savedScissorRect);
    }

    // Set render state for ImGui
    ImGui_ImplRW_SetupRenderState(device);

    // Render command lists
    // (Because we merged all vertices into a single buffer, we maintain our own offset into them)
    int global_vtx_offset = 0;
    ImVec2 clip_off = draw_data->DisplayPos;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        for (int i = 0; i < cmd_list->CmdBuffer.Size; i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[i];
            if (pcmd->UserCallback != nullptr)
            {
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplRW_SetupRenderState(device);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
                ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                // Apply scissor rect via D3D device
                if (device)
                {
                    const RECT r = { (LONG)clip_min.x, (LONG)clip_min.y, (LONG)clip_max.x, (LONG)clip_max.y };
                    device->SetScissorRect(&r);
                }

                // Bind texture
                RwTexture* tex = (RwTexture*)pcmd->GetTexID();
                if (tex && tex->raster)
                    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, tex->raster);
                else
                    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);

                // Draw — use pcmd->IdxOffset (not a sequential accumulator) because
                // ImGui may reorder commands (e.g. modal dim background via push_front)
                RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST,
                    bd->VertexBuffer + global_vtx_offset, cmd_list->VtxBuffer.Size,
                    cmd_list->IdxBuffer.Data + pcmd->IdxOffset, pcmd->ElemCount);
            }
        }
        global_vtx_offset += cmd_list->VtxBuffer.Size;
    }

    // Restore RW render states
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)(uintptr_t)savedVertexAlpha);
    RwRenderStateSet(rwRENDERSTATESRCBLEND,          (void*)(uintptr_t)savedSrcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,         (void*)(uintptr_t)savedDstBlend);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE,       (void*)(uintptr_t)savedZTest);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,      (void*)(uintptr_t)savedZWrite);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE,         (void*)(uintptr_t)savedFog);
    RwRenderStateSet(rwRENDERSTATECULLMODE,          (void*)(uintptr_t)savedCullMode);
    RwRenderStateSet(rwRENDERSTATESHADEMODE,         (void*)(uintptr_t)savedShadeMode);
    RwRenderStateSet(rwRENDERSTATESTENCILENABLE,     (void*)(uintptr_t)savedStencil);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER,     savedTexRaster);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSU,   (void*)(uintptr_t)savedAddrU);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSV,   (void*)(uintptr_t)savedAddrV);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER,     (void*)(uintptr_t)savedFilter);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)(uintptr_t)savedAlphaTestFunc);

    // Restore D3D scissor state
    if (device)
    {
        device->SetRenderState(D3DRS_SCISSORTESTENABLE, savedScissorTest);
        device->SetScissorRect(&savedScissorRect);
    }
}

// --------------------------------------------------------------------------
// Init / Shutdown / NewFrame
// --------------------------------------------------------------------------

bool ImGui_ImplRW_Init()
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");

    ImGui_ImplRW_Data* bd = IM_NEW(ImGui_ImplRW_Data)();
    io.BackendRendererUserData = (void*)bd;
    io.BackendRendererName = "imgui_impl_rw";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Renderer_TextureMaxWidth  = 4096;
    platform_io.Renderer_TextureMaxHeight = 4096;

    return true;
}

void ImGui_ImplRW_Shutdown()
{
    ImGui_ImplRW_Data* bd = ImGui_ImplRW_GetBackendData();
    IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplRW_InvalidateDeviceObjects();

    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendFlags &= ~ImGuiBackendFlags_RendererHasTextures;
    ImGui::GetPlatformIO().ClearRendererHandlers();

    IM_DELETE(bd);
}

void ImGui_ImplRW_InvalidateDeviceObjects()
{
    ImGui_ImplRW_Data* bd = ImGui_ImplRW_GetBackendData();
    if (!bd) return;

    for (ImTextureData* tex : ImGui::GetPlatformIO().Textures)
        if (tex->RefCount == 1)
        {
            tex->SetStatus(ImTextureStatus_WantDestroy);
            ImGui_ImplRW_UpdateTexture(tex);
        }

    if (bd->VertexBuffer)
    {
        RwFree(bd->VertexBuffer);
        bd->VertexBuffer     = nullptr;
        bd->VertexBufferSize = 0;
    }
}

bool ImGui_ImplRW_CreateDeviceObjects()
{
    // Textures are created lazily via UpdateTexture — nothing to do here
    return true;
}

void ImGui_ImplRW_NewFrame()
{
    ImGui_ImplRW_Data* bd = ImGui_ImplRW_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplRW_Init()?");
    IM_UNUSED(bd);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)plugin::screen::GetScreenWidth(),
                            (float)plugin::screen::GetScreenHeight());
    io.DeltaTime   = CTimer::ms_fTimeStepNonClipped;
}
