---@meta

---Applies a highlighting effect to the rendering of a specified function if a given condition is true.
---
--- This function is designed to modify the rendering of a specified function by applying a highlighting effect
--- when the specified condition is true. The highlighting effect involves changing the background color of the
--- rendering to a blend of the original color and a specified blend color with reduced alpha transparency.
---
--- @param cond bool boolean condition that determines whether the highlighting effect should be applied.
--- @param render fun() function that renders the highlighted content.
ImGui.IncorrectHighlight = function (cond, callback)
    
end