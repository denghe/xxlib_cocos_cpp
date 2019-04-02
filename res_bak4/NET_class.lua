
NET_PkgGenMd5_Value = 'd1df1395fb14750d0792453934c01b74'

--[[
通用返回
]]
NET_Generic_Success = {
    typeName = "NET_Generic_Success",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = NET_Generic_Success
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( NET_Generic_Success )
--[[
通用错误返回
]]
NET_Generic_Error = {
    typeName = "NET_Generic_Error",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = NET_Generic_Error
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.number = 0 -- Int64
        o.message = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        o.number = bb:ReadInt64()
        o.message = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.number )
        bb:WriteObject( o.message )
    end
}
BBuffer.Register( NET_Generic_Error )
--[[
心跳保持兼延迟测试 -- 请求
]]
NET_Generic_Ping = {
    typeName = "NET_Generic_Ping",
    typeId = 5,
    Create = function()
        local o = {}
        o.__proto = NET_Generic_Ping
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.ticks = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        o.ticks = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.ticks )
    end
}
BBuffer.Register( NET_Generic_Ping )
--[[
心跳保持兼延迟测试 -- 回应
]]
NET_Generic_Pong = {
    typeName = "NET_Generic_Pong",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = NET_Generic_Pong
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.ticks = 0 -- Int64
        return o
    end,
    FromBBuffer = function( bb, o )
        o.ticks = bb:ReadInt64()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt64( o.ticks )
    end
}
BBuffer.Register( NET_Generic_Pong )
NET_Test_Foo = {
    typeName = "NET_Test_Foo",
    typeId = 14,
    Create = function()
        local o = {}
        o.__proto = NET_Test_Foo
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        return o
    end,
    FromBBuffer = function( bb, o )
    end,
    ToBBuffer = function( bb, o )
    end
}
BBuffer.Register( NET_Test_Foo )
NET_Test_Player = {
    typeName = "NET_Test_Player",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = NET_Test_Player
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        o.owner = MakeRef() -- Ref_NET_Test_Scene
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
        o.owner = MakeRef( ReadObject( bb ) )
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
        bb:WriteObject( o.owner.Lock() )
    end
}
BBuffer.Register( NET_Test_Player )
NET_Test_Scene = {
    typeName = "NET_Test_Scene",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = NET_Test_Scene
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.fishs = null -- List_NET_Test_Fish_
        o.players = null -- List_Ref_NET_Test_Player_
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.fishs = ReadObject( bb )
        o.players = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.fishs )
        WriteObject( bb, o.players )
    end
}
BBuffer.Register( NET_Test_Scene )
List_NET_Test_Fish_ = {
    typeName = "List_NET_Test_Fish_",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = List_NET_Test_Fish_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadObject
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_NET_Test_Fish_ )
NET_Test_Fish = {
    typeName = "NET_Test_Fish",
    typeId = 10,
    Create = function()
        local o = {}
        o.__proto = NET_Test_Fish
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.id = 0 -- Int32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.id = bb:ReadInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteInt32( o.id )
    end
}
BBuffer.Register( NET_Test_Fish )
List_Ref_NET_Test_Player_ = {
    typeName = "List_Ref_NET_Test_Player_",
    typeId = 11,
    Create = function()
        local o = {}
        o.__proto = List_Ref_NET_Test_Player_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadObject
		for i = 1, len do
			o[ i ] = MakeRef( f( bb ) )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ].Lock() )
		end
    end
}
BBuffer.Register( List_Ref_NET_Test_Player_ )
NET_Test_EnterSuccess = {
    typeName = "NET_Test_EnterSuccess",
    typeId = 12,
    Create = function()
        local o = {}
        o.__proto = NET_Test_EnterSuccess
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.players = null -- List_NET_Test_Player_
        o.scene = null -- NET_Test_Scene
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.players = ReadObject( bb )
        o.scene = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.players )
        WriteObject( bb, o.scene )
    end
}
BBuffer.Register( NET_Test_EnterSuccess )
List_NET_Test_Player_ = {
    typeName = "List_NET_Test_Player_",
    typeId = 13,
    Create = function()
        local o = {}
        o.__proto = List_NET_Test_Player_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadObject
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_NET_Test_Player_ )