#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <map>
#include <list>
#include "Errs.h"
#include "StringUtils.h"
#ifndef WIN32
#include <pthread.h>
#else
#include <process.h>
#endif
#include "Mutex.h"
#include "xd.h"
#include <stdio.h>
#include <ctime>

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
map < pthread_t, deque < mutex_inspector_entry > >mutex_inspector_list;
_mutex  mutex_inspector_listMX_;

mutex_inspector::mutex_inspector(const char *ff, int ll)
{
    mutex_inspector_entry e;
    e.f = ff;
    e.l = ll;
    e.t = time(NULL);
    {
        MUTEX_LOCK kaka(mutex_inspector_listMX_);
        mutex_inspector_list[pthread_self()].push_front(e);
    }
}
mutex_inspector::mutex_inspector(const char *ff, int ll, const char *ss)
{
    mutex_inspector_entry e;
    e.f = ff;
    e.l = ll;
    e.t = time(NULL);
    e.s = ss;
    {
        MUTEX_LOCK kaka(mutex_inspector_listMX_);
        mutex_inspector_list[pthread_self()].push_front(e);
    }
}
mutex_inspector::~mutex_inspector()
{
    pthread_t pt = pthread_self();
    {
        MUTEX_LOCK kaka(mutex_inspector_listMX_);
        if (mutex_inspector_list[pt].size()) {
            mutex_inspector_list[pt].erase(mutex_inspector_list[pt].begin());
        }
        if (mutex_inspector_list[pt].size() == 0) {
            mutex_inspector_list.erase(pt);
        }
    }
}
string dump_mutex_inspectors()
{
    MUTEX_LOCK kall(mutex_inspector_listMX_);
    string out;
    for (map < pthread_t, deque < mutex_inspector_entry > >::iterator i = mutex_inspector_list.begin(); i != mutex_inspector_list.end(); i++) {

        if(i->second.size())
        {
            if(i->second.begin()->s)
            {
                string s=(string)i->second.begin()->s;
                s=StringUtils::Upper(s);
                if(s=="SLEEP") {
                    out+= "SLEEP "+StringUtils::itostring(i->first)+"\n";
                    continue;
                }
                if(s=="USLEEP") {
                    out+= "USLEEP "+StringUtils::itostring(i->first)+"\n";
                    continue;
                }
                if(s=="SELECT") {
                    out+= "SELECT "+StringUtils::itostring(i->first)+"\n";
                    continue;
                }
                if(s=="ACCEPT") {
                    out+= "ACCEPT "+StringUtils::itostring(i->first)+"\n";
                    continue;
                }
            }
        }
        time_t tt = time(NULL);
        char s[200];
        snprintf(s,sizeof(s)-1,"---THREAD REPORT %d (0x%X)\n", i->first, i->first);
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
    MUTEX_LOCK kall(mutex_inspector_listMX_);
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
string dump_mutex_inspector(pthread_t pt)
{
    MUTEX_LOCK kall(mutex_inspector_listMX_);
    string out;
    map < pthread_t, deque < mutex_inspector_entry > >::iterator i=mutex_inspector_list.find(pt);

    if(i!=mutex_inspector_list.end())
    {
        if(i->second.size())
        {
            if(i->second.begin()->s)
            {
                string s=(string)i->second.begin()->s;
                s=StringUtils::Upper(s);
                if(s=="SLEEP") return "SLEEP "+StringUtils::itostring(i->first)+"\n";
                if(s=="USLEEP") return "USLEEP "+StringUtils::itostring(i->first)+"\n";
                if(s=="SELECT") return "SELECT "+StringUtils::itostring(i->first)+"\n";
                if(s=="ACCEPT") return "ACCEPT "+StringUtils::itostring(i->first)+"\n";
            }
        }
        time_t tt = time(NULL);
        char s[200];
        snprintf(s,sizeof(s)-1,"---THREAD REPORT %d (0x%X)\n", i->first, i->first);
        out += s;
        for (deque < mutex_inspector_entry >::iterator j = i->second.begin(); j != i->second.end(); j++) {
            snprintf(s,sizeof(s)-1,"\t%s: %d, %s, dt:%02d (%02d)\n", j->f?j->f:"", j->l, j->s?j->s:"", tt - j->t,tt - j->t);
            out+=s;
        }
    }
    return out;
    return	"";
}
