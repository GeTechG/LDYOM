---@meta

-- ─── CarrecPathInfo ───────────────────────────────────────────────────────────

---Info about a single carrec (vehicle recording) path.
---@class CarrecPathInfo
---@field name string Display name of the path.
---@field uuid string UUID string identifier.
---@field frame_count integer Number of recorded frames.
---@field duration number Duration of the recording in seconds.
local CarrecPathInfo = {}

-- ─── carrec_paths ─────────────────────────────────────────────────────────────

---API for playing back vehicle carrec (CarRec) paths on GTA vehicles.
---Vehicle refs are standard GTA script handles (integers).
---@class carrec_paths
carrec_paths = {}

---Returns a list of all available carrec paths.
---@return CarrecPathInfo[]
function carrec_paths.get_paths() end

---Starts carrec playback on a vehicle.
---`path_uuid_or_name` can be a UUID string or the display name of the path.
---Returns `true` on success.
---@param vehicle_ref integer GTA script handle of the vehicle.
---@param path_uuid_or_name string UUID or display name of the carrec path.
---@param use_ai boolean? Whether to use car AI during playback (default: false).
---@param looped boolean? Whether to loop the playback (default: false).
---@return boolean
function carrec_paths.start_playback(vehicle_ref, path_uuid_or_name, use_ai, looped) end

---Stops carrec playback on a vehicle.
---@param vehicle_ref integer GTA script handle of the vehicle.
function carrec_paths.stop_playback(vehicle_ref) end

---Returns `true` if the vehicle currently has an active carrec playback.
---@param vehicle_ref integer GTA script handle of the vehicle.
---@return boolean
function carrec_paths.is_playing(vehicle_ref) end
