function class(super_class)
    local cls = {}
	
	if super_class and type(super_class) == 'table' then
		-- our new class is a shallow copy of the base class!
		for i,v in pairs(super_class) do
			cls[i] = v
		end
		cls.__super_class = super_class
	end
	
    cls.__index = cls
	
    return setmetatable(
		cls, 
		{
			__call = function (c, ...)
				local instance = setmetatable({}, cls)
				if cls.__init then
					cls.__init(instance, ...)
				end
				return instance
			end
		}
	)
end
