#include <procmgr_debug.h>
#include <procmgr_shell.h>
#include <procmgr_context.h>

#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/file.h>

#define LOCK_FILENAME "/tmp/.procmgr.lock"

#define PREDEBUG_ERR(_fmt, ...) \
    fprintf(stderr, "\n%s()[%d]: "_fmt, __FUNCTION__, \
           __LINE__, ##__VA_ARGS__);

void
exit_to_parent (int err)
{
    printf("\nActivating pid: %d and exiting...\n", procmgr_context()->parent_shell_pid);
    kill(procmgr_context()->parent_shell_pid, SIGCONT);
    exit(err);
}

void
procmgr_main_ensure_singleton (void)
{
    FILE *fp = NULL;
    if ((fp = fopen(LOCK_FILENAME, "w+")) == NULL) {
        PREDEBUG_ERR("Failed to initialize process with error code=0x%x",
                     errno);
        exit_to_parent(errno);
    }

    if (flock(fileno(fp), LOCK_EX | LOCK_NB) != 0) {
        PREDEBUG_ERR("Another instance of the process is already running");
        PREDEBUG_ERR("Failed to initialize process with error code=0x%x",
                     errno);
        exit_to_parent(errno);
    }
}


void
procmgr_main_reassign_parent (void)
{
    procmgr_context()->parent_shell_pid = getppid();    
    int master;
    //pid_t pid = forkpty(&master, NULL, NULL, NULL);
    pid_t pid = fork();
    printf("Came here0");
    if (pid > 0) {
        printf("Came here1");
        /* Free up stdout on current tty by killing parent shell fork */
        kill(procmgr_context()->parent_shell_pid, SIGSTOP);
        exit(0);
    } else if (pid < 0) {
        PREDEBUG_ERR("Failed to initialize process with error code=0x%x",
                     errno);
        exit_to_parent(errno);
    }
    printf("Came here2");
}

void
procmgr_event_loop (void)
{
    while (FALSE) {
        sleep(2);
    }
    pthread_join(procmgr_context()->shell_tid, NULL);
}

int
main ()
{
    (void) procmgr_main_reassign_parent();
    (void) procmgr_main_ensure_singleton();

    (void) procmgr_context_init();
    (void) procmgr_debug_init("/tmp/");
    
    (void) procmgr_shell_start();    

    (void) procmgr_event_loop();
    
    exit_to_parent(0);
}
