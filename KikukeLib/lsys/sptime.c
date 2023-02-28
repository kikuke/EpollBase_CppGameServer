#include "sptime.h"

double getTimeDist(const struct timeval* start, const struct timeval* end)
{
    double dist;

    dist = ((*end).tv_sec - (*start).tv_sec);
    dist += ((*end).tv_usec - (*start).tv_usec) / 1000000;

    return dist;
}