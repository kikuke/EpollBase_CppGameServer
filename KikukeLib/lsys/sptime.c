#include "sptime.h"

double getTimeDist(struct timeval* start, struct timeval* end)
{
    double dist;

    dist = ((*end).tv_sec - (*start).tv_sec);
    dist += ((*end).tv_usec - (*start).tv_usec) / 1000000;

    return dist;
}