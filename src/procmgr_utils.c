#include <procmgr_utils.h>

#include <time.h>
#include <string.h>
#include <sys/time.h>

void
procmgr_utils_print_current_timestamp (FILE *file)
{
    char timestamp[64] = { '\0' };
    struct timespec ts = { 0 };

    clock_gettime(CLOCK_REALTIME, &ts);
    time_t epoch = ts.tv_sec;
    
    strftime(timestamp, sizeof(timestamp), 
            "%d-%b-%Y %H:%M:%S", localtime(&epoch));
    fprintf(file, "%s.%03ld", timestamp, ts.tv_nsec / 1000000);
    strftime(timestamp, sizeof(timestamp), 
            " %Z [UTC %z]", localtime(&epoch));
    int len = strlen(timestamp);
    timestamp[len + 1] = '\0';
    for (int i = 0; i < 3; i++) {
        timestamp[len - i] = timestamp[len - i - 1];
    }
    timestamp[len - 3] = ':';
    fprintf(file, "%s", timestamp);
}
