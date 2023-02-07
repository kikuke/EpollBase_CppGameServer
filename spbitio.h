#ifndef SPBIT_IO
#define SPBIT_IO

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

//return ByteOffset
size_t BitToByteIdx(size_t bitIdx);

size_t BitToByteIdxRem(size_t bitIdx);

size_t GetBitLen(size_t sBitIdx, size_t eBitIdx);

//return bits to int value.
//use it uInt or flagData.
unsigned int BitsToUInt(size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

//Set One Bit
size_t SetBit(size_t bitIdx,int bool, void* buf, size_t bufSize);

//return set bits
//copy bitMask to sBitIdx ~ eBitIdx in Buf. If need set manyBit, more efficient than SetBit.
size_t SetBitsByMask(unsigned int bitMask, size_t sBitIdx, size_t eBitIdx, void* buf, size_t bufSize);

//0 is false. other is true
int CheckBitMask(unsigned int bitMask, size_t sBitIdx, size_t eBitIdx, const void* buf, size_t bufUseSize);

//return convert num. convert bits to Bools. Available maximum convert is sizeof(unsigned int) * 4
size_t BitsToBools(size_t sBitIdx, int* bool, size_t boolSize, const void* srcbuf, size_t srcBufUseSize);

#ifdef __cplusplus
}
#endif

#endif