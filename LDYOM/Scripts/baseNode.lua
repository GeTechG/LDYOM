class = require "LDYOM.Scripts.middleclass"
ffi = require "ffi"

BaseNode = class("BaseNode");

function BaseNode:initialize(id)
	self.id = id;
	self.Pins = {};
end

function BaseNode:getPinValue(idx,data)
	if self.Pins[idx].link then
        local link = data.links[self.Pins[idx].link]
        local node = data.nodes[math.floor(link.id_out/100)*100]
        if node.class.type == 3 then
            --return nodes2.real_vars[node.var]
        else
            return node.Pins[link.id_out].value;
        end
    else
        return self.Pins[idx].value;
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
	if (value == nil) then
		self.value = getValueByType(type_);
	else
		self.value = value;
	end
end

BaseLink = class("BaseLink");

function BaseLink:initialize(id_in,id_out)
	self.id_in = id_in;
	self.id_out = id_out;
end