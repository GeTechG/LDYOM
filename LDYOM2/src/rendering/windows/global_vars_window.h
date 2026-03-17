#pragma once
#include "modal_popup_window.h"
#include <string>

class GlobalVarsWindow : public ModalPopupWindow {
    static void renderContent(GlobalVarsWindow* window);

    int         m_pendingDeleteIndex = -1;
    std::string m_newVarName;

  public:
    GlobalVarsWindow();
    ~GlobalVarsWindow() override = default;
};
