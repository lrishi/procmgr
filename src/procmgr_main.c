#include <procmgr_debug.h>
#include <procmgr_context.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>

#define LOCK_FILENAME "/tmp/.procmgr.lock"

#define PREDEBUG_ERR(_fmt, ...) \
    fprintf(stderr, "\n%s()[%d]: "_fmt, __FUNCTION__, \
           __LINE__, ##__VA_ARGS__);

void
procmgr_main_ensure_singleton (void)
{
    FILE *fp = NULL;
    if ((fp = fopen(LOCK_FILENAME, "w+")) == NULL) {
        PREDEBUG_ERR("Failed to initialize process with error code=0x%x",
                     errno);
        exit(errno);
    }

    if (flock(fileno(fp), LOCK_EX | LOCK_NB) != 0) {
        PREDEBUG_ERR("Another instance of the process is already running");
        PREDEBUG_ERR("Failed to initialize process with error code=0x%x",
                     errno);
        exit(errno);
    }
}


void
procmgr_main_reassign_parent (void)
{
    pid_t pid = fork();

    if (pid > 0) {
        exit(0);
    } else if (pid < 0) {
        PREDEBUG_ERR("Failed to initialize process with error code=0x%x",
                     errno);
        exit(errno);
    }
}

void
procmgr_event_loop (void)
{
    while (TRUE) {
        sleep(100);
    }
}

int
main ()
{
    (void) procmgr_main_reassign_parent();
    (void) procmgr_main_ensure_singleton();
    (void) procmgr_context_init();
    (void) procmgr_debug_init("/tmp/");
    
    (void) procmgr_event_loop();
    
    return 0;
}
