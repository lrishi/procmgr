#include <procmgr_builtins.h>
#include <procmgr_proc.h>

#include <string.h>
#include <sys/wait.h>

void
procmgr_builtins_handle (char *cmd)
{
    pid_t pid;
    if (strcmp(cmd, "show process") == 0) {
        printf("\n");
        char *args[] = {
            "aux",
        };
        procmgr_process_create("/usr/bin/ps", 1, args, &pid);
        waitpid(pid, NULL, 0);
        return;
    }
    if (strcmp(cmd, "show kernel") == 0) {
        printf("\n");
        char *args[] = {
            "-a",
        };
        procmgr_process_create("/usr/bin/uname", 1, args, &pid);
        waitpid(pid, NULL, 0);
        return;
    }
}
