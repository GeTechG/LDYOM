class = require "LDYOM.Scripts.middleclass"
ffi = require "ffi"
require "LDYOM.Scripts.utf8"

BaseNode = class("BaseNode");

function BaseNode:initialize(id)
	self.id = id;
	self.Pins = {};
	self.pos = {
		["x"] = 0,
		["y"] = 0
	}
	self.runnable = false;
end

function BaseNode:getPinValue(idx,data,mission)
	if self.Pins[idx].link then
        local link = data.links[self.Pins[idx].link]
        local node = data.nodes[math.floor(link.id_out/100)*100]
        if node.class.type == 3 then
            --return nodes2.real_vars[node.var]
        else
			if not node.runnable then
				node:play(data,mission);
				node.runnable = true;
			end
            return node.Pins[link.id_out].value;
        end
    else
        return self.Pins[idx].value;
    end
end

function BaseNode:callOutputLinks(mission,id_out)
	if self.Pins[id_out].links ~= nil then
		for k,v in pairs(self.Pins[id_out].links) do
			local link = mission.nodeGraph.links[v];
			for k2,v2 in pairs(link) do
				local strr = Utf8ToAnsi(k2);
				if strr == "id_in" then
					local node_num = math.floor(v2 / 100)*100;
					mission.nodeGraph.nodes[node_num]:play(mission);
					break;
				end
			end
		end
	end
end

BasePin = class("BasePin");

function getValueByType(type_)
	if type_ == imgui.imnodes.PinType.number then
		return ffi.new("float[1]",0);
	elseif type_ == imgui.imnodes.PinType.boolean then
		return ffi.new("bool[1]",false);
	elseif type_ == imgui.imnodes.PinType.string then
		return ffi.new("char[128]","");
	elseif type_ == imgui.imnodes.PinType.void then
		return nil;
	end
end

function BasePin:initialize(id, type_, pin_type, value)
	self.id = id;
	self.type = type_;
	if (pin_type == 0) then
		self.link = nil;
	else
		self.links = {};
	end
	self.pin_type = pin_type;
	self.value = value;
end

BaseLink = class("BaseLink");

function BaseLink:initialize(id_in,id_out)
	self.id_in = id_in;
	self.id_out = id_out;
end

Variable = class("Variable");

function Variable:initialize()
	self.var_name = ffi.new("char[65]",ldyom.langt("variable")..' #'..#ldyom.currentNodeGraph.vars);
	--0 - value
	--1 - string
	--2 - boolean
    self.typeValue = ffi.new("unsigned char[1]",0);
    self.value = nil
end