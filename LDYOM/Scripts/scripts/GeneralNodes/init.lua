local Test = bitser.registerClass(class("test"));

function Test:initialize()
    self.apple = 5;
end

local test = Test:new();


log(bitser.dumps(test));