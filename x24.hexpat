import std.string;

using str32 = std::string::SizedString< u32 >;

struct x24
{
    u32     unknown01;  // flags?
    str32   typeName;
    str32   srcName;
    str32   dstName;
    u32     dstOffset;
    u32     srcSize;
    u32     dstSize;
    
    u8      unknown02[24];
};

x24 x @ 0x00;