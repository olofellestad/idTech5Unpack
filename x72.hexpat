import std.string;

using str32 = std::string::SizedString< u32 >;

struct x24_0
{
    u8      unknown01[24];
};

struct x24
{
    be u32  unknown01;  // flags?
    str32   typeName;
    str32   srcName;
    str32   dstName;
    be u32  dstOffset;
    be u32  srcSize;
    be u32  dstSize;
    
    be u32  unknown02;
    x24_0   unknown03[unknown02];
    u8      unknown04[20];
};

x24 x @ 0x00;