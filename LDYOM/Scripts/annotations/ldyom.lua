---@meta

---@module "libs.uuid"
uuid = {}
---@module "dkjson"
json = {}

---@generic T
---@param t T
---@return T
table.deepcopy = function(t)
end

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


---@param ptr CPed
---@return integer
function GetPedRef(ptr)
end

---@param ptr CVehicle
---@return integer
function GetVehicleRef(ptr)
end

---@param ptr CObject
---@return integer
function GetObjectRef(ptr)
end


function emitSignal()
end

---@param posn number[]
---@param out number[]
---@param checkMaxVisible boolean
---@param checkMinVisible boolean
---@return boolean
---@return float w
---@return float h
function CalcScreenCoors(posn, out, checkMaxVisible, checkMinVisible)
end


---@enum MathCondition
MathCondition = {
    Equal = 0,
    NotEqual = 1,
    More = 2,
    MoreOrEqual = 3,
    Less = 4,
    LessOrEqual = 5
}

---@class ScriptTable
---@field name string
---@field id string
---@field init function

---@type { [string]: ScriptTable }
scripts = {}



ld = {}

---@alias uuid userdata

---finds the index by uuid in a vector
---@generic T : userdata
---@param vector T[]
---@param uuid string
---@return integer
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








ld.window = {}

---@param drawFunction function
ld.window.openLuaWindow = function(drawFunction)
end

ld.window.closeLuaWindow = function()
end

ld.window.closeMainMenu = function()
end

ld.window.openMainMenu = function()
end

---@param executeFunc function
---@param undoFunc function
ld.window.replaceWindow = function(executeFunc, undoFunc)
end

---@param name string
---@param drawFunction function
ld.window.setMainMenuRender = function(name, drawFunction)
end

---@param name string
ld.window.removeMainMenuRender = function(name)
end

---@return boolean
ld.window.getLuaWindowState = function () end

---@param state boolean
ld.window.setLuaWindowState = function (state) end

---@param state boolean
ld.window.setRenderWindows = function (state) end

---@param name string
---@param drawFunction function
ld.window.addRender = function (name, drawFunction) end

---@param name string
ld.window.removeRender = function (name) end






ld.events = {}
---@type function[]
ld.events.onStartProject = {}
---@type fun(scene: LDScene, tasklist: ktcoro_tasklist)[]
ld.events.onStartScene = {}
---@type fun(tasklist: ktcoro_tasklist)[]
ld.events.mainLoop = {}
---@type fun(projectDirectory: string)[]
ld.events.onSaveProject = {}
---@type fun(projectDirectory: string)[]
ld.events.onLoadProject = {}
---@type fun(objective: LDObjective, ktcoro_tasklist: ktcoro_tasklist)[]
ld.events.onStartObjective = {}




---@enum GlobalVariableType
GlobalVariableType = {
    Float = 0,
    Int = 1,
    String = 2,
    Bool = 3
}

---@class GlobalVariable
---@field type GlobalVariableType
---@field name string
---@field uuid string
---@field value any


ld.data = {}
---@type GlobalVariable[]
ld.data.globalVariables = {}




---@class LDProjectPlayerService
LDProjectPlayerService = {
    ---@return LDScene
    getCurrentScene = function(self) end,
    ---@return LDObjective
    getCurrentObjective = function(self) end,
    ---@param scene integer
    setNextScene = function(self, scene) end,
    ---@param objective integer
    setNextObjective = function(self, objective) end,
    stopCurrentScene = function(self) end,
    ---@return ktcoro_tasklist
    getSceneTasklist = function(self) end,
}

---@type LDProjectPlayerService
ld.projectPlayerService = nil


---@class LDTasker
LDTasker = {
    ---@param name string
    ---@param task function
    addTask = function(self, name, task) end,
    ---@param name string
    removeTask = function(self, name) end,
}


---@type LDTasker
ld.tasker = {}




---@class LDHotKey
---@field functionName string
---@field functionKeys integer



---@class LDHotKeyService
LDHotKeyService = {
    ---@param name string
    ---@return LDHotKey
    getHotKeyByName = function(self, name) end,
    ---@param name string
    stringToKeyCombo = function(self, name) end,
    ---@param repeat_ boolean
    ---@return LDHotKey
    getHotKey = function(self, repeat_) end,
}


---@type LDHotKeyService
ld.hotkeyService = {}

---@param functionKeys integer
---@return string
ld.GetHotKeyLib = function(functionKeys)
end


KeyCheck = {
    Update = function () end,
    ---@param key integer
    ---@return boolean
    CheckJustDown = function (key) end,
}


ld.models = {
    ---@return integer[]
    getPedModels = function() end,
    ---@return integer[]
    getWeaponIds = function() end,
    ---@return string[]
    getSpecialsPed = function() end,
    ---@return integer[]
    getVehicleModels = function() end,
    ---@return string[]
    getParticlesNames = function() end,
    ---@return { [integer]: string }
    getPedBones = function() end,
    ---@return string[]
    getPacksNames = function() end,
    ---@return { [string]: string[] }
    getAnimations = function() end,
    ---@return integer[]
    getTextStyles = function() end,
}




data = {}
data.playing = {}
---@type {[string]: any}
data.playing.globalVariables = {}




---@alias CPed userdata
---@alias CVehicle userdata
---@alias CObject userdata
---@alias CFire userdata
---@alias CQuaternion userdata


---@class ktcoro_tasklist
ktcoro_tasklist = {
    ---@param task function
    add_task = function(self, task) end,
}

---@class LDCheckpoint
LDCheckpoint = {
    ---@return uuid
    getUuid = function() end,
    ---@return userdata | nil
    getEditorCheckpoint = function() end,
    ---@return userdata | nil
    getProjectCheckpoint = function() end,
    ---@return integer | nil
    getEditorSphere = function() end,
    ---@return integer | nil
    getProjectSphere = function() end,
    updateLocation = function() end,
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return integer
    getType = function() end,
    ---@return number
    getRadius = function() end,
    ---@return integer
    getBlipColor = function() end,
    ---@return number[]
    getColor = function() end,
    ---@return integer
    getCheckpointType = function() end,
    ---@return integer
    getBlipType = function() end,
    ---@return integer
    getBlipSprite = function() end,
    ---@return number
    getAngle = function() end,
    spawnEditorCheckpoint = function() end,
    deleteEditorCheckpoint = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    spawnEditorBlip = function() end,
    deleteEditorBlip = function() end,
    spawnProjectBlip = function() end,
    deleteProjectBlip = function() end,
    drawSphere = function() end,
    existProjectEntity = function() end,
    ---@return number[]
    getProjectEntityPosition = function() end,
    ---@return LDCheckpoint
    copy = function() end,
}

---@class LDVisualEffect
LDVisualEffect = {
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return uuid
    getUuid = function() end,
    ---@return integer
    getType = function() end,
    ---@return number
    getSize = function() end,
    ---@return integer
    getEffectType = function() end,
    ---@return integer
    getFlare = function() end,
    ---@return number[]
    getColor = function() end,
    ---@return boolean
    isDrawing = function() end,
    ---@return number
    getAngle = function() end,
    spawnEditorVisualEffect = function() end,
    deleteEditorVisualEffect = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    drawVisualEffect = function() end,
    ---@return LDVisualEffect
    copy = function() end,
}

---@class LDAudio
LDAudio = {
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return boolean
    isAudio3D = function() end,
    ---@return integer
    getAttachType3d = function() end,
    ---@return uuid
    getAttachUuid = function() end,
    ---@return boolean
    isIsLooped = function() end,
    ---@return number
    getVolume = function() end,
    ---@return uuid
    getUuid = function() end,
    updateLocation = function() end,
    ---@return SignalVoid
    getSignalDeleteAudio = function() end,
    spawnEditorAudio = function() end,
    deleteEditorAudio = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    ---@return LDAudio
    copy = function() end,
}

---@class LDPyrotechnics
LDPyrotechnics = {
    ---@return LDPyrotechnics
    copy = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return CFire
    getEditorFire = function() end,
    ---@return CFire
    getProjectFire = function() end,
    ---@return integer
    getType = function() end,
    ---@return integer
    getTypeExplosion = function() end,
    ---@return integer
    getSizeFire = function() end,
    ---@return integer
    getPropagationFire = function() end,
    ---@return integer
    getProjectFireId = function() end,
    updateLocation = function() end,
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    spawnEditorPyrotechnics = function() end,
    deleteEditorPyrotechnics = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
}


---@class LDPickup
LDPickup = {
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return integer
    getModelId = function() end,
    ---@return integer
    getType = function() end,
    ---@return integer
    getSpawnType = function() end,
    ---@return integer
    getWeapon = function() end,
    ---@return integer
    getAmmo = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return integer
    getProjectPickupIndex = function() end,
    ---@return integer
    getEditorPickupIndex = function() end,
    updateLocation = function() end,
    ---@return SignalVoid
    getSignalDeletePickup = function() end,
    spawnEditorPickup = function() end,
    deleteEditorPickup = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    ---@return LDPickup
    copy = function() end,
}

---@class LDTrain
LDTrain = {
    ---@return LDTrain
    copy = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return userdata | nil
    getEditorTrain = function() end,
    ---@return userdata | nil
    getProjectTrain = function() end,
    ---@return boolean
    isRotate = function() end,
    ---@return integer
    getHealth = function() end,
    ---@return integer
    getTrainType = function() end,
    ---@return number
    getSpeed = function() end,
    ---@return number
    getCruiseSpeed = function() end,
    updateLocation = function() end,
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    spawnEditorTrain = function() end,
    deleteEditorTrain = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
}

---@class LDParticle
LDParticle = {
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return CQuaternion
    getRotations = function() end,
    ---@return number[]
    getScale = function() end,
    ---@return integer
    getParticleType = function() end,
    ---@return integer
    getAttachType = function() end,
    ---@return uuid
    getAttachUuid = function() end,
    ---@return integer
    getPedBodeId = function() end,
    ---@return integer
    getProjectParticleId = function() end,
    updateLocation = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return SignalVoid
    getSignalDeleteParticle = function() end,
    spawnEditorParticle = function() end,
    deleteEditorParticle = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    ---@return LDParticle
    copy = function() end,
}

---@class LDObject
LDObject = {
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return CObject
    getEditorObject = function() end,
    ---@return CObject
    getProjectObject = function() end,
    ---@return CQuaternion
    getRotations = function() end,
    ---@return number[]
    getScale = function() end,
    ---@return integer
    getModelId = function() end,
    updateLocation = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return SignalVoid
    getSignalDeleteObject = function() end,
    spawnEditorObject = function() end,
    deleteEditorObject = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    ---@return LDObject
    copy = function() end,
}

---@class LDObjective
LDObjective = {
    ---@return string
    getName = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return integer
    getCategory = function() end,
    ---@return integer
    getTypeCategory = function() end,
    open = function() end,
    close = function() end,
    ---@param scene LDScene
    ---@param result LDResult
    ---@param tasklist ktcoro_tasklist
    ---@return ktwait
    execute = function(scene, result, tasklist) end,
}


---@class LDVehicle
LDVehicle = {
    ---@return LDVehicle
    copy = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return userdata | nil
    getEditorVehicle = function() end,
    ---@return userdata | nil
    getProjectVehicle = function() end,
    ---@return integer
    getModelId = function() end,
    ---@return boolean
    isShouldNotDie = function() end,
    ---@return integer
    getHealth = function() end,
    ---@return boolean
    isBulletproof = function() end,
    ---@return boolean
    isFireproof = function() end,
    ---@return boolean
    isExplosionproof = function() end,
    ---@return boolean
    isCollisionproof = function() end,
    ---@return boolean
    isMeleeproof = function() end,
    ---@return boolean
    isTiresVulnerability = function() end,
    ---@return boolean
    isExtendedColor = function() end,
    ---@return boolean
    isLocked = function() end,
    ---@return integer
    getComponentTypeA = function() end,
    ---@return integer
    getComponentTypeB = function() end,
    ---@return integer
    getNumberplateCity = function() end,
    ---@return number[]
    getPrimaryColor = function() end,
    ---@return number[]
    getSecondaryColor = function() end,
    ---@return string
    getNumberplate = function() end,
    updateLocation = function() end,
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return number
    getHeadingAngle = function() end,
    setEditorPrimaryColor = function() end,
    setEditorSecondaryColor = function() end,
    spawnEditorVehicle = function() end,
    deleteEditorVehicle = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
}

---@class LDWeapon
---@field weapon integer
---@field ammo integer


---@class LDActor
LDActor = {
    ---@return string
    getName = function() end,
    ---@return number x
    ---@return number y
    ---@return number z
    getPosition = function() end,
    ---@param x number
    ---@param y number
    ---@param z number
    setPosition = function(x, y, z) end,
    ---@return CPed
    getEditorPed = function() end,
    ---@return CPed
    getProjectPed = function() end,
    ---@return number
    getHeadingAngle = function() end,
    ---@return integer
    getGroup = function() end,
    ---@return integer
    getModelId = function() end,
    ---@return LDWeapon[]
    getWeapons = function() end,
    ---@return integer
    getDefaultWeapon = function() end,
    ---@return integer
    getModelType = function() end,
    ---@return integer
    getSlot = function() end,
    ---@return integer
    getAccuracy = function() end,
    ---@return integer
    getHealth = function() end,
    ---@return boolean
    isRandomSpawn = function() end,
    ---@return boolean
    isShouldNotDie = function() end,
    ---@return boolean
    isStayInSamePlace = function() end,
    ---@return boolean
    isKindaStayInSamePlace = function() end,
    ---@return boolean
    isHeadshot = function() end,
    ---@return boolean
    isDropWeapons = function() end,
    ---@return boolean
    isDressUp = function() end,
    ---@return integer[]
    getClotherMAnModelKeys = function() end,
    ---@return integer[]
    getClotherMAnTextureKeys = function() end,
    ---@return number
    getFatStat = function() end,
    ---@return number
    getMusculeStat = function() end,
    ---@return integer
    getInteriorId = function() end,
    ---@return boolean
    isShowHealthBarCounter = function() end,
    ---@return string
    getHealthBarCounterText = function() end,
    ---@return uuid
    getUuid = function() end,
    ---@return SignalVoid
    getSignalDeleteActor = function() end,
    ---@return integer
    getProjectHealthBarCounter = function() end,
    updateLocation = function() end,
    spawnEditorPed = function() end,
    deleteEditorPed = function() end,
    spawnProjectEntity = function() end,
    deleteProjectEntity = function() end,
    ---@return LDActor
    copy = function() end,
}

---@class LDSceneSettings
---@field groupRelations integer[][]
---@field time integer[]
---@field trafficPed number
---@field trafficCar number
---@field wantedMin integer
---@field wantedMax integer
---@field weather integer
---@field riot boolean

---@class LDScene
LDScene = {
    ---@return string
    getName = function() end,
    ---@return integer
    getId = function() end,
    ---@return LDObjective[]
    getObjectives = function() end,
    ---@return LDActor[]
    getActors = function() end,
    ---@return LDVehicle[]
    getVehicles = function() end,
    ---@return LDObject[]
    getObjects = function() end,
    ---@return LDParticle[]
    getParticles = function() end,
    ---@return LDTrain[]
    getTrains = function() end,
    ---@return LDPickup[]
    getPickups = function() end,
    ---@return LDPyrotechnics[]
    getPyrotechnics = function() end,
    ---@return LDAudio[]
    getAudio = function() end,
    ---@return LDVisualEffect[]
    getVisualEffects = function() end,
    ---@return LDCheckpoint[]
    getCheckpoints = function() end,
    ---@return LDSceneSettings
    getSceneSettings = function() end,
    ---@return boolean
    isToggleSceneSettings = function() end,
    createNewActor = function() end,
    createNewVehicle = function() end,
    createNewObject = function() end,
    createNewParticle = function() end,
    createNewTrain = function() end,
    createNewPickup = function() end,
    createNewPyrotechnics = function() end,
    createNewAudio = function() end,
    createNewVisualEffect = function() end,
    createNewCheckpoint = function() end,
    updateEditorObjectsCollision = function() end,
    unloadEditorScene = function() end,
    unloadProjectScene = function() end,
    loadEditorScene = function() end,
}


---@class LDProjectData
LDProjectData = {
    ---@return LDScene
    getCurrentScene = function() end,
    ---@return integer
    getCurrentSceneIndex = function() end,
    ---@return SignalVoid
    onChangedScene = function() end,
    ---@param change boolean
    newScene = function(change) end,
    ---@param scene integer
    changeScene = function(scene) end,
    ---@param scene integer
    deleteScene = function(scene) end,
    ---@return table<integer, LDScene>
    getScenes = function() end,
    ---@return LDProjectInfo
    getProjectInfo = function() end,
}

---@class LDProjectInfo
---@field name string
---@field authorName string
---@field startScene integer
---@field directory string

---@class LDProjectsService
LDProjectsService = {
    createNewProject = function(self) end,
    saveCurrentProject = function(self) end,
    ---@param projectIdx integer
    loadProject = function(self,projectIdx) end,
    ---@param projectIdx integer
    loadProductionProject = function(self,projectIdx) end,
    ---@param projectIdx integer
    deleteProject = function(self,projectIdx) end,
    ---@param projectIdx integer
    deleteProductionProject = function(self,projectIdx) end,
    ---@param projectIdx integer
    makeProjectProduction = function(self,projectIdx) end,
    ---@return LDProjectInfo[]
    getProjectsInfos = function(self) end,
    ---@return LDProjectInfo[]
    getProductionProjectsInfos = function(self) end,
    ---@return SignalVoid
    onUpdate = function(self) end,
    ---@return LDProjectData
    getCurrentProject = function(self) end,
}

---@return LDProjectsService
ld.projectsService = function ()
end


ld.settings = {
    ---@param key string
    ---@return boolean | nil
    getBool = function(key) end,
    ---@param key string
    ---@return integer | nil
    getInt = function(key) end,
    ---@param key string
    ---@return number | nil
    getFloat = function(key) end,
    ---@param key string
    ---@return string | nil
    getString = function(key) end,
    ---@param key string
    ---@param value boolean
    setBool = function(key, value) end,
    ---@param key string
    ---@param value integer
    setInt = function(key, value) end,
    ---@param key string
    ---@param value number
    setFloat = function(key, value) end,
    ---@param key string
    ---@param value string
    setString = function(key, value) end,
    save = function() end,
}


time = {}

---current time in milliseconds
---@type number
time.snTimeInMilliseconds = nil