#include <procmgr_types.h>
#include <procmgr_error.h>
#include <procmgr_proc.h>
#include <procmgr_debug.h>

#include <pty.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define READ_END 0
#define WRITE_END 1

pmerr
procmgr_process_create (char    *executable,
                        int      argc,
                        char   **args,
                        pid_t   *pid)
{
    pmerr   pe          = PM_EOK;
    char  **arguments   = NULL;

    pid_t   create_pid;
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        PROCMGR_LOG_ERROR("Failed to create named pipe");
        return errno;
    }
    
    if (fork() == 0) {
        
        if (dup2(pipefd[READ_END], STDIN_FILENO) == -1) {
            PROCMGR_LOG_ERROR("Failed to duplicate named pipe");
            return errno;
        }

        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);
        
        char *args[] = {
            "/usr/bin/more",
            NULL
        };

        execvp("/usr/bin/more", args);
    }

    create_pid = fork();
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
        
        if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1) {
            PROCMGR_LOG_ERROR("Failed to duplicate named pipe");
            return -1;
        }

        // Close the pipe now that we've duplicated it
        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);

        if (execvp(executable, arguments) == -1) {
            /* Notify error (TODO) then exit */
            exit(errno);
        }

        /* We must never reach here */
        exit(ENOTSUP);

    } else {
        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);
        /* Parent process */
        if (pid != NULL) {
            *pid = create_pid;
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

