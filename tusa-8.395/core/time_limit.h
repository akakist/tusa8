#ifndef TIME_LIMIT___H
#define TIME_LIMIT___H
#ifdef TIME_LIMIT
#ifndef COMPILE_TIME
#error undefined COMPILE_TIME 
#endif
#define CKTIME {if(time(NULL)>COMPILE_TIME+3600*24*30*5) throw "unknown error";}
#else
#define CKTIME
#endif
#endif
