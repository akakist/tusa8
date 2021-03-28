#ifndef threadable__h
#define threadable__h
#include "M_MAP.h"
#include <string>
using namespace std;
#define STOP_THREAD 0
class threadable
{
public:
    virtual const char* getclassname()const =0;

    virtual ~threadable();
    threadable();
    bool SLEEP(int sec);
    bool USLEEP(int usec);
    void create_thread(void * (*start_routine)(void *), const std::string & __name);

    void kill_threads();

private:



};
void g_create_thread(void * (*start_routine)(void *), void *P,const std::string & __name);
void g_kill_threads();
extern bool g_must_terminate;


#endif
