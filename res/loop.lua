if gMainLoopCoro then
	local ok, msg = resume(gMainLoopCoro)
	if not ok then
		print(msg)
		gMainLoopCoro = nil
	end
end
