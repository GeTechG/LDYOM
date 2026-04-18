---
change: improve-hotkey-editor-ux-and-theme
updated: 2026-04-19T00:00:00Z
status: completed
---

## In-Flight
none

## Decisions
- KeyHover fill-color precedence: Recorded > Pressed > Hover > Normal.
- KeyRecorded = ImGuiCol_CheckMark; KeyPressed = ImGuiCol_Header.
- [4.4] `recordingFirstKey` local one-shot defers memset(keyDown) until first key arrives.
- [4.5] Modifier set: Left/Right Ctrl/Alt/Shift only. Removed ImGuiKey_Mod* virtuals — they don't exist in this project's ImGui version.
- [4.8] Outline rect from GetItemRectMin/Max after last-row EndGroup — covers bottom row only.
- [6.3] PackKeyDown(const bool[512]) file-scope helper; Set path reuses it.
- [6.6] unsaved_discard is BeginPopup (not Modal) — anchors near Selectable.
- [7.1] Conflict detection two-pass: counts saved-entry shares; also flags when live unsaved value collides with a saved entry.
- [7.3] Only first conflicting peer name shown — ImGui renders one tooltip per item.
- [8.1] Icon-set tooltip check uses strcmp against a file-scope static const char* array.
- [9] Two-line Selectable with AllowOverlap, cursor rewind + manual advance, conflict bullet re-centered on name line via GetTextLineHeight().
- [11.2] ru.json is a sparse overlay — added hotkey_editor section with only the 9 new keys.

## Gotchas
- ImGuiCol_Button default alpha=0.40 — BuildKeyboardStyleFromTheme forces alpha=1.0 on opaque colors.
- _() returns std::string — never chain .c_str() on temporaries; store in local first.
- Clang LSP shows spdlog/ImU32 errors in imHotKey.h — pre-existing Conan+MSVC noise; real MSVC build is clean.
- [10.1] Escape check fires even if unsaved_discard is open (non-modal). Acceptable for current behavior.
- [9] Capture GetItemRectMin/Max on the Selectable *before* cursor rewind.

## Open Questions

## Next Steps
Archive via `/openspec-archive-change improve-hotkey-editor-ux-and-theme`.
