---@meta

---@class ldyom
ldyom = {}

---Push a toast notification.
---@param type "success"|"info"|"warning"|"error"
---@param title string
---@param message string
---@param duration_ms? integer Dismiss time in ms (default 3000)
---@overload fun(type: "success"|"info"|"warning"|"error", title: string, message: string)
---@overload fun(type: "success"|"info"|"warning"|"error", title: string, message: string, duration_ms: integer)
function ldyom.notify(type, title, message, duration_ms) end
