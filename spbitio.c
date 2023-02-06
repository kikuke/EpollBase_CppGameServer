#include <string.h>

#include "spbitio.h"

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
    data = *(unsigned int*)brokenData << sRem;
    data >>= (sizeof(unsigned int)*8 - bitLen);

    if(bitLen > 8*3 && sRem > 0){
        brokenData[4] >>= (8 - (BitToByteIdxRem(eBitIdx) + 1));
        data |= (unsigned char)brokenData[4];
    }

    return data;
}

size_t SetBitsByMask(unsigned int bitMask, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufSize)
{
    unsigned char brokenData[5];
    unsigned int initMask;
    unsigned int tempMask;
    size_t bitLen = GetBitLen(sBitIdx, eBitIdx);
    size_t sRem = BitToByteIdxRem(sBitIdx);

    if(bitLen > sizeof(unsigned int)*8)
        return -1;

    memcpy(brokenData, ((unsigned char*)buf)[BitToByteIdx(sBitIdx)], 5);

    initMask = -1;
    initMask <<= sRem;
    if(bitLen > 8*3 && sRem > 0){
        initMask >>= sRem;

        brokenData[4] <<= BitToByteIdxRem(eBitIdx)+1;
        brokenData[4] >>= BitToByteIdxRem(eBitIdx)+1;
    } else {
        initMask >>= (sizeof(unsigned int)*8 - bitLen);
        initMask <<= ((sizeof(unsigned int)*8 - bitLen) - sRem);
    }
    initMask ^= 1;
    *(unsigned int*)brokenData &= initMask;

    if(bitLen > 8*3 && sRem > 0){
        tempMask = bitMask >> (BitToByteIdxRem(eBitIdx) + 1);
        *(unsigned int*)brokenData |= tempMask;

        tempMask = bitMask << (sizeof(unsigned int)*8 - (BitToByteIdxRem(eBitIdx) + 1));
        tempMask >>= (sizeof(unsigned int)-1)*8;
        brokenData[4] |= (unsigned char)tempMask;
    } else {
        bitMask <<= ((sizeof(unsigned int)*8 - bitLen) - sRem);
        *(unsigned int*)brokenData |= bitMask;
    }

    memcpy(&buf[BitToByteIdx(sBitIdx)], brokenData, 5);

    return bitLen;
}

int CheckBitMask(unsigned int bitMask, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize)
{
    unsigned int data = BitsToUInt(sBitIdx, eBitIdx, buf, bufUseSize);

    return bitMask & data;
}

size_t BitsToBools(size_t sBitIdx, int* bool, size_t boolSize, const void* srcbuf, size_t srcBufUseSize)
{
    unsigned int data = BitsToUInt(sBitIdx, sBitIdx + boolSize - 1, srcbuf, srcBufUseSize);

    if(data == -1)
        return;

    for(int i=0; i<boolSize; i++){
        bool[i] = ((data >> (boolSize - (i + 1))) & 1);
    }

    return boolSize;
}