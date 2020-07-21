#include <procmgr_types.h>
#include <procmgr_debug.h>
#include <procmgr_error.h>
#include <procmgr_context.h>

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
    newtc.c_cc[VTIME] = 0;
    newtc.c_cc[VMIN] = 1;
   tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
   ch = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
   return ch;
}


void
sighandler(int signum) {
    fprintf(stdout, "\nType exit to exit\n");
    fprintf(stdout, "\nProcessManager# "); 
    fflush(stdout);
    memset(cmd, '\0', CMD_LENGTH_MAX);
    current_length = 0;
    
}

void
signals_register ()
{
    signal(SIGINT, sighandler);
    signal(SIGSTOP, SIG_IGN);
}



//TODO: Handling of esc keys, up, down, right, bottom and history keeping.

void*
procmgr_shell_thread_func (void *arg)
{
    printf("\n"); 
    signals_register();
    while (TRUE) {
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
            if (c == '\n') {
                break;
            }
            if (c == '\t') {
                break;
            }
            putchar(c); fflush(stdout);
            if (current_length == CMD_LENGTH_MAX) {
                break;
            }
            cmd[current_length++] = c;
            cmd[current_length] = '\0';
        }

        if (strcmp(cmd, "exit") == 0) {
            printf("\nGoodbye!\n");
            return NULL;
        }
        printf("\n%s\n", cmd);


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
