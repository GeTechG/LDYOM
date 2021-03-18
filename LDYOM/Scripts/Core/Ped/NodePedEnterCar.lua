ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedEnterCar", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodePedEnterCar");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.void, 1),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.void, 1),
	};
end

function Node:draw()
	imgui.imnodes.BeginNode(self.id,self.type)
	
	imgui.imnodes.BeginNodeTitleBar();
	imgui.Text(self.class.static.name);
	if ldyom.getLastNode() == self.id then
		imgui.SameLine(0,0);
		imgui.TextColored(imgui.ImVec4.new(1.0,0.0,0.0,1.0)," \xef\x86\x88");
	end
	imgui.imnodes.EndNodeTitleBar();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text(ldyom.langt("CoreHandleCar"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("speed_walk"));
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+4].value, ldyom.langMenu["speed_walk_to_car"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text(ldyom.langt("seat"));
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+5].value, ldyom.langMenu["place_in_car"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	if not self.Pins[self.id+6].link then
		imgui.ToggleButton(ldyom.langt("teleport"), self.Pins[self.id+6].value);
	else
		imgui.Text(ldyom.langt("teleport"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+7);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+8);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	addThreadObj(function(data_pack)
		local data = data_pack[1];
		local mission = data_pack[2];
		local node = data_pack[3];
		
		local ped = node:getPinValue(node.id+2,data,mission)[0];
		local car = node:getPinValue(node.id+3,data,mission)[0];
        local speed_walk = node:getPinValue(node.id+4,data,mission)[0]
        local seat = node:getPinValue(node.id+5,data,mission)[0]
        local teleport = node:getPinValue(node.id+6,data,mission)[0]
		ldyom.setLastNode(self.id);
		
        callOpcode(0x07A1, {{speed_walk+4,"int"}});
		if seat == 0 then
            if teleport then
				callOpcode(0x05CB, {{ped,"int"}, {car,"int"}, {0,"int"}});
            else
				callOpcode(0x05CB, {{ped,"int"}, {car,"int"}, {-1,"int"}});
            end
        else
            if teleport then
				callOpcode(0x05CA, {{ped,"int"}, {car,"int"}, {0,"int"}, {seat-1,"int"}});
            else
				callOpcode(0x05CA, {{ped,"int"}, {car,"int"}, {-1,"int"}, {seat-1,"int"}});
            end
        end

        while true do
            wait(0)
            if not callOpcode(0x056D,{{ped,"int"}}) then
                return
            end
			if callOpcode(0x00DF, {{ped,"int"}}) then
				break;
			end
        end
		
		
		node:callOutputLinks(data, mission, node.id+7);
		
	end, {data,mission,self});
	
	self:callOutputLinks(data, mission, self.id+8);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);