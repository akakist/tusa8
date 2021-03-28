#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <map>
#include <list>
#include "errs.h"
#include "str_lib.h"
#ifndef WIN32
#include <pthread.h>
#else
#include <process.h>
#endif
#include "_mutex.h"
#include "_mutex1.h"
#include "mutex_inspector.h"
#include <stdio.h>
#include <ctime>
#include "M_MAP.h"

using namespace std;
void mutex_inspector_cpp() {}
#ifdef WIN32
#define snprintf _snprintf
#endif
struct mutex_inspector_entry
{
    struct 	iint
    {
        iint() {
        }
        string file;
        int line;
    };
    const char * f;
    int l;
    time_t t;
    const char * s;
    mutex_inspector_entry():f(NULL),l(0),t(0),s("") {}
};

#ifdef THREAD_NAME_DBUG
/*#ifdef pid_t
#define pid_t int
#endif*/
M_MAP <int,string> thread_name_by_pid;
M_MAP <pthread_t,string> thread_name_by_pthread;
M_MAP <pthread_t,string> thread_descr_by_pthread;
#endif

#ifdef MUTEX_INSPECTOR_DBUG
map < pthread_t, deque < mutex_inspector_entry > >mutex_inspector_list;
_mutex1  mutex_inspector_listMX_;
#endif
#ifdef LOCKCTL_DBUG
map < pthread_t,deque<mutex_inspector_entry::iint> > mutex_lockctl;
_mutex1 mutex_lockctl_MX;
#endif
#ifndef WIN32
M_MAP <pthread_t, pid_t> threads_pids;
#endif
#ifdef LOCKCTL_DBUG
lockctl::lockctl(const char *f,int l)
{
    MUTEX_LOCK1 kall(mutex_lockctl_MX);
    if(mutex_lockctl[pthread_self()].size()) {
        logErr2("mutex_lockctl.size() BEGIN at %s %d",f,l);
        for(deque<mutex_inspector_entry::iint>::const_iterator i=mutex_lockctl[pthread_self()].begin(); i!=mutex_lockctl[pthread_self()].end(); i++)
        {
            logErr2("%s %d",i->file.c_str(),i->line);
        }
        logErr2("mutex_lockctl.size() END");
        logErr2("mutex_lockctl.DMI %s",_DMI().c_str());
    }
    mutex_inspector_entry::iint ii;
    ii.file=f;
    ii.line=l;
    mutex_lockctl[pthread_self()].push_front(ii);
}
lockctl::~lockctl()
{
    MUTEX_LOCK1 kall(mutex_lockctl_MX);
    if(mutex_lockctl[pthread_self()].size())
        mutex_lockctl[pthread_self()].erase(mutex_lockctl[pthread_self()].begin());
}
#endif

#ifdef MUTEX_INSPECTOR_DBUG
mutex_inspector::mutex_inspector(const char *ff, int ll)
{
    mutex_inspector_entry e;
    e.f = ff;
    e.l = ll;
    e.t = time(NULL);
    {
        MUTEX_LOCK1 kaka(mutex_inspector_listMX_);
        mutex_inspector_list[pthread_self()].push_front(e);
    }
}
#endif
#ifdef MUTEX_INSPECTOR_DBUG
mutex_inspector::mutex_inspector(const char *ff, int ll, const char *ss)
{
    mutex_inspector_entry e;
    e.f = ff;
    e.l = ll;
    e.t = time(NULL);
    e.s = ss;
    {
        MUTEX_LOCK1 kaka(mutex_inspector_listMX_);
        mutex_inspector_list[pthread_self()].push_front(e);
    }
}
#endif
#ifdef MUTEX_INSPECTOR_DBUG
mutex_inspector::~mutex_inspector()
{
    pthread_t pt = pthread_self();
    {
        MUTEX_LOCK1 kaka(mutex_inspector_listMX_);
        if (mutex_inspector_list[pt].size()) {
            mutex_inspector_list[pt].erase(mutex_inspector_list[pt].begin());
        }
        if (mutex_inspector_list[pt].size() == 0) {
            mutex_inspector_list.erase(pt);
        }
    }
}
#endif
#ifndef WIN32
void threads_pids_set(pthread_t pth, pid_t pid)
{
#ifndef WIN32
    threads_pids.set(pth, pid);
#endif
}
#endif
#ifdef MUTEX_INSPECTOR_DBUG
string dump_mutex_inspectors()
{
    MUTEX_LOCK1 kall(mutex_inspector_listMX_);
    string out;
    for (map < pthread_t, deque < mutex_inspector_entry > >::iterator i = mutex_inspector_list.begin(); i != mutex_inspector_list.end(); i++) {

        if(i->second.size())
        {
            if(i->second.begin()->s)
            {
                string s=strupper((string)i->second.begin()->s);
                if(s=="SLEEP") {
                    out+= "SLEEP "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                    continue;
                }
                if(s=="USLEEP") {
                    out+= "USLEEP "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                    continue;
                }
                if(s=="SELECT") {
                    out+= "SELECT "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                    continue;
                }
                if(s=="ACCEPT") {
                    out+= "ACCEPT "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                    continue;
                }
                if(s=="mysql_real_query") {
                    out+= "mysql_real_query "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                    continue;
                }
            }
        }
        time_t tt = time(NULL);
        char s[200];
        snprintf(s,sizeof(s)-1,"---THREAD REPORT %d (0x%X) %s \n", i->first, i->first,thread_name_by_pthread.get(i->first).c_str());
        out += s;
        time_t mint=0;
        for ( deque < mutex_inspector_entry > ::iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            if(mint==0) mint=j->t;
            if(j->t<mint) mint=j->t;
        }
        for ( deque < mutex_inspector_entry > ::iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            snprintf(s,sizeof(s)-1,"\t%s: %d, %s, dt:%02d\n", j->f?j->f:"", j->l, j->s?j->s:"", j->t - mint);
            out+=s;
        }
    }
    return out;
    return	"";
}
string dump_mutex_inspectors_www()
{
    MUTEX_LOCK1 kall(mutex_inspector_listMX_);
    string out;
    for (map < pthread_t, deque < mutex_inspector_entry > >::iterator i = mutex_inspector_list.begin(); i != mutex_inspector_list.end(); i++) {


        time_t tt = time(NULL);
        char s[200];
        snprintf(s,sizeof(s)-1,"<p>---THREAD REPORT %d (0x%X)<br>\n", i->first, i->first);
        out += s;

        time_t mint=0;
        for ( deque < mutex_inspector_entry > ::iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            if(mint==0) mint=j->t;
            if(j->t<mint) mint=j->t;
        }

        for ( deque < mutex_inspector_entry > ::iterator j = i->second.begin(); j != i->second.end(); j++) {
            snprintf(s,sizeof(s)-1,"\t%s: %d, %s, dt:%02d (%02d)<br>\n", j->f?j->f:"", j->l, j->s?string((string)"<b>"+j->s+"</b>").c_str():"",  j->t - mint,tt - j->t);
            out+=s;
        }
    }
    return out;
    return	"";
}
#endif
#ifdef MUTEX_INSPECTOR_DBUG
string dump_mutex_inspector(pthread_t pt)
{
    MUTEX_LOCK1 kall(mutex_inspector_listMX_);
    string out;
    map < pthread_t, deque < mutex_inspector_entry > >::iterator i=mutex_inspector_list.find(pt);

    if(i!=mutex_inspector_list.end())
    {
        if(i->second.size())
        {
            if(i->second.begin()->s)
            {
                string s=strupper((string)i->second.begin()->s);
                if(s=="SLEEP") return "SLEEP "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                if(s=="USLEEP") return "USLEEP "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                if(s=="SELECT") return "SELECT "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                if(s=="ACCEPT") return "ACCEPT "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                if(s=="POLL") return "POLL "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
                if(s=="MYSQL_REAL_QUERY") return "MYSQL_REAL_QUERY "+itostring((int)i->first)+" "+thread_name_by_pthread.get(i->first)+"\n";
            }
        }
        time_t tt = time(NULL);
        char s[200];
        snprintf(s,sizeof(s)-1,"---THREAD REPORT %d (0x%X) %s\n", i->first, i->first,thread_name_by_pthread.get(i->first).c_str());
        out += s;
        for (deque < mutex_inspector_entry >::iterator j = i->second.begin(); j != i->second.end(); j++) {
            snprintf(s,sizeof(s)-1,"\t%s: %d, %s, dt:%02d (%02d)\n", j->f?j->f:"", j->l, j->s?j->s:"", tt - j->t,tt - j->t);
            out+=s;
        }
    }
    return out;
    return	"";
}
#endif


void log_start_thread(const string &_thread_name)
{
    string thread_name;
#ifndef HIDE_THREAD_NAME
    thread_name=_thread_name;
#endif
#ifdef THREAD_NAME_DBUG
#ifndef WIN32
    threads_pids_set(pthread_self(), getpid());
    thread_name_by_pid.set(getpid(),thread_name);
#endif
    thread_name_by_pthread.set(pthread_self(),thread_name);

#ifdef MUTEX_INSPECTOR_DBUG

#ifdef WIN32
    logErr2("started thread '%s' pthread=%d (0x%X)",thread_name.c_str(), pthread_self(),pthread_self());
#else
    logErr2("started thread '%s' pid=%d (0x%X) pthread=%d (0x%X)",thread_name.c_str(), getpid(),getpid(),pthread_self(),pthread_self());
#endif


#else
#endif
#endif
}

void log_stop_thread(const string &_thread_name)
{
    string thread_name;
#ifndef HIDE_THREAD_NAME
    thread_name=_thread_name;
#endif
#ifdef THREAD_NAME_DBUG
#ifndef WIN32
    threads_pids_set(pthread_self(), getpid());
    thread_name_by_pid.set(getpid(),thread_name);
#endif
    thread_name_by_pthread.set(pthread_self(),thread_name);

#ifdef MUTEX_INSPECTOR_DBUG

#ifdef WIN32
    logErr2("stopped thread '%s' pthread=%d (0x%X)",thread_name.c_str(), pthread_self(),pthread_self());
#else
    logErr2("stopped thread '%s' pid=%d (0x%X) pthread=%d (0x%X)",thread_name.c_str(), getpid(),getpid(),pthread_self(),pthread_self());
#endif


#else
#endif
#endif
}

#ifdef THREAD_NAME_DBUG
void set_thread_description(const string &descr)
{
    thread_descr_by_pthread.set(pthread_self(),descr);
}
string get_thread_description()
{
    return thread_descr_by_pthread.get(pthread_self());
}
#endif


void local_free_handler__Mutex()
{
#ifdef  THREAD_NAME_DBUG
    thread_name_by_pid.clear();
    thread_name_by_pthread.clear();
    thread_descr_by_pthread.clear();
#endif
#ifndef WIN32
    threads_pids.clear();
#endif
#ifdef MUTEX_INSPECTOR_DBUG
    mutex_inspector_list.clear();
#endif
#ifdef LOCKCTL_DBUG
    mutex_lockctl.clear();
#endif

}


#ifdef LEAKCTLDEBUG
struct leakctl
{
    void *p;
    const char *name;
    const char *file;
    int line;
    leakctl(void *_p,const char *_name, const char *_file,int _line):p(_p),name(_name),file(_file),line(_line) {}
};
map<void *,leakctl> leakctl_container;
_mutex leakctl_containerMX("");
static int tot_leaks=0;
void add_leakctlitem(void *_p,const char *_name, const char *_file,int _line)
{
    char err[300];
    M_LOCK(leakctl_containerMX);

    if(leakctl_container.count(_p)) {
        snprintf(err,sizeof(err),"alloc already exists 0x%x %s %d (%s)",_p,_file,_line,_name);
        logErr2("%s",err);
    }
    leakctl l(_p,_name, _file,_line);
    leakctl_container.insert(make_pair(_p,l));
    tot_leaks++;
}
void add_leakctlitem(void *_p,const char *_file,int _line)
{
    char err[300];
    M_LOCK(leakctl_containerMX);
    if(leakctl_container.count(_p)) {
        snprintf(err,sizeof(err),"alloc already exists 0x%x %s %d",_p,_file,_line);
        logErr2("%s",err);
    }
    leakctl l(_p,NULL, _file,_line);
    leakctl_container.insert(make_pair(_p,l));
    tot_leaks++;
}
void remove_leakctlitem(void *p, const char* _f,int _l)
{
    M_LOCK(leakctl_containerMX);
    if(!leakctl_container.count(p))
    {
        char err[300];
        snprintf(err,sizeof(err),"alloc not exists 0x%x %s %d",p,_f,_l);
        logErr2("%s",err);
    }
    leakctl_container.erase(p);
    tot_leaks--;
}
void remove_leakctlitem(void *p, const char *_n,const char* _f,int _l)
{
    M_LOCK(leakctl_containerMX);
    if(!leakctl_container.count(p))
    {
        char err[300];
        snprintf(err,sizeof(err),"alloc not exists 0x%x %s %d (%s)",p,_f,_l,_n);
        logErr2("%s",err);
    }
    leakctl_container.erase(p);
    tot_leaks--;
}
void remove_leakctlitem(void *p, const char* s)
{
    M_LOCK(leakctl_containerMX);

    if(!leakctl_container.count(p))
    {
        char err[300];
        snprintf(err,sizeof(err),"alloc not exists 0x%x (%s)",p,s);
        logErr2("%s",err);

    }
    leakctl_container.erase(p);
    tot_leaks--;
}
string report_leaks()
{
    string out;
    map<string,int> m;
    {
        M_LOCK(leakctl_containerMX);
        for(map<void *,leakctl> ::iterator i=leakctl_container.begin(); i!=leakctl_container.end(); i++)
        {
            char s[400];
            snprintf(s,sizeof(s),"%s %d (%s)",i->second.file,i->second.line,i->second.name?i->second.name:"");
            map<string,int>::iterator K=m.find(s);
            if(K==m.end()) m[s]=1;
            else K->second++;
        }
    }
    for(map<string,int>::iterator i=m.begin(); i!=m.end(); i++)
    {
        out+="<br>"+i->first+" "+itostring(i->second);
    }
    return out;
}

#endif
