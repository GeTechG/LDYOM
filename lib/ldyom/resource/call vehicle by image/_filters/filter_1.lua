local func = function(model)
	local t = {403,514,515,485,583,606,607,531,552,525,435,450,584,591,606,607,608,610,611};
	for i=1, #t do 
		if model == t[i] then 
			return true
		end
	end
	return false
end

return {
	name = 'Trucks & Trailers';
	filter = func;
}