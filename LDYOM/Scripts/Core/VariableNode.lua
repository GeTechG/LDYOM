ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

NodeVariable = bitser.registerClass(class("NodeVariable", BaseNode));

function NodeVariable:initialize(id,varr,setter)
	BaseNode.initialize(self,id);
	self.type = 3;
	self.var = varr;
	self.setter = setter or false;
	print(tostring(varr));
	print(tostring(ldyom.currentNodeGraph.vars[varr]));
	local ttype = ldyom.currentNodeGraph.vars[varr].typeValue[0];
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
		self.Pins[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.void, 1);
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
		
		imgui.imnodes.BeginOutputAttribute(self.id+3);
		imgui.Dummy(imgui.ImVec2:new(0,10));
		imgui.imnodes.EndOutputAttribute();
	else
		imgui.imnodes.BeginOutputAttribute(self.id+1);
		imgui.Dummy(imgui.ImVec2:new(0,10));
		imgui.imnodes.EndOutputAttribute();
	end
	
	imgui.imnodes.EndNode();
	
end

function NodeVariable:play(data, mission)
	local ttype = ldyom.currentNodeGraph.vars[self.var].typeValue[0];
	if self.setter then
		if ttype == 1 then
			ffi.copy(ldyom.realVariable[self.var],self:getPinValue(self.id+2,data,mission));
		else
			ldyom.realVariable[self.var][0] = self:getPinValue(self.id+2,data,mission)[0];
		end
		self:callOutputLinks(data, mission, self.id+3);
	end
end