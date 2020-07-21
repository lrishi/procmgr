#ifndef __PROCMGR_H__
#define __PROCMGR_H__

#include <procmgr_types.h>
#include <procmgr_error.h>

pmerr
procmgr_create_process(char    *executable,
                       int      argc,
                       char   **args,
                       pid_t   *pid);

pmerr
procmgr_process_send_signal(pid_t   pid,
                            int     signal);

#endif // __PROCMGR_H__
