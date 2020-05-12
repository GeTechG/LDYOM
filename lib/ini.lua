#!/usr/bin/luajit
--[[
    ini.lua - read/write access to INI files in Lua
    Copyright (C) 2012-2013 Jens Oliver John <asterisk@2ion.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Project home: <https://github.com/2ion/ini.lua>
--]]

local Path = require("pl.path")
local ffi = require("ffi")

ffi.cdef([[
enum base64_encodestep { step_A, step_B, step_C };
struct base64_encodestate { enum base64_encodestep step; char result; int stepcount; };
enum base64_decodestep { step_a, step_b, step_c, step_d };
struct base64_decodestate { enum base64_decodestep step; char plainchar; };

void base64_init_encodestate(struct base64_encodestate* state_in);
char base64_encode_value(char value_in);
int base64_encode_block(const char* plaintext_in, int length_in,
    char* code_out, struct base64_encodestate* state_in);

int base64_encode_blockend(char* code_out, struct base64_encodestate* state_in);
void base64_init_decodestate(struct base64_decodestate* state_in);
int base64_decode_value(char value_in);
int base64_decode_block(const char* code_in, const int length_in,
    char* plaintext_out, struct base64_decodestate* state_in);
]])

local b64 = ffi.load("libb64.so.0d")

--- Encode the data in str to base64.
-- @param str input string, can hold arbitrary binary data
-- @return Lua string with the base64 representation, base64 byte count
-- @see base64_decode()
local function base64_encode(str)
    local str = type(str) == "string" and str or tostring(str)
    local len = #str + 1 -- we also encode empty strings!
    local inbuf = ffi.new("char[?]", len, str)
    local inlen = ffi.new("int", len)
    local buf = ffi.new("char[?]", 4 * ((len - len % 3 + 3)/3))
    local state = ffi.new("struct base64_encodestate[1]")
    local cnt = 0

    b64.base64_init_encodestate(state)
    cnt = b64.base64_encode_block(inbuf, inlen, buf, state)

    return ffi.string(buf), cnt
end

--- Decode a string holding base64 encoded data to a Lua string. The
-- resulting string may hold arbitrary binary data.
-- @param b64str Lua string, base64 encoded data
-- @return Lua string with the deoded data, conversion byte count
-- @see base64_encode()
local function base64_decode(b64str)
    local len = #b64str
    local inbuf = ffi.new("char[?]", len, b64str)
    local inlen = ffi.new("int", len)
    local buf = ffi.new("char[?]", len) -- FIXME: use smallest length possible
    local state = ffi.new("struct base64_decodestate[1]")

    b64.base64_init_decodestate(state)
    local cnt = b64.base64_decode_block(inbuf, inlen, buf, state)

    return ffi.string(buf), cnt
end

--- Print out the kv-pairs of a Lua table, with a prefix per line.
-- Use to enumerate the contents of a INI data table.
-- @param t INI data table
-- @param section line prefix, may be empty
-- @return Nothing
local function debug_enum(t, section)
    for k,v in pairs(t) do
        if type(v) == "table" then
            debug_enum(v, tostring(k))
        else
            print(section or "", k, "", "",v)
        end
    end
end

--- Parse a flat INI file and map the data to a Lua table.
-- The resulting kv-pairs will always be pairs of strings.
-- @param file the file to read the data from
-- @return NIL in case of an error (file does not exist or is not
-- readable). In case of success: data table, table with invalid lines
-- in the source file.
local function read(file)
    if not Path.isfile(file) then return nil end
   
    local file = io.open(file, "r")
    local data = {}
    local rejected = {}
    local parent = data
    local i = 0
    local m, n

    local function parse(line)
        local m, n

        -- kv-pair
        m,n = line:match("^([%w%p]-)=(.*)$")
        if m then
            parent[m] = n
            return true
        end

        -- section opening
        m = line:match("^%[([%w%p]+)%][%s]*")
        if m then
            data[m] = {}
            parent = data[m]
            return true
        end

        if line:match("^$") then
            return true
        end

        -- comment
        if line:match("^#") then
            return true
        end

        return false
    end

    for line in file:lines() do
        i = i + 1
        if not parse(line) then
            table.insert(rejected, i)
        end
    end
    file:close()
    return data, rejected
end


--- Like read(), but treat values starting with the sequence `base64:`
-- as special: the base64 encoded data will be decoded into a string
-- value and may hold arbitrary binary data. Standard Lua string
-- functions may not work reliably on these strings. Binary values will
-- have a metatable with the key `__ini_is_binary` set to a TRUE value.
-- @param file source file
-- @return in case of an error: NIL. Otherwise: data table, table with
-- lines rejected by the parser.
-- @see read
local function read_typed(file)
    if not Path.isfile(file) then return nil end
   
    local file = io.open(file, "r")
    local data = {}
    local rejected = {}
    local parent = data
    local i = 0
    local m, n

    local function parse(line)
        local m, n

        -- kv-pair
        m,n = line:match("^([%w%p]-)=(.*)$")
        if m then
            local n_type, n_value = n:match("^([%a%d^:]*):(.*)")
            if n_type == "base64" then
                local v = base64_decode(n_value)
                debug.setmetatable(v, { __ini_is_binary = true })
                parent[m] = v
            elseif n_type == "string" then
                parent[m] = n_value
            elseif n_type == "number" then
                parent[m] = tonumber(n_value)
            elseif n_type == "boolean" then
                parent[m] = n_value == "true" and true or "false"
            end
            return true
        end

        -- section opening
        m = line:match("^%[([%w%p]+)%][%s]*")
        if m then
            data[m] = {}
            parent = data[m]
            return true
        end

        if line:match("^$") then
            return true
        end

        -- comment
        if line:match("^#") then
            return true
        end

        return false
    end

    for line in file:lines() do
        i = i + 1
        if not parse(line) then
            table.insert(rejected, i)
        end
    end
    file:close()
    return data, rejected
end



--- Like read(), but treat values starting with the sequence `base64:`
-- as special: the base64 encoded data will be decoded into a string
-- value and may hold arbitrary binary data. Standard Lua string
-- functions may not work reliably on these strings. Binary values will
-- have a metatable with the key `__ini_is_binary` set to a TRUE value.
-- @param file source file
-- @return in case of an error: NIL. Otherwise: data table, table with
-- lines rejected by the parser.
-- @see read
local function read64(file)
    if not Path.isfile(file) then return nil end
   
    local file = io.open(file, "r")
    local data = {}
    local rejected = {}
    local parent = data
    local i = 0
    local m, n

    local function parse(line)
        local line = line
        local m, n = nil, nil

        -- kv-pair
        m, n = line:match("^([%w%p]-)=(.*)$")
        if m then
            if n:match("^base64:") then
                local n = n:match("^base64:(.*)")
                local v = base64_decode(n)
                debug.setmetatable(v, { __ini_is_binary = true })
                parent[m] = v
            else
                parent[m] = n
            end
            return true
        end

        -- section opening
        m = line:match("^%[([%w%p]+)%][%s]*")
        if m then
            data[m] = {}
            parent = data[m]
            return true
        end

        if line:match("^$") then
            return true
        end

        -- comment
        if line:match("^#") then
            return true
        end

        return false
    end

    for line in file:lines() do
        i = i + 1
        if not parse(line) then
            table.insert(rejected, i)
        end
    end
    file:close()
    return data, rejected
end

--- Like read(), but reads only nested INI files. There is no
-- autodetection of the nesting, so be careful.
-- @param file input file
-- @return In case of an error. Otherwise: data table, table with
-- rejected lines
-- @see read()
local function read_nested(file)
    if not Path.isfile(file) then return nil end

    local file = io.open(file, "r")
    local d = {}
    local h = {}
    local r = {}
    local p = d
    local i = 0

    local function parse(line)
        local m, n

        -- section opening
        m = line:match("^[%s]*%[([^/.]+)%]$")
        if m then
            table.insert(h, { p, m=m })
            p[m] = {}
            p = p[m]
            return true
        end

        -- section closing
        m = line:match("^[%s]*%[/([^/.]+)%]$")
        if m then
            local hl = #h
            if hl == 0 or h[hl].m ~= m then
                return nil
            end
            p = table.remove(h).p
            if not p then p = d end
            return true
        end

        -- kv-pair
        m,n = line:match("^[%s]*([%w%p]-)=(.*)$")
        if m then
            p[m] = n
            return true
        end

        -- ignore empty lines
        if line:match("^$") then
            return true
        end

        -- ignore comments
        if line:match("^#") then
            return true
        end

        -- reject everything else
        return nil
    end

    for line in file:lines() do
        i = i + 1
        if not parse(line) then
            table.insert(r, i)
        end
    end

    file:close()
    return d, r
end

--- Writes a Lua table as a flat INI file to a file. Sections cannot be
-- nested.
-- @param file output file
-- @param data data table. Format: { <SectionA> = { key = value[, ...] }, <SectionN>= { [...] } }
-- @return In case of an error (could not open file, no input data):
-- NIL. Else: true
-- @see read()
local function write(file, data)
    if type(data) ~= "table" then return nil end
    local file = io.open(file, "w")
    if not file then
        return nil
    end
    for s,t in pairs(data) do
        file:write(string.format("[%s]\n", s))
        for k,v in pairs(t) do
            file:write(string.format("%s=%s\n", tostring(k), tostring(v)))
        end
    end
    file:close()
    return true
end

--- Like write(), but treats values with a metatable which has the key
-- __ini_is_binary set to a TRUE value specially. These values will be
-- encoded to base64 before being written out, allowing arbitrary binary
-- data to be written. The data may be retrieved using only read64().
-- @param file output file
-- @param data data table
-- @return NIL (could not open file or no data), otherwise: true
local function write64(file, data)
    if type(data) ~= "table" then return nil end
    local file = io.open(file, "w")
    if not file then
        return nil
    end
    for s,t in pairs(data) do
        file:write(string.format("[%s]\n", s))
        for k,v in pairs(t) do
            local mt = getmetatable(v)
            if mt and mt.__ini_is_binary then
                file:write(string.format("%s=base64:%s\n", tostring(k), base64_encode(v)))
            else
                file:write(string.format("%s=%s\n", tostring(k), tostring(v)))
            end
        end
    end
    file:close()
    return true
end

local function write_typed(file, data)
    if type(data) ~= "table" then return nil end
    local file = io.open(file, "w")
    if not file then
        return nil
    end
    -- sections
    for s,t in pairs(data) do
        file:write(string.format("[%s]\n", s))
        -- values
        for k,v in pairs(t) do
            local mt = getmetatable(v)
            if mt and mt.__ini_is_binary then
                file:write(string.format("%s=base64:%s\n", tostring(k), base64_encode(v)))
            else
                file:write(string.format("%s=%s:%s\n", tostring(k), type(v), tostring(v)))
            end
        end
    end
    file:close()
    return true
end

--- Like write(), but can write a nested INI data structure.
-- @param file output file
-- @param data like in write(), but allowing nested sections: { A = { B -- = { k = v }, k = v }, B = { [...] } }
-- @return NIL in case of an error, otherwise: TRUE
local function write_nested(file, data)
    if type(data) ~= "table" then return nil end
    local file = io.open(file, "w")
    local function w(t)
        for i,j in pairs(t) do
            if type(j) == "table" then
                file:write(string.format("[%s]\n", i))
                w(j)
                file:write(string.format("[/%s]\n", i))
            else
                file:write(string.format("%s=%s\n", tostring(i), tostring(j)))
            end
        end
    end
    w(data)
    file:close()
    return true
end

return {
    read = read,
    read64 = read64,
    read_nested = read_nested,
    write = write,
    write64 = write64,
    write_nested = write_nested,
    read_typed = read_typed,
    write_typed = write_typed,
    debug = { enum = debug_enum }
}
