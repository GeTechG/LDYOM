---@meta

-- encode a string to gxt format
---@param text string
---@return string
function gxtEncode(text)
end

-- calculate the distance between two points
---@param x1 number
---@param y1 number
---@param z1 number
---@param x2 number
---@param y2 number
---@param z2 number
---@return number
function distanceBetweenPoints(x1, y1, z1, x2, y2, z2)
end

ld = {}

---@alias uuid userdata

---finds the index by uuid in a vector
---@generic T : userdata
---@param vector T[]
---@param uuid uuid
ld.indexByUuid = function(vector, uuid)
end

ld.loc = {}

---adds a directory to the localization system
---@param path string
ld.loc.addLocalizationDirectory = function(path)
end

---gets a localized string
---@param key string
---@return string
ld.loc.get = function(key)
end

---gets a localized array
---@param key string
---@return string[]
ld.loc.getArray = function(key)
end

time = {}

---current time in milliseconds
---@type number
time.snTimeInMilliseconds = nil