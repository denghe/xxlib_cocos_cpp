return function()
	if gMainLoopCoro then
		local ok, msg = resume(gMainLoopCoro)
		if not ok then
			print("resume not ok, msg = "..msg)
			gMainLoopCoro = nil
		end
	end
end