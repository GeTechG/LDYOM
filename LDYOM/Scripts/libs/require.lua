req = {}

setmetatable(req, {__call = function(_,path) return require(path) end})

-- req.tree private functions
--
local cache = {}

local function toFSPath(requirePath) return requirePath:gsub("%.", "/") end
local function toRequirePath(fsPath) return fsPath:gsub('/','.') end
local function noExtension(path)     return path:gsub('%.lua$', '') end
local function noEndDot(str)         return str:gsub('%.$', '') end

local function scandir(directory)
  local t = {}
  for line in io.popen("dir \"LDYOM/Scripts/"..directory.."\" /a /b", "r"):lines() do
    t[#t+1] = line;
  end
  return t;
end

local function directory_exist(dir_path)
  local f = io.popen('[ -d "LDYOM/Scripts/' .. dir_path .. '" ] && echo -n y')
  local result = f:read(1)
  f:close()
  return result == "y"
end


function req.tree(requirePath)
  if not cache[requirePath] then
    local result = {}

    local fsPath = toFSPath(requirePath)
    local entries = Dirs.entries(fsPath)

    for _,entry in ipairs(entries) do
      fsPath = toFSPath(requirePath .. '.' .. entry)
      if Dirs.exists(fsPath) then
        result[entry] = req.tree(toRequirePath(fsPath))
      else
        entry = noExtension(entry)
        result[entry] = req(toRequirePath(requirePath .. '/' .. entry))
      end
    end

    cache[requirePath] = result
  end

  return cache[requirePath]
end

function req.path(filePath)
  return noEndDot(noExtension(filePath):match("(.-)[^%.]*$"))
end

function req.relative(...)
  local args = {...}
  local first, last = args[1], args[#args]
  local path = req.path(first)
  return req(path .. '.' .. last)
end


