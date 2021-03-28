#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <vector>
#include <map>
#include <map>
#include "tcp_server_connection.h"
#include <stdio.h>
#include "_mutex.h"
#include "errs.h"
#include "str_lib.h"
#include "st_stuff.h"
#include <errno.h>

#include "file_set.h"
#include "mutex_inspector.h"
#include <algorithm>
#include <set>
#include <deque>

#include "M_DEQUE.h"

#include "tcp_server_data.h"

#include "tcp_stats.h"
#ifdef WIN32
#define snprintf _snprintf
#endif
#ifdef THREAD_NAME_DBUG
extern M_MAP <pid_t,string> thread_name_by_pid;
extern M_MAP <pthread_t,string> thread_name_by_pthread;
extern M_MAP <pthread_t,string> thread_descr_by_pthread;
#endif

#ifdef __linux__
time_t thread_wtime(pid_t pid);
extern M_MAP <pthread_t, pid_t> threads_pids;

time_t thread_wtime(pid_t pid)
{
    int total_work_time = 0;
    string fn = "/proc/" + itostring(pid) + "/stat";
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
    {
    }
    return total_work_time;
}
time_t total_wtime()
{
    int total_work_time = 0;
    vector < pid_t > pids=threads_pids.values();
{   for (unsigned int i = 0; i < pids.size(); i++) {


            string fn = "/proc/" + itostring(pids[i]) + "/stat";
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
            {
            }
        }}
    return total_work_time;
}
#else
time_t total_wtime()
{
    return 0;
}
#ifdef __linux__
time_t thread_wtime(pid_t pid)
{
    return 0;
}
#endif
#endif

string thread_info_html(const map<pthread_t, string> &add_map)
{
#ifndef WIN32

#ifdef THREAD_NAME_DBUG
    map <pthread_t, pid_t> _threads_pids=threads_pids.get_container();
    map <pid_t,pthread_t> p2p;
    map <pid_t,string> _thread_name_by_pid=thread_name_by_pid.get_container();
    map <pthread_t,string> _thread_name_by_pthread=thread_name_by_pthread.get_container();
    map <pthread_t,string> _thread_descr_by_pthread=thread_descr_by_pthread.get_container();
    string out;
    out+="<p><b>Thread info</b>\n";
    out+="<table>\n";
    out+="<tr><td>pid</td><td>pthread</td><td>name</td><td>CPU</td></tr>\n";
    for(typeof(_threads_pids.begin()) i=_threads_pids.begin(); i!=_threads_pids.end(); i++)
    {
        p2p[i->second]=i->first;
    }
    for(typeof(p2p.begin()) i=p2p.begin(); i!=p2p.end(); i++)
    {
        char s[500];
        double twt=thread_wtime(i->first);
        twt/=100;
        typeof(add_map.begin()) ii=add_map.find(i->second);
        string ainf;
        if(ii!=add_map.end()) ainf=ii->second;
        snprintf(s,sizeof(s)-1,"<tr><td>%d (0x%X)</td><td>%d (0x%X)</td><td>%s</td><td>%2.2f</td><td>%s</td></tr>\n",
                 i->first,i->first,i->second,i->second,
                 _thread_name_by_pid[i->first].c_str(),
                 twt,ainf.c_str()
                );
        out+=s;
    }

    out+="</table>\n";
    return out;
#else
    return "";
#endif
#else
    return "";
#endif
}
#ifndef WIN32
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
    for(typeof(_threads_pids.begin()) i=_threads_pids.begin(); i!=_threads_pids.end(); i++)
    {
        p2p[i->second]=i->first;
    }
    for(typeof(p2p.begin()) i=p2p.begin(); i!=p2p.end(); i++)
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

    out+="</table>\n";
    return out;
#else
    return "";
#endif
#else
    return "";
#endif
}
#endif
