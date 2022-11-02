local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@enum PinType
PinType = {
    In = 0,
    Out = 1
}

---@enum ValueType
ValueType = {
    Void = 0,
    Number = 1,
    Boolean = 2,
    String = 3
}

---@enum NodeType
NodeType = {
    Function = 0,
    Loops = 1,
	Event = 2,
	Condition = 3,
	Variable = 4,
    Constant = 5
}

local entities = {}

---@class Pin
---@field id integer
---@field name string
---@field type PinType
---@field valueType ValueType
---@field value any
---@field connected boolean;
local Pin = class("Pin");

---@param id integer
---@param type PinType
---@param name string
---@param valueType ValueType
---@param defaultValue any
function Pin:initialize(id, name, type, valueType, defaultValue, size)
    self.id = id;
    self.name = name;
    self.type = type;
    self.valueType = valueType;
    self.connected = false;
    if valueType == ValueType.Number then
        self.value = ffi.new("float[1]", {defaultValue or 0});
    elseif valueType == ValueType.Boolean then
        self.value = ffi.new("bool[1]", {defaultValue or false});
    elseif valueType == ValueType.String and size ~= nil then
        self.value = ffi.new("char["..tostring(size).."]", defaultValue);
    end 
end

bitser.registerClass(Pin);

---@class Link
---@field pinInId integer;
---@field pinOutId integer;
---@field workspace integer;
local Link = class("Link");

---@param pinIn Pin;
---@param pinOut Pin;
function Link:initialize(pinIn, pinOut)
    if pinIn == PinType.In then
        self.pinInId = pinIn.id;
        self.pinOutId = pinOut.id;
    else
        self.pinInId = pinOut.id;
        self.pinOutId = pinIn.id;
    end
end

bitser.registerClass(Link);

---@class Workspace
---@field name ffi.cdata*
---@field context userdata

---@class NodeEditorContext
---@field nodes BaseNode[];
---@field links Link[];
---@field workspaces Workspace[];
---@field currentWorkspace integer;
---@field idIterator integer;
local NodeEditorContext = class("NodeEditorContext");
function NodeEditorContext:initialize()
    self.idIterator = 0;
    self.nodes = {};
    self.links = {};
    self.workspaces = {};
    self:newWorkspace();
end

function NodeEditorContext:newWorkspace()
    self.workspaces[#self.workspaces+1] = {
        name = ffi.new("char["..constants.TEXT_SIZE.."]", loc.get("other.workspace").." "..#self.workspaces);
        context = ImNodes.editorContextCreate();
    }
    self.currentWorkspace = #self.workspaces;
end

function NodeEditorContext:deleteWorkspace()
    for id, node in pairs(self.nodes) do
        if node.workspace == self.currentWorkspace then
            self:deleteNode(id);
        end
    end
    self.workspaces[self.currentWorkspace] = nil;
    self.currentWorkspace = #self.workspaces;
end

function NodeEditorContext:getNewID()
    local new_id = self.idIterator;
    self.idIterator = self.idIterator + 1;
    return new_id;
end

function NodeEditorContext:newNode(class_node, ...)
    local new_id = self:getNewID() * 100;
    self.nodes[new_id] = class_node:new(new_id, ...);
    self.nodes[new_id].workspace = self.currentWorkspace;
    local mx, my = ImGui.getMousePos();
    ImNodes.setNodeScreenSpacePos(new_id, mx, my);
end

function NodeEditorContext:deleteNode(nodeId)
    self.nodes[nodeId] = nil;
    for i, link in pairs(self.links) do
        if math.floor(link.pinInId / 100) * 100 == nodeId or math.floor(link.pinOutId / 100) * 100 == nodeId then
            self:deleteLink(i);
        end
    end
end

function NodeEditorContext:deleteNodeByClass(clazz, paramName, paramValue)
    for id, node in pairs(self.nodes) do
        if node.class == clazz then
            if paramName then
                if node[paramName] == paramValue then
                    self:deleteNode(id);
                end
            else
                self:deleteNode(id);
            end
        end
    end
end

function NodeEditorContext:getLastNodeId()
    local m = 0;
    for i, _ in pairs(self.nodes) do
        m = math.max(m, i);
    end
    return m;
end

---@param nodeId integer
---@param pinType PinType
---@param pinValueType ValueType
function NodeEditorContext:findPinByValueType(nodeId, pinType, pinValueType)
    for i, pin in ipairs(self.nodes[nodeId].pins) do
        if pin.type == pinType and pin.valueType == pinValueType then
            return pin;
        end
    end
    return nil;
end

---@param startedNodeId integer
---@param startedPinId integer
---@param endNodeId integer
---@param endPinId integer
function NodeEditorContext:createLink(startedNodeId, startedPinId, endNodeId, endPinId)
    if startedNodeId ~= endNodeId then
        local startedPin = self.nodes[startedNodeId].pins[startedPinId % 100];
        local endPin = self.nodes[endNodeId].pins[endPinId % 100];

        if startedPin.valueType == endPin.valueType then
            local linkId;
            if startedPin.type == PinType.In then
               linkId = self:getPinInLink(startedPinId);
            else
                linkId = self:getPinInLink(endPinId);
            end
            if not linkId then
                linkId = #self.links+1;
            end
            
            self.links[linkId] = Link:new(startedPin, endPin);
            self.links[linkId].workspace = self.currentWorkspace;

            self:updatePinConnected();
            return true;
        end
    end

    return false;
end

function NodeEditorContext:deleteLink(linkId)
    self.links[linkId] = nil;
    self:updatePinConnected();
end

function NodeEditorContext:getPin(pinId)
    return self.nodes[math.floor(pinId / 100) * 100].pins[pinId % 100]
end

function NodeEditorContext:updatePinConnected()
    for _, node in pairs(self.nodes) do
        for _, pin in pairs(node.pins) do
            pin.connected = false;
            for _, link in pairs(self.links) do
                if pin.id == link.pinInId or pin.id == link.pinOutId then
                    pin.connected = true;
                end
            end
        end
    end
end

---@param pinId integer
---@param valueType ValueType
function NodeEditorContext:IsPinHaveValueType(pinId, pinType, valueType)
    local pin = self:getPin(pinId);
    return pin.valueType == valueType and pin.type == pinType;
end

---@param pinInId integer
function NodeEditorContext:getPinInLink(pinInId)
    for i, link in pairs(self.links) do
        if link.pinInId == pinInId then
            return i;
        end
    end
    return nil;
end

---@param pinInId integer
---@param queryId integer
---@return any|nil;
function NodeEditorContext:getValue(scene, tasklist, pinInId, queryId)
    if not self:getPin(pinInId).connected then
        return self:getPin(pinInId).value;
    end
    for i, link in pairs(self.links) do
        if link.pinInId == pinInId then
            local outNode = self.nodes[math.floor(link.pinOutId / 100) * 100];
            if outNode.lastQuery ~= queryId and not outNode.activation then
                outNode:execute(scene, tasklist, self);
                outNode.lastQuery = queryId;
            end
            return outNode.pins[link.pinOutId % 100].value;
        end
    end
    return nil;
end

function NodeEditorContext:callNodes(scene, tasklist, pinOutId)
    for i, link in pairs(self.links) do
        if link.pinOutId == pinOutId then
            local node = self.nodes[math.floor(link.pinInId / 100) * 100]
            node:execute(scene, tasklist, self);
        end
    end
end

---@param nodeId integer
---@param workspace integer
function NodeEditorContext:moveNodeToWorkspace(nodeId, workspace)
    self.nodes[nodeId].workspace = workspace;
    self.nodes[nodeId].new_pos = ImNodes.getNodeEditorSpacePos(nodeId);
    for _, link in pairs(self.links) do
        if math.floor(link.pinInId / 100) * 100 == nodeId or math.floor(link.pinOutId / 100) * 100 == nodeId then
            if self.nodes[math.floor(link.pinInId / 100) * 100].workspace ~= workspace then
                self:moveNodeToWorkspace(math.floor(link.pinInId / 100) * 100, workspace);
            end
            if self.nodes[math.floor(link.pinOutId / 100) * 100].workspace ~= workspace then
                self:moveNodeToWorkspace(math.floor(link.pinOutId / 100) * 100, workspace);
            end
            link.workspace = workspace;
        end
    end
end

function NodeEditorContext:save()
    local save_data = {
        nodes = self.nodes,
        links = self.links,
        idIterator = self.idIterator,
        workspaces = {},
        nodes_positions = {}
    }
    for i, node in pairs(self.nodes) do
        ImNodes.editorContextSet(self.workspaces[node.workspace].context);
        local pos = ImNodes.getNodeEditorSpacePos(node.id);
        save_data.nodes_positions[i] = {
            x = pos.x,
            y = pos.y
        }
    end
    for i, workspace in pairs(self.workspaces) do
        save_data.workspaces[i] = workspace.name;
    end
    local save_data_dump = bitser.dumps(save_data);
    return save_data_dump;
end

function NodeEditorContext:load(data)
    local load_data = bitser.loads(data);
    self.workspaces = {};
    for i, workspace in pairs(load_data.workspaces) do
        self.workspaces[i] = {
            name = workspace,
            context = ImNodes.editorContextCreate();
        }
    end
    self.idIterator = load_data.idIterator;
    self.links = load_data.links;
    self.nodes = load_data.nodes;
    for i, node in pairs(self.nodes) do
        ImNodes.editorContextSet(self.workspaces[node.workspace].context);
        ImNodes.setNodeEditorSpacePos(node.id, ImVec2:new(load_data.nodes_positions[i].x, load_data.nodes_positions[i].y));
    end
    self.currentWorkspace = #self.workspaces;
end

bitser.registerClass(NodeEditorContext);

---@class BaseNode 
---@field id integer
---@field type NodeType
---@field name string
---@field activation boolean
---@field pins Pin[]
---@field workspace integer
---@field lastQuery integer
local BaseNode = class("BaseNode");


---@param id integer
function BaseNode:initialize(id)
    self.id = id;
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function BaseNode:execute(scene, tasklist, context) end

bitser.registerClass(BaseNode);

entities.NodeEditorContext = NodeEditorContext;
entities.NodeType = NodeType;
entities.BaseNode = BaseNode;
entities.PinType = PinType;
entities.ValueType = ValueType;
entities.Pin = Pin;
entities.Link = Link;

return entities;