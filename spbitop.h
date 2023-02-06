#ifndef SPBIT_OP
#define SPBIT_OP

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

//return convert num. convert bits to Bools. Available maximum convert is sizeof(unsigned int) * 4
size_t BitsToBools(size_t sBitIdx, int* bool, size_t boolSize, const void* srcbuf, size_t srcBufUseSize);

//역변환 함수들도 만들기

#ifdef __cplusplus
}
#endif

#endif