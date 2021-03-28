#include "stdafx.h"
#include <string>
#include "str_lib.h"
#include <stdio.h>
#include <mysql/mysql.h>
#include "mysql_extens.h"
#include "_mutex.h"
#include "mutex_inspector.h"
#include "file_set.h"
#include "st_stuff.h"

#include "db_config.h"
#include "lockctl.h"
#ifdef WIN32
#define snprintf _snprintf
#include <time.h>
#endif
#include "cMYSQL.h"
#include <sys/timeb.h>
MYSQL *std_mysql_real_connect_1(MYSQL *_mysqlp);
MYSQL *std_mysql_real_connect_2(MYSQL *_mysqlp);


cMYSQL *std_New_cMYSQL_1();
cMYSQL *std_New_cMYSQL_2();

void mysql_extens_cpp() {}

_mutex sql_log_mx("sqlmutex");
FILE *sql_f=NULL;
void sql_log(const string & r)
{
    return;
    MUTEX_LOCK kall(sql_log_mx);
    if(sql_f==NULL)
        sql_f=fopen("sql.log","ab");

    fprintf(sql_f,"%s: %s\n",CTIME(time(NULL)).c_str(),r.c_str());
    fclose(sql_f);
    sql_f=NULL;
}

static int UM_mysql_escape_string(char *buf, const char *s, unsigned int sz)
{
    return mysql_escape_string(buf, s,sz);
}

string MysqlEscapeString(const string & _str)
{
    size_t sz=_str.size() * 2;
    st_malloc buf(sz);

    size_t len = mysql_escape_string((char*)buf.buf, _str.c_str(), _str.size());
    string ret((char*)buf.buf, len);
    return ret;
}

vector < vector <string> >cMYSQL::exec(const string & query)
{   MUTEX_INSPECTOR;
    last_query_time=time(NULL);
    vector < vector <string> >rr;
    real_query(query);

    MYSQL_RES *r;
    {
        r= mysql_store_result(mysql);
    }
    if (r == NULL) {
        logErr("mysql_store_result failed: %s %s", mysql_error(mysql), query.c_str());
        error=true;
        err_str="mysql_store_result: Query: " + query + " Error: " + mysql_error(mysql)+_DMI();
        throw cError(err_str);
    }
    MYSQL_ROW row;
    int num_fields;
    {
        num_fields = mysql_num_fields(r);
    }

    while (1) {
        {
            row=mysql_fetch_row(r);
        }
        if(!row)break;
        unsigned long *lengths;
        {
            lengths = mysql_fetch_lengths(r);
        }
        vector <string> v;
        for (int i = 0; i < num_fields; i++) {
            string s = "";
            if (row[i]) {
                string a(row[i], lengths[i]);
                s = a;
            }
            v.push_back(s);
        }
        rr.push_back(v);
    }
    {
        mysql_free_result(r);
    }
    query_total_size += query.size();
    query_counter++;

    return rr;
}

void cMYSQL::real_query(const string & query)
{
    MUTEX_INSPECTOR;
    last_query_time=time(NULL);
    size_t res;
    {
        struct timeb t1;
        ftime(&t1);
        res=mysql_real_query(mysql, query.data(), query.size());
        struct timeb t2;
        ftime(&t2);
        int sek=t2.time-t1.time;
        int msek=sek*1000 + t2.millitm - t1.millitm;
#ifdef TUSA
//		if(msek)
//		    logErrN("sql","%d %s",msek,query.c_str());
#endif

    }
    if (res) {
        MUTEX_INSPECTOR;
        error=true;
        err_str=(string) "mysql_real_query failed on query:" + query + " Error:" + mysql_error(mysql)+_DMI();
        throw cError(err_str);
    }
    query_total_size += query.size();
    query_counter++;
}



cMYSQL::~cMYSQL()
{

    if (connected) {
        mysql_close(mysql);
    }
    if(mysql) {
        LEAKCTL_REMOVENS(mysql);
        delete mysql;
        mysql=NULL;
    }
}

string cDBH::select_1(const string & query)
{
    last_query_time=time(NULL);
    string ret;

    vector < vector <string> >res = exec(query);
    if (res.size()) {
        if (res[0].size())
            ret = res[0][0];
    }
    return ret;

}

vector <string> cDBH::select_1_column(const string & query)
{
    last_query_time=time(NULL);
    vector <string> ret;
    vector < vector <string> >res = exec(query);
    for (vector < vector <string> >::const_iterator i = res.begin(); i != res.end(); i++) {
        if ((*i).size())
            ret.push_back((*i)[0]);
    }
    return ret;
}

vector <string> cDBH::select_1_row(const string & query)
{
    last_query_time=time(NULL);
    vector <string> ret;
    vector < vector <string> >res = exec(query);
    if (res.size()!=0) {
        for (unsigned int i = 0; i < res[0].size(); i++)
            ret.push_back(res[0][i]);
    }
    return ret;
}
vector <string> cDBH::select_1_rowQ(const QUERY & query)
{
    last_query_time=time(NULL);
    QUERY q=query;
    return select_1_row(q.prepare());
}
void cDBH::real_queryQ(const QUERY & query)
{
    MUTEX_INSPECTOR;
    last_query_time=time(NULL);
    QUERY q=query;
    real_query(q.prepare());
}
vector <string> cDBH::select_1_columnQ(const QUERY & query)
{
    last_query_time=time(NULL);
    QUERY q=query;
    return select_1_column(q.prepare());
}
string cDBH::select_1Q(const QUERY & query)
{
    last_query_time=time(NULL);
    QUERY q=query;
    return select_1(q.prepare());
}
vector < vector <string> >cDBH::execQ(const QUERY & query)
{
    last_query_time=time(NULL);
    QUERY q=query;
    return exec(q.prepare());
}




MYSQL *std_mysql_real_connect_1(MYSQL *_mysqlp)
{
    return mysql_real_connect(_mysqlp,
                              strupper(string(db_config.dbhost))=="NULL"?NULL:string(db_config.dbhost).c_str(),
                              strupper(string(db_config.dbuser))=="NULL"?NULL:string(db_config.dbuser).c_str(),
                              strupper(string(db_config.dbpasswd))=="NULL"?NULL:string(db_config.dbpasswd).c_str(),
                              strupper(string(db_config.dbname))=="NULL"?NULL:string(db_config.dbname).c_str(),
                              db_config.dbport,
                              strupper(string(db_config.dbsock))=="NULL"?NULL:string(db_config.dbsock).c_str(),
                              0);
}

cMYSQL *std_New_cMYSQL_1()
{
    MYSQL *_mysqlp=new MYSQL;
    LEAKCTL_ADDNS(_mysqlp);
    _mysqlp=mysql_init(_mysqlp);
    if(!_mysqlp) {
        logErr2("---Error: mysql_init failed seems not sufficient memory");
        throw cError("mysql_init failed seems not sufficient memory"+_DMI());
    }
    MYSQL *res=std_mysql_real_connect_1(_mysqlp);
    if(!res) {
        const char *e=mysql_error(_mysqlp);
#ifdef DEBUG
        char errs[200];
        snprintf(errs,sizeof(errs)-1,"mysql_real_connect to '%s' failed [%s]  %s %d",string(db_config.dbname).c_str(),e?e:"mysql error undefined",__FL__);
        logErr2(errs);
        throw cError(errs+_DMI());
#else
        char errs[200];
        snprintf(errs,sizeof(errs)-1,"mysql_real_connect to '%s' failed [%s]",string(db_config.dbname).c_str(),e?e:"mysql error undefined");
        logErr2(errs);
        throw cError(errs+_DMI());
#endif
    }
    cMYSQL *mysqlp=new cMYSQL(_mysqlp);
    LEAKCTL_ADD(mysqlp);
    if(mysqlp==NULL)
    {
        string s("---Error cMYSQL *mysqlp=new cMYSQL(_mysqlp) failed");
        logErr2("%s",s.c_str());
        throw cError(s+_DMI());
    }
    mysqlp->real_query(db_config.dbstart_code);
//	logErr2("SET CHARSET cp1251");
    mysqlp->connected=true;
    return mysqlp;
    return NULL;
}



static _mutex __dbh_set_mutex("__dbh_set_mutex");
static set<cDBH *> __dbh_set;

static m_var<time_t> last_flush_time_1(0);
st_dbh::~st_dbh()
{
    MUTEX_INSPECTOR;

    if (dbh) {
        MUTEX_LOCK kall(__dbh_set_mutex);
        {
            if(dbh->error) {
                logErr2("st_dbh::~st_dbh error %s",dbh->err_str.c_str());
                LEAKCTL_REMOVE(dbh);
                delete dbh;
            }
            else __dbh_set.insert(dbh);
        }
    }
}
st_dbh::st_dbh():dbh(NULL)
{
    MUTEX_INSPECTOR;
    if(time(NULL)-last_flush_time_1.get()>60)
    {
        last_flush_time_1=time(NULL);
        MUTEX_LOCK kall(__dbh_set_mutex);
        vector<cDBH*> to_close;
        for (set<cDBH *>::const_iterator i = __dbh_set.begin(); i != __dbh_set.end(); i++) {
            MUTEX_INSPECTOR;
            if (1) {
                dbh = (*i);
                if(dbh)
                {
                    if(time(NULL)-dbh->last_query_time>600)
                    {
                        to_close.push_back(dbh);

                    }
                }
            }
        }
        {   for(unsigned i=0; i<to_close.size(); i++)
            {
                __dbh_set.erase(to_close[i]);
                LEAKCTL_REMOVE(to_close[i]);
                delete to_close[i];
            }
        }
    }
    {
        MUTEX_LOCK kall(__dbh_set_mutex);
        if(__dbh_set.size()) {
            dbh=*__dbh_set.begin();
            __dbh_set.erase(__dbh_set.begin());
            return;
        }
    }
    dbh = std_New_cMYSQL_1();

    if (!dbh)
        throw cError("!dbh11"+_DMI());

}



cMYSQL::cMYSQL(MYSQL*m)
{
    mysql=m;
    last_query_time=time(NULL);
    error=false;
    connected=false;
    query_counter=0;
    query_total_size=0;
}
