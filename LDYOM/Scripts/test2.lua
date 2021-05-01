ffi = require "ffi"
require "LDYOM.Scripts.utf8"
local bitser = require "LDYOM.Scripts.bitser"
local class = require "LDYOM.Scripts.middleclass"

local da = class('TestHr');
function da:initialize(name)
	self.name = name
	self[1] = 'instance can be sequence'
end

function da:da()
	print("da");
end

print("test2");
addR(da);