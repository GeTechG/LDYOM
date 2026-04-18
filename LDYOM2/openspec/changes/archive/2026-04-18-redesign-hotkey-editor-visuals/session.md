---
change: redesign-hotkey-editor-visuals
updated: 2026-04-17T00:00:00Z
status: completed
---

## In-Flight
none

## Decisions
- [task 1.2] Default style instance named `DefaultKeyboardStyle` (matches existing capitalization convention).
- [task 2.4] Face rect bottom edge uses `FaceInset * 2.0f` on Y-axis for raised-keycap bevel.
- [task 1.3] `scaleFactor`/`borderThick`/`faceInset` recomputed per-key inside the loop.
- [task 3.2] `recordedThisFrame` is a non-static local `bool[512]` inside `Edit()`, zero-initialized every call.
- [task 3.2] State precedence: Recorded > Pressed > Normal. Recorded shows for exactly one frame (transient flash).
- [task 3.2] Click-toggle path does NOT set `recordedThisFrame` — only physical key presses do.
- Color values: KeyNormal=IM_COL32(45,45,50,255); KeyPressed=IM_COL32(180,100,40,255) warm amber; KeyRecorded=IM_COL32(40,110,180,255) cool blue.
- [task 4.x] All icons use `size = max(4.0f, faceRect.GetHeight() * 0.4f)` and `faceRect.GetCenter()` as anchor.
- [task 4.3] Shift uses identical upward triangle as Up-arrow (not a two-stroke chevron) for visual consistency.
- [task 4.7] Min-size check is the FIRST thing inside `DrawKeyIcon` — returns false to fall back to text path.
- [task 5.1] Text overflow threshold: `CalcTextSize.x > faceRect.GetWidth() - 4.0f` (2px inset each side); clip rect uses `intersect_with_current = true`.
- [task 5.2] Abbreviation table left as TODO comment only — math at min popup size (960×400) shows Backspace fits; clip catches edge cases.

## Gotchas
- LSP shows `ImU32`/`ImSwap`/`ImFormatStringV` as undeclared — false positives cascading from `spdlog/spdlog.h not found`. `cmake --build Debug` is the source of truth and succeeds clean.
- Both "Shift" entries in the Keys table (left 0x2A and right 0xE5) share `lib = "Shift"` so both naturally get the chevron icon.

## Open Questions

## Next Steps
- All tasks complete. User confirmed in-game validation passed (2026-04-17). Ready to archive via `/openspec-archive-change redesign-hotkey-editor-visuals`.
