#ifndef _TRACK_LOAD_DATA_H
#define _TRACK_LOAD_DATA_H

#define MAX_CALL_NAME_LEN 100
struct SYSCALL_ENTRY {
    char cname[MAX_CALL_NAME_LEN];
    int value;
    int uses;
    struct SYSCALL_ENTRY *next;
};
typedef struct SYSCALL_ENTRY call;
call *head, *curr;

int load_syscall_data (void);
#include "load_syscall_data.c"

#endif
