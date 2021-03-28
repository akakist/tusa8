#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "db_var.h"
#include "copy_file.h"
#include "file_set.h"
#include <sys/stat.h>
#include "wrappers.h"
#include <gdbm.h>
#include "M_MAP.h"
#include "server_config.h"
#include "mutex_inspector.h"
#include "db_maps.h"
string __dbmaps::full_path(const string & classname, const string & path)
{
    string p = "";
    if (path.size())
        p = "/" + path;
    string fullpath;
    if (path.size())
        fullpath = (string) classname + "/" + path + ".db";
    else
        fullpath = (string) classname + ".db";
    return fullpath;
}

void __dbmaps::purge(time_t update_timeout, time_t drop_timeout)
{
    map < db_map_base *, string /*classname */  > all;


    {
        M_LOCK(Mutex);
        for (map < string /*classname */ , map < string /*path */ , db_map_base *> >::iterator i = container.begin(); i != container.end(); i++) {
            for (map < string /*path */ , db_map_base *>::iterator j = i->second.begin(); j != i->second.end(); j++) {
                all[j->second] = i->first;
            }
        }
    }
    for (map < db_map_base *, string /*classname */  >::iterator i = all.begin(); i != all.end(); i++) {
        i->first->purge(update_timeout, drop_timeout);
    }
    {
        M_LOCK(Mutex);
        vector < string > to_erase_i;
        for (map < string /*classname */ , map < string /*path */ , db_map_base  *> >::iterator i = container.begin(); i != container.end(); i++) {
            vector < string > to_erase_j;
            for (map < string /*path */ , db_map_base  *>::iterator j = i->second.begin(); j != i->second.end(); j++) {
                if(j->second->size()==0)
                    to_erase_j.push_back(j->first);
            }
            for(unsigned k=0; k<to_erase_j.size(); k++)
            {
                LEAKCTL_REMOVE(i->second[to_erase_j[k]]);
                delete i->second[to_erase_j[k]];
                i->second.erase(to_erase_j[k]);
            }
            if(i->second.size()==0)
                to_erase_i.push_back(i->first);
        }
        for(unsigned k=0; k<to_erase_i.size(); k++)
        {
            container.erase(to_erase_i[k]);
        }

    }
}
