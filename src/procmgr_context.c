#include <procmgr_context.h>

#include <unistd.h>

static procmgr_context_st context;

procmgr_context_st*
procmgr_context (void)
{
    return &context;
}


void
procmgr_context_init (void)
{
    procmgr_context()->mypid = getpid();
}
