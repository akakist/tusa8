/*
core
(c) belialov.com
*/
#ifndef LOCK_CTL_H
#define LOCK_CTL_H

#ifdef LOCKCTL_DBUG
#define LOCKCTL lockctl kallowo(__FL__);	
#else
#define LOCKCTL 
#endif

#endif
