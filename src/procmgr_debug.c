#include <procmgr_debug.h>
#include <procmgr_error.h>
#include <procmgr_context.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <time.h>
#include <sys/time.h>

#define SYSLOG(_level, _fmt, ...)                                   \
    procmgr_context()->debug.flags.log_to_syslog ?                  \
        syslog(_level, "%s()[%d]:"_fmt, __FUNCTION__, __LINE__,     \
           ##__VA_ARGS__) : (void) NULL 


static char *debug_level_to_string[] = {
    [PROCMGR_DEBUG_LEVEL_DISABLED]      = "DISABLED",
    [PROCMGR_DEBUG_LEVEL_DEBUG]         = "DEBUG",
    [PROCMGR_DEBUG_LEVEL_INFO]          = "INFO ",
    [PROCMGR_DEBUG_LEVEL_WARNING]       = "WARN ",
    [PROCMGR_DEBUG_LEVEL_ERROR]         = "ERROR",
    [PROCMGR_DEBUG_LEVEL_FATAL]         = "FATAL",
    [PROCMGR_DEBUG_LEVEL_CRITICAL]      = "CRIT ",
};

void
procmgr_debug_log_to_file (bool selection)
{
    procmgr_context()->debug.flags.log_to_file = selection;
}

void
procmgr_debug_log_to_syslog (bool selection)
{
    procmgr_context()->debug.flags.log_to_syslog = selection;
}

void
procmgr_debug_log_to_console (bool selection)
{
    procmgr_context()->debug.flags.log_to_console = selection;
}

static void
procmgr_debug_enable_all_logging ()
{
    (void) procmgr_debug_log_to_file(TRUE);
    (void) procmgr_debug_log_to_syslog(TRUE);
    (void) procmgr_debug_log_to_console(TRUE);
}


pmerr
procmgr_debug_init (char *logdir)
{
   
    procmgr_context()->debug.logdir = strdup(logdir);
    
    (void) procmgr_debug_enable_all_logging();

    if (procmgr_context()->debug.logdir == NULL) {
        SYSLOG(LOG_CRIT,
               "Unable to allocate memory for logdir string");
        return errno;
    }
    
    if (access(procmgr_context()->debug.logdir, R_OK | W_OK) != 0) {
        SYSLOG(LOG_CRIT,
               "Write access denied to log directory: %s",
                procmgr_context()->debug.logdir);
        return errno;
    }
     
    SYSLOG(LOG_INFO,
           "Completed successfully");
    return PM_EOK; 
}


void
procmgr_debug_log (procmgr_debug_level_t    level,
                   const char              *function,
                   int                      line,
                   char                    *format,
                   ...)
{
    if (level != PROCMGR_DEBUG_LEVEL_ERROR) {
        //return;
    }
    char timestamp[64] = { '\0' };
    struct timespec ts = { 0 };

    clock_gettime(CLOCK_REALTIME, &ts);
    time_t epoch = ts.tv_sec;
    
    strftime(timestamp, sizeof(timestamp), 
            "%d-%b-%Y %H:%M:%S", localtime(&epoch));
    va_list list;
    va_start(list, format);
    fprintf(stdout, "\n%s.%06ld %% %s %% %s()+%d: ",
            timestamp,
            ts.tv_nsec / 1000,
            debug_level_to_string[level],
            function, line);
    vfprintf(stdout, format, list);
    va_end(list);
    fprintf(stdout, "\n");
    fflush(stdout);
}

