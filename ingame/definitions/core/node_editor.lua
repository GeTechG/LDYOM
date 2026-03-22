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

--- Get the semantic type of a static input pin by 1-based index.
--- Returns "" for flow pins or out-of-range indices.
--- @param index integer 1-based pin index
--- @return string
function LuaNodeHandle:getInputPinType(index) end

--- Get the semantic type of a static output pin by 1-based index.
--- Returns "" for flow pins or out-of-range indices.
--- @param index integer 1-based pin index
--- @return string
function LuaNodeHandle:getOutputPinType(index) end

--- Change the semantic type of a static input pin by 1-based index.
--- Drops the existing connection if the type changes. Updates pin style automatically.
--- type must be one of: "float", "int", "bool", "string".
--- Call from on_draw to apply a type stored in nodeData.
--- @param index integer 1-based pin index
--- @param type "float"|"int"|"bool"|"string"
function LuaNodeHandle:setInputPinType(index, type) end

--- Change the semantic type of a static output pin by 1-based index.
--- Drops all connected input links if the type changes. Updates pin style automatically.
--- type must be one of: "float", "int", "bool", "string".
--- Call from on_draw to apply a type stored in nodeData.
--- @param index integer 1-based pin index
--- @param type "float"|"int"|"bool"|"string"
function LuaNodeHandle:setOutputPinType(index, type) end

-- ─── LuaNode ─────────────────────────────────────────────────────────────────

--- Lightweight reference to a node in the graph.
--- WARNING: do NOT store this value past the node's lifetime in the graph.
--- @class LuaNode
--- @field uid integer The node's unique identifier (NodeUID).
--- @field type string The node type key (e.g. "my_addon.delay").
--- @field handle LuaNodeHandle The node's Lua handle (shared ownership; safe to store).
local LuaNode = {}

-- ─── NodePinDescriptor ───────────────────────────────────────────────────────

--- Describes a single pin on a node.
--- @class NodePinDescriptor
--- @field title string Display name of the pin. Required.
--- @field type? "flow"|"number"|"bool"|"string" Semantic pin type. Defaults to "number".
--- @field dir? "in"|"out" Pin direction. Defaults to "in".
--- @field default? any Default value for unconnected input pins.
--- @field behaviour? fun(handle: LuaNodeHandle): any Out-pin value producer. Called each time the pin value is pulled (pure/value nodes).
--- @field on_render? fun(handle: LuaNodeHandle) Optional custom ImGui renderer drawn in place of the pin's default widget.

-- ─── NodeDescriptor ──────────────────────────────────────────────────────────

--- Full descriptor table passed to node_editor.register().
--- @class NodeDescriptor
--- @field type string Unique type key for this node, e.g. "my_addon.delay". Required.
--- @field category? string Category shown in the Add-Node menu. Defaults to "Uncategorized".
--- @field style? "event"|"function"|"loop"|"variable"|"branch"|string Semantic style key registered via node_editor.register_style(). Defaults to "function".
--- @field is_pure? boolean If true, the node has no flow pins and is evaluated lazily (pull-based) with per-step caching. Defaults to false.
--- @field pins? NodePinDescriptor[] Ordered list of pin descriptors (inputs first, then outputs).
--- @field default_data? table Initial key/value pairs copied into nodeData when the node is first created.
--- @field on_init? fun(handle: LuaNodeHandle) Called once after the node is spawned in the graph.
--- @field on_draw? fun(handle: LuaNodeHandle) Called every frame to render custom ImGui content inside the node body.
--- @field on_execute? fun(handle: LuaNodeHandle): any Called by the flow executor when the node's input flow pin fires.

-- ─── NodeStyleDescriptor ─────────────────────────────────────────────────────

--- Descriptor table passed to node_editor.register_style().
--- @class NodeStyleDescriptor
--- @field key string Unique style key used in NodeDescriptor.style. Required.
--- @field header_bg integer[] RGBA color table {r, g, b, a} (0-255) for the node header background. Required.
--- @field title_color? integer[] RGBA color table {r, g, b, a} for the title text. Defaults to white.
--- @field icon? string UTF-8 FontAwesome glyph string (e.g. ICON_FA_BOLT). Optional.
--- @field radius? number Corner radius of the node header in pixels. Defaults to 6.5.

-- ─── LazyNodeDescriptor ──────────────────────────────────────────────────────

--- Descriptor table passed to node_editor.register_lazy().
--- @class LazyNodeDescriptor
--- @field type string Unique type key for this node. Required.
--- @field path string Absolute path to the Lua file that registers the full NodeDescriptor on first use. Required.
--- @field category? string Category shown in the Add-Node menu. Defaults to "Uncategorized".

-- ─── node_editor ─────────────────────────────────────────────────────────────

--- @class node_editor
node_editor = {}

--- Register a node type from a full descriptor table.
--- @param descriptor NodeDescriptor
--- @return boolean success Returns false if the descriptor is missing required fields.
function node_editor.register(descriptor) end

--- Register a node type lazily.
--- The Lua file at descriptor.path is executed only the first time the node type is needed.
--- @param descriptor LazyNodeDescriptor
--- @return boolean success Returns false if type or path fields are missing.
function node_editor.register_lazy(descriptor) end

--- Register a named visual style for node headers.
--- @param descriptor NodeStyleDescriptor
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
--- Returns a table with UIDs of ALL connected nodes (empty table if none).
--- @param uid integer NodeUID
--- @param pinIndex integer 0-based output pin index
--- @return integer[]
function node_editor.get_next_flow_nodes(uid, pinIndex) end

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
