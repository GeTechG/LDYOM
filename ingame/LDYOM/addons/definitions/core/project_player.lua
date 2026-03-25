---@meta

-- ─── ProjectPlayer ────────────────────────────────────────────────────────────

---Mission execution engine. Access via the `project_player` singleton.
---@class ProjectPlayer
---@field is_playing boolean Whether a project is currently playing (read-only).
---@field current_scene_id string UUID string of the currently active scene (read-only).
---@field current_objective_index integer 0-based index of the currently running objective (read-only).
---@field is_faded boolean Whether the screen is faded to black. Read-write.
local ProjectPlayer = {}

---Starts the current project, optionally from a specific scene by UUID string.
---@param sceneId? string Scene UUID to start from. Omit to use the default start scene.
function ProjectPlayer:start(sceneId) end

---Stops the currently playing project immediately.
function ProjectPlayer:stop() end

---Fails the currently playing project (triggers the fail flow).
function ProjectPlayer:fail() end

---Requests a scene transition to the given scene UUID.
---The transition is queued and executed at the next safe opportunity.
---@param sceneId string Target scene UUID string.
function ProjectPlayer:request_scene_transition(sceneId) end

---Returns all entities spawned in the current scene.
---@return Entity[]
function ProjectPlayer:get_entities() end

---Emits a signal by name. Any `wait_signal` objective or node listening for
---this name will be unblocked immediately.
---@param signal string The signal name to emit.
function ProjectPlayer:send_signal(signal) end

---Global singleton for controlling mission playback.
---@type ProjectPlayer
project_player = {}

-- ─── events ───────────────────────────────────────────────────────────────────

---Event subscription table. Each function subscribes a callback and returns
---a disconnect function that cancels the subscription when called.
---@class events
events = {}

---Subscribe to the project-started event.
---Called once when the project begins playing.
---@param callback fun() Callback with no arguments.
---@return fun() disconnect Call to unsubscribe.
function events.on_project_started(callback) end

---Subscribe to the project-stopped event.
---Called once when the project stops (finish, fail, or manual stop).
---@param callback fun() Callback with no arguments.
---@return fun() disconnect Call to unsubscribe.
function events.on_project_stopped(callback) end

---Subscribe to the scene-started event.
---Called each time a new scene begins (including transitions).
---@param callback fun(sceneId: string) Callback receiving the scene UUID string.
---@return fun() disconnect Call to unsubscribe.
function events.on_scene_started(callback) end

---Subscribe to the scene-ended event.
---Called just before a scene ends — either before a transition to another scene or before the project finishes.
---@param callback fun(sceneId: string) Callback receiving the UUID string of the scene that just ended.
---@return fun() disconnect Call to unsubscribe.
function events.on_scene_ended(callback) end

---Subscribe to the objective-started event.
---Called when each objective begins execution.
---@param callback fun(index: integer) Callback receiving the 0-based objective index.
---@return fun() disconnect Call to unsubscribe.
function events.on_objective_started(callback) end

---Subscribe to the objective-completed event.
---Called when an objective finishes successfully.
---@param callback fun(index: integer) Callback receiving the 0-based objective index.
---@return fun() disconnect Call to unsubscribe.
function events.on_objective_completed(callback) end
