
PKG_PkgGenMd5_Value = '616f8410ebbcc955a8038c36ddfb65f2'

PKG_Generic_Xxx = {
    typeName = "PKG_Generic_Xxx",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = PKG_Generic_Xxx
        o.__index = o
        o.__newindex = o


        o.ticks = null -- NullableInt32
        return o
    end,
    FromBBuffer = function( bb, o )
        o.ticks = bb:ReadNullableInt32()
    end,
    ToBBuffer = function( bb, o )
        bb:WriteNullableInt32( o.ticks )
    end
}
BBuffer.Register( PKG_Generic_Xxx )