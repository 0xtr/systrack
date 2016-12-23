#ifndef SYS_HIST_AVG_H
#define SYS_HIST_AVG_H

struct {
    int runs;
    int total;
} data;
FILE *cmdinfo = NULL;
extern int get_historical_avg (int args, char **cmd);
#include "get_historical_avg.c"

#endif
