#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <errno.h>
#include <map>
#include "_mutex.h"
#include "cutils.h"
#include "str_lib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "_mutex.h"
#include <stdio.h>
#include "errs.h"
#include "file_set.h"
#include "chat_colors.h"
#include "server_config.h"
#include "chat_config.h"
#include "time_limit.h"
__c_packs c_packs;

static void l(const string &fn,string &p,map<string,string> &m, const string &name)
{
    if (!m.count(name)) {
        logErr2("color_pack (%s) member '%s' undefined",fn.c_str(),name.c_str());
        throw cError("color_pack member undefined"+_DMI());
    }
    p=m[name];
}
void load_color_packs(const string &scpack,const string& conf_name)
{
    st_dbh d;
    d.dbh->real_query("DROP TABLE IF EXISTS `chat_colors`");
    d.dbh->real_query("CREATE TABLE `chat_colors` (\
  `id` int(11) NOT NULL default '0',\
  `name` text NOT NULL,\
  `imagefname` text NOT NULL,\
  `makeit` text NOT NULL,\
  `nick` text NOT NULL,\
  `bnick` text NOT NULL,\
  `bnick2` text NOT NULL,\
  `nickclass` text NOT NULL,\
  `nickclass_birthdate` text NOT NULL,\
  `privfrom` text NOT NULL,\
  `privto` text NOT NULL,\
  `std` text NOT NULL,\
  `stdfrom` text NOT NULL,\
  `stdto` text NOT NULL,\
  `sys` text NOT NULL,\
  `syspriv` text NOT NULL,\
  KEY `id` (`id`)\
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;\
");


    MUTEX_INSPECTOR;
    CKTIME;
    vector<string> v=splitTOK(",\r\n;",scpack);
    for (unsigned i=0; i<v.size(); i++)
    {
        MUTEX_INSPECTOR;
        vector<string> w=splitTOK(":=",v[i]);
        if (w.size()==2)
        {
            MUTEX_INSPECTOR;
            unsigned id=atoi(w[0]);
            map<string,string> m=load_m_file(w[1]);
            if (m.size())
            {
                MUTEX_INSPECTOR;
                __c_pack *p=new __c_pack;
                LEAKCTL_ADD(p);
                p->id=id;
                p->_conffile=w[1];
                l(w[1],p->bnick,m,"bnick");
                l(w[1],p->bnick2,m,"bnick2");
                l(w[1],p->makeit,m,"makeit");
                l(w[1],p->name,m,"name");
                l(w[1],p->nick,m,"nick");
                l(w[1],p->nickclass,m,"nickclass");
                l(w[1],p->nickclass_birthdate,m,"nickclass_birthdate");
                l(w[1],p->privfrom,m,"privfrom");
                l(w[1],p->privto,m,"privto");
                l(w[1],p->stdfrom,m,"stdfrom");
                l(w[1],p->std,m,"std");
                l(w[1],p->stdto,m,"stdto");
                l(w[1],p->sys,m,"sys");
                l(w[1],p->syspriv,m,"syspriv");

                {
                    MUTEX_INSPECTOR;
                    {
                        MUTEX_INSPECTOR;
                        c_packs.insert(id,p);
                    }
                    {

                        MUTEX_INSPECTOR;
                        d.dbh->real_queryQ((QUERY)"insert into chat_colors (id,bnick,bnick2,makeit,name,nick,nickclass,nickclass_birthdate,privfrom,privto,stdfrom,std,stdto,sys,syspriv)\
								values('?','?','?','?','?','?','?','?','?','?','?','?','?','?','?')"<<
                                           p->id<<
                                           p->bnick<<
                                           p->bnick2<<
                                           p->makeit<<
                                           p->name<<
                                           p->nick<<
                                           p->nickclass<<
                                           p->nickclass_birthdate<<
                                           p->privfrom<<
                                           p->privto<<
                                           p->stdfrom<<
                                           p->std<<
                                           p->stdto<<
                                           p->sys);
                    }
                }
            }
        }
        else
        {
            logErr2("--Error: in %s, item color_pack '%s' has invalid syntax",conf_name.c_str(),v[i].c_str());
            continue;
        }
    }
}

void __c_packs::insert(unsigned int id, const PTR_getter<__c_pack > &c)
{
    M_LOCK(Mutex);
    container.insert(pair < unsigned int, PTR_getter<__c_pack > >(id, c));
}
PTR_getter<__c_pack > __c_packs::get(unsigned int id)
{
    M_LOCK(Mutex);
    map< unsigned int, PTR_getter<__c_pack > >::const_iterator i = container.find(id);
    if (i == container.end()) {
        if (container.size())
            return container.begin()->second;
        else
            throw cError("color packs size=0"+_DMI());
    }
    return i->second;
}
unsigned int __c_packs::get_first_id()
{
    M_LOCK(Mutex);
    if (container.size())
        return container.begin()->first;
    else
        throw cError("color packs size=0"+_DMI());
}
int __c_packs::count(unsigned int n)
{
    M_LOCK(Mutex);
    return container.count(n);
}
string _nick$(const user_nick &n,const bool priv,const PTR_getter<__c_pack> &cp)
{
    map<string,string> m;
    char s[200];
    string mname=MES(n.name);
    snprintf(s,sizeof(s),"Answer(%d,%d,'%s',%d)",CONTAINER(n.uid),n.id,mname.c_str(),priv);
    m["~nick~"]=mname;
    m["~action~"]=s;
    m["~uid~"]=itostring(CONTAINER(n.uid));

    string nick;
    {
        PGLR(__c_pack, c,cp);
        {
            nick=c->nick;
        }
    }

    string out;
    out=replace_vals(m,nick);

    return out;

}

string _badnick$(const string &s,const PTR_getter<__c_pack> &cp)
{
    map<string,string> m;
    m["~nick~"]=s;
    string bnick;
    {
        PGLR(__c_pack, c,cp);
        {
            bnick=c->bnick;
        }
    }
    string r=replace_vals(m,bnick);
    return r;

}

string _badnick$2(const user_nick &s,const PTR_getter<__c_pack> &cp)
{
    map<string,string> m;
    m["~nick~"]=s.name;
    m["~uid~"]=itostring(CONTAINER(s.uid));

    string bnick;
    {
        PGLR(__c_pack, c,cp);
        {
            bnick=c->bnick2;
        }
    }
    string r=replace_vals(m,bnick);
    return r;

}
