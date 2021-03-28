#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif



#include "mutex_inspector.h"
#include <time.h>
#include <map>
#include "mysql_extens.h"

#include "user.h"
#include "cutils.h"
#include "copy_file.h"
#include "user_credits.h"
#include "server_config.h"
#include "update_cmd.h"
#include "app_con.h"
#include "chat_config.h"

static _mutex __LOG_dump("__LOG_dump");


static void ___dump_log(const string & pn_pref, const string &s)
{
    MUTEX_INSPECTOR;
    tm tt=LOCALTIME(time(NULL));
    char ss[200];
    snprintf(ss,sizeof(ss)-1,"%s.%04d%02d%02d",pn_pref.c_str(),1900+tt.tm_year,tt.tm_mon+1,tt.tm_mday);
    M_LOCK(__LOG_dump);
    check_path_wfn(ss);
    FILE *f = ::fopen(ss, "a");
    if (f) {
        if (s.size())
            ::fprintf(f, "%s\n", s.c_str());
        ::fclose(f);
    }

}

M_DEQUE<credit_transaction> credit_transactions;

void dump_credits_log()
{
    MUTEX_INSPECTOR;
    DB_id zzz;
    CONTAINER(zzz)=0;
    credit_transaction ct(zzz,zzz);
    st_dbh d;
    while (credit_transactions.extract_first(ct))
    {
        char s[100];
        snprintf(s,sizeof(s)-1,"%f",ct.summa.container);
        d.dbh->real_queryQ((QUERY)"insert into credit_log (dt,from_uid,to_uid,summa,opcode) values (FROM_UNIXTIME(?),'?','?','?','?')"
                           <<ct.t<<CONTAINER(ct.from_user_id)<<CONTAINER(ct.to_user_id)<<s<<ct.operation_code);
    }
}



bool inc_credit_summa(const DB_id& to_uid,Double summa, const string &opcode)
{

    string tlg;
    if (summa.container<0) throw cError("if(summa<0)");
    try {
        PTR_getter<user_profile> p(NULL);
        Double t_before;
        if (p) {
            PGLW(user_profile,u,p);
            tlg=u->login;
        }
        st_dbh d;
        d.dbh->real_queryQ((QUERY)"update tbl_users set credit_summa=credit_summa+? where id=?"
                           <<summa.container<<CONTAINER(to_uid));

        logErrN("credit","credit$inc_summa  t %s (%d) sum %f (t=%s) %s",tlg.c_str(),CONTAINER(to_uid),summa.container,d.dbh->select_1Q((QUERY)"select credit_summa from tbl_users where id=?"<<CONTAINER(to_uid)).c_str(),opcode.c_str());
    }
    catch (...) {
    }

    DB_id id0;
    CONTAINER(id0)=0;
    credit_transaction ct(id0,to_uid);
    ct.from_user_id=id0;
    ct.to_user_id=to_uid;
    ct.summa=summa;
    ct.operation_code=opcode;
    ct.t=time(NULL);
    credit_transactions.push_back(ct);
    return true;
}
bool buy_thing(const DB_id& from_uid,const DB_id& to_uid,const Double &summa, const string &opcode,Double& reminder_from, Double &reminder_to) // true if success
{
    MUTEX_INSPECTOR;
    if (summa.container<0) throw cError("if(summa<0)");
    if (from_uid==to_uid)return false;
    st_dbh d;
    double from_summa=atof(d.dbh->select_1Q((QUERY)"select credit_summa from tbl_users where id=?"<<CONTAINER(from_uid)).c_str());
    if (from_summa<summa.container) {
        return false;
    }
    d.dbh->real_queryQ((QUERY)"update tbl_users set credit_summa=credit_summa-? where id=?"<<summa.container<<CONTAINER(from_uid));
    d.dbh->real_queryQ((QUERY)"update tbl_users set credit_summa=credit_summa+? where id=?"<<summa.container<<CONTAINER(to_uid));

    string flg,tlg;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(to_uid)));
        tlg=u->login;
    }
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(from_uid)));
        flg=u->login;
    }

    try {
        reminder_from.container=atof(d.dbh->select_1Q((QUERY)"select credit_summa from tbl_users where id=?"<<CONTAINER(from_uid)).c_str());
        reminder_to.container=atof(d.dbh->select_1Q((QUERY)"select credit_summa from tbl_users where id=?"<<CONTAINER(to_uid)).c_str());
        logErrN("credit","credit$move_summa f %s (%d) t %s (%d) sum %f (f=%f,t=%f) %s",flg.c_str(),CONTAINER(from_uid),tlg.c_str(),CONTAINER(to_uid),summa.container,reminder_from.container,reminder_to.container,opcode.c_str());
    }
    STDCATCHS("buy_thing");

    credit_transaction ct(from_uid,to_uid);
    ct.from_user_id=from_uid;
    ct.to_user_id=to_uid;
    ct.summa=summa;
    ct.operation_code=opcode;
    ct.t=time(NULL);
    credit_transactions.push_back(ct);

    return true;

}


