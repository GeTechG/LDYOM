ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeMissionComplete", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("event")..' '..ldyom.langt("CoreNodeMissionComplete");
Node.static.storyline = true;

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 0;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("int[1]",0)),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.void, 1),
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
	
	imgui.imnodes.BeginStaticAttribute(self.id+1);
	local names = ldyom.getStorylineMissionsNames();
	imgui.Text(ldyom.langt("mission"));
	if (self.Pins[self.id+1].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ComboVecStr('',self.Pins[self.id+1].value,names);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+2);
	imgui.Dummy(imgui.ImVec2:new(0,20));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, storyline)
	ldyom.setLastNode(self.id);
	if self.Pins[self.id+2].links ~= nil then
		for k,v in pairs(self.Pins[self.id+2].links) do
			local link = data.links[v];
			for k2,v2 in pairs(link) do
				local strr = Utf8ToAnsi(k2);
				if strr == "id_in" then
					local node_num = math.floor(v2 / 100)*100;
					data.nodes[node_num]:play(data,storyline);
					break;
				end
			end
		end
	end
end

ldyom.nodeEditor.addNodeClass("Storyline",Node);