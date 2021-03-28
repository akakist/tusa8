#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
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
#include "mysql_extens.h"
#include <algorithm>
#include "file_set.h"
#include "tcp_stats.h"
#include "server_config.h"
#include "lockctl.h"
#include "c_conn.h"
#include "tcp_dialog_server.h"

#ifdef __linux__
extern M_MAP <pthread_t, pid_t> threads_pids;
time_t total_wtime()
{
    int total_work_time = 0;
    vector < pid_t > pids=threads_pids.values();
    set<pid_t> sp;
    for(unsigned i=0; i< pids.size(); i++)
    {
        sp.insert(pids[i]);
    }
{   for (auto i=sp.begin(); i!=sp.end(); i++) {


            string fn = "/proc/" + itostring(*i) + "/stat";
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
        }
    }
    return total_work_time;
}
#endif
