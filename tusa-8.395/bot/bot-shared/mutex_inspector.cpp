/*
core
(c) belialov.com
*/
//#include "stdafx.h"
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

//#ifdef DEBUG
//#include "_mutex1.h"
//#include "mutex_inspector.h"
//#endif

#include <stdio.h>
#include <ctime>
//#include "M_MAP.h"
using namespace std;
void mutex_inspector_cpp(){}
#ifdef WIN32
#define snprintf _snprintf
#endif
struct iint{
iint(){//n=0;
}
string file;
int line;
};
struct mutex_inspector_entry
{
	const char * f;
	int l;
	time_t t;
	const char * s;
	mutex_inspector_entry():f(NULL),l(0),t(0),s(0){}
};
#ifdef DEBUG
map < pthread_t,deque<iint> > mutex_lockctl;_mutex1 mutex_lockctl_MX;
map < pthread_t, deque < mutex_inspector_entry > >mutex_inspector_list;_mutex1  mutex_inspector_listMX_;
#endif
M_MAP <pthread_t, pid_t> threads_pids;
#ifdef DEBUG
lockctl::lockctl(const char *f,int l)
{
	MUTEX_LOCK1 kall(mutex_lockctl_MX);
	if(mutex_lockctl[pthread_self()].size()){
		logErr("mutex_lockctl.size() BEGIN at %s %d",f,l);
		for(typeof(mutex_lockctl[pthread_self()].begin()) i=mutex_lockctl[pthread_self()].begin();i!=mutex_lockctl[pthread_self()].end();i++)
		{
			logErr("%s %d",i->file.c_str(),i->line);
		}
		logErr("mutex_lockctl.size() END");
	}
	iint ii;
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

#ifdef DEBUG
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
#ifdef DEBUG
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
#ifdef DEBUG
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
void threads_pids_set(pthread_t pth, pid_t pid)
{
#ifndef WIN32
	threads_pids.set(pth, pid);
#endif	
}
#ifdef DEBUG
string dump_mutex_inspectors()
{
	MUTEX_LOCK1 kall(mutex_inspector_listMX_);
	string out;
	for (typeof(mutex_inspector_list.begin()) i = mutex_inspector_list.begin(); i != mutex_inspector_list.end(); i++) {
		time_t tt = time(NULL);
		char s[200];
		snprintf(s,sizeof(s)-1,"---THREAD REPORT %d (0x%X)\n", i->first, i->first);
		out += s;
		for (typeof(i->second.begin()) j = i->second.begin(); j != i->second.end(); j++) {
			snprintf(s,sizeof(s)-1,"\t%s: %d, %s, dt:%02d\n", j->f?j->f:"", j->l, j->s?j->s:"", tt - j->t);
			out+=s;
		}
	}
	return out;
return	"";
}
#endif 
#ifdef DEBUG
string dump_mutex_inspector(pthread_t pt)
{
	MUTEX_LOCK1 kall(mutex_inspector_listMX_);
	string out;
	typeof(mutex_inspector_list.find(pt)) i=mutex_inspector_list.find(pt);
	if(i!=mutex_inspector_list.end())
	{
		time_t tt = time(NULL);
		char s[200];
		snprintf(s,sizeof(s)-1,"---THREAD REPORT %d (0x%X)\n", i->first, i->first);
		out += s;
		for (typeof(i->second.begin()) j = i->second.begin(); j != i->second.end(); j++) {
			snprintf(s,sizeof(s)-1,"\t%s: %d, %s, dt:%02d\n", j->f?j->f:"", j->l, j->s?j->s:"", tt - j->t);
			out+=s;
		}
	}
	return out;
return	"";
}
#endif 



M_MAP <pid_t,string> thread_name_by_pid;
M_MAP <pthread_t,string> thread_name_by_pthread;

void log_start_thread(const string &thread_name)
{
#ifndef WIN32
	threads_pids_set(pthread_self(), getpid());
	thread_name_by_pid.set(getpid(),thread_name);
#endif	
	thread_name_by_pthread.set(pthread_self(),thread_name);
#ifdef DEBUG	
#ifdef WIN32
	logErr("started thread '%s' pthread=%d (0x%X) ",thread_name.c_str(), pthread_self(),pthread_self());
#else
	logErr("started thread '%s' pid=%d (0x%X) pthread=%d (0x%X) ",thread_name.c_str(), getpid(),getpid(),pthread_self(),pthread_self());
#endif	
#endif
	
}
string thread_info_html()
{
#ifndef WIN32
	map <pid_t,string> pids=thread_name_by_pid.get_container();
	map <pthread_t,string> pths=thread_name_by_pthread.get_container();
	string out;
	out+="<B>By pid_t</B><br>\n<table>\n";
	for(typeof(pids.begin()) i=pids.begin();i!=pids.end();i++)
	{
		out+="<tr><td>"+itostring(i->first)+"</td><td>"+i->second+"</td></tr>\n";
	}
	out+="</table>";
	out+="<B>By pthread_t</B><br>\n<table>\n";
	{for(typeof(pths.begin()) i=pths.begin();i!=pths.end();i++)
	{
		out+="<tr><td>"+itostring((int)i->first)+"</td><td>"+i->second+"</td></tr>\n";
	}
	}
	out+="</table>";
	return out;
#else
return "";
#endif	
}


