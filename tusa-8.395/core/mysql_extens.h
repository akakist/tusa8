#ifndef MYSQL_EXTENS__Hjj
#define MYSQL_EXTENS__Hjj
#include <string>
#include <vector>
#include "_QUERY.h"

class cDBH
{
public:
    void real_queryQ(const QUERY& query);
    vector < vector <string> >execQ (const QUERY &query);
    string select_1(const string &query);
    string select_1Q(const QUERY &query);
    vector<string> select_1_column(const string &query);
    vector<string> select_1_columnQ(const QUERY &query);
    vector<string> select_1_row(const string &query);
    vector<string> select_1_rowQ(const QUERY &query);
    virtual void real_query(const string& query)=0;
    virtual vector < vector <string> >exec (const string &query)=0;

    time_t last_query_time;
    string err_str;
    bool error;

    cDBH():last_query_time(time(NULL)),error(false) {}
    virtual ~cDBH() {}

};


struct st_dbh
{
    cDBH *dbh;
    st_dbh();
    ~st_dbh();
};




string MysqlEscapeString(const string& _str);
#ifdef NO_DBH_ALLOWED
#endif
#define MES(a) MysqlEscapeString((a))

#endif
