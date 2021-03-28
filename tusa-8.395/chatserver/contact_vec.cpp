#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
#include "db_var.h"
#include "contact_vec.h"
#include "user.h"

#include "mysql_extens.h"
contact_vec::contact_vec() {
    id=0;
}
contact_vec::~contact_vec() {}

int contact_vec::load_from_db(const string & table_name)
{
    MUTEX_INSPECTOR;


    st_dbh d;

    vector<string> v=d.dbh->select_1_columnQ((QUERY)"select cuid from contact where uid=?"<<id);
    for (unsigned i=0; i<v.size(); i++)
    {
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=atoi(v[i]);
        contact.insert(id);
    }

    return 0;

}
void contact_vec::pack(out_oscar_buffer &b) const
{
    b<<contact;
}
void contact_vec::unpack(oscar_buffer &b)
{
    b>>contact;
}
void contact_vec::on_id_not_exists(const string &tbname, unsigned int _id)
{
    logErr2("invalid call on_id_not_exists %s %d %s",tbname.c_str(),_id,_DMI().c_str());
    throw cError("invalid call on_id_not_exists %s %d %s",tbname.c_str(),_id,_DMI().c_str());
}
