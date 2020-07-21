#include <procmgr_debug.h>
#include <procmgr_error.h>
#include <procmgr_context.h>

#include <unistd.h>
#include <string.h>
#include <syslog.h>


#define SYSLOG(_level, _fmt, ...)                                   \
    procmgr_context()->debug.flags.log_to_syslog ?                  \
        syslog(_level, "%s()[%d]:"_fmt, __FUNCTION__, __LINE__,     \
           ##__VA_ARGS__) : (void) NULL 



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

