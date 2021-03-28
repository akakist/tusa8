#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <list>
#include "_mutex.h"
#include "mutex_inspector.h"
#include <stdio.h>
#include "errs.h"
#include <signal.h>
#include "st_stuff.h"
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <algorithm>
#include "file_set.h"
#include "tcp_stats.h"
#include "M_MAP.h"

#include "lockctl.h"
#ifndef WIN32
#include <arpa/inet.h>
#endif

#ifdef THREAD_NAME_DBUG
extern M_MAP <pid_t,string> thread_name_by_pid;
extern M_MAP <pthread_t,string> thread_name_by_pthread;
extern M_MAP <pthread_t,string> thread_descr_by_pthread;
#endif
#ifdef __linux__
extern M_MAP <pthread_t, pid_t> threads_pids;

static time_t thread_wtime(pid_t pid)
{
    int total_work_time = 0;
    string fn = "/proc/" + std::to_string(pid) + "/stat";
    FILE *f = fopen(fn.c_str(), "rb");
    if (f) {

        string ss;
        while(!feof(f))
        {
            ss+=char(fgetc(f));
        }
        fclose(f);
        vector <string> v = splitTOK(" ", ss);
        if (v.size() > 14) {

            total_work_time += atoi(v[13].c_str());
        }
        else
            logErr("v.size()=%d", v.size());
    }
    else
        logErr("f==NULL %s", fn.c_str());
    return total_work_time;
}
#endif

string thread_info_txt()
{
#ifndef WIN32

#ifdef THREAD_NAME_DBUG
    map <pthread_t, pid_t> _threads_pids=threads_pids.get_container();
    map <pid_t,pthread_t> p2p;
    map <pid_t,string> _thread_name_by_pid=thread_name_by_pid.get_container();
    map <pthread_t,string> _thread_name_by_pthread=thread_name_by_pthread.get_container();
    map <pthread_t,string> _thread_descr_by_pthread=thread_descr_by_pthread.get_container();
    string out;
    out+="Thread info\n";
    out+="\n";
    out+="pid\t\tthread\t\tname\t\tCPU\n";
    for(auto i=_threads_pids.begin(); i!=_threads_pids.end(); i++)
    {
        p2p[i->second]=i->first;
    }
    for(auto i=p2p.begin(); i!=p2p.end(); i++)
    {
        char s[500];
        double twt=thread_wtime(i->first);
        twt/=100;
        snprintf(s,sizeof(s)-1,"%d (0x%X)\t\t%d (0x%X)\t\t%s\t\t%2.2f\n",
                 i->first,i->first,i->second,i->second,
                 _thread_name_by_pid[i->first].c_str(),
                 twt
                );
        out+=s;
    }
    return out;
#else
    return "";
#endif
#else
    return "";
#endif
}
