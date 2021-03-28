#include "errs.h"
#include <cstdarg>
#include <ctime>
#include <stdio.h>
#include "config.h"
#include <deque>
#include "mutex_inspector.h"

void logErr (char *fmt, ...)
{
    va_list argptr;
    va_start (argptr, fmt);
    FILE *f = fopen (params["LOGNAME"].c_str (), "a");
    if (f)
    {
        time_t t = time (NULL);
        string ss = ctime (&t);
        if (ss.size ())
            fprintf (f, "%s: ", ss.substr (0, ss.size () - 1).c_str ());
        vfprintf (f, fmt, argptr);
        fprintf (f, "\n");
        fclose (f);
    }
}


cError::cError (const string & e)
{
  error_code = E_COMMON;
  error_string = e;
}

cError::cError (int e, const string & s)
{
  error_code = e;
  error_string = s;
}

struct mutex_inspector_entry
{
    const char *f;
    int l;
    time_t t;
    const char *s;
    mutex_inspector_entry ():f (NULL), l (0), t (0), s (0)
    {
    }
};
#ifdef DEBUG
map < pthread_t, deque < mutex_inspector_entry > >mutex_inspector_list;

mutex_inspector::mutex_inspector (const char *ff, int ll)
{
    mutex_inspector_entry e;
    e.f = ff;
    e.l = ll;
    e.t = time (NULL);
    {
        mutex_inspector_list[pthread_self ()].push_front (e);
    }
}
mutex_inspector::mutex_inspector (const char *ff, int ll, const char *ss)
{
    mutex_inspector_entry e;
    e.f = ff;
    e.l = ll;
    e.t = time (NULL);
    e.s = ss;
    {
        mutex_inspector_list[pthread_self ()].push_front (e);
    }
}

mutex_inspector::~mutex_inspector ()
{
    pthread_t pt = pthread_self ();
    {
        if (mutex_inspector_list[pt].size ())
        {
            mutex_inspector_list[pt].erase (mutex_inspector_list[pt].begin ());
        }
        if (mutex_inspector_list[pt].size () == 0)
        {
            mutex_inspector_list.erase (pt);
        }
    }
}
string dump_mutex_inspectors ()
{
    string out;
    for (typeof(mutex_inspector_list.begin()) i =
                mutex_inspector_list.begin (); i != mutex_inspector_list.end (); i++)
    {
        time_t tt = time (NULL);
        char s[200];
        snprintf (s, sizeof (s) - 1, "---THREAD REPORT %d (0x%X)\n", i->first,
                  i->first);
        out += s;
        for (typeof(i->second.begin()) j = i->second.begin();
                j != i->second.end(); j++)
        {
            snprintf (s, sizeof (s) - 1, "\t%s: %d, %s, dt:%02d\n",
                      j->f ? j->f : "", j->l, j->s ? j->s : "", tt - j->t);
            out += s;
        }
    }
    return out;
    return "";
}

#endif
