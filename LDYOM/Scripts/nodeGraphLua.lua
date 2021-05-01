ffi = require "ffi"
require "LDYOM.Scripts.utf8"
bitser = require "LDYOM.Scripts.bitser"

function addRegister(clas)
	bitser.registerClass(clas)
end

function addRo(clas)
	print(clas.name);
	local da = class('Horse');
	Horse = bitser.registerClass(da);
	function Horse:initialize(name)
		self.name = name
		self[1] = 'instance can be sequence'
	end

	function Horse:da()
		print("da");
	end

	local bojack = Horse('Bojack Horseman')
	print(bitser.dumps(bojack));
end
--return bitser.registerClass(class);