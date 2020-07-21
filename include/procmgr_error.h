#ifndef __PROCMGR_ERROR_H__
#define __PROCMGR_ERROR_H__

#include <procmgr_types.h>

#include <errno.h>


#define PM_EOK 0

typedef uint64_t pmerr;

#define PMERR_IS_OK(_arg) \
    ((_arg) == PM_EOK)

#define PMERR_IS_NOTOK(_arg) \
    ((_arg) != PM_EOK)


#endif // __PROCMGR_ERROR_H__
