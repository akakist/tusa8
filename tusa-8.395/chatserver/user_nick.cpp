#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
using namespace std;
#include "oscar_buffer.h"
#include "mutex_inspector.h"
#include "DB_id.h"
#include "user_nick.h"
#include "user.h"
#include "cutils.h"
user_nick::user_nick(const string &_name,unsigned int _id, const DB_id& _uid):uid(_uid)
{
    name=_name;
    id=_id;
    uid=_uid;
}
user_nick::user_nick()
{
    CONTAINER(uid)=0;
    name="";
    id=user_state::UNDEF;
    CONTAINER(uid)=user_state::UNDEF;

}
bool NICK_EXISTS(const DB_id& uid,const string &nick)
{


    PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(uid)));
    string n=str_nick(nick);
    for (map<unsigned int,user_nick>::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++) {
        if (str_nick(i->second.name)==n) return true;
    }
    return false;

}

user_nick get_nick(const DB_id&uid,unsigned int n)
{
    PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(uid)));

    user_nick nn;
    map<unsigned int,user_nick>::const_iterator i=u->u_nicks.find(n);
    if (i!=u->u_nicks.end())
    {
        return i->second;
    }
    if (i==u->u_nicks.end() && u->u_nicks.size())
        return u->u_nicks.begin()->second;

    return nn;
}
user_nick get_nick(const DB_id&uid,const string& n)
{
    PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(uid)));

    user_nick nn;
    string N=str_nick(n);
    for (map<unsigned int,user_nick>::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++)
    {
        if (N==str_nick(i->second.name)) return i->second;
    }
    if (u->u_nicks.size())
        return u->u_nicks.begin()->second;

    return nn;
}
unsigned int get_cnick_id(const user_pair&__U)
{

    {
        PGLR(user_profile, u,__U.up);
        return u->last_nick_id;
    }
    return 0;
}

string get_cnick(const user_pair&__U)
{

    {
        PGLR(user_profile, u,__U.up);
        return u->last_nick;
    }
    return "";
}

/*string get_cnick_dbid(const DB_id& uid)
{
	PTR_getter<user_profile> __P=user$profiles.FindByID(CONTAINER(uid));
    if(__P){
        PGLR(user_profile, u,__P);
		if(u)
		{
			return u->last_nick;
		}
    }
    return "";
}*/

bool user_nick::operator!=(const user_nick& a) const
{
    if (a.name!=name)return true;
    if (a.id!=id)return true;
    if (a.uid!=uid)return true;
    return false;
}
out_oscar_buffer & operator<<(out_oscar_buffer&b, const user_nick &n)
{

    b<<n.uid<<n.id<<n.name;
    return b;
}
oscar_buffer & operator>>(oscar_buffer&b, user_nick &n)
{


    b>>n.uid>>n.id>>n.name;
    return b;
}
int operator < (const user_nick& a, const user_nick& b) {
    return str_nick(a.name)<str_nick(b.name);
}
