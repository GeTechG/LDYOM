ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeHideActor", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeHideActor");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.void, 1),
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
	local names = ldyom.namesActors;
	imgui.Text(ldyom.langt("actor"));
	if (self.Pins[self.id+2].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ComboVecChars("",self.Pins[self.id+2].value,names);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local actor = self:getPinValue(self.id+2,data,mission)[0];
	ldyom.setLastNode(self.id);
	assert(actor < #mission.list_actors,"The ID of the actor exceeds the number of actors.");
	mission.list_actors[actor+1]:removeMissionPed();
	for k,v in pairs(data.nodes) do
		local name = imgui.imnodes.getNodeIcon("event")..' '..ldyom.langt("CoreNodeDisapperActor");
		local name_node = v["class"]["static"]["name"];
		if name == name_node then
			callNodeThread(v,data,mission);
		end
	end
	if self.Pins[self.id+3].links ~= nil then
		for k,v in pairs(self.Pins[self.id+3].links) do
			local link = data.links[v];
			for k2,v2 in pairs(link) do
				local strr = Utf8ToAnsi(k2);
				if strr == "id_in" then
					local node_num = math.floor(v2 / 100)*100;
					data.nodes[node_num]:play(data,mission);
					break;
				end
			end
		end
	end
end

ldyom.nodeEditor.addNodeClass("Actor",Node);