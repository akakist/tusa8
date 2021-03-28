#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
#include "db_var.h"
#include "nick_vec.h"
#include "user.h"
#include "cutils.h"
#include "web_tools.h"

#include <math.h>
nick_vec::nick_vec() {
    id=0;
}
nick_vec::~nick_vec() {}

int nick_vec::load_from_db(const string & table_name)
{


    st_dbh d;
    u_nicks.clear();
    vector<vector<string> >v=d.dbh->execQ((QUERY)"select id,nick,uid from nicks where banned=0 and uid=? and id<>0"<<id);
    for (unsigned i=0; i<v.size(); i++)
    {
        if (v[i].size()==3)
        {
            DB_id uid;
            CONTAINER(uid)=0;
            CONTAINER(uid)=atoi(v[i][2]);

            user_nick n(v[i][1],atoi(v[i][0]),uid);
            u_nicks.insert(pair<unsigned int, user_nick>(n.id,n));
        }
    }

    return 0;
}

int srt_nick__(const pair<string,string> &a,const pair<string,string> &b)
{
    return str_nick(a.second)<str_nick(b.second);
}


string build_nicks_select(const PTR_getter<nick_vec> &__N,const string &name,const string defnick,const string &addp)
{
    vector<pair<string,string> >m;
    int def=0;
    PGLR(nick_vec, u,__N);
    for (map<unsigned int,user_nick>::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++) {
        if (str_nick(defnick)==str_nick(i->second.name)) {
            def=i->second.id;
        }
        m.push_back(pair<string,string> (itostring(i->second.id),i->second.name));


    }
    sort(m.begin(),m.end(),srt_nick__);
    return make_select(name,m,itostring(def),addp);

}
string build_nicks_select(const PTR_getter<nick_vec> &__N,const string &name,int def,const string &addp)
{
    vector< pair<string,string> > m;
    PGLR(nick_vec, u,__N);
    for (map<unsigned int,user_nick>::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++) {
        m.push_back(pair<string,string> (itostring(i->second.id),i->second.name));
    }
    sort(m.begin(),m.end(),srt_nick__);
    return make_select(name,m,itostring(def),addp);

}
string build_nicks_select(const DB_id& uid,const string &name,int def,const string &addp)
{
    return build_nicks_select(nick_vecs.find_by_id_create(CONTAINER(uid)),name,def,addp);
}
string build_nicks_select(const DB_id& uid,const string &name,const string &defn,const string &addp)
{
    return build_nicks_select(nick_vecs.find_by_id_create(CONTAINER(uid)),name,defn,addp);
}



void nick_vec::pack(out_oscar_buffer &b) const
{
    b<<u_nicks;
}
void nick_vec::unpack(oscar_buffer &b)
{
    b>>u_nicks;
}

void nick_vec::on_id_not_exists(const string &tbname, unsigned int _id)
{

    logErr2("invalid call on_id_not_exists %s %d %s",tbname.c_str(),_id,_DMI().c_str());
    throw cError("invalid call on_id_not_exists %s %d %s",tbname.c_str(),_id,_DMI().c_str());
}
