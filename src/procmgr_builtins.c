#include <procmgr_builtins.h>
#include <procmgr_proc.h>

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void
procmgr_builtins_handle (char *cmd)
{
    char *token = NULL;
    char *cmds = strdup(cmd);
    if (cmd == NULL || strlen(cmd) == 0) {
        return;
    }
    if (strcmp(token = strtok(cmd, " "), "exec") == 0) {
        int argc = 0;
        for (int i = 0; i < strlen(cmds); i++) {
            if (cmds[i] == ' ') {
                argc++;
            }
        }
        argc--;
        token = strtok(NULL, " ");
        char **args = calloc(argc, sizeof(char*));
        for (int i = 0; i < argc; i++) {
            args[i] = strtok(NULL, " ");
        }
        procmgr_process_create_v2(token, argc, args, NULL);
        free(cmds);
    } 
    if (strcmp(cmd, "show process") == 0) {
        printf("\n");
        char *args[] = {
            "aux",
        };
        procmgr_process_create("/usr/bin/ps", 1, args, NULL);
    }
    if (strcmp(cmd, "show kernel") == 0) {
        printf("\n");
        char *args[] = {
            "-a",
        };
        procmgr_process_create("/usr/bin/uname", 1, args, NULL);
    }
    while (wait(NULL) >= 0);
}
