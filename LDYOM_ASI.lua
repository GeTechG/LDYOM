require "memory"

local show = 0

function main()
    while true do
        wait(0)
        if getCleoSharedVar(700) ~= show then
            show = getCleoSharedVar(700)
			if show == 1 then
				showCursor(true,true)
			else
				showCursor(false,false)
			end
        end
		if isKeyJustPressed(0x59) then
			print(getCleoSharedVar(800))
		end
    end
end