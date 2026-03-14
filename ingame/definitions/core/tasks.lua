---@meta

---Coroutine-based task manager. Both `tasks` and `node_tasks` are instances of this class.
---@class TaskManager
local TaskManager = {}

---Starts a new coroutine task identified by `key`.
---The function `f` is wrapped in a coroutine and called with the provided arguments.
---Returns false if a task with the same key is already running or pending.
---@param key string Unique identifier for the task
---@param f function The function to run as a coroutine
---@param ... any Arguments forwarded to `f`
---@return boolean success
function TaskManager.run(key, f, ...) end

---Returns whether a task with the given key is currently running or pending.
---@param key string Task identifier
---@return boolean
function TaskManager.is_running(key) end

---Cancels a running or pending task by key.
---Returns false if no task with that key exists.
---@param key string Task identifier
---@return boolean success
function TaskManager.cancel(key) end

---Suspends the current task for the specified number of milliseconds.
---Must only be called from inside a task coroutine (via `run`).
---@param ms number Duration to sleep in milliseconds
function TaskManager.sleep(ms) end

---Suspends the current task until the task identified by `key` finishes.
---Resumes immediately if the key is not found (already completed or never started).
---Must only be called from inside a task coroutine (via `run`).
---@param key string Key of the task to wait for
function TaskManager.wait_for(key) end

---Suspends the current task for one frame, then resumes.
---Must only be called from inside a task coroutine (via `run`).
function TaskManager.yield() end

---Global task manager for scene/project scripts.
---@type TaskManager
tasks = {}

---Task manager scoped to the node editor.
---@type TaskManager
node_tasks = {}
