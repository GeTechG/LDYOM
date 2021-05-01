ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeGotoMission", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeGotoMission");
Node.static.storyline = true;

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.mission = ffi.new("int[1]",0);
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.void, 1)
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
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+2);
	local names = ldyom.getStorylineMissionsNames();
	imgui.Text(ldyom.langt("mission"));
	imgui.SetNextItemWidth(150);
	imgui.ComboVecStr("",self.mission,names);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, storyline)
	ldyom.setLastNode(self.id);
	local name = ldyom.getStorylineMissionsNames()[self.mission[0]+1];
	ldyom.start_storyline_mission(name);
	ldyom.set_last_mission(self.mission[0]);
	for k,v in pairs(data.nodes) do
		local name = imgui.imnodes.getNodeIcon("event")..' '..ldyom.langt("CoreNodeMissionComplete");
		local name_node = v["class"]["static"]["name"];
		if name == name_node and v["Pins"][v.id+1].value[0] == self.mission[0] then
			callNodeThreadS(v,data,storyline);
		end
	end
	if self.Pins[self.id+3].links ~= nil then
		for k,v in pairs(self.Pins[self.id+3].links) do
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