#ifndef SPTIME_H
#define SPTIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

double getTimeDist(struct timeval* start, struct timeval* end);

#ifdef __cplusplus
}
#endif

#endif