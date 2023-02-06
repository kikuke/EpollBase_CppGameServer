//use BitField
struct
{
    unsigned int first : 9;
    unsigned int second : 7;
    unsigned int may_straddle : 30;
    unsigned int last : 18;
} tricky_bits;