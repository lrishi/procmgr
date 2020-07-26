#ifndef __PROCMGR_PROC_H__
#define __PROCMGR_PROC_H__

#include <procmgr_types.h>
#include <procmgr_error.h>

pmerr
procmgr_process_create(char    *executable,
                       int      argc,
                       char   **args,
                       pid_t   *pid);

pmerr
procmgr_process_send_signal(pid_t   pid,
                            int     signal);

#endif // __PROCMGR_PROC_H__
