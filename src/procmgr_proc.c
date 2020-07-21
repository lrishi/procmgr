#include <procmgr_types.h>
#include <procmgr_error.h>


#include <stdlib.h>
#include <string.h>
#include <unistd.h>


pmerr
procmgr_create_process (char    *executable,
                        int      argc,
                        char   **args,
                        pid_t   *pid)
{
    pmerr   pe          = PM_EOK;
    pid_t   create_pid  = fork();
    char  **arguments   = NULL;


    if (create_pid == 0) {
        /* New Process */
        
        arguments = calloc(argc + 2, sizeof(char*));

        if (arguments == NULL) {
            /* Notify error (TODO) then exit */
            exit(ENOMEM);
        }

        arguments[0] = strdup(executable);

        for (int i = 0; i < argc; i++) {
            arguments[i + 1] = strdup(args[i]);
        }

        if (execvp(executable, arguments) == -1) {
            /* Notify error (TODO) then exit */
            exit(errno);
        }

        /* We must never reach here */
        exit(ENOTSUP);

    } else {
        /* Parent process */
        
        if (errno != 0) {
            pe = errno;
        } else {
            if (pid != NULL) {
                *pid = create_pid;
            }
        }
    }
    
    return pe;
}


pmerr
procmgr_process_send_signal (pid_t   pid,
                             int     signal)
{
    return ENOTSUP;
}

