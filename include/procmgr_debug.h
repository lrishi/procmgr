#ifndef __PROCMGR_DEBUG_H__
#define __PROCMGR_DEBUG_H__

#include <procmgr_error.h>
#include <stdio.h>

typedef struct procmgr_debug_context_st_ {
    char           *logdir;
    volatile struct {
        bool        lock;
        uint32_t    index;
        FILE       *file;
    } file;
    struct {
        bool        log_to_file:1;
        bool        log_to_syslog:1;
        bool        log_to_console:1;
    } flags;
} procmgr_debug_context_st;

pmerr
procmgr_debug_init(char *logdir);

void
procmgr_debug_log_to_syslog(bool yes);

void
procmgr_debug_log_to_console(bool yes);

void
procmgr_debug_log_to_file(bool yes);


#endif // __PROCMGR_DEBUG_H__
