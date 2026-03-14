--- @meta

-- ─── LuaNodeHandle ───────────────────────────────────────────────────────────

--- Per-node proxy object passed to on_init / on_draw / on_execute callbacks.
--- @class LuaNodeHandle
--- @field typeKey string The node type key (read-only).
local LuaNodeHandle = {}

--- Read the value from an input pin by 1-based index.
--- Returns nil for out-of-range indices or flow pins.
--- @param index integer 1-based pin index
--- @return any
function LuaNodeHandle:getInput(index) end

--- Get the default value of an unconnected input pin by 1-based index.
--- @param index integer 1-based pin index
--- @return any
function LuaNodeHandle:getInputDefault(index) end

--- Set the default value of an unconnected input pin by 1-based index.
--- @param index integer 1-based pin index
--- @param value any
function LuaNodeHandle:setInputDefault(index, value) end

--- Returns true if the input pin at the given 1-based index is connected.
--- @param index integer 1-based pin index
--- @return boolean
function LuaNodeHandle:isInputConnected(index) end

--- Returns true if the output pin at the given 1-based index is connected.
--- @param index integer 1-based pin index
--- @return boolean
function LuaNodeHandle:isOutputConnected(index) end

--- Get a persistent (serialized) value by key.
--- @param key string
--- @return any
function LuaNodeHandle:getData(key) end

--- Set a persistent (serialized) value by key.
--- @param key string
--- @param value any
function LuaNodeHandle:setData(key, value) end

--- Get the cached output value at the given 1-based output pin index.
--- @param index integer 1-based pin index
--- @return any
function LuaNodeHandle:getOutput(index) end

--- Set the cached output value at the given 1-based output pin index.
--- @param index integer 1-based pin index
--- @param value any
function LuaNodeHandle:setOutput(index, value) end

-- ─── LuaNode ─────────────────────────────────────────────────────────────────

--- Lightweight reference to a node in the graph.
--- WARNING: do NOT store this value past the node's lifetime in the graph.
--- @class LuaNode
--- @field uid integer The node's unique identifier (NodeUID).
--- @field type string The node type key (e.g. "my_addon.delay").
--- @field handle LuaNodeHandle The node's Lua handle (shared ownership; safe to store).
local LuaNode = {}

-- ─── node_editor ─────────────────────────────────────────────────────────────

--- @class node_editor
node_editor = {}

--- Register a node type from a descriptor table.
--- @param descriptor table
--- @return boolean
function node_editor.register(descriptor) end

--- Register a node type lazily (loaded from a Lua file on first use).
--- @param descriptor table  Must contain `type`, `category`, `path` fields.
--- @return boolean
function node_editor.register_lazy(descriptor) end

--- Register a named visual style for node headers.
--- Descriptor fields: key (string), icon (string, optional), header_bg ({r,g,b,a}),
--- title_color ({r,g,b,a}, optional), radius (number, optional).
--- @param descriptor table
function node_editor.register_style(descriptor) end

--- Returns a flat list of all LuaNode entries across all workspaces.
--- Each entry is a table: { uid = integer, type = string, workspace = integer }.
--- @return { uid: integer, type: string, workspace: integer }[]
function node_editor.get_scene_nodes() end

--- Returns the LuaNode usertype for the given UID, or nil if not found.
--- Provides OOP access: node.uid, node.type, node.handle.
--- WARNING: do NOT store past the node's lifetime.
--- @param uid integer NodeUID
--- @return LuaNode|nil
function node_editor.get_node(uid) end

--- Returns the LuaNodeHandle for the node with the given UID, or nil.
--- The handle is reference-counted and remains valid even after node removal.
--- @param uid integer NodeUID
--- @return LuaNodeHandle|nil
function node_editor.get_node_handle(uid) end

--- Returns the type string for the node with the given UID, or nil.
--- @param uid integer NodeUID
--- @return string|nil
function node_editor.get_node_type(uid) end

--- Returns the on_execute function for the node with the given UID, or nil.
--- @param uid integer NodeUID
--- @return function|nil
function node_editor.get_node_execute_fn(uid) end

--- Invalidates the evaluation cache of all pure nodes.
--- Call this once at the start of each flow-node execution step.
function node_editor.bump_pure_generation() end

--- Follows the output flow pin at pinIndex (0-based) on the given node.
--- Returns the UID of the connected node, or nil if not connected.
--- @param uid integer NodeUID
--- @param pinIndex integer 0-based output pin index
--- @return integer|nil
function node_editor.get_next_flow_node(uid, pinIndex) end

--- Executes the full node-flow chain starting from startUID.
--- Must be called from a coroutine context (supports yielding execute functions).
--- @param startUID integer NodeUID
function node_editor.run_flow_from(startUID) end

--- Returns the number of workspaces in the node editor.
--- @return integer
function node_editor.get_workspace_count() end

--- Returns an ordered list of workspace names.
--- @return string[]
function node_editor.get_workspace_names() end

--- Saves all workspaces to a JSON file at the given path.
--- @param path string
function node_editor.save_graph(path) end

--- Loads workspaces from a JSON file at the given path.
--- @param path string
function node_editor.load_graph(path) end

--- Clears the active workspace's graph.
function node_editor.clear_graph() end
