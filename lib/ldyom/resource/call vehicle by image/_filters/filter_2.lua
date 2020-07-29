local func = function(model)
	local t = {438,420,416,407,490,544,427,596,597,598,599,601};
	for i=1, #t do 
		if model == t[i] then 
			return true
		end
	end
	return false
end

return {
	name = 'Misson';
	filter = func;
}