---@meta
---@alias Checkpoint integer
---@alias ControllerMode integer
---@alias Pickup integer
---@alias DecisionMakerChar integer
---@alias Fx integer
---@alias File integer
---@alias DecisionMakerGroup integer
---@alias gxt_key integer
---@alias DynamicLibrary integer
---@alias model_object integer
---@alias Particle integer
---@alias Searchlight integer
---@alias Blip integer
---@alias model_any integer
---@alias Train integer
---@alias SetPieceType integer
---@alias ColPoint integer
---@alias bool boolean
---@alias model_vehicle integer
---@alias CleoBlip integer
---@alias Char integer
---@alias zone_key integer
---@alias DecisionMaker integer
---@alias label integer
---@alias int integer
---@alias AudioStream3D integer
---@alias Interior integer
---@alias Player integer
---@alias string string
---@alias Sphere integer
---@alias FindFile integer
---@alias CarGenerator integer
---@alias arguments integer
---@alias Group integer
---@alias Menu integer
---@alias RenderObject integer
---@alias AudioStream integer
---@alias User3DMarker integer
---@alias Object integer
---@alias MenuGrid integer
---@alias float number
---@alias Plane integer
---@alias script_id integer
---@alias Boat integer
---@alias Car integer
---@alias ScriptFire integer
---@alias Attractor integer
---@alias List integer
---@alias Trailer integer
---@alias model_char integer
---@alias Sequence integer
---@alias Heli integer
CameraOp = {
        ---default
        ---| Shakes the camera with the given intensity 
	---@param intensity int
        shake = function(intensity) end,
	---default
        ---| Returns true if any part of the radius of the specified point is visible on screen 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return bool
        isPointOnScreen = function(x, y, z, radius) end,
	---default
        ---| Attaches the camera to the specified vehicle 
	---@param vehicle Car
	---@param mode CameraMode
	---@param switchStyle SwitchType
        pointAtCar = function(vehicle, mode, switchStyle) end,
	---default
        ---| Attaches the camera to the specified character 
	---@param char Char
	---@param mode CameraMode
	---@param switchStyle SwitchType
        pointAtChar = function(char, mode, switchStyle) end,
	---default
        ---| Restores the camera to its usual position 
        restore = function() end,
	---default
        ---| Sets the fixed camera's position and up vector offset 
	---@param x float
	---@param y float
	---@param z float
	---@param upVecOffsetX float
	---@param upVecOffsetY float
	---@param upVecOffsetZ float
        setFixedPosition = function(x, y, z, upVecOffsetX, upVecOffsetY, upVecOffsetZ) end,
	---default
        ---| Points the camera at the specified location and applies the position set by 0159 
	---@param x float
	---@param y float
	---@param z float
	---@param switchStyle SwitchType
        pointAtPoint = function(x, y, z, switchStyle) end,
	---default
        ---| Sets the RGB color of the fade command (016A) 
	---@param r int
	---@param g int
	---@param b int
        setFadingColor = function(r, g, b) end,
	---default
        ---| Fades the screen for the specified time 
	---@param time int
	---@param direction Fade
        doFade = function(time, direction) end,
	---default
        ---| Returns true if the screen is fading (016A) 
	---@return bool
        getFadingStatus = function() end,
	---default
        ---| Restores the camera, putting it back behind the player 
        restoreJumpcut = function() end,
	---default
        ---| Sets how far behind the camera is from the player 
	---@param zoom int
        setZoom = function(zoom) end,
	---default
        ---| Puts the camera behind the player 
        setBehindPlayer = function() end,
	---default
        ---| Puts the camera in front of the player, pointing towards the player 
        setInFrontOfPlayer = function() end,
	---default
        ---| Sets camera minimum drawing distance 
	---@param distance float
        setNearClip = function(distance) end,
	---default
        ---| Returns the debug camera position 
	---@return float x
	---@return float y
	---@return float z
        getDebugCoordinates = function() end,
	---default
        ---| Sets how long the camera transition will last 
	---@param interpolationToStopMoving float
	---@param time int
        setInterpolationParameters = function(interpolationToStopMoving, time) end,
	---default
        ---| Stores the location the debug camera is pointing to 
	---@return float x
	---@return float y
	---@return float z
        getDebugPointAt = function() end,
	---default
        ---| Keeps the camera relative to the car with the specified offset 
	---@param handle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
	---@param tilt float
	---@param switchStyle SwitchType
        attachToVehicle = function(handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation, tilt, switchStyle) end,
	---default
        ---| Puts the camera on the vehicle like in 0679 
	---@param handle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param vehicle Car
	---@param tilt float
	---@param switchStyle SwitchType
        attachToVehicleLookAtVehicle = function(handle, xOffset, yOffset, zOffset, vehicle, tilt, switchStyle) end,
	---default
        ---| Attaches the camera to the vehicle and points it at the specified character 
	---@param car Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param char Char
	---@param tilt float
	---@param switchStyle SwitchType
        attachToVehicleLookAtChar = function(car, xOffset, yOffset, zOffset, char, tilt, switchStyle) end,
	---default
        ---| Keeps the camera relative to the char with the specified offset 
	---@param handle Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
	---@param tilt float
	---@param switchStyle SwitchType
        attachToChar = function(handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation, tilt, switchStyle) end,
	---default
        ---|  
	---@param char Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param vehicle Car
	---@param tilt float
	---@param switchStyle SwitchType
        attachToCharLookAtVehicle = function(char, xOffset, yOffset, zOffset, vehicle, tilt, switchStyle) end,
	---default
        ---| Puts the camera on the character like with 067C 
	---@param handle Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param char Char
	---@param tilt float
	---@param switchStyle SwitchType
        attachToCharLookAtChar = function(handle, xOffset, yOffset, zOffset, char, tilt, switchStyle) end,
	---default
        ---| Stores the cameras coordinates 
	---@return float x
	---@return float y
	---@return float z
        getActiveCoordinates = function() end,
	---default
        ---| Gets the coordinates the camera is pointing to 
	---@return float x
	---@return float y
	---@return float z
        getActivePointAt = function() end,
	---default
        ---| Enables the cooperative camera mode 
	---@param state bool
        setTwoPlayerMode = function(state) end,
	---default
        ---| Returns the cameras field of view 
	---@return float fov
        getFov = function() end,
	---default
        ---| Enables vehicle bumper view for the camera 
	---@param state bool
        setFirstPersonInCarMode = function(state) end,
	---default
        ---| Bumps the camera in the specified direction as if it had collided 
	---@param xOffset float
	---@param yOffset float
        doBump = function(xOffset, yOffset) end,
	---default
        ---| Makes the camera point at the first coordinates and then rotate to point at the second coordinates 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@param time int
	---@param ease bool
        setVectorTrack = function(fromX, fromY, fromZ, toX, toY, toZ, time, ease) end,
	---default
        ---| Sets the cameras zoom factors 
	---@param from float
	---@param to float
	---@param time int
	---@param ease bool
        setLerpFov = function(from, to, time, ease) end,
	---default
        ---| Darkens the game 
	---@param enable bool
	---@param pitchBlack int
        setDarknessEffect = function(enable, pitchBlack) end,
	---default
        ---| Stops the camera propagating, interpolating, shaking and zooming 
        resetNewScriptables = function() end,
	---default
        ---| Locks the camera target point in position after propagating 
	---@param state bool
        persistTrack = function(state) end,
	---default
        ---| Locks the cameras position 
	---@param state bool
        persistPos = function(state) end,
	---default
        ---| Locks the zoom level after the camera has finished zooming 
	---@param state bool
        persistFov = function(state) end,
	---default
        ---| Returns true if the camera is moving in position 
	---@return bool
        isVectorMoveRunning = function() end,
	---default
        ---| Returns true if the camera is moving in angle 
	---@return bool
        isVectorTrackRunning = function() end,
	---default
        ---| Puts the camera at the position of the first passed coordinates and moves it to the second passed coordinates 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@param time int
	---@param ease bool
        setVectorMove = function(fromX, fromY, fromZ, toX, toY, toZ, time, ease) end,
	---default
        ---| Locks the camera on cinematic vehicle mode 
	---@param state bool
        setCinema = function(state) end,
	---default
        ---| Puts the camera in front of the specified character 
	---@param handle Char
        setInFrontOfChar = function(handle) end,
	---default
        ---| Jiggles the camera in a variety of different ways 
	---@param type int
	---@param timeInMs float
	---@param intensity float
        setShakeSimulationSimple = function(type, timeInMs, intensity) end,
	---default
        ---| Changes the camera mode on the current vehicle, just like when the user presses the 'change view' key 
	---@param mode int
        setPlayerInCarMode = function(mode) end,
	---default
        ---| Makes the camera remain behind the player when in any garage 
	---@param state bool
        allowFixedCollision = function(state) end,
	---default
        ---| Sets the position the camera automatically moves to while driving a vehicle of the specified type 
	---@param modelId model_vehicle
	---@param distance float
	---@param altitude float
	---@param angle float
        setVehicleTweak = function(modelId, distance, altitude, angle) end,
	---default
        ---| Resets any changes made with 09EF 
        resetVehicleTweak = function() end,
	---default
        ---| Takes a screenshot of the screen without any HUD elements and stores the file in the "GTA San Andreas User FilesGallery" folder 
	---@param _p1 bool
        takePhoto = function(_p1) end,
	---default
        ---| Sets the position of the camera to an offset of the targeted entity 
	---@param xOffset float
	---@param yOffset float
        setPositionUnfixed = function(xOffset, yOffset) end,
	---default
        ---| Puts the camera in first-person mode if the player is holding a weapon with a first-person shooting mode (such as a sniper rifle or camera) 
	---@param state bool
        setPhotoEffect = function(state) end,
	---default
        ---| Gets the players chosen camera mode of the current vehicle 
	---@return int mode
        getPlayerInCarMode = function() end,
	---CLEO+
        ---| Enables rotational control of the camera 
	---@param state bool
        setCameraControl = function(state) end,
	---CLEO+
        ---| Returns the current camera mode 
	---@return CameraMode mode
        getCurrentMode = function() end,
	---CLEO+
        ---| Returns world coordinates for the offset from the camera 
	---@param offsetX float
	---@param offsetY float
	---@param offsetZ float
	---@return float x
	---@return float y
	---@return float z
        getOffsetFromCameraInWorldCoords = function(offsetX, offsetY, offsetZ) end,
	---CLEO+
        ---| Returns True if the camera is located within the specified radius of the coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return bool
        locateDistanceToCoordinates = function(x, y, z, radius) end,
	---CLEO+
        ---| Returns the current alpha of the fade being performed ( < 255.0 max ) 
	---@return float alpha
        getFadeAlpha = function() end,
	---CLEO+
        ---| Returns the third person camera target 
	---@param range float
	---@param sourceX float
	---@param sourceY float
	---@param sourceZ float
	---@return float startX
	---@return float startY
	---@return float startZ
	---@return float endX
	---@return float endY
	---@return float endZ
        getThirdPersonTarget = function(range, sourceX, sourceY, sourceZ) end,
	---CLEO+
        ---| Returns the active camera rotation 
	---@return float x
	---@return float y
	---@return float z
        getActiveRotation = function() end,
	---CLEO+
        ---| Returns the address of TheCamera (CCamera) and ActiveCam (CCam) 
	---@param cCamera int
	---@return int activeCCam
        getStruct = function(cCamera) end,
	---CLEO+
        ---| Returns the camera's rotation input values 
	---@return float x
	---@return float y
        getRotationInputValues = function() end,
	---CLEO+
        ---| Sets the camera's rotation Input values 
	---@param x float
	---@param y float
        setRotationInputValues = function(x, y) end,
    }
PlayerOp = {
        ---default
        ---| Creates a player at the specified location 
	---@param playerIndex int
	---@param x float
	---@param y float
	---@param z float
	---@return Player handle
        create = function(playerIndex, x, y, z) end,
	---default
        ---| Adds to the player's money 
	---@param self Player
	---@param money int
        addScore = function(self, money) end,
	---default
        ---| Returns true if the player's money is over the specified value 
	---@param self Player
	---@param money int
	---@return bool
        isScoreGreater = function(self, money) end,
	---default
        ---| Returns the player's money 
	---@param self Player
	---@return int money
        storeScore = function(self) end,
	---default
        ---| Sets the player's wanted level 
	---@param self Player
	---@param wantedLevel int
        alterWantedLevel = function(self, wantedLevel) end,
	---default
        ---| Sets the player's wanted level if the specified level is higher than the current one 
	---@param self Player
	---@param wantedLevel int
        alterWantedLevelNoDrop = function(self, wantedLevel) end,
	---default
        ---| Returns true if the player's wanted level is over the specified value 
	---@param self Player
	---@param wantedLevel int
	---@return bool
        isWantedLevelGreater = function(self, wantedLevel) end,
	---default
        ---| Clears the player's wanted level 
	---@param self Player
        clearWantedLevel = function(self) end,
	---default
        ---| Returns true when player is dead (wasted) 
	---@param self Player
	---@return bool
        isDead = function(self) end,
	---default
        ---| Returns true if the player is honking the horn in a car 
	---@param self Player
	---@return bool
        isPressingHorn = function(self) end,
	---default
        ---| Sets whether player's control is enabled 
	---@param self Player
	---@param state bool
        setControl = function(self, state) end,
	---default
        ---| Returns the player's current wanted level 
	---@param self Player
	---@return int wantedLevel
        storeWantedLevel = function(self) end,
	---default
        ---| Gets the character handle for the specified player 
	---@param self Player
	---@return Char handle
        getChar = function(self) end,
	---default
        ---| Applies brakes to the player's car 
	---@param self Player
	---@param state bool
        applyBrakesToCar = function(self, state) end,
	---default
        ---| Returns true if the player is controlling a remote-control vehicle 
	---@param self Player
	---@return bool
        isInRemoteMode = function(self) end,
	---default
        ---| Returns true if the player hasn't been wasted or busted (the player is still playing) 
	---@param self Player
	---@return bool
        isPlaying = function(self) end,
	---default
        ---| Resets the count of how many times the player has destroyed a certain model 
	---@param self Player
        resetNumOfModelsKilled = function(self) end,
	---default
        ---| Returns the number of times the player has destroyed a specific model 
	---@param self Player
	---@param modelId model_any
	---@return int amount
        getNumOfModelsKilled = function(self, modelId) end,
	---default
        ---| Defines whether the player can run fast forever 
	---@param self Player
	---@param state bool
        setNeverGetsTired = function(self, state) end,
	---default
        ---| Defines whether the player can reload their gun 4x times faster 
	---@param self Player
	---@param state bool
        setFastReload = function(self, state) end,
	---default
        ---| Returns true if the player can move 
	---@param self Player
	---@return bool
        canStartMission = function(self) end,
	---default
        ---| Makes the player safe, putting the character in a safe location 
	---@param self Player
        makeSafeForCutscene = function(self) end,
	---default
        ---| Sets whether the player loses the cash when gets wasted (works once) 
	---@param self Player
	---@param state bool
        setFreeHealthCare = function(self, state) end,
	---default
        ---| Returns true if the player is aiming at the specified character 
	---@param self Player
	---@param handle Char
	---@return bool
        isTargetingChar = function(self, handle) end,
	---default
        ---| Returns true if the player is aiming at the specified object 
	---@param self Player
	---@param handle Object
	---@return bool
        isTargetingObject = function(self, handle) end,
	---default
        ---| Sets the players mood, affecting the dialogue spoken by the player 
	---@param self Player
	---@param mood PlayerMood
	---@param time int
        setMood = function(self, mood, time) end,
	---default
        ---| Returns the stats of the most recent wheelie or stoppie attempt 
	---@param self Player
	---@return int twoWheelsTime
	---@return float twoWheelsDistance
	---@return int wheelieTime
	---@return float wheelieDistance
	---@return int stoppieTime
	---@return float stoppieDistance
        getWheelieStats = function(self) end,
	---default
        ---| Returns true if the player's bodypart has the specified model (0784 or 087B)  
	---@param self Player
	---@param bodyPart BodyPart
	---@param modelName string
	---@return bool
        isWearing = function(self, bodyPart, modelName) end,
	---default
        ---| Sets the players driveby mode 
	---@param self Player
	---@param state bool
        setCanDoDriveBy = function(self, state) end,
	---default
        ---| Makes the camera start moving around in a swirling motion with the specified intensity as if drunk 
	---@param self Player
	---@param intensity int
        setDrunkenness = function(self, intensity) end,
	---default
        ---| Makes the player immune to fire 
	---@param self Player
	---@param state bool
        makeFireProof = function(self, state) end,
	---default
        ---| Increases the player's max health by the specified value and changes current health to the new maximum 
	---@param self Player
	---@param value int
        increaseMaxHealth = function(self, value) end,
	---default
        ---| Increases the player's max armor by the specified value and changes current armor to the new maximum 
	---@param self Player
	---@param value int
        increaseMaxArmor = function(self, value) end,
	---default
        ---| Sets the amount of ammo a player has during a driveby 
	---@param self Player
	---@param ammo int
        ensureHasDriveByWeapon = function(self, ammo) end,
	---default
        ---| Returns true if the player is in the specified zone 
	---@param self Player
	---@param infoZone gxt_key
	---@return bool
        isInInfoZone = function(self, infoZone) end,
	---default
        ---| Returns true if the specified player is auto-aiming at a ped or object 
	---@param self Player
	---@return bool
        isTargetingAnything = function(self) end,
	---default
        ---|  
	---@param self Player
	---@param state bool
        disableSprint = function(self, state) end,
	---default
        ---| Removes the specified player 
	---@param self Player
        delete = function(self) end,
	---default
        ---| Rebuilds the player model, applying any required texture changes 
	---@param self Player
        buildModel = function(self) end,
	---default
        ---|  
	---@param self Player
	---@param textureHash int
	---@param modelHash int
	---@param bodyPart BodyPart
        giveClothes = function(self, textureHash, modelHash, bodyPart) end,
	---default
        ---| Stores the players current clothes to later be restored with 0794 
        storeClothesState = function() end,
	---default
        ---| Restores the players clothes stored with 0793 
        restoreClothesState = function() end,
	---default
        ---|  
	---@param self Player
	---@return Group handle
        getGroup = function(self) end,
	---default
        ---| Sets the player's ability to recruit new members to the group 
	---@param self Player
	---@param state bool
        setGroupRecruitment = function(self, state) end,
	---default
        ---|  
	---@param self Player
	---@return bool
        isPerformingWheelie = function(self) end,
	---default
        ---| Returns true if the player is performing a stoppie 
	---@param self Player
	---@return bool
        isPerformingStoppie = function(self) end,
	---default
        ---| Returns the number of peds killed by the player since the last reset (0297) 
	---@param self Player
	---@return int numPeds
        getTotalNumberOfPedsKilled = function(self) end,
	---default
        ---| Gets the player's current city 
	---@param self Player
	---@return Level level
        getCityIsIn = function(self) end,
	---default
        ---| Sets the view angle for the player attached to an object or vehicle 
	---@param self Player
	---@param heading float
	---@param headingRange float
        setHeadingForAttached = function(self, heading, headingRange) end,
	---default
        ---| Returns true if the heading has finished being applied, as started by 0858 
	---@param self Player
	---@return bool
        isAttachedHeadingAchieved = function(self) end,
	---default
        ---| Sets the players clothing 
	---@param self Player
	---@param textureName string
	---@param modelName string
	---@param bodyPart BodyPart
        giveClothesOutsideShop = function(self, textureName, modelName, bodyPart) end,
	---default
        ---|  
        makeGangDisappear = function() end,
	---default
        ---|  
        makeGangReappear = function() end,
	---default
        ---|  
	---@param self Player
	---@param bodyPart BodyPart
	---@return int textureHash
	---@return int modelHash
        getClothesItem = function(self, bodyPart) end,
	---default
        ---|  
	---@param self Player
	---@return int maxArmour
        getMaxArmor = function(self) end,
	---default
        ---| Changes the player to use the specified model 
	---@param self Player
	---@param modelId int
        setModel = function(self, modelId) end,
	---default
        ---|  
	---@param self Player
	---@param state bool
        forceInteriorLighting = function(self, state) end,
	---default
        ---| Detonates all satchel charges and car bombs planted by the player 
        useDetonator = function() end,
	---default
        ---| Returns true if the player control hasn't been disabled using 01B4 
	---@param self Player
	---@return bool
        isControlOn = function(self) end,
	---default
        ---| Removes the players Goggles and disables night/heat vision 
	---@param self Player
	---@param animate bool
        takeOffGoggles = function(self, animate) end,
	---default
        ---| Returns true if player is using a jetpack 
	---@param self Player
	---@return bool
        isUsingJetpack = function(self) end,
	---default
        ---| Controls the players ability to tell their group to wait and automatically orders any group members to continue following 
	---@param self Player
	---@param state bool
        setGroupToFollowAlways = function(self, state) end,
	---default
        ---| Returns true if the player is climbing 
	---@param self Player
	---@return bool
        isClimbing = function(self) end,
	---default
        ---| Sets whether the player's group stops following the player, even if the player uses the "group follow" button 
	---@param self Player
	---@param state bool
        setGroupToFollowNever = function(self, state) end,
	---default
        ---| Returns true if the player's last shot model is the model specified 
	---@param self Player
	---@param modelId model_any
	---@return bool
        isLastBuildingModelShot = function(self, modelId) end,
	---default
        ---| Resets the status of the last model the player has shot 
	---@param self Player
        clearLastBuildingModelShot = function(self) end,
	---CLEO
        ---|  
	---@param self Player
	---@return bool
	---@return Char handle
        getCharIsTargeting = function(self) end,
	---CLEO+
        ---| Changes player money by set, add or remove 
	---@param self Player
	---@param mode ChangeMoney
	---@param value int
        changeMoney = function(self, mode, value) end,
    }
MathOp = {
        ---default
        ---| Returns the absolute value of the global integer variable 
	---@return int number
        abs = function() end,
	---default
        ---| Returns the absolute value of the local integer variable 
	---@return int number
        abs = function() end,
	---default
        ---| Returns the absolute value of the global float variable 
	---@return float number
        abs = function() end,
	---default
        ---| Returns the absolute value of the local float variable 
	---@return float number
        abs = function() end,
	---default
        ---| Returns a random float between 0.0 to 1.0 
	---@return float float
        random = function() end,
	---default
        ---| Returns a random integer between 0 and 32767 
	---@return int int
        random = function() end,
	---default
        ---| Returns the square root of a number 
	---@param num float
	---@return float result
        sqrt = function(num) end,
	---default
        ---| Returns a random float between the specified ranges 
	---@param min float
	---@param max float
	---@return float result
        randomFloatInRange = function(min, max) end,
	---default
        ---| Returns a random integer greater or equal to min and lesser or equal to max 
	---@param min int
	---@param max int
	---@return int result
        randomIntInRange = function(min, max) end,
	---default
        ---| Returns the sine of the angle 
	---@param angle float
	---@return float result
        sin = function(angle) end,
	---default
        ---| Returns the cosine of the angle 
	---@param angle float
	---@return float result
        cos = function(angle) end,
	---default
        ---| Returns the result of converting meters to feet 
	---@param meters float
	---@return float feet
        convertMetersToFeet = function(meters) end,
	---default
        ---| Returns the result of converting meters to feet 
	---@param meters int
	---@return int feet
        convertMetersToFeet = function(meters) end,
	---default
        ---| Gets the distance between two points 
	---@param fromX float
	---@param fromY float
	---@param toX float
	---@param toZ float
	---@return float distance
        getDistanceBetweenCoords2D = function(fromX, fromY, toX, toZ) end,
	---default
        ---| Gets the distance between two points 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@return float distance
        getDistanceBetweenCoords3D = function(fromX, fromY, fromZ, toX, toY, toZ) end,
	---default
        ---| Gets the angle between the two 2D vectors 
	---@param x1 float
	---@param y1 float
	---@param x2 float
	---@param y2 float
	---@return float angle
        getAngleBetween2DVectors = function(x1, y1, x2, y2) end,
	---default
        ---| Returns true if rectangle1 is inside rectangle2 or partially intersects it 
	---@param rectangle1PositionX float
	---@param rectangle1PositionY float
	---@param rectangle1SizeX float
	---@param rectangle1SizeY float
	---@param rectangle2PositionX float
	---@param rectangle2PositionY float
	---@param rectangle2SizeX float
	---@param rectangle2SizeY float
	---@return bool
        do2DRectanglesCollide = function(rectangle1PositionX, rectangle1PositionY, rectangle1SizeX, rectangle1SizeY, rectangle2PositionX, rectangle2PositionY, rectangle2SizeX, rectangle2SizeY) end,
	---default
        ---| Returns the point of intersection of two lines. If they do not intersect, both returned values are -1000000.0 and the condition result is false 
	---@param line1StartX float
	---@param line1StartY float
	---@param line1EndX float
	---@param line1EndY float
	---@param line2StartX float
	---@param line2StartY float
	---@param line2EndX float
	---@param line2EndY float
	---@return bool
	---@return float intersectPointX
	---@return float intersectPointY
        get2DLinesIntersectPoint = function(line1StartX, line1StartY, line1EndX, line1EndY, line2StartX, line2StartY, line2EndX, line2EndY) end,
	---default
        ---| Gets the angle for the XY offset 
	---@param x float
	---@param y float
	---@return float heading
        getHeadingFromVector2D = function(x, y) end,
	---default
        ---| Gets the exact angle of an angle 
	---@param value float
	---@return float result
        limitAngle = function(value) end,
	---default
        ---| Checks if the nth bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---default
        ---| Checks if the nth bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---default
        ---| Checks if the nth bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---default
        ---| Checks if the nth bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---default
        ---| Checks if the nth bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---default
        ---| Checks if the nth bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---default
        ---| Sets the nth bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---default
        ---| Sets the nth bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---default
        ---| Sets the nth bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---default
        ---| Sets the nth bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---default
        ---| Sets the nth bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---default
        ---| Sets the nth bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---default
        ---| Clears the nth bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---default
        ---| Clears the nth bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---default
        ---| Clears the nth bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---default
        ---| Clears the nth bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---default
        ---| Clears the nth bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---default
        ---| Clears the nth bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---CLEO
        ---| Returns the specified number raised to the specified power 
	---@param number float
	---@param power float
	---@return float result
        pow = function(number, power) end,
	---CLEO
        ---| Returns the logarithm of the specified number in the specified base 
	---@param number float
	---@param base float
	---@return float result
        log = function(number, base) end,
	---CLEO+
        ---| Calculates the 1D Perlin simplex noise 
	---@param x float
	---@return float result
        perlinNoise = function(x) end,
	---CLEO+
        ---| Eases k value in range of 0.0 to 1.0, resulting in a easing value based on mode and way, useful for smooth animations 
	---@param k float
	---@param mode EaseMode
	---@param way EaseWay
	---@return float result
        ease = function(k, mode, way) end,
	---CLEO+
        ---|  
	---@param x1 float
	---@param y1 float
	---@param x2 float
	---@param y2 float
	---@return float angle
        getAngleFromTwoCoords = function(x1, y1, x2, y2) end,
	---CLEO+
        ---| Calculates the Fractal Brownian Motion (fBm) summation of 1D Perlin Simplex noise 
	---@param x float
	---@param octaves int
	---@param frequency float
	---@param amplitude float
	---@param lacunarity float
	---@param persistence float
	---@return float result
        perlinNoiseFractal = function(x, octaves, frequency, amplitude, lacunarity, persistence) end,
	---CLEO+
        ---| Returns randomly True the specified percent of the time 
	---@param percent int
	---@return bool
        randomPercent = function(percent) end,
	---CLEO+
        ---| Generates a randomInteger from min to < max with seed 
	---@param seed int
	---@param min int
	---@param max int
	---@return int randomInteger
        generateRandomIntInRangeWithSeed = function(seed, min, max) end,
	---CLEO+
        ---| Generates a randomFloat from min to < max with seed 
	---@param seed int
	---@param min float
	---@param max float
	---@return float randomFloat
        generateRandomFloatInRangeWithSeed = function(seed, min, max) end,
	---CLEO+
        ---| Calculates the Fractal Brownian Motion (fBm) summation of 2D Perlin Simplex noise 
	---@param x float
	---@param y float
	---@param octaves int
	---@param frequency float
	---@param amplitude float
	---@param lacunarity float
	---@param persistence float
	---@return float result
        perlinNoiseFractal2D = function(x, y, octaves, frequency, amplitude, lacunarity, persistence) end,
	---CLEO+
        ---| Calculates the Fractal Brownian Motion (fBm) summation of 3D Perlin Simplex noise 
	---@param x float
	---@param y float
	---@param z float
	---@param octaves int
	---@param frequency float
	---@param amplitude float
	---@param lacunarity float
	---@param persistence float
	---@return float result
        perlinNoiseFractal3D = function(x, y, z, octaves, frequency, amplitude, lacunarity, persistence) end,
	---CLEO+
        ---| Returns the clamped value of the specified float between the min and max values 
	---@param float float
	---@param min float
	---@param max float
	---@return float clamped
        clampFloat = function(float, min, max) end,
	---CLEO+
        ---| Returns the clamped value of the specified integer between the min and max values 
	---@param integer int
	---@param min int
	---@param max int
	---@return int clamped
        clampInt = function(integer, min, max) end,
	---bitwise
        ---| Extends a 1-, 2- or 3-byte long integer value to a 4-byte (32-bit), while preserving the sign (+/-) 
	---@param value int
	---@param fromSize int
        signExtend = function(value, fromSize) end,
	---math
        ---| Checks if n-th bit of the number is set 
	---@param number int
	---@param bitIndex int
	---@return bool
        isBitSet = function(number, bitIndex) end,
	---math
        ---| Sets n-th bit of the number 
	---@param number int
	---@param bitIndex int
        setBit = function(number, bitIndex) end,
	---math
        ---| Clears n-th bit of the number 
	---@param number int
	---@param bitIndex int
        clearBit = function(number, bitIndex) end,
	---math
        ---| Sets state of n-th bit in the number 
	---@param number int
	---@param bitIndex int
	---@param state bool
        toggleBit = function(number, bitIndex, state) end,
	---math
        ---| Checks if value contains number different than 0 or not empty string 
	---@param value any
	---@return bool
        isTruthy = function(value) end,
	---math
        ---| Selects one random integer from provided values 
	---@param values arguments
	---@return int result
        randomPick = function(values) end,
	---math
        ---| Selects one random float from provided values 
	---@param values arguments
	---@return float result
        randomPick = function(values) end,
	---math
        ---| Selects one random text from provided values 
	---@param values arguments
	---@return string result
        randomPick = function(values) end,
	---math
        ---| Sets random condition result, with percent chance to be true 
	---@param percent float
	---@return bool
        randomChance = function(percent) end,
    }
CharOp = {
        ---default
        ---| Creates a character at the specified location, with the specified model and pedtype 
	---@param pedType PedType
	---@param modelId model_char
	---@param x float
	---@param y float
	---@param z float
	---@return Char handle
        create = function(pedType, modelId, x, y, z) end,
	---default
        ---| Removes the character from the game and mission cleanup list, freeing game memory 
	---@param self Char
        delete = function(self) end,
	---default
        ---| Returns the character's coordinates 
	---@param self Char
	---@return float x
	---@return float y
	---@return float z
        getCoordinates = function(self) end,
	---default
        ---| Puts the character at the specified location 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setCoordinates = function(self, x, y, z) end,
	---default
        ---| Returns true if the character is within the specified 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isInArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the character is within the specified 3D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isInArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Returns the current vehicle of the character and adds it to the mission cleanup list (alts:03C0,0811,0484) 
	---@param self Char
	---@return Car handle
        storeCarIsIn = function(self) end,
	---default
        ---| Returns true if the character is in the specified vehicle 
	---@param self Char
	---@param vehicle Car
	---@return bool
        isInCar = function(self, vehicle) end,
	---default
        ---| Returns true if the character is driving a vehicle with the specified model 
	---@param self Char
	---@param modelId model_vehicle
	---@return bool
        isInModel = function(self, modelId) end,
	---default
        ---| Returns true if the character has a vehicle, even if they are not actually sat inside it (opening and closing the door) 
	---@param self Char
	---@return bool
        isInAnyCar = function(self) end,
	---default
        ---| Returns true if the character is within the 2D radius of the coordinates point 
	---@param self Char
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeans2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the coordinates point on foot 
	---@param self Char
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFoot2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the coordinates point in a vehicle 
	---@param self Char
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateInCar2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the 2D radius of the coordinates point 
	---@param self Char
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateStoppedAnyMeans2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the 2D radius of the coordinates point on foot 
	---@param self Char
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateStoppedOnFoot2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the 2D radius of the coordinates point in a vehicle 
	---@param self Char
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateStoppedInCar2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the other character 
	---@param self Char
	---@param target Char
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeansChar2D = function(self, target, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the other character on foot 
	---@param self Char
	---@param target Char
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFootChar2D = function(self, target, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the other character in a vehicle 
	---@param self Char
	---@param otherChar Char
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateInCarChar2D = function(self, otherChar, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the coordinates point 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeans3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the coordinates point on foot 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFoot3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the coordinates point in a vehicle 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateInCar3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the 3D radius of the coordinates point 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateStoppedAnyMeans3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the 3D radius of the coordinates point on foot 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateStoppedOnFoot3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the 3D radius of the coordinates point in a vehicle 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateStoppedInCar3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the other character 
	---@param self Char
	---@param target Char
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeansChar3D = function(self, target, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the other character on foot 
	---@param self Char
	---@param target Char
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFootChar3D = function(self, target, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the other character in a vehicle 
	---@param self Char
	---@param target Char
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateInCarChar3D = function(self, target, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Adds the specified amount of ammo to the character's weapon, if the character has the weapon 
	---@param self Char
	---@param weaponType WeaponType
	---@param ammo int
        addAmmo = function(self, weaponType, ammo) end,
	---default
        ---| Returns true if the handle is an invalid character handle or the character is dead (wasted) 
	---@param handle any
	---@return bool
        isDead = function(handle) end,
	---default
        ---| Creates a character in the driver's seat of the vehicle 
	---@param vehicle Car
	---@param pedType PedType
	---@param modelId model_char
	---@return Char handle
        createInsideCar = function(vehicle, pedType, modelId) end,
	---default
        ---| Returns true if the character is in the specified map zone 
	---@param self Char
	---@param zone zone_key
	---@return bool
        isInZone = function(self, zone) end,
	---default
        ---| Returns the character's heading (z-angle) 
	---@param self Char
	---@return float heading
        getHeading = function(self) end,
	---default
        ---| Sets the character's heading (z-angle) 
	---@param self Char
	---@param heading float
        setHeading = function(self, heading) end,
	---default
        ---| Returns true if the character is colliding with the specified object 
	---@param self Char
	---@param object Object
	---@return bool
        isTouchingObject = function(self, object) end,
	---default
        ---| Sets the amount of ammo the character has in the specified weapon 
	---@param self Char
	---@param weaponType WeaponType
	---@param ammo int
        setAmmo = function(self, weaponType, ammo) end,
	---default
        ---| Returns true if the character's health is over the specified value 
	---@param self Char
	---@param health int
	---@return bool
        isHealthGreater = function(self, health) end,
	---default
        ---| Returns true if the character is within the specified 2D area on foot 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isInAreaOnFoot2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the character is within the specified 2D area in a vehicle 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isInAreaInCar2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the specified 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isStoppedInArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the specified 2D area on foot 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isStoppedInAreaOnFoot2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the specified 2D area in a vehicle 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isStoppedInAreaInCar2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the character is within the specified 3D area on foot 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isInAreaOnFoot3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Returns true if the character is within the specified 3D area in a vehicle 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isInAreaInCar3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the specified 3D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isStoppedInArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the specified 3D area on foot 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isStoppedInAreaOnFoot3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Returns true if the character stopped within the specified 3D area in a vehicle 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isStoppedInAreaInCar3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Gives the character the weapon with the specified amount of ammo 
	---@param self Char
	---@param weaponType WeaponType
	---@param ammo int
        giveWeapon = function(self, weaponType, ammo) end,
	---default
        ---| Sets the character's currently held weapon 
	---@param self Char
	---@param weaponType WeaponType
        setCurrentWeapon = function(self, weaponType) end,
	---default
        ---| Allows the character to be deleted by the game if necessary, and also removes them from the mission cleanup list, if applicable 
	---@param self Char
        markAsNoLongerNeeded = function(self) end,
	---default
        ---| Removes the character from the mission cleanup list, preventing it from being deleted when the mission ends 
	---@param self Char
        dontRemove = function(self) end,
	---default
        ---| Creates a character with the specified model in the passenger seat of the vehicle 
	---@param vehicle Car
	---@param pedType PedType
	---@param modelId model_char
	---@param seat SeatId
	---@return Char handle
        createAsPassenger = function(vehicle, pedType, modelId, seat) end,
	---default
        ---| Returns true if the character is within the 2D radius of the vehicle 
	---@param self Char
	---@param vehicle Car
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeansCar2D = function(self, vehicle, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the vehicle on foot 
	---@param self Char
	---@param vehicle Car
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFootCar2D = function(self, vehicle, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the vehicle in a vehicle 
	---@param self Char
	---@param handle Car
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateInCarCar2D = function(self, handle, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the vehicle 
	---@param self Char
	---@param vehicle Car
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeansCar3D = function(self, vehicle, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the vehicle on foot 
	---@param self Char
	---@param vehicle Car
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFootCar3D = function(self, vehicle, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the vehicle in a vehicle 
	---@param self Char
	---@param vehicle Car
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateInCarCar3D = function(self, vehicle, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Sets the character's health 
	---@param self Char
	---@param health int
        setHealth = function(self, health) end,
	---default
        ---| Returns the character's health 
	---@param self Char
	---@return int health
        getHealth = function(self) end,
	---default
        ---| Returns true if the character is colliding with the specified object on foot 
	---@param self Char
	---@param object Object
	---@return bool
        isTouchingObjectOnFoot = function(self, object) end,
	---default
        ---| Sets the animation group for the character 
	---@param self Char
	---@param animGroup AnimGroup
        setAnimGroup = function(self, animGroup) end,
	---default
        ---| Returns true if the character is not moving 
	---@param self Char
	---@return bool
        isStopped = function(self) end,
	---default
        ---| Makes a character immune to everything except the player 
	---@param self Char
	---@param state bool
        setOnlyDamagedByPlayer = function(self, state) end,
	---default
        ---| Sets the character's immunities 
	---@param self Char
	---@param bulletProof bool
	---@param fireProof bool
	---@param explosionProof bool
	---@param collisionProof bool
	---@param meleeProof bool
        setProofs = function(self, bulletProof, fireProof, explosionProof, collisionProof, meleeProof) end,
	---default
        ---| Returns true if the character is visible 
	---@param self Char
	---@return bool
        isOnScreen = function(self) end,
	---default
        ---| Returns true if the character fired a weapon within the specified 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param topRightX float
	---@param topRightY float
	---@param drawSphere bool
	---@return bool
        isShootingInArea = function(self, leftBottomX, leftBottomY, topRightX, topRightY, drawSphere) end,
	---default
        ---| Returns true if the character is holding the given type of weapon 
	---@param self Char
	---@param weaponType WeaponType
	---@return bool
        isCurrentWeapon = function(self, weaponType) end,
	---default
        ---| Returns true if the character is firing a weapon 
	---@param self Char
	---@return bool
        isShooting = function(self) end,
	---default
        ---| Affects how often the character will hit the target when attacking with a weapon 
	---@param self Char
	---@param accuracy int
        setAccuracy = function(self, accuracy) end,
	---default
        ---| Returns true if the character's model ID is equivalent to the model ID passed 
	---@param self Char
	---@param modelId model_char
	---@return bool
        isModel = function(self, modelId) end,
	---default
        ---| Returns true if the character has been hit by the specified weapon 
	---@param self Char
	---@param weaponType WeaponType
	---@return bool
        hasBeenDamagedByWeapon = function(self, weaponType) end,
	---default
        ---| Dismembers the character 
	---@param self Char
        explodeHead = function(self) end,
	---default
        ---| Makes a character bleed 
	---@param self Char
	---@param state bool
        setBleeding = function(self, state) end,
	---default
        ---| Sets whether the character is visible or not 
	---@param self Char
	---@param state bool
        setVisible = function(self, state) end,
	---default
        ---| Removes the character with a fade, freeing game memory 
	---@param self Char
        removeElegantly = function(self) end,
	---default
        ---| Makes the character maintain their position when attacked 
	---@param self Char
	---@param state bool
        setStayInSamePlace = function(self, state) end,
	---default
        ---| Increases the character's armor by the specified value to the maximum of 100 
	---@param self Char
	---@param amount int
        addArmor = function(self, amount) end,
	---default
        ---| Pulls the character out of their car and places at the location 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        warpFromCarToCoord = function(self, x, y, z) end,
	---default
        ---| Returns true if the character can see the target character 
	---@param self Char
	---@param target Char
	---@return bool
        hasSpottedChar = function(self, target) end,
	---default
        ---| Puts the character in the specified vehicle 
	---@param self Char
	---@param vehicle Car
        warpIntoCar = function(self, vehicle) end,
	---default
        ---| Creates a character with a randomised model and pedtype at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return Char handle
        createRandom = function(x, y, z) end,
	---default
        ---| Makes an char perform an animation at the specified speed 
	---@param self Char
	---@param animName string
	---@param animSpeed float
        setAnimSpeed = function(self, animName, animSpeed) end,
	---default
        ---| Locks the character while in a car 
	---@param self Char
	---@param state bool
        setCantBeDraggedOut = function(self, state) end,
	---default
        ---| Returns true if the character is male 
	---@param self Char
	---@return bool
        isMale = function(self) end,
	---default
        ---| Returns the character's vehicle handle without marking it as used by the script, therefore allowing it to be deleted by the game at any time (alts:00D9,0811,0484) 
	---@param self Char
	---@return Car handle
        storeCarIsInNoSave = function(self) end,
	---default
        ---| Sets the character's cash sum, setting how much cash they will drop when dead 
	---@param self Char
	---@param amount int
        setMoney = function(self, amount) end,
	---default
        ---| Gets the amount of ammo in the specified weapon of the character 
	---@param self Char
	---@param weaponType WeaponType
	---@return int ammo
        getAmmoInWeapon = function(self, weaponType) end,
	---default
        ---| Puts the character into a vehicle's passenger seat 
	---@param self Char
	---@param handle Car
	---@param seat SeatId
        warpIntoCarAsPassenger = function(self, handle, seat) end,
	---default
        ---| Sets whether the character is a psychotic killer or not 
	---@param self Char
	---@param state bool
        setIsChrisCriminal = function(self, state) end,
	---default
        ---| Sets whether the specified character is immune to headshots 
	---@param self Char
	---@param state bool
        setSuffersCriticalHits = function(self, state) end,
	---default
        ---| Returns true if the character is sitting in the specified vehicle 
	---@param self Char
	---@param vehicle Car
	---@return bool
        isSittingInCar = function(self, vehicle) end,
	---default
        ---| Returns true if the character is sitting in any vehicle 
	---@param self Char
	---@return bool
        isSittingInAnyCar = function(self) end,
	---default
        ---| Returns true if the character is on foot, and not occupying a vehicle 
	---@param self Char
	---@return bool
        isOnFoot = function(self) end,
	---default
        ---| Puts character into a turret on the vehicle, allowing them to shoot 
	---@param self Char
	---@param vehicle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param heading Facing
	---@param headingRange float
	---@param weaponType WeaponType
        attachToCar = function(self, vehicle, xOffset, yOffset, zOffset, heading, headingRange, weaponType) end,
	---default
        ---| Takes the character out of turret mode (0464) 
	---@param self Char
        detachFromCar = function(self) end,
	---default
        ---| Clears the character's last weapon damage (see 031D) 
	---@param self Char
        clearLastWeaponDamage = function(self) end,
	---default
        ---| Returns the number of members which are in a group of the character (01DE) 
	---@param self Char
	---@return int number
        getNumberOfFollowers = function(self) end,
	---default
        ---| Returns the type of weapon that the character is currently holding 
	---@param self Char
	---@return WeaponType weaponType
        getCurrentWeapon = function(self) end,
	---default
        ---| Returns true if the character is within the 2D radius of the object 
	---@param self Char
	---@param object Object
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeansObject2D = function(self, object, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the object on foot 
	---@param self Char
	---@param object Object
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFootObject2D = function(self, object, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 2D radius of the object in a vehicle 
	---@param self Char
	---@param object Object
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateInCarObject2D = function(self, object, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the object 
	---@param self Char
	---@param object Object
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateAnyMeansObject3D = function(self, object, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the object on foot 
	---@param self Char
	---@param object Object
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateOnFootObject3D = function(self, object, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is within the 3D radius of the object in a vehicle 
	---@param self Char
	---@param object Object
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateInCarObject3D = function(self, object, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the character is riding a bike 
	---@param self Char
	---@return bool
        isOnAnyBike = function(self) end,
	---default
        ---| Returns true if the character sees a dead body of the given type 
	---@param self Char
	---@param pedType PedType
	---@return bool
        canSeeDeadChar = function(self, pedType) end,
	---default
        ---| Sets the character's ability to talk 
	---@param self Char
	---@param state bool
        shutUp = function(self, state) end,
	---default
        ---| Removes the characters weapons 
	---@param self Char
        removeAllWeapons = function(self) end,
	---default
        ---| Returns true if the character has the specified weapon 
	---@param self Char
	---@param weaponType WeaponType
	---@return bool
        hasGotWeapon = function(self, weaponType) end,
	---default
        ---| Returns true if the character is driving a boat 
	---@param self Char
	---@return bool
        isInAnyBoat = function(self) end,
	---default
        ---| Returns true if the character is flying a helicopter 
	---@param self Char
	---@return bool
        isInAnyHeli = function(self) end,
	---default
        ---| Returns true if the character is in a plane 
	---@param self Char
	---@return bool
        isInAnyPlane = function(self) end,
	---default
        ---| Returns true if the character is in water 
	---@param self Char
	---@return bool
        isInWater = function(self) end,
	---default
        ---| Returns the weapon type, ammo and model from the specified slot 
	---@param self Char
	---@param weaponSlotId WeaponSlot
	---@return WeaponType weaponType
	---@return int weaponAmmo
	---@return model_object weaponModel
        getWeaponInSlot = function(self, weaponSlotId) end,
	---default
        ---| Returns the coordinates of the character, with an offset 
	---@param self Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@return float x
	---@return float y
	---@return float z
        getOffsetInWorldCoords = function(self, xOffset, yOffset, zOffset) end,
	---default
        ---| Returns true if the character has been photographed 
	---@param self Char
	---@return bool
        hasBeenPhotographed = function(self) end,
	---default
        ---| Returns true if the character is in a flying vehicle 
	---@param self Char
	---@return bool
        isInFlyingVehicle = function(self) end,
	---default
        ---| Sets whether the character's position remains unchanged 
	---@param self Char
	---@param state bool
        freezePosition = function(self, state) end,
	---default
        ---| Controls whether the character can drown in water 
	---@param self Char
	---@param state bool
        setDrownsInWater = function(self, state) end,
	---default
        ---| Returns the character's armor amount 
	---@param self Char
	---@return int armor
        getArmor = function(self) end,
	---default
        ---|  
	---@param self Char
	---@return bool
        isWaitingForWorldCollision = function(self) end,
	---default
        ---| Attaches the character to the specified object, in turret mode 
	---@param self Char
	---@param handle Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param heading Facing
	---@param headingRange float
	---@param weaponType WeaponType
        attachToObject = function(self, handle, xOffset, yOffset, zOffset, heading, headingRange, weaponType) end,
	---default
        ---| Creates a character descending from a rope 
	---@param pedType PedType
	---@param modelId model_char
	---@param x float
	---@param y float
	---@param z float
	---@return Char handle
        createSwatRope = function(pedType, modelId, x, y, z) end,
	---default
        ---| Returns true if the character has been hurt by the other character 
	---@param self Char
	---@param handle Char
	---@return bool
        hasBeenDamagedByChar = function(self, handle) end,
	---default
        ---| Returns true if the char has been hurt by the specified vehicle 
	---@param self Char
	---@param handle Car
	---@return bool
        hasBeenDamagedByCar = function(self, handle) end,
	---default
        ---| Makes the character stay in the vehicle when it is jacked (characters let themselves get "kidnapped") 
	---@param self Char
	---@param state bool
        setStayInCarWhenJacked = function(self, state) end,
	---default
        ---| Returns true if the character is colliding with a car 
	---@param self Char
	---@param handle Car
	---@return bool
        isTouchingVehicle = function(self, handle) end,
	---default
        ---| Makes the character immune to a damage while in a vehicle 
	---@param self Char
	---@param state bool
        setCanBeShotInVehicle = function(self, state) end,
	---default
        ---|  
	---@param self Char
        clearLastDamageEntity = function(self) end,
	---default
        ---| Removes the weapon from the character 
	---@param self Char
	---@param weaponType WeaponType
        removeWeapon = function(self, weaponType) end,
	---default
        ---| Creates a driver in the vehicle 
	---@param vehicle Car
	---@return Char handle
        createRandomAsDriver = function(vehicle) end,
	---default
        ---| Creates a random character in the passenger seat of the vehicle 
	---@param vehicle Car
	---@param seat SeatId
	---@return Char handle
        createRandomAsPassenger = function(vehicle, seat) end,
	---default
        ---| Sets whether the character won't be targeted by the autoaim system 
	---@param self Char
	---@param state bool
        setNeverTargeted = function(self, state) end,
	---default
        ---| Returns true if the character is driving a police vehicle 
	---@param self Char
	---@return bool
        isInAnyPoliceVehicle = function(self) end,
	---default
        ---| Returns true if the handle is a valid character handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        freezePositionAndDontLoadCollision = function(self, state) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        setLoadCollisionFlag = function(self, state) end,
	---default
        ---| Returns true if the specified character is crouching 
	---@param self Char
	---@return bool
        isDucking = function(self) end,
	---default
        ---| Checks if the character is within the angled 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Checks if the character is within the angled 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledAreaOnFoot2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Checks if the character is in a car which is within the angled 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledAreaInCar2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Checks if the character is within the angled 2D area and is motionless 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isStoppedInAngledArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Checks if the character is within the angled 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isStoppedInAngledAreaOnFoot2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Checks if the character is in a motionless car within the angled 2D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isStoppedInAngledAreaInCar2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Checks if the character is within the angled 3D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---| Checks if the character is within the angled 3D area 
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledAreaOnFoot3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---|  
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledAreaInCar3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---|  
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isStoppedInAngledArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---|  
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isStoppedInAngledAreaOnFoot3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---|  
	---@param self Char
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isStoppedInAngledAreaInCar3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---| Returns true if the character is driving a taxi 
	---@param self Char
	---@return bool
        isInTaxi = function(self) end,
	---default
        ---| Sets the decision maker for the character 
	---@param self Char
	---@param handleOrTemplate DecisionMakerCharTemplate
        setDecisionMaker = function(self, handleOrTemplate) end,
	---default
        ---| Sets the seeing and hearing range for the specified character or for all mission characters when handle is -1 
	---@param handle Char
	---@param range float
        setSenseRange = function(handle, range) end,
	---default
        ---| Returns true if character is performing the specified animation 
	---@param self Char
	---@param animationName string
	---@return bool
        isPlayingAnim = function(self, animationName) end,
	---default
        ---| Sets whether the animation is playing 
	---@param self Char
	---@param animationName string
	---@param flag bool
        setAnimPlayingFlag = function(self, animationName, flag) end,
	---default
        ---| Returns the progress of the animation on the char, ranging from 0.0 to 1.0 
	---@param self Char
	---@param animationName string
	---@return float time
        getAnimCurrentTime = function(self, animationName) end,
	---default
        ---| Sets how far through the animation the character is, with 1 
	---@param self Char
	---@param animationName string
	---@param time float
        setAnimCurrentTime = function(self, animationName, time) end,
	---default
        ---| Assigns the character to the specified action sequence 
	---@param self Char
	---@param sequence Sequence
        performSequence = function(self, sequence) end,
	---default
        ---| Sets whether collision detection is enabled for the character 
	---@param self Char
	---@param state bool
        setCollision = function(self, state) end,
	---default
        ---| Returns a float of the length of the animation in milliseconds 
	---@param self Char
	---@param animationName string
	---@return float totalTime
        getAnimTotalTime = function(self, animationName) end,
	---default
        ---|  
	---@param pedType PedType
	---@param modelId model_char
	---@param taskId int
	---@param attractor Attractor
	---@return Char handle
        createAtAttractor = function(pedType, modelId, taskId, attractor) end,
	---default
        ---| Returns the status of the specified script task of the character 
	---@param self Char
	---@param taskId int
	---@return TaskStatus status
        getScriptTaskStatus = function(self, taskId) end,
	---default
        ---|  
	---@param self Char
	---@param handle Attractor
	---@return bool
        isAtScriptedAttractor = function(self, handle) end,
	---default
        ---| Gets the characters task sequence progress, as started by 0618 
	---@param self Char
	---@return int progress
        getSequenceProgress = function(self) end,
	---default
        ---| Clears the char's look task, making them stop looking at whatever they were assigned to look at 
	---@param self Char
        clearLookAt = function(self) end,
	---default
        ---| Sets the range within which the char responds to events 
	---@param self Char
	---@param range float
        setFollowNodeThresholdDistance = function(self, range) end,
	---default
        ---| Returns the characters model 
	---@param self Char
	---@return int modelId
        getModel = function(self) end,
	---default
        ---| Clears the char's task, making them quit whatever they were doing 
	---@param self Char
        clearTasks = function(self) end,
	---default
        ---|  
	---@param self Char
	---@param vehicle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param heading Facing
	---@param headingRange float
	---@param pitchRange float
	---@param weaponType WeaponType
        attachToBike = function(self, vehicle, xOffset, yOffset, zOffset, heading, headingRange, pitchRange, weaponType) end,
	---default
        ---| Hides all of the specified char's weapons 
	---@param self Char
	---@param state bool
        hideWeaponForScriptedCutscene = function(self, state) end,
	---default
        ---| Returns the char's movement speed 
	---@param self Char
	---@return float speed
        getSpeed = function(self) end,
	---default
        ---| Removes the character from their current group 
	---@param self Char
        removeFromGroup = function(self) end,
	---default
        ---| Returns true if the character is a member of the specified group 
	---@param self Char
	---@param handle Group
	---@return bool
        isGroupMember = function(self, handle) end,
	---default
        ---| Returns true if the character is the leader of the specified group 
	---@param self Char
	---@param handle Group
	---@return bool
        isGroupLeader = function(self, handle) end,
	---default
        ---| Returns true if any characters are within range of the character 
	---@param self Char
	---@param radius float
	---@return bool
        isNearAnyChars = function(self, radius) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        dropObject = function(self, state) end,
	---default
        ---| Returns true if the char is lifting the specified object 
	---@param self Char
	---@param handle Object
	---@return bool
        isHoldingObject = function(self, handle) end,
	---default
        ---| Returns true if the character has been arrested 
	---@param self Char
	---@return bool
        hasBeenArrested = function(self) end,
	---default
        ---|  
	---@param self Char
	---@param radius float
	---@param _p3 int
        setInformRespectedFriends = function(self, radius, _p3) end,
	---default
        ---| Returns true if the character is responding to the specified ped event 
	---@param self Char
	---@param event Event
	---@return bool
        isRespondingToEvent = function(self, event) end,
	---default
        ---| Causes the auto aim to be more likely to target the specified char than actors without this flag 
	---@param self Char
	---@param state bool
        setIsTargetPriority = function(self, state) end,
	---default
        ---|  
	---@param self Char
	---@param relationshipType RelationshipType
	---@param pedType PedType
        setRelationship = function(self, relationshipType, pedType) end,
	---default
        ---|  
	---@param self Char
	---@param relationshipType RelationshipType
	---@param toPedType PedType
        clearRelationship = function(self, relationshipType, toPedType) end,
	---default
        ---|  
	---@param self Char
	---@param relationshipType RelationshipType
        clearAllRelationships = function(self, relationshipType) end,
	---default
        ---| Clears all the characters tasks immediately, resetting the character to an idle state 
	---@param self Char
        clearTasksImmediately = function(self) end,
	---default
        ---|  
	---@param self Char
	---@param sequence Sequence
	---@param startTaskIndex int
	---@param endTaskIndex int
        performSequenceFromProgress = function(self, sequence, startTaskIndex, endTaskIndex) end,
	---default
        ---| Sets how the character chooses to go to their destination in the next task without a parameter specifying this 
	---@param moveState MoveState
        setNextDesiredMoveState = function(moveState) end,
	---default
        ---|  
	---@param self Char
	---@return int _p2
	---@return int _p3
        getSequenceProgressRecursive = function(self) end,
	---default
        ---| Returns the handle for the searchlight that's targeting the character 
	---@param self Char
	---@return bool
	---@return Searchlight handle
        isInAnySearchlight = function(self) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        listenToPlayerGroupCommands = function(self, state) end,
	---default
        ---| Sets the attack rate of the char 
	---@param self Char
	---@param rate int
        setShootRate = function(self, rate) end,
	---default
        ---| Sets the specified characters fighting style and moves 
	---@param self Char
	---@param style FightStyle
	---@param move FightMoves
        giveMeleeAttack = function(self, style, move) end,
	---default
        ---| Gets the characters active ped event 
	---@param self Char
	---@return Event event
        getHighestPriorityEvent = function(self) end,
	---default
        ---| Stores a handle for the vehicle the character is in or entering (alts: 00D9,03C0,0484) 
	---@param self Char
	---@return Car handle
        getCarIsUsing = function(self) end,
	---default
        ---| Sets whether the character shouldn't chase their victim far (to attempt a melee attack or get in weapon range) 
	---@param self Char
	---@param state bool
        setKindaStayInSamePlace = function(self, state) end,
	---default
        ---| Returns true if the character is in the air 
	---@param self Char
	---@return bool
        isInAir = function(self) end,
	---default
        ---| Returns the char's distance from ground 
	---@param self Char
	---@return float height
        getHeightAboveGround = function(self) end,
	---default
        ---| Sets the character's fire arms wielding style 
	---@param self Char
	---@param skill WeaponSkill
        setWeaponSkill = function(self, skill) end,
	---default
        ---| Sets the characters velocity 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setVelocity = function(self, x, y, z) end,
	---default
        ---| Gets the characters velocity 
	---@param self Char
	---@return float x
	---@return float y
	---@return float z
        getVelocity = function(self) end,
	---default
        ---| Sets the characters rotation 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setRotation = function(self, x, y, z) end,
	---default
        ---| Decreases the characters health 
	---@param self Char
	---@param amount int
	---@param damageArmour bool
        damage = function(self, amount, damageArmour) end,
	---default
        ---| Sets whether the character can crouch 
	---@param self Char
	---@param state bool
        setAllowedToDuck = function(self, state) end,
	---default
        ---| Sets the interior that the char is in 
	---@param self Char
	---@param areaId int
        setAreaVisible = function(self, areaId) end,
	---default
        ---| Sets whether the character will drop any of their weapons when they die 
	---@param self Char
	---@param state bool
        setDropsWeaponsWhenDead = function(self, state) end,
	---default
        ---| Prevents the character from leaving their group 
	---@param self Char
	---@param state bool
        setNeverLeavesGroup = function(self, state) end,
	---default
        ---| Sets the heading limit for a character attached to an object or vehicle 
	---@param self Char
	---@param heading Facing
	---@param headingRange float
        setHeadingLimitForAttached = function(self, heading, headingRange) end,
	---default
        ---|  
	---@param self Char
	---@return float x
	---@return float y
	---@return float z
        getCoordinatesOfDied = function(self) end,
	---default
        ---| Gets the ped type of the character 
	---@param self Char
	---@return PedType pedType
        getPedType = function(self) end,
	---default
        ---| Locates the entry/exit marker in the specified radius of the specified coordinates and links it to the character, also setting the appropriate interior ID for the character and setting the appropriate sky color if the character is player-controlled 
	---@param self Char
	---@param x float
	---@param y float
	---@param radius float
        setHasUsedEntryExit = function(self, x, y, radius) end,
	---default
        ---| Sets the characters max health 
	---@param self Char
	---@param maxHealth int
        setMaxHealth = function(self, maxHealth) end,
	---default
        ---| Sets whether the character always stays on bike in collisions 
	---@param self Char
	---@param stayOnBike bool
        setCanBeKnockedOffBike = function(self, stayOnBike) end,
	---default
        ---| Sets the character's coordinates without warping the rest of their group 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesDontWarpGang = function(self, x, y, z) end,
	---default
        ---| Specifies that the character should only use upper-body damage animations, meaning they can still run if shot in the legs etc 
	---@param self Char
	---@param state bool
        setBulletproofVest = function(self, state) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        setUsesUpperbodyDamageAnimsOnly = function(self, state) end,
	---default
        ---| Works similar to 05C1, but returns which phrase was spoken and is not run as a task 
	---@param self Char
	---@param phrase SpeechId
	---@return int _p3
        setSayContext = function(self, phrase) end,
	---default
        ---| Gets the name of the characters interior 
	---@param self Char
	---@return string interiorName
        getNameOfEntryExitUsed = function(self) end,
	---default
        ---| Returns the coordinates and heading of the entry (enex) marker the character used to get to the current interior 
	---@param self Char
	---@return float x
	---@return float y
	---@return float z
	---@return float heading
        getPositionOfEntryExitCharUsed = function(self) end,
	---default
        ---| Returns true if the character is playing any speech 
	---@param self Char
	---@return bool
        isTalking = function(self) end,
	---default
        ---| Prevents any character speech from playing 
	---@param self Char
	---@param stopNow bool
        disableSpeech = function(self, stopNow) end,
	---default
        ---| Enables pain audio if it was disabled using 094E 
	---@param self Char
        enableSpeech = function(self) end,
	---default
        ---| Returns true if the char is stuck under a car 
	---@param self Char
	---@return bool
        isStuckUnderCar = function(self) end,
	---default
        ---| Sets whether the character should keep their tasks after mission cleanup (basically cleanup will be skipped for this character) 
	---@param self Char
	---@param state bool
        setKeepTask = function(self, state) end,
	---default
        ---|  
	---@param self Char
	---@return bool
        isSwimming = function(self) end,
	---default
        ---|  
	---@param self Char
	---@return SwimState state
        getSwimState = function(self) end,
	---default
        ---| Makes a character move their mouth as if they were talking 
	---@param self Char
	---@param duration int
        startFacialTalk = function(self, duration) end,
	---default
        ---| Stops the character moving their mouth as if they were talking 
	---@param self Char
        stopFacialTalk = function(self) end,
	---default
        ---| Puts the characters at the coordinates by the center of body instead of the feet 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesNoOffset = function(self, x, y, z) end,
	---default
        ---| Makes a character remain in the car upon death 
	---@param self Char
	---@param state bool
        setForceDieInCar = function(self, state) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        dropSecondObject = function(self, state) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        setDruggedUp = function(self, state) end,
	---default
        ---| Returns true if the character has had its head shot off 
	---@param self Char
	---@return bool
        isHeadMissing = function(self) end,
	---default
        ---| Returns true if the specified character is in a train 
	---@param self Char
	---@return bool
        isInAnyTrain = function(self) end,
	---default
        ---| Sets whether the character signals after killing 
	---@param self Char
	---@param state bool
        setSignalAfterKill = function(self, state) end,
	---default
        ---| Sets whether police should chase the character 
	---@param self Char
	---@param state bool
        setWantedByPolice = function(self, state) end,
	---default
        ---| This command is a combination of 0972 and 08C7 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesDontWarpGangNoOffset = function(self, x, y, z) end,
	---default
        ---| Returns true if the character is using a map attractor 
	---@param self Char
	---@return bool
        isUsingMapAttractor = function(self) end,
	---default
        ---| Removes the character from the vehicle 
	---@param self Char
	---@param vehicle Car
        removeFromCarMaintainPosition = function(self, vehicle) end,
	---default
        ---|  
	---@param self Char
	---@param phrase SpeechId
	---@param overrideSilence bool
	---@param ignoreMute bool
	---@param frontEnd bool
	---@return int saidVariant
        setSayContextImportant = function(self, phrase, overrideSilence, ignoreMute, frontEnd) end,
	---default
        ---|  
	---@param self Char
	---@param _p2 int
	---@param _p3 bool
	---@param _p4 bool
	---@param _p5 bool
        setSayScript = function(self, _p2, _p3, _p4, _p5) end,
	---default
        ---| Returns true if the character is entering a car, but is not in the car 
	---@param self Char
	---@return bool
        isGettingInToACar = function(self) end,
	---default
        ---| Returns the interior ID that the character is in 
	---@param self Char
	---@return int areaId
        getAreaVisible = function(self) end,
	---default
        ---| Returns true if the character can see the other character in front of them 
	---@param self Char
	---@param handle Char
	---@return bool
        hasSpottedCharInFront = function(self, handle) end,
	---default
        ---|  
	---@param self Char
	---@param state bool
        ignoreHeightDifferenceFollowingNodes = function(self, state) end,
	---default
        ---| Controls whether the character will try to exit an upside-down car until it is on fire 
	---@param self Char
	---@param state bool
        setGetOutUpsideDownCar = function(self, state) end,
	---default
        ---| Works similar to 0489, but mutes more things, including ambient speeches (needs confirming) 
	---@param self Char
	---@param state bool
        shutUpForScriptedSpeech = function(self, state) end,
	---default
        ---| Returns true if the character is touching the other character 
	---@param self Char
	---@param other Char
	---@return bool
        isTouchingChar = function(self, other) end,
	---default
        ---| Prevents pickups, which are created when this character dies, from disappearing until picked up by the player 
	---@param self Char
	---@param state bool
        setDeathWeaponsPersist = function(self, state) end,
	---default
        ---| Sets the speed that the character swims at, changing their swimming animation speed 
	---@param self Char
	---@param speed float
        setSwimSpeed = function(self, speed) end,
	---default
        ---| Returns true if the char is turreted on any vehicle 
	---@param self Char
	---@return bool
        isAttachedToAnyCar = function(self) end,
	---default
        ---| Returns the vehicle the character is attached to 
	---@param self Char
	---@return Car handle
        storeCarIsAttachedToNoSave = function(self) end,
	---CLEO
        ---| Stores the handles of a vehicle and ped closest to the char or -1 otherwise. Ignores script created entities 
	---@param self Char
	---@return Car carHandle
	---@return Char charHandle
        storeClosestEntities = function(self) end,
	---CLEO+
        ---| Returns the address of the character's specified bone matrix 
	---@param self Char
	---@param pedBone PedBone
	---@return bool
	---@return int matrix
        getBoneMatrix = function(self, pedBone) end,
	---CLEO+
        ---| Set's the specified character's transparency alpha 
	---@param self Char
	---@param alpha int
        setModelAlpha = function(self, alpha) end,
	---CLEO+
        ---|  
	---@param self Char
	---@param pedBone PedBone
	---@return bool
	---@return int address
        getBone = function(self, pedBone) end,
	---CLEO+
        ---|  
	---@param pedBone PedBone
	---@return int offsetVector
        getBoneOffsetVector = function(pedBone) end,
	---CLEO+
        ---| Returns pointer to the quaterion under bone address obtained with GET_CHAR_BONE 
	---@param bone int
	---@return int quat
        getBoneQuat = function(bone) end,
	---CLEO+
        ---| Returns the character's maximum health (08AF) 
	---@param self Char
	---@return float maxHealth
        getMaxHealth = function(self) end,
	---CLEO+
        ---| Returns true if the specified character is controlled by script 
	---@param self Char
	---@return bool
        isScriptControlled = function(self) end,
	---CLEO+
        ---| Marks the character as script controlled 
	---@param self Char
        markAsNeeded = function(self) end,
	---CLEO+
        ---| Inits additional variables for this char. Identifier can be "AUTO" for unique ID based on script pointer 
	---@param self Char
	---@param identifier string
	---@param totalVars int
	---@return bool
        initExtendedVars = function(self, identifier, totalVars) end,
	---CLEO+
        ---| Sets extended var value for this char. Requires initialization (0E14), otherwise returns false 
	---@param self Char
	---@param identifier string
	---@param varNumber int
	---@param value any
	---@return bool
        setExtendedVar = function(self, identifier, varNumber, value) end,
	---CLEO+
        ---| Gets extended var value for this char. Returns false if not initialized (0E14) 
	---@param self Char
	---@param identifier string
	---@param varNumber int
	---@return bool
	---@return any value
        getExtendedVar = function(self, identifier, varNumber) end,
	---CLEO+
        ---|  
	---@param self Char
	---@param fixGround bool
	---@param fixBrightness bool
	---@param fadeIn bool
        fixGroundBrightnessAndFadeIn = function(self, fixGround, fixBrightness, fadeIn) end,
	---CLEO+
        ---| Sets the renderObject to AutoHide if the character is dead, is using a weapon, or enters a car 
	---@param self Char
	---@param dead bool
	---@param weapon bool
	---@param car bool
        setRenderObjectAutoHide = function(self, dead, weapon, car) end,
	---CLEO+
        ---| Sets the character's coordinates without exception protocols 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesSimple = function(self, x, y, z) end,
	---CLEO+
        ---| Returns true if the character is performing the specified task 
	---@param self Char
	---@param taskId TaskId
	---@return bool
        isDoingTaskId = function(self, taskId) end,
	---CLEO+
        ---| Returns the address of the character's task by taskId 
	---@param self Char
	---@param taskId TaskId
	---@return bool
	---@return int address
        getTaskPointerById = function(self, taskId) end,
	---CLEO+
        ---| Returns the handle of the killTarget of the specified character 
	---@param self Char
	---@return bool
	---@return Char killTarget
        getKillTargetChar = function(self) end,
	---CLEO+
        ---| Returns true if the specified character is using a gun 
	---@param self Char
	---@return bool
        isUsingGun = function(self) end,
	---CLEO+
        ---| Returns true if the specified character is fighting 
	---@param self Char
	---@return bool
        isFighting = function(self) end,
	---CLEO+
        ---| Returns true if the specified character has fallen on the ground 
	---@param self Char
	---@return bool
        isFallenOnGround = function(self) end,
	---CLEO+
        ---| Returns true if the specified character is entering any car 
	---@param self Char
	---@return bool
        isEnteringAnyCar = function(self) end,
	---CLEO+
        ---| Returns true if the specified character is exiting any car 
	---@param self Char
	---@return bool
        isExitingAnyCar = function(self) end,
	---CLEO+
        ---| Returns true if the specified character is playing any script animation 
	---@param self Char
	---@param includeAnims bool
	---@return bool
        isPlayingAnyScriptAnimation = function(self, includeAnims) end,
	---CLEO+
        ---| Returns true if the specified character is doing any important task 
	---@param self Char
	---@param includeAnims bool
	---@return bool
        isDoingAnyImportantTask = function(self, includeAnims) end,
	---CLEO+
        ---| Returns the character's health as a floating-point percentage 
	---@param self Char
	---@return float healthPercent
        getHealthPercent = function(self) end,
	---CLEO+
        ---| Returns a pointer to the character's current weaponInfo struct 
	---@param self Char
	---@return bool
	---@return WeaponInfo handle
        getCurrentWeaponinfo = function(self) end,
	---CLEO+
        ---| Returns the character's current weaponState 
	---@param self Char
	---@return WeaponState weaponState
        getWeaponState = function(self) end,
	---CLEO+
        ---| Returns the character's current WeaponClip 
	---@param self Char
	---@return int weaponClip
        getCharWeaponClip = function(self) end,
	---CLEO+
        ---| Returns the specified character's collision surfaceType 
	---@param self Char
	---@return SurfaceType surfaceType
        getCollisionSurface = function(self) end,
	---CLEO+
        ---| Returns the specified character's collision lighting 
	---@param self Char
	---@return float lighting
        getCollisionLighting = function(self) end,
	---CLEO+
        ---| Returns True when the character is really in air, including while using a parachute or jetpack 
	---@param self Char
	---@return bool
        isReallyInAir = function(self) end,
	---CLEO+
        ---| Clears the specified character's primary tasks 
	---@param self Char
        clearPrimaryTasks = function(self) end,
	---CLEO+
        ---| Clears the character's secondary tasks 
	---@param self Char
        clearSecondaryTasks = function(self) end,
	---CLEO+
        ---| Sets this char as controlled by player two 
	---@param self Char
	---@param enableCamera bool
	---@param separateCars bool
        setSecondPlayer = function(self, enableCamera, separateCars) end,
	---CLEO+
        ---| Returns True if the character is on fire 
	---@param self Char
	---@return bool
        isOnFire = function(self) end,
	---CLEO+
        ---| Returns the handle of the closestCop to the specified character 
	---@param self Char
	---@param radius float
	---@param alive bool
	---@param inCar bool
	---@param onFoot bool
	---@param seenInFront bool
	---@return bool
	---@return Char closestCop
        getClosestCop = function(self, radius, alive, inCar, onFoot, seenInFront) end,
	---CLEO+
        ---| Sets the specified character's pedState to Arrested 
	---@param self Char
        setArrested = function(self) end,
	---CLEO+
        ---| Returns the specified character's pedState 
	---@param self Char
	---@return PedState pedState
        getPedState = function(self) end,
	---CLEO+
        ---| Returns the specified character's damage proofs 
	---@param self Char
	---@return bool bullet
	---@return bool fire
	---@return bool explosion
	---@return bool collision
	---@return bool melee
        getProofs = function(self) end,
	---CLEO+
        ---| Returns true if the specified character has weapon visible set 
	---@param self Char
	---@return bool
        isWeaponVisibleSet = function(self) end,
	---CLEO+
        ---| Returns the specified character's pedStat ID (data\pedstats.dat) 
	---@param self Char
	---@return PedStat pedStat
        getStatId = function(self) end,
	---CLEO+
        ---| Returns the character's damaging entity and weaponType, bodyPart damaged, and intensity of damage last frame 
	---@param self Char
	---@return bool
	---@return Char entity
	---@return WeaponType weaponType
	---@return BodyPart bodyPart
	---@return float intensity
        getDamageLastFrame = function(self) end,
	---CLEO+
        ---| Returns the randomSeed of the specified character 
	---@param self Char
	---@return int randomSeed
        getRandomSeed = function(self) end,
	---CLEO+
        ---|  
	---@param self Char
	---@return MoveState moveState
        getMoveState = function(self) end,
	---CLEO+
        ---| Prevents the deletion of the specified character until MsFromNow 
	---@param self Char
	---@param msFromNow int
        dontDeleteUntilTime = function(self, msFromNow) end,
	---CLEO+
        ---| Returns the TimeIsDead in milliseconds since the specified character is dead 
	---@param self Char
	---@return int timeIsDead
        getTimeIsDead = function(self) end,
	---CLEO+
        ---|  
	---@param self Char
	---@param state bool
        setIgnoreDamageAnims = function(self, state) end,
	---CLEO+
        ---| Returns True if the character(self) is within the radius of the specified character 
	---@param self Char
	---@param character Char
	---@param radius float
	---@return bool
        locateDistanceToChar = function(self, character, radius) end,
	---CLEO+
        ---| Returns True if the character is within the radius of the specified car 
	---@param self Char
	---@param car Car
	---@param radius float
	---@return bool
        locateDistanceToCar = function(self, car, radius) end,
	---CLEO+
        ---| Returns True if the character is within the radius of the specified object 
	---@param self Char
	---@param object Object
	---@param radius float
	---@return bool
        locateDistanceToObject = function(self, object, radius) end,
	---CLEO+
        ---| Returns True if the character is within the radius of the specified coordinates 
	---@param self Char
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return bool
        locateDistanceToCoordinates = function(self, x, y, z, radius) end,
	---CLEO+
        ---| Returns the specified character's fear level (see pedstats.dat) 
	---@param self Char
	---@return int fear
        getFear = function(self) end,
	---CLEO+
        ---| Returns the character's simplest active taskId and the address of the task 
	---@param self Char
	---@return bool
	---@return TaskId taskId
	---@return int address
        getSimplestActiveTask = function(self) end,
	---CLEO+
        ---|  
	---@param self Char
	---@param specialModel int
	---@param pedBone PedBone
	---@param x float
	---@param y float
	---@param z float
	---@param rx float
	---@param ry float
	---@param rz float
	---@return int renderobject
        createRenderObjectToCharBoneFromSpecial = function(self, specialModel, pedBone, x, y, z, rx, ry, rz) end,
	---SAMPFUNCS
        ---| Returns the samp player's id controlling the character handle. Returns -1 if the character isn't controlled by any Player 
	---@param self Char
	---@return SampPlayer idAsHandle
        getSampId = function(self) end,
    }
CarOp = {
        ---default
        ---| Creates a vehicle at the specified location, with the specified model 
	---@param modelId model_vehicle
	---@param x float
	---@param y float
	---@param z float
	---@return Car handle
        create = function(modelId, x, y, z) end,
	---default
        ---| Removes the vehicle from the game 
	---@param self Car
        delete = function(self) end,
	---default
        ---| Makes the AI drive to the specified location by any means 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        gotoCoordinates = function(self, x, y, z) end,
	---default
        ---| Clears any current tasks the vehicle has and makes it drive around aimlessly 
	---@param self Car
        wanderRandomly = function(self) end,
	---default
        ---| Sets the car's mission to idle (MISSION_NONE), stopping any driving activity 
	---@param self Car
        setIdle = function(self) end,
	---default
        ---| Returns the vehicle's coordinates 
	---@param self Car
	---@return float x
	---@return float y
	---@return float z
        getCoordinates = function(self) end,
	---default
        ---| Puts the vehicle at the specified location 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        setCoordinates = function(self, x, y, z) end,
	---default
        ---| Sets the vehicle's max speed 
	---@param self Car
	---@param maxSpeed float
        setCruiseSpeed = function(self, maxSpeed) end,
	---default
        ---| Sets the behavior of the vehicle's AI driver 
	---@param self Car
	---@param drivingStyle DrivingMode
        setDrivingStyle = function(self, drivingStyle) end,
	---default
        ---| Sets the mission of the vehicle's AI driver 
	---@param self Car
	---@param carMission CarMission
        setMission = function(self, carMission) end,
	---default
        ---| Returns true if the vehicle is located within the specified 2D area 
	---@param self Car
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isInArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the vehicle is located within the specified 3D area 
	---@param self Car
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isInArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Returns true if the handle is an invalid vehicle handle or the vehicle has been destroyed (wrecked) 
	---@param handle any
	---@return bool
        isDead = function(handle) end,
	---default
        ---| Returns true if the vehicle has the specified model 
	---@param self Car
	---@param modelId model_vehicle
	---@return bool
        isModel = function(self, modelId) end,
	---default
        ---| Returns the vehicle's heading (z-angle) 
	---@param self Car
	---@return float heading
        getHeading = function(self) end,
	---default
        ---| Sets the vehicle's heading (z-angle) 
	---@param self Car
	---@param heading float
        setHeading = function(self, heading) end,
	---default
        ---| Returns true if the car's health is over the specified value 
	---@param self Car
	---@param health int
	---@return bool
        isHealthGreater = function(self, health) end,
	---default
        ---| Returns true if the car has been upside down for more than 2 seconds (requires 0190) 
	---@param self Car
	---@return bool
        isStuckOnRoof = function(self) end,
	---default
        ---| Activates upside-down car check for the car 
	---@param self Car
        addUpsidedownCheck = function(self) end,
	---default
        ---| Deactivates upside-down car check (0190) for the car 
	---@param self Car
        removeUpsidedownCheck = function(self) end,
	---default
        ---|  
	---@param self Car
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isStoppedInArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---|  
	---@param self Car
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isStoppedInArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---|  
	---@param self Car
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locate2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---|  
	---@param self Car
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locateStopped2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---|  
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locate3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the car is stopped in the radius of the specified point 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locateStopped3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the vehicle is not moving 
	---@param self Car
	---@return bool
        isStopped = function(self) end,
	---default
        ---| Allows the vehicle to be deleted by the game if necessary, and also removes it from the mission cleanup list, if applicable 
	---@param self Car
        markAsNoLongerNeeded = function(self) end,
	---default
        ---| Returns the number of passengers sitting in the car 
	---@param self Car
	---@return int count
        getNumberOfPassengers = function(self) end,
	---default
        ---| Returns the maximum number of passengers that could sit in the car 
	---@param self Car
	---@return int count
        getMaximumNumberOfPassengers = function(self) end,
	---default
        ---| Sets whether the car is heavy 
	---@param self Car
	---@param state bool
        setHeavy = function(self, state) end,
	---default
        ---| Returns true if the vehicle is in the air 
	---@param self Car
	---@return bool
        isInAirProper = function(self) end,
	---default
        ---| Returns true if the car is upside down 
	---@param self Car
	---@return bool
        isUpsidedown = function(self) end,
	---default
        ---| Sets the locked status of the car's doors 
	---@param self Car
	---@param lockStatus CarLock
        lockDoors = function(self, lockStatus) end,
	---default
        ---| Makes the vehicle explode 
	---@param self Car
        explode = function(self) end,
	---default
        ---| Returns true if the vehicle is in the normal position (upright) 
	---@param self Car
	---@return bool
        isUpright = function(self) end,
	---default
        ---| Sets whether the taxi's roof light is on 
	---@param self Car
	---@param state bool
        setTaxiLights = function(self, state) end,
	---default
        ---| Sets the vehicle's health 
	---@param self Car
	---@param health int
        setHealth = function(self, health) end,
	---default
        ---| Returns the vehicle's health 
	---@param self Car
	---@return int health
        getHealth = function(self) end,
	---default
        ---| Sets the car's primary and secondary colors 
	---@param self Car
	---@param color1 int
	---@param color2 int
        changeColor = function(self, color1, color2) end,
	---default
        ---| Makes car keep current colors when Pay'n'Spray is used 
	---@param self Car
	---@param changeColors bool
        setCanRespray = function(self, changeColors) end,
	---default
        ---| Makes a vehicle immune to everything except the player 
	---@param self Car
	---@param state bool
        setOnlyDamagedByPlayer = function(self, state) end,
	---default
        ---| Sets the vehicle's immunities 
	---@param self Car
	---@param bulletProof bool
	---@param fireProof bool
	---@param explosionProof bool
	---@param collisionProof bool
	---@param meleeProof bool
        setProofs = function(self, bulletProof, fireProof, explosionProof, collisionProof, meleeProof) end,
	---default
        ---| Returns true if the vehicle is submerged in water 
	---@param self Car
	---@return bool
        isInWater = function(self) end,
	---default
        ---| Makes the AI drive to the specified location obeying the traffic rules 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        gotoCoordinatesAccurate = function(self, x, y, z) end,
	---default
        ---| Returns true if the car is visible 
	---@param self Car
	---@return bool
        isOnScreen = function(self) end,
	---default
        ---| Gets the car's speed 
	---@param self Car
	---@return float speed
        getSpeed = function(self) end,
	---default
        ---| Returns the X coord of the vehicle's angle 
	---@param self Car
	---@return float x
        getForwardX = function(self) end,
	---default
        ---| Returns the Y coord of the vehicle's angle 
	---@param self Car
	---@return float y
        getForwardY = function(self) end,
	---default
        ---| Returns true if the vehicle has been hit by the specified weapon 
	---@param self Car
	---@param weaponType WeaponType
	---@return bool
        hasBeenDamagedByWeapon = function(self, weaponType) end,
	---default
        ---| Sets whether the vehicle is visible or not 
	---@param self Car
	---@param state bool
        setVisible = function(self, state) end,
	---default
        ---| Sets whether the car's alarm can be activated 
	---@param self Car
	---@param state bool
        switchSiren = function(self, state) end,
	---default
        ---| Makes the vehicle watertight, meaning characters inside will not be harmed if the vehicle is submerged in water 
	---@param self Car
	---@param state bool
        setWatertight = function(self, state) end,
	---default
        ---| Sets the car's heading so that it is facing the 2D coordinate 
	---@param self Car
	---@param x float
	---@param y float
        turnToFaceCoord = function(self, x, y) end,
	---default
        ---| Sets the car's status 
	---@param self Car
	---@param status EntityStatus
        setStatus = function(self, status) end,
	---default
        ---| Defines whether the car is more resistant to collisions than normal 
	---@param self Car
	---@param state bool
        setStrong = function(self, state) end,
	---default
        ---| Returns true if any of the car components is visibly damaged or lost 
	---@param self Car
	---@return bool
        isVisiblyDamaged = function(self) end,
	---default
        ---| Disables the car from exploding when it is upside down, as long as the player is not in the vehicle 
	---@param self Car
	---@param state bool
        setUpsidedownNotDamaged = function(self, state) end,
	---default
        ---| Gets the car's primary and secondary colors 
	---@param self Car
	---@return int color1
	---@return int color2
        getColors = function(self) end,
	---default
        ---| Sets whether the car receives damage 
	---@param self Car
	---@param state bool
        setCanBeDamaged = function(self, state) end,
	---default
        ---| Returns the coordinates of an offset of the vehicle's position, depending on the vehicle's rotation 
	---@param self Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@return float x
	---@return float y
	---@return float z
        getOffsetInWorldCoords = function(self, xOffset, yOffset, zOffset) end,
	---default
        ---| Overrides the default AI controlled vehicle traction value of 1.0 
	---@param self Car
	---@param traction float
        setTraction = function(self, traction) end,
	---default
        ---| Sets whether the vehicle will avoid paths between levels (0426) 
	---@param self Car
	---@param state bool
        setAvoidLevelTransitions = function(self, state) end,
	---default
        ---| Returns true if the specified car seat is empty 
	---@param self Car
	---@param seat SeatId
	---@return bool
        isPassengerSeatFree = function(self, seat) end,
	---default
        ---| Returns the handle of a character sitting in the specified car seat 
	---@param self Car
	---@param seat SeatId
	---@return Char handle
        getCharInPassengerSeat = function(self, seat) end,
	---default
        ---| Returns the car's model id 
	---@param self Car
	---@return model_vehicle modelId
        getModel = function(self) end,
	---default
        ---|  
	---@param self Car
	---@param state bool
        setStayInFastLane = function(self, state) end,
	---default
        ---| Clears the vehicle's last weapon damage (see 031E) 
	---@param self Car
        clearLastWeaponDamage = function(self) end,
	---default
        ---| Returns the car's driver handle 
	---@param self Car
	---@return Char handle
        getDriver = function(self) end,
	---default
        ---| Makes the AI driver perform the action in the vehicle for the specified period of time 
	---@param self Car
	---@param actionId TempAction
	---@param time int
        setTempAction = function(self, actionId, time) end,
	---default
        ---| Sets the car on a specific route 
	---@param self Car
	---@param routeSeed int
        setRandomRouteSeed = function(self, routeSeed) end,
	---default
        ---| Returns true if the car is burning 
	---@param self Car
	---@return bool
        isOnFire = function(self) end,
	---default
        ---| Returns true if a given tire on the car is deflated 
	---@param self Car
	---@param tireId WheelId
	---@return bool
        isTireBurst = function(self, tireId) end,
	---default
        ---| Sets the speed of the car 
	---@param self Car
	---@param forwardSpeed float
        setForwardSpeed = function(self, forwardSpeed) end,
	---default
        ---| Marks the car as being part of a convoy, which seems to follow a path set by 0994 
	---@param self Car
	---@param state bool
        markAsConvoyCar = function(self, state) end,
	---default
        ---| Sets the minimum distance for the AI driver to start ignoring car paths and go straight to the target 
	---@param self Car
	---@param distance int
        setStraightLineDistance = function(self, distance) end,
	---default
        ---| Opens the car's trunk and keeps it open 
	---@param self Car
        popBoot = function(self) end,
	---default
        ---|  
	---@param self Car
	---@return bool
        isWaitingForWorldCollision = function(self) end,
	---default
        ---| Deflates the car's tire 
	---@param self Car
	---@param tireId WheelId
        burstTire = function(self, tireId) end,
	---default
        ---| Sets the variation of the next car to be created 
	---@param _unused model_vehicle
	---@param component1 int
	---@param component2 int
        setModelComponents = function(_unused, component1, component2) end,
	---default
        ---| Closes all car doors, hoods and boots 
	---@param self Car
        closeAllDoors = function(self) end,
	---default
        ---| Locks the vehicle's position 
	---@param self Car
	---@param state bool
        freezePosition = function(self, state) end,
	---default
        ---| Returns true if the car has been damaged by the specified char 
	---@param self Car
	---@param handle Char
	---@return bool
        hasBeenDamagedByChar = function(self, handle) end,
	---default
        ---| Returns true if the vehicle has been damaged by another specified vehicle 
	---@param self Car
	---@param other Car
	---@return bool
        hasBeenDamagedByCar = function(self, other) end,
	---default
        ---| Sets whether the car's tires can be deflated 
	---@param self Car
	---@param state bool
        setCanBurstTires = function(self, state) end,
	---default
        ---| Clears the car's last damage entity 
	---@param self Car
        clearLastDamageEntity = function(self) end,
	---default
        ---| Returns true if the handle is a valid vehicle handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---default
        ---| Makes the car maintain its position 
	---@param self Car
	---@param state bool
        freezePositionAndDontLoadCollision = function(self, state) end,
	---default
        ---|  
	---@param self Car
	---@param state bool
        setLoadCollisionFlag = function(self, state) end,
	---default
        ---| Sets the alpha transparency of a distant vehicle 
	---@param self Car
	---@param alpha int
        setToFadeIn = function(self, alpha) end,
	---default
        ---| Assigns a car to a path 
	---@param self Car
	---@param path int
        startPlayback = function(self, path) end,
	---default
        ---| Stops car from following path 
	---@param self Car
        stopPlayback = function(self) end,
	---default
        ---| Freezes the car on its path 
	---@param self Car
        pausePlayback = function(self) end,
	---default
        ---| Unfreezes the vehicle on its path 
	---@param self Car
        unpausePlayback = function(self) end,
	---default
        ---| Makes the vehicle stay on the other vehicle's left side, keeping parallel 
	---@param self Car
	---@param handle Car
        setEscortCarLeft = function(self, handle) end,
	---default
        ---| Makes the vehicle stay by the right side of the other vehicle, keeping parallel 
	---@param self Car
	---@param handle Car
        setEscortCarRight = function(self, handle) end,
	---default
        ---| Makes the vehicle stay behind the other car, keeping parallel 
	---@param self Car
	---@param handle Car
        setEscortCarRear = function(self, handle) end,
	---default
        ---| Makes the vehicle stay in front of the other, keeping parallel 
	---@param self Car
	---@param handle Car
        setEscortCarFront = function(self, handle) end,
	---default
        ---| Returns true if the car is assigned to a path 
	---@param self Car
	---@return bool
        isPlaybackGoingOn = function(self) end,
	---default
        ---| Opens the specified car door 
	---@param self Car
	---@param door CarDoor
        openDoor = function(self, door) end,
	---default
        ---| Sets the numberplate of the next car to be spawned with the specified model 
	---@param modelId model_vehicle
	---@param text string
        customPlateForNextCar = function(modelId, text) end,
	---default
        ---| Sets an override for the car's lights 
	---@param self Car
	---@param lightMode CarLights
        forceLights = function(self, lightMode) end,
	---default
        ---|  
	---@param self Car
	---@param handle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
        attachToCar = function(self, handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation) end,
	---default
        ---|  
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
	---@param collisionDetection bool
        detach = function(self, x, y, z, collisionDetection) end,
	---default
        ---|  
	---@param self Car
	---@return bool
        isAttached = function(self) end,
	---default
        ---| Removes the specified car door component from the car 
	---@param self Car
	---@param door CarDoor
	---@param visibility bool
        popDoor = function(self, door, visibility) end,
	---default
        ---| Repairs the car door 
	---@param self Car
	---@param door CarDoor
        fixDoor = function(self, door) end,
	---default
        ---| Makes all passengers of the car leave it 
	---@param self Car
        taskEveryoneLeave = function(self) end,
	---default
        ---|  
	---@param self Car
	---@param panelId int
	---@param visibility bool
        popPanel = function(self, panelId, visibility) end,
	---default
        ---|  
	---@param self Car
	---@param panelId int
        fixPanel = function(self, panelId) end,
	---default
        ---| Repairs a car's tire 
	---@param self Car
	---@param tireId WheelId
        fixTire = function(self, tireId) end,
	---default
        ---|  
	---@param self Car
	---@return float x
	---@return float y
	---@return float z
        getSpeedVector = function(self) end,
	---default
        ---| Returns the vehicle's mass 
	---@param self Car
	---@return float mass
        getMass = function(self) end,
	---default
        ---| Returns the Y Angle of the vehicle 
	---@param self Car
	---@return float angle
        getRoll = function(self) end,
	---default
        ---|  
	---@param self Car
        skipToEndAndStopPlayback = function(self) end,
	---default
        ---| Returns a model id available for the vehicle's mod slot, or -1 otherwise 
	---@param self Car
	---@param slotId ModSlot
	---@return model_object modelId
        getAvailableMod = function(self, slotId) end,
	---default
        ---| Adds a new mod with the model to the vehicle 
	---@param self Car
	---@param modelId model_object
	---@return int handle
        addMod = function(self, modelId) end,
	---default
        ---| Removes the vehicle's mod with the specified model 
	---@param self Car
	---@param modelId model_object
        removeMod = function(self, modelId) end,
	---default
        ---| Gets the number of possible paintjobs that can be applied to the car 
	---@param self Car
	---@return int numPaintjobs
        getNumAvailablePaintjobs = function(self) end,
	---default
        ---| Sets the car's paintjob 
	---@param self Car
	---@param paintjobId int
        givePaintjob = function(self, paintjobId) end,
	---default
        ---| Returns true if the car has car stuck check enabled 
	---@param self Car
	---@return bool
        doesHaveStuckCarCheck = function(self) end,
	---default
        ---| Sets the playback speed of the car playing a car recording 
	---@param self Car
	---@param speed float
        setPlaybackSpeed = function(self, speed) end,
	---default
        ---| Makes the AI drive to the destination as fast as possible, trying to overtake other vehicles 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        gotoCoordinatesRacing = function(self, x, y, z) end,
	---default
        ---| Starts the playback of a recorded car with driver AI enabled 
	---@param self Car
	---@param pathId int
        startPlaybackUsingAi = function(self, pathId) end,
	---default
        ---| Advances the recorded car playback by the specified amount 
	---@param self Car
	---@param amount float
        skipInPlayback = function(self, amount) end,
	---default
        ---| Makes the vehicle explode without affecting its surroundings 
	---@param self Car
        explodeInCutscene = function(self) end,
	---default
        ---|  
	---@param self Car
	---@param state bool
        setStayInSlowLane = function(self, state) end,
	---default
        ---| Damages a panel on the car 
	---@param self Car
	---@param panelId int
        damagePanel = function(self, panelId) end,
	---default
        ---| Sets the Y Angle of the vehicle to the specified value 
	---@param self Car
	---@param yAngle float
        setRoll = function(self, yAngle) end,
	---default
        ---| Sets whether the vehicle will drive the wrong way on roads 
	---@param self Car
	---@param state bool
        setCanGoAgainstTraffic = function(self, state) end,
	---default
        ---| Damages a component on the vehicle 
	---@param self Car
	---@param door CarDoor
        damageDoor = function(self, door) end,
	---default
        ---| Sets the script as the owner of the vehicle and adds it to the mission cleanup list 
	---@param self Car
        setAsMissionCar = function(self) end,
	---default
        ---| Returns the X Angle of the vehicle 
	---@param self Car
	---@return float angle
        getPitch = function(self) end,
	---default
        ---| Gets the quaternion values of the car 
	---@param self Car
	---@return float x
	---@return float y
	---@return float z
	---@return float w
        getQuaternion = function(self) end,
	---default
        ---| Sets the rotation of a vehicle using quaternion values 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
	---@param w float
        setQuaternion = function(self, x, y, z, w) end,
	---default
        ---|  
	---@param self Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
        applyForce = function(self, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation) end,
	---default
        ---|  
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        addToRotationVelocity = function(self, x, y, z) end,
	---default
        ---|  
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        setRotationVelocity = function(self, x, y, z) end,
	---default
        ---|  
	---@param self Car
	---@param state bool
        setAlwaysCreateSkids = function(self, state) end,
	---default
        ---|  
	---@param self Car
	---@param _p2 float
	---@param _p3 float
	---@param _p4 float
	---@param _p5 float
        controlHydraulics = function(self, _p2, _p3, _p4, _p5) end,
	---default
        ---|  
	---@param self Car
	---@param handle Car
	---@param radius float
        setFollowCar = function(self, handle, radius) end,
	---default
        ---| Enables hydraulic suspension on the car 
	---@param self Car
	---@param state bool
        setHydraulics = function(self, state) end,
	---default
        ---| Returns true if the car has hydraulics installed 
	---@param self Car
	---@return bool
        doesHaveHydraulics = function(self) end,
	---default
        ---| Sets whether the car's engine is broken 
	---@param self Car
	---@param state bool
        setEngineBroken = function(self, state) end,
	---default
        ---| Gets the car's vertical angle 
	---@param self Car
	---@return float value
        getUprightValue = function(self) end,
	---default
        ---|  
	---@param self Car
	---@param interiorId int
        setAreaVisible = function(self, interiorId) end,
	---default
        ---| Sets the vehicle to use its secondary guns 
	---@param self Car
	---@param _p2 int
        selectWeapons = function(self, _p2) end,
	---default
        ---| Sets whether the vehicle can be targeted 
	---@param self Car
	---@param state bool
        setCanBeTargeted = function(self, state) end,
	---default
        ---| Sets whether the vehicle can be visibly damaged 
	---@param self Car
	---@param state bool
        setCanBeVisiblyDamaged = function(self, state) end,
	---default
        ---| Starts looped playback of a recorded car path 
	---@param self Car
	---@param pathId int
        startPlaybackLooped = function(self, pathId) end,
	---default
        ---| Sets the dirt level of the car 
	---@param self Car
	---@param level float
        setDirtLevel = function(self, level) end,
	---default
        ---| Sets the air resistance for the vehicle 
	---@param self Car
	---@param multiplier float
        setAirResistanceMultiplier = function(self, multiplier) end,
	---default
        ---| Sets the vehicle coordinates without applying offsets to account for the height of the vehicle 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesNoOffset = function(self, x, y, z) end,
	---default
        ---| Returns true if the vehicle is in contact with the object 
	---@param self Car
	---@param handle Object
	---@return bool
        isTouchingObject = function(self, handle) end,
	---default
        ---| Sets the angle of a vehicle's extra 
	---@param self Car
	---@param range float
        controlMovablePart = function(self, range) end,
	---default
        ---| Sets whether the vehicle can be picked up using the magnocrane 
	---@param self Car
	---@param state bool
        winchCanPickUp = function(self, state) end,
	---default
        ---| Sets the angle of a car door 
	---@param self Car
	---@param door CarDoor
	---@param value float
        openDoorABit = function(self, door, value) end,
	---default
        ---|  
	---@param self Car
	---@param door CarDoor
	---@return bool
        isDoorFullyOpen = function(self, door) end,
	---default
        ---| Causes the vehicle to explode, without damage to surrounding entities 
	---@param self Car
	---@param shake bool
	---@param effect bool
	---@param sound bool
        explodeInCutsceneShakeAndBits = function(self, shake, effect, sound) end,
	---default
        ---| Returns the vehicle's class as defined in vehicles.ide 
	---@param self Car
	---@return VehicleClass class
        getClass = function(self) end,
	---default
        ---| Sets whether the player can target this vehicle with a heatseeking rocket launcher 
	---@param self Car
	---@param state bool
        canBeTargetedByHsMissile = function(self, state) end,
	---default
        ---| Sets whether the player can receive items from this vehicle, such as shotgun ammo from a police car and cash from a taxi 
	---@param self Car
	---@param state bool
        setFreebies = function(self, state) end,
	---default
        ---| Sets whether the vehicle's engine is turned on or off 
	---@param self Car
	---@param state bool
        setEngineOn = function(self, state) end,
	---default
        ---| Sets whether the vehicle's lights are on 
	---@param self Car
	---@param state bool
        setLightsOn = function(self, state) end,
	---default
        ---| Attaches the car to object with offset and rotation 
	---@param self Car
	---@param handle Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
        attachToObject = function(self, handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation) end,
	---default
        ---| Sets whether characters in combat will choose to use the vehicle as cover from gunfire 
	---@param self Car
	---@param state bool
        doesProvideCover = function(self, state) end,
	---default
        ---| Sets the car's door angle and latch state 
	---@param self Car
	---@param door CarDoor
	---@param state CarDoorState
	---@param angle float
        controlDoor = function(self, door, state, angle) end,
	---default
        ---| Gets the specified car doors angle, relative to the hinge 
	---@param self Car
	---@param door CarDoor
	---@return float ratio
        getDoorAngleRatio = function(self, door) end,
	---default
        ---| Returns true if the specified vehicle has the 'is big' flag set in vehicles 
	---@param self Car
	---@return bool
        isBig = function(self) end,
	---default
        ---|  
        storeModState = function() end,
	---default
        ---|  
        restoreModState = function() end,
	---default
        ---| Returns the model of the component installed on the specified slot of the vehicle, or -1 otherwise 
	---@param self Car
	---@param slot ModSlot
	---@return model_object modelId
        getCurrentMod = function(self, slot) end,
	---default
        ---| Returns true if the vehicle is a low rider 
	---@param self Car
	---@return bool
        isLowRider = function(self) end,
	---default
        ---| Returns true if the vehicle is a street racer 
	---@param self Car
	---@return bool
        isStreetRacer = function(self) end,
	---default
        ---| Returns true if the vehicle is an emergency vehicle 
	---@param self Car
	---@return bool
        isEmergencyServices = function(self) end,
	---default
        ---| Returns number of color variations defined for the model of this car in carcols.dat 
	---@param self Car
	---@return int count
        getNumColors = function(self) end,
	---default
        ---| Returns a handle of the vehicle preventing this car from getting to its destination 
	---@param self Car
	---@return Car handle
        getBlockingCar = function(self) end,
	---default
        ---| Gets the car's paintjob 
	---@param self Car
	---@return int paintjobNumber
        getCurrentPaintjob = function(self) end,
	---default
        ---| Sets the angle of a vehicle's extra 
	---@param self Car
	---@return float offset
        getMovingComponentOffset = function(self) end,
	---default
        ---|  
	---@param self Car
	---@param state bool
        setCollision = function(self, state) end,
	---default
        ---| Changes vehicle control from playback to AI driven 
	---@param self Car
        changePlaybackToUseAi = function(self) end,
	---default
        ---| Makes a passenger in the vehicle speak from an ambient speech ID, if one exists for the character 
	---@param self Car
	---@param phrase SpeechId
        randomPassengerSay = function(self, phrase) end,
	---default
        ---| Makes player character ignore the car when enter vehicle key is used 
	---@param self Car
	---@param state bool
        setIsConsideredByPlayer = function(self, state) end,
	---default
        ---| Returns the door lock mode of the vehicle 
	---@param self Car
	---@return CarLock lockStatus
        getDoorLockStatus = function(self) end,
	---default
        ---| Returns true if the specified vehicle part is visibly damaged 
	---@param self Car
	---@param door CarDoor
	---@return bool
        isDoorDamaged = function(self, door) end,
	---default
        ---| Sets whether the car can be blown up by shooting at the petrol tank 
	---@param self Car
	---@param state bool
        setPetrolTankWeakpoint = function(self, state) end,
	---default
        ---| Returns true if the car is touching the other car 
	---@param self Car
	---@param handle Car
	---@return bool
        isTouchingCar = function(self, handle) end,
	---default
        ---| Returns true if all the vehicle's wheels are touching the ground 
	---@param self Car
	---@return bool
        isOnAllWheels = function(self) end,
	---default
        ---| Returns the value of the specified car model 
	---@param model model_vehicle
	---@return int value
        getModelValue = function(model) end,
	---default
        ---| Makes the car have one nitro 
	---@param self Car
        giveNonPlayerNitro = function(self) end,
	---default
        ---| This resets all the hydraulics on the car, making it "sit" 
	---@param self Car
        resetHydraulics = function(self) end,
	---default
        ---| Sets the car's ternary and quaternary colors. See also 0229 
	---@param self Car
	---@param color3 int
	---@param color4 int
        setExtraColors = function(self, color3, color4) end,
	---default
        ---| Returns the car's tertiary and quaternary colors. See also 03F3 
	---@param self Car
	---@return int color3
	---@return int color4
        getExtraColors = function(self) end,
	---default
        ---| Returns true if the vehicle was resprayed in the last frame 
	---@param self Car
	---@return bool
        hasBeenResprayed = function(self) end,
	---default
        ---| Sets whether a ped driven vehicle's handling is affected by the 'perfect handling' cheat 
	---@param self Car
	---@param state bool
        improveByCheating = function(self, state) end,
	---default
        ---| Restores the vehicle to full health and removes the damage 
	---@param self Car
        fix = function(self) end,
	---CLEO
        ---| Gets the total number of gears of the vehicle and stores it to the variable 
	---@param self Car
	---@return int numGear
        getNumberOfGears = function(self) end,
	---CLEO
        ---| Returns the current gear of the vehicle 
	---@param self Car
	---@return int gear
        getCurrentGear = function(self) end,
	---CLEO
        ---|  
	---@param self Car
	---@return bool
        isSirenOn = function(self) end,
	---CLEO
        ---|  
	---@param self Car
	---@return bool
        isEngineOn = function(self) end,
	---CLEO
        ---| Sets whether the vehicle's engine is turned on or off 
	---@param self Car
	---@param state bool
        cleoSetEngineOn = function(self, state) end,
	---CLEO+
        ---| Set's the specified car's transparency alpha 
	---@param self Car
	---@param alpha int
        setModelAlpha = function(self, alpha) end,
	---CLEO+
        ---| Sets the window state of the specified door of the car 
	---@param self Car
	---@param door CarNodeDoor
	---@param state bool
        setDoorWindowState = function(self, door, state) end,
	---CLEO+
        ---| Returns the status of the car's alarm 
	---@param self Car
	---@return CarAlarm status
        getAlarm = function(self) end,
	---CLEO+
        ---| Returns true if the specified vehicle is controlled by script 
	---@param self Car
	---@return bool
        isScriptControlled = function(self) end,
	---CLEO+
        ---| Marks the vehicle as script controlled 
	---@param self Car
        markAsNeeded = function(self) end,
	---CLEO+
        ---| Returns vehicle subclass, useful to check if vehicle is motorbike, bicycle, trailer etc 
	---@param self Car
	---@return VehicleSubclass subclass
        getSubclass = function(self) end,
	---CLEO+
        ---| Inits additional variables for this car. Identifier can be "AUTO" for unique ID based on script pointer 
	---@param self Car
	---@param identifier string
	---@param totalVars int
	---@return bool
        initExtendedVars = function(self, identifier, totalVars) end,
	---CLEO+
        ---| Sets extended var value for this car. Requires initialization (0E17), otherwise returns false 
	---@param self Car
	---@param identifier string
	---@param varNumber int
	---@param value any
	---@return bool
        setExtendedVar = function(self, identifier, varNumber, value) end,
	---CLEO+
        ---| Gets extended var value for this car. Returns false if not initialized (0E17) 
	---@param self Car
	---@param identifier string
	---@param varNumber int
	---@return bool
	---@return any value
        getExtendedCarVar = function(self, identifier, varNumber) end,
	---CLEO+
        ---| Returns the handle of a trailer towed by this car 
	---@param self Car
	---@return bool
	---@return Car trailer
        getTrailer = function(self) end,
	---CLEO+
        ---| Returns the handle of a tractor towing this car 
	---@param self Car
	---@return bool
	---@return Car tractor
        getTractor = function(self) end,
	---CLEO+
        ---| Returns the coordinates of the specified car's vehicleDummy 
	---@param self Car
	---@param vehicleDummy VehicleDummy
	---@param worldCoords bool
	---@param invertX bool
	---@return bool
	---@return float x
	---@return float y
	---@return float z
        getDummyCoord = function(self, vehicleDummy, worldCoords, invertX) end,
	---CLEO+
        ---| Plays the car's horn (if the player is driving the car) 
	---@param self Car
        playHorn = function(self) end,
	---CLEO+
        ---| Sets the status of the car's alarm 
	---@param self Car
	---@param status CarAlarm
        setAlarm = function(self, status) end,
	---CLEO+
        ---| Returns the intensity of the last collision of the specified car 
	---@param self Car
	---@return bool
	---@return float intensity
        getCollisionIntensity = function(self) end,
	---CLEO+
        ---| Returns the coordinates of the last collision of the specified car 
	---@param self Car
	---@return float x
	---@return float y
	---@return float z
        getCollisionCoordinates = function(self) end,
	---CLEO+
        ---| Returns True if the car has the specified carNode part 
	---@param self Car
	---@param carNode CarNode
	---@return bool
        doesHavePartNode = function(self, carNode) end,
	---CLEO+
        ---| Returns the specified car's collision surfaceType 
	---@param self Car
	---@return SurfaceType surfaceType
        getCollisionSurface = function(self) end,
	---CLEO+
        ---| Returns the specified car's collision lighting 
	---@param self Car
	---@return float lighting
        getCollisionLighting = function(self) end,
	---CLEO+
        ---| Returns True if the specified car is really in the air, and False for boats floating on water 
	---@param self Car
	---@return bool
        isReallyInAir = function(self) end,
	---CLEO+
        ---| Returns the specified car's damage proofs 
	---@param self Car
	---@return bool bullet
	---@return bool fire
	---@return bool explosion
	---@return bool collision
	---@return bool melee
        getProofs = function(self) end,
	---CLEO+
        ---| Sets the specified car's coordinates without exception protocols 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesSimple = function(self, x, y, z) end,
	---CLEO+
        ---| Gets car damage by weapon last frame. Char can be invalid, and returns false if no damage last frame 
	---@param self Car
	---@return bool
	---@return Char char
	---@return WeaponType weaponType
	---@return float intensity
        getWeaponDamageLastFrame = function(self) end,
	---CLEO+
        ---| Returns the randomSeed of the specified car 
	---@param self Car
	---@return int randomSeed
        getRandomSeed = function(self) end,
	---CLEO+
        ---| Prevents the deletion of the specified car until MsFromNow 
	---@param self Car
	---@param msFromNow int
        dontDeleteUntilTime = function(self, msFromNow) end,
	---CLEO+
        ---| Returns the TimeIsDead in milliseconds since the specified car is dead 
	---@param self Car
	---@return int timeIsDead
        getTimeIsDead = function(self) end,
	---CLEO+
        ---| Returns True if the car is within the radius of the specified object 
	---@param self Car
	---@param object Object
	---@param radius float
	---@return bool
        locateDistanceToObject = function(self, object, radius) end,
	---CLEO+
        ---| Returns True if the car(self) is within the radius of the specified car 
	---@param self Car
	---@param car Car
	---@param radius float
	---@return bool
        locateDistanceToCar = function(self, car, radius) end,
	---CLEO+
        ---| Returns True if the car is within the radius of the specified coordinates 
	---@param self Car
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return bool
        locateDistanceToCoordinates = function(self, x, y, z, radius) end,
	---CLEO+
        ---| Returns True if the specified car is owned by the player 
	---@param self Car
	---@return bool
        isOwnedByPlayer = function(self) end,
	---CLEO+
        ---| Sets the specified car as OwnedByPlayer 
	---@param self Car
	---@param ownedByPlayer bool
        setOwnedByPlayer = function(self, ownedByPlayer) end,
	---CLEO+
        ---| Returns the carAnimGroup of the specified car 
	---@param self Car
	---@return CarAnimGroup carAnimGroup
        getAnimGroup = function(self) end,
	---CLEO+
        ---| Returns True if the specified car is a convertible 
	---@param self Car
	---@return bool
        isConvertible = function(self) end,
	---CLEO+
        ---| Returns the monetary value of the car 
	---@param self Car
	---@return int value
        getValue = function(self) end,
	---CLEO+
        ---| Returns the value of the car's gas and brake pedals 
	---@param self Car
	---@return float gas
	---@return float brake
        getPedals = function(self) end,
	---NewOpcodes
        ---| Gets matrix of car component 
	---@param self Car
	---@param componentName string
	---@return bool
	---@return Matrix handle
        getComponentMatrix = function(self, componentName) end,
	---NewOpcodes
        ---| Gets car component by name 
	---@param self Car
	---@param name string
	---@return bool
	---@return Component handle
        getComponent = function(self, name) end,
	---NewOpcodes
        ---| Sets alpha value for car's component model 
	---@param self Car
	---@param componentName string
	---@param alpha int
	---@return bool
        setComponentModelAlpha = function(self, componentName, alpha) end,
	---SAMPFUNCS
        ---| Returns the SAMP vehicle ID using its car handle 
	---@param self Car
	---@return int id
        getSampId = function(self) end,
    }
TextOp = {
        ---default
        ---| Displays a styled message for the specified time 
	---@param key gxt_key
	---@param time int
	---@param style TextStyle
        printBig = function(key, time, style) end,
	---default
        ---| Displays a message positioned on the bottom of the screen for the specified time 
	---@param key gxt_key
	---@param time int
	---@param flag int
        print = function(key, time, flag) end,
	---default
        ---| Displays a message positioned on the bottom of the screen for the specified time 
	---@param key gxt_key
	---@param time int
	---@param flag int
        printNow = function(key, time, flag) end,
	---default
        ---| Clears all priority text and some styles of big texts 
        clearPrints = function() end,
	---default
        ---| Displays a styled message in which the first string token~1~ is substituted with the specified number 
	---@param key gxt_key
	---@param num int
	---@param duration int
	---@param style TextStyle
        printWithNumberBig = function(key, num, duration, style) end,
	---default
        ---| Displays a styled message in which the first string token ~1~ is substituted with the specified number 
	---@param key gxt_key
	---@param num int
	---@param duration int
	---@param flag int
        printWithNumber = function(key, num, duration, flag) end,
	---default
        ---| Displays a styled message in which the first string token ~1~ is substituted with the specified number 
	---@param key gxt_key
	---@param num int
	---@param duration int
	---@param flag int
        printWithNumberNow = function(key, num, duration, flag) end,
	---default
        ---| Displays a low-priority styled message for the specified time 
	---@param key gxt_key
	---@param duration int
	---@param style TextStyle
        printBigQ = function(key, duration, style) end,
	---default
        ---| Displays a styled message in which the first two ~1~ tokens are substituted with the specified numbers 
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
	---@param duration int
	---@param style TextStyle
        printWith2NumbersNow = function(key, num1, num2, duration, style) end,
	---default
        ---| Displays a styled message in which the first three ~1~ tokens are substituted with the specified numbers 
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
	---@param num3 int
	---@param duration int
	---@param style TextStyle
        printWith3Numbers = function(key, num1, num2, num3, duration, style) end,
	---default
        ---| Displays a styled message in which the first four ~1~ tokens are substituted with the specified numbers 
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
	---@param num3 int
	---@param num4 int
	---@param duration int
	---@param style TextStyle
        printWith4Numbers = function(key, num1, num2, num3, num4, duration, style) end,
	---default
        ---| Displays a styled message in which the first four ~1~ tokens are substituted with the specified numbers 
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
	---@param num3 int
	---@param num4 int
	---@param duration int
	---@param style TextStyle
        printWith4NumbersNow = function(key, num1, num2, num3, num4, duration, style) end,
	---default
        ---| Displays a styled message in which the first six ~1~ tokens are substituted with the specified numbers 
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
	---@param num3 int
	---@param num4 int
	---@param num5 int
	---@param num6 int
	---@param duration int
	---@param style TextStyle
        printWith6Numbers = function(key, num1, num2, num3, num4, num5, num6, duration, style) end,
	---default
        ---| Draws text at the specified on-screen position 
	---@param offsetLeft float
	---@param offsetTop float
	---@param key gxt_key
        display = function(offsetLeft, offsetTop, key) end,
	---default
        ---| Scales the width and height of the text letters 
	---@param widthScale float
	---@param heightScale float
        setScale = function(widthScale, heightScale) end,
	---default
        ---| Sets the color of the text letters 
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
        setColor = function(red, green, blue, alpha) end,
	---default
        ---| Sets the text to be drawn justified, which means the text will wrap in order to fill an even rectangle of space 
	---@param state bool
        setJustify = function(state) end,
	---default
        ---| Centers the text 
	---@param state bool
        setCenter = function(state) end,
	---default
        ---| Sets the line width of the text 
	---@param width float
        setWrapX = function(width) end,
	---default
        ---| Sets the line width of the centered text 
	---@param width float
        setCenterSize = function(width) end,
	---default
        ---| Gives the text a background (0346) 
	---@param state bool
        setBackground = function(state) end,
	---default
        ---| Makes the text size proportionate 
	---@param state bool
        setProportional = function(state) end,
	---default
        ---| Sets the text draw font 
	---@param font Font
        setFont = function(font) end,
	---default
        ---| Displays a styled message in which the first two ~1~ tokens are substituted with the specified numbers 
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
	---@param duration int
	---@param style TextStyle
        printWith2NumbersBig = function(key, num1, num2, duration, style) end,
	---default
        ---| Displays a styled message in which the first string token ~a~ is substituted with the specified text 
	---@param templateKey gxt_key
	---@param replacementKey gxt_key
	---@param duration int
	---@param style TextStyle
        printStringInStringNow = function(templateKey, replacementKey, duration, style) end,
	---default
        ---| Removes the priority text from the screen 
	---@param key gxt_key
        clearThisPrint = function(key) end,
	---default
        ---| Removes the styled text from the screen 
	---@param key gxt_key
        clearThisBigPrint = function(key) end,
	---default
        ---| Causes the next text to be drawn before the fade is drawn 
	---@param state bool
        setDrawBeforeFade = function(state) end,
	---default
        ---| Sets the text draw to be aligned to the right 
	---@param state bool
        setRightJustify = function(state) end,
	---default
        ---| Displays a black text box for a few seconds 
	---@param key gxt_key
        printHelp = function(key) end,
	---default
        ---| Removes the text box from the screen 
        clearHelp = function() end,
	---default
        ---| Clears small messages from the screen 
        clearSmallPrints = function() end,
	---default
        ---| Enables text and texture drawing 
	---@param state bool
        useCommands = function(state) end,
	---default
        ---| Draws text with one number 
	---@param offsetLeft float
	---@param offsetTop float
	---@param key gxt_key
	---@param num int
        displayWithNumber = function(offsetLeft, offsetTop, key, num) end,
	---default
        ---| Draws text with two numbers 
	---@param offsetLeft float
	---@param offsetTop float
	---@param key gxt_key
	---@param num1 int
	---@param num2 int
        displayWith2Numbers = function(offsetLeft, offsetTop, key, num1, num2) end,
	---default
        ---| Shows a text box which stays on screen until it is removed by another command 
	---@param key gxt_key
        printHelpForever = function(key) end,
	---default
        ---| Shows a text box with one number 
	---@param gxt gxt_key
	---@param number int
        printHelpForeverWithNumber = function(gxt, number) end,
	---default
        ---| Makes the game use GXT Entries from the specified GXT Table 
	---@param tableName string
        loadMissionText = function(tableName) end,
	---default
        ---| Sets shadow for the current text draw 
	---@param intensity int
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
        setDropshadow = function(intensity, red, green, blue, alpha) end,
	---default
        ---| Returns true if a priority GXT string is displayed on screen 
	---@return bool
        isMessageBeingDisplayed = function() end,
	---default
        ---| Converts the float to two separate numbers to use in a 2-numbered GXT entry, and draws the text 
	---@param leftTopX float
	---@param leftTopY float
	---@param key gxt_key
	---@param value float
	---@param precision int
        displayWithFloat = function(leftTopX, leftTopY, key, value, precision) end,
	---default
        ---| Adds an outline to the next text drawn using a text draw command 
	---@param size int
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
        setEdge = function(size, red, green, blue, alpha) end,
	---default
        ---| Returns true if the string is empty 
	---@param text string
	---@return bool
        isEmpty = function(text) end,
	---default
        ---| Returns true if the string is empty 
	---@param text string
	---@return bool
        isEmpty = function(text) end,
	---default
        ---| Returns true if the string is empty 
	---@param text string
	---@return bool
        isEmpty = function(text) end,
	---default
        ---| Returns true if the string is empty 
	---@param text string
	---@return bool
        isEmpty = function(text) end,
	---default
        ---| Returns true if any help message is being displayed 
	---@return bool
        isHelpMessageBeingDisplayed = function() end,
	---default
        ---| Overrides the position of the text on screen 
	---@param customPosition bool
	---@param margin int
	---@param width int
        setMessageFormatting = function(customPosition, margin, width) end,
	---default
        ---| Sets the global width of text boxes displayed on screen 
	---@param size int
        setHelpMessageBoxSize = function(size) end,
	---default
        ---| Returns the CRC hash of the input string 
	---@param text string
	---@return int hash
        getHashKey = function(text) end,
	---default
        ---| Sets whether the next text is added to the brief in the menu 
	---@param state bool
        addNextMessageToPreviousBriefs = function(state) end,
	---default
        ---| Gets the width of the GXT entry string 
	---@param entry gxt_key
	---@return int width
        getStringWidth = function(entry) end,
	---default
        ---| Gets the width of the GXT entry string with the specified number 
	---@param gxtEntry gxt_key
	---@param number any
	---@return int width
        getStringWidthWithNumber = function(gxtEntry, number) end,
	---default
        ---| Removes the print big text with the specified style from the screen 
	---@param textStyle TextStyle
        clearThisPrintBigNow = function(textStyle) end,
	---default
        ---| Displays the text of the specified GXT entry using San Andreas' area name text style 
	---@param name string
        setAreaName = function(name) end,
	---default
        ---| Returns true if a help message with the specified GXT entry is being displayed 
	---@param gxt gxt_key
	---@return bool
        isThisHelpMessageBeingDisplayed = function(gxt) end,
	---default
        ---| Sets whether the text stays on the screen when it fades out 
	---@param state bool
        drawSubtitlesBeforeFade = function(state) end,
	---default
        ---| Sets whether the styled text stays on the screen when it fades out 
	---@param state bool
        drawOddjobTitleBeforeFade = function(state) end,
	---default
        ---| Overrides the text block set by 09BD 
	---@param state bool
        displayNonMinigameHelpMessages = function(state) end,
	---CLEO
        ---| Displays a custom text (provided as a literal or an address) in a black box similarly to PRINT_HELP 
	---@param text string
        printHelpString = function(text) end,
	---CLEO
        ---| Displays a custom text (provided as a literal or an address) similarly to PRINT_BIG 
	---@param text string
	---@param time int
	---@param style TextStyle
        printBigString = function(text, time, style) end,
	---CLEO
        ---| Displays a custom text (provided as a literal or an address) similarly to PRINT 
	---@param text string
	---@param time int
        printString = function(text, time) end,
	---CLEO
        ---| Displays a custom text (provided as a literal or an address) similarly to PRINT_NOW 
	---@param text string
	---@param time int
        printStringNow = function(text, time) end,
	---CLEO
        ---| Displays a black text box for a few seconds respecting the format of the String entered 
	---@param text string
	---@param args arguments
        printHelpFormatted = function(text, args) end,
	---CLEO
        ---| Formats args according to the format string, then displays it similarly to PRINT_BIG 
	---@param format string
	---@param time int
	---@param style TextStyle
	---@param args arguments
        printBigFormatted = function(format, time, style, args) end,
	---CLEO
        ---| Formats args according to the format string, then displays it similarly to PRINT 
	---@param format string
	---@param time int
	---@param args arguments
        printFormatted = function(format, time, args) end,
	---CLEO
        ---| Formats args according to the format string, then displays it similarly to PRINT_NOW 
	---@param format string
	---@param time int
	---@param args arguments
        printFormattedNow = function(format, time, args) end,
	---CLEO
        ---| Formats a text according to the format string and given arguments and writes it in the result 
	---@param result string
	---@param format string
	---@param args arguments
        stringFormat = function(result, format, args) end,
	---CLEO
        ---| Extracts data from a string using sscanf 
	---@param string string
	---@param format string
	---@return bool
	---@return int nValues
	---@return arguments values
        scanString = function(string, format) end,
	---CLEO
        ---| Returns the text associated with the GXT key 
	---@param key gxt_key
	---@return string text
        getLabelString = function(key) end,
	---CLEO
        ---| Adds or updates the text associated with the dynamic GXT key. It does nothing if the same key is defined in a FXT file 
	---@param dynamicKey gxt_key
	---@param text string
        addLabel = function(dynamicKey, text) end,
	---CLEO
        ---| Deletes the key and associated text created with ADD_TEXT_LABEL or defined in a FXT file 
	---@param key gxt_key
        removeLabel = function(key) end,
	---CLEO
        ---|  
	---@param number float
	---@param format string
	---@return string text
        stringFloatFormat = function(number, format) end,
	---CLEO+
        ---| Returns the string length 
	---@param text string
	---@return int length
        getStringLength = function(text) end,
	---CLEO+
        ---| Copies the string to the specified address 
	---@param string string
	---@param address int
        copyString = function(string, address) end,
	---CLEO+
        ---| Draws string once on specific drawing event 
	---@param string string
	---@param drawEvent DrawEvent
	---@param posX float
	---@param posY float
	---@param sizeX float
	---@param sizeY float
	---@param fixAr bool
	---@param font Font
        drawString = function(string, drawEvent, posX, posY, sizeX, sizeY, fixAr, font) end,
	---CLEO+
        ---| Draws string once on specific drawing event with extended text styling 
	---@param string string
	---@param drawEvent DrawEvent
	---@param posX float
	---@param posY float
	---@param sizeX float
	---@param sizeY float
	---@param fixAr bool
	---@param font Font
	---@param prop bool
	---@param align Align
	---@param wrap float
	---@param justify bool
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
	---@param edge int
	---@param shadow int
	---@param dropRed int
	---@param dropGreen int
	---@param dropBlue int
	---@param dropAlpha int
	---@param background bool
	---@param backRed int
	---@param backGreen int
	---@param backBlue int
	---@param backAlpha int
        drawStringExt = function(string, drawEvent, posX, posY, sizeX, sizeY, fixAr, font, prop, align, wrap, justify, red, green, blue, alpha, edge, shadow, dropRed, dropGreen, dropBlue, dropAlpha, background, backRed, backGreen, backBlue, backAlpha) end,
	---CLEO+
        ---| Returns True if string1 and string2 match 
	---@param string1 string
	---@param string2 string
	---@param maxSize int
	---@param caseSensitive bool
	---@param ignoreCharacter string
	---@return bool
        isStringEqual = function(string1, string2, maxSize, caseSensitive, ignoreCharacter) end,
	---CLEO+
        ---| Returns True if the string starts with a hash (#), semicolon (;) or double slash (//) 
	---@param string string
	---@return bool
        isStringComment = function(string) end,
	---CLEO+
        ---| Sets the string at the specified address to all upper case 
	---@param stringAddress int
        setStringUpper = function(stringAddress) end,
	---CLEO+
        ---| Sets the string at the specified address to all lower case 
	---@param stringAddress int
        setStringLower = function(stringAddress) end,
	---CLEO+
        ---| Returns the character index where strFind is found within stringOrigin 
	---@param stringFind StringFind
	---@param stringOrigin string
	---@param strFind string
	---@return bool
	---@return int index
        stringFind = function(stringFind, stringOrigin, strFind) end,
	---CLEO+
        ---| Cuts stringAddress at the specified character index 
	---@param stringAddress int
	---@param index int
        cutStringAt = function(stringAddress, index) end,
	---CLEO+
        ---| Returns True if the specified characters are found at the index of the string 
	---@param string string
	---@param characters string
	---@param index int
	---@return bool
        isStringCharacterAt = function(string, characters, index) end,
	---text
        ---| Checks if string length is equal to zero 
	---@param string string
	---@return bool
        isEmpty = function(string) end,
	---text
        ---| Compares two texts 
	---@param text string
	---@param another string
	---@param ignoreCase bool
	---@return bool
        isEqual = function(text, another, ignoreCase) end,
	---text
        ---| Checks if text contains specified text 
	---@param text string
	---@param subText string
	---@param ignoreCase bool
	---@return bool
        contains = function(text, subText, ignoreCase) end,
	---text
        ---| Checks if text starts with specified prefix text 
	---@param text string
	---@param prefix string
	---@param ignoreCase bool
	---@return bool
        startsWith = function(text, prefix, ignoreCase) end,
	---text
        ---| Checks if text ends with specified suffix text 
	---@param text string
	---@param suffix string
	---@param ignoreCase bool
	---@return bool
        endsWith = function(text, suffix, ignoreCase) end,
	---text
        ---| Formats args according to the format string then draws text at the specified on-screen position. See 033E 
	---@param offsetLeft float
	---@param offsetTop float
	---@param format string
	---@param args arguments
        displayFormatted = function(offsetLeft, offsetTop, format, args) end,
	---text
        ---| Loads GXT texts from selected FXT dictionary file 
	---@param filepath string
	---@return bool
        loadFxt = function(filepath) end,
	---text
        ---| Unloads GXT labels defined in selected Fxt file 
	---@param filepath string
	---@return bool
        unloadFxt = function(filepath) end,
	---text
        ---| Returns count of characters in the text 
	---@param text string
	---@return int length
        getLength = function(text) end,
	---text
        ---| Adds or updates the text associated with the dynamic GXT key. It does nothing if the same key is defined in a FXT file 
	---@param dynamicKey string
	---@param format string
	---@param args arguments
        addLabelFormatted = function(dynamicKey, format, args) end,
    }
ClockOp = {
        ---default
        ---| Returns the number of hours and minutes passed since midnight 
	---@return int hours
	---@return int minutes
        getTimeOfDay = function() end,
	---default
        ---| Sets the current in-game time 
	---@param hours int
	---@param minutes int
        setTimeOfDay = function(hours, minutes) end,
	---default
        ---| Returns the number of minutes left until the clock matches the time specified 
	---@param hours int
	---@param minutes int
	---@return int minutesLeft
        getMinutesToTimeOfDay = function(hours, minutes) end,
	---default
        ---| Sets the game to run at the specified speed 
	---@param scale float
        setTimeScale = function(scale) end,
	---default
        ---| Returns the time passed in milliseconds since the game started 
	---@return int time
        getGameTimer = function() end,
	---default
        ---| Saves the current time in game 
        store = function() end,
	---default
        ---| Restores the game time to the time when it was saved with 0253 
        restore = function() end,
	---default
        ---| Returns an integer representation of the in-game day of the week 
	---@return int day
        getCurrentDayOfWeek = function() end,
	---default
        ---| Returns the in-game day of the month and month of the year 
	---@return int day
	---@return int month
        getCurrentDate = function() end,
	---default
        ---| Progresses the game to the next day 
        setTimeOneDayForward = function() end,
	---CLEO+
        ---| Returns the full local time of the player's PC 
	---@return int year
	---@return int month
	---@return int weekDay
	---@return int day
	---@return int hour
	---@return int minute
	---@return int second
	---@return int millisecond
        getLocalTime = function() end,
	---CLEO+
        ---| Returns the clock's current hour 
	---@return int hour
        getCurrentHour = function() end,
	---CLEO+
        ---| Returns the clock's current minute 
	---@return int minute
        getCurrentMinute = function() end,
	---CLEO+
        ---| Simulates the passage of time on the clock, calendar and environment 
	---@param minutes int
        passTime = function(minutes) end,
    }
DebuggerOp = {
        ---default
        ---| Activates debug features in this script 
        enable = function() end,
	---default
        ---| Deactivates debug features in this script 
        disable = function() end,
	---default
        ---|  
	---@return bool
        isDebugCameraOn = function() end,
	---debug
        ---| Creates a debug breakpoint in script 
	---@param blocking bool
	---@param text string
	---@param args arguments
        breakpoint = function(blocking, text, args) end,
	---debug
        ---| Prints a debug message on screen and adds it to .cleo.log 
	---@param text string
	---@param args arguments
        trace = function(text, args) end,
	---debug
        ---| Appends new line to file. Timestamp parameter decides if datetime prefix in format 'DD/MM/YYYY hh:mm:ss.fraction' will be added. Command is not affected by debug mode status 
	---@param filename string
	---@param timestamp bool
	---@param text string
	---@param args arguments
        logLine = function(filename, timestamp, text, args) end,
    }
MissionOp = {
        ---default
        ---| Resets multiple settings that are usually set during missions and in some scripts 
        finish = function() end,
	---default
        ---| Loads a mission from the list defined in the main.scm header 
	---@param index int
        loadAndLaunchInternal = function(index) end,
	---default
        ---| Terminates the active mission by executing its mission cleanup routine 
        fail = function() end,
	---CLEO+
        ---| Returns true if the player is on a mission (the variable set in 0180 is not zero) 
	---@return bool
        isOn = function() end,
	---CLEO+
        ---| Checks if is playing scripted mission cutscene, that is, original widescreen borders are toggled on 
	---@return bool
        isOnScriptedCutscene = function() end,
    }
PadOp = {
        ---default
        ---| Returns true if the pad's button has been pressed 
	---@param pad PadId
	---@param buttonId Button
	---@return bool
        isButtonPressed = function(pad, buttonId) end,
	---default
        ---| Stores the status of the specified key into a variable 
	---@param pad PadId
	---@param buttonId Button
	---@return int state
        getState = function(pad, buttonId) end,
	---default
        ---| Shakes the player's joypad at the specified intensity for the specified time 
	---@param pad PadId
	---@param time int
	---@param intensity int
        shake = function(pad, time, intensity) end,
	---default
        ---| Returns the controller mode 
	---@return ControllerMode mode
        getControllerMode = function() end,
	---default
        ---| Affects the delay to the left and right steering while driving 
	---@param pad PadId
	---@param delay int
        setDrunkInputDelay = function(pad, delay) end,
	---default
        ---| Returns the offset of the specified Left/Right, Up/Down, Look Left/Look Right and Look Up/Look Down keys 
	---@param pad PadId
	---@return int leftStickX
	---@return int leftStickY
	---@return int rightStickX
	---@return int rightStickY
        getPositionOfAnalogueSticks = function(pad) end,
	---default
        ---| Sets whether the player can enter and exit vehicles 
	---@param playerId Player
	---@param enabled bool
        setPlayerEnterCarButton = function(playerId, enabled) end,
	---default
        ---| Sets whether the player can use the crouch button 
	---@param playerId Player
	---@param enabled bool
        setPlayerDuckButton = function(playerId, enabled) end,
	---default
        ---| Sets whether the player is able to use weapons 
	---@param playerId Player
	---@param enabled bool
        setPlayerFireButton = function(playerId, enabled) end,
	---default
        ---| Returns true if the player is pressing a key used to skip cutscenes or the game has been minimised 
	---@return bool
        isSkipCutsceneButtonPressed = function() end,
	---default
        ---| Sets whether the player can jump 
	---@param playerId Player
	---@param enabled bool
        setPlayerJumpButton = function(playerId, enabled) end,
	---default
        ---| Sets whether a player can use the ACTION key to display their stats 
	---@param playerId Player
	---@param enabled bool
        setPlayerDisplayVitalStatsButton = function(playerId, enabled) end,
	---default
        ---|  
	---@param playerId Player
	---@param enabled bool
        setPlayerCycleWeaponButton = function(playerId, enabled) end,
	---CLEO
        ---| Returns true if the player is pressing a keyboard button with the specified code 
	---@param keyCode KeyCode
	---@return bool
        isKeyPressed = function(keyCode) end,
	---CLEO
        ---| Returns true if the specified string of letters has been typed on the keyboard 
	---@param input string
	---@return bool
        testCheat = function(input) end,
	---CLEO+
        ---| Returns true if the player has just started to press a specified key this frame 
	---@param keyCode KeyCode
	---@return bool
        isKeyJustPressed = function(keyCode) end,
	---CLEO+
        ---| Returns true if the pad's button has just started to be pressed this frame 
	---@param pad PadId
	---@param buttonId Button
	---@return bool
        isButtonJustPressed = function(pad, buttonId) end,
	---CLEO+
        ---| Returns True if the pad's aim button is pressed 
	---@param pad PadId
	---@return bool
        isAimButtonPressed = function(pad) end,
	---CLEO+
        ---| Enables the specified control pad 
	---@param pad PadId
	---@param enabled bool
        setControl = function(pad, enabled) end,
	---CLEO+
        ---| Enables the specified control pad's movement 
	---@param pad PadId
	---@param movement bool
        setMovement = function(pad, movement) end,
	---CLEO+
        ---| Returns True if the pad's primary or secondary fire button is pressed 
	---@param pad PadId
	---@return bool
        isAnyFireButtonPressed = function(pad) end,
	---CLEO+
        ---| Returns the time in milliseconds since the pad has been touched 
	---@param pad PadId
	---@return int timeInMs
        getTimeNotTouching = function(pad) end,
	---input
        ---| Returns true if the player has just started to press a specified key this frame 
	---@param keyCode KeyCode
	---@return bool
        isKeyJustPressed = function(keyCode) end,
	---input
        ---| Gets code of first currently hold down key in range between minKeyCode and maxKeyCode. If no key is pressed return value is unmodified and logical result is set to false 
	---@param minKeyCode KeyCode
	---@param maxKeyCode KeyCode
	---@return bool
	---@return KeyCode keyCode
        getKeyPressedInRange = function(minKeyCode, maxKeyCode) end,
	---input
        ---| Gets code of first just pressed key in range between minKeyCode and maxKeyCode. If no key was pressed return value is unmodified and logical result is set to false 
	---@param minKeyCode KeyCode
	---@param maxKeyCode KeyCode
	---@return bool
	---@return KeyCode keyCode
        getKeyJustPressedInRange = function(minKeyCode, maxKeyCode) end,
	---input
        ---| Simulates key press event 
	---@param keyCode KeyCode
        emulateKeyPress = function(keyCode) end,
	---input
        ---| Simulates key release event 
	---@param keyCode KeyCode
        emulateKeyRelease = function(keyCode) end,
	---input
        ---| Returns n-th alternate key assigned to pad's action. If no key is bound then return value is unchanged and logical result is false 
	---@param action ControllerAction
	---@param altKey ControllerAltKey
	---@return bool
	---@return KeyCode keyCode
        getControllerKey = function(action, altKey) end,
	---input
        ---| Returns keyboard/mouse key name text of specified keyCode. If key code has no name return value is unchanged and logical result is false 
	---@param keyCode KeyCode
	---@return bool
	---@return string name
        getKeyName = function(keyCode) end,
    }
ObjectOp = {
        ---default
        ---| Creates an object at the specified location, with the specified model 
	---@param modelId model_object
	---@param x float
	---@param y float
	---@param z float
	---@return Object handle
        create = function(modelId, x, y, z) end,
	---default
        ---| Destroys the object, freeing game memory 
	---@param self Object
        delete = function(self) end,
	---default
        ---| Returns the object's heading (z-angle) 
	---@param self Object
	---@return float heading
        getHeading = function(self) end,
	---default
        ---| Sets the object's heading (z-angle) 
	---@param self Object
	---@param heading float
        setHeading = function(self, heading) end,
	---default
        ---| Returns the object's coordinates 
	---@param self Object
	---@return float x
	---@return float y
	---@return float z
        getCoordinates = function(self) end,
	---default
        ---| Puts the object at the specified location 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        setCoordinates = function(self, x, y, z) end,
	---default
        ---| Allows the object to be deleted by the game if necessary, and also removes it from the mission cleanup list, if applicable 
	---@param self Object
        markAsNoLongerNeeded = function(self) end,
	---default
        ---| Removes the object from the mission cleanup list, preventing it from being deleted when the mission ends 
	---@param self Object
        dontRemove = function(self) end,
	---default
        ---| Creates an object without offset at the location 
	---@param modelId model_object
	---@param x float
	---@param y float
	---@param z float
	---@return Object handle
        createNoOffset = function(modelId, x, y, z) end,
	---default
        ---| Returns true if the object is visible 
	---@param self Object
	---@return bool
        isOnScreen = function(self) end,
	---default
        ---| Rotates the object from one angle to another, optionally accounting for a collision during the rotation 
	---@param self Object
	---@param fromAngle float
	---@param toAngle float
	---@param collisionCheck bool
	---@return bool
        rotate = function(self, fromAngle, toAngle, collisionCheck) end,
	---default
        ---| Animates object movement toward specified coordinates. Returns true if the object has finished moving 
	---@param self Object
	---@param posX float
	---@param posY float
	---@param posZ float
	---@param speedX float
	---@param speedY float
	---@param speedZ float
	---@param collisionCheck bool
	---@return bool
        slide = function(self, posX, posY, posZ, speedX, speedY, speedZ, collisionCheck) end,
	---default
        ---| Places the object at an offset from the car 
	---@param self Object
	---@param vehicle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
        placeRelativeToCar = function(self, vehicle, xOffset, yOffset, zOffset) end,
	---default
        ---| Sets whether the object can be targeted (auto-aimed) or not 
	---@param self Object
	---@param state bool
        makeTargetable = function(self, state) end,
	---default
        ---| Returns true if the object is damaged 
	---@param self Object
	---@return bool
        hasBeenDamaged = function(self) end,
	---default
        ---| Sets the object's velocity 
	---@param self Object
	---@param xSpeed float
	---@param ySpeed float
	---@param zSpeed float
        setVelocity = function(self, xSpeed, ySpeed, zSpeed) end,
	---default
        ---| Sets the object's collision detection 
	---@param self Object
	---@param state bool
        setCollision = function(self, state) end,
	---default
        ---| Adds the given vector to the object's velocity (0381) 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        addToVelocity = function(self, x, y, z) end,
	---default
        ---| Defines whether or not the object is moveable 
	---@param self Object
	---@param state bool
        setDynamic = function(self, state) end,
	---default
        ---| Returns true if the handle is a valid object handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---default
        ---| Returns the object's coordinates with an offset 
	---@param self Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@return float x
	---@return float y
	---@return float z
        getOffsetInWorldCoords = function(self, xOffset, yOffset, zOffset) end,
	---default
        ---| Sets the specified object to always draw on top of other objects 
	---@param self Object
	---@param state bool
        setDrawLast = function(self, state) end,
	---default
        ---| Sets the object rotation along X, Y and Z axis 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        setRotation = function(self, x, y, z) end,
	---default
        ---| Enables the use of collision checking for the object 
	---@param self Object
	---@param state bool
        setRecordsCollisions = function(self, state) end,
	---default
        ---| Returns true if the object has collided 
	---@param self Object
	---@return bool
        hasCollidedWithAnything = function(self) end,
	---default
        ---| Returns true if the object is near the specified coordinates 
	---@param self Object
	---@param x float
	---@param y float
	---@param xRadius float
	---@param yRadius float
	---@param drawSphere bool
	---@return bool
        locate2D = function(self, x, y, xRadius, yRadius, drawSphere) end,
	---default
        ---| Returns true if the object is near the specified point 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
	---@param xRadius float
	---@param yRadius float
	---@param zRadius float
	---@param drawSphere bool
	---@return bool
        locate3D = function(self, x, y, z, xRadius, yRadius, zRadius, drawSphere) end,
	---default
        ---| Returns true if the object is in water 
	---@param self Object
	---@return bool
        isInWater = function(self) end,
	---default
        ---| Returns true if object is in the specified area 
	---@param self Object
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param drawSphere bool
	---@return bool
        isInArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, drawSphere) end,
	---default
        ---| Returns true if the object is in the specified area 
	---@param self Object
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param drawSphere bool
	---@return bool
        isInArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere) end,
	---default
        ---| Makes the specified car have no collision with the specified object 
	---@param self Object
	---@param handle Car
        sortOutCollisionWithCar = function(self, handle) end,
	---default
        ---| Sets whether the object's position remains unchanged 
	---@param self Object
	---@param state bool
        freezePosition = function(self, state) end,
	---default
        ---| Sets the visibility of the object to the specified interior 
	---@param self Object
	---@param areaId int
        setAreaVisible = function(self, areaId) end,
	---default
        ---| Returns the object's X, Y, and Z velocity 
	---@param self Object
	---@return float x
	---@return float y
	---@return float z
        getVelocity = function(self) end,
	---default
        ---| Sets the object's rotation velocity from the center of its body 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        addToRotationVelocity = function(self, x, y, z) end,
	---default
        ---| Sets the object's rotation velocity with frame sync applied? 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        setRotationVelocity = function(self, x, y, z) end,
	---default
        ---| Returns true if the object is not moving 
	---@param self Object
	---@return bool
        isStatic = function(self) end,
	---default
        ---|  
	---@param self Object
	---@return float x
	---@return float y
	---@return float z
        getRotationVelocity = function(self) end,
	---default
        ---| Sets the object's velocity 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        addVelocityRelative = function(self, x, y, z) end,
	---default
        ---| Gets the speed of the object 
	---@param self Object
	---@return float speed
        getSpeed = function(self) end,
	---default
        ---| Makes the object look like it has been burnt 
	---@param self Object
	---@param state bool
        setRenderScorched = function(self, state) end,
	---default
        ---|  
	---@param self Object
	---@param handle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
        attachToCar = function(self, handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation) end,
	---default
        ---|  
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
	---@param collisionDetection bool
        detach = function(self, x, y, z, collisionDetection) end,
	---default
        ---|  
	---@param self Object
	---@return bool
        isAttached = function(self) end,
	---default
        ---|  
	---@param self Object
	---@param handle Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
        attachToObject = function(self, handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation) end,
	---default
        ---|  
	---@param self Object
	---@param handle Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xRotation float
	---@param yRotation float
	---@param zRotation float
        attachToChar = function(self, handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation) end,
	---default
        ---|  
	---@param self Object
	---@return int health
        getHealth = function(self) end,
	---default
        ---|  
	---@param self Object
	---@param health int
        setHealth = function(self, health) end,
	---default
        ---| Smashes the object to pieces 
	---@param self Object
	---@param intensity int
        ["break"] = function(self, intensity) end,
	---default
        ---| Sets whether the object is visible 
	---@param self Object
	---@param state bool
        setVisible = function(self, state) end,
	---default
        ---| Plays an object animation 
	---@param self Object
	---@param animationName string
	---@param animationFile string
	---@param frameDelta float
	---@param lockF bool
	---@param loop bool
        playAnim = function(self, animationName, animationFile, frameDelta, lockF, loop) end,
	---default
        ---|  
	---@param self Object
	---@return float height
        getRopeHeight = function(self) end,
	---default
        ---|  
	---@param self Object
	---@param height float
        setRopeHeight = function(self, height) end,
	---default
        ---|  
	---@param self Object
	---@return Car vehicle
	---@return Char char
	---@return Object object
        grabEntityOnRope = function(self) end,
	---default
        ---|  
	---@param self Object
        releaseEntityFromRope = function(self) end,
	---default
        ---| Gets the object's quaternion 
	---@param self Object
	---@return float x
	---@return float y
	---@return float z
	---@return float w
        getQuaternion = function(self) end,
	---default
        ---| Sets the object's quaternion 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
	---@param w float
        setQuaternion = function(self, x, y, z, w) end,
	---default
        ---| Sets whether the object can be destroyed or not 
	---@param self Object
	---@param state bool
        setCollisionDamageEffect = function(self, state) end,
	---default
        ---|  
	---@param self Object
	---@param nth int
	---@return float x
	---@return float y
	---@return float z
        getLevelDesignCoords = function(self, nth) end,
	---default
        ---| Sets the object's coordinates without affecting the rotation 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
        setCoordinatesAndVelocity = function(self, x, y, z) end,
	---default
        ---| Sets which LOD object should show when the object is being viewed from far away 
	---@param self Object
	---@param lodObject Object
        connectLods = function(self, lodObject) end,
	---default
        ---| Returns true if the object has been photographed 
	---@param self Object
	---@return bool
        hasBeenPhotographed = function(self) end,
	---default
        ---| Sets the object's animation speed 
	---@param self Object
	---@param animationName string
	---@param speed float
        setAnimSpeed = function(self, animationName, speed) end,
	---default
        ---| Returns true if the object is playing the specified animation 
	---@param self Object
	---@param animationName string
	---@return bool
        isPlayingAnim = function(self, animationName) end,
	---default
        ---| Gets the current progress of the object's animation 
	---@param self Object
	---@param animationName string
	---@return float time
        getAnimCurrentTime = function(self, animationName) end,
	---default
        ---| Sets the progress of an animation, with 0 
	---@param self Object
	---@param animationName string
	---@param time float
        setAnimCurrentTime = function(self, animationName, time) end,
	---default
        ---| Makes the object damageable only by the player 
	---@param self Object
	---@param state bool
        setOnlyDamagedByPlayer = function(self, state) end,
	---default
        ---| Sets the scale of the object 
	---@param self Object
	---@param scale float
        setScale = function(self, scale) end,
	---default
        ---|  
	---@param self Object
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledArea2D = function(self, leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---|  
	---@param self Object
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isInAngledArea3D = function(self, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---| Sets whether the object can be picked up and carried 
	---@param self Object
	---@param state bool
        setAsStealable = function(self, state) end,
	---default
        ---| Returns true if the object has been damaged by the specified weapon or damage type 
	---@param self Object
	---@param weaponType WeaponType
	---@return bool
        hasBeenDamagedByWeapon = function(self, weaponType) end,
	---default
        ---| Clears the object's last damaging weapon ID 
	---@param self Object
        clearLastWeaponDamage = function(self) end,
	---default
        ---| Sets whether the door object is locked at its current rotation and allows it to be pushed open by entities once 
	---@param self Object
	---@param state bool
        lockDoor = function(self, state) end,
	---default
        ---| Sets the object's mass 
	---@param self Object
	---@param mass float
        setMass = function(self, mass) end,
	---default
        ---| Returns the object's mass 
	---@param self Object
	---@return float mass
        getMass = function(self) end,
	---default
        ---| Sets the object's turn mass 
	---@param self Object
	---@param turnMass float
        setTurnMass = function(self, turnMass) end,
	---default
        ---| Returns the object's turn mass 
	---@param self Object
	---@return float turnMass
        getTurnMass = function(self) end,
	---default
        ---| Sets whether the object can be picked up with the magnocrane 
	---@param self Object
	---@param state bool
        winchCanPickUp = function(self, state) end,
	---default
        ---| Returns true if the object has been made moveable by the 0392 
	---@param self Object
	---@return bool
        hasBeenUprooted = function(self) end,
	---default
        ---| Returns true if the object is within the external script trigger radius 
	---@param self Object
	---@return bool
        isWithinBrainActivationRange = function(self) end,
	---default
        ---| Returns the object's model index 
	---@param self Object
	---@return int model
        getModel = function(self) end,
	---default
        ---| Fades the object out of existence, freeing game memory 
	---@param self Object
        removeElegantly = function(self) end,
	---default
        ---| Sets what immunities the object has 
	---@param self Object
	---@param bulletProof bool
	---@param fireProof bool
	---@param explosionProof bool
	---@param collisionProof bool
	---@param meleeProof bool
        setProofs = function(self, bulletProof, fireProof, explosionProof, collisionProof, meleeProof) end,
	---default
        ---| Returns true if the object's model is the model specified 
	---@param self Object
	---@param modelId model_any
	---@return bool
        doesHaveThisModel = function(self, modelId) end,
	---default
        ---| Appears to return true if something had entered the object's position since it was created or its position was changed 
	---@param self Object
	---@return bool
        isIntersectingWorld = function(self) end,
	---default
        ---| Sets whether the object attracts spawned peds to interact with it 
	---@param self Object
	---@param state bool
        enableDisabledAttractors = function(self, state) end,
	---CLEO+
        ---| Set's the specified object's transparency alpha 
	---@param self Object
	---@param alpha int
        setModelAlpha = function(self, alpha) end,
	---CLEO+
        ---| Creates an no save game object at the specified location, with the specified model 
	---@param modelId model_object
	---@param x float
	---@param y float
	---@param z float
	---@param useOffset bool
	---@param useGround bool
	---@return Object handle
        createNoSave = function(modelId, x, y, z, useOffset, useGround) end,
	---CLEO+
        ---| Returns true if the specified object is controlled by a script 
	---@param self Object
	---@return bool
        isScriptControlled = function(self) end,
	---CLEO+
        ---| marks object as script controlled 
	---@param self Object
        markAsNeeded = function(self) end,
	---CLEO+
        ---| Inits additional variables for this object. Identifier can be "AUTO" for unique ID based on script pointer 
	---@param self Object
	---@param identifier string
	---@param totalVars int
	---@return bool
        initExtendedVars = function(self, identifier, totalVars) end,
	---CLEO+
        ---| Sets extended var value for this object. Requires initialization (0E1A), otherwise returns false 
	---@param self Object
	---@param identifier string
	---@param varNumber int
	---@param value any
	---@return bool
        setExtendedVar = function(self, identifier, varNumber, value) end,
	---CLEO+
        ---| Gets extended var value for this char. Returns false if not initialized (0E1A) 
	---@param self Object
	---@param identifier string
	---@param varNumber int
	---@return bool
	---@return any value
        getExtendedVar = function(self, identifier, varNumber) end,
	---CLEO+
        ---| Gets the distance between the object model's center of mass to its base 
	---@param self Object
	---@return float distance
        getDistanceFromCenterOfMassToBaseOfModel = function(self) end,
	---CLEO+
        ---| Returns True if the specified object is not on a solid surface and not submerged 
	---@param self Object
	---@return bool
        isReallyInAir = function(self) end,
	---CLEO+
        ---| Simulates the specified damage amount and weaponType on the object 
	---@param self Object
	---@param damage float
	---@param weaponType WeaponType
        simulateDamage = function(self, damage, weaponType) end,
	---CLEO+
        ---| Returns the specified object's damage proofs 
	---@param self Object
	---@return bool bullet
	---@return bool fire
	---@return bool explosion
	---@return bool collision
	---@return bool melee
        getProofs = function(self) end,
	---CLEO+
        ---| Returns the randomSeed of the specified object 
	---@param self Object
	---@return int randomSeed
        getRandomSeed = function(self) end,
	---CLEO+
        ---| Returns True if the object(self) is within the radius of the specified object 
	---@param self Object
	---@param object Object
	---@param radius float
	---@return bool
        locateDistanceToObject = function(self, object, radius) end,
	---CLEO+
        ---| Returns True if the object is within the radius of the specified coordinates 
	---@param self Object
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return bool
        locateDistanceToCoordinates = function(self, x, y, z, radius) end,
	---CLEO+
        ---|  
	---@param self Object
	---@param modelId model_any
	---@param x float
	---@param y float
	---@param z float
	---@param rx float
	---@param ry float
	---@param rz float
	---@return int renderobject
        createRenderObjectToObject = function(self, modelId, x, y, z, rx, ry, rz) end,
	---CLEO+
        ---|  
	---@param self Object
	---@param specialModel int
	---@param x float
	---@param y float
	---@param z float
	---@param rx float
	---@param ry float
	---@param rz float
	---@return int renderobject
        createRenderObjectToObjectFromSpecial = function(self, specialModel, x, y, z, rx, ry, rz) end,
	---SAMPFUNCS
        ---| Returns the SAMP ID of an object using its handle 
	---@param self Object
	---@return bool
	---@return int id
        getSampId = function(self) end,
    }
CarGeneratorOp = {
        ---default
        ---| Initializes a parked car generator (modelId -1 selects a random vehicle from the local popcycle) 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param modelId model_vehicle
	---@param primaryColor int
	---@param secondaryColor int
	---@param forceSpawn bool
	---@param alarmChance int
	---@param doorLockChance int
	---@param minDelay int
	---@param maxDelay int
	---@return CarGenerator handle
        create = function(x, y, z, heading, modelId, primaryColor, secondaryColor, forceSpawn, alarmChance, doorLockChance, minDelay, maxDelay) end,
	---default
        ---| Specifies the number of times the car generator spawns a car (101 - infinite) 
	---@param self CarGenerator
	---@param amount int
        switch = function(self, amount) end,
	---default
        ---| Prevents the specified car model from spawning for car generators 
	---@param model model_vehicle
        suppressCarModel = function(model) end,
	---default
        ---| Allows the specified car model to spawn for car generators 
	---@param modelId model_vehicle
        dontSuppressCarModel = function(modelId) end,
	---default
        ---| Resets the disabled car model list for car generators 
        dontSuppressAnyCarModels = function() end,
	---default
        ---| Creates a parked car generator with a number plate (modelId -1 selects a random vehicle from the local popcycle) 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param modelId model_vehicle
	---@param primaryColor int
	---@param secondaryColor int
	---@param forceSpawn bool
	---@param alarmChance int
	---@param doorLockChance int
	---@param minDelay int
	---@param maxDelay int
	---@param plateName string
	---@return CarGenerator handle
        createWithPlate = function(x, y, z, heading, modelId, primaryColor, secondaryColor, forceSpawn, alarmChance, doorLockChance, minDelay, maxDelay, plateName) end,
	---default
        ---| Sets whether the player will not receive a wanted level when entering a vehicle from this generator when the police is around 
	---@param self CarGenerator
	---@param state bool
        setHasBeenOwned = function(self, state) end,
	---CLEO+
        ---|  
	---@param self CarGenerator
        setNoSave = function(self) end,
    }
HudOp = {
        ---default
        ---| Creates a countdown or countup onscreen timer 
	---@param timer int
	---@param direction TimerDirection
        displayTimer = function(timer, direction) end,
	---default
        ---| Removes the onscreen timer 
	---@param timer int
        clearTimer = function(timer) end,
	---default
        ---| Removes the onscreen counter (0150 or 03C4) 
	---@param counter int
        clearCounter = function(counter) end,
	---default
        ---| Enables widescreen 
	---@param state bool
        switchWidescreen = function(state) end,
	---default
        ---| Draws a loaded texture (038F) at the specified on-screen X and Y coordinates, with the specified size and RGBA color 
	---@param spriteSlot int
	---@param offsetLeft float
	---@param offsetTop float
	---@param width float
	---@param height float
	---@param r int
	---@param g int
	---@param b int
	---@param a int
        drawSprite = function(spriteSlot, offsetLeft, offsetTop, width, height, r, g, b, a) end,
	---default
        ---| Draws a box at the specified screen X and Y position, with the specified size and RGBA colors 
	---@param x float
	---@param y float
	---@param width float
	---@param height float
	---@param r int
	---@param g int
	---@param b int
	---@param a int
        drawRect = function(x, y, width, height, r, g, b, a) end,
	---default
        ---| Makes the on-screen timer stop updating 
	---@param state bool
        freezeTimer = function(state) end,
	---default
        ---| Creates a countdown or countup onscreen timer with the text 
	---@param timer int
	---@param direction TimerDirection
	---@param text gxt_key
        displayTimerWithString = function(timer, direction, text) end,
	---default
        ---| Displays an onscreen counter with the text, either shown in numbers or as a bar 
	---@param counter int
	---@param display CounterDisplay
	---@param text gxt_key
        displayCounterWithString = function(counter, display, text) end,
	---default
        ---| Causes the next texture to be drawn (038D) before the fade is drawn 
	---@param state bool
        setSpritesDrawBeforeFade = function(state) end,
	---default
        ---| Makes a specific part of the HUD disappear and reappear several times 
	---@param object HudObject
        flashObject = function(object) end,
	---default
        ---| Displays an onscreen counter with the text in the specified slot, either shown in numbers or as a bar 
	---@param counter int
	---@param display CounterDisplay
	---@param slot int
	---@param text gxt_key
        displayNthCounterWithString = function(counter, display, slot, text) end,
	---default
        ---| Displays or hides the radar 
	---@param state bool
        displayRadar = function(state) end,
	---default
        ---|  
	---@param counter int
	---@param state bool
        setCounterFlashWhenFirstDisplayed = function(counter, state) end,
	---default
        ---| This is an extended version of 038D with scale and angle parameters 
	---@param spriteSlot int
	---@param offsetLeft float
	---@param offsetTop float
	---@param width float
	---@param height float
	---@param angle float
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
        drawSpriteWithRotation = function(spriteSlot, offsetLeft, offsetTop, width, height, angle, red, green, blue, alpha) end,
	---default
        ---|  
	---@param zoom int
        setRadarZoom = function(zoom) end,
	---default
        ---| Sets whether the HUD displays 
	---@param state bool
        display = function(state) end,
	---default
        ---| Starts a sound when the countdown timer reaches the specified number of seconds 
	---@param timer int
	---@param timeInSec int
        setTimerBeepCountdownTime = function(timer, timeInSec) end,
	---default
        ---| Returns the RGBA of the specified HUD color 
	---@param color HudColors
	---@return int red
	---@return int green
	---@return int blue
	---@return int alpha
        getColor = function(color) end,
	---default
        ---| Draws a black box with styled text from corner A to corner B 
	---@param leftTopX float
	---@param leftTopY float
	---@param rightBottomX float
	---@param rightBottomY float
	---@param header gxt_key
	---@param zIndex int
        drawWindow = function(leftTopX, leftTopY, rightBottomX, rightBottomY, header, zIndex) end,
	---default
        ---| Sets whether the HUD should always display weapon aiming crosshairs, used in the mission 'Catalyst' where the player must throw crates of ammo to Ryder 
	---@param state bool
        drawCrosshair = function(state) end,
	---default
        ---| Sets whether the name of the current vehicle should be displayed 
	---@param state bool
        displayCarNames = function(state) end,
	---default
        ---| Sets whether the area text for the current area should show 
	---@param state bool
        displayZoneNames = function(state) end,
	---default
        ---| Prevents timers and big texts from being hidden if there is another conflicting type of text on screen 
	---@param state bool
        forceBigMessageAndCounter = function(state) end,
	---CLEO+
        ---| Gets x and y values based on window aspect ratio, useful for text and hud scaling 
	---@param x float
	---@param y float
	---@return float x
	---@return float y
        getFixedXyAspectRatio = function(x, y) end,
	---CLEO+
        ---| Creates a countdown or countup onscreen timer 
	---@param timer int
	---@param direction TimerDirection
        displayTimerLocal = function(timer, direction) end,
	---CLEO+
        ---| Creates a countdown or countup onscreen timer with the text 
	---@param timer int
	---@param direction TimerDirection
	---@param text gxt_key
        displayTimerWithStringLocal = function(timer, direction, text) end,
	---CLEO+
        ---| Displays an onscreen counter, either shown in numbers or as a bar 
	---@param timer int
	---@param display CounterDisplay
        displayCounterLocal = function(timer, display) end,
	---CLEO+
        ---| Displays an onscreen counter with the text, either shown in numbers or as a bar 
	---@param counter int
	---@param display CounterDisplay
	---@param text gxt_key
        displayCounterWithStringLocal = function(counter, display, text) end,
	---CLEO+
        ---| Displays two onscreen counters separated by a slash 
	---@param leftCounter int
	---@param rightCounter int
        displayTwoCountersLocal = function(leftCounter, rightCounter) end,
	---CLEO+
        ---| Displays two onscreen counters separated by a slash with the text 
	---@param leftCounter int
	---@param rightCounter int
	---@param text gxt_key
        displayTwoCountersWithStringLocal = function(leftCounter, rightCounter, text) end,
	---CLEO+
        ---| Removes the local onscreen timer 
	---@param timer int
        clearTimerLocal = function(timer) end,
	---CLEO+
        ---| Removes the local onscreen counter 
	---@param counter int
        clearCounterLocal = function(counter) end,
	---CLEO+
        ---| Sets the local counter to flash when first displayed 
	---@param counter int
	---@param state bool
        setCounterFlashWhenFirstDisplayedLocal = function(counter, state) end,
	---CLEO+
        ---| Starts a sound when the countdown timer reaches the specified number of seconds 
	---@param timer int
	---@param timeInSec int
        setTimerBeepCountdownTimeLocal = function(timer, timeInSec) end,
	---CLEO+
        ---| Sets the color of the specified local counter 
	---@param counter int
	---@param color HudColors
        setCounterColorLocal = function(counter, color) end,
	---CLEO+
        ---|  
	---@return bool
        isRadarVisible = function() end,
	---CLEO+
        ---|  
	---@return bool
        isVisible = function() end,
    }
BlipOp = {
        ---default
        ---| Adds a blip with properties to the vehicle 
	---@param vehicle Car
	---@param color BlipColor
	---@param display BlipDisplay
	---@return Blip handle
        addForCarOld = function(vehicle, color, display) end,
	---default
        ---| Removes the blip 
	---@param self Blip
        remove = function(self) end,
	---default
        ---| Sets the blip's color 
	---@param self Blip
	---@param color BlipColor
        changeColor = function(self, color) end,
	---default
        ---| Adds a blip with properties at the location 
	---@param x float
	---@param y float
	---@param z float
	---@param colour BlipColor
	---@param display BlipDisplay
	---@return Blip handle
        addForCoordOld = function(x, y, z, colour, display) end,
	---default
        ---| Sets the blip's size 
	---@param self Blip
	---@param size int
        changeScale = function(self, size) end,
	---default
        ---| Adds a blip and a marker to the vehicle 
	---@param vehicle Car
	---@return Blip handle
        addForCar = function(vehicle) end,
	---default
        ---| Adds a blip and a marker to the character 
	---@param char Char
	---@return Blip handle
        addForChar = function(char) end,
	---default
        ---| Adds a blip and a marker to the object 
	---@param object Object
	---@return Blip handle
        addForObject = function(object) end,
	---default
        ---| Adds a blip to the location 
	---@param x float
	---@param y float
	---@param z float
	---@return Blip handle
        addForCoord = function(x, y, z) end,
	---default
        ---| Changes the display of the specified blip 
	---@param self Blip
	---@param display BlipDisplay
        changeDisplay = function(self, display) end,
	---default
        ---| Adds a long range sprite blip and sphere to the contact point that is not displayed while on mission 
	---@param x float
	---@param y float
	---@param z float
	---@param sprite RadarSprite
	---@return Blip handle
        addSpriteForContactPoint = function(x, y, z, sprite) end,
	---default
        ---| Adds a sprite blip to the location 
	---@param x float
	---@param y float
	---@param z float
	---@param sprite RadarSprite
	---@return Blip handle
        addSpriteForCoord = function(x, y, z, sprite) end,
	---default
        ---| Adds a blip and a marker to the pickup 
	---@param pickup Pickup
	---@return Blip handle
        addForPickup = function(pickup) end,
	---default
        ---| Adds a sprite blip for the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param sprite RadarSprite
	---@return Blip handle
        addShortRangeSpriteForCoord = function(x, y, z, sprite) end,
	---default
        ---| Adds a short range sprite blip and sphere to the contact point that is not displayed while on mission 
	---@param x float
	---@param y float
	---@param z float
	---@param sprite RadarSprite
	---@return Blip handle
        addShortRangeSpriteForContactPoint = function(x, y, z, sprite) end,
	---default
        ---| Creates a blip indicating the searchlights position on the radar 
	---@param searchlight Searchlight
	---@return Blip handle
        addForSearchlight = function(searchlight) end,
	---default
        ---| Returns true if the handle is a valid blip handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---default
        ---| Sets whether the tracking blip will remain regardless of the entities existance 
	---@param self Blip
	---@param state bool
        setAlwaysDisplayOnZoomedRadar = function(self, state) end,
	---default
        ---|  
	---@param self Blip
	---@param state bool
        setAsFriendly = function(self, state) end,
	---default
        ---| Adds a blip and a marker to the character (identical to 0187) 
	---@param char Char
	---@return Blip handle
        addForDeadChar = function(char) end,
	---default
        ---| Assigns the blip to the specified entrance/exit marker 
	---@param self Blip
	---@param x float
	---@param y float
	---@param radius float
        setEntryExit = function(self, x, y, radius) end,
	---default
        ---| Works similar to 0165, except this command does not work on tracking blips, has different colors and does not support direct RGBA setting 
	---@param self Blip
	---@param color CoordAppearance
        setCoordAppearance = function(self, color) end,
    }
RestartOp = {
        ---default
        ---| Adds a hospital restart, which is where the player will spawn after death (wasted) if the point is closer than any other hospital restart 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param citiesPassed int
        addHospital = function(x, y, z, heading, citiesPassed) end,
	---default
        ---| Adds a police restart, which is where the player will spawn after being arrested (busted) if the point is closer than any other police restart 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param citiesPassed int
        addPolice = function(x, y, z, heading, citiesPassed) end,
	---default
        ---| Forces this location to be the next respawn location 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
        overrideNext = function(x, y, z, heading) end,
	---default
        ---| Stops the player from spawning at the override location (016E) 
        cancelOverride = function() end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param heading float
        setExtraHospitalRestartPoint = function(x, y, z, radius, heading) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param heading float
        setExtraPoliceStationRestartPoint = function(x, y, z, radius, heading) end,
	---default
        ---| Overrides the respawn point 
	---@param x float
	---@param y float
	---@param z float
        setRespawnPointForDurationOfMission = function(x, y, z) end,
    }
FxOp = {
        ---default
        ---| Draws a shadow in the current frame 
	---@param textureType ShadowTextureType
	---@param x float
	---@param y float
	---@param z float
	---@param angle float
	---@param length float
	---@param intensity int
	---@param r int
	---@param g int
	---@param b int
        drawShadow = function(textureType, x, y, z, angle, length, intensity, r, g, b) end,
	---default
        ---| Creates an explosion at the point 
	---@param x float
	---@param y float
	---@param z float
	---@param type ExplosionType
        addExplosion = function(x, y, z, type) end,
	---default
        ---| Displays a corona with fade in-out effect at the specified location 
	---@param x float
	---@param y float
	---@param z float
	---@param size float
	---@param coronaType CoronaType
	---@param flareType FlareType
	---@param r int
	---@param g int
	---@param b int
        drawCorona = function(x, y, z, size, coronaType, flareType, r, g, b) end,
	---default
        ---| Displays a corona with the lowered draw distance at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param size float
	---@param coronaType CoronaType
	---@param flareType FlareType
	---@param r int
	---@param g int
	---@param b int
        drawWeaponshopCorona = function(x, y, z, size, coronaType, flareType, r, g, b) end,
	---default
        ---| Creates an explosion with no sound 
	---@param x float
	---@param y float
	---@param z float
	---@param type ExplosionType
        addExplosionNoSound = function(x, y, z, type) end,
	---default
        ---| Creates a gun flash particle effect 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
        addBigGunFlash = function(fromX, fromY, fromZ, toX, toY, toZ) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param velocityX float
	---@param velocityY float
	---@param velocityZ float
	---@param density int
        addSparks = function(x, y, z, velocityX, velocityY, velocityZ, density) end,
	---default
        ---| Creates an explosion at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param type int
	---@param shake float
        addExplosionVariableShake = function(x, y, z, type, shake) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param velocityX float
	---@param velocityY float
	---@param velocityZ float
	---@param red float
	---@param green float
	---@param blue float
	---@param alpha float
	---@param size float
	---@param lastFactor float
        addSmokeParticle = function(x, y, z, velocityX, velocityY, velocityZ, red, green, blue, alpha, size, lastFactor) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param offsetX float
	---@param offsetY float
	---@param offsetZ float
	---@param density int
	---@param handle Char
        addBlood = function(x, y, z, offsetX, offsetY, offsetZ, density, handle) end,
	---default
        ---| Draws colored light in radius of the specified point 
	---@param x float
	---@param y float
	---@param z float
	---@param red int
	---@param green int
	---@param blue int
	---@param radius float
        drawLightWithRange = function(x, y, z, red, green, blue, radius) end,
	---CLEO+
        ---| Returns the address of the specified FX system associated with the given particle, or 0 if handle is invalid 
	---@param particle Particle
	---@return bool
	---@return int address
        getAddress = function(particle) end,
	---CLEO+
        ---| Adds an FX system particle 
	---@param particle Particle
	---@param posX float
	---@param posY float
	---@param posZ float
	---@param velX float
	---@param velY float
	---@param velZ float
	---@param size float
	---@param brightness float
	---@param r float
	---@param g float
	---@param b float
	---@param a float
	---@param lastFactor float
        addParticle = function(particle, posX, posY, posZ, velX, velY, velZ, size, brightness, r, g, b, a, lastFactor) end,
	---CLEO+
        ---| Returns True if an FX system with the specified name is available 
	---@param name string
	---@return bool
        isAvailableWithName = function(name) end,
    }
SoundOp = {
        ---default
        ---| Plays a sound with the specified ID at the location 
	---@param x float
	---@param y float
	---@param z float
	---@param soundId ScriptSound
        addOneOffSound = function(x, y, z, soundId) end,
    }
WeatherOp = {
        ---default
        ---| Forces the game weather to the specified type 
	---@param type WeatherType
        force = function(type) end,
	---default
        ---| Forces the upcoming weather to the specified type 
	---@param type WeatherType
        forceNow = function(type) end,
	---default
        ---| Allows the game to continue its usual weather pattern after using 01B5 
        release = function() end,
	---default
        ---| Specifies whether the heat haze effect should be enabled in sunny conditions 
	---@param state bool
        setHeathazeEffect = function(state) end,
	---default
        ---| Sets the weather appropriate to the weather region the player is currently in 
        setToAppropriateTypeNow = function() end,
	---CLEO+
        ---| Gets weather type that is being blended from 
	---@return WeatherType type
        getCurrent = function() end,
	---CLEO+
        ---| Gets weather type that is being blended to 
	---@return WeatherType type
        getNext = function() end,
	---CLEO+
        ---| Sets weather type which will be blend to 
	---@param type WeatherType
        setNext = function(type) end,
	---CLEO+
        ---| Gets rain intensity in range of 0.0 to 1.0 
	---@return float intensity
        getRainIntensity = function() end,
	---CLEO+
        ---| Sets rain intensity in range of 0.0 to 1.0 
	---@param intensity float
        setRainIntensity = function(intensity) end,
	---CLEO+
        ---| Returns the intensity of the night filter 
	---@return float intensity
        getDayNightBalance = function() end,
	---CLEO+
        ---| Returns the intensity of the underwater filter 
	---@return float intensity
        getUnderwaterness = function() end,
	---CLEO+
        ---| Returns the forced weather type 
	---@return WeatherType weather
        getForced = function() end,
    }
PathOp = {
        ---default
        ---| Enables all car paths in the given area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        switchRoadsOn = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Disables all car paths in the given area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        switchRoadsOff = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Enables all ped paths in the given area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        switchPedRoadsOn = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Disables all ped paths in the given area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        switchPedRoadsOff = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Returns the nearest path node from the specified coordinates that a pedestrian can walk on 
	---@param x float
	---@param y float
	---@param z float
	---@return float nodeX
	---@return float nodeY
	---@return float nodeZ
        getClosestCharNode = function(x, y, z) end,
	---default
        ---| Returns the nearest path note from the specified coordinates that a vehicle can drive on 
	---@param x float
	---@param y float
	---@param z float
	---@return float nodeX
	---@return float nodeY
	---@return float nodeZ
        getClosestCarNode = function(x, y, z) end,
	---default
        ---| Returns the position and heading of the closest vehicle path node to the specified position 
	---@param x float
	---@param y float
	---@param z float
	---@return float nodeX
	---@return float nodeY
	---@return float nodeZ
	---@return float angle
        getClosestCarNodeWithHeading = function(x, y, z) end,
	---default
        ---| Gets two closest path nodes within the specified distance range 
	---@param x float
	---@param y float
	---@param z float
	---@param minDist float
	---@param maxDist float
	---@return float node1X
	---@return float node1Y
	---@return float node1Z
	---@return float node2X
	---@return float node2Y
	---@return float node2Z
	---@return float angle
        getClosestStraightRoad = function(x, y, z, minDist, maxDist) end,
	---default
        ---| Gets the coordinates of the nth car path node closest to the given coordinates 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param n int
	---@return float x
	---@return float y
	---@return float z
        getNthClosestCarNode = function(fromX, fromY, fromZ, n) end,
	---default
        ---| Flushes the task route 
        flushRoute = function() end,
	---default
        ---| Adds a point to the task route 
	---@param x float
	---@param y float
	---@param z float
        extendRoute = function(x, y, z) end,
	---default
        ---| Adds an area where script created cars will avoid driving in 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
        loadPathNodesInArea = function(leftBottomX, leftBottomY, rightTopX, rightTopY) end,
	---default
        ---| Removes areas forbidden for scripted cars set up by 0606 
        releaseNodes = function() end,
	---default
        ---|  
	---@param xCoord float
	---@param yCoord float
	---@param zCoord float
	---@param nth int
	---@return float x
	---@return float y
	---@return float z
	---@return float heading
        getNthClosestCarNodeWithHeading = function(xCoord, yCoord, zCoord, nth) end,
	---default
        ---| Clears all previous patrol data to start a new patrol route, which can be used in combination with 0755 to create patrol routes 
        flushPatrolRoute = function() end,
	---default
        ---| Adds a new point to the patrol route 
	---@param x float
	---@param y float
	---@param z float
	---@param animationName string
	---@param animationFile string
        extendPatrolRoute = function(x, y, z, animationName, animationFile) end,
	---default
        ---| Reverts all changes to car paths done with SWITCH_ROADS_ON and SWITCH_ROADS_OFF 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        switchRoadsBackToOriginal = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Reverts all changes to ped paths done with SWITCH_PED_ROADS_ON and SWITCH_PED_ROADS_OFF 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        switchPedRoadsBackToOriginal = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
        markRoadNodeAsDontWander = function(x, y, z) end,
	---default
        ---|  
        unmarkAllRoadNodesAsDontWander = function() end,
    }
WorldOp = {
        ---default
        ---| Sets the quantity of traffic that will spawn in the game 
	---@param multiplier float
        setCarDensityMultiplier = function(multiplier) end,
	---default
        ---| Stores the ground position at the location 
	---@param x float
	---@param y float
	---@param z float
	---@return float groundZ
        getGroundZFor3DCoord = function(x, y, z) end,
	---default
        ---| Returns true if a projectile is in the specified 3D area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@return bool
        isProjectileInArea = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Removes all script fires (02CF) 
        removeAllScriptFires = function() end,
	---default
        ---| Returns the handle of a random car with the specified model in the specified 2D area, or -1 otherwise 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param modelId model_vehicle
	---@return Car handle
        getRandomCarOfTypeInArea = function(leftBottomX, leftBottomY, rightTopX, rightTopY, modelId) end,
	---default
        ---| Returns true if there is anything with the specified properties within the 3D area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param solid bool
	---@param car bool
	---@param char bool
	---@param object bool
	---@param particle bool
	---@return bool
        isAreaOccupied = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, solid, car, char, object, particle) end,
	---default
        ---| Returns true if there is an explosion of the specified type in the 3D area 
	---@param explosionType ExplosionType
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@return bool
        isExplosionInArea = function(explosionType, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Sets the visibility of the object closest to the specified coordinates, matching the specified model 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param modelId model_object
	---@param state bool
        setVisibilityOfClosestObjectOfType = function(x, y, z, radius, modelId, state) end,
	---default
        ---| Returns true if there is a vehicle in the specified area 
	---@param x float
	---@param y float
	---@param z float
	---@param radiusX float
	---@param radiusY float
	---@param radiusZ float
	---@return bool
        isPointObscuredByAMissionEntity = function(x, y, z, radiusX, radiusY, radiusZ) end,
	---default
        ---| Clears the area, removing all vehicles and pedestrians that are not marked as needed by a mission 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param clearParticles bool
        clearArea = function(x, y, z, radius, clearParticles) end,
	---default
        ---| Swaps a map model with another map model nearest to the center of the search area 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param fromModelId model_object
	---@param toModelId model_object
        swapNearestBuildingModel = function(x, y, z, radius, fromModelId, toModelId) end,
	---default
        ---| Sets whether the game should render the world or only the cutscene objects 
	---@param state bool
        switchProcessing = function(state) end,
	---default
        ---| Clears all cars in the specified 3D area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        clearAreaOfCars = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Starts spawning random cars at the specified location 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
        createRandomCarForCarPark = function(x, y, z, heading) end,
	---default
        ---| Sets the quantity of pedestrians to spawn in the game 
	---@param multiplier float
        setPedDensityMultiplier = function(multiplier) end,
	---default
        ---| Clears all pedestrians from the given area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        clearAreaOfChars = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Returns true if the pickup at the specified coordinates is available to be picked up 
	---@param x float
	---@param y float
	---@param z float
	---@return bool
        isAnyPickupAtCoords = function(x, y, z) end,
	---default
        ---| Returns appropriate coordinates for creating a pickup by a dead character 
	---@param char Char
	---@return float x
	---@return float y
	---@return float z
        getDeadCharPickupCoords = function(char) end,
	---default
        ---| Creates a roadblock in the specified area with the specified type 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param type int
        createScriptRoadblock = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, type) end,
	---default
        ---| Removes references to all created roadblocks (04C0), freeing game memory 
        clearAllScriptRoadblocks = function() end,
	---default
        ---| Creates a trigger zone for police to appear during chases 
	---@param type SetPieceType
	---@param fromX float
	---@param fromY float
	---@param toX float
	---@param toY float
	---@param spawnPoliceAAtX float
	---@param spawnPoliceAAtY float
	---@param headedTowardsAAtX float
	---@param headedTowardsAAtY float
	---@param spawnPoliceBAtX float
	---@param spawnPoliceBAtY float
	---@param headedTowardsBAtX float
	---@param headedTowardsBAtY float
        addSetPiece = function(type, fromX, fromY, toX, toY, spawnPoliceAAtX, spawnPoliceAAtY, headedTowardsAAtX, headedTowardsAAtY, spawnPoliceBAtX, spawnPoliceBAtY, headedTowardsBAtX, headedTowardsBAtY) end,
	---default
        ---| Sets the extra color of the sky 
	---@param color int
	---@param fade bool
        setExtraColors = function(color, fade) end,
	---default
        ---| Clears the extra color of the sky 
	---@param withFade bool
        clearExtraColors = function(withFade) end,
	---default
        ---| Loops through the pool of vehicles to retrieve one that matches the specified model in the specified 2D area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param modelId model_vehicle
	---@return Car handle
        getRandomCarOfTypeInAreaNoSave = function(leftBottomX, leftBottomY, rightTopX, rightTopY, modelId) end,
	---default
        ---|  
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@param energy int
        fireSingleBullet = function(fromX, fromY, fromZ, toX, toY, toZ, energy) end,
	---default
        ---| Checks if there is something in the range of the two specified points 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@param buildings bool
	---@param cars bool
	---@param chars bool
	---@param objects bool
	---@param particles bool
	---@return bool
        isLineOfSightClear = function(fromX, fromY, fromZ, toX, toY, toZ, buildings, cars, chars, objects, particles) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return int numFires
        getNumberOfFiresInRange = function(x, y, z, radius) end,
	---default
        ---| Destroys all script-created trains 
        deleteMissionTrains = function() end,
	---default
        ---| Destroys all trains, including those that are not created by the script 
        deleteAllTrains = function() end,
	---default
        ---| Gets the percentage of the number of tags sprayed in the area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@return int percent
        getPercentageTaggedInArea = function(leftBottomX, leftBottomY, rightTopX, rightTopY) end,
	---default
        ---| Sets whether all tags in the area are sprayed 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param percent int
        setTagStatusInArea = function(leftBottomX, leftBottomY, rightTopX, rightTopY, percent) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param modelId model_object
	---@param smashed bool
	---@param damaged bool
	---@return bool
        isClosestObjectOfTypeSmashedOrDamaged = function(x, y, z, radius, modelId, smashed, damaged) end,
	---default
        ---| Returns true if there's any fire particles within the specified area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isFlameInAngledArea2D = function(leftBottomX, leftBottomY, rightTopX, rightTopY, angle, drawSphere) end,
	---default
        ---| Returns true if there's any flames within the specified area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@param angle float
	---@param drawSphere bool
	---@return bool
        isFlameInAngledArea3D = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param model model_vehicle
	---@return Car handle
        getRandomCarInSphereNoSave = function(x, y, z, radius, model) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param civilian bool
	---@param gang bool
	---@param criminal bool
	---@return Char handle
        getRandomCharInSphere = function(x, y, z, radius, civilian, gang, criminal) end,
	---default
        ---| Gets the number of fires within the specified area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@return int numFires
        getNumberOfFiresInArea = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---|  
	---@param xCoord float
	---@param yCoord float
	---@param zCoord float
	---@return float x
	---@return float y
	---@return float z
        getNearestTagPosition = function(xCoord, yCoord, zCoord) end,
	---default
        ---|  
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
        removeOilPuddlesInArea = function(leftBottomX, leftBottomY, rightTopX, rightTopY) end,
	---default
        ---| Returns the city the specified location is within 
	---@param x float
	---@param y float
	---@param z float
	---@return Level level
        getCityFromCoords = function(x, y, z) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param modelId model_object
	---@return bool
        hasObjectOfTypeBeenSmashed = function(x, y, z, radius, modelId) end,
	---default
        ---| Locates the enex marker via the specified name and sets whether it is visible and usable 
	---@param interiorName string
	---@param state bool
        switchEntryExit = function(interiorName, state) end,
	---default
        ---| Stores the coordinates of the nearest car park node in the specified area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@return float x
	---@return float y
	---@return float z
        getParkingNodeInArea = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Creates a trigger for a Unique Jump bonus 
	---@param startX float
	---@param startY float
	---@param startZ float
	---@param startRadiusX float
	---@param startRadiusY float
	---@param startRadiusZ float
	---@param finishX float
	---@param finishY float
	---@param finishZ float
	---@param finishRadiusX float
	---@param finishRadiusY float
	---@param finishRadiusZ float
	---@param cameraX float
	---@param cameraY float
	---@param cameraZ float
	---@param reward int
        addStuntJump = function(startX, startY, startZ, startRadiusX, startRadiusY, startRadiusZ, finishX, finishY, finishZ, finishRadiusX, finishRadiusY, finishRadiusZ, cameraX, cameraY, cameraZ, reward) end,
	---default
        ---| Creates a pool collision object 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
        setPoolTableCoords = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Gets the level that the character can hear noise at the specified position 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@return float level
        getSoundLevelAtCoords = function(handle, x, y, z) end,
	---default
        ---| Creates an emergency service vehicle on the closest road to the specified coordinates 
	---@param model model_vehicle
	---@param x float
	---@param y float
	---@param z float
        createEmergencyServicesCar = function(model, x, y, z) end,
	---default
        ---| Gets the closest object which can be stolen for burglary missions 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return Object handle
        getClosestStealableObject = function(x, y, z, radius) end,
	---default
        ---| Creates a flock of birds flying in the specified direction 
	---@param xFrom float
	---@param yFrom float
	---@param zFrom float
	---@param xTo float
	---@param yTo float
	---@param zTo float
	---@param quantity int
	---@param type int
        createBirds = function(xFrom, yFrom, zFrom, xTo, yTo, zTo, quantity, type) end,
	---default
        ---| Toggles collision of the object closest to the given coordinates and matching the model 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param modelId model_object
	---@param state bool
        setUsesCollisionOfClosestObjectOfType = function(x, y, z, radius, modelId, state) end,
	---default
        ---| Loops through the ped pool and returns the first character that is within the specified radius and has the "buys drugs" flag set in peds 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return Char handle
        getRandomCharInSphereOnlyDrugsBuyers = function(x, y, z, radius) end,
	---default
        ---| Finds the nearest character to the specified point, in the specified radius 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return Char handle
        getRandomCharInSphereNoBrain = function(x, y, z, radius) end,
	---default
        ---| Disables all entry/exit markers 
	---@param state bool
        disableAllEntryExits = function(state) end,
	---default
        ---| Returns the character using a map attractor with the specified model in the specified area 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param modelId model_object
	---@param attractorName string
	---@return Char handle
        getUserOfClosestMapAttractor = function(x, y, z, radius, modelId, attractorName) end,
	---default
        ---| Gets the height of the water at the specified 2D coordinates 
	---@param x float
	---@param y float
	---@param waves bool
	---@return float height
        getWaterHeightAtCoords = function(x, y, waves) end,
	---default
        ---| Flattens water waves 
        syncWater = function() end,
	---default
        ---| Removes all fires within the specified area 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
        extinguishFireAtPoint = function(x, y, z, radius) end,
	---default
        ---| Sets whether collision of the object closest to the given coordinates and matching the model applies to the target character 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param modelId model_object
	---@param state bool
	---@param target Char
        setCharUsesCollisionClosestObjectOfType = function(x, y, z, radius, modelId, state, target) end,
	---default
        ---| Marks all fires as no longer needed, allowing them to disappear 
        clearAllScriptFireFlags = function() end,
	---default
        ---| This command is like 098E, except it finds the appropriate enex marker via its position instead of its name 
	---@param x float
	---@param y float
	---@param radius float
	---@param flag EntryexitsFlag
	---@param state bool
        setClosestEntryExitFlag = function(x, y, radius, flag, state) end,
	---default
        ---|  
	---@param leftBottomX float
	---@param leftBottomY float
	---@param rightTopX float
	---@param rightTopY float
	---@param angle float
	---@param modelId model_vehicle
	---@return Car vehicle
        getRandomCarOfTypeInAngledAreaNoSave = function(leftBottomX, leftBottomY, rightTopX, rightTopY, angle, modelId) end,
	---default
        ---| Returns true if there's any kind of police vehicle in the specified 3D area 
	---@param leftBottomX float
	---@param leftBottomY float
	---@param leftBottomZ float
	---@param rightTopX float
	---@param rightTopY float
	---@param rightTopZ float
	---@return bool
        isCopVehicleInArea3DNoSave = function(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ) end,
	---default
        ---| Returns true if a money pickup exists near the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return bool
        isMoneyPickupAtCoords = function(x, y, z) end,
	---default
        ---| Returns the first char in the ped pool within radius of the specified point 
	---@param x float
	---@param y float
	---@param z float
	---@param radiusX float
	---@param radiusY float
	---@param radiusZ float
	---@return Char handle
        getRandomCharInAreaOffsetNoSave = function(x, y, z, radiusX, radiusY, radiusZ) end,
	---default
        ---| Sets the friction/slowdown rate on all rail tracks 
	---@param mult float
        setRailtrackResistanceMult = function(mult) end,
	---CLEO
        ---| Gets the coordinates of the location targeted in the game map 
	---@return bool
	---@return float x
	---@return float y
	---@return float z
        getTargetCoords = function() end,
	---CLEO
        ---| Creates a vehicle with the model (no pre-loading needed) in front of the player 
	---@param modelId model_vehicle
        spawnVehicleByCheating = function(modelId) end,
	---CLEO
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param findNext bool
	---@param filter CharSearchFilter
	---@return bool
	---@return Char handle
        getRandomCharInSphereNoSaveRecursive = function(x, y, z, radius, findNext, filter) end,
	---CLEO
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param findNext bool
	---@param skipWrecked bool
	---@return bool
	---@return Car handle
        getRandomCarInSphereNoSaveRecursive = function(x, y, z, radius, findNext, skipWrecked) end,
	---CLEO
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param findNext bool
	---@return bool
	---@return Object handle
        getRandomObjectInSphereNoSaveRecursive = function(x, y, z, radius, findNext) end,
	---CLEO+
        ---| Returns the handle of a pickup at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param onlyValid bool
	---@return bool
	---@return Pickup handle
        getPickupThisCoord = function(x, y, z, onlyValid) end,
	---CLEO+
        ---| Returns 2D screen position and distance related text size for world coordinates between nearClip and farClip 
	---@param x float
	---@param y float
	---@param z float
	---@param nearClip bool
	---@param farClip bool
	---@return bool
	---@return float x
	---@return float y
	---@return float sizeX
	---@return float sizeY
        convert3DToScreen2D = function(x, y, z, nearClip, farClip) end,
	---CLEO+
        ---| Returns the closestCop to the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param alive bool
	---@param inCar bool
	---@param onFoot bool
	---@return bool
	---@return Char closestCop
        getClosestCopNearPos = function(x, y, z, radius, alive, inCar, onFoot) end,
	---CLEO+
        ---| Returns the handle of anyChar in the pool, starting at the previously returned progress index 
	---@param progress int
	---@return bool
	---@return int progress
	---@return Char anyChar
        getAnyCharNoSaveRecursive = function(progress) end,
	---CLEO+
        ---| Returns the handle of anyCar in the pool, starting at the previously returned progress index 
	---@param progress int
	---@return bool
	---@return int progress
	---@return Car anyCar
        getAnyCarNoSaveRecursive = function(progress) end,
	---CLEO+
        ---| Returns the handle of anyObject in the pool, starting at the previously returned progress index 
	---@param progress int
	---@return bool
	---@return int progress
	---@return Object anyObject
        getAnyObjectNoSaveRecursive = function(progress) end,
	---CLEO+
        ---| Returns 2D coordinates for a location relative to x and y at the specified angle and distance 
	---@param x float
	---@param y float
	---@param angle float
	---@param distance float
	---@return float x
	---@return float y
        getCoordFromAngledDistance = function(x, y, angle, distance) end,
    }
GameOp = {
        ---default
        ---| Sets the maximum wanted level the player can receive 
	---@param wantedLevel int
        setMaxWantedLevel = function(wantedLevel) end,
	---default
        ---| Sets whether cops should ignore the player regardless of wanted level 
	---@param player Player
	---@param state bool
        setPoliceIgnorePlayer = function(player, state) end,
	---default
        ---| Sets the total number of hidden packages to collect 
	---@param amount int
        setCollectableTotal = function(amount) end,
	---default
        ---| Defines whether the player can respray their car for free 
	---@param state bool
        setFreeResprays = function(state) end,
	---default
        ---| Makes pedestrians pay no attention to the player 
	---@param player Player
	---@param state bool
        setEveryoneIgnorePlayer = function(player, state) end,
	---default
        ---| Sets sensitivity to crime, changing how many crimes a player can commit before police begin to pursue 
	---@param multiplier float
        setWantedMultiplier = function(multiplier) end,
	---default
        ---| Displays a screen prompting the player to save 
        activateSaveMenu = function() end,
	---default
        ---| Returns true if the player has saved their game 
	---@return bool
        hasSaveGameFinished = function() end,
	---default
        ---| Sets whether all cars receive damage 
	---@param state bool
        setAllCarsCanBeDamaged = function(state) end,
	---default
        ---| Returns true if the game language is set to German 
	---@return bool
        isGerman = function() end,
	---default
        ---| Returns true if the game uses metric measurements (meters instead of feet) 
	---@return bool
        areMeasurementsInMeters = function() end,
	---default
        ---| Returns true if the player has used any of the cheats 
	---@return bool
        areAnyCarCheatsActivated = function() end,
	---default
        ---| Returns true on PC versions of the game 
	---@return bool
        isPcVersion = function() end,
	---default
        ---| Gets the maximum wanted level the player can receive 
	---@return int level
        getMaxWantedLevel = function() end,
	---default
        ---| Toggles whether all taxis have nitrous 
	---@param state bool
        setAllTaxisHaveNitro = function(state) end,
	---default
        ---| Greys out the radar 
	---@param state bool
        setIsInStadium = function(state) end,
	---default
        ---| Returns true if the current game is an Australian release 
	---@return bool
        isAustralian = function() end,
	---default
        ---| Enables the LS Riots, making smoke appear on houses, random car fires occur, peds stealing things and attacking each other in a frenzy 
	---@param state bool
        setLaRiots = function(state) end,
	---default
        ---| Sets whether emergency traffic spawns 
	---@param state bool
        switchEmergencyServices = function(state) end,
	---default
        ---| Sets whether trains are generated 
	---@param state bool
        switchRandomTrains = function(state) end,
	---default
        ---| Sets how far apart players can get on 2-player mode 
	---@param distance float
        limitTwoPlayerDistance = function(distance) end,
	---default
        ---| Releases the distance limit set by LIMIT_TWO_PLAYER_DISTANCE 
        releaseTwoPlayerDistance = function() end,
	---default
        ---| Enables each player to target the other player 
	---@param state bool
        setPlayerPlayerTargeting = function(state) end,
	---default
        ---| Sets whether the players can be in separate cars during a 2-player mission 
	---@param state bool
        setPlayersCanBeInSeparateCars = function(state) end,
	---default
        ---| Disables the game from creating police bikes and their riders on the roads 
	---@param state bool
        switchCopsOnBikes = function(state) end,
	---default
        ---| Sets the attitude of peds with one pedtype towards peds of another pedtype 
	---@param relationshipType RelationshipType
	---@param ofPedType PedType
	---@param toPedType PedType
        setRelationship = function(relationshipType, ofPedType, toPedType) end,
	---default
        ---|  
	---@param relationshipType RelationshipType
	---@param ofPedType PedType
	---@param toPedType PedType
        clearRelationship = function(relationshipType, ofPedType, toPedType) end,
	---default
        ---| Enables or disables the SAM site at the Area 51 
	---@param state bool
        setArea51SamSite = function(state) end,
	---default
        ---| Returns true if the specified relationship between ped types is set 
	---@param relationshipType RelationshipType
	---@param ofPedType PedType
	---@param toPedType PedType
	---@return bool
        isRelationshipSet = function(relationshipType, ofPedType, toPedType) end,
	---default
        ---| Returns true if the game is in 2-player mode 
	---@return bool
        is2PlayerGameGoingOn = function() end,
	---default
        ---| Sets the limit on how many fires can be created from other fires when "propagation" was enabled on 02CF 
	---@param limit int
        setMaxFireGenerations = function(limit) end,
	---default
        ---| Enables ped spawning in interiors 
	---@param state bool
        activateInteriorPeds = function(state) end,
	---default
        ---| Enables the entry/exit marker in the specified radius of the coordinates 
	---@param x float
	---@param y float
	---@param radius float
	---@param state bool
        enableEntryExitPlayerGroupWarping = function(x, y, radius, state) end,
	---default
        ---| Returns true in interactive interiors 
	---@param areaId int
	---@return bool
        isProceduralInteriorActive = function(areaId) end,
	---default
        ---| Sets whether gang wars can be started by the player or enemy gangs 
	---@param state bool
        setGangWarsActive = function(state) end,
	---default
        ---| Returns true if there is a gang war happening 
	---@return bool
        isGangWarGoingOn = function() end,
	---default
        ---| Allows the player to provoke turf wars while a mission is active 
	---@param state bool
        canTriggerGangWarWhenOnAMission = function(state) end,
	---default
        ---| Enables an increase in the distance that markers hovering above entities can be seen from 
	---@param state bool
        setAlwaysDraw3DMarkers = function(state) end,
	---default
        ---| Disables highlighting of gang territory on the map and radar 
	---@param state bool
        setGangWarsTrainingMission = function(state) end,
	---default
        ---| Enables night vision effects 
	---@param state bool
        setNightVision = function(state) end,
	---default
        ---| Enables thermal vision effects 
	---@param state bool
        setInfraredVision = function(state) end,
	---default
        ---| Sets whether or not the player loses their weapons and inventory when taken to hospital 
	---@param state bool
        switchDeathPenalties = function(state) end,
	---default
        ---| Sets whether or not the player loses their weapons and inventory when busted 
	---@param state bool
        switchArrestPenalties = function(state) end,
	---default
        ---| Sets whether gang members will spawn 
	---@param state bool
        setCreateRandomGangMembers = function(state) end,
	---default
        ---| Sets the maximum number of members that the player can recruit 
	---@param maxSize int
        setScriptLimitToGangSize = function(maxSize) end,
	---default
        ---| Enables turf wars to be provoked in all zones 
        clearSpecificZonesToTriggerGangWar = function() end,
	---default
        ---| Enables or disables planes 
	---@param state bool
        switchAmbientPlanes = function(state) end,
	---default
        ---| Returns a number of group members the player can recruit with the current respect 
	---@return int maxNum
        findMaxNumberOfGroupMembers = function() end,
	---default
        ---| Sets whether ghetto birds spawn 
	---@param state bool
        switchPoliceHelis = function(state) end,
	---default
        ---|  
        forceDeathRestart = function() end,
	---default
        ---| Emulates the shared effects of being wasted or busted 
        resetStuffUponResurrection = function() end,
	---default
        ---| Sets whether gangs appear everywhere, like when "Gangs control the streets" cheat is activated 
	---@param state bool
        setOnlyCreateGangMembers = function(state) end,
	---default
        ---|  
	---@param range float
        setGunshotSenseRangeForRiot2 = function(range) end,
	---default
        ---| Sets the specified enex flag 
	---@param name string
	---@param flag EntryexitsFlag
	---@param state bool
        setNamedEntryExitFlag = function(name, flag, state) end,
	---default
        ---| Returns true if night vision is active 
	---@return bool
        isNightVisionActive = function() end,
	---default
        ---|  
	---@param state bool
        setCreateRandomCops = function(state) end,
	---default
        ---| Enables entity blips showing on the radar and map while in interiors 
	---@param state bool
        showBlipsOnAllLevels = function(state) end,
	---default
        ---|  
	---@param state bool
        hideAllFrontendBlips = function(state) end,
	---default
        ---| Disables displaying help messages in other scripts 
	---@param state bool
        setMinigameInProgress = function(state) end,
	---default
        ---| Returns true if 09BD has been used in any script to disable help messages 
	---@return bool
        isMinigameInProgress = function() end,
	---default
        ---| Forces all cars spawned to be of the specified model 
	---@param modelId model_vehicle
        setForceRandomCarModel = function(modelId) end,
	---default
        ---| Returns true if subtitles are switched on in the settings menu 
	---@return bool
        areSubtitlesSwitchedOn = function() end,
	---default
        ---| Sets whether cops will chase and kill criminals when their task is 'TASK_COMPLEX_KILL_CRIMINAL' 
	---@param state bool
        enableAmbientCrime = function(state) end,
	---default
        ---| Suspends the current players wanted level 
        clearWantedLevelInGarage = function() end,
	---default
        ---| Ensures there is x amount of space for new members to be added to the players gang 
	---@param _p1 int
        makeRoomInPlayerGangForMissionPeds = function(_p1) end,
	---default
        ---| Enables missiles to be fired from the aircraft carrier by Easter Bay Naval Station, San Fierro 
	---@param state bool
        setAircraftCarrierSamSite = function(state) end,
	---default
        ---| Switches enex markers used for burglary missions on or off 
	---@param state bool
        enableBurglaryHouses = function(state) end,
	---default
        ---| Prevents all peds from attempting to start conversations with the player 
	---@param state bool
        shutAllCharsUp = function(state) end,
	---default
        ---| Gives all the weapons of player 1 to player 2 during a cooperative mission 
        doWeaponStuffAtStartOf2PGame = function() end,
	---default
        ---| Returns true if the player just exited the menu on the last frame 
	---@return bool
        hasGameJustReturnedFromFrontend = function() end,
	---default
        ---| Returns the current language set in the menu language settings 
	---@return Language languageSlot
        getCurrentLanguage = function() end,
	---default
        ---| Returns true if the player provoked a gang war or is defending territory 
	---@return bool
        isGangWarFightingGoingOn = function() end,
	---default
        ---| Returns true if the current language set is different from the previous language set 
	---@return bool
        hasLanguageChanged = function() end,
	---default
        ---|  
        manageAllPopulation = function() end,
	---default
        ---| Disables respray garages from opening for the player 
	---@param state bool
        setNoResprays = function(state) end,
	---default
        ---| Returns true if widescreen is switched on in the display settings 
	---@return bool
        isWidescreenOnInOptions = function() end,
	---default
        ---| Disables all vehicle lights from being rendered if enabled 
	---@param state bool
        forceAllVehicleLightsOff = function(state) end,
	---default
        ---| Sets whether sleeping with a prostitute earns you money instead of taking it away from you 
	---@param state bool
        activatePimpCheat = function(state) end,
	---default
        ---| Sets an unused flag at address 0x96A8A8 
	---@param state bool
        setScriptCoopGame = function(state) end,
	---default
        ---| Cancels any prostitute invitations received in-game and makes any current prostitutes quit 
        getRidOfPlayerProstitute = function() end,
	---default
        ---| Enables or disables all triggers of the specified type (0928 or 0929) 
	---@param type ScriptBrainAttachType
	---@param state bool
        switchObjectBrains = function(type, state) end,
	---default
        ---| Enables the player to access the pause menu while widescreen is enabled 
	---@param state bool
        allowPauseInWidescreen = function(state) end,
	---default
        ---| Returns true if players controls are set to joystick and not mouse+keyboard 
	---@return bool
        isPcUsingJoypad = function() end,
	---CLEO
        ---| Returns true if the game version is vanilla 1.0 
	---@return bool
        isVersionOriginal = function() end,
	---CLEO
        ---| Returns platform type info (device/operating system) 
	---@return Platform platform
        getPlatform = function() end,
	---CLEO+
        ---| Gets the game window width and height resolution 
	---@return int width
	---@return int height
        getCurrentResolution = function() end,
	---CLEO+
        ---| Returns true if the current game runs on San Andreas Multiplayer (SA-MP) 
	---@return bool
        isSamp = function() end,
	---CLEO+
        ---| Gets loaded save slot number. 0 if new game 
	---@return int slot
        getCurrentSaveSlot = function() end,
	---CLEO+
        ---| Is first gameplay start (game was not reloaded) 
	---@return bool
        isFirstStart = function() end,
	---CLEO+
        ---| Returns True every mod number of frames 
	---@param mod int
	---@return bool
        frameMod = function(mod) end,
	---CLEO+
        ---| Returns True if the specified cheat is togglable and active 
	---@param cheat Cheats
	---@return bool
        isCheatActive = function(cheat) end,
	---CLEO+
        ---| Returns True if menu Select was just pressed 
	---@return bool
        isSelectMenuJustPressed = function() end,
	---CLEO+
        ---|  
	---@param restoreCamera bool
        disableSecondPlayer = function(restoreCamera) end,
	---CLEO+
        ---| Enables fixes for making two players use separated cars 
        fixTwoPlayersSeparatedCars = function() end,
	---CLEO+
        ---| Sets the game's On Mission status without referencing a global variable 
	---@param status bool
        setOnMission = function(status) end,
	---imgui
        ---| Returns game FPS 
	---@return int fps
        getFramerate = function() end,
    }
KillFrenzyOp = {
        ---default
        ---| Starts a rampage 
	---@param text gxt_key
	---@param weaponType WeaponType
	---@param timeInMs int
	---@param targetsNum int
	---@param targetModel1 model_any
	---@param targetModel2 model_any
	---@param targetModel3 model_any
	---@param targetModel4 model_any
	---@param betaSoundsAndMessages bool
        start = function(text, weaponType, timeInMs, targetsNum, targetModel1, targetModel2, targetModel3, targetModel4, betaSoundsAndMessages) end,
	---default
        ---| Returns the status of the current rampage 
	---@return int status
        readStatus = function() end,
	---default
        ---| Cancels current rampage, setting the rampage status to failed 
        fail = function() end,
    }
PickupOp = {
        ---default
        ---| Creates a pickup with the given model and type 
	---@param modelId model_object
	---@param pickupType PickupType
	---@param x float
	---@param y float
	---@param z float
	---@return Pickup handle
        create = function(modelId, pickupType, x, y, z) end,
	---default
        ---| Returns true if specified pickup has been collected 
	---@param self Pickup
	---@return bool
        hasBeenCollected = function(self) end,
	---default
        ---| Destroys the specified pickup, freeing game memory 
	---@param self Pickup
        remove = function(self) end,
	---default
        ---| Creates a money pickup with the specified cash value 
	---@param x float
	---@param y float
	---@param z float
	---@param cashAmount int
	---@param permanent bool
	---@return Pickup handle
        createMoney = function(x, y, z, cashAmount, permanent) end,
	---default
        ---| Creates a weapon pickup, giving the player the specified amount of ammo when they pick it up 
	---@param modelId model_object
	---@param pickupType PickupType
	---@param ammo int
	---@param x float
	---@param y float
	---@param z float
	---@return Pickup handle
        createWithAmmo = function(modelId, pickupType, ammo, x, y, z) end,
	---default
        ---| Creates an asset revenue pickup 
	---@param x float
	---@param y float
	---@param z float
	---@param revenueLimit int
	---@param revenueRate int
	---@return Pickup handle
        createProtection = function(x, y, z, revenueLimit, revenueRate) end,
	---default
        ---| Creates an asset icon for an asset that is not for sale 
	---@param x float
	---@param y float
	---@param z float
	---@param message gxt_key
	---@return Pickup handle
        createLockedProperty = function(x, y, z, message) end,
	---default
        ---| Creates an asset pickup for an asset which can be bought 
	---@param x float
	---@param y float
	---@param z float
	---@param price int
	---@param message gxt_key
	---@return Pickup handle
        createForSaleProperty = function(x, y, z, price, message) end,
	---default
        ---| Returns the X, Y and Z coordinates of the pickup 
	---@param self Pickup
	---@return float x
	---@return float y
	---@return float z
        getCoordinates = function(self) end,
	---default
        ---|  
	---@param self Pickup
	---@param value int
        updateMoneyPerDay = function(self, value) end,
	---default
        ---| Creates a collectible snapshot at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return Pickup handle
        createSnapshot = function(x, y, z) end,
	---default
        ---| Creates a collectible horseshoe at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return Pickup handle
        createHorseshoe = function(x, y, z) end,
	---default
        ---| Creates a collectible oyster at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return Pickup handle
        createOyster = function(x, y, z) end,
	---default
        ---| Returns true if the handle is a valid pickup handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---CLEO+
        ---| Returns the model of a specified pickup 
	---@param self Pickup
	---@return bool
	---@return model_any modelId
        getModel = function(self) end,
	---CLEO+
        ---| Returns a pointer to the struct of a specified pickup 
	---@param self Pickup
	---@return bool
	---@return int pointer
        getPointer = function(self) end,
	---CLEO+
        ---| Returns the type of a specified pickup 
	---@param self Pickup
	---@return PickupType type
        getType = function(self) end,
	---SAMPFUNCS
        ---| Returns the SAMP ID of a pickup using its handle 
	---@param self Pickup
	---@return bool
	---@return int id
        getSampId = function(self) end,
    }
GarageOp = {
        ---default
        ---| Sets the specified garage to only accept the specified vehicle 
	---@param garageName GarageName
	---@param vehicle Car
        setTargetCarForMission = function(garageName, vehicle) end,
	---default
        ---| Activates the garage 
	---@param garageId string
        activate = function(garageId) end,
	---default
        ---| Deactivates the garage 
	---@param garageId string
        deactivate = function(garageId) end,
	---default
        ---| Sets the garage's type 
	---@param garageId string
	---@param type GarageType
        changeType = function(garageId, type) end,
	---default
        ---| Opens the garage 
	---@param garageId string
        open = function(garageId) end,
	---default
        ---| Closes the garage 
	---@param garageId string
        close = function(garageId) end,
	---default
        ---| Returns true if the garage's door is open 
	---@param garageId string
	---@return bool
        isOpen = function(garageId) end,
	---default
        ---| Returns true if the garage's door is closed 
	---@param garageId string
	---@return bool
        isClosed = function(garageId) end,
	---default
        ---|  
	---@param garageId string
	---@param state bool
        setResprayFree = function(garageId, state) end,
    }
GangOp = {
        ---default
        ---| Sets the weapons that the specified gang can use 
	---@param gangId GangType
	---@param weaponType1 WeaponType
	---@param weaponType2 WeaponType
	---@param weaponType3 WeaponType
        setWeapons = function(gangId, weaponType1, weaponType2, weaponType3) end,
    }
StreamingOp = {
        ---default
        ---| Requests a special character's model to be loaded into the specified slot 
	---@param slotId int
	---@param modelName string
        loadSpecialCharacter = function(slotId, modelName) end,
	---default
        ---| Returns true if the special character's model (023C) is available for creation 
	---@param slotId int
	---@return bool
        hasSpecialCharacterLoaded = function(slotId) end,
	---default
        ---| Requests a new model to load 
	---@param modelId model_any
        requestModel = function(modelId) end,
	---default
        ---| Returns true if the model is available for creation 
	---@param modelId model_any
	---@return bool
        hasModelLoaded = function(modelId) end,
	---default
        ---| Releases the specified model, freeing game memory 
	---@param modelId model_any
        markModelAsNoLongerNeeded = function(modelId) end,
	---default
        ---| Releases the special character (023C), freeing game memory 
	---@param slotId int
        unloadSpecialCharacter = function(slotId) end,
	---default
        ---| Loads any requested models (0247 or 0353) synchronously 
        loadAllModelsNow = function() end,
	---default
        ---| Sets the streaming of additional models like peds, cars, and maps 
	---@param state bool
        switch = function(state) end,
	---default
        ---| Starts loading a specific location, just like if the player was there, removing LOD textures 
	---@param x float
	---@param y float
	---@param z float
        loadScene = function(x, y, z) end,
	---default
        ---| Returns true if the specified model exists in the loaded  
	---@param modelId model_any
	---@return bool
        isModelAvailable = function(modelId) end,
	---default
        ---| Sets the visibility of an interior area 
	---@param areaId int
        setAreaVisible = function(areaId) end,
	---default
        ---| Reloads the area at the specified coordinates 
	---@param x float
	---@param y float
        requestCollision = function(x, y) end,
	---default
        ---| Loads the specified IFP File 
	---@param animationFile string
        requestAnimation = function(animationFile) end,
	---default
        ---| Returns true if the specified IFP file is loaded 
	---@param animationFile string
	---@return bool
        hasAnimationLoaded = function(animationFile) end,
	---default
        ---| Releases the specified IFP file, freeing game memory 
	---@param animationFile string
        removeAnimation = function(animationFile) end,
	---default
        ---| Marks the train as no longer needed by the script, allowing it to be deleted by the game 
        markMissionTrainsAsNoLongerNeeded = function() end,
	---default
        ---| Returns a slot the upgrade model is for 
	---@param modelId model_object
	---@return ModSlot slotId
        getVehicleModType = function(modelId) end,
	---default
        ---| Loads the upgrade model and any associated models 
	---@param modelId model_object
        requestVehicleMod = function(modelId) end,
	---default
        ---| Returns true if the vehicle upgrade model has loaded 
	---@param modelId model_object
	---@return bool
        hasVehicleModLoaded = function(modelId) end,
	---default
        ---| Marks the vehicle upgrade model as no longer needed, allowing it to be unloaded by the streamer 
	---@param modelId model_object
        markVehicleModAsNoLongerNeeded = function(modelId) end,
	---default
        ---| Sets the town ID of the license plate which is created on the specified model, affecting which texture is chosen for the plate 
	---@param modelId model_vehicle
	---@param design CarPlateDesign
        customPlateDesignForNextCar = function(modelId, design) end,
	---default
        ---|  
	---@param iplName string
        requestIpl = function(iplName) end,
	---default
        ---|  
	---@param iplName string
        removeIpl = function(iplName) end,
	---default
        ---|  
	---@param iplName string
        removeIplDiscreetly = function(iplName) end,
	---default
        ---| Gets the current interior ID 
	---@return int areaId
        getAreaVisible = function() end,
	---default
        ---| Loads the specified car recording 
	---@param pathId int
        requestCarRecording = function(pathId) end,
	---default
        ---| Returns true if the car recording has finished loading 
	---@param pathId int
	---@return bool
        hasCarRecordingBeenLoaded = function(pathId) end,
	---default
        ---| Returns true if a file for the model exists 
	---@param modelId model_any
	---@return bool
        isModelInCdimage = function(modelId) end,
	---default
        ---|  
	---@param modelId model_any
	---@return float leftBottomBackX
	---@return float leftBottomBackY
	---@return float leftBottomBackZ
	---@return float rightTopFrontX
	---@return float rightTopFrontY
	---@return float rightTopFrontZ
        getModelDimensions = function(modelId) end,
	---default
        ---| Returns true if the model is the model of a boat 
	---@param modelId model_any
	---@return bool
        isThisModelABoat = function(modelId) end,
	---default
        ---| Returns true if the model is the model of a plane 
	---@param modelId model_any
	---@return bool
        isThisModelAPlane = function(modelId) end,
	---default
        ---| Returns true if the model is the model of a helicopter 
	---@param modelId model_any
	---@return bool
        isThisModelAHeli = function(modelId) end,
	---default
        ---| Unloads the car recording 
	---@param pathId int
        removeCarRecording = function(pathId) end,
	---default
        ---| Sets an animation pack to be loaded along with the specified model 
	---@param pedModelId int
	---@param animationFile string
        attachAnimsToModel = function(pedModelId, animationFile) end,
	---default
        ---|  
	---@param normalOnly bool
	---@return model_vehicle modelId
	---@return int class
        getRandomCarModelInMemory = function(normalOnly) end,
	---default
        ---| Returns true if a valid car model is passed 
	---@param modelId model_any
	---@return bool
        isThisModelACar = function(modelId) end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
        loadSceneInDirection = function(x, y, z, heading) end,
	---CLEO
        ---|  
	---@param modelId model_vehicle
	---@return string carName
        getNameOfVehicleModel = function(modelId) end,
	---CLEO+
        ---| Returns the type of the specified model 
	---@param model model_any
	---@return ModelInfoType type
        getModelType = function(model) end,
	---CLEO+
        ---| Requests a priority modelID to be loaded 
	---@param modelId model_any
        requestPriorityModel = function(modelId) end,
	---CLEO+
        ---| This is a duplicate of LOAD_ALL_MODELS_NOW 
        loadAllPriorityModelsNow = function() end,
	---CLEO+
        ---|  
	---@param id int
	---@param name string
        loadSpecialCharacterForId = function(id, name) end,
	---CLEO+
        ---|  
	---@param id int
        unloadSpecialCharacterFromId = function(id) end,
	---CLEO+
        ---| Returns the modelID by name 
	---@param name string
	---@return bool
	---@return model_any modelId
        getModelByName = function(name) end,
	---CLEO+
        ---| Returns true if the specified model name is available as a valid special character 
	---@param name string
	---@return bool
        isModelAvailableByName = function(name) end,
	---CLEO+
        ---|  
        removeAllUnusedModels = function() end,
	---CLEO+
        ---| Removes the specified ModelID from memory if unused 
	---@param modelId model_any
        removeModelIfUnused = function(modelId) end,
	---CLEO+
        ---| Returns the address of the modelInfo of the specified modelId 
	---@param modelId model_any
	---@return bool
	---@return int modelInfo
        getModelInfo = function(modelId) end,
	---CLEO+
        ---|  
	---@param dff string
	---@param txd string
	---@return bool
	---@return int specialModel
        loadSpecialModel = function(dff, txd) end,
	---CLEO+
        ---|  
	---@param specialModel int
        removeSpecialModel = function(specialModel) end,
	---CLEO+
        ---|  
	---@param specialModel int
	---@return int clump
	---@return int atomic
	---@return int txdIndex
        getSpecialModelData = function(specialModel) end,
    }
CutsceneOp = {
        ---default
        ---| Sets the position for a cutscene 
	---@param x float
	---@param y float
	---@param z float
        setOffset = function(x, y, z) end,
	---default
        ---| Loads the data for the specified cutscene 
	---@param name string
        load = function(name) end,
	---default
        ---| Starts the loaded cutscene (02E4) 
        start = function() end,
	---default
        ---| Returns the time in milliseconds passed since the cutscene has started (02E7) 
	---@return int time
        getTime = function() end,
	---default
        ---| Returns true if the cutscene has finished 
	---@return bool
        hasFinished = function() end,
	---default
        ---| Ends the current cutscene, freeing game memory 
        clear = function() end,
	---default
        ---| Returns true if the cutscene was skipped 
	---@return bool
        wasSkipped = function() end,
	---default
        ---| Returns true if the cutscene has finished loading 
	---@return bool
        hasLoaded = function() end,
	---default
        ---| Stores the offset of the currently loaded cutscene 
	---@return float xOffset
	---@return float yOffset
	---@return float zOffset
        getOffset = function() end,
	---default
        ---|  
	---@param objectName string
	---@param animName string
        appendToNext = function(objectName, animName) end,
	---CLEO+
        ---| Returns true if a cutscene is active (02E7) 
	---@return bool
        isOn = function() end,
    }
ScriptFireOp = {
        ---default
        ---| Creates a fire at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@param propagation int
	---@param size int
	---@return ScriptFire handle
        start = function(x, y, z, propagation, size) end,
	---default
        ---| Returns true if the script fire has been put out 
	---@param self ScriptFire
	---@return bool
        isExtinguished = function(self) end,
	---default
        ---| Removes the script fire 
	---@param self ScriptFire
        remove = function(self) end,
	---default
        ---| Creates a script fire on the vehicle 
	---@param vehicle Car
	---@return ScriptFire handle
        createCarFire = function(vehicle) end,
	---default
        ---| Creates a script fire on the character 
	---@param char Char
	---@return ScriptFire handle
        createCharFire = function(char) end,
	---default
        ---| Gets the coordinates of the fire 
	---@param self ScriptFire
	---@return float x
	---@return float y
	---@return float z
        getCoords = function(self) end,
	---default
        ---| Returns true if the handle is a valid script fire handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
    }
BoatOp = {
        ---default
        ---| Makes the boat sail to the location 
	---@param self Boat
	---@param x float
	---@param y float
	---@param z float
        goto = function(self, x, y, z) end,
	---default
        ---| Turns off the car's engine 
	---@param self Boat
        stop = function(self) end,
	---default
        ---| Sets the boat's max speed 
	---@param self Boat
	---@param maxSpeed float
        setCruiseSpeed = function(self, maxSpeed) end,
	---default
        ---| Makes the boat stay motionless in the water 
	---@param self Boat
	---@param state bool
        anchor = function(self, state) end,
    }
ZoneOp = {
        ---default
        ---| Gets a random character in the specified zone whose pedtype matches the specified values 
	---@param zone zone_key
	---@param civilian bool
	---@param gang bool
	---@param criminalOrProstitute bool
	---@return Char handle
        getRandomChar = function(zone, civilian, gang, criminalOrProstitute) end,
	---default
        ---|  
	---@param zone zone_key
	---@param type ZoneType
        setPopulationType = function(zone, type) end,
	---default
        ---| Sets the total number of drug dealers in the zone 
	---@param zone zone_key
	---@param strength int
        setDealerStrength = function(zone, strength) end,
	---default
        ---| Returns the drug dealer density of the specified zone 
	---@param zone zone_key
	---@return int density
        getDealerStrength = function(zone) end,
	---default
        ---| Sets the density of the gang members in the specified zone 
	---@param zoneId zone_key
	---@param gangId GangType
	---@param density int
        setGangStrength = function(zoneId, gangId, density) end,
	---default
        ---| Returns the density of the gang members in the specified zone 
	---@param zone zone_key
	---@param gangId GangType
	---@return int density
        getGangStrength = function(zone, gangId) end,
	---default
        ---| Returns the GXT key associated with the zone at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return gxt_key key
        getTextKey = function(x, y, z) end,
	---default
        ---|  
	---@param zone zone_key
	---@param races RaceSet
        setPopulationRace = function(zone, races) end,
	---default
        ---| Sets the zone as the only zone where a turf war can be provoked 
	---@param zone zone_key
        setForGangWarsTraining = function(zone) end,
	---default
        ---| Resets all changes made to the zone info 
        initPopulationSettings = function() end,
	---default
        ---|  
	---@return int type
        getCurrentPopulationZoneType = function() end,
	---default
        ---| Returns the name of the zone at the specified coordinates 
	---@param x float
	---@param y float
	---@param z float
	---@return string name
        getName = function(x, y, z) end,
	---default
        ---|  
	---@param zone zone_key
        setTriggerGangWar = function(zone) end,
	---default
        ---| Sets whether the IPL defined audio for the specified area should play 
	---@param zone zone_key
	---@param state bool
        switchAudio = function(zone, state) end,
	---default
        ---| Sets whether cops should be prevented from spawning in the specified area 
	---@param zone zone_key
	---@param state bool
        setNoCops = function(zone, state) end,
	---default
        ---| Causes the players wanted level to be set at 4 when in restricted areas 
	---@param state bool
        setDisableMilitaryZones = function(state) end,
    }
StatOp = {
        ---default
        ---| Increases the progress made stat by the specified amount 
	---@param progress int
        playerMadeProgress = function(progress) end,
	---default
        ---| Sets the maximum progress the player can reach 
	---@param maxProgress int
        setProgressTotal = function(maxProgress) end,
	---default
        ---| Increments the number of mission attempts stat by one 
        registerMissionGiven = function() end,
	---default
        ---| Sets the GXT of the last mission passed 
	---@param key gxt_key
        registerMissionPassed = function(key) end,
	---default
        ---| Sets the total number of missions that can be completed 
	---@param numMissions int
        setTotalNumberOfMissions = function(numMissions) end,
	---default
        ---| Updates the stat if the value is lower than the current stat value 
	---@param id StatId
	---@param value int
        registerFastestTime = function(id, value) end,
	---default
        ---| Updates the race best position 
	---@param id StatId
	---@param position int
        registerBestPosition = function(id, position) end,
	---default
        ---| Gets the progress of completion as a percentage 
	---@return float percentage
        getProgressPercentage = function() end,
	---default
        ---| Sets the latest odd job mission passed 
        registerOddjobMissionPassed = function() end,
	---default
        ---| Increases the integer stat by the value given 
	---@param id StatId
	---@param value int
        incrementInt = function(id, value) end,
	---default
        ---| Increases the float stat by the value specified 
	---@param id StatId
	---@param value float
        incrementFloat = function(id, value) end,
	---default
        ---| Decreases the integer stat by the value given 
	---@param id StatId
	---@param value int
        decrementInt = function(id, value) end,
	---default
        ---| Decreases the float stat by the value given 
	---@param id StatId
	---@param value float
        decrementFloat = function(id, value) end,
	---default
        ---| Updates the specified integer stat 
	---@param id StatId
	---@param value int
        registerInt = function(id, value) end,
	---default
        ---| Sets the specified stat to the specified value, if the specified value is greater than the current stat value 
	---@param id StatId
	---@param value float
        registerFloat = function(id, value) end,
	---default
        ---| Sets the integer stat to the specified value 
	---@param id StatId
	---@param value int
        setInt = function(id, value) end,
	---default
        ---| Sets the float stat to the specified value 
	---@param id StatId
	---@param value float
        setFloat = function(id, value) end,
	---default
        ---| Returns the value of the specified integer stat 
	---@param id StatId
	---@return int value
        getInt = function(id) end,
	---default
        ---| Returns the value of the specified float stat 
	---@param id StatId
	---@return float value
        getFloat = function(id) end,
	---default
        ---| Gets the number of spraytags painted over 
	---@return int numTags
        findNumberTagsTagged = function() end,
	---default
        ---|  
	---@return int percentage
        getTerritoryUnderControlPercentage = function() end,
	---default
        ---| Displays help boxes indicating that the players stats have been updated 
	---@param state bool
        showUpdateStats = function(state) end,
	---default
        ---| Sets the total value of mission respect points (stat 228) 
	---@param value int
        setMissionRespectTotal = function(value) end,
	---default
        ---| Increments the earned mission respect points (stat 224) by the given value 
	---@param value int
        awardPlayerMissionRespect = function(value) end,
	---default
        ---| Increases the integer stat by the value given without displaying popup message 
	---@param id StatId
	---@param value int
        incrementIntNoMessage = function(id, value) end,
	---default
        ---| Increases the float stat by the value given without displaying popup message 
	---@param id StatId
	---@param value float
        incrementFloatNoMessage = function(id, value) end,
    }
TxdOp = {
        ---default
        ---| Loads a sprite from the most recently loaded texture dictionary (0390) 
	---@param spriteSlot int
	---@param textureName string
        loadSprite = function(spriteSlot, textureName) end,
	---default
        ---| Loads the texture dictionary for use in drawing sprites (038D) on the screen 
	---@param name string
        loadDictionary = function(name) end,
	---default
        ---| Unloads all currently loaded textures (038F), as well as texture dictionaries (0390), freeing game memory 
        remove = function() end,
	---CLEO+
        ---| Draws RwTexture or spriteSlot once on specific drawing event and optional mask, no limits 
	---@param rwTextureOrSprite int
	---@param drawEvent DrawEvent
	---@param posX float
	---@param posY float
	---@param sizeX float
	---@param sizeY float
	---@param angle float
	---@param depth float
	---@param fixAr bool
	---@param maskVertCount int
	---@param maskVertArray int
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
        drawTexturePlus = function(rwTextureOrSprite, drawEvent, posX, posY, sizeX, sizeY, angle, depth, fixAr, maskVertCount, maskVertArray, red, green, blue, alpha) end,
	---CLEO+
        ---| Returns the rwTexture pointer from sprite index 
	---@param spriteSlot int
	---@return int rwTexture
        getTextureFromSprite = function(spriteSlot) end,
    }
AudioOp = {
        ---default
        ---| Plays an audio file with the specified ID from the Audio directory 
	---@param soundId int
        playMissionPassedTune = function(soundId) end,
	---default
        ---| Loads the file from the audio directory 
	---@param slotId MissionAudioSlot
	---@param audioId int
        loadMissionAudio = function(slotId, audioId) end,
	---default
        ---| Returns true if the mission audio requested with LOAD_MISSION_AUDIO has loaded 
	---@param slotId MissionAudioSlot
	---@return bool
        hasMissionAudioLoaded = function(slotId) end,
	---default
        ---| Plays the loaded sound (03CF) 
	---@param slotId MissionAudioSlot
        playMissionAudio = function(slotId) end,
	---default
        ---| Returns true if the audio (03CF) is no longer playing 
	---@param slotId MissionAudioSlot
	---@return bool
        hasMissionAudioFinished = function(slotId) end,
	---default
        ---| Sets the location of the mission audio (03CF) where it can be heard 
	---@param slotId MissionAudioSlot
	---@param x float
	---@param y float
	---@param z float
        setMissionAudioPosition = function(slotId, x, y, z) end,
	---default
        ---| Unloads the mission audio (03CF), freeing game memory 
	---@param slotId MissionAudioSlot
        clearMissionAudio = function(slotId) end,
	---default
        ---| Sets the current radio station that is playing, if the player is in a vehicle 
	---@param channel RadioChannel
        setRadioChannel = function(channel) end,
	---default
        ---| Sets whether sounds should fade along with the screen 
	---@param state bool
        setMusicDoesFade = function(state) end,
	---default
        ---| Returns the current radio station that is being played 
	---@return RadioChannel channel
        getRadioChannel = function() end,
	---default
        ---|  
	---@param _p1 int
	---@return int _p2
	---@return int _p3
	---@return int _p4
        getBeatProximity = function(_p1) end,
	---default
        ---| Sets the loaded audio to play at the char's location 
	---@param slotId MissionAudioSlot
	---@param handle Char
        attachMissionAudioToChar = function(slotId, handle) end,
	---default
        ---|  
	---@param trackId int
        preloadBeatTrack = function(trackId) end,
	---default
        ---|  
	---@return int status
        getBeatTrackStatus = function() end,
	---default
        ---|  
        playBeatTrack = function() end,
	---default
        ---|  
        stopBeatTrack = function() end,
	---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param soundId ScriptSound
        reportMissionAudioEventAtPosition = function(x, y, z, soundId) end,
	---default
        ---|  
	---@param handle Object
	---@param soundId ScriptSound
        reportMissionAudioEventAtObject = function(handle, soundId) end,
	---default
        ---| Sets the loaded audio to play at the object's location 
	---@param slotId MissionAudioSlot
	---@param handle Object
        attachMissionAudioToObject = function(slotId, handle) end,
	---default
        ---| Sets whether the loaded soundtrack is paused 
	---@param state bool
        pauseCurrentBeatTrack = function(state) end,
	---default
        ---|  
	---@param handle Char
	---@param soundId ScriptSound
        reportMissionAudioEventAtChar = function(handle, soundId) end,
	---default
        ---| Plays the audio event at the car's position 
	---@param handle Car
	---@param soundId ScriptSound
        reportMissionAudioEventAtCar = function(handle, soundId) end,
	---default
        ---| Sets the loaded audio to play at the vehicle's location 
	---@param slotId MissionAudioSlot
	---@param handle Car
        attachMissionAudioToCar = function(slotId, handle) end,
	---default
        ---| Sets the radio station of the vehicle the player is currently in to the favourite station, retrieved from the stats (ID 326) 
        setRadioToPlayersFavouriteStation = function() end,
	---CLEO+
        ---| Gets the SFX volume set in the game options 
	---@return float volume
        getSfxVolume = function() end,
	---CLEO+
        ---| Gets the radio volume set in the game options 
	---@return float volume
        getRadioVolume = function() end,
    }
SphereOp = {
        ---default
        ---| Displays a red cylinder sphere 
	---@param x float
	---@param y float
	---@param z float
	---@param diameter float
        draw = function(x, y, z, diameter) end,
	---default
        ---| Creates a static sphere at the location, with the specified radius 
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@return Sphere handle
        create = function(x, y, z, radius) end,
	---default
        ---| Destroys a static sphere 
	---@param self Sphere
        remove = function(self) end,
	---Sphere
        ---| Changes the color of the sphere 
	---@param self Sphere
	---@param r int
	---@param g int
	---@param b int
        setColor = function(self, r, g, b) end,
    }
StuckCarCheckOp = {
        ---default
        ---| Adds the vehicle to the stuck cars array 
	---@param vehicle Car
	---@param distance float
	---@param time int
        add = function(vehicle, distance, time) end,
	---default
        ---| Removes the vehicle from the stuck cars array 
	---@param vehicle Car
        remove = function(vehicle) end,
	---default
        ---| Returns true if the car is stuck 
	---@param vehicle Car
	---@return bool
        isCarStuck = function(vehicle) end,
	---default
        ---| Attempts to automatically restore vehicles that get stuck or flipped 
	---@param vehicle Car
	---@param distance float
	---@param time int
	---@param stuck bool
	---@param flipped bool
	---@param inWater bool
	---@param numNodesToCheck int
        addWithWarp = function(vehicle, distance, time, stuck, flipped, inWater, numNodesToCheck) end,
    }
CreditsOp = {
        ---default
        ---| Makes the credits scroll up the screen 
        start = function() end,
	---default
        ---| Stops the credits text from showing 
        stop = function() end,
	---default
        ---| Returns true if the credits have finished 
	---@return bool
        areFinished = function() end,
    }
RcOp = {
        ---default
        ---| Puts the player in control of a remote-control vehicle 
	---@param handle Player
	---@param x float
	---@param y float
	---@param z float
	---@param angle float
	---@param modelId model_vehicle
        giveModelToPlayer = function(handle, x, y, z, angle, modelId) end,
	---default
        ---| Returns the player's radio-controlled vehicle (alts:00D9,03C0,0811) 
	---@param player Player
	---@return Car car
        getCar = function(player) end,
	---default
        ---| Enables a remote-control vehicle detonation 
	---@param state bool
        setEnableDetonate = function(state) end,
	---default
        ---| Sets whether RC Bandits detonate on contact with the wheels of any four-wheeled vehicles 
	---@param state bool
        setEnableDetonateOnContact = function(state) end,
	---default
        ---| Exits remote-control mode 
        removeBuggy = function() end,
	---default
        ---| Puts the specified player in control of a remote-control vehicle 
	---@param player Player
	---@param vehicle Car
        takeCar = function(player, vehicle) end,
    }
HeliOp = {
        ---default
        ---| Makes the helicopter fly to the specified location, keeping a specific Z height/altitude 
	---@param self Heli
	---@param x float
	---@param y float
	---@param z float
	---@param minAltitude float
	---@param maxAltitude float
        gotoCoords = function(self, x, y, z, minAltitude, maxAltitude) end,
	---default
        ---| Forces the heli rotation relative to the north 
	---@param self Heli
	---@param angle float
        setOrientation = function(self, angle) end,
	---default
        ---| Resets the heli rotation set with 04D0 
	---@param self Heli
        clearOrientation = function(self) end,
	---default
        ---| Limits the amount a helicopter can tilt 
	---@param self Heli
	---@param state bool
        setStabiliser = function(self, state) end,
	---default
        ---| Makes the Hunter helicopter fire cannon gun 
	---@param self Heli
        fireHunterGun = function(self) end,
	---default
        ---| Makes helicopter simulate crash landing, exploding on the way if high up 
	---@param self Heli
        makeComeCrashingDown = function(self) end,
	---default
        ---| Makes the heli follow and attack the current player in the given radius 
	---@param self Heli
	---@param handle Player
	---@param radius float
        attackPlayer = function(self, handle, radius) end,
	---default
        ---| Makes the heli follow the specified char or vehicle in the air 
	---@param self Heli
	---@param char Char
	---@param vehicle Car
	---@param radius float
        followEntity = function(self, char, vehicle, radius) end,
	---default
        ---| Makes the helicopter hunt down the character or the vehicle within the specified radius 
	---@param self Heli
	---@param char Char
	---@param vehicle Car
	---@param radius float
        chaseEntity = function(self, char, vehicle, radius) end,
	---default
        ---|  
	---@param self Heli
	---@param x float
	---@param y float
	---@param z float
	---@param minAltitude float
	---@param maxAltitude float
        landAtCoords = function(self, x, y, z, minAltitude, maxAltitude) end,
	---default
        ---|  
	---@param self Heli
	---@param char Char
	---@param vehicle Car
	---@param minAltitude float
	---@param maxAltitude any
        keepEntityInView = function(self, char, vehicle, minAltitude, maxAltitude) end,
	---default
        ---|  
	---@param self Heli
	---@param state bool
        attachWinch = function(self, state) end,
	---default
        ---|  
	---@param self Heli
        releaseEntityFromWinch = function(self) end,
	---default
        ---| Retrieves the entity attached to the heli's magnet and returns to specific variables depending on the entities type 
	---@param self Heli
	---@return Char char
	---@return Car vehicle
	---@return Object object
        grabEntityOnWinch = function(self) end,
	---default
        ---| Provides the heli with extra thrust power 
	---@param self Heli
	---@param power int
        activateSpeedCheat = function(self, power) end,
	---default
        ---| Makes the helicopter rotor spin at full speed instantly 
	---@param self Heli
        setBladesFullSpeed = function(self) end,
	---default
        ---|  
	---@param self Heli
	---@param distance int
        setReachedTargetDistance = function(self, distance) end,
	---default
        ---| Sets whether the helicopter sound is muted 
	---@param self Heli
	---@param state bool
        disableAudio = function(self, state) end,
    }
PlaneOp = {
        ---default
        ---|  
	---@param self Plane
	---@param x float
	---@param y float
	---@param z float
	---@param minAltitude float
	---@param maxAltitude float
        gotoCoords = function(self, x, y, z, minAltitude, maxAltitude) end,
	---default
        ---| Sets the planes mission to attack the player 
	---@param self Plane
	---@param handle Player
	---@param radius float
        attackPlayer = function(self, handle, radius) end,
	---default
        ---|  
	---@param self Plane
	---@param heading float
	---@param minAltitude float
	---@param maxAltitude float
        flyInDirection = function(self, heading, minAltitude, maxAltitude) end,
	---default
        ---|  
	---@param self Plane
	---@param char Char
	---@param vehicle Car
	---@param altitude float
        followEntity = function(self, char, vehicle, altitude) end,
	---default
        ---|  
	---@param self Plane
	---@param throttle float
        setThrottle = function(self, throttle) end,
	---default
        ---| Provides the aircraft with full power so it can start flying mid-air 
	---@param self Plane
        startsInAir = function(self) end,
	---default
        ---| Sets the plane mission to attack the player while maintaining the minimum altitude 
	---@param self Plane
	---@param player Player
	---@param altitude float
        attackPlayerUsingDogFight = function(self, player, altitude) end,
	---default
        ---| Sets whether the plane's landing wheels are up 
	---@param self Plane
	---@param state bool
        setUndercarriageUp = function(self, state) end,
	---default
        ---|  
	---@param self Plane
	---@return float position
        getUndercarriagePosition = function(self) end,
    }
TaskOp = {
        ---default
        ---| Makes the character crouch 
	---@param handle Char
	---@param state bool
        toggleDuck = function(handle, state) end,
	---default
        ---| Makes the character pause for the specified amount of time 
	---@param handle Char
	---@param time int
        pause = function(handle, time) end,
	---default
        ---| Makes the character stand still 
	---@param handle Char
	---@param time int
        standStill = function(handle, time) end,
	---default
        ---| Makes char fall to the ground and stay there for the specified time 
	---@param handle Char
	---@param fallDown bool
	---@param timeOnGround int
        fallAndGetUp = function(handle, fallDown, timeOnGround) end,
	---default
        ---| Makes the char perform a jump 
	---@param handle Char
	---@param state bool
        jump = function(handle, state) end,
	---default
        ---| Makes the char stop to regain breath 
	---@param handle Char
	---@param time int
        tired = function(handle, time) end,
	---default
        ---| Kills the character 
	---@param handle Char
        die = function(handle) end,
	---default
        ---| Makes the character look at another character 
	---@param observer Char
	---@param target Char
	---@param time int
        lookAtChar = function(observer, target, time) end,
	---default
        ---| Makes the char look at the specified vehicle 
	---@param char Char
	---@param vehicle Car
	---@param time int
        lookAtVehicle = function(char, vehicle, time) end,
	---default
        ---| Makes the character say a phrase from the specified audio table 
	---@param handle Char
	---@param phrase SpeechId
        say = function(handle, phrase) end,
	---default
        ---| Makes the char lift their hand up in the air angrily 
	---@param handle Char
        shakeFist = function(handle) end,
	---default
        ---| Makes the char stumble backwards with their arms in front of their face as if he is backing away from something in fear 
	---@param handle Char
        cower = function(handle) end,
	---default
        ---| Makes the char put their hands in the air 
	---@param handle Char
	---@param time int
        handsUp = function(handle, time) end,
	---default
        ---| Makes a character duck with their arms over head 
	---@param handle Char
	---@param time int
        duck = function(handle, time) end,
	---default
        ---| Makes a character use an ATM machine 
	---@param handle Char
        useAtm = function(handle) end,
	---default
        ---| Makes a character scratch their head while looking around 
	---@param handle Char
        scratchHead = function(handle) end,
	---default
        ---| Makes a character look out ahead 
	---@param handle Char
	---@param time int
        lookAbout = function(handle, time) end,
	---default
        ---| Makes a character approach the car and occupy the specified passenger seat 
	---@param char Char
	---@param vehicle Car
	---@param time int
	---@param seat SeatId
        enterCarAsPassenger = function(char, vehicle, time, seat) end,
	---default
        ---| Makes a character approach the car and occupy the driver seat 
	---@param char Char
	---@param vehicle Car
	---@param time int
        enterCarAsDriver = function(char, vehicle, time) end,
	---default
        ---| Makes the character exit the specified vehicle, if they are currently in it 
	---@param char Char
	---@param vehicle Car
        leaveCar = function(char, vehicle) end,
	---default
        ---| Makes the character exit the vehicle and flee to the specified position 
	---@param char Char
	---@param vehicle Car
	---@param x float
	---@param y float
	---@param z float
        leaveCarAndFlee = function(char, vehicle, x, y, z) end,
	---default
        ---|  
	---@param driver Char
	---@param vehicle Car
	---@param x float
	---@param y float
	---@param z float
	---@param speed float
	---@param driveStyle DriveMode
	---@param modelId model_vehicle
	---@param drivingStyle DrivingMode
        carDriveToCoord = function(driver, vehicle, x, y, z, speed, driveStyle, modelId, drivingStyle) end,
	---default
        ---| Makes the character drive around aimlessly in a vehicle 
	---@param char Char
	---@param vehicle Car
	---@param speed float
	---@param drivingMode DrivingMode
        carDriveWander = function(char, vehicle, speed, drivingMode) end,
	---default
        ---| Makes the character walk to the specified coordinates 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param speed MoveState
	---@param time int
        goStraightToCoord = function(handle, x, y, z, speed, time) end,
	---default
        ---| Rotates a character to the specified angle 
	---@param handle Char
	---@param heading float
        achieveHeading = function(handle, heading) end,
	---default
        ---| Makes the character follow the path route 
	---@param handle Char
	---@param speed MoveState
	---@param mode RouteMode
        followPointRoute = function(handle, speed, mode) end,
	---default
        ---| Approaches the character from any direction within the specified radius 
	---@param walking Char
	---@param target Char
	---@param time int
	---@param radius float
        gotoChar = function(walking, target, time, radius) end,
	---default
        ---| Makes the character run away from a point, scared and often screaming 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param time int
        fleePoint = function(handle, x, y, z, radius, time) end,
	---default
        ---| Makes the character run away from another character 
	---@param handle Char
	---@param threat Char
	---@param radius float
	---@param time int
        fleeChar = function(handle, threat, radius, time) end,
	---default
        ---| Makes the character run away from the specified coordinates 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param time int
        smartFleePoint = function(handle, x, y, z, radius, time) end,
	---default
        ---| Makes the character flee from another character 
	---@param handle Char
	---@param threat Char
	---@param radius float
	---@param time int
        smartFleeChar = function(handle, threat, radius, time) end,
	---default
        ---| Makes the character walk around the ped path 
	---@param handle Char
        wanderStandard = function(handle) end,
	---default
        ---| Makes a character attack another character on foot 
	---@param killer Char
	---@param target Char
        killCharOnFoot = function(killer, target) end,
	---default
        ---| Makes the character go to the specified coordinates 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param speed MoveState
	---@param time int
        followPathNodesToCoord = function(handle, x, y, z, speed, time) end,
	---default
        ---| Assigns the character the task of getting to the specified coordinates 
	---@param char Char
	---@param x float
	---@param y float
	---@param z float
	---@param speed MoveState
	---@param vehicle Car
        goToCoordAnyMeans = function(char, x, y, z, speed, vehicle) end,
	---default
        ---| Makes the character perform an animation 
	---@param handle Char
	---@param animationName string
	---@param animationFile string
	---@param blendSpeed float
	---@param loop bool
	---@param lockX bool
	---@param lockY bool
	---@param keepLastFrame bool
	---@param time int
        playAnim = function(handle, animationName, animationFile, blendSpeed, loop, lockX, lockY, keepLastFrame, time) end,
	---default
        ---| Makes the character jump out of the vehicle while it is in motion 
	---@param char Char
	---@param vehicle Car
        leaveCarImmediately = function(char, vehicle) end,
	---default
        ---| Makes the char exit the car, if he is in one 
	---@param handle Char
        leaveAnyCar = function(handle) end,
	---default
        ---|  
	---@param char Char
	---@param target Char
	---@param flags int
	---@param actionDelay int
	---@param actionChance int
        killCharOnFootWhileDucking = function(char, target, flags, actionDelay, actionChance) end,
	---default
        ---| Makes a character aim at another character 
	---@param char Char
	---@param target Char
	---@param time int
        aimGunAtChar = function(char, target, time) end,
	---default
        ---| Makes a character go to the location while shooting at another character 
	---@param char Char
	---@param x float
	---@param y float
	---@param z float
	---@param speed MoveState
	---@param turnRadius float
	---@param stopRadius float
	---@param target Char
        goToCoordWhileShooting = function(char, x, y, z, speed, turnRadius, stopRadius, target) end,
	---default
        ---| Makes the character stay in the same place 
	---@param handle Char
	---@param state bool
        stayInSamePlace = function(handle, state) end,
	---default
        ---| Makes a character face another character 
	---@param char Char
	---@param target Char
        turnCharToFaceChar = function(char, target) end,
	---default
        ---| Makes the character look at an object 
	---@param char Char
	---@param object Object
	---@param time any
        lookAtObject = function(char, object, time) end,
	---default
        ---| Makes the character aim at the specified coordinates 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param time int
        aimGunAtCoord = function(handle, x, y, z, time) end,
	---default
        ---| Makes the character turn round and shoot at the specified coordinates 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param time int
        shootAtCoord = function(handle, x, y, z, time) end,
	---default
        ---| Makes the character attack a vehicle 
	---@param char Char
	---@param vehicle Car
        destroyCar = function(char, vehicle) end,
	---default
        ---| Makes the character perform a dive in the specified direction 
	---@param handle Char
	---@param directionX float
	---@param directionY float
	---@param timeOnGround int
        diveAndGetUp = function(handle, directionX, directionY, timeOnGround) end,
	---default
        ---| Makes the character move to the seat on the right 
	---@param char Char
	---@param vehicle Car
        shuffleToNextCarSeat = function(char, vehicle) end,
	---default
        ---| Makes the character chat with another character 
	---@param char Char
	---@param other Char
	---@param leadSpeaker bool
	---@param _p4 int
        chatWithChar = function(char, other, leadSpeaker, _p4) end,
	---default
        ---|  
	---@param handle Char
	---@param onFoot bool
	---@param inCar bool
	---@param duringScriptTask bool
        togglePedThreatScanner = function(handle, onFoot, inCar, duringScriptTask) end,
	---default
        ---|  
	---@param handle Char
	---@param time int
        diveFromAttachmentAndGetUp = function(handle, time) end,
	---default
        ---| Approaches the char at the specified offset, specified by the radius and angle 
	---@param char Char
	---@param target Char
	---@param time int
	---@param radius float
	---@param heading float
        gotoCharOffset = function(char, target, time, radius, heading) end,
	---default
        ---| Makes the char look at the specified coordinates 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param time int
        lookAtCoord = function(handle, x, y, z, time) end,
	---default
        ---| Makes the char sit down for the specified amount of time 
	---@param handle Char
	---@param time int
        sitDown = function(handle, time) end,
	---default
        ---|  
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
        turnCharToFaceCoord = function(handle, x, y, z) end,
	---default
        ---|  
	---@param char Char
	---@param vehicle Car
	---@param speed int
        drivePointRoute = function(char, vehicle, speed) end,
	---default
        ---|  
	---@param char Char
	---@param x float
	---@param y float
	---@param z float
	---@param speed MoveState
	---@param turnRadius float
	---@param stopRadius float
	---@param target Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
        goToCoordWhileAiming = function(char, x, y, z, speed, turnRadius, stopRadius, target, xOffset, yOffset, zOffset) end,
	---default
        ---| Makes the AI driver perform the action in the vehicle for the specified period of time 
	---@param char Char
	---@param vehicle Car
	---@param actionId TempAction
	---@param time int
        carTempAction = function(char, vehicle, actionId, time) end,
	---default
        ---| Sets the car's current mission with various parameters 
	---@param char Char
	---@param vehicle Car
	---@param targetVehicle Car
	---@param missionId CarMission
	---@param cruiseSpeed float
	---@param drivingStyle DrivingMode
        carMission = function(char, vehicle, targetVehicle, missionId, cruiseSpeed, drivingStyle) end,
	---default
        ---| Makes the character go to an object 
	---@param char Char
	---@param object Object
	---@param time int
	---@param radius float
        goToObject = function(char, object, time, radius) end,
	---default
        ---|  
	---@param handle Char
	---@param direction bool
        weaponRoll = function(handle, direction) end,
	---default
        ---| Makes the character attempt to arrest another character 
	---@param char Char
	---@param target Char
        charArrestChar = function(char, target) end,
	---default
        ---| Attaches the specified char to an object with the optional addition of having it perform an animation 
	---@param char Char
	---@param object Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param bone PedBoneId
	---@param orientation HoldOrientation
	---@param animationName string
	---@param animationFile string
	---@param time int
        pickUpObject = function(char, object, xOffset, yOffset, zOffset, bone, orientation, animationName, animationFile, time) end,
	---default
        ---|  
	---@param handle Char
	---@param targetChar Char
	---@param targetVehicle Car
	---@param x float
	---@param y float
	---@param z float
	---@param radius float
	---@param type DriveByType
	---@param rightHandCarSeat bool
	---@param fireRate int
        driveBy = function(handle, targetChar, targetVehicle, x, y, z, radius, type, rightHandCarSeat, fireRate) end,
	---default
        ---| Makes a character pull out a cellphone, answer it, and hold it to their ear 
	---@param handle Char
	---@param start bool
        useMobilePhone = function(handle, start) end,
	---default
        ---| Warps the character into the specified vehicle's driver seat 
	---@param char Char
	---@param vehicle Car
        warpCharIntoCarAsDriver = function(char, vehicle) end,
	---default
        ---|  
	---@param char Char
	---@param vehicle Car
	---@param seatId SeatId
        warpCharIntoCarAsPassenger = function(char, vehicle, seatId) end,
	---default
        ---|  
	---@param char Char
	---@param attractor Attractor
        useAttractor = function(char, attractor) end,
	---default
        ---|  
	---@param handle Char
	---@param target Char
	---@param time int
        shootAtChar = function(handle, target, time) end,
	---default
        ---|  
	---@param handle Char
	---@param threat Char
	---@param safeDist float
	---@param time int
	---@param shoot bool
	---@param shootTime int
	---@param shootCooldownTime int
	---@param stealCarDist float
        fleeCharAnyMeans = function(handle, threat, safeDist, time, shoot, shootTime, shootCooldownTime, stealCarDist) end,
	---default
        ---| Kills the character 
	---@param handle Char
        dead = function(handle) end,
	---default
        ---|  
	---@param char Char
	---@param vehicle Car
	---@param time int
	---@param radius float
        gotoCar = function(char, vehicle, time, radius) end,
	---default
        ---| Makes the character jump and climb on an object 
	---@param handle Char
	---@param flag bool
        climb = function(handle, flag) end,
	---default
        ---|  
	---@param handle Char
	---@param target Char
	---@param radiusFrom float
	---@param radiusTo float
        gotoCharAiming = function(handle, target, radiusFrom, radiusTo) end,
	---default
        ---| Makes the character attack the specified character 
	---@param handle Char
	---@param target Char
	---@param time int
        killCharOnFootTimed = function(handle, target, time) end,
	---default
        ---|  
	---@param handle Char
        jetpack = function(handle) end,
	---default
        ---| Sets the decision maker used by the specified char 
	---@param char Char
	---@param handleOrTemplate DecisionMakerCharTemplate
        setCharDecisionMaker = function(char, handleOrTemplate) end,
	---default
        ---|  
	---@param char Char
	---@param object Object
        complexPickupObject = function(char, object) end,
	---default
        ---|  
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param angle float
	---@param radius float
        charSlideToCoord = function(handle, x, y, z, angle, radius) end,
	---default
        ---|  
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
        swimToCoord = function(handle, x, y, z) end,
	---default
        ---|  
	---@param char Char
	---@param vehicle Car
	---@param speed float
	---@param driveStyle DriveMode
	---@param modelId model_vehicle
	---@param drivingStyle DrivingMode
        drivePointRouteAdvanced = function(char, vehicle, speed, driveStyle, modelId, drivingStyle) end,
	---default
        ---| Makes a character walk to the specified point, trun to heading, then play an animation 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param radius float
	---@param animationName string
	---@param animationFile string
	---@param blendSpeed float
	---@param loop bool
	---@param lockX bool
	---@param lockY bool
	---@param keepLastFrame bool
	---@param time int
        charSlideToCoordAndPlayAnim = function(handle, x, y, z, heading, radius, animationName, animationFile, blendSpeed, loop, lockX, lockY, keepLastFrame, time) end,
	---default
        ---| Makes the character perform an animation like TASK_PLAY_ANIM, except it will not be disturbed by any events 
	---@param handle Char
	---@param animationName string
	---@param animationFile string
	---@param blendSpeed float
	---@param loop bool
	---@param lockX bool
	---@param lockY bool
	---@param keepLastFrame bool
	---@param time int
        playAnimNonInterruptable = function(handle, animationName, animationFile, blendSpeed, loop, lockX, lockY, keepLastFrame, time) end,
	---default
        ---| Assigns the character to the patrol path 
	---@param handle Char
	---@param speed MoveState
	---@param mode RouteMode
        followPatrolRoute = function(handle, speed, mode) end,
	---default
        ---| Makes a character greet another character with a handshake 
	---@param handle Char
	---@param partner Char
	---@param approachRatio float
	---@param greetStyle int
        greetPartner = function(handle, partner, approachRatio, greetStyle) end,
	---default
        ---| Makes the char perform an animation similarly to 0605 
	---@param handle Char
	---@param animationName string
	---@param animationFile string
	---@param blendSpeed float
	---@param time int
        dieNamedAnim = function(handle, animationName, animationFile, blendSpeed, time) end,
	---default
        ---| Makes one char follow another 
	---@param handle Char
	---@param target Char
        followFootsteps = function(handle, target) end,
	---default
        ---| Makes the character walk alongside the specified character 
	---@param handle Char
	---@param target Char
        walkAlongsideChar = function(handle, target) end,
	---default
        ---| Makes the character stay near their current position 
	---@param handle Char
	---@param state bool
        kindaStayInSamePlace = function(handle, state) end,
	---default
        ---| Makes the char perform an animation 
	---@param handle Char
	---@param animationName string
	---@param animationFile string
	---@param frameDelta float
	---@param loop bool
	---@param lockX bool
	---@param lockY bool
	---@param lockF bool
	---@param time int
	---@param disableForce bool
	---@param disableLockZ bool
        playAnimWithFlags = function(handle, animationName, animationFile, frameDelta, loop, lockX, lockY, lockF, time, disableForce, disableLockZ) end,
	---default
        ---|  
	---@param handle Char
	---@param radius float
	---@param modelId model_object
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param name string
        useClosestMapAttractor = function(handle, radius, modelId, fromX, fromY, fromZ, name) end,
	---default
        ---|  
	---@param handle Char
	---@param state bool
        setIgnoreWeaponRangeFlag = function(handle, state) end,
	---default
        ---|  
	---@param char Char
	---@param object Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param boneId PedBoneId
	---@param orientation HoldOrientation
	---@param animationName string
	---@param animationFile string
	---@param time int
        pickUpSecondObject = function(char, object, xOffset, yOffset, zOffset, boneId, orientation, animationName, animationFile, time) end,
	---default
        ---| Makes a character play an animation that affects only the upper half of their body 
	---@param handle Char
	---@param animationName string
	---@param animationFile string
	---@param blendSpeed float
	---@param loop bool
	---@param lockX bool
	---@param lockY bool
	---@param keepLastFrame bool
	---@param time int
        playAnimSecondary = function(handle, animationName, animationFile, blendSpeed, loop, lockX, lockY, keepLastFrame, time) end,
	---default
        ---| Makes a character face the other character and make a gesture 
	---@param handle Char
	---@param target Char
        handGesture = function(handle, target) end,
	---default
        ---| Makes the specified character run in panic to the specified position 
	---@param handle Char
	---@param x float
	---@param y float
	---@param z float
	---@param speed MoveState
	---@param time int
	---@param radius float
        followPathNodesToCoordWithRadius = function(handle, x, y, z, speed, time, radius) end,
    }
CardDecksOp = {
        ---default
        ---|  
	---@param type int
        shuffle = function(type) end,
	---default
        ---| Returns a random number between 1 and 52, inclusive 
	---@return int number
        fetchNextCard = function() end,
    }
DecisionMakerCharOp = {
        ---default
        ---| Creates a decision maker with the specified type and adds it to mission cleanup list. Otherwise should be released with REMOVE_DECISION_MAKER 
	---@param type DecisionMakerType
	---@return DecisionMakerChar handle
        load = function(type) end,
	---default
        ---| Resets the task for the event of the specified decision maker 
	---@param self DecisionMakerChar
	---@param event Event
        clearEventResponse = function(self, event) end,
	---default
        ---| Sets which action should occur according to the event on the following parameters 
	---@param self DecisionMakerChar
	---@param event Event
	---@param taskId TaskId
	---@param respect float
	---@param hate float
	---@param like float
	---@param dislike float
	---@param inCar bool
	---@param onFoot bool
        addEventResponse = function(self, event, taskId, respect, hate, like, dislike, inCar, onFoot) end,
	---default
        ---| Copies a decision makers data to another decision maker 
	---@param handleOrTemplate DecisionMakerCharTemplate
	---@return DecisionMakerChar handle
        copy = function(handleOrTemplate) end,
	---default
        ---| Creates decision maker instance based on template. Adds itself to mission cleanup list. Otherwise should be released with REMOVE_DECISION_MAKER 
	---@param template DecisionMakerCharTemplate
	---@return DecisionMakerChar handle
        copyShared = function(template) end,
    }
SequenceOp = {
        ---default
        ---| Begins a sequence of up to 8 tasks 
	---@return Sequence handle
        open = function() end,
	---default
        ---| Ends the task sequence 
	---@param self Sequence
        close = function(self) end,
	---default
        ---| Clears the task sequence 
	---@param self Sequence
        clear = function(self) end,
	---default
        ---| Sets whether the task sequence repeats continuously 
	---@param self Sequence
	---@param state bool
        setToRepeat = function(self, state) end,
    }
AttractorOp = {
        ---default
        ---| Adds a ped attractor 
	---@param x float
	---@param y float
	---@param z float
	---@param angle float
	---@param _p5 float
	---@param sequence Sequence
	---@return Attractor handle
        add = function(x, y, z, angle, _p5, sequence) end,
	---default
        ---|  
	---@param self Attractor
        clear = function(self) end,
	---default
        ---|  
	---@param self Attractor
	---@param pedType PedType
        addPedTypeAsUser = function(self, pedType) end,
    }
GroupOp = {
        ---default
        ---| Creates a new group, which multiple characters can be assigned to, allowing control over all of them as a group 
	---@param defaultTaskAllocator DefaultTaskAllocator
	---@return Group handle
        create = function(defaultTaskAllocator) end,
	---default
        ---| Puts the specified character into the group as the leader 
	---@param self Group
	---@param handle Char
        setLeader = function(self, handle) end,
	---default
        ---| Puts the specified character into the group as a member 
	---@param self Group
	---@param handle Char
        setMember = function(self, handle) end,
	---default
        ---| Releases the group 
	---@param self Group
        remove = function(self) end,
	---default
        ---| Sets the decision maker for a group of characters 
	---@param self Group
	---@param handleOrTemplate DecisionMakerGroupTemplate
        setDecisionMaker = function(self, handleOrTemplate) end,
	---default
        ---| Sets how far members of the group can be from the leader before they are removed from the group 
	---@param self Group
	---@param range float
        setSeparationRange = function(self, range) end,
	---default
        ---|  
	---@param self Group
	---@param defaultTaskAllocator DefaultTaskAllocator
        setDefaultTaskAllocator = function(self, defaultTaskAllocator) end,
	---default
        ---|  
	---@param self Group
	---@return int numLeaders
	---@return int numMembers
        getSize = function(self) end,
	---default
        ---| Returns true if the handle is a valid group handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---default
        ---| Sets the default task sequence for members of the group 
	---@param self Group
	---@param sequence Sequence
        setSequence = function(self, sequence) end,
	---default
        ---| Returns the nth group member 
	---@param self Group
	---@param slotId int
	---@return Char handle
        getMember = function(self, slotId) end,
	---default
        ---| Sets whether the group members enter a car when the leader does 
	---@param self Group
	---@param state bool
        setFollowStatus = function(self, state) end,
    }
ParticleOp = {
        ---default
        ---| Creates a particle effect 
	---@param name string
	---@param x float
	---@param y float
	---@param z float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        create = function(name, x, y, z, ignoreBoundingChecks) end,
	---default
        ---| Makes the specified particle visible 
	---@param self Particle
        play = function(self) end,
	---default
        ---| Stops the specified particle at the source 
	---@param self Particle
        stop = function(self) end,
	---default
        ---| Starts the particle effect and relinquishes script control over it 
	---@param self Particle
        playAndKill = function(self) end,
	---default
        ---| Stops the particle and deletes it 
	---@param self Particle
        kill = function(self) end,
	---default
        ---| Creates a particle attached to a character 
	---@param name string
	---@param char Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        createOnChar = function(name, char, xOffset, yOffset, zOffset, ignoreBoundingChecks) end,
	---default
        ---| Creates a particle effect attached to a character 
	---@param name string
	---@param char Char
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xDirection float
	---@param yDirection float
	---@param zDirection float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        createOnCharWithDirection = function(name, char, xOffset, yOffset, zOffset, xDirection, yDirection, zDirection, ignoreBoundingChecks) end,
	---default
        ---| Creates a particle effect attached to a vehicle 
	---@param name string
	---@param vehicle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        createOnCar = function(name, vehicle, xOffset, yOffset, zOffset, ignoreBoundingChecks) end,
	---default
        ---| Creates a particle and attaches it to the specified vehicle with the specified offset and direction 
	---@param name string
	---@param vehicle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xDirection float
	---@param yDirection float
	---@param zDirection float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        createOnCarWithDirection = function(name, vehicle, xOffset, yOffset, zOffset, xDirection, yDirection, zDirection, ignoreBoundingChecks) end,
	---default
        ---| Creates a particle effect on an object 
	---@param name string
	---@param object Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        createOnObject = function(name, object, xOffset, yOffset, zOffset, ignoreBoundingChecks) end,
	---default
        ---| Creates particle effect on an object 
	---@param name string
	---@param object Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xDirection float
	---@param yDirection float
	---@param zDirection float
	---@param ignoreBoundingChecks bool
	---@return Particle handle
        createOnObjectWithDirection = function(name, object, xOffset, yOffset, zOffset, xDirection, yDirection, zDirection, ignoreBoundingChecks) end,
	---default
        ---| Attaches the specified particle to the specified character 
	---@param self Particle
	---@param handle Char
	---@param pedBone PedBone
        attachToCharBone = function(self, handle, pedBone) end,
	---default
        ---| Destroys the specified particle 
	---@param self Particle
        killNow = function(self) end,
    }
DecisionMakerOp = {
        ---default
        ---| Removes the decision maker 
	---@param self DecisionMaker
        remove = function(self) end,
	---default
        ---| Returns true if the handle is a valid decision maker handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
    }
DecisionMakerGroupOp = {
        ---default
        ---| Creates a decision maker for use on groups of actors 
	---@param type DecisionMakerGroupType
	---@return DecisionMakerGroup handle
        load = function(type) end,
	---default
        ---| Resets the task for the event of the specified group decision maker 
	---@param self DecisionMakerGroup
	---@param event Event
        clearEventResponse = function(self, event) end,
	---default
        ---| Sets which action should occur according to the event on the following parameters 
	---@param self DecisionMakerGroup
	---@param event Event
	---@param taskId TaskId
	---@param respect float
	---@param hate float
	---@param like float
	---@param dislike float
	---@param inCar bool
	---@param onFoot bool
        addEventResponse = function(self, event, taskId, respect, hate, like, dislike, inCar, onFoot) end,
	---default
        ---| Creates copy of group decision maker and adds it to mission cleanup list. Otherwise should be released with REMOVE_DECISION_MAKER 
	---@param handleOrTemplate DecisionMakerGroupTemplate
	---@return DecisionMakerGroup handle
        copy = function(handleOrTemplate) end,
    }
SearchlightOp = {
        ---default
        ---|  
	---@param x float
	---@param y float
	---@param z float
	---@param xPoint float
	---@param yPoint float
	---@param zPoint float
	---@param radius float
	---@param radiusPoint float
	---@return Searchlight handle
        create = function(x, y, z, xPoint, yPoint, zPoint, radius, radiusPoint) end,
	---default
        ---|  
	---@param self Searchlight
        delete = function(self) end,
	---default
        ---| Returns true if the handle is a valid searchlight handle 
	---@param handle any
	---@return bool
        doesExist = function(handle) end,
	---default
        ---| Makes the specified searchlight travel between the two specified points with the specified speed 
	---@param self Searchlight
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@param speed float
        moveBetweenCoords = function(self, fromX, fromY, fromZ, toX, toY, toZ, speed) end,
	---default
        ---| Makes the searchlight target move/travel to the specified coords 
	---@param self Searchlight
	---@param x float
	---@param y float
	---@param z float
	---@param speed float
        pointAtCoord = function(self, x, y, z, speed) end,
	---default
        ---| Makes the searchlight follow the specified char 
	---@param self Searchlight
	---@param handle Char
	---@param speed float
        pointAtChar = function(self, handle, speed) end,
	---default
        ---| Returns true if the searchlight has spotted the char 
	---@param self Searchlight
	---@param handle Char
	---@return bool
        isCharIn = function(self, handle) end,
	---default
        ---|  
	---@param self Searchlight
	---@param handle Car
	---@param speed float
        pointAtVehicle = function(self, handle, speed) end,
	---default
        ---| Returns true if the searchlights light is on the vehicle 
	---@param self Searchlight
	---@param handle Car
	---@return bool
        isVehicleIn = function(self, handle) end,
	---default
        ---| Creates a searchlight-styled light cone on a car with the specified offset and points to a certain point 
	---@param vehicle Car
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
	---@param xPoint float
	---@param yPoint float
	---@param zPoint float
	---@param pointRadius float
	---@param radius float
	---@return Searchlight handle
        createOnVehicle = function(vehicle, xOffset, yOffset, zOffset, xPoint, yPoint, zPoint, pointRadius, radius) end,
	---default
        ---| Attaches the searchlight to the specified objects 
	---@param self Searchlight
	---@param spotTower Object
	---@param spotHousing Object
	---@param spotBulb Object
	---@param xOffset float
	---@param yOffset float
	---@param zOffset float
        attachToObject = function(self, spotTower, spotHousing, spotBulb, xOffset, yOffset, zOffset) end,
	---default
        ---|  
	---@param self Searchlight
	---@param state bool
        setClipIfColliding = function(self, state) end,
	---default
        ---| Sets whether the searchlight shows a shadow effect on the surface it hits 
	---@param self Searchlight
	---@param state bool
        switchOnGround = function(self, state) end,
    }
CheckpointOp = {
        ---default
        ---| Creates racing/flight style red checkpoint object 
	---@param type CheckpointType
	---@param x float
	---@param y float
	---@param z float
	---@param pointX float
	---@param pointY float
	---@param pointZ float
	---@param radius float
	---@return Checkpoint handle
        create = function(type, x, y, z, pointX, pointY, pointZ, radius) end,
	---default
        ---|  
	---@param self Checkpoint
        delete = function(self) end,
	---default
        ---|  
	---@param self Checkpoint
	---@param x float
	---@param y float
	---@param z float
        setCoords = function(self, x, y, z) end,
	---default
        ---|  
	---@param self Checkpoint
	---@param heading float
        setHeading = function(self, heading) end,
    }
TrainOp = {
        ---default
        ---| Creates a script handled train from a predefined type (the type dictates how long the train is and the varieties of carriages) and sets the direction for the train to head in 
	---@param type int
	---@param x float
	---@param y float
	---@param z float
	---@param direction bool
	---@return Train handle
        create = function(type, x, y, z, direction) end,
	---default
        ---| Sets the trains acceleration 
	---@param self Train
	---@param speed float
        setSpeed = function(self, speed) end,
	---default
        ---| Sets the trains speed 
	---@param self Train
	---@param speed float
        setCruiseSpeed = function(self, speed) end,
	---default
        ---| Gets the handle of the last carriage (known as the "caboose") 
	---@param self Train
	---@return Car caboose
        getCaboose = function(self) end,
	---default
        ---| Gets the nth train carriage 
	---@param self Train
	---@param number int
	---@return Car carriage
        getCarriage = function(self, number) end,
	---default
        ---| Removes the specified script created train 
	---@param self Train
        delete = function(self) end,
	---default
        ---| Removes the specified script created train from the list of trains that the game shouldn't delete 
	---@param self Train
        markAsNoLongerNeeded = function(self) end,
	---default
        ---| Puts the train on the rails nearest to the specified coordinates 
	---@param self Train
	---@param x float
	---@param y float
	---@param z float
        setCoordinates = function(self, x, y, z) end,
	---default
        ---| Returns true if the train has derailed (usually from going too fast) 
	---@param self Train
	---@return bool
        hasDerailed = function(self) end,
	---default
        ---| Sets whether the train should stop at each station it encounters 
	---@param self Train
	---@param state bool
        setForcedToSlowDown = function(self, state) end,
	---default
        ---| Returns true if the train is travelling clockwise, around San Andreas 
	---@param self Train
	---@return bool
        findDirection = function(self) end,
	---default
        ---| Returns true if the next station is accessible (at the start of the game, railroad blocks prevent the player from travelling to stations whose area is not unlocked) 
	---@param self Train
	---@return bool
        isNextStationAllowed = function(self) end,
	---default
        ---| Puts the script created train at the next allowed station 
	---@param self Train
        skipToNextAllowedStation = function(self) end,
    }
ConversationOp = {
        ---default
        ---| Starts a conversation between the character and the player and clears the conversation lines 
	---@param handle Char
        startSettingUp = function(handle) end,
	---default
        ---| Finalizes the current conversation sequence started with 0717. Selected answers will be subtitled 
        finishSettingUp = function() end,
	---default
        ---| Returns true if the conversation is at the specified node 
	---@param handle Char
	---@param speech gxt_key
	---@return bool
        isAtNode = function(handle, speech) end,
	---default
        ---| Returns true if there is a conversation going on between the character and the player and both the character and the player are able to communicate with one another 
	---@param handle Char
	---@return bool
        isPlayerInPosition = function(handle) end,
	---default
        ---| Pauses the scripted conversation assigned to the specified character 
	---@param handle Char
	---@param state bool
        enable = function(handle, state) end,
	---default
        ---|  
	---@param handle Char
        clearForChar = function(handle) end,
	---default
        ---| Specifies the dialogue GXT's and audio ID's 
	---@param question gxt_key
	---@param positiveAnswer gxt_key
	---@param negativeAnswer gxt_key
	---@param questionPhrase SpeechId
	---@param positiveAnswerPhrase SpeechId
	---@param negativeAnswerPhrase SpeechId
        setUpNodeWithSpeech = function(question, positiveAnswer, negativeAnswer, questionPhrase, positiveAnswerPhrase, negativeAnswerPhrase) end,
	---default
        ---| Sets the speech sound for the specified conversation response node 
	---@param text gxt_key
	---@param phrase SpeechId
        setUpEndNodeWithSpeech = function(text, phrase) end,
	---default
        ---| Adds a new line to the scripted conversation 
	---@param question gxt_key
	---@param positiveAnswer gxt_key
	---@param negativeAnswer gxt_key
	---@param questionSoundId int
	---@param positiveAnswerSoundId int
	---@param negativeAnswerSoundId int
        setUpNodeWithScriptedSpeech = function(question, positiveAnswer, negativeAnswer, questionSoundId, positiveAnswerSoundId, negativeAnswerSoundId) end,
	---default
        ---| Sets the script audio ID (see 03CF) for the specified conversation response node 
	---@param speech gxt_key
	---@param speechSoundId int
        setUpEndNodeWithScriptedSpeech = function(speech, speechSoundId) end,
	---default
        ---| Finalizes the current conversation sequence started with 0717. Selected answers will not be subtitled 
        finishSettingUpNoSubtitles = function() end,
    }
ShoppingOp = {
        ---default
        ---|  
	---@param sectionName string
        loadPrices = function(sectionName) end,
	---default
        ---|  
	---@param name string
        load = function(name) end,
	---default
        ---|  
	---@return int numItems
        getNumberOfItems = function() end,
	---default
        ---| Returns an identifier for an item associated with the shopping data entry 
	---@param nth int
	---@return int id
        getItem = function(nth) end,
	---default
        ---|  
	---@param itemId int
	---@return int price
        getPriceOfItem = function(itemId) end,
	---default
        ---|  
	---@param itemId int
	---@param flag int
	---@return int value
        getExtraInfo = function(itemId, flag) end,
	---default
        ---|  
	---@param itemId int
	---@return string name
        getNameOfItem = function(itemId) end,
	---default
        ---| Charges the player for the purchase of the item and in many cases, automatically gives the item to the player 
	---@param itemId int
        buyItem = function(itemId) end,
	---default
        ---| Returns the name of currently loaded subsection in shopping 
	---@return string name
        getLoaded = function() end,
	---default
        ---| Releases the loaded shopping data 
        clearLoaded = function() end,
	---default
        ---| Sets a new base price for the shopping.dat item 
	---@param itemId int
	---@param price int
        addPriceModifier = function(itemId, price) end,
	---default
        ---| Restores the base price for a shopping.dat item altered by ADD_PRICE_MODIFIER 
	---@param itemId int
        removePriceModifier = function(itemId) end,
	---default
        ---| Returns true if the shopping item has been bought 
	---@param itemId int
	---@return bool
        hasPlayerBoughtItem = function(itemId) end,
    }
WeaponOp = {
        ---default
        ---| Gets the model ID of the weapon according to the weapon type 
	---@param weaponType WeaponType
	---@return model_object modelId
        getModel = function(weaponType) end,
	---default
        ---|  
	---@param weaponType WeaponType
	---@return int slot
        getSlot = function(weaponType) end,
	---CLEO+
        ---| Checks if the weapon has the specified fire type 
	---@param weaponType WeaponType
	---@param weaponFire WeaponFire
	---@return bool
        isFireType = function(weaponType, weaponFire) end,
	---CLEO+
        ---| Returns a pointer to the CWeaponInfo struct of the the specified weaponType and weaponSkill 
	---@param weaponType WeaponType
	---@param weaponSkill WeaponSkill
	---@return bool
	---@return WeaponInfo handle
        getWeaponInfo = function(weaponType, weaponSkill) end,
    }
CraneOp = {
        ---default
        ---| Puts the player in the San Fierro dock crane 
        playerEnteredDockCrane = function() end,
	---default
        ---| Puts the player in the San Fierro building site crane 
        playerEnteredBuildingsiteCrane = function() end,
	---default
        ---| Removes the player from the current crane 
        playerLeftCrane = function() end,
	---default
        ---| Puts the player in the crane at the quarry near Las Venturras 
        playerEnteredQuarryCrane = function() end,
	---default
        ---| Puts the player in the crane at the building site in Las Venturras 
        playerEnteredLasVegasCrane = function() end,
	---default
        ---| Enables/disables individual crane controls 
	---@param up bool
	---@param down bool
	---@param release bool
        enableControls = function(up, down, release) end,
    }
TrailerOp = {
        ---default
        ---| Returns true if CAR A has CAR B attached to it like a trailer 
	---@param self Trailer
	---@param cab Car
	---@return bool
        isAttachedToCab = function(self, cab) end,
	---default
        ---| Detaches the trailer from the car which it is attached to 
	---@param self Trailer
	---@param cab Car
        detachFromCab = function(self, cab) end,
	---default
        ---|  
	---@param self Trailer
	---@param cab Car
        attachToCab = function(self, cab) end,
    }
StreamedScriptOp = {
        ---default
        ---| Sets the name for the script brain, enabling it to be initiated by the EXE 
	---@param id script_id
	---@param _p2 string
        registerScriptBrainForCodeUse = function(id, _p2) end,
	---default
        ---| Registers a script brain for the specified ped attractor 
	---@param id script_id
	---@param _p2 string
        registerAttractorScriptBrainForCodeUse = function(id, _p2) end,
	---default
        ---| Loads the ambient script with the specified ID from the script.img file 
	---@param id script_id
        stream = function(id) end,
	---default
        ---| Returns true if the ambient script has finished loading (08A9) 
	---@param id script_id
	---@return bool
        hasLoaded = function(id) end,
	---default
        ---| Ends the specified script brain 
	---@param id script_id
        markAsNoLongerNeeded = function(id) end,
	---default
        ---| Releases the ambient script with the specified ID, freeing game memory 
	---@param id script_id
        remove = function(id) end,
	---default
        ---| Runs the ambient script with the specified ID 
	---@param id script_id
	---@param args arguments
        startNew = function(id, args) end,
	---default
        ---| Gets the number of instances of a script 
	---@param id script_id
	---@return int numScripts
        getNumberOfInstances = function(id) end,
	---default
        ---| Makes the game start an ambient script when the player is nearby a character of the specified model 
	---@param id script_id
	---@param modelId model_char
	---@param priority int
        allocateToRandomPed = function(id, modelId, priority) end,
	---default
        ---| Makes the game start an ambient script when the player is nearby an object of the specified model 
	---@param id script_id
	---@param modelId model_object
	---@param priority int
	---@param radius float
	---@param type int
        allocateToObject = function(id, modelId, priority, radius, type) end,
    }
MenuOp = {
        ---default
        ---| Creates the specified panel on the screen with basic settings 
	---@param header gxt_key
	---@param topLeftX float
	---@param topLeftY float
	---@param width float
	---@param numColumns int
	---@param interactive bool
	---@param background bool
	---@param alignment Align
	---@return Menu handle
        create = function(header, topLeftX, topLeftY, width, numColumns, interactive, background, alignment) end,
	---default
        ---|  
	---@param self Menu
	---@param column int
	---@param alignment Align
        setColumnOrientation = function(self, column, alignment) end,
	---default
        ---| Returns the currently highlighted row in a panel 
	---@param self Menu
	---@return int row
        getItemSelected = function(self) end,
	---default
        ---| Returns the last row of a panel selected with the sprint key 
	---@param self Menu
	---@return int row
        getItemAccepted = function(self) end,
	---default
        ---|  
	---@param self Menu
	---@param row int
	---@param state bool
        activateItem = function(self, row, state) end,
	---default
        ---| Removes the specified panel from the screen 
	---@param self Menu
        delete = function(self) end,
	---default
        ---|  
	---@param self Menu
	---@param column int
	---@param title gxt_key
	---@param row0 gxt_key
	---@param row1 gxt_key
	---@param row2 gxt_key
	---@param row3 gxt_key
	---@param row4 gxt_key
	---@param row5 gxt_key
	---@param row6 gxt_key
	---@param row7 gxt_key
	---@param row8 gxt_key
	---@param row9 gxt_key
	---@param row10 gxt_key
	---@param row11 gxt_key
        setColumn = function(self, column, title, row0, row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11) end,
	---default
        ---| Sets the numbered GXT of the specified panel row 
	---@param self Menu
	---@param column int
	---@param row int
	---@param gxt gxt_key
	---@param number int
        setItemWithNumber = function(self, column, row, gxt, number) end,
	---default
        ---|  
	---@param self Menu
	---@param column int
	---@param row int
	---@param gxt gxt_key
	---@param number1 int
	---@param number2 int
        setItemWith2Numbers = function(self, column, row, gxt, number1, number2) end,
	---default
        ---|  
	---@param self Menu
	---@param row int
        setActiveItem = function(self, row) end,
	---default
        ---| Sets the width of the specified menu column 
	---@param self Menu
	---@param column int
	---@param width int
        setColumnWidth = function(self, column, width) end,
	---default
        ---|  
	---@param self Menu
	---@param vehicle Car
	---@param colorSlot int
	---@param row int
        changeCarColor = function(self, vehicle, colorSlot, row) end,
	---default
        ---| Highlights the menu item - used to indicate an owned shopping item 
	---@param self Menu
	---@param row int
	---@param state bool
        highlightItem = function(self, row, state) end,
    }
SkipOp = {
        ---default
        ---| Fades out the screen and teleports the player to the specified coordinates and angle 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
        setUp = function(x, y, z, heading) end,
	---default
        ---|  
        clear = function() end,
	---default
        ---| Fades the screen out and teleports the player to the specified coordinates and angle 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
        setUpAfterMission = function(x, y, z, heading) end,
	---default
        ---| Teleports the player to the specified coordinates and sets the specified angle when in the specified car 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param handle Car
        setUpForSpecificVehicle = function(x, y, z, heading, handle) end,
	---default
        ---| Teleports the player to the specified coordinates and sets the specified angle with the screen fading in when in the specified car 
	---@param x float
	---@param y float
	---@param z float
	---@param heading float
	---@param vehicle Car
        setUpForVehicleFinishedByScript = function(x, y, z, heading, vehicle) end,
	---default
        ---| Returns true if the trip skip created with 0A35 has finished teleporting the vehicle and is ready to allow the script to fade in 
	---@return bool
        isWaitingForScriptToFadeIn = function() end,
    }
MenuGridOp = {
        ---default
        ---| Creates the same color chart that you see in car modification shops 
	---@param header gxt_key
	---@param topLeftX float
	---@param topLeftY float
	---@param width float
	---@param numColumns int
	---@param interactive bool
	---@param background bool
	---@param alignment Align
	---@return MenuGrid handle
        create = function(header, topLeftX, topLeftY, width, numColumns, interactive, background, alignment) end,
    }
User3DMarkerOp = {
        ---default
        ---| Creates a marker similar to the yellow enex markers 
	---@param x float
	---@param y float
	---@param z float
	---@param color HudColors
	---@return User3DMarker handle
        create = function(x, y, z, color) end,
	---default
        ---| Destroys a marker created with 0A40 
	---@param self User3DMarker
        remove = function(self) end,
    }
MouseOp = {
        ---default
        ---| Gives the offset of the mouse or right thumbstick movement 
	---@return float deltaX
	---@return float deltaY
        getMovement = function() end,
	---default
        ---| Returns true if the players settings are set to invert the mouse 
	---@return bool
        isUsingVerticalInversion = function() end,
	---CLEO+
        ---| Returns true if the mouse wheel has been scrolled up 
	---@return bool
        isWheelUp = function() end,
	---CLEO+
        ---| Returns true if the mouse wheel has been scrolled down 
	---@return bool
        isWheelDown = function() end,
	---CLEO+
        ---| Gets the mouse sensibility set in the game options 
	---@return float sensibility
        getSensibility = function() end,
    }
MemoryOp = {
        ---CLEO
        ---| Writes the value at the memory address 
	---@param address int
	---@param size int
	---@param value any
	---@param vp bool
        write = function(address, size, value, vp) end,
	---CLEO
        ---| Reads a value from the game memory 
	---@param address int
	---@param size int
	---@param vp bool
	---@return any result
        read = function(address, size, vp) end,
	---CLEO
        ---| Gets the address of the ped struct in the game memory by its handle 
	---@param char Char
	---@return int address
        getPedPointer = function(char) end,
	---CLEO
        ---| Gets the address of the vehicle struct in the game memory by its handle 
	---@param handle Car
	---@return int address
        getVehiclePointer = function(handle) end,
	---CLEO
        ---| Gets the address of the object struct in the game memory by its handle 
	---@param object Object
	---@return int address
        getObjectPointer = function(object) end,
	---CLEO
        ---| Gets the address of the current script structure in the game memory 
	---@return int address
        getThisScriptStruct = function() end,
	---CLEO
        ---| Calls a function at the address with the given arguments and the calling convention defined by the pop parameter where 0 means 'stdcall' and a value equal to numParams means  'cdecl' 
	---@param address int
	---@param numArgs int
	---@param pop int
	---@param args arguments
        callFunction = function(address, numArgs, pop, args) end,
	---CLEO
        ---| Calls a method of the object (struct) with the given arguments and the 'thiscall' calling convention (pop is always 0) 
	---@param address int
	---@param struct int
	---@param numArgs int
	---@param pop int
	---@param args arguments
        callMethod = function(address, struct, numArgs, pop, args) end,
	---CLEO
        ---| Calls a function similarly to 0AA5 and writes the result into the variable following the arguments list 
	---@param address int
	---@param numArgs int
	---@param pop int
	---@param args arguments
	---@return any funcRet
        callFunctionReturn = function(address, numArgs, pop, args) end,
	---CLEO
        ---| Calls a method of the object (struct) similarly to 0AA6 and writes the result into the variable following the arguments list 
	---@param address int
	---@param struct int
	---@param numArgs int
	---@param pop int
	---@param args arguments
	---@return any funcRet
        callMethodReturn = function(address, struct, numArgs, pop, args) end,
	---CLEO
        ---| Gets the address of a running script which name matches the given string or 0 otherwise 
	---@param scriptName string
	---@return int address
        getScriptStructNamed = function(scriptName) end,
	---CLEO
        ---| Stores the absolute address of a code location marked with the label 
	---@param label label
	---@return int address
        getLabelPointer = function(label) end,
	---CLEO
        ---| Stores the absolute address of the variable 
	---@param any any
	---@return int address
        getVarPointer = function(any) end,
	---CLEO
        ---| Allocates a chunk of memory of the given size and stores its address to the variable 
	---@param size int
	---@return bool
	---@return int address
        allocate = function(size) end,
	---CLEO
        ---| Frees the memory allocated with 0AC8 
	---@param address int
        free = function(address) end,
	---CLEO
        ---| Returns a floating-point number stored as the result of the function called (0AA5, 0AA6, 0AA7, 0AA8) immediately before this command 
	---@return float number
        popFloat = function() end,
	---CLEO
        ---| Gets the corresponding handle of the char located at the given address in memory 
	---@param address int
	---@return Char handle
        getPedRef = function(address) end,
	---CLEO
        ---| Gets the corresponding handle of the vehicle located at the given address in memory 
	---@param address int
	---@return Car handle
        getVehicleRef = function(address) end,
	---CLEO
        ---| Gets the corresponding handle of the object located at the given address in memory 
	---@param address int
	---@return Object handle
        getObjectRef = function(address) end,
	---CLEO+
        ---| Copies each memory byte from src address to dest address 
	---@param src int
	---@param dest int
	---@param size int
        copy = function(src, dest, size) end,
	---CLEO+
        ---| Writes the dword value to the struct address by index (index*4+address) see: 0E28 
	---@param address int
	---@param index int
	---@param value int
        writeStructParam = function(address, index, value) end,
	---CLEO+
        ---| Reads the dword value from the struct address by index (index*4+address) see: 0D4E 
	---@param address int
	---@param index int
	---@return int value
        readStructParam = function(address, index) end,
	---CLEO+
        ---| Reads a value from the given offset from the memory address (see: 0D38) 
	---@param address int
	---@param offset int
	---@param size int
	---@return any result
        readStructOffset = function(address, offset, size) end,
	---CLEO+
        ---| Writes the value at the given offset from the memory address (see: 0D37) 
	---@param address int
	---@param offset int
	---@param size int
	---@param value int
        writeStructOffset = function(address, offset, size, value) end,
	---CLEO+
        ---| Fills memory address with 0x90 with given size 
	---@param address int
	---@param size int
        makeNop = function(address, size) end,
	---CLEO+
        ---| Gets the address of the last created custom script 
	---@return bool
	---@return int address
        getLastCreatedCustomScript = function() end,
	---CLEO+
        ---| Reads a value from the given offset from the memory address multiple times as array/vector 
	---@param address int
	---@param offset int
	---@param count int
	---@param size int
	---@return arguments results
        readStructOffsetMulti = function(address, offset, count, size) end,
	---CLEO+
        ---| Writes the value at the given offset from the memory address multiple times as array/vector 
	---@param address int
	---@param offset int
	---@param count int
	---@param size int
	---@param params arguments
        writeStructOffsetMulti = function(address, offset, count, size, params) end,
	---memory
        ---| Copies a block of memory from src address to dest address. src and dest regions may overlap (memmove behavior) 
	---@param src int
	---@param dest int
	---@param size int
        copy = function(src, dest, size) end,
	---memory
        ---| Reads a value from the given offset from the memory address (see: 2402) 
	---@param address int
	---@param offset int
	---@param size int
	---@return int result
        readWithOffset = function(address, offset, size) end,
	---memory
        ---| Writes the value at the given offset from the memory address (see: 2401) 
	---@param address int
	---@param offset int
	---@param size int
	---@param value any
        writeWithOffset = function(address, offset, size, value) end,
	---memory
        ---| Makes the memory chunk allocated with 0AC8 persistent across load/start new game events. The memory is still released when game closes causing no leaks 
	---@param address int
        forget = function(address) end,
	---memory
        ---| Returns the address of a most recently (globally) created script (004F, 00D7, 0A92, 0A94) 
	---@return int address
        getScriptStructJustCreated = function() end,
	---memory
        ---| Checks if address points at valid and running script 
	---@param address int
	---@return bool
        isScriptRunning = function(address) end,
	---memory
        ---| Compares two memory blocks 
	---@param addressA int
	---@param addressB int
	---@param size int
	---@return bool
        isEqual = function(addressA, addressB, size) end,
    }
FsOp = {
        ---CLEO
        ---| Sets the current working directory (cwd) to a predefined location with a value of 0 or 1, or to an arbitrary path with a string value 
	---@param path any
        setCurrentDirectory = function(path) end,
	---CLEO
        ---| Returns true if a file at the given path exists 
	---@param path string
	---@return bool
        doesFileExist = function(path) end,
	---CLEO
        ---| Returns true if a directory at the given path exists 
	---@param path string
	---@return bool
        doesDirectoryExist = function(path) end,
	---CLEO
        ---| Creates a directory at the given path 
	---@param path string
	---@return bool
        createDirectory = function(path) end,
	---file
        ---| Deletes a file at the given path and returns true if the operation is successful 
	---@param path string
	---@return bool
        deleteFile = function(path) end,
	---file
        ---| Deletes a directory at the given path and returns true if the operation is successful 
	---@param path string
	---@param recursive bool
	---@return bool
        deleteDirectory = function(path, recursive) end,
	---file
        ---| Moves an existing file and returns true if the operation is successful 
	---@param fileName string
	---@param newFileName string
	---@return bool
        moveFile = function(fileName, newFileName) end,
	---file
        ---| Moves an existing directory and returns true if the operation is successful 
	---@param dirPath string
	---@param newDirPath string
	---@return bool
        moveDirectory = function(dirPath, newDirPath) end,
	---file
        ---| Copies an existing file to a new file and returns true if the operation is successful 
	---@param fileName string
	---@param newFileName string
	---@return bool
        copyFile = function(fileName, newFileName) end,
	---file
        ---| Copies an existing directory to a new directory and returns true if the operation is successful 
	---@param dirPath string
	---@param newDirPath string
	---@return bool
        copyDirectory = function(dirPath, newDirPath) end,
	---file
        ---| Resolves absolute filepath. Input path can be relative, starts with 'virtual path' prefix or be already absolute filepath 
	---@param path string
	---@return string resolved
        resolvePath = function(path) end,
	---file
        ---| Returns a file name or full file path of a script at the address (0A9F, 0AAA, 2404). If the address is -1 then this script is used. If no script with a given pointer is found, then the condition result is set to false and output argument is not modified 
	---@param address int
	---@param fullPath bool
	---@return bool
	---@return string filenameOrPath
        getScriptFilename = function(address, fullPath) end,
	---file
        ---| Gets last modification time of the file. On fail condition result is set to false, output parameters remain unchanged 
	---@param fileName string
	---@return bool
	---@return int year
	---@return int month
	---@return int day
	---@return int hour
	---@return int minute
	---@return int second
	---@return int milisecond
        getFileWriteTime = function(fileName) end,
    }
FileOp = {
        ---CLEO
        ---| Opens the file in the specified mode, sets the condition result to True if the open operation has been successful, or to False otherwise, and writes the file handle to the variable 
	---@param filePathName string
	---@param mode FileMode
	---@return bool
	---@return File handle
        open = function(filePathName, mode) end,
	---CLEO
        ---| Closes the file and frees the memory 
	---@param self File
        close = function(self) end,
	---CLEO
        ---| Gets the file size in bytes 
	---@param self File
	---@return int size
        getSize = function(self) end,
	---CLEO
        ---| Reads the specified number of bytes from the opened file and writes them to the memory region starting from the address of the destination variable 
	---@param self File
	---@param size int
	---@return int destination
        read = function(self, size) end,
	---CLEO
        ---| Copies the specified number of bytes of the memory region starting from the address of the source variable to the file 
	---@param self File
	---@param size int
	---@param source int
        write = function(self, size, source) end,
	---CLEO
        ---| Sets the position of the file to the given offset from the origin 
	---@param self File
	---@param offset int
	---@param origin SeekOrigin
	---@return bool
        seek = function(self, offset, origin) end,
	---CLEO
        ---| Returns true if all data has been read or any file error occurred 
	---@param self File
	---@return bool
        isEndReached = function(self) end,
	---CLEO
        ---| Reads up to maxLength-1 text characters from the file until the newline or the end-of-file is reached. Result will be null-terminated string 
	---@param self File
	---@param storeTo string
	---@param maxLength int
	---@return bool
        readString = function(self, storeTo, maxLength) end,
	---CLEO
        ---| Copies data from the source string to the file up to but not including the null character 
	---@param self File
	---@param source string
	---@return bool
        writeString = function(self, source) end,
	---CLEO
        ---| Writes a formatted string to the file 
	---@param self File
	---@param format string
	---@param args arguments
        writeFormattedString = function(self, format, args) end,
	---CLEO
        ---| Extracts data from a file using fscanf 
	---@param self File
	---@param format string
	---@return bool
	---@return int nValues
	---@return arguments values
        scan = function(self, format) end,
	---file
        ---| Gets current offset of read-write carret within the file stream 
	---@param self File
	---@return int position
        getPosition = function(self) end,
	---file
        ---| Reads data from file into buffer at specified address 
	---@param self File
	---@param size int
	---@param address int
	---@return bool
        readBlock = function(self, size, address) end,
	---file
        ---| Writes data from memory address into file 
	---@param self File
	---@param size int
	---@param address int
	---@return bool
        writeBlock = function(self, size, address) end,
    }
DynamicLibraryOp = {
        ---CLEO
        ---| Loads the specified module (usually a dynamic-link library (DLL)) into the address space of the game 
	---@param fileName string
	---@return bool
	---@return DynamicLibrary handle
        load = function(fileName) end,
	---CLEO
        ---| Frees the loaded dynamic-link library (DLL) module and unloads it from the address space of the game 
	---@param self DynamicLibrary
        free = function(self) end,
	---CLEO
        ---| Retrieves the address of an exported function or variable from the specified dynamic-link library (DLL) 
	---@param procName string
	---@param self DynamicLibrary
	---@return bool
	---@return int address
        getProcedure = function(procName, self) end,
	---CLEO+
        ---| Returns the address of a loaded dynamic-link library (DLL) 
	---@param fileName string
	---@return bool
	---@return DynamicLibrary handle
        getLoadedLibrary = function(fileName) end,
    }
FindFileOp = {
        ---CLEO
        ---| Searches a directory for a file or subdirectory with a name that matches a specific name (or partial name if wildcards are used) 
	---@param searchMask string
	---@return bool
	---@return FindFile handle
	---@return string fileName
        first = function(searchMask) end,
	---CLEO
        ---| Continues a file search from a previous call to 0AE6 
	---@param self FindFile
	---@return bool
	---@return string fileName
        next = function(self) end,
	---CLEO
        ---| Closes a file search handle opened by 0AE6 
	---@param self FindFile
        close = function(self) end,
    }
ClipboardOp = {
        ---CLEO+
        ---| Copies the specified number of bytes of text from the clipboard to the address 
	---@param address int
	---@param number int
        readData = function(address, number) end,
	---CLEO+
        ---|  
	---@param address int
	---@param number int
        writeData = function(address, number) end,
	---clipboard
        ---| Copies the specified number of bytes of text from the clipboard to the address 
	---@param address int
	---@param number int
        readData = function(address, number) end,
	---clipboard
        ---| Copies  the specified number of bytes of text from the address to the clipboard 
	---@param address int
	---@param number int
        writeData = function(address, number) end,
    }
ColPointOp = {
        ---CLEO+
        ---| Returns the colPoint, coordinates, and collision entity between two points 
	---@param fromX float
	---@param fromY float
	---@param fromZ float
	---@param toX float
	---@param toY float
	---@param toZ float
	---@param buildings bool
	---@param vehicles bool
	---@param peds bool
	---@param objects bool
	---@param dummies bool
	---@param seeThroughCheck bool
	---@param cameraIgnoreCheck bool
	---@param shotThroughCheck bool
	---@param entityToIgnore int
	---@return bool
	---@return ColPoint handle
	---@return float outX
	---@return float outY
	---@return float outZ
	---@return int entity
        getCollisionBetweenPoints = function(fromX, fromY, fromZ, toX, toY, toZ, buildings, vehicles, peds, objects, dummies, seeThroughCheck, cameraIgnoreCheck, shotThroughCheck, entityToIgnore) end,
	---CLEO+
        ---| Returns the vector of the specified ColPoint 
	---@param self ColPoint
	---@return float x
	---@return float y
	---@return float z
        getNormalVector = function(self) end,
	---CLEO+
        ---| Returns the surfaceType of the specified ColPoint 
	---@param self ColPoint
	---@return SurfaceType surfaceType
        getSurface = function(self) end,
	---CLEO+
        ---| Returns the depth of the specified ColPoint 
	---@param self ColPoint
	---@return float depth
        getDepth = function(self) end,
	---CLEO+
        ---| Returns the lighting of the specified ColPoint 
	---@param self ColPoint
	---@param fromNight bool
	---@return int lighting
        getLighting = function(self, fromNight) end,
	---CLEO+
        ---| Returns the coordinates of the specified ColPoint 
	---@param self ColPoint
	---@return float x
	---@return float y
	---@return float z
        getCoordinates = function(self) end,
    }
EntityOp = {
        ---CLEO+
        ---| Gets the type of the entity 
	---@param entity any
	---@return EntityType type
        getType = function(entity) end,
	---CLEO+
        ---| Returns True if entityB is within the radius of entityA 
	---@param entityA int
	---@param entityB int
	---@param radius float
	---@return bool
        locateDistanceToEntity = function(entityA, entityB, radius) end,
	---CLEO+
        ---| Returns the coordinates of the entity at the specified address 
	---@param address int
	---@return float x
	---@return float y
	---@return float z
        getCoordinates = function(address) end,
	---CLEO+
        ---| Returns the heading of the entity at the specified address 
	---@param address int
	---@return float heading
        getHeading = function(address) end,
    }
CleoBlipOp = {
        ---CLEO+
        ---| Creates a blip that don't saves, supports any texture, alpha and don't hits the blip limit 
	---@param rwTextureOrRadarSprite any
	---@param x float
	---@param y float
	---@param short bool
	---@param red int
	---@param green int
	---@param blue int
	---@param alpha int
	---@return CleoBlip handle
        add = function(rwTextureOrRadarSprite, x, y, short, red, green, blue, alpha) end,
	---CLEO+
        ---| Removes a cleo blip 
	---@param self CleoBlip
        remove = function(self) end,
    }
RenderObjectOp = {
        ---CLEO+
        ---| Creates renderObject to character bone 
	---@param char Char
	---@param modelId model_object
	---@param pedBone PedBone
	---@param x float
	---@param y float
	---@param z float
	---@param rx float
	---@param ry float
	---@param rz float
	---@return RenderObject handle
        createToCharBone = function(char, modelId, pedBone, x, y, z, rx, ry, rz) end,
	---CLEO+
        ---| Deletes the renderObject 
	---@param self RenderObject
        delete = function(self) end,
	---CLEO+
        ---| Sets the visible status of the renderObject 
	---@param self RenderObject
	---@param visible bool
        setVisible = function(self, visible) end,
	---CLEO+
        ---| Sets the position of a renderObject 
	---@param self RenderObject
	---@param x float
	---@param y float
	---@param z float
        setPosition = function(self, x, y, z) end,
	---CLEO+
        ---| Sets the rotation of the renderObject 
	---@param self RenderObject
	---@param x float
	---@param y float
	---@param z float
        setRotation = function(self, x, y, z) end,
	---CLEO+
        ---| Sets the scale of the renderObject 
	---@param self RenderObject
	---@param x float
	---@param y float
	---@param z float
        setScale = function(self, x, y, z) end,
	---CLEO+
        ---| Sets the renderObject's distortion 
	---@param self RenderObject
	---@param x float
	---@param y float
	---@param z float
	---@param w float
        setDistortion = function(self, x, y, z, w) end,
    }
AudioStreamOp = {
        ---CLEO+
        ---| Returns the internal reference of a given AUDIOSTREAM to be used in bass.dll functions 
	---@param self AudioStream
	---@return int address
        getInternal = function(self) end,
	---audio
        ---| Loads an audio file and creates a new audio stream (see 0AC1) 
	---@param audioFileName string
	---@return bool
	---@return AudioStream handle
        load = function(audioFileName) end,
	---audio
        ---| Sets the state of the audio stream 
	---@param self AudioStream
	---@param action AudioStreamAction
        setState = function(self, action) end,
	---audio
        ---| Unloads the audio stream and frees the memory 
	---@param self AudioStream
        remove = function(self) end,
	---audio
        ---| Gets the audio stream length in seconds 
	---@param self AudioStream
	---@return int length
        getLength = function(self) end,
	---audio
        ---| Returns the state of the audio stream 
	---@param self AudioStream
	---@return AudioStreamState state
        getState = function(self) end,
	---audio
        ---| Returns the audio stream volume (from 0.0 to 1.0) 
	---@param self AudioStream
	---@return float volume
        getVolume = function(self) end,
	---audio
        ---| Sets the audio stream volume (default is 1.0). Pauses/Starts stream playback if neccesarry 
	---@param self AudioStream
	---@param volume float
        setVolume = function(self, volume) end,
	---audio
        ---| Makes the audio stream repeat endlessly 
	---@param self AudioStream
	---@param state bool
        setLooped = function(self, state) end,
	---audio
        ---| Checks if audio stream is currently in 'play' state 
	---@param self AudioStream
	---@return bool
        isPlaying = function(self) end,
	---audio
        ---| Gets the audio stream total duration considering its speed (see 2505) 
	---@param self AudioStream
	---@return float seconds
        getDuration = function(self) end,
	---audio
        ---| Gets audio stream playback speed multiplier 
	---@param self AudioStream
	---@return float speed
        getSpeed = function(self) end,
	---audio
        ---| Sets audio stream playback speed multiplier 
	---@param self AudioStream
	---@param speed float
        setSpeed = function(self, speed) end,
	---audio
        ---| Changes stream volume with smooth transition 
	---@param self AudioStream
	---@param volume float
	---@param timeMs int
        setVolumeWithTransition = function(self, volume, timeMs) end,
	---audio
        ---| Changes stream speed with smooth transition. Pauses/Starts stream playback if neccesarry 
	---@param self AudioStream
	---@param speed float
	---@param timeMs int
        setSpeedWithTransition = function(self, speed, timeMs) end,
	---audio
        ---| Gets audio stream playback progress as value from 0.0 to 1.0 
	---@param self AudioStream
	---@return float progress
        getProgress = function(self) end,
	---audio
        ---| Sets audio stream playback position. Progress range is from 0.0 to 1.0 
	---@param self AudioStream
	---@param progress float
        setProgress = function(self, progress) end,
	---audio
        ---| Returns current volume settings type group of the stream 
	---@param self AudioStream
	---@return AudioStreamType type
        getType = function(self) end,
	---audio
        ---| Sets which game's master volume settings the stream should follow 
	---@param self AudioStream
	---@param type AudioStreamType
        setType = function(self, type) end,
	---audio
        ---| Returns current playback progress in seconds. Does not take in consideration current stream's playback speed 
	---@param self AudioStream
	---@return float progress
        getProgressSeconds = function(self) end,
	---audio
        ---| Sets current playback progress in seconds. Does not take in consideration current stream's playback speed 
	---@param self AudioStream
	---@param progress float
        setProgressSeconds = function(self, progress) end,
    }
ListOp = {
        ---CLEO+
        ---| Creates a list of the specified type 
	---@param type ListType
	---@return List handle
        create = function(type) end,
	---CLEO+
        ---| Deletes the specified list 
	---@param self List
        delete = function(self) end,
	---CLEO+
        ---| Adds a numerical value to the specified list 
	---@param self List
	---@param value any
        add = function(self, value) end,
	---CLEO+
        ---| Removes a numerical value from the specified list 
	---@param self List
	---@param value any
        removeValue = function(self, value) end,
	---CLEO+
        ---| Removes an entry from the specified list by index 
	---@param self List
	---@param index int
        removeIndex = function(self, index) end,
	---CLEO+
        ---| Returns the number of entries in the specified list 
	---@param self List
	---@return int entries
        getSize = function(self) end,
	---CLEO+
        ---| Returns a numerical value from the specified list by index 
	---@param self List
	---@param index int
	---@return any value
        getValueByIndex = function(self, index) end,
	---CLEO+
        ---| Resets all entries within the specified list 
	---@param self List
        reset = function(self) end,
	---CLEO+
        ---| Returns a string value from the specified list by index 
	---@param self List
	---@param index int
	---@return string string
        getStringValueByIndex = function(self, index) end,
	---CLEO+
        ---| Adds a string value to the specified list 
	---@param self List
	---@param string string
        addString = function(self, string) end,
	---CLEO+
        ---| Removes a string value from the specified list 
	---@param self List
	---@param string string
        removeStringValue = function(self, string) end,
	---CLEO+
        ---| Removes entries from the specified list by index range 
	---@param self List
	---@param start int
	---@param stop int
        removeIndexRange = function(self, start, stop) end,
	---CLEO+
        ---| Reverses the order of entries within the specified list 
	---@param self List
        reverse = function(self) end,
	---CLEO+
        ---| Replaces a value on a list by index 
	---@param self List
	---@param index int
	---@param value any
        replaceValueByIndex = function(self, index, value) end,
	---CLEO+
        ---| Replaces a string value on a list by index 
	---@param self List
	---@param index int
	---@param string string
        replaceStringValueByIndex = function(self, index, string) end,
	---CLEO+
        ---| Inserts a value into a list by index 
	---@param self List
	---@param index int
	---@param value any
        insertValueByIndex = function(self, index, value) end,
	---CLEO+
        ---| Inserts a string value into a list by index 
	---@param self List
	---@param index int
	---@param string string
        insertStringValueByIndex = function(self, index, string) end,
    }
WeaponInfoOp = {
        ---CLEO+
        ---| Returns the model1 and model2 of the current weaponInfo struct 
	---@param self WeaponInfo
	---@return model_any model1
	---@return model_any model2
        getModels = function(self) end,
	---CLEO+
        ---| Returns the flags of the specified WeaponInfo struct 
	---@param self WeaponInfo
	---@return int flags
        getFlags = function(self) end,
	---CLEO+
        ---| Returns the FireType of the current weaponInfo struct 
	---@param self WeaponInfo
	---@return AnimGrp animGroup
        getAnimgroup = function(self) end,
	---CLEO+
        ---| Returns the totalClip size for the current weaponInfo struct 
	---@param self WeaponInfo
	---@return int totalClip
        getTotalClip = function(self) end,
	---CLEO+
        ---| Returns the FireType of the current weaponInfo struct 
	---@param self WeaponInfo
	---@return WeaponFire fireType
        getFireType = function(self) end,
	---CLEO+
        ---| Returns the weaponSlot of the current weaponInfo struct 
	---@param self WeaponInfo
	---@return WeaponSlot weaponSlot
        getSlot = function(self) end,
    }
MatrixOp = {
        ---NewOpcodes
        ---| Multiplies matrices 
	---@param matrixA Matrix
	---@param matrixB Matrix
	---@return Matrix handle
        multiply = function(matrixA, matrixB) end,
	---NewOpcodes
        ---| Rotates matrix on axis 
	---@param self Matrix
	---@param axisX float
	---@param axisY float
	---@param axisZ float
	---@param angle float
	---@param combineOp RwCombine
        rotateOnAxis = function(self, axisX, axisY, axisZ, angle, combineOp) end,
	---NewOpcodes
        ---| Gets the X angle of matrix (in degrees) 
	---@param self Matrix
	---@return float xAngle
        getXAngle = function(self) end,
	---NewOpcodes
        ---| Gets the Y angle of matrix (in degrees) 
	---@param self Matrix
	---@return float yAngle
        getYAngle = function(self) end,
	---NewOpcodes
        ---| Gets the Z angle of matrix (in degrees) 
	---@param self Matrix
	---@return float zAngle
        getZAngle = function(self) end,
	---NewOpcodes
        ---| Sets position for matrix 
	---@param self Matrix
	---@param x float
	---@param y float
	---@param z float
        setPosition = function(self, x, y, z) end,
	---NewOpcodes
        ---| Sets matrix angles 
	---@param self Matrix
	---@param x float
	---@param y float
	---@param z float
        rotate = function(self, x, y, z) end,
	---NewOpcodes
        ---| Copies matrix to another matrix 
	---@param self Matrix
	---@param destination Matrix
        copy = function(self, destination) end,
	---NewOpcodes
        ---| Gets coords on offset from matrix 
	---@param self Matrix
	---@param x float
	---@param y float
	---@param z float
	---@return float x
	---@return float y
	---@return float z
        getOffset = function(self, x, y, z) end,
    }
ComponentOp = {
        ---NewOpcodes
        ---| Gets component child component 
	---@param self Component
	---@return Component child
        getChildComponent = function(self) end,
	---NewOpcodes
        ---| Gets component next component 
	---@param self Component
	---@return Component nextComponent
        getNextComponent = function(self) end,
	---NewOpcodes
        ---| Gets component name component 
	---@param self Component
	---@return string name
        getName = function(self) end,
	---NewOpcodes
        ---| Gets component world matrix 
	---@param self Component
	---@return any worldMatrix
        getWorldMatrix = function(self) end,
	---NewOpcodes
        ---| Gets component modelling matrix 
	---@param self Component
	---@return any modellingMatrix
        getModellingMatrix = function(self) end,
	---NewOpcodes
        ---| Gets component parent component 
	---@param self Component
	---@return Component parentComponent
        getParentComponent = function(self) end,
	---NewOpcodes
        ---| Gets component number of objects 
	---@param self Component
	---@return int numObjects
        getNumObjects = function(self) end,
    }
TextureOp = {
        ---NewOpcodes
        ---| Loads texture from BMP file. Can be used with texture memory option to load file from memory 
	---@param bmp string
	---@param mask string
	---@return bool
	---@return any texture
        loadFromBmpFile = function(bmp, mask) end,
	---NewOpcodes
        ---| Loads texture from PNG file. Can be used with texturememory option to load file from memory 
	---@param png string
	---@return bool
	---@return any texture
        loadFromPngFile = function(png) end,
	---NewOpcodes
        ---| Loads texture from DDS file 
	---@param dds string
	---@return bool
	---@return any texture
        loadFromDdsFile = function(dds) end,
	---NewOpcodes
        ---| Cleans loaded texture 
	---@param self Texture
        cleanLoaded = function(self) end,
    }
SpriteOp = {
        ---NewOpcodes
        ---| Draws 2D sprite 
	---@param texture int
	---@param cornerAx float
	---@param cornerAy float
	---@param cornerBx float
	---@param cornerBy float
	---@param red int
	---@param blue int
	---@param green int
	---@param aplha int
	---@param angle float
        draw2D = function(texture, cornerAx, cornerAy, cornerBx, cornerBy, red, blue, green, aplha, angle) end,
	---NewOpcodes
        ---| Draws 2D sprite with gradient 
	---@param texture int
	---@param cornerAx float
	---@param cornerAy float
	---@param cornerBx float
	---@param cornerBy float
	---@param red0 int
	---@param green0 int
	---@param blue0 int
	---@param alpha0 int
	---@param red1 int
	---@param green1 int
	---@param blue1 int
	---@param alpha1 int
	---@param red2 int
	---@param green2 int
	---@param blue2 int
	---@param alpha2 int
	---@param red3 int
	---@param green3 int
	---@param blue3 int
	---@param alpha3 int
	---@param angle float
        draw2DWithGradient = function(texture, cornerAx, cornerAy, cornerBx, cornerBy, red0, green0, blue0, alpha0, red1, green1, blue1, alpha1, red2, green2, blue2, alpha2, red3, green3, blue3, alpha3, angle) end,
    }
SampMyPlayerOp = {
        ---SAMPFUNCS
        ---| Sends SampRpc.Spawn to SAMP Server. Teleports our character to spawn as well 
        forceSpawn = function() end,
	---SAMPFUNCS
        ---| Changes our nickname (visually) 
	---@param nickname string
        setNickname = function(nickname) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if our player has been spawned already 
	---@return bool
        isSpawned = function() end,
    }
SampClientOp = {
        ---SAMPFUNCS
        ---| Returns a pointer to the memory address of samp.dll 
	---@return int pointer
        getBaseAddress = function() end,
	---SAMPFUNCS
        ---| Evaluates as logical true if SAMP structures was initialized. Used when checking if gta sa is running on SAMP or in Single Player 
	---@return bool
        isAvailable = function() end,
	---SAMPFUNCS
        ---| Sets the special action for our Player 
	---@param id SampSpecialAction
        setSpecialAction = function(id) end,
	---SAMPFUNCS
        ---| Returns the vehicle handle using its samp vehicle id. Returns 0 If the car is not in the stream zone 
	---@param id int
	---@return Car handle
        getCar = function(id) end,
	---SAMPFUNCS
        ---| Sets the periodic delay (in milliseconds) of sending specific data type to Server 
	---@param type SampSendRateType
	---@param delay int
        setSendRate = function(type, delay) end,
	---SAMPFUNCS
        ---| Sets the connection status to the server. Useful when attempting to reconnect or disconnect to server 
	---@param id SampGameState
        setGameState = function(id) end,
	---SAMPFUNCS
        ---| Writes the value with size to samp.dll+offset 
	---@param offset int
	---@param value any
	---@param size int
        writeMemoryWithOffset = function(offset, value, size) end,
	---SAMPFUNCS
        ---| Reads the value with size from samp.dll+offset 
	---@param offset int
	---@param size int
	---@return any value
        readMemoryWithOffset = function(offset, size) end,
	---SAMPFUNCS
        ---| Returns the server's port and stores server's IP address to buffer 
	---@return string buffer
	---@return int port
        getServerAddress = function() end,
	---SAMPFUNCS
        ---| Stores the server name to buffer 
	---@return string buffer
        getServerName = function() end,
	---SAMPFUNCS
        ---| Shows an artificial SAMP dialog with specified parameter attributes 
	---@param id int
	---@param title string
	---@param content string
	---@param leftOrMiddleButtonName string
	---@param rightButtonName string
	---@param style SampDialogStyle
        showDialog = function(id, title, content, leftOrMiddleButtonName, rightButtonName, style) end,
	---SAMPFUNCS
        ---| Returns logical true if the last submitted SAMP Dialog is equal to the specified dialogid 
	---@param dialogId int
	---@return bool
	---@return int buttonId
	---@return int listItemId
	---@return int inputTextBuffer
        hasDialogResponded = function(dialogId) end,
	---SAMPFUNCS
        ---| Closes the active dialog by pressing the specified button programmatically 
	---@param button SampDialogButton
	---@return bool
        closeDialog = function(button) end,
	---SAMPFUNCS
        ---| Returns the ID of the currently selected item on the list 
	---@return bool
	---@return int itemId
        getDialogSelectedItem = function() end,
	---SAMPFUNCS
        ---| Selects the id of an element in the dialog list 
	---@param itemId int
	---@return bool
        selectDialogItem = function(itemId) end,
	---SAMPFUNCS
        ---| Stores the text from the input field of the active dialog to the buffer 
	---@return bool
	---@return string buffer
        getDialogEditboxText = function() end,
	---SAMPFUNCS
        ---| Sets the text of the edit field of the active dialog's box 
	---@param text string
	---@return bool
        setDialogEditboxText = function(text) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified dialog with id is currently visible 
	---@param id int
	---@return bool
        isDialogActive = function(id) end,
	---SAMPFUNCS
        ---| Returns the style of the active dialog. If the dialog is not opened, returns the style of the last opened dialog instead 
	---@return bool
	---@return SampDialogStyle style
        getDialogStyle = function() end,
	---SAMPFUNCS
        ---| Returns the ID of the active dialog. If the dialog is not opened, returns the ID of the last opened dialog instead 
	---@return bool
	---@return int id
        getDialogId = function() end,
	---SAMPFUNCS
        ---| Returns our connection status towards the server 
	---@return SampGameState statusId
        getGamestate = function() end,
	---SAMPFUNCS
        ---| Returns the handle of an object using its SAMP ID 
	---@param id int
	---@return bool
	---@return Object handle
        getObject = function(id) end,
	---SAMPFUNCS
        ---| Returns the handle of a pickup using its SAMP ID 
	---@param id int
	---@return bool
	---@return Pickup handle
        getPickup = function(id) end,
	---SAMPFUNCS
        ---| Returns the total number of items found on the active dialog's list. If the dialog is not opened, the last opened dialog is used instead 
	---@return bool
	---@return int itemsCount
        getDialogItemsCount = function() end,
	---SAMPFUNCS
        ---| Stores the filename and animname of the animation using its SAMP ID 
	---@param animId int
	---@return bool
	---@return int fileName
	---@return int animName
        getAnimName = function(animId) end,
	---SAMPFUNCS
        ---| Returns the Animation SAMP ID using its name and the file it was loaded from 
	---@param animName string
	---@param fileName string
	---@return bool
	---@return int animId
        getAnimId = function(animName, fileName) end,
	---SAMPFUNCS
        ---| Stores the text of an item with specific id from the active dialog's list, to buffer. If the dialog is not opened, the last opened dialog is evaluated instead 
	---@param itemId int
	---@return bool
	---@return string buffer
        getDialogItemText = function(itemId) end,
	---SAMPFUNCS
        ---| Sets the visibility status of mouse cursor 
	---@param isVisible bool
        setCursorVisibility = function(isVisible) end,
	---SAMPFUNCS
        ---| Returns a pointer to a RakClientInterface object 
	---@return int pointer
        getRakClientInterface = function() end,
	---SAMPFUNCS
        ---| Returns a pointer to RakPeer 
	---@return int pointer
        getRakPeer = function() end,
	---SAMPFUNCS
        ---| Returns the address of the RakClientInterface virtual method table function by index 
	---@param index int
	---@return int address
        getRakClientFunc = function(index) end,
	---SAMPFUNCS
        ---| Returns the RPC callback address by index 
	---@param index int
	---@return bool
	---@return int address
        getRpcFunc = function(index) end,
	---SAMPFUNCS
        ---| Returns a pointer to the RPC structure at index 
	---@param index int
	---@return bool
	---@return int address
        getRpcNode = function(index) end,
	---SAMPFUNCS
        ---| Returns a pointer to a SampInfo structure 
	---@return int address
        getInfoPtr = function() end,
	---SAMPFUNCS
        ---| Checks if the mouse cursor were both visible and movable 
	---@return bool
        isCursorActive = function() end,
	---SAMPFUNCS
        ---| Sets the cursor mode 
	---@param mode SampCursorMode
        setCursorMode = function(mode) end,
	---SAMPFUNCS
        ---| Returns the current mouse cursor mode 
	---@return bool
	---@return SampCursorMode mode
        getCursorMode = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to server settings structure 
	---@return int address
        getServerSettingsPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp pools structure 
	---@return int address
        getPoolsPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp chat information structure 
	---@return int address
        getChatInfoPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp chat input field structure 
	---@return int address
        getChatInfoPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp dialog structure 
	---@return int address
        getDialogInfoPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp kill list structure 
	---@return int address
        getKillInfoPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to a structure of various samp miscellaneous data 
	---@return int address
        getMiscInfoPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp textdraw pool structure 
	---@return int address
        getTextDrawPoolPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp object pool structure 
	---@return int address
        getObjectPoolPtr = function() end,
	---SAMPFUNCS
        ---| Returns a pointer to the pool of samp allocated territories (gang territories) 
	---@return int address
        getGangZonePoolPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to 3D text pool structure 
	---@return int address
        getTextLabelPoolPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to player pool structure 
	---@return int address
        getPlayerPoolPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp car pool structure 
	---@return int address
        getCarPoolPtr = function() end,
	---SAMPFUNCS
        ---| Returns the pointer to samp pickup pool structure 
	---@return int address
        getPickupPoolPtr = function() end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the scoreboard is visible 
	---@return bool
        isScoreboardVisible = function() end,
	---SAMPFUNCS
        ---| Sets the visibility of the scoreboard 
	---@param isVisible bool
        setScoreboardVisibility = function(isVisible) end,
	---SAMPFUNCS
        ---| Returns the content of the active dialog to buffer. If the dialog is not opened, the last opened dialog is evaluated instead 
	---@param buffer string
        getDialogContent = function(buffer) end,
	---SAMPFUNCS
        ---| Stores the title of the active dialog to buffer. If the dialog is not opened, the last opened dialog is evaluated instead 
	---@param buffer string
        getDialogTitle = function(buffer) end,
	---SAMPFUNCS
        ---| Sets whether interactions with the active dialog would sync to the server(server-sided dialog) or not(client-sided dialog) 
	---@param isClientSide bool
        setDialogEnv = function(isClientSide) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the active dialog is has a client side attribute 
	---@return bool
        isDialogClientSide = function() end,
	---SAMPFUNCS
        ---| Returns the maximum player ID currently streamed if streamedOnly = true. Else, returns the maximum player ID allowed in the server 
	---@param streamedOnly bool
	---@return bool
	---@return int maxPlayerId
        maxPlayerId = function(streamedOnly) end,
	---SAMPFUNCS
        ---| Returns the number of players that are currently streamed if streamedOnly = true. Else, returns the number of players allowed in the server 
	---@param streamedOnly bool
	---@return int playerCount
        getPlayerCount = function(streamedOnly) end,
    }
SampChatOp = {
        ---SAMPFUNCS
        ---| Adds one line of colored message to the chat 
	---@param format string
	---@param color int
	---@param args arguments
        addMsg = function(format, color, args) end,
	---SAMPFUNCS
        ---| Changes the chat line's text into a custom one 
	---@param chatLineid int
	---@param body string
	---@param prefix string
	---@param bodyColor int
	---@param prefixColor int
	---@return bool
        setLine = function(chatLineid, body, prefix, bodyColor, prefixColor) end,
	---SAMPFUNCS
        ---| Returns the chat line's text parameters 
	---@param index int
	---@return bool
	---@return string bodyBuffer
	---@return string prefixBuffer
	---@return int bodyColor
	---@return int prefixColor
        getLine = function(index) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if chat lines are visible 
	---@return bool
        isVisible = function() end,
	---SAMPFUNCS
        ---| Returns the current chat display mode 
	---@return SampChatDisplayMode mode
        getDisplayMode = function() end,
	---SAMPFUNCS
        ---| Sets the chat display mode 
	---@param mode SampChatDisplayMode
        setDisplayMode = function(mode) end,
    }
SampRaknetOp = {
        ---SAMPFUNCS
        ---| Sends SampRpc.Chat containing the message or command to the server 
	---@param format string
	---@param args arguments
        sendRpcChat = function(format, args) end,
	---SAMPFUNCS
        ---| Sends SampRpc.RequestClass to Server 
	---@param classId int
        sendRpcRequestClass = function(classId) end,
	---SAMPFUNCS
        ---| Sends SampRpc.ScmEvent information about car modification to the server 
	---@param carId int
	---@param type SampScmEvent
	---@param param1 int
	---@param param2 int
        sendRpcScmEvent = function(carId, type, param1, param2) end,
	---SAMPFUNCS
        ---| Sends SampRpc.Death to Server without our character actually dying 
	---@param playerId int
	---@param weaponId WeaponType
        sendRpcDeath = function(playerId, weaponId) end,
	---SAMPFUNCS
        ---| Sends SampRpc.ScrServerQuit to the server without actually disconnecting our client to the server 
	---@param id SampDisconnectReason
        sendRpcDisconnected = function(id) end,
	---SAMPFUNCS
        ---| Sends SampRpc.EnterCar to Server 
	---@param carId int
	---@param isPassenger bool
        sendRpcEnterCar = function(carId, isPassenger) end,
	---SAMPFUNCS
        ---| Sends SampRpc.ExitCar to Server 
	---@param carId int
        sendRpcExitCar = function(carId) end,
	---SAMPFUNCS
        ---| Sends a SampRpc.Spawn to SAMP Server without spawning our character 
        sendRpcSpawn = function() end,
	---SAMPFUNCS
        ---| Sends a SampRpc.DamageCar to the Server 
	---@param carId int
	---@param bodyFlags int
	---@param doorFlags int
	---@param lightFlags int
	---@param wheelFlags int
        sendRpcDamageCar = function(carId, bodyFlags, doorFlags, lightFlags, wheelFlags) end,
	---SAMPFUNCS
        ---| Connects to the specified samp server information 
	---@param serverIp string
	---@param port int
        connect = function(serverIp, port) end,
	---SAMPFUNCS
        ---| Sends an RCON command to the server 
	---@param command string
        sendRpcRconCmd = function(command) end,
	---SAMPFUNCS
        ---| Sends SampPacket.OnFootSync containing stOnFootData payload to the server 
	---@param payloadPointer int
        sendPktOnFootData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.DrivingSync containing stInCarData payload to the server 
	---@param payloadPointer int
        sendPktDrivingData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.PassengerSync containing stPassengerData payload to the server 
	---@param payloadPointer int
        sendPktPassengerData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.AimSync containing stAimData payload to the server  
	---@param payloadPointer int
        sendPktAimData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.BulletSync containing stBulletData payload to the server 
	---@param payloadPointer int
        sendPktBulletData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.TrailerSync containing stTrailerData payload to the server 
	---@param payloadPointer int
        sendPktTrailerData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.UnoccupiedCarSync containing stUnoccupiedData payload to the server 
	---@param payloadPointer int
        sendPktUnoccupiedCarData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampPacket.SpectatorSync containing stSpectatorData  payload to the server 
	---@param payloadPointer int
        sendPktSpectatorData = function(payloadPointer) end,
	---SAMPFUNCS
        ---| Sends SampRpc.ClickPlayer about double click on player (from Scoreboard for example) 
	---@param playerId int
	---@param clickSourceId int
        sendRpcClickPlayer = function(playerId, clickSourceId) end,
	---SAMPFUNCS
        ---| Sends an SampRpc.DialogResponse to the server 
	---@param dialogId int
	---@param whichButton SampDialogButton
	---@param itemId int
	---@param inputText string
        sendRpcDialogResponse = function(dialogId, whichButton, itemId, inputText) end,
	---SAMPFUNCS
        ---| Sends an SampRpc.ClickTextDraw to the server 
	---@param textDrawId int
        sendRpcClickTextDraw = function(textDrawId) end,
	---SAMPFUNCS
        ---| Sends an SampRpc.GiveTakeDamage to the server about damage dealt by another player 
	---@param playerId int
	---@param floatDamage SampWeaponDamage
	---@param weapon WeaponType
	---@param bodyPart BodyPart
        sendRpcGiveDamage = function(playerId, floatDamage, weapon, bodyPart) end,
	---SAMPFUNCS
        ---| Sends an SampRpc.GiveTakeDamage to the server about damage taken from another player 
	---@param playerId int
	---@param floatDamage SampWeaponDamage
	---@param weapon WeaponType
	---@param bodyPart SampBodyPart
        sendRpcTakeDamage = function(playerId, floatDamage, weapon, bodyPart) end,
	---SAMPFUNCS
        ---| Sends an SampRpc.EditObject about changing the structure of an object in its editing mode 
	---@param isLocalObject bool
	---@param id int
	---@param response SampEditObjectResponse
	---@param coordX float
	---@param coordY float
	---@param coordZ float
	---@param pitch float
	---@param roll float
	---@param yaw float
        sendRpcEditObject = function(isLocalObject, id, response, coordX, coordY, coordZ, pitch, roll, yaw) end,
	---SAMPFUNCS
        ---| Sends SampRpc.EditAttachedObject about a change to an attached object in object edit mode 
	---@param response SampEditObjectResponse
	---@param index int
	---@param model model_object
	---@param pedBone PedBone
	---@param offsetX float
	---@param offsetY float
	---@param offsetZ float
	---@param pitch float
	---@param roll float
	---@param yaw float
	---@param scaleX float
	---@param scaleY float
	---@param scaleZ float
        sendRpcEditAttachedObject = function(response, index, model, pedBone, offsetX, offsetY, offsetZ, pitch, roll, yaw, scaleX, scaleY, scaleZ) end,
	---SAMPFUNCS
        ---| Sends SampRpc.RequestInteriorChange 
	---@param id int
        sendRpcRequestInteriorChange = function(id) end,
	---SAMPFUNCS
        ---| Sends SampRpc.RequestSpawn 
        sendRpcRequestSpawn = function() end,
	---SAMPFUNCS
        ---| Sends SampRpc.PickedUpPickup to take a pickup 
	---@param id int
        sendRpcPickedupPickup = function(id) end,
	---SAMPFUNCS
        ---| Sends SampRpc.MenuSelect about selecting an item in a menu (GTA:SA menu) 
	---@param elementId int
        sendRpcMenuSelectRow = function(elementId) end,
	---SAMPFUNCS
        ---| Sends SampRpc.MenuQuit to tell the server we exited the menu (GTA:SA menu) 
        sendRpcQuitMenu = function() end,
	---SAMPFUNCS
        ---| Sends SampRpc.CarDestroyed about destruction of a specific car (exploded or drenched in water) 
	---@param id int
        sendRpcCarDestroyed = function(id) end,
	---SAMPFUNCS
        ---| Returns the flow of control to RakNet and decides whether the currently intercepted data will be processed by RakNet or not 
	---@param processData bool
        ["return"] = function(processData) end,
	---SAMPFUNCS
        ---| Redirects all outgoing RPCs to the specified callback for subsequent processing before sending to the server 
	---@param callback label
	---@return bool
        hookRpcOut = function(callback) end,
	---SAMPFUNCS
        ---| Redirects all outgoing Packets to the specified callback for subsequent processing before sending to the server 
	---@param callback label
	---@return bool
        hookPacketOut = function(callback) end,
	---SAMPFUNCS
        ---| Redirects all incoming RPCs to the specified callback for subsequent processing before acceptance on the local client 
	---@param callback label
	---@return bool
        hookRpcIn = function(callback) end,
	---SAMPFUNCS
        ---| Redirects all incoming packets to the specified callback for subsequent processing before acceptance on the local client 
	---@param callback label
	---@return bool
        hookPacketIn = function(callback) end,
	---SAMPFUNCS
        ---| Returns the hook parameter's value of the currently executing callback 
	---@param type SampRakNetHookParam
	---@return bool
	---@return any value
        getHookParam = function(type) end,
	---SAMPFUNCS
        ---| Sets the hook parameter's value of the currently executing callback 
	---@param type SampRakNetHookParam
	---@param value any
	---@return bool
        setHookParam = function(type, value) end,
	---SAMPFUNCS
        ---| Returns a pointer to the RPC ID's Name 
	---@param id int
	---@return bool
	---@return string name
        getRpcName = function(id) end,
	---SAMPFUNCS
        ---| Returns a pointer to the Packet ID's Name 
	---@param id int
	---@return bool
	---@return int name
        getPacketName = function(id) end,
	---SAMPFUNCS
        ---| Sends SampPacket.DrivingSync to server about our character driving a car with ID 
	---@param id int
        forceDrivingSync = function(id) end,
	---SAMPFUNCS
        ---| Sends SampPacket.UnoccupiedCarSync to server about a seatId of a car with carId being unoccupied 
	---@param carId int
	---@param seatId SeatId
        forceUnnocupiedSync = function(carId, seatId) end,
	---SAMPFUNCS
        ---| Sends SampPacket.OnFootSync to server about our character being onfoot 
        forceOnfootSync = function() end,
	---SAMPFUNCS
        ---| Sends SampPacket.AimSync containing our current aim data, to server 
        forceAimSync = function() end,
	---SAMPFUNCS
        ---| Sends SampPacket.TrailerSync to server about a trailer with ID being attached to our player 
	---@param id int
        forceTrailerSync = function(id) end,
	---SAMPFUNCS
        ---| Sends SampPacket.PassengerSync to server about our character sitting at seatId of a car with carId 
	---@param carId int
	---@param seatId SeatId
        forcePassengerSync = function(carId, seatId) end,
	---SAMPFUNCS
        ---| Sends SampPacket.StatsUpdate containing our current money and drunk level, to server 
        forceStatsSync = function() end,
	---SAMPFUNCS
        ---| Sends SampPacket.WeaponsUpdate containing our character's current weapons, to server 
        forceWeaponsSync = function() end,
    }
SampPlayerOp = {
        ---SAMPFUNCS
        ---| Returns the character handle using the Player ID. Returns -1 if the player is not in the stream zone 
	---@param self SampPlayer
	---@return Char handle
        getChar = function(self) end,
	---SAMPFUNCS
        ---| Returns true if REMOTE Player with the given ID is connected 
	---@param self SampPlayer
	---@return bool
        isRemotelyConnected = function(self) end,
	---SAMPFUNCS
        ---| Returns the player's samp structure. Returns 0 (NULL Pointer) if player isn't connected 
	---@param self SampPlayer
	---@return int address
        getPtr = function(self) end,
	---SAMPFUNCS
        ---| Returns the amount of health the specified player has 
	---@param self SampPlayer
	---@return int health
        getHealth = function(self) end,
	---SAMPFUNCS
        ---| Returns the amount of armor the specified player has 
	---@param self SampPlayer
	---@return int armor
        getArmor = function(self) end,
	---SAMPFUNCS
        ---| Returns the ping of the specified player with ID 
	---@param self SampPlayer
	---@return int ping
        getPing = function(self) end,
	---SAMPFUNCS
        ---| Returns the 3D Coordinates of a player who is outside the stream zone, if the server allows it 
	---@param self SampPlayer
	---@return float x
	---@return float y
	---@return float z
        getStreamedOutCoords = function(self) end,
	---SAMPFUNCS
        ---| Returns a pointer to the nickname of the specified player 
	---@param self SampPlayer
	---@return int nickname
        getName = function(self) end,
	---SAMPFUNCS
        ---| Returns the color of the specified player in 0xAARRGGBB format 
	---@param self SampPlayer
	---@return int color
        getColor = function(self) end,
	---SAMPFUNCS
        ---| Returns the SAMP ID of the animation currently being played by the specified player 
	---@param self SampPlayer
	---@return bool
	---@return int animId
        getAnim = function(self) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified player is in paused state (or AFK) 
	---@param self SampPlayer
	---@return bool
        isPaused = function(self) end,
	---SAMPFUNCS
        ---| Returns the special action ID of the specified player 
	---@param self SampPlayer
	---@return bool
	---@return SampSpecialAction specialActionId
        getSpecialAction = function(self) end,
	---SAMPFUNCS
        ---| Checks if the specified player is an NPC 
	---@param self SampPlayer
	---@return bool
        isNpc = function(self) end,
	---SAMPFUNCS
        ---| Returns the current score of the specified player 
	---@param self SampPlayer
	---@return bool
	---@return int score
        getScore = function(self) end,
	---SAMPFUNCS
        ---| Stores the current player's onFootData structure to the buffer 
	---@param self SampPlayer
	---@return bool
	---@return int buffer
        storeOnFootData = function(self) end,
	---SAMPFUNCS
        ---| Stores the player's current inCarData structure to the buffer 
	---@param self SampPlayer
	---@return bool
	---@return int buffer
        storeDrivingData = function(self) end,
	---SAMPFUNCS
        ---| Stores the current passengerData structure of the player to the buffer 
	---@param self SampPlayer
	---@return bool
	---@return int buffer
        storePassengerData = function(self) end,
	---SAMPFUNCS
        ---| Stores the player's current trailerData structure to the buffer 
	---@param self SampPlayer
	---@return bool
	---@return int buffer
        storeTrailerData = function(self) end,
	---SAMPFUNCS
        ---| Stores the specified player's aimData structure to the buffer 
	---@param self SampPlayer
	---@return bool
	---@return int buffer
        storeAimData = function(self) end,
    }
SampChatInputOp = {
        ---SAMPFUNCS
        ---| Checks if the chat box input is open/visible 
	---@return bool
        isVisible = function() end,
	---SAMPFUNCS
        ---| Overwrites the content written at the chat input box 
	---@param text string
        setText = function(text) end,
	---SAMPFUNCS
        ---| Returns the current text in the chat input box 
	---@return bool
	---@return string buffer
        getText = function() end,
	---SAMPFUNCS
        ---| Sets the visibility status (open/closed) of the chat input box 
	---@param isVisible bool
        setVisibility = function(isVisible) end,
	---SAMPFUNCS
        ---| Processes a message by SAMPFUNCS callbacks, then sent to server as SampRpc.Chat 
	---@param text string
        process = function(text) end,
    }
SampLocalChatCmdOp = {
        ---SAMPFUNCS
        ---| Registers a callback hooked from a client sided chat command 
	---@param chatCommand string
	---@param callback label
	---@return bool
        hook = function(chatCommand, callback) end,
	---SAMPFUNCS
        ---| Removes all callbacks hooked from a chatCommand created by SAMP_REGISTER_CLIENTSIDE_COMMAND, suppressing all its callback's operation  
	---@param chatCommand string
	---@return bool
        unhook = function(chatCommand) end,
	---SAMPFUNCS
        ---| Sets the description for the local command 
	---@param chatCommand string
	---@param decription string
        setDescription = function(chatCommand, decription) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified command is localized by a callback hook 
	---@param chatCommand string
	---@return bool
        isHooked = function(chatCommand) end,
    }
SfOp = {
        ---SAMPFUNCS
        ---| Returns a pointer to a string containing the parameters of the last command entered at console or SAMP Chat 
	---@return bool
	---@return int pointer
        getCmdParams = function() end,
	---SAMPFUNCS
        ---| Marks the end of a command callback. Used as its returning statement 
        cmdRet = function() end,
	---SAMPFUNCS
        ---| Converts 3D coordinates from the world into window screen coordinates (pixels) 
	---@param worldCoordX float
	---@param worldCoordY float
	---@param worldCoordZ float
	---@return bool
	---@return int screenCoordX
	---@return int screenCoordY
        from3DTo2DCoords = function(worldCoordX, worldCoordY, worldCoordZ) end,
	---SAMPFUNCS
        ---| Sets the press status of a game (NOT keyboard) key 
	---@param buttonId Button
	---@param behaviorValue int
        setButton = function(buttonId, behaviorValue) end,
	---SAMPFUNCS
        ---| Returns the current window screen resolution in pixels 
	---@return int width
	---@return int height
        getScreenRes = function() end,
	---SAMPFUNCS
        ---| Returns the mouse cursor's window screen coordinates in pixels 
	---@return int windowScreenCoordX
	---@return int windowScreenCoordY
        getCursorCoord = function() end,
	---SAMPFUNCS
        ---| Returns the GameScreen Coordinates counterpart of the specified WindowScreen Coordinates 
	---@param windowScreenCoordX int
	---@param windowScreenCoordY int
	---@return float gameScreenCoordX
	---@return float gameScreenCoordY
        fromWindowToGameScreenCoords = function(windowScreenCoordX, windowScreenCoordY) end,
	---SAMPFUNCS
        ---| Returns the WindowScreen Coordinates counterpart of the specified GameScreen Coordinates 
	---@param gameScreenCoordX float
	---@param gameScreenCoordY float
	---@return int windowScreenCoordX
	---@return int windowScreenCoordY
        fromGameToWindowScreenCoords = function(gameScreenCoordX, gameScreenCoordY) end,
	---SAMPFUNCS
        ---| Splits 0xAARRGGBB colorcode format into individial color channels 
	---@param color int
	---@return int alpha
	---@return int red
	---@return int green
	---@return int blue
        hexToArgb = function(color) end,
	---SAMPFUNCS
        ---| mixes color channels into 0xAARRGGBB colorcode format 
	---@param alpha int
	---@param red int
	---@param green int
	---@param blue int
	---@return int color
        argbToHex = function(alpha, red, green, blue) end,
	---SAMPFUNCS
        ---| Converts window screen coordinates (pixels) to world's 3D coordinates with the specified depth 
	---@param screenCoordX int
	---@param screenCoordY int
	---@param depth float
	---@return float worldCoordX
	---@return float worldCoordY
	---@return float worldCoordZ
        from2DTo3DCoords = function(screenCoordX, screenCoordY, depth) end,
	---SAMPFUNCS
        ---| Sets the specified script's isActive parameter to 0 
	---@param scriptId int
        pauseScript = function(scriptId) end,
	---SAMPFUNCS
        ---| Sets the specified script's isActive parameter to 1 
	---@param scriptId int
        resumeScript = function(scriptId) end,
	---SAMPFUNCS
        ---| Saves the values of all local variables from the currently executing script (Main Script or a Function) to a separate memory that can be recovered later on by SF_POP_LOCAL_VARIABLES 
        pushLocalVars = function() end,
	---SAMPFUNCS
        ---| Restores all previously saved local variables by SF_PUSH_LOCAL_VARIABLES to the local variables of the currently executing script (Main Script or a Function) 
        popLocalVars = function() end,
	---SAMPFUNCS
        ---| Returns the OS's current uptime in milliseconds. Recommended substitute for TIMERA and TIMERB when requiring precise timing operations 
	---@return int tickCount
        getTickCount = function() end,
	---SAMPFUNCS
        ---| Checks for collisions using flags from the start of the vector to its end 
	---@param fromCoordX float
	---@param fromCoordY float
	---@param fromCoordZ float
	---@param toCoordX float
	---@param toCoordY float
	---@param toCoordZ float
	---@param checkBuildings bool
	---@param checkCars bool
	---@param checkChars bool
	---@param checkObjects bool
	---@param checkParticles bool
	---@param seeThrough bool
	---@param ignoreCameras bool
	---@return bool
	---@return any colPointPtr
	---@return any entityPtr
        processLineOfSight = function(fromCoordX, fromCoordY, fromCoordZ, toCoordX, toCoordY, toCoordZ, checkBuildings, checkCars, checkChars, checkObjects, checkParticles, seeThrough, ignoreCameras) end,
	---SAMPFUNCS
        ---| Returns the absolute value of a number (float or integer) 
	---@param number any
	---@return any result
        abs = function(number) end,
	---SAMPFUNCS
        ---| Converts radians to degrees 
	---@param radians float
	---@return float degrees
        radToDeg = function(radians) end,
	---SAMPFUNCS
        ---| Converts degrees to radians 
	---@param degrees float
	---@return float radians
        degToRad = function(degrees) end,
	---SAMPFUNCS
        ---| Returns the sine of the specified radians 
	---@param radians float
	---@return float ratio
        sin = function(radians) end,
	---SAMPFUNCS
        ---| Returns the radian arcsine of the specified ratio 
	---@param ratio float
	---@return float radians
        aSin = function(ratio) end,
	---SAMPFUNCS
        ---| Returns the cosine of the specified radians 
	---@param radians float
	---@return float ratio
        cos = function(radians) end,
	---SAMPFUNCS
        ---| Returns the radian arccosine of the specified ratio 
	---@param ratio float
	---@return float radians
        aCos = function(ratio) end,
	---SAMPFUNCS
        ---| Returns the tangent of the specified radians 
	---@param radians float
	---@return float ratio
        tan = function(radians) end,
	---SAMPFUNCS
        ---| Returns the radian arctangent of the specified ratio 
	---@param ratio float
	---@return float radians
        aTan = function(ratio) end,
	---SAMPFUNCS
        ---| Raises a number to the specified power 
	---@param coefficient float
	---@param exponent float
	---@return float power
        pow = function(coefficient, exponent) end,
	---SAMPFUNCS
        ---| Returns the rounded-up result the specified number 
	---@param number float
	---@return float result
        ceil = function(number) end,
	---SAMPFUNCS
        ---| Returns the rounded-down result the specified number 
	---@param number float
	---@return float result
        floor = function(number) end,
	---SAMPFUNCS
        ---| Reads the value with size from memory address with offset 
	---@param address int
	---@param offset int
	---@param size int
	---@return bool
	---@return any value
        readMemoryWithOffset = function(address, offset, size) end,
	---SAMPFUNCS
        ---| Writes the value with size to memory address with offset 
	---@param address int
	---@param offset int
	---@param size int
	---@param value any
	---@return bool
        writeMemoryWithOffset = function(address, offset, size, value) end,
	---SAMPFUNCS
        ---| Reads the value from the specified element index of an array 
	---@param array int
	---@param index int
	---@return bool
	---@return any value
        readElementOf4BytesArray = function(array, index) end,
	---SAMPFUNCS
        ---| Writes the value to the specified element index of an array 
	---@param array int
	---@param index int
	---@param value any
	---@return bool
        writeElementOf4BytesArray = function(array, index, value) end,
	---SAMPFUNCS
        ---| Copies a memory block with size from source address to destination address 
	---@param destination int
	---@param source int
	---@param size int
	---@return bool
        memCpy = function(destination, source, size) end,
	---SAMPFUNCS
        ---| Fills a sized memory block with value byte-by-byte 
	---@param address int
	---@param byteValue int
	---@param size int
	---@return bool
        memFill = function(address, byteValue, size) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if both memory blocks have the same content 
	---@param addressA int
	---@param addressB int
	---@param size int
	---@return bool
        memEq = function(addressA, addressB, size) end,
	---SAMPFUNCS
        ---| Copies the string from source to destination 
	---@param destination string
	---@param source string
        strCpy = function(destination, source) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if both case-sensitive strings are equal 
	---@param stringA string
	---@param stringB string
	---@return bool
        strEq = function(stringA, stringB) end,
	---SAMPFUNCS
        ---| Appends the appendedString at the end of the string found at the stringBuffer 
	---@param stringBuffer string
	---@param appendedString string
        strCat = function(stringBuffer, appendedString) end,
	---SAMPFUNCS
        ---| Searches the stringBuffer for any delimiters then replaces the first hit with a NULL terminator (0x00). Returns the (token) pointer next to the first hit. Can be used as a condition which evaluates as logical false if none of the delimiters were found at the string 
	---@param stringBuffer string
	---@param delimiters string
	---@return bool
	---@return string token
        strTok = function(stringBuffer, delimiters) end,
	---SAMPFUNCS
        ---| Returns the length of a string 
	---@param string string
	---@return int length
        strLen = function(string) end,
	---SAMPFUNCS
        ---| Returns a pointer to the first occurence of a specified substring in the source string 
	---@param source string
	---@param subString string
	---@return bool
	---@return string matchAddress
        strStr = function(source, subString) end,
	---SAMPFUNCS
        ---| Searches the string for any matching characters at the characterList. Returns the character index of the first occurence from the string and evaluates as logical true if used as a condition. Returns the length of the string if nothing matched the characterSet 
	---@param source string
	---@param characterList string
	---@return bool
	---@return int index
        strCspn = function(source, characterList) end,
	---SAMPFUNCS
        ---| Converts an ascii string into a decimal integer 
	---@param ascii string
	---@return int integer
        atoI = function(ascii) end,
	---SAMPFUNCS
        ---| Converts an ascii string into a floating point number 
	---@param ascii string
	---@return float float
        atoF = function(ascii) end,
	---SAMPFUNCS
        ---| Converts a baseN integer into an ascii string then stores it to stringBuffer 
	---@param integer int
	---@param radix int
	---@return string stringBuffer
        itoA = function(integer, radix) end,
	---SAMPFUNCS
        ---| Reads the value of the sized array's element at index. The size per element is constrained between 1 to 4 
	---@param array int
	---@param index int
	---@param size int
	---@return bool
	---@return any value
        readElementOfSizedArray = function(array, index, size) end,
	---SAMPFUNCS
        ---| Writes the value to the specified element index of a sized array. The size per element is constrained between 1 to 4 
	---@param array int
	---@param index int
	---@param size int
	---@param value any
	---@return bool
        writeElementOfSizedArray = function(array, index, size, value) end,
	---SAMPFUNCS
        ---| Returns a pointer to the element of an array of sized buffers using its element index 
	---@param array int
	---@param index int
	---@param size int
	---@return int address
        getElementPointerOfBufferArray = function(array, index, size) end,
	---SAMPFUNCS
        ---| Writes a string at the buffer element of an array of sized buffers using its element index 
	---@param array int
	---@param index int
	---@param size int
	---@param string string
        writeStringToElementOfBufferArray = function(array, index, size, string) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if both case-insensitive strings are equal 
	---@param stringA string
	---@param stringB string
	---@return bool
        striEq = function(stringA, stringB) end,
	---SAMPFUNCS
        ---| Converts binary text to hexadecimal text and stores at stringBuffer. Can be used as a condition which evaluates as logical false if the bufferSize is not enough to contain the converted hexadecimal text 
	---@param binaryString string
	---@param stringBuffer string
	---@param bufferSize int
	---@return bool
        bintoHex = function(binaryString, stringBuffer, bufferSize) end,
	---SAMPFUNCS
        ---| Converts hexadecimal text to binary text and stores at stringBuffer. Can be used as a condition which evaluates as logical false if the bufferSize is not enough to contain the converted binary text 
	---@param hexString string
	---@param stringBuffer string
	---@param bufferSize int
	---@return bool
        hextoBin = function(hexString, stringBuffer, bufferSize) end,
	---SAMPFUNCS
        ---| Copies the string safely from source to stringBuffer. The copied string will be truncated if the occupied size is beyond the specified bufferSize 
	---@param stringBuffer int
	---@param source int
	---@param bufferSize int
        strnCpy = function(stringBuffer, source, bufferSize) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if all characters with length at the beginning of both case-sensitive strings are equal 
	---@param stringA string
	---@param stringB string
	---@param length int
	---@return bool
        strnEq = function(stringA, stringB, length) end,
	---SAMPFUNCS
        ---| Converts all letters of the source's string into CAPITAL LETTERS stored at destination 
	---@param source string
	---@return string destination
        strUpr = function(source) end,
	---SAMPFUNCS
        ---| Appends a appendedString safely at the end of the string found at the stringBuffer. The appendedString will be truncated if the overall size is beyond the specified bufferSize 
	---@param stringBuffer string
	---@param appendedString string
	---@param bufferSize int
        strnCat = function(stringBuffer, appendedString, bufferSize) end,
	---SAMPFUNCS
        ---| Converts all letters of the source's string into non-capital letters stored at destination 
	---@param source string
	---@return string destination
        strLwr = function(source) end,
	---SAMPFUNCS
        ---| Returns a pointer to the first case-insensitive occurence of the specified substring in the source string 
	---@param source string
	---@param subString string
	---@return bool
	---@return string matchAddress
        striStr = function(source, subString) end,
	---SAMPFUNCS
        ---| Returns a pointer to the first occurence of the specified ascii character in the source string 
	---@param source string
	---@param character int
	---@return bool
	---@return int matchAddress
        strChr = function(source, character) end,
	---SAMPFUNCS
        ---| Searches the source's string for any matching characters at the characterList. Returns a pointer to the first occurence from the source's string and evaluates as logical true if used as a condition 
	---@param source string
	---@param characterList string
	---@return bool
	---@return string matchAddress
        strPBrk = function(source, characterList) end,
	---SAMPFUNCS
        ---| Returns a pointer to the last occurence of the specified ascii character in the source string 
	---@param source string
	---@param character int
	---@return bool
	---@return string matchAddress
        strRChr = function(source, character) end,
	---SAMPFUNCS
        ---| Reverses the source's string, changing the positions of characters, then stores it to the destination 
	---@param source string
	---@return string destination
        strRev = function(source) end,
	---SAMPFUNCS
        ---| Searches the source's string then returns the index of the first character that does not match any of the specified characters in the characterList and evaluates as logical true if used as a condition. Returns the length of the string if all characters matches any characters at the characterList 
	---@param source string
	---@param characterList string
	---@return bool
	---@return int index
        strSpn = function(source, characterList) end,
	---SAMPFUNCS
        ---| Ignores any whitespace at the beginning of the source's string, converting the next characters into a baseN longInteger. The scanning stops when it comes across the first non-integer character, stores its pointer to unscannedAddress 
	---@param source int
	---@param radix int
	---@return int unscannedAddress
	---@return int longInteger
        strTol = function(source, radix) end,
	---SAMPFUNCS
        ---| Converts a matrix structure into a quaternion structure stored at quaternionBuffer 
	---@param matrix int
	---@return int quaternionBuffer
        matrixToQuat = function(matrix) end,
	---SAMPFUNCS
        ---| Converts a quaternion structure into a matrix structure stored at matrixBuffer 
	---@param quaternion int
	---@return int matrixBuffer
        quatToMatrix = function(quaternion) end,
	---SAMPFUNCS
        ---| Converts the basis vectors of a rotation matrix into quaternion values 
	---@param pitchVectorX float
	---@param pitchVectorY float
	---@param pitchVectorZ float
	---@param rollVectorX float
	---@param rollVectorY float
	---@param rollVectorZ float
	---@param yawVectorX float
	---@param yawVectorY float
	---@param yawVectorZ float
	---@return float quaternionW
	---@return float quaternionX
	---@return float quaternionY
	---@return float quaternionZ
        axesToQuat = function(pitchVectorX, pitchVectorY, pitchVectorZ, rollVectorX, rollVectorY, rollVectorZ, yawVectorX, yawVectorY, yawVectorZ) end,
	---SAMPFUNCS
        ---| Converts the quaternion values into basis vectors of a rotation matrix 
	---@param quaternionW float
	---@param quaternionX float
	---@param quaternionY float
	---@param pitchVectorZ float
	---@return float pitchVectorX
	---@return float pitchVectorY
	---@return float pitchVectorZ
	---@return float rollVectorX
	---@return float rollVectorY
	---@return float rollVectorZ
	---@return float yawVectorX
	---@return float yawVectorY
	---@return float yawVectorZ
        mathQuatToMatrixVectors = function(quaternionW, quaternionX, quaternionY, pitchVectorZ) end,
	---SAMPFUNCS
        ---| Stores a pointer to the specified string into the var 
	---@param var int
	---@param string string
	---@param unused arguments
        strPtr = function(var, string, unused) end,
	---SAMPFUNCS
        ---| Stores the contents of the specified Windows variable (or environment variable) name to buffer 
	---@param varName string
	---@param stringBuffer string
	---@param buffersize int
	---@return bool
        getOsEnv = function(varName, stringBuffer, buffersize) end,
	---SAMPFUNCS
        ---| Converts a Unicode string to ANSI string then stores it to ansiStringBuffer 
	---@param unicodeString int
	---@param ansiStringBuffer string
	---@param bufferSize int
	---@return bool
        unicodeToAnsi = function(unicodeString, ansiStringBuffer, bufferSize) end,
	---SAMPFUNCS
        ---| Converts an ANSI string to Unicode string then stores it to ansiStringBuffer 
	---@param ansiString string
	---@param unicodeStringBuffer int
	---@param buffersize int
	---@return bool
        ansiToUnicode = function(ansiString, unicodeStringBuffer, buffersize) end,
	---SAMPFUNCS
        ---| Returns the handle of a loaded module with name (moduleName) 
	---@param moduleName string
	---@return bool
	---@return int handle
        getModule = function(moduleName) end,
	---SAMPFUNCS
        ---| Returns a pointer (functionPtr) to specified functionName inside the moduleHandle 
	---@param moduleHandle int
	---@param functionName string
	---@return bool
	---@return int functionPtr
        getModuleProc = function(moduleHandle, functionName) end,
	---SAMPFUNCS
        ---| Sets whether a specific virtual key code is pressed or not 
	---@param keyCode KeyCode
	---@param isPressed bool
        setKeyStatus = function(keyCode, isPressed) end,
	---SAMPFUNCS
        ---| Sets whether a specific virtual key represented in ascii character format is pressed or not 
	---@param ascii int
	---@param isPressed bool
        setCharKeyStatus = function(ascii, isPressed) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified virtual keyCode has been pressed just now 
	---@param keyCode KeyCode
	---@return bool
        keyJustPressed = function(keyCode) end,
	---SAMPFUNCS
        ---| Sets the text on the clipboard 
	---@param text string
        clipboardWriteText = function(text) end,
	---SAMPFUNCS
        ---| Copies raw binary or text from the clipboard to buffer 
	---@param buffer int
	---@param size int
	---@return bool
        clipboardReadData = function(buffer, size) end,
	---SAMPFUNCS
        ---| Returns the installed version of CLEO 
	---@return int version
        getCleoLibVer = function() end,
	---SAMPFUNCS
        ---| Makes the script where this command is executed invisible across other scripts 
	---@param isPrivate int
        makeScriptPrivate = function(isPrivate) end,
    }
SampBitstreamOp = {
        ---SAMPFUNCS
        ---| Creates a new raknet bitstream object 
	---@return SampBitstream handle
        create = function() end,
	---SAMPFUNCS
        ---| Remove's the specified bitstream object, freeing it from memory 
	---@param self SampBitstream
	---@return bool
        delete = function(self) end,
	---SAMPFUNCS
        ---| Resets all parameters/clears BitStream 
	---@param self SampBitstream
	---@return bool
        reset = function(self) end,
	---SAMPFUNCS
        ---| Writes a specified value with datatype and datasize at the "write pointer" of the specified bitstream, then advances its "write pointer" by the same size 
	---@param self SampBitstream
	---@param value any
	---@param dataType SampBitStreamDataType
	---@param dataSize int
	---@return bool
        write = function(self, value, dataType, dataSize) end,
	---SAMPFUNCS
        ---| Sends a BitStream as an RPC with the specified parameters 
	---@param rpc SampRpc
	---@param self SampBitstream
	---@param priority SampPriority
	---@param reliability SampReliability
	---@param orderingChannel int
	---@param shiftTimeStamp int
	---@return bool
        sendAsRpcWithParams = function(rpc, self, priority, reliability, orderingChannel, shiftTimeStamp) end,
	---SAMPFUNCS
        ---| Sends a Packet BitStream with the specified parameters. Commonly used to send dacket data to server 
	---@param self SampBitstream
	---@param priority SampPriority
	---@param reliability SampReliability
	---@param orderingChannel int
	---@return bool
        sendAsPacketWithParams = function(self, priority, reliability, orderingChannel) end,
	---SAMPFUNCS
        ---| Sends BitStream as RPC to Server 
	---@param rpcId SampRpc
	---@param self SampBitstream
	---@return bool
        sendAsRpc = function(rpcId, self) end,
	---SAMPFUNCS
        ---| Sends Packet BitStream to Server 
	---@param self SampBitstream
	---@return bool
        sendAsPacket = function(self) end,
	---SAMPFUNCS
        ---| Reads a value with datatype and datasize at the "read pointer" of the specified bitstream , then advances its "read pointer" by the same size 
	---@param self SampBitstream
	---@param dataType SampBitStreamDataType
	---@return bool
	---@return any value
        read = function(self, dataType) end,
	---SAMPFUNCS
        ---| Stores an array of bytes from the Bitstream to buffer 
	---@param self SampBitstream
	---@param buffer int
	---@param size int
	---@return bool
        readArray = function(self, buffer, size) end,
	---SAMPFUNCS
        ---| Resets the read pointer of the BitStream. Setting "write offset = 0" 
	---@param self SampBitstream
	---@return bool
        resetReadPtr = function(self) end,
	---SAMPFUNCS
        ---| Resets (THIS COMMAND IS BUGGED, SETS THE WRITE POINTER AT THE BEGINNING OF THE DATA ( WRITE OFFSET = 0) WHICH IS WRONG. DO NOT USE) the value write pointer in the BitStream 
	---@param self SampBitstream
	---@return bool
        resetWritePtr = function(self) end,
	---SAMPFUNCS
        ---| Increases both its "read pointer" and "write pointer" of the Bitstream by the specified bit count 
	---@param self SampBitstream
	---@param bitCount int
	---@return bool
        skipBits = function(self, bitCount) end,
	---SAMPFUNCS
        ---| Sets the write offset of the BitStream 
	---@param self SampBitstream
	---@param offset int
	---@return bool
        setWriteOffset = function(self, offset) end,
	---SAMPFUNCS
        ---| Sets the read offset of the BitStream 
	---@param self SampBitstream
	---@param offset int
	---@return bool
        setReadPtr = function(self, offset) end,
	---SAMPFUNCS
        ---| Returns the number of used bits in the BitStream 
	---@param self SampBitstream
	---@return int usedBits
        getBitsUsed = function(self) end,
	---SAMPFUNCS
        ---| Returns the number of bytes used in the BitStream 
	---@param self SampBitstream
	---@return bool
	---@return int usedBytes
        getBytesUsed = function(self) end,
	---SAMPFUNCS
        ---| Returns the number of unread bits in the BitStream 
	---@param self SampBitstream
	---@return bool
	---@return int unreadBits
        getUnreadBits = function(self) end,
	---SAMPFUNCS
        ---| Returns the current write offset of the BitStream 
	---@param self SampBitstream
	---@return bool
	---@return int offset
        getWriteOffset = function(self) end,
	---SAMPFUNCS
        ---| Returns the current read offset of the BitStream 
	---@param self SampBitstream
	---@return bool
	---@return int offset
        getReadOffset = function(self) end,
	---SAMPFUNCS
        ---| Returns a pointer to the BitStream's data 
	---@param self SampBitstream
	---@return bool
	---@return int address
        getDataPtr = function(self) end,
	---SAMPFUNCS
        ---| Decrypts a compressed string (CString) from the bitstream then stores this to the buffer 
	---@param self SampBitstream
	---@param buffer int
	---@param buffersize int
	---@return bool
        decodeCString = function(self, buffer, buffersize) end,
	---SAMPFUNCS
        ---| Encrypts a string stored in the buffer then stores this compressed string (CString) to the bitstream 
	---@param self SampBitstream
	---@param buffer int
	---@param bufferSize int
	---@return bool
        encodeString = function(self, buffer, bufferSize) end,
	---SAMPFUNCS
        ---| Emulates the BitStream's data like an incoming RPC 
	---@param id SampRpc
	---@param self SampBitstream
	---@return bool
        emulateAsRpcIn = function(id, self) end,
	---SAMPFUNCS
        ---| Emulates the BitStream's data like an Incoming Packet 
	---@param id SampPacket
	---@param self SampBitstream
	---@return bool
        emulateAsPacketIn = function(id, self) end,
    }
SampTextLabel3DOp = {
        ---SAMPFUNCS
        ---| Creates an artificial SAMP 3D text with the specified parameter attributes 
	---@param text string
	---@param color int
	---@param coordX float
	---@param coordY float
	---@param coordZ float
	---@param visibilityRadius float
	---@param showBehindWalls bool
	---@param attachedPlayerId int
	---@param attachedCarId int
	---@return bool
	---@return SampTextLabel3D handle
        create = function(text, color, coordX, coordY, coordZ, visibilityRadius, showBehindWalls, attachedPlayerId, attachedCarId) end,
	---SAMPFUNCS
        ---| Destroys a 3D text using it's ID 
	---@param self SampTextLabel3D
	---@return bool
        delete = function(self) end,
	---SAMPFUNCS
        ---| Returns logical true if the specified 3D text exists 
	---@param self SampTextLabel3D
	---@return bool
        doesExist = function(self) end,
	---SAMPFUNCS
        ---| Creates/overwrites a 3D text with the specified ID 
	---@param idAsHandle SampTextLabel3D
	---@param text int
	---@param color int
	---@param coordX float
	---@param coordY float
	---@param coordZ float
	---@param visibilityRadius float
	---@param showBehindWalls bool
	---@param attachedPlayerId int
	---@param attachedCarId int
        createWithId = function(idAsHandle, text, color, coordX, coordY, coordZ, visibilityRadius, showBehindWalls, attachedPlayerId, attachedCarId) end,
	---SAMPFUNCS
        ---| Returns all the informations about a SampTextLabel3D using its ID 
	---@param self SampTextLabel3D
	---@return bool
	---@return string text
	---@return int color
	---@return float coordX
	---@return float coordY
	---@return float coordZ
	---@return float visibilityRadius
	---@return bool showBehindWalls
	---@return int attachedPlayerId
	---@return int attachedCarId
        getParams = function(self) end,
	---SAMPFUNCS
        ---| Sets new text for 3D text 
	---@param self SampTextLabel3D
	---@param text string
	---@return bool
        setText = function(self, text) end,
    }
SfD3DOp = {
        ---SAMPFUNCS
        ---| Draws a line between two window screen coordinates 
	---@param fromCoordX int
	---@param fromCoordY int
	---@param toCoordX int
	---@param toCoordY int
	---@param tickness int
	---@param color int
        drawLine = function(fromCoordX, fromCoordY, toCoordX, toCoordY, tickness, color) end,
	---SAMPFUNCS
        ---| Draws a rectangular area at the specified coordinates 
	---@param coordX int
	---@param coordY int
	---@param width int
	---@param height int
	---@param color int
        drawBorderlessBox = function(coordX, coordY, width, height, color) end,
	---SAMPFUNCS
        ---| Draws a rectangular area with border at the specified coordinates 
	---@param coordX int
	---@param coordY int
	---@param width int
	---@param height int
	---@param color int
	---@param borderSize int
	---@param borderColor int
        drawBorderedBox = function(coordX, coordY, width, height, color, borderSize, borderColor) end,
	---SAMPFUNCS
        ---| Draws a polygon with the specified parameters 
	---@param coordX int
	---@param coordY int
	---@param width int
	---@param height int
	---@param cornerCount int
	---@param heading float
	---@param color int
        drawPolygon = function(coordX, coordY, width, height, cornerCount, heading, color) end,
    }
SfD3DFontOp = {
        ---SAMPFUNCS
        ---| Returns the width (in pixels) that will be occupied by the text with font 
	---@param self SfD3DFont
	---@param text string
	---@return int width
        getDrawWidth = function(self, text) end,
	---SAMPFUNCS
        ---| Returns the height (in pixels) occupied by any text that uses the specified font 
	---@param self SfD3DFont
	---@return int height
        getDrawHeight = function(self) end,
	---SAMPFUNCS
        ---| Creates a D3DFont Object 
	---@param fontName string
	---@param size int
	---@param styleFlags SampTextStyle
	---@return SfD3DFont handle
        create = function(fontName, size, styleFlags) end,
	---SAMPFUNCS
        ---| Destroys the specified font object, freeing it from memory 
	---@param self SfD3DFont
	---@return bool
        delete = function(self) end,
	---SAMPFUNCS
        ---| Draws text using the specified font 
	---@param self SfD3DFont
	---@param text string
	---@param coordX int
	---@param coordY int
	---@param color int
        drawText = function(self, text, coordX, coordY, color) end,
    }
SfD3DTextureOp = {
        ---SAMPFUNCS
        ---| Loads a file (any image, or txd) as D3DTexture Object 
	---@param filePath string
	---@return bool
	---@return SfD3DTexture handle
        load = function(filePath) end,
	---SAMPFUNCS
        ---| Releases the D3DTexture Object, freeing it from memory 
	---@param self SfD3DTexture
	---@return bool
        release = function(self) end,
	---SAMPFUNCS
        ---| Draws the texture on the screen 
	---@param self SfD3DTexture
	---@param coordX int
	---@param coordY int
	---@param width int
	---@param height int
	---@param heading float
	---@param contrast int
        draw = function(self, coordX, coordY, width, height, heading, contrast) end,
	---SAMPFUNCS
        ---| Loads a texture from a file located in memory buffer 
	---@param buffer int
	---@param bufferSize int
	---@return bool
	---@return SfD3DTexture handle
        loadTextureFromFileMemory = function(buffer, bufferSize) end,
    }
SfConsoleOp = {
        ---SAMPFUNCS
        ---| Adds a line to the SAMPFUNCS console and logs it 
	---@param format string
	---@param args arguments
        log = function(format, args) end,
	---SAMPFUNCS
        ---| Executes the specified command in the SAMPFUNCS console 
	---@param command string
        execCmd = function(command) end,
	---SAMPFUNCS
        ---| Registers a callback hooked on a SAMPFUNCS console command with maximum length of 64 characters 
	---@param command string
	---@param callback label
	---@return bool
        registerCmd = function(command, callback) end,
	---SAMPFUNCS
        ---| Removes the specified SAMPFUNCS console command. This command does nothing if the the specified command isn't a registered in the SAMPFUNCS console then evaluates as logical false if used as a condition 
	---@param command string
	---@return bool
        unregisterCmd = function(command) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the SAMPFUNCS console is open 
	---@return bool
        isOpen = function() end,
	---SAMPFUNCS
        ---| Sets the description for the console command 
	---@param command string
	---@param description string
        setCmdDesc = function(command, description) end,
	---SAMPFUNCS
        ---| Returns logical true if the specified console command is registered 
	---@param command string
	---@return bool
        isCmdRegistered = function(command) end,
    }
SfDxutDialogOp = {
        ---SAMPFUNCS
        ---| Creates a DXUTDialog Object with Title 
	---@param title string
	---@return SfDxutDialog handle
        create = function(title) end,
	---SAMPFUNCS
        ---| Returns the last event and component ID that occurred with the specified dialog 
	---@param self SfDxutDialog
	---@return bool
	---@return int eventId
	---@return int controlId
        pop = function(self) end,
	---SAMPFUNCS
        ---| Adds a button on the DXUTDialog 
	---@param self SfDxutDialog
	---@param id int
	---@param text string
	---@param relCoordX int
	---@param relCoordY int
	---@param width int
	---@param height int
        addButton = function(self, id, text, relCoordX, relCoordY, width, height) end,
	---SAMPFUNCS
        ---| Creates a checkbox on the DxutDialog 
	---@param self SfDxutDialog
	---@param id int
	---@param text string
	---@param relCoordX int
	---@param relCoordY int
	---@param width int
	---@param height int
        addCheckbox = function(self, id, text, relCoordX, relCoordY, width, height) end,
	---SAMPFUNCS
        ---| Sets the coordinates and dimensions of the DxutDialog 
	---@param self SfDxutDialog
	---@param coordX int
	---@param coordY int
	---@param width int
	---@param height int
        setCoordsAndDims = function(self, coordX, coordY, width, height) end,
	---SAMPFUNCS
        ---| Returns the Coordinates and Dimensions of the DxutDialog 
	---@param self SfDxutDialog
	---@return int coordX
	---@return int coordY
	---@return int width
	---@return int height
        getCoordsAndDims = function(self) end,
	---SAMPFUNCS
        ---| Sets the visibility status of the DxutDialog 
	---@param self SfDxutDialog
	---@param isVisible bool
        visibility = function(self, isVisible) end,
	---SAMPFUNCS
        ---| Checks if the DxutDialog is visible 
	---@param self SfDxutDialog
	---@return bool
        isVisible = function(self) end,
	---SAMPFUNCS
        ---| Creates a text input field on a DxutDialog 
	---@param self SfDxutDialog
	---@param id int
	---@param initialText string
	---@param coordX int
	---@param coordY int
	---@param width int
	---@param height int
        addEditbox = function(self, id, initialText, coordX, coordY, width, height) end,
	---SAMPFUNCS
        ---| Returns the text of a control using its ID 
	---@param self SfDxutDialog
	---@param id int
	---@return bool
	---@return int text
        getControlText = function(self, id) end,
	---SAMPFUNCS
        ---| Sets the visibility of the DXUTDialog's control element with ID 
	---@param self SfDxutDialog
	---@param controlId int
	---@param isVisible int
        setVisibilityOfControl = function(self, controlId, isVisible) end,
	---SAMPFUNCS
        ---| Adds a static text control in the DxutDialog 
	---@param self SfDxutDialog
	---@param id int
	---@param text string
	---@param relCoordX int
	---@param relCoordY int
	---@param width int
	---@param height int
        addStaticText = function(self, id, text, relCoordX, relCoordY, width, height) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the checkbox control of the DxutDialog is checked 
	---@param self SfDxutDialog
	---@param checkboxId int
	---@return bool
        isCheckBoxChecked = function(self, checkboxId) end,
	---SAMPFUNCS
        ---| Sets the background color of the DxutDialog 
	---@param self SfDxutDialog
	---@param color int
        setBgColor = function(self, color) end,
	---SAMPFUNCS
        ---| Sets the text of a control using its ID 
	---@param self SfDxutDialog
	---@param id int
	---@param text string
	---@return bool
        setControlText = function(self, id, text) end,
	---SAMPFUNCS
        ---| Evaluates as true if the DxutDialog's control element with ID is visible 
	---@param self SfDxutDialog
	---@param id int
	---@return bool
        isControlVisible = function(self, id) end,
	---SAMPFUNCS
        ---| Creates a horizontal slider on the DxutDialog 
	---@param self SfDxutDialog
	---@param id int
	---@param relCoordX int
	---@param relCoordY int
	---@param width int
	---@param height int
	---@param maxValue int
        addSlider = function(self, id, relCoordX, relCoordY, width, height, maxValue) end,
	---SAMPFUNCS
        ---| Returns the thumb position value of the DxutDialog's slider with ID 
	---@param self SfDxutDialog
	---@param id int
	---@return int value
        getSliderValue = function(self, id) end,
	---SAMPFUNCS
        ---| Sets the thumb position value of the DxutDialog's slider with ID 
	---@param self SfDxutDialog
	---@param id int
	---@param value int
        setSliderValue = function(self, id, value) end,
	---SAMPFUNCS
        ---| Creates a listbox on the dialog 
	---@param self SfDxutDialog
	---@param id int
	---@param relCoordX int
	---@param relCoordY int
	---@param width int
	---@param height int
        addListbox = function(self, id, relCoordX, relCoordY, width, height) end,
	---SAMPFUNCS
        ---| Inserts a new element into the DxutDialog's listbox with ID 
	---@param self SfDxutDialog
	---@param id int
	---@param text int
	---@param data int
	---@param index int
	---@return bool
        insertListboxElement = function(self, id, text, data, index) end,
	---SAMPFUNCS
        ---| Returns the index of the selected element and the count/number of elements in the DxutDialog's listbox with ID 
	---@param self SfDxutDialog
	---@param id int
	---@return int index
	---@return int count
        getSelectedListboxElement = function(self, id) end,
	---SAMPFUNCS
        ---| Removes the element found at the specified index from the DxutDialog's listbox with ID 
	---@param self SfDxutDialog
	---@param id int
	---@param index int
	---@return bool
        deleteListboxElement = function(self, id, index) end,
	---SAMPFUNCS
        ---| Returns the text and data associated with the DxutDialog's listbox element by index 
	---@param self SfDxutDialog
	---@param id int
	---@param index int
	---@return bool
	---@return string text
	---@return int data
        getListboxElement = function(self, id, index) end,
	---SAMPFUNCS
        ---| Sets the status of a checkbox 
	---@param self SfDxutDialog
	---@param id int
	---@param isChecked bool
        setCheckboxStatus = function(self, id, isChecked) end,
	---SAMPFUNCS
        ---| Sets the visibility of the DxutDialog's title 
	---@param self SfDxutDialog
	---@param isVisible bool
        setTitleVisibility = function(self, isVisible) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the DxutDialog's title is visible 
	---@param self SfDxutDialog
	---@return bool
        isTitleVisible = function(self) end,
	---SAMPFUNCS
        ---| Sets the minimized status of the DxutDialog 
	---@param self SfDxutDialog
	---@param isMinimized bool
        setMinimized = function(self, isMinimized) end,
	---SAMPFUNCS
        ---| Returns logical true if the DxutDialog is minimized 
	---@param self SfDxutDialog
	---@return bool
        isMinimized = function(self) end,
	---SAMPFUNCS
        ---| Removes a DxutDialog's control with ID and frees the memory allocated for it 
	---@param self SfDxutDialog
	---@param id int
        deleteControl = function(self, id) end,
	---SAMPFUNCS
        ---| Deletes the DxutDialog and frees the memory allocated for it 
	---@param self SfDxutDialog
        delete = function(self) end,
	---SAMPFUNCS
        ---| Sets the focus of user interaction to a specific DxutDialog control with ID 
	---@param self SfDxutDialog
	---@param id int
        toggleControlFocus = function(self, id) end,
	---SAMPFUNCS
        ---| Changes the dimensions of the DxutDialog control with ID 
	---@param self SfDxutDialog
	---@param id int
	---@param width int
	---@param height int
        setDims = function(self, id, width, height) end,
	---SAMPFUNCS
        ---| Returns the dimensions of the DxutDialog control with ID 
	---@param self SfDxutDialog
	---@param id int
	---@return int width
	---@return int height
        getControlDims = function(self, id) end,
	---SAMPFUNCS
        ---| Sets the window position of the DxutDialog control with ID 
	---@param self SfDxutDialog
	---@param id int
	---@param relCoordX int
	---@param relCoordY int
        setControlCoords = function(self, id, relCoordX, relCoordY) end,
	---SAMPFUNCS
        ---| Returns the window position of a DxutDialog control 
	---@param self SfDxutDialog
	---@param id int
	---@return int coordX
	---@return int coordY
        getControlCoords = function(self, id) end,
	---SAMPFUNCS
        ---| Sets the color of DxutDialog's checkbox control with ID 
	---@param self SfDxutDialog
	---@param id int
	---@param color int
        setCheckboxColor = function(self, id, color) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified DxutDialog exists 
	---@param self SfDxutDialog
	---@return bool
        doesExist = function(self) end,
    }
SfGVarOp = {
        ---SAMPFUNCS
        ---| Sets the value of the custom global variable with name 
	---@param name string
	---@param value any
	---@return bool
        set = function(name, value) end,
	---SAMPFUNCS
        ---| Returns the value of the custom global variable with name 
	---@param name string
	---@return bool
	---@return any value
        get = function(name) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if a custom global variable with specified name existed 
	---@param name string
	---@return bool
        doesExist = function(name) end,
	---SAMPFUNCS
        ---| Sets whether the specified script can read or write to the specified custom global variable 
	---@param gVarName string
	---@param scriptPtr int
	---@param canRead bool
	---@param canWrite bool
	---@return bool
        setScope = function(gVarName, scriptPtr, canRead, canWrite) end,
	---SAMPFUNCS
        ---| Returns the specified script's read/write permissions to the specified custom global variable 
	---@param gVarName int
	---@param scriptPtr int
	---@return bool
	---@return bool canRead
	---@return bool canWrite
        getScope = function(gVarName, scriptPtr) end,
    }
SfGFuncOp = {
        ---SAMPFUNCS
        ---| Registers the name of a custom global function allowing other scripts to call it using SF_CALL_CUSTOM_GLOBAL_FUNCTION command 
	---@param name string
	---@param label label
	---@return bool
        register = function(name, label) end,
	---SAMPFUNCS
        ---| Calls a registered custom global function with name 
	---@param name string
	---@param numArgs int
	---@param args arguments
	---@return bool
        call = function(name, numArgs, args) end,
	---SAMPFUNCS
        ---| Returns the flow of the execution to the next instruction after the custom global function call, optionally returning one or more parameters as result 
	---@param numRet int
	---@param retParams arguments
	---@return bool
        ["return"] = function(numRet, retParams) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified name is registered a custom global function 
	---@param name string
	---@return bool
        isRegistered = function(name) end,
	---SAMPFUNCS
        ---| Returns the origin informations of a custom global function with name 
	---@param name string
	---@return bool
	---@return int hostThreadAddress
	---@return int functionAddress
        getOrigin = function(name) end,
	---SAMPFUNCS
        ---| Removes the currently registered custom global function. Allowing its name to be re-registered by SF_REGISTER_CUSTOM_GLOBAL_FUNCTION command 
	---@param name string
        unregister = function(name) end,
    }
SampTextDrawOp = {
        ---SAMPFUNCS
        ---| Modifies a textdraw with the specified parameters, creating it if it doesn't exist 
	---@param id SampTextDraw
	---@param text string
	---@param coordX float
	---@param coordY float
	---@return bool
        create = function(id, text, coordX, coordY) end,
	---SAMPFUNCS
        ---| Sets the parameters of the "box" (rectangle) of the text draw 
	---@param self SampTextDraw
	---@param isVisible bool
	---@param color int
	---@param width float
	---@param height float
	---@return bool
        setBox = function(self, isVisible, color, width, height) end,
	---SAMPFUNCS
        ---| Sets the alignment of the text in the textdraw 
	---@param self SampTextDraw
	---@param alignment SampTextDrawAlignment
	---@return bool
        setAlignment = function(self, alignment) end,
	---SAMPFUNCS
        ---| Sets whether the text scaling status is proportional to the text draw or not 
	---@param self SampTextDraw
	---@param isProportional bool
	---@return bool
        setProportionality = function(self, isProportional) end,
	---SAMPFUNCS
        ---| Sets the text draw style 
	---@param self SampTextDraw
	---@param style int
	---@return bool
        setStyle = function(self, style) end,
	---SAMPFUNCS
        ---| Sets a shadow on the text draw 
	---@param self SampTextDraw
	---@param tickness int
	---@param color int
	---@return bool
        setShadow = function(self, tickness, color) end,
	---SAMPFUNCS
        ---| Sets the outline of the text draw 
	---@param self SampTextDraw
	---@param tickness int
	---@param color int
	---@return bool
        setOutline = function(self, tickness, color) end,
	---SAMPFUNCS
        ---| Sets the model (object, auto) of the text draw for style 5 
	---@param self SampTextDraw
	---@param model model_any
	---@param rotPitch float
	---@param rotRoll float
	---@param rotYaw float
	---@param camZoomRange float
	---@param primaryColor int
	---@param secondaryColor int
	---@return bool
        setModel = function(self, model, rotPitch, rotRoll, rotYaw, camZoomRange, primaryColor, secondaryColor) end,
	---SAMPFUNCS
        ---| Sets the text of the textdraw 
	---@param self SampTextDraw
	---@param text string
	---@return bool
        setText = function(self, text) end,
	---SAMPFUNCS
        ---| Sets the gamescreen coordinates of the textdraw 
	---@param self SampTextDraw
	---@param coordX float
	---@param coordY float
	---@return bool
        setCoords = function(self, coordX, coordY) end,
	---SAMPFUNCS
        ---| Sets the size and color property of all characters in the textdraw 
	---@param self SampTextDraw
	---@param width float
	---@param height float
	---@param color int
	---@return bool
        setCharProps = function(self, width, height, color) end,
	---SAMPFUNCS
        ---| Returns the parameters of the "box" (rectangle) of the specified text draw 
	---@param self SampTextDraw
	---@return bool
	---@return bool isVisible
	---@return int color
	---@return float width
	---@return float height
        getBox = function(self) end,
	---SAMPFUNCS
        ---| Sets the alignment of the text in the textdraw 
	---@param self SampTextDraw
	---@return bool
	---@return SampTextDrawAlignment alignment
        getAlignment = function(self) end,
	---SAMPFUNCS
        ---| Returns true if the text scaling status is proportional to the text draw 
	---@param self SampTextDraw
	---@return bool
	---@return bool isProportional
        getProportionality = function(self) end,
	---SAMPFUNCS
        ---| Returns the text draw style 
	---@param self SampTextDraw
	---@return bool
	---@return int style
        getStyle = function(self) end,
	---SAMPFUNCS
        ---| Returns the tickness and color property of the text draw's shadow 
	---@param self SampTextDraw
	---@return bool
	---@return int tickness
	---@return int color
        getShadow = function(self) end,
	---SAMPFUNCS
        ---| Returns the tickness and color property of the text draw's outline 
	---@param self SampTextDraw
	---@return bool
	---@return int tickness
	---@return int color
        getOutline = function(self) end,
	---SAMPFUNCS
        ---| Returns the properties of the text draw as a model (style 5) 
	---@param self SampTextDraw
	---@return bool
	---@return model_any model
	---@return float rotPitch
	---@return float rotRoll
	---@return float rotYaw
	---@return float camZoomRange
	---@return int primaryColor
	---@return int secondaryColor
        getModel = function(self) end,
	---SAMPFUNCS
        ---| Stores the textdraw's text to stringBuffer 
	---@param self SampTextDraw
	---@return bool
	---@return string stringBuffer
        storeText = function(self) end,
	---SAMPFUNCS
        ---| Returns the gamescreen coordinates of the textdraw 
	---@param self SampTextDraw
	---@return bool
	---@return float coordX
	---@return float coordY
        getCoords = function(self) end,
	---SAMPFUNCS
        ---| Returns the size and color property of all characters in the textdraw 
	---@param self SampTextDraw
	---@return bool
	---@return float width
	---@return float height
	---@return int color
        getCharProps = function(self) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the specified textdraw exists 
	---@param self SampTextDraw
	---@return bool
        doesExist = function(self) end,
	---SAMPFUNCS
        ---| Deletes the specified textdraw 
	---@param self SampTextDraw
	---@return bool
        delete = function(self) end,
    }
SfDownloadOp = {
        ---SAMPFUNCS
        ---| Downloads a file from the specified url source asynchronosly then saves it to filepath. Returns a handle to this file's SfDownload object for tracking purposes 
	---@param url string
	---@param filePath string
	---@return SfDownload handle
        file = function(url, filePath) end,
	---SAMPFUNCS
        ---| Returns the download status of the SfDownload object 
	---@param self SfDownload
	---@return bool
	---@return SfDownloadState state
        getState = function(self) end,
	---SAMPFUNCS
        ---| Frees an SfDownload object from memory 
	---@param self SfDownload
	---@return bool
        release = function(self) end,
    }
SfScriptOp = {
        ---SAMPFUNCS
        ---| Starts a new script from the specified scriptLabel then stores its pointer at newScriptPtrTo parameter 
	---@param scriptLabel label
	---@param newScriptPtrTo SfScript
	---@param passedValues arguments
        runFromLabel = function(scriptLabel, newScriptPtrTo, passedValues) end,
	---SAMPFUNCS
        ---| Starts a new script from the specified memory location where a script binary data is stored (scriptBin) then stores its pointer at newScriptPtrTo parameter 
	---@param scriptBin int
	---@param newScriptPtrTo SfScript
	---@param passedValues arguments
        runFromPtr = function(scriptBin, newScriptPtrTo, passedValues) end,
	---SAMPFUNCS
        ---| Sets the values for a local variable in the specified script 
	---@param self SfScript
	---@param varIndex int
	---@param value any
        setLVar = function(self, varIndex, value) end,
	---SAMPFUNCS
        ---| Returns the value of a local variable in the specified script 
	---@param self SfScript
	---@param varIndex int
	---@return any value
        getLVar = function(self, varIndex) end,
	---SAMPFUNCS
        ---| Terminates a script pointed by the address 
	---@param self SfScript
        terminate = function(self) end,
	---SAMPFUNCS
        ---| Restarts a script pointed by the address 
	---@param self SfScript
	---@param passedValues arguments
        restart = function(self, passedValues) end,
    }
SfTimerOp = {
        ---SAMPFUNCS
        ---| Creates and starts an SfTimer object that periodically executes its callback upon expiration 
	---@param interval int
	---@param callback label
	---@return bool
	---@return SfTimer handle
        create = function(interval, callback) end,
	---SAMPFUNCS
        ---| Removes the specified SfTimer object, freeing it from memory 
	---@param self SfTimer
	---@return bool
        delete = function(self) end,
	---SAMPFUNCS
        ---| Resets the expiration time of the SfTimer, restarting its trigger countdown 
	---@param self SfTimer
	---@return bool
        reset = function(self) end,
	---SAMPFUNCS
        ---| Sets a new interval for the SfTimer 
	---@param self SfTimer
	---@param interval int
	---@return bool
        setInterval = function(self, interval) end,
	---SAMPFUNCS
        ---| Activates or pauses an SfTimer's functionality 
	---@param self SfTimer
	---@param isActive bool
	---@return bool
        setStatus = function(self, isActive) end,
	---SAMPFUNCS
        ---| Evaluates as logical true if the SfTimer is active 
	---@param self SfTimer
	---@return bool
        isActive = function(self) end,
	---SAMPFUNCS
        ---| Returns the SfTimer's configured interval 
	---@param self SfTimer
	---@return bool
	---@return int interval
        getInterval = function(self) end,
	---SAMPFUNCS
        ---| Returns the number of milliseconds that have passed since the last SfTimer reset (including after the interval has passed) 
	---@param self SfTimer
	---@return bool
	---@return int elapsedTime
        getElapsedTime = function(self) end,
	---SAMPFUNCS
        ---| Returns the number of milliseconds remaining before the SfTimer expires (decremented if the timer is active) 
	---@param self SfTimer
	---@return bool
	---@return int timeLeft
        getTimeLeft = function(self) end,
    }
AudioStream3DOp = {
        ---audio
        ---| Loads an audio file and creates a new 3d audio stream (see 0AAC) 
	---@param audioFileName string
	---@return bool
	---@return AudioStream3D handle
        load = function(audioFileName) end,
	---audio
        ---| Sets sound source position to specific world location 
	---@param self AudioStream3D
	---@param x float
	---@param y float
	---@param z float
        setPlayAtCoords = function(self, x, y, z) end,
	---audio
        ---| Attaches sound source to the object 
	---@param self AudioStream3D
	---@param object Object
        setPlayAtObject = function(self, object) end,
	---audio
        ---| Attaches sound source to the character 
	---@param self AudioStream3D
	---@param char Char
        setPlayAtChar = function(self, char) end,
	---audio
        ---| Attaches sound source to the vehicle 
	---@param self AudioStream3D
	---@param car Car
        setPlayAtCar = function(self, car) end,
	---audio
        ---| Sets size of 3d audio stream sound source in the world. Volume will not decay within the specified distance 
	---@param self AudioStream3D
	---@param radius float
        setSourceSize = function(self, radius) end,
    }
ImGuiOp = {
        ---imgui
        ---| Creates a unique frame with its own space in memory. Must be enclosed with IMGUI_END_FRAME 
	---@param uniqueId string
        beginFrame = function(uniqueId) end,
	---imgui
        ---| Ends unique ImGui frame created with IMGUI_BEGIN_FRAME 
        endFrame = function() end,
	---imgui
        ---| Creates the window 
	---@param windowName string
	---@param state bool
	---@param noTitleBar bool
	---@param noResize bool
	---@param noMove bool
	---@param autoResize bool
	---@return bool state
        begin = function(windowName, state, noTitleBar, noResize, noMove, autoResize) end,
	---imgui
        ---| Ends the window 
        ["end"] = function() end,
	---imgui
        ---| Creates the main menu bar 
	---@param uniqueId string
        beginMainMenuBar = function(uniqueId) end,
	---imgui
        ---| Ends the main menu bar 
        endMainMenuBar = function() end,
	---imgui
        ---| Creates a child window widget inside the main window 
	---@param uniqueId string
        beginChild = function(uniqueId) end,
	---imgui
        ---| Ends the child window widget created with 0C25 
        endChild = function() end,
	---imgui
        ---| Pass tab names separated by comma. Returns the index of the visible tab 
	---@param name string
	---@param tabNames string
	---@return int index
        tabs = function(name, tabNames) end,
	---imgui
        ---| Adds the collapsing header 
	---@param label string
	---@return bool
        collapsingHeader = function(label) end,
	---imgui
        ---| Sets the current window position. Must be called inside Begin()...End() 
	---@param x float
	---@param y float
	---@param imGuiCond ImGuiCond
        setWindowPos = function(x, y, imGuiCond) end,
	---imgui
        ---| Sets the current window size. Must be called inside Begin()...End() 
	---@param width float
	---@param height float
	---@param imGuiCond ImGuiCond
        setWindowSize = function(width, height, imGuiCond) end,
	---imgui
        ---| Sets the current window position. Applies to the next window ( aka Begin() ) 
	---@param x float
	---@param y float
	---@param imGuiCond ImGuiCond
        setNextWindowPos = function(x, y, imGuiCond) end,
	---imgui
        ---| Sets the current window size. Applies to the next window ( aka Begin() ) 
	---@param width float
	---@param height float
	---@param imGuiCond ImGuiCond
        setNextWindowSize = function(width, height, imGuiCond) end,
	---imgui
        ---| Creates the text line 
	---@param text string
        text = function(text) end,
	---imgui
        ---| Displays a center aligned ImGui text widget 
	---@param text string
        textCentered = function(text) end,
	---imgui
        ---| Creates the text line with the disabled color ( Grayish by default ) 
	---@param text string
        textDisabled = function(text) end,
	---imgui
        ---| Creates the text line that wraps to a newline if the text goes beyond the window width 
	---@param text string
        textWrapped = function(text) end,
	---imgui
        ---| Creates the text line of the given RGBA color (0.0f-1.0f) 
	---@param text string
	---@param red float
	---@param green float
	---@param blue float
	---@param alpha float
        textColored = function(text, red, green, blue, alpha) end,
	---imgui
        ---| Creates the text line with a bullet point 
	---@param text string
        textWithBullet = function(text) end,
	---imgui
        ---| Creates a bullet point 
        bullet = function() end,
	---imgui
        ---| Creates the checkbox 
	---@param label string
	---@param isChecked bool
	---@return bool state
        checkbox = function(label, isChecked) end,
	---imgui
        ---| Creates a combo box widget. Pass options separated by commas "item1,item2,item3" 
	---@param name string
	---@param options string
	---@param selection int
	---@return int selected
        comboBox = function(name, options, selection) end,
	---imgui
        ---| Creates the popup window with the given text 
	---@param text string
        setTooltip = function(text) end,
	---imgui
        ---| Creates the button 
	---@param buttonName string
	---@param width float
	---@param height float
	---@return bool
        button = function(buttonName, width, height) end,
	---imgui
        ---| Creates a ImGui button with specified image 
	---@param name string
	---@param image int
	---@param width float
	---@param height float
	---@return bool
        buttonImage = function(name, image, width, height) end,
	---imgui
        ---| Creates the invisible button 
	---@param buttonName string
	---@param width float
	---@param height float
	---@return bool
        buttonInvisible = function(buttonName, width, height) end,
	---imgui
        ---| Creates the button with custom colors 
	---@param buttonName string
	---@param red float
	---@param green float
	---@param blue float
	---@param alpha float
	---@param width float
	---@param height float
	---@return bool
        buttonColored = function(buttonName, red, green, blue, alpha, width, height) end,
	---imgui
        ---| Creates the arrow button in the specified direction 
	---@param name string
	---@param imGuiDir ImGuiDir
	---@return bool
        buttonArrow = function(name, imGuiDir) end,
	---imgui
        ---| Creates the int slider input 
	---@param label string
	---@param initValue int
	---@param min int
	---@param max int
	---@return int val
        sliderInt = function(label, initValue, min, max) end,
	---imgui
        ---| Creates the float slider input 
	---@param label string
	---@param initValue float
	---@param min float
	---@param max float
	---@return float val
        sliderFloat = function(label, initValue, min, max) end,
	---imgui
        ---| Creates the int input 
	---@param label string
	---@param initValue int
	---@param min int
	---@param max int
	---@return int val
        inputInt = function(label, initValue, min, max) end,
	---imgui
        ---| Creates the float input 
	---@param label string
	---@param initValue float
	---@param min float
	---@param max float
	---@return float val
        inputFloat = function(label, initValue, min, max) end,
	---imgui
        ---| Creates the text input 
	---@param label string
	---@return string text
        inputText = function(label) end,
	---imgui
        ---| Creates the radio button 
	---@param label string
	---@param selectedBtn int
	---@param btnNo int
	---@return int val
        radioButton = function(label, selectedBtn, btnNo) end,
	---imgui
        ---| Creates the color picker and sets the default color (0-255) 
	---@param label string
	---@return int red
	---@return int green
	---@return int blue
	---@return int alpha
        colorPicker = function(label) end,
	---imgui
        ---| Adds the menu item 
	---@param text string
	---@param selected bool
	---@param enabled bool
	---@return bool
        menuItem = function(text, selected, enabled) end,
	---imgui
        ---| Adds the selectable widget 
	---@param text string
	---@param selected bool
	---@return bool
        selectable = function(text, selected) end,
	---imgui
        ---| Creates the dummy widget. Used for spacing 
	---@param width float
	---@param height float
        dummy = function(width, height) end,
	---imgui
        ---| Appends the next widget to the same line as the previous widget 
        sameLine = function() end,
	---imgui
        ---| Creates a new line for the next widget 
        newLine = function() end,
	---imgui
        ---| Divides the window width into N columns. Close this with Columns(1) 
	---@param count int
        columns = function(count) end,
	---imgui
        ---| Puts the next widgets on the next column. Used alongside 0C16 
        nextColumn = function() end,
	---imgui
        ---| Adds some spacing after the previous widget 
        spacing = function() end,
	---imgui
        ---| Adds a horizontal separator line 
        separator = function() end,
	---imgui
        ---| Sets the item width for the next widgets 
	---@param width float
        pushItemWidth = function(width) end,
	---imgui
        ---| Removes the pushed item width (0C27) from the stack 
        popItemWidth = function() end,
	---imgui
        ---| Returns true if the previous widget is in active state 
	---@param uniqueId string
	---@return bool
        isItemActive = function(uniqueId) end,
	---imgui
        ---| Returns true if the previous widget is clicked 
	---@param uniqueId string
	---@return bool
        isItemClicked = function(uniqueId) end,
	---imgui
        ---| Returns true if the previous widget is focused 
	---@param uniqueId string
	---@return bool
        isItemFocused = function(uniqueId) end,
	---imgui
        ---| Returns true if the previous widget is hovered with mouse 
	---@param uniqueId string
	---@return bool
        isItemHovered = function(uniqueId) end,
	---imgui
        ---| Sets the value of input int & slider int widget 
	---@param id string
	---@param val int
        setItemValueInt = function(id, val) end,
	---imgui
        ---| Sets the value of input float & slider float widget 
	---@param id string
	---@param val float
        setItemValueFloat = function(id, val) end,
	---imgui
        ---| Sets value of input text widget 
	---@param id string
	---@param val string
        setItemValueText = function(id, val) end,
	---imgui
        ---| Sets image background color 
	---@param r float
	---@param g float
	---@param b float
	---@param a float
        setImageBgColor = function(r, g, b, a) end,
	---imgui
        ---| Sets image tint color 
	---@param r float
	---@param g float
	---@param b float
	---@param a float
        setImageTintColor = function(r, g, b, a) end,
	---imgui
        ---| Loads a image file from disk. Relative to CLEO directory 
	---@param path string
	---@return int image
        loadImage = function(path) end,
	---imgui
        ---| Frees a loaded image data 
	---@param image int
        freeImage = function(image) end,
	---imgui
        ---| Pushes a ImGuiStyleVar value to the stack. Use PopStyleVar to undo the effect 
	---@param imGuiStyleVar ImGuiStyleVar
	---@param val float
        pushStyleVar = function(imGuiStyleVar, val) end,
	---imgui
        ---| Pushes a ImGuiStyleVar value to the stack. Use PopStyleVar to undo the effect 
	---@param imGuiStyleVar ImGuiStyleVar
	---@param x float
	---@param y float
        pushStyleVar2 = function(imGuiStyleVar, x, y) end,
	---imgui
        ---| Pushes a ImGuiCol value to the stack. Use PopStyleColor to undo the effect 
	---@param imGuiCol ImGuiCol
	---@param r int
	---@param g int
	---@param b int
	---@param a int
        pushStyleColor = function(imGuiCol, r, g, b, a) end,
	---imgui
        ---| Removes the recent imGuiStyleVar from the stack 
	---@param count int
        popStyleVar = function(count) end,
	---imgui
        ---| Removes the recent ImGuiCol from the stack 
	---@param count int
        popStyleColor = function(count) end,
	---imgui
        ---| Returns pointer to foreground draw list 
	---@return int drawList
        getForegroundDrawList = function() end,
	---imgui
        ---| Returns pointer to ImGui background drawlist 
	---@return int drawList
        getBackgroundDrawList = function() end,
	---imgui
        ---| Returns pointer to ImGui window drawList 
	---@return int drawList
        getWindowDrawlist = function() end,
	---imgui
        ---| Adds text at specified position 
	---@param drawList int
	---@param posX float
	---@param posY float
	---@param r int
	---@param g int
	---@param b int
	---@param a int
	---@param text string
        addText = function(drawList, posX, posY, r, g, b, a, text) end,
	---imgui
        ---| Adds a line form point A to B 
	---@param drawList int
	---@param p1X float
	---@param p1Y float
	---@param p2X float
	---@param p2Y float
	---@param r int
	---@param g int
	---@param b int
	---@param a int
	---@param thickness float
        addLine = function(drawList, p1X, p1Y, p2X, p2Y, r, g, b, a, thickness) end,
	---imgui
        ---| Returns the ImGui version 
	---@return int version
        getVersion = function() end,
	---imgui
        ---| Returns the ImGuiRedux version 
	---@return int version
        getPluginVersion = function() end,
	---imgui
        ---| Toggles the cursor 
	---@param show bool
        setCursorVisible = function(show) end,
	---imgui
        ---| Returns the ImGui frame height 
	---@return float height
        getFrameHeight = function() end,
	---imgui
        ---| Returns the x,y coordinates of the window on the screen 
	---@param uniqueId string
	---@return float x
	---@return float y
        getWindowPos = function(uniqueId) end,
	---imgui
        ---| Returns the width and height of the window 
	---@param uniqueId string
	---@return float width
	---@return float height
        getWindowSize = function(uniqueId) end,
	---imgui
        ---| Returns the width and height of the given text 
	---@param text string
	---@return float width
	---@return float height
        calcTextSize = function(text) end,
	---imgui
        ---| Returns the content region width of the window 
	---@param uniqueId string
	---@return float width
        getWindowContentRegionWidth = function(uniqueId) end,
	---imgui
        ---| Returns the width and height scaling factor based on the window size 
	---@param uniqueId string
	---@param count int
	---@param spacing bool
	---@return float x
	---@return float y
        getScalingSize = function(uniqueId, count, spacing) end,
	---imgui
        ---| Returns the width & height of the display 
	---@return float width
	---@return float height
        getDisplaySize = function() end,
	---imgui
        ---| Sets the background transparency of next window (0.0f-1.0f) 
	---@param alpha float
        setNextWindowTransparency = function(alpha) end,
	---imgui
        ---| Displays a text message on top left corner of the screen. Useful for games without `showTextBox(...)` support 
	---@param text string
        setMessage = function(text) end,
	---imgui
        ---| Sets the width of the column 
	---@param index int
	---@param width float
        setColumnWidth = function(index, width) end,
	---imgui
        ---| Creates a child window widget inside the main window 
	---@param uniqueId string
	---@param width float
	---@param height float
	---@param border bool
	---@param flags int
        beginChildEx = function(uniqueId, width, height, border, flags) end,
	---imgui
        ---| Disables ImGui widgets inside this block 
	---@param disabled bool
        beginDisabled = function(disabled) end,
	---imgui
        ---| Closes the ImGui disable block 
        endDisabled = function() end,
	---imgui
        ---| Begins a ImGui menu block 
	---@param label string
	---@param enabled bool
	---@return bool
        beginMenu = function(label, enabled) end,
	---imgui
        ---| Ends a ImGui menu block 
        endMenu = function() end,
    }
IniFileOp = {
        ---ini
        ---| Reads an integer value from the ini file 
	---@param path string
	---@param section string
	---@param key string
	---@return bool
	---@return int value
        readInt = function(path, section, key) end,
	---ini
        ---| Writes the integer value to the ini file 
	---@param value int
	---@param path string
	---@param section string
	---@param key string
	---@return bool
        writeInt = function(value, path, section, key) end,
	---ini
        ---| Reads a floating-point value from the ini file 
	---@param path string
	---@param section string
	---@param key string
	---@return bool
	---@return float value
        readFloat = function(path, section, key) end,
	---ini
        ---| Writes the floating-point value to the ini file 
	---@param value float
	---@param path string
	---@param section string
	---@param key string
	---@return bool
        writeFloat = function(value, path, section, key) end,
	---ini
        ---| Reads a string value from the ini file 
	---@param path string
	---@param section string
	---@param key string
	---@return bool
	---@return string value
        readString = function(path, section, key) end,
	---ini
        ---| Writes the string value to the ini file 
	---@param value string
	---@param path string
	---@param section string
	---@param key string
	---@return bool
        writeString = function(value, path, section, key) end,
	---ini
        ---| Deletes the section from the ini file 
	---@param path string
	---@param section string
	---@return bool
        deleteSection = function(path, section) end,
	---ini
        ---| Deletes the key from the ini file 
	---@param path string
	---@param section string
	---@param key string
	---@return bool
        deleteKey = function(path, section, key) end,
    }