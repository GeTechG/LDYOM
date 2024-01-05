render = function()
    local a = ImVec2.new(20, 20)
    local b = true;
    isShow, b = ImGui.Begin("Hello, world!", b, 0)
    if (isShow) then
        ImGui.Text("This is some useful text.")
    end
    ImGui.End()
end