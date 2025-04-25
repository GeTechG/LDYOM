#ifndef IMGUI_HOOK_H
#define IMGUI_HOOK_H

#include <functional>

void HookImgui(std::function<void()> renderFunc);
void UnhookImgui();
void MouseShow(bool show);
void ControlEnabled(bool enable);

#endif // IMGUI_HOOK_H