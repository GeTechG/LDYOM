--[=[  ------   ffi-enum.lua   ------  [=[--

Syntactic sugar for enum-like FFI structs.

Released under the MIT License
(full text at the end of the file).

    enum = require"ffi-enum"

    e = enum[[
      FOO, BAR, /* comments are */
      BAZ = 7,  // parsed properly 
      QUX,
      QUUX = 0xc
    ]]

    assert(e.FOO == 0 and e.BAR == 1
       and e.BAZ == 7 and e.QUX == 8
       and e.QUUX == 254)

    -- note that leading spaces are allowed for indenting.

    d = fe.define[[
      #define foo 0 
      #define bar 1
      #define baz 5
      #define qux 6
      #define quux 0xFe
    ]]

    assert(d.FOO == 0 and d.BAR == 1
       and d.BAZ == 7 and d.QUX == 8
       and d.QUUX == 12)

In both cases, the code is compiled to

    ffi.new[[
      struct{
        static const int FOO = 0;
        static const int BAR = 1;
        static const int BAZ = 7;
        static const int QUX = 8;
        static const int QUUX = 12;
      }
    ]]

The compiler treats the fields as constants, which enables 
optimizations not possible with Lua table fields.

Please note that these are not seen as enums on the C side.

-- ]=]  ------   --   --   --   ------  ]=]--


local t_concat = require"table".concat
local new = require"ffi".new
local cache = setmetatable({}, {__mode = "v"})

local inline_comment = "^%s*//[^\n]*\n()"
local multi_line_comment = "^%s*/%*.-*/%s*()"
local enumpat = "^(%s*([%w_][%a_]*)%s*(=?)%s*([x%x]*)%s*())"

local function enum(defs)
  local cached = cache[defs]
  if cached then return cached end


  local N = 0
  local pos = 1
  local len = #defs
  local res = {}
  local coma = false

  while true do
    if pos == len + 1 then break end
    if pos > len + 1 then error("LARGER: "..pos.." "..len) end

    local p = defs:match(inline_comment, pos) or defs:match(multi_line_comment, pos)

    if not p then
      if coma then
        p = defs:match("^%s*,%s*()", pos)
        if not p then error"malformed enum: coma expected" end
        coma = false
      else
        local chunk, name, eq, value
        chunk, name, eq, value, p = defs:match(enumpat, pos)
        if not p then error("malformed enum definition") end

        if value ~= "" then
          assert(value:find"^%-?%d+$" or value:find"0x%x+", "badly formed number "..value.." in enum")
          N = tonumber(value)
        end

        local i = N
        N = N + 1

        if eq == "" and value == "" or eq == "=" and value ~= "" then
          res[#res+1] = "  static const int "..name.." = "..i..";"
        else
          error("badly formed enum: " .. chunk)
        end
        coma = true
      end -- if coma
    end -- if not p

    pos = p
  end -- while true

  res = new("struct{ \n"..t_concat(res, "\n").."\n}")
  cache[defs] = res
  return res
end

local definepat = "^(#define[ \t]+([%w_][%a_]*)[ \t]+([x%x]+)[ \t]*(\n?)())"

local function define(defs)
  local cached = cache[defs]
  if cached then return cached end

  local pos = defs:match("^%s*\n()") or 1
  local len = #defs
  local res = {}

  while true do
    if pos == len + 1 then break end
    if pos > len + 1 then error("LARGER: "..pos.." "..len) end

    local chunk, name, value, lf, p = defs:match(definepat, pos)
    p = p or defs:match(inline_comment, pos) or defs:match(multi_line_comment, pos)
    if chunk then
      if lf ~= "\n" and p ~= len + 1 then
        error("end of line expected after: "..chunk)
      end
      assert(value:find"^%-?%d+$" or value:find"0x%x+", "badly formed number "..value.." in enum")

      res[#res+1] = "  static const int "..name.." = "..value..";"
    elseif not p then
      p = defs:match("^[ \t]+()", pos)
      assert(p, "malformed #define")
    end -- if chunk
    pos = p
  end -- while true
  res = new("struct{ \n"..t_concat(res, "\n").."\n}")
  cache[defs] = res
  return res
end

 -- tests
pcall(function() if arg[0]:find"ffi%-enum.lua$" then
  xpcall(function()
    local e
    e = enum[[foo, bar, baz]]
    assert(e.foo == 0, e.bar == 1, e.baz == 2)

    e = enum[[
      foo, bar, 
      baz = 5, qux, 
      quux = 0xFe
    ]]
    assert(e.foo == 0, e.bar == 1, e.baz == 5, e.qux == 6, e.quux == 254)

    e = enum[[
      // Blah
      foo, /* TRAP! */bar, /**/ //
      baz = 6, qux, 
      quux = 0xFe
      /* end */
    ]]
    assert(e.foo == 0, e.bar == 1, e.baz == 6, e.qux == 7, e.quux == 254)

    e = define[[
      // hello
      #define foo 0 
      #define bar 1
        /* gfdfg */
      #define baz 5
      #define qux 6
      #define quux 0xFe
    ]]
    assert(e.foo == 0, e.bar == 1, e.baz == 5, e.qux == 6, e.quux == 254)

    print"ok"
   end, function(e) print(e) os.exit(1) end)
end end)


return {enum = enum, define = define}

--[[

ffi-enum.lua

Copyright (c) 2014 Pierre-Yves Gérardy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

]]
