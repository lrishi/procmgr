#include <procmgr_types.h>
#include <procmgr_debug.h>
#include <procmgr_error.h>
#include <procmgr_context.h>
#include <procmgr_utils.h>
#include <procmgr_pam.h>

#include <ctype.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>

#define CMD_LENGTH_MAX  1024
static char cmd[CMD_LENGTH_MAX] = {'\0'};
static int  current_length = 0;


static char
getch ()
{
   struct termios oldtc;
   struct termios newtc;
   int ch;
   tcgetattr(STDIN_FILENO, &oldtc);
   newtc = oldtc;
   newtc.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
   ch = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
   return ch;
}


static void
sighandler (int signum) {
    fprintf(stdout, "\nType exit to exit\n");
    fflush(stdout);
    memset(cmd, '\0', CMD_LENGTH_MAX);
    current_length = 0;
}

static void
procmgr_shell_signal_handlers_init ()
{
    signal(SIGINT, sighandler);
    signal(SIGSTOP, SIG_IGN);
}



//TODO: Handling of esc keys, up, down, right, bottom and history keeping.

void*
procmgr_shell_thread_func (void *arg)
{
    (void) procmgr_shell_signal_handlers_init();
    printf("\n"); 
    bool is_authenticated = FALSE;
    while (TRUE) {
        while (is_authenticated == FALSE) {
            printf("\nPress any key to get started...");
            char ch = getch();
            printf("\nUsername: ");
            char username[24] = {'\0'};
            for (int i = 0; ;i++) {
                char chh = getch();
                if (chh == '\b' || chh == 8 || chh == 127) {
                    if (i == 0) {
                        continue;
                    }
                    i--;
                    username[i] = '\0';
                    fprintf(stdout, "\b \b");
                    fflush(stdout);
                    continue;
                }
                if (chh == '\n') {
                    fprintf(stdout, "\n");
                    fflush(stdout);
                    break;
                }
                fprintf(stdout, "%c", chh);
                fflush(stdout);
                if (i < 23) {
                    username[i] = chh;
                    username[i+1] = '\0';
                }
            }

            printf("Password: ");
            fflush(stdout);
            fflush(stdin);
            char password[24] = {'\0'};
            for (int i = 0; ;i++) {
                char chh = getch();
                if (chh == '\b' || chh == 8 || chh == 127) {
                    if (i == 0) {
                        continue;
                    }
                    i--;
                    password[i] = '\0';
                    fprintf(stdout, "\b \b");
                    fflush(stdout);
                    continue;
                }
                if (chh == '\n') {
                    fprintf(stdout, "\n");
                    fflush(stdout);
                    break;
                }
                fprintf(stdout, "%c", '*');
                fflush(stdout);
                if (i < 23) {
                    password[i] = chh;
                    password[i+1] = '\0';
                }
            }
            if (strlen(username) == 0 || strlen(password) == 0) {
                continue;
            }
            if (procmgr_authenticate_system(username, password) == PM_EOK) {
                is_authenticated = TRUE;
                break;
            } else {
                PROCMGR_LOG_ERROR("Authentication failed: Validate username or password");
                sleep(2); // Prevent Bruteforce
                continue;
            }
        }
        printf("ProcessManager# ");
        memset(cmd, '\0', sizeof(cmd));
        current_length = 0;
        while (TRUE) {
            char c;
            c = tolower(getch());

            if (c == 27) {
                printf("esc");
                continue;
            }
            if (c == '\b' || c == 8 || c == 127) {
                if (current_length == 0) {
                    continue;
                }
                current_length--;
                cmd[current_length] = '\0';
                fprintf(stdout, "\b \b");
                fflush(stdout);
                continue;
            }
            if (c == '?') {
                printf("?\nProcessManager# %s", cmd);
                continue;
            }
            if (c == '\n') {
                printf("\n");
                procmgr_utils_print_current_timestamp(stdout);
                break;
            }
            if (c == '\t') {
                printf("\nProcessManager# %s", cmd);
                continue;
            }
            putchar(c); fflush(stdout);
            if (current_length == CMD_LENGTH_MAX) {
                break;
            }
            cmd[current_length++] = c;
            cmd[current_length] = '\0';
        }

        printf("\n");

        if (strcmp(cmd, "exit") == 0) {
            printf("\nGoodbye!\n");
            is_authenticated=FALSE;
            continue;
        }
    }
    
    /* We must never come here */
    return (void*) 0xdead;
}


void
procmgr_shell_start (void)
{
    int re = pthread_create(&(procmgr_context()->shell_tid),
                            NULL,
                            &procmgr_shell_thread_func,
                            NULL);
    if (re != 0) {
        exit(errno);
    }
}
