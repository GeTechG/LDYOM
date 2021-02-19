ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

NodeVariable = bitser.registerClass(class("NodeVariable", BaseNode));

function NodeVariable:initialize(id,var,setter)
	BaseNode.initialize(self,id);
	self.type = 3;
	self.var = var;
	self.setter = setter or false;
	local ttype = ldyom.currentNodeGraph.vars[var].typeValue[0];
	local data;
	if ttype == 0 then
		data = ffi.new("float[1]");
	elseif ttype == 1 then
		data = ffi.new("char[128]");
	elseif ttype == 2 then
		data = ffi.new("bool[1]");
	end
	if setter then
		self.Pins[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0);
		self.Pins[self.id+2] = BasePin:new(self.id+2,ttype, 0, data);
	else
		self.Pins[self.id+1] = BasePin:new(self.id+1,ttype, 1);
	end
end

function NodeVariable:update_value()
	local ttype = ldyom.currentNodeGraph.vars[self.var].typeValue[0];
	local data;
	if ttype == 0 then
		data = ffi.new("float[1]");
	elseif ttype == 1 then
		data = ffi.new("char[128]");
	elseif ttype == 2 then
		data = ffi.new("bool[1]");
	end
	if self.setter then
		self.Pins[self.id+2].type = ttype;
		self.Pins[self.id+2].value = data;
	else
		self.Pins[self.id+1].type = ttype;
	end
end

function NodeVariable:draw()
	imgui.imnodes.BeginNode(self.id,self.type)
	
	imgui.imnodes.BeginNodeTitleBar();
	imgui.Text(imgui.imnodes.getNodeIcon("var")..' '..ffi.string(ldyom.currentNodeGraph.vars[self.var].var_name));
	if ldyom.getLastNode() == self.id then
		imgui.SameLine(0,0);
		imgui.TextColored(imgui.ImVec4.new(1.0,0.0,0.0,1.0)," \xef\x86\x88");
	end
	imgui.imnodes.EndNodeTitleBar();
	
	if self.setter then
		imgui.imnodes.BeginInputAttribute(self.id+1);
		imgui.Dummy(imgui.ImVec2:new(0,10));
		imgui.imnodes.EndInputAttribute();
		
		imgui.imnodes.BeginInputAttribute(self.id+2);
		imgui.Text(ldyom.langt("value"));
		if (self.Pins[self.id+2].link == nil) then
			imgui.SetNextItemWidth(150);
			if self.Pins[self.id+2].type == 0 then
				imgui.InputFloat("",self.Pins[self.id+2].value);
			elseif self.Pins[self.id+2].type == 1 then
				imgui.InputText("",self.Pins[self.id+2].value, ffi.sizeof(self.Pins[self.id+2].value));
			elseif self.Pins[self.id+2].type == 2 then
				imgui.ToggleButton("",self.Pins[self.id+2].value);
			end
		end
		imgui.imnodes.EndInputAttribute();
	else
		imgui.imnodes.BeginOutputAttribute(self.id+1);
		imgui.Dummy(imgui.ImVec2:new(0,10));
		imgui.imnodes.EndOutputAttribute();
	end
	
	imgui.imnodes.EndNode();
	
end

function NodeVariable:play(data, mission)
	local self_target = self:getPinValue(self.id+2,data)[0];
	ldyom.setLastNode(self.id);
	assert(self_target < #mission.list_targets,"The ID of the objective exceeds the number of actors.");
	if ldyom.getCurrTarget() == self_target then
		self:callOutputLinks(data,mission,self.id+3);
	else
		self:callOutputLinks(data,mission,self.id+4);
	end
	self:callOutputLinks(data,mission,self.id+5);
end