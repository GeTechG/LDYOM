manager = {}
--debtab = require 'debug_table'
json = require 'ldyom.json'
lzw = require 'ldyom.lualzw'
local path = getWorkingDirectory() .. "\\Missions_pack\\"
local path_s = getWorkingDirectory() .. "\\Storylines\\"
local path_backup = getWorkingDirectory() .. "\\Backup\\"
cyr = encoding.CP1251
u8 = encoding.UTF8

local CountMPacks = 0

function manager.save(pack)
	createDirectory(path_backup)
	local path_pack = ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1])..'\\'
	local name_miss = ffi.string(vr.temp_var.list_name_missions[vr.current_mission_pack[0]+1][vr.current_mission[0]+1])
	local old_save
	if doesFileExist(path..path_pack..name_miss..'.bin') then
		local f = io.open(path..path_pack..name_miss..'.bin',"rb")
		old_save = f:read("*all")
		f:close()
		local f = io.open(path_backup .. name_miss .. '_' .. os.time() .. '.bin',"wb")
		f:write(old_save)
		f:close()
	end
	local f = io.open(path..path_pack..name_miss..'.bin',"wb")
	local save = lzw.compress(bitser.dumps(pack))
	f:write(save)
	f:close()
end

function manager.save_s(story)
	createDirectory(path_backup)
	local name_story = ffi.string(vr.temp_var.list_name_storylines[vr.current_storyline[0]+1])
	local old_save
	if doesFileExist(path_s .. name_story .. '.bin') then
		local f = io.open(path_s .. name_story .. '.bin',"rb")
		old_save = f:read("*all")
		f:close()
		local f = io.open(path_backup .. name_story .. '_' .. os.time() .. '.bin',"wb")
		f:write(old_save)
		f:close()
	end
	local f = io.open(path_s .. name_story .. '.bin',"wb")
	local save = lzw.compress(bitser.dumps(story))
	f:write(save)
	f:close()
end

function manager.load()
	local path_pack = ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1])..'\\'
	local name_miss = ffi.string(vr.temp_var.list_name_missions[vr.current_mission_pack[0]+1][vr.current_mission[0]+1])
	local f = io.open(path..path_pack..name_miss..'.bin',"rb")
	local packk = bitser.loads(lzw.decompress(f:read("*all")))
	f:close()
	return packk
end

function manager.load_s()
	local name_miss = ffi.string(vr.temp_var.list_name_storylines[vr.current_storyline[0]+1])
	local f = io.open(path_s .. name_miss .. '.bin',"rb")
	local story = bitser.loads(lzw.decompress(f:read("*all")))
	f:close()
	return story
end

function manager.saveListMiss()
	local path_pack = ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1])..'\\'
	local f = io.open(path..path_pack..'list.bin','wb')
	local save = bitser.dumps(vr.temp_var.list_name_missions[vr.current_mission_pack[0]+1])
	f:write(save)
	f:close()
end

function manager.loadListMiss()
	local path_pack = ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1])..'\\'
	local f = io.open(path..path_pack..'list.bin','rb')
	local list = bitser.loads(f:read("*all"))
	f:close()
	return list
end

return manager
