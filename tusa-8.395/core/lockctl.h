#ifndef LOCK_CTL_H
#define LOCK_CTL_H

#ifdef LOCKCTL_DBUG
#define LOCKCTL lockctl kallowo(__FILE__,__LINE__);
#else
#define LOCKCTL
#endif

#endif
