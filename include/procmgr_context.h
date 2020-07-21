#ifndef __PROCMGR_CONTEXT_H__
#define __PROCMGR_CONTEXT_H__

#include <procmgr_debug.h>

#include <sys/types.h>

typedef struct procmgr_context_st_ {
    pid_t                    mypid;
    procmgr_debug_context_st debug;
    
} procmgr_context_st;

procmgr_context_st*
procmgr_context(void);

void
procmgr_context_init(void);

#endif //__PROCMGR_CONTEXT_H__
