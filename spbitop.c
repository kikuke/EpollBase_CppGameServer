#include <string.h>

#include "spbitop.h"

size_t BitToByteIdx(size_t bitIdx)
{
    return bitIdx/8;
}

size_t BitToByteIdxRem(size_t bitIdx)
{
    return bitIdx%8;
}

size_t GetBitLen(size_t sBitIdx, size_t eBitIdx)
{
    return eBitIdx - sBitIdx + 1;
}

unsigned int BitsToUInt(size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize)
{
    unsigned char brokenData[5];
    unsigned int data;
    size_t bitLen = GetBitLen(sBitIdx, eBitIdx);
    size_t sRem = BitToByteIdxRem(sBitIdx);

    if(bitLen > sizeof(unsigned int)*8)
        return -1;

    memcpy(brokenData, ((unsigned char*)buf)[BitToByteIdx(sBitIdx)], 5);
    data = (int)brokenData << sRem;
    data = data >> sizeof(unsigned int)*8 - bitLen;

    if(bitLen > 8*3 && sRem > 0){
        brokenData[4] = brokenData[4] >> (8 - (BitToByteIdxRem(eBitIdx) + 1));
        data |= (unsigned int)brokenData[4];
    }

    return data;
}

size_t BitsToBools(size_t sBitIdx, int* bool, size_t boolSize, const void* srcbuf, size_t srcBufUseSize)
{
    unsigned int data = BitsToUInt(sBitIdx, sBitIdx + boolSize - 1, srcbuf, srcBufUseSize);

    if(data == -1)
        return;

    for(int i=0; i<boolSize; i++){
        bool[i] = (data >> (boolSize - (i + 1))) & 1;
    }

    return boolSize;
}