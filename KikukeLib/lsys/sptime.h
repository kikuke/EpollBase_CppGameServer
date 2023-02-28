#ifndef SPTIME_H
#define SPTIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

double getTimeDist(const struct timeval* start, const struct timeval* end);

#ifdef __cplusplus
}
#endif

#endif