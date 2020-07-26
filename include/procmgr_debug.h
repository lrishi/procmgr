#ifndef __PROCMGR_DEBUG_H__
#define __PROCMGR_DEBUG_H__

#include <procmgr_error.h>
#include <stdio.h>

typedef struct procmgr_debug_context_st_ {
    char           *logdir;
    volatile struct {
        bool        lock;
        uint32_t    index;
        FILE       *file;
    } file;
    struct {
        bool        log_to_file:1;
        bool        log_to_syslog:1;
        bool        log_to_console:1;
    } flags;
} procmgr_debug_context_st;

typedef enum procmgr_debug_level_t_ {
    PROCMGR_DEBUG_LEVEL_DISABLED = 0,
    PROCMGR_DEBUG_LEVEL_DEBUG,
    PROCMGR_DEBUG_LEVEL_INFO,
    PROCMGR_DEBUG_LEVEL_WARNING,
    PROCMGR_DEBUG_LEVEL_ERROR,
    PROCMGR_DEBUG_LEVEL_FATAL,
    PROCMGR_DEBUG_LEVEL_CRITICAL
} procmgr_debug_level_t;


pmerr
procmgr_debug_init(char *logdir);

void
procmgr_debug_log_to_syslog(bool selection);

void
procmgr_debug_log_to_console(bool selection);

void
procmgr_debug_log_to_file(bool selection);


void
procmgr_debug_log(procmgr_debug_level_t    level,
                  const char              *function,
                  int                      line,
                  char                    *format,
                  ...);


#define PROCMGR_LOG_DEBUG(__fmt, ...)                   \
    (void) procmgr_debug_log(PROCMGR_DEBUG_LEVEL_DEBUG, \
                             __FUNCTION__,              \
                             __LINE__,                  \
                             __fmt,                     \
                             ##__VA_ARGS__)


#define PROCMGR_LOG_INFO(__fmt, ...)                    \
    (void) procmgr_debug_log(PROCMGR_DEBUG_LEVEL_INFO,  \
                             __FUNCTION__,              \
                             __LINE__,                  \
                             __fmt,                     \
                             ##__VA_ARGS__)

#define PROCMGR_LOG_WARNING(__fmt, ...)                     \
    (void) procmgr_debug_log(PROCMGR_DEBUG_LEVEL_WARNING,   \
                             __FUNCTION__,                  \
                             __LINE__,                      \
                             __fmt,                         \
                             ##__VA_ARGS__)

#define PROCMGR_LOG_ERROR(__fmt, ...)                   \
    (void) procmgr_debug_log(PROCMGR_DEBUG_LEVEL_ERROR, \
                             __FUNCTION__,              \
                             __LINE__,                  \
                             __fmt,                     \
                             ##__VA_ARGS__)

#endif // __PROCMGR_DEBUG_H__
