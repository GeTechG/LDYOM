---@meta

loc = {}

---Translate a key with optional placeholder arguments
---@param key string The translation key (dot-separated, e.g. "objectives.checkpoint.name")
---@vararg string Placeholder values substituted into the translation string
---@return string
function loc.trv(key, ...) end

---Translate a key with plural form selection and optional placeholder arguments
---@param key string The translation key (dot-separated)
---@param count integer The count used to select singular/plural form
---@vararg string Placeholder values substituted into the translation string
---@return string
function loc.trPluralv(key, count, ...) end
