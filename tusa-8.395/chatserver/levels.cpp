#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "map"
#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <errno.h>
#include <map>
#include "_mutex.h"
#include "user.h"
#include "cutils.h"
#include "str_lib.h"
#include "web_server.h"
#include "message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include "chat_def.h"
#include "_mutex.h"
#include <stdio.h>
#include "errs.h"
#include "M_MAP.h"
#include "file_set.h"
#include "web_tools.h"
#include "web_server_data.h"
#include "web_server.h"
#include "levels.h"
#include "server_config.h"

#include "chat_config.h"
#include <ctype.h>

map<string,string> load_m_file(const string &fn);
vector<string> load_l_file(const string &fn);


static map<unsigned int, c_level> levels;
static _mutex levels_Mutex("levels_Mutex");



map<unsigned int, c_level> get_all_levels()
{
    M_LOCK(levels_Mutex);
    return levels;
}
string LEVEL_NAME(unsigned int id,unsigned char gender)
{
    M_LOCK(levels_Mutex);
    map<unsigned int, c_level>::const_iterator i=levels.find(id);
    if (i==levels.end()) return "";
    return i->second.name(gender);

}
static bool is_emptyS(const string &s)
{
    bool ret=true;
    for (unsigned i=0; i<s.size(); i++)
    {
        if (!isspace(s[i])) ret=false;
    }
    return ret;
}
void pack_levels(out_oscar_buffer &b)
{
    M_LOCK(levels_Mutex);
    b<<levels;
}
void unpack_levels(oscar_buffer &b)
{
    M_LOCK(levels_Mutex);
    b>>levels;
}

bool load_levels(const string &pn)
{
    MUTEX_INSPECTOR;
    st_dbh d;
    d.dbh->real_query("drop table IF EXISTS levels");
    d.dbh->real_query("drop table IF EXISTS level_names");
    d.dbh->real_query("drop table IF EXISTS level_colors");
    d.dbh->real_query("drop table IF EXISTS level_replacements");
    d.dbh->real_query("CREATE table levels (\
                      `ID` int(20) not null default 0,\
                      `msg_len` int(20) not null default 100,\
                      nicks_count  int(20) not null default 3,\
                      privs  int(20) not null default 0,\
                      invisibility  int(20) not null default 0,\
                      levelup_ability  int(20) not null default 0,\
                      kick_shield  int(20) not null default 0,\
                      kick_ability  int(20) not null default 0,\
                      replacements text not null default '',\
                      KEY(id));");
    d.dbh->real_query("CREATE table level_names (\
                      `ID` int(20) not null default 0,\
                      `name` text,\
                      `gender` int(2),\
                      KEY(id),KEY(gender))");

    d.dbh->real_query("CREATE table level_colors (\
                      `ID` int(20) not null default 0,\
                      `cpack` int(20),\
                      KEY(id))");

    d.dbh->real_query("CREATE table level_replacements (\
                      `ID` int(20) not null default 0,\
                      `replacement` int(20),\
                      KEY(id))");

    M_LOCK(levels_Mutex);
    logErr("Loading levels...");
    levels.clear();
    params_map levels_conf;
    levels_conf.clear();
    levels_conf.read(pn);
    map<string,string> c=levels_conf.get_container();
    for (map<string,string>::const_iterator i=c.begin(); i!=c.end(); i++) {
        string nn=i->first;
        if (nn.substr(0,1)=="#" ||!nn.size()) continue;
        if (is_emptyS(nn)) continue;
        unsigned int n=atoi(nn);
        map<string,string> params=load_m_file(i->second);
        c_level lev;
        if (levels.find(n)!=levels.end()) lev=levels[n];
        lev.colors.clear();
        lev.replacements.clear();
        lev.id=n;
        lev.max_msg_len=atoi(params["msg_len"]);
        lev._max_nicks_count=atoi(params["nicks_count"]);

        lev.invisibility=atoi(params["invisibility"].c_str());
        lev.levelup_ability=atoi(params["levelup_ability"]);

        lev.kick_shield=atoi(params["kick_shield"]);
        lev.kick_ability=atoi(params["kick_ability"]);
        string repl=params["replacements"];
        d.dbh->real_queryQ((QUERY)"INSERT DELAYED INTO levels (id,msg_len,nicks_count,invisibility,levelup_ability,kick_shield,kick_ability,replacements)\
                           values(?,?,?,?,?,?,?,'?')\
                           "<<lev.id<<lev.max_msg_len<<lev._max_nicks_count<<lev.invisibility<<lev.levelup_ability<< lev.kick_shield<<lev.kick_ability<<repl);
        for (map<string,string>::const_iterator q=params.begin(); q!=params.end(); q++) {
            string s=q->first;
            int pz=s.find("@",0);
            if (pz!=-1) {
                if (s.substr(0,2)!="en")
                {
                    s=s.substr(pz+1,s.size()-pz-1);
                }
            }
            if (s=="names") {
                vector<string> v=splitTOK(",",q->second);
                if (!v.size()) {
                    logErr("---Error: no names for level %d defined. ignoring...",lev.id);
                    continue;
                }
                for (unsigned j=v.size(); j<=2; j++) v.push_back(v[0]);
                lev.names=v;
                for (unsigned j=0; j<lev.names.size(); j++)
                {
                    d.dbh->real_queryQ((QUERY)"INSERT DELAYED into level_names (id,name,gender) values ('?','?','?')"<<lev.id<<lev.names[j]<<j);
                }
            }
            if (s=="replacements") {
                repl=q->second;
                vector<string> repls=load_l_file(q->second);
                for (unsigned int z=0; z<repls.size(); z++) {
                    string k=repls[z];
                    int ppz=k.find(" ");
                    if (ppz==-1) {
                        logErr("--Error: load_levels: replacements: syntax error in %s line %d",q->second.c_str(),z);
                        continue;
                    }
                    string key=k.substr(0,ppz);
                    string val=k.substr(ppz+1,k.size()-ppz-1);
                    lev.replacements[key]=val;
                }
            }
            if (s=="color_packs")
            {
                vector<string> c=splitTOK(",",q->second);
                for (unsigned i=0; i<c.size(); i++)
                {
                    lev.colors.insert(atoi(c[i]));
                    d.dbh->real_queryQ((QUERY)"INSERT DELAYED into level_colors (id,cpack) values ('?','?')"<<lev.id<<c[i]);
                }
            }
        }
        levels[n]=lev;
    }
    return true;
}
map<unsigned int, c_level> LEVELS_GET()
{
    M_LOCK(levels_Mutex);
    return levels;
}

c_level __LEVELS(unsigned int n)
{
    M_LOCK(levels_Mutex);
    c_level l;
    map<unsigned int, c_level>::const_iterator i=levels.find(n);
    if (i!=levels.end()) {
        l=i->second;
    } else {
        throw cError("--Error: level n="+itostring(n)+" not found!" +_DMI());
    }
    return l;
}

string c_level::name(unsigned int g) const
{

    if (g<names.size())
        return names[g];
    else
        throw cError("level name  has (i->sec.size()<=gender) n="+itostring(id)+" gender="+itostring(g)+_DMI());
    return "";
}

void c_level::assign_names(const vector<string>& v)
{
    names=v;
}

void c_level::add_names(const vector<string> &v)
{
    names=v;
}

void c_level::print()
{
    MUTEX_INSPECTOR;
    logErr("Loaded level %d: nicks_count=%d message_len=%d invisibility=%2.2f kick_shield=%2.2f kick_ability=%2.2f levelup_ability=%d",id,_max_nicks_count,max_msg_len,invisibility,kick_shield,kick_ability,levelup_ability);
    string ln;
    for (unsigned int j=0; j<names.size(); j++) {
        ln+=names[j]+" ";
    }
    string cl;
    {
        for (set<unsigned int>::const_iterator i=colors.begin(); i!=colors.end(); i++) {
            cl+=itostring(*i)+" ";
        }
    }
    logErr("\t level names: %s",ln.c_str());
    logErr("\t color: %s",cl.c_str());
}
oscar_buffer& operator>>(oscar_buffer & b, c_level   &c)
{
    b>>c.names;


    b>>c._max_nicks_count;
    b>>c.max_msg_len;

    b>>c.colors;
    b>>c.id;
    b>>c.levelup_ability;

    b>>c.invisibility;
    b>>c.kick_shield;
    b>>c.kick_ability;
    return b;

}
out_oscar_buffer& operator<<(out_oscar_buffer & b, const c_level & c)
{
    b<<c.names;


    b<<c._max_nicks_count;
    b<<c.max_msg_len;

    b<<c.colors;
    b<<c.id;
    b<<c.levelup_ability;

    b<<c.invisibility;
    b<<c.kick_shield;
    b<<c.kick_ability;
    return b;
}
