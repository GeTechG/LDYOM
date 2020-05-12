nodes_s = {}
nodes_s.nodes = {}
nodes_s.links = {}

nodes_s.show = new.bool(false)

db = require "debug_table"

nilo = "nilo"

function nodes_s.fif(condition, if_true, if_false)
    if condition then return if_true else return if_false end
end

function nodes_s.COLOR_U32(r, g, b, a)
    a = a or 255
    return imgui.ColorConvertFloat4ToU32(imgui.ImVec4(r/255,g/255,b/255,a/255))
end

function nodes_s.COLOR_U32_TO_RGBA(col)
    return imgui.ColorConvertU32ToFloat4(col)*255
end

function GetID()
    uID = uID + 1
    return uID
end

function nodes_s.GetDistVec2(v1,v2)
    return math.sqrt((v2.x - v1.x)^2 + (v2.y - v1.y)^2)
end

function nodes_s.Node(type,pos, inputs, outputs)
    local node = {}
    node.ID = new.int(GetID())
    node.Type = new.int(type)
    node.Name = new.char[32]()
    node.Pos = pos
    node.Size = imgui.ImVec2()
    node.Inputs = inputs
    node.Outputs = outputs
    return node
end
function GetInputSlotPos(tab,slot_no)
    return imgui.ImVec2(tab.Pos.x, tab.Pos.y + tab.Size.y * slot_no / (#tab.Inputs + 1) + 20)
end
function GetOutputSlotPos(tab,slot_no)
    return imgui.ImVec2(tab.Pos.x + tab.Size.x, tab.Pos.y + (tab.Size.y * slot_no) / (#tab.Outputs + 1) + 20)
end

function NodeLink(input_idx, input_slot, output_idx, output_slot)
    local link = {}
    link.InputIdx = input_idx
    link.InputSlot = input_slot
    link.OutputIdx = output_idx
    link.OutputSlot = output_slot
    return link
end

-- 0 - int

function nodes_s.Connect(typee,value,isInp)
    local conn = {}
    conn.Type = typee
    conn.Value = value
    if isInp then
        conn.linkIdx = nil
    else
        conn.linkIdxT = {}
    end
    return conn
end

--Nodes
function nodes_s.NodeIntValue(pos,value)
    local node = nodes_s.Node(0,pos,{},{
        nodes_s.Connect(1,new.int(value or 0))
    })
    node.Name = new.char[32]("Int value")
    node.create = function()
        imgui.Text("%s", node.Name);
        imgui.NewLine()
        imgui.InputInt("", node.Outputs[1].Value);
    end
    return node
end
function nodes_s.NodeReadInt(pos)
    local node = nodes_s.Node(1,pos,{
        nodes_s.Connect(1,new.int(value or 0),true)
    },{})
    node.Name = new.char[32]("Int read")
    node.create = function()
        imgui.Text("%s", node.Name);
        imgui.NewLine()
        if not node.Inputs[1].linkIdx then
            imgui.InputInt("", node.Inputs[1].Value);
        else
            imgui.Text("int")
        end
    end
    return node
end
function nodes_s.NodeReadFloat(pos)
    local node = nodes_s.Node(2,pos,{
        nodes_s.Connect(2,new.float(value or 0),true)
    },{})
    node.Name = new.char[32]("Int read")
    node.create = function()
        imgui.Text("%s", node.Name);
        imgui.NewLine()
        imgui.InputFloat("", node.Inputs[1].Value);
    end
    return node
end

function nodes_s.NodeToTarget(pos)
    local node = nodes_s.Node(2,pos,{
        nodes_s.Connect(0,nil,true),
        nilo
    },{})
    node.Name = new.char[32]("To Target")
    node.target = new.int()
    node.create = function()
        imgui.Text("%s", node.Name);
        imgui.NewLine()
        imgui.NewLine()
        imgui.Combo("target", node.target,new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets);
    end
    return node
end

function nodes_s.NodeTargetEnd(pos,value)
    local node = nodes_s.Node(3,pos,{},{
        nilo,
        nodes_s.Connect(0)
    })
    node.Name = new.char[32]("End target")
    node.target = new.int()
    node.create = function()
        imgui.Text("%s", node.Name);
        imgui.NewLine()
        imgui.Combo("target", node.target,new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets);
        imgui.NewLine()
    end
    return node
end

-- State
uID = 0
nodes_s.nodes = {}
nodes_s.links = {}
scrolling = imgui.ImVec2(0.0, 0.0);
inited = false
show_grid = new.bool(true)
node_selected = -1
output_selected = -1
state = 0
nodePopup = -1

STATE_NONE = 0
STATE_DRAG_OUTPUT = 1
STATE_DRAG_INPUT = 2

local nodes_name = {
    {"End target",nodes_s.NodeTargetEnd},
    {"To Target",nodes_s.NodeToTarget}
}

if not inited then
    inited = true;
end


imgui.OnFrame(function() return nodes_s.show[0] end,
function()

    local style = imgui.GetStyle();
    local colors = style.Colors
    local clr = imgui.Col

    --imgui.ShowDemoWindow(new.bool(true))

    io = imgui.GetIO()
    imgui.SetNextWindowSize(imgui.ImVec2(700,600),imgui.Cond.FirstUseEver)

    imgui.Begin("Example: Custom Node Graph", nodes_s.show)

    -- Draw a list of nodes_s.nodes on the left side
    local open_context_menu = false
    local node_hovered_in_list = -1
    local node_hovered_in_scene = -1
    imgui.BeginGroup();

    NODE_SLOT_RADIUS = 4.0;
    NODE_WINDOW_PADDING = imgui.ImVec2(8.0, 8.0);

    -- Create our child canvas
    imgui.Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
    imgui.SameLine(imgui.GetWindowWidth() - 100);
    imgui.Checkbox("Show grid", show_grid);
    imgui.PushStyleVarVec2(imgui.StyleVar.FramePadding, imgui.ImVec2(1, 1));
    imgui.PushStyleVarVec2(imgui.StyleVar.WindowPadding, imgui.ImVec2(0, 0));
    imgui.PushStyleColor(imgui.Col.ChildBg, colors[clr.ChildBg]);
    imgui.BeginChild("scrolling_region", imgui.ImVec2(0, 0), true, imgui.WindowFlags.NoScrollbar + imgui.WindowFlags.NoMove);
    imgui.PopStyleVar(); -- WindowPadding
    imgui.PushItemWidth(120.0);

    local offset = imgui.GetCursorScreenPos() + scrolling;
    local draw_list = imgui.GetWindowDrawList()

    draw_list:ChannelsSplit(3);
    draw_list:ChannelsSetCurrent(0); -- Background

    -- Display grid
    if show_grid[0] then
        local GRID_COLOR = imgui.ColorConvertFloat4ToU32(colors[clr.Border]);
        local GRID_SZ = new.float(64.0)
        local win_pos = imgui.GetCursorScreenPos();
        local canvas_sz = imgui.GetWindowSize();
        for x = scrolling.x % GRID_SZ[0],canvas_sz.x, GRID_SZ[0] do
            draw_list:AddLine(imgui.ImVec2(x, 0.0) + win_pos, imgui.ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
        end
        for y = scrolling.y % GRID_SZ[0], canvas_sz.y,GRID_SZ[0] do
            draw_list:AddLine(imgui.ImVec2(0.0, y) + win_pos, imgui.ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
        end
    end

    local node_rect_min
    -- Display nodes_s.nodes
    for node_idx = 1,#nodes_s.nodes do
        local node = nodes_s.nodes[node_idx];
        imgui.PushIDInt(node.ID[0]);
        node_rect_min = offset + node.Pos;

        -- Display nodes_s.links
        for out_idx = 1, #node.Outputs do
            if node.Outputs[out_idx] ~= nilo then
                for TlinkIdx = 1,#node.Outputs[out_idx].linkIdxT do
                    local link = nodes_s.links[node.Outputs[out_idx].linkIdxT[TlinkIdx]]
                    node_inp = nodes_s.nodes[link.InputIdx];
                    local p1 = offset + GetOutputSlotPos(node,out_idx);
                    local p2 = offset + GetInputSlotPos(node_inp,link.InputSlot);
                    draw_list:AddBezierCurve(p1, p1 + imgui.ImVec2(50, 0), p2 + imgui.ImVec2(-50, 0), p2, imgui.ColorConvertFloat4ToU32(imgui.ImVec4(200, 200, 100, 255)), 3.0);
                end
            end
        end

        if state == STATE_DRAG_OUTPUT and imgui.IsMouseDragging(0) then
            local node_out = nodes_s.nodes[node_selected]
            p1 = offset + GetOutputSlotPos(node_out,output_selected);
            p2 = imgui.GetMousePos()
            draw_list:AddBezierCurve(p1, p1 + imgui.ImVec2(50, 0), p2 + imgui.ImVec2(-50, 0), p2, imgui.ColorConvertFloat4ToU32(imgui.ImVec4(200, 200, 100, 255)), 3.0);
        elseif state == STATE_DRAG_OUTPUT then
            local node_out = nodes_s.nodes[node_selected]
            for input_idx = 1,#node.Inputs do
                inp_pos = offset + GetInputSlotPos(node,input_idx)
                if nodes_s.GetDistVec2(imgui.GetMousePos(),inp_pos) <= 8 then
                    if node.Inputs[input_idx].Type == node_out.Outputs[output_selected].Type then
                        if node.Inputs[input_idx].linkIdx then
                            local link = nodes_s.links[node.Inputs[input_idx].linkIdx]
                            for i = 1, #nodes_s.nodes[link.OutputIdx].Outputs[link.OutputSlot].linkIdxT do
                                if nodes_s.nodes[link.OutputIdx].Outputs[link.OutputSlot].linkIdxT[i] == node.Inputs[input_idx].linkIdx then
                                    table.remove(nodes_s.nodes[link.OutputIdx].Outputs[link.OutputSlot].linkIdxT,i)
                                    break
                                end
                            end

                            nodes_s.links[node.Inputs[input_idx].linkIdx] = nil --!!!
                            node.Inputs[input_idx].linkIdx = nil
                            print(db.tostring(nodes_s.links))
                        end
                        local link = NodeLink(node_idx,input_idx,node_selected,output_selected)
                        nodes_s.links[#nodes_s.links+1] = link
                        node.Inputs[input_idx].linkIdx = #nodes_s.links
                        node_out.Outputs[output_selected].linkIdxT[#node_out.Outputs[output_selected].linkIdxT + 1] = #nodes_s.links
                    end
                end
            end
        end

        -- Display node contents first
        draw_list:ChannelsSetCurrent(1); -- Foreground
        old_any_active = imgui.IsAnyItemActive();
        imgui.SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
        imgui.BeginGroup(); -- Lock horizontal position
        node.create()
        imgui.EndGroup();

        -- Save the size of what we have emitted and whether any of the widgets are being used
        node.Size = imgui.GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
        node_rect_max = node_rect_min + node.Size;

        for slot_idx = 1, #node.Inputs do
            if node.Inputs[slot_idx] ~= nilo then
                imgui.PushIDInt(slot_idx)
                size_input = imgui.ImVec2(NODE_SLOT_RADIUS,NODE_SLOT_RADIUS)
                imgui.SetCursorScreenPos(offset + GetInputSlotPos(node,slot_idx) - size_input);
                imgui.InvisibleButton("input", size_input * 2)
                local color_dot
                if node.Inputs[slot_idx].Type == 0 then
                    color_dot = nodes_s.COLOR_U32(150, 150, 150)
                elseif node.Inputs[slot_idx].Type == 1 then
                    color_dot = nodes_s.COLOR_U32(0, 132, 208)
                end
                color_dot = nodes_s.fif(imgui.IsItemHovered(), nodes_s.COLOR_U32(20, 20, 20) + color_dot, color_dot)
                if state == STATE_DRAG_OUTPUT and nodes_s.nodes[node_selected].Outputs[output_selected].Type == node.Inputs[slot_idx].Type then
                    color_dot = color_dot + nodes_s.COLOR_U32(30,30,30)
                end
                draw_list:AddCircleFilled(offset + GetInputSlotPos(node,slot_idx), NODE_SLOT_RADIUS, color_dot)
                imgui.PopID()
            end
        end
        for slot_idx = 1, #node.Outputs do
            if node.Outputs[slot_idx] ~= nilo then
                imgui.PushIDInt(slot_idx)
                size_input = imgui.ImVec2(NODE_SLOT_RADIUS,NODE_SLOT_RADIUS)
                imgui.SetCursorScreenPos(offset + GetOutputSlotPos(node,slot_idx) - size_input);
                imgui.InvisibleButton("output", size_input * 2)
                if imgui.IsItemActive() then
                    output_selected = slot_idx
                    state = STATE_DRAG_OUTPUT
                    node_selected = node.ID[0]
                end
                local color_dot
                if node.Outputs[slot_idx].Type == 0 then
                    color_dot = nodes_s.COLOR_U32(150, 150, 150)
                elseif node.Outputs[slot_idx].Type == 1 then
                    color_dot = nodes_s.COLOR_U32(0, 132, 208)
                end
                local color_dot = nodes_s.fif(imgui.IsItemHovered(), nodes_s.COLOR_U32(20, 20, 20) - color_dot, color_dot)
                draw_list:AddCircleFilled(offset + GetOutputSlotPos(node,slot_idx), NODE_SLOT_RADIUS, color_dot)
                imgui.PopID()
            end
        end

        -- Display node box
        draw_list:ChannelsSetCurrent(0); -- Background
        imgui.SetCursorScreenPos(node_rect_min);
        imgui.InvisibleButton("node", node.Size);
        if imgui.IsItemHovered() then
            node_hovered_in_scene = node.ID[0];
        end
        node_moving_active = imgui.IsItemActive();
        if node_widgets_active or node_moving_active then
            node_selected = node.ID[0];
        end
        if node_moving_active and imgui.IsMouseDragging(0) then
            node.Pos = node.Pos + io.MouseDelta
        end

        local node_bg_color = nodes_s.fif( node_hovered_in_scene == node.ID[0] or node_hovered_in_list == node.ID[0], nodes_s.COLOR_U32(75, 75, 75), nodes_s.COLOR_U32(59, 59, 59))
        local node_head_color = node.color or nodes_s.COLOR_U32(69, 67, 140)
        local node_head_color = nodes_s.fif(node_hovered_in_scene == node.ID[0] or node_hovered_in_list == node.ID[0] , node_head_color+nodes_s.COLOR_U32(1, 1, 1), node_head_color)
        draw_list:AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0);
        draw_list:AddRectFilled(node_rect_min, imgui.ImVec2(node_rect_max.x,node_rect_min.y+30), node_head_color, 4.0, imgui.DrawCornerFlags.Top);
        draw_list:ChannelsSetCurrent(2); -- Foreground

        imgui.PopID();
    end
    if state == STATE_DRAG_OUTPUT and not imgui.IsMouseDragging(0) then
        state = STATE_NONE
    end
    draw_list:ChannelsMerge();

    -- Open context menu
    if imgui.IsMouseReleased(1) then
        if node_hovered_in_scene ~= -1 then
            nodePopup = node_hovered_in_scene
        end
        imgui.OpenPopup("context_menu")
    end

    -- Draw context menu
    imgui.PushStyleVarVec2(imgui.StyleVar.WindowPadding, imgui.ImVec2(8, 8));
    if imgui.BeginPopup("context_menu") then
        node = nodes_s.fif( nodePopup ~= -1, nodes_s.nodes[nodePopup], nil)
        scene_pos = imgui.GetMousePosOnOpeningCurrentPopup() - offset;
        if node then
            imgui.Text("Node '%s'", node.Name);
            imgui.Separator();
            if imgui.MenuItemBool("Rename..", NULL, false, false) then end
            if imgui.MenuItemBool("Delete", NULL, false, false) then end
            if imgui.MenuItemBool("Copy", NULL, false, false) then end
        else
            if (imgui.BeginMenu("Add")) then
                
                for n = 1,#nodes_name do
                    if imgui.MenuItemBool(nodes_name[n][1]) then
                        nodes_s.nodes[#nodes_s.nodes+1] = nodes_name[n][2](imgui.GetMousePos()-offset)
                        imgui.CloseCurrentPopup()
                    end
                end
                
                imgui.EndMenu()
            end
            if imgui.MenuItemBool("Paste", NULL, false, false) then end
        end
        imgui.EndPopup();
    elseif nodePopup ~= -1 then
        nodePopup = -1
    end
    imgui.PopStyleVar();

    -- Scrolling
    if imgui.IsWindowHovered() and not imgui.IsAnyItemActive() and imgui.IsMouseDragging(2, 0.0) then
        scrolling = scrolling + io.MouseDelta;
    end

    imgui.PopItemWidth();
    imgui.EndChild();
    imgui.PopStyleColor();
    imgui.PopStyleVar();
    imgui.EndGroup();

    imgui.End()

end)

return nodes_s