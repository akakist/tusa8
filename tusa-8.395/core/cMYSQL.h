#ifndef CMYSQL__H
#define CMYSQL__H
class cMYSQL: public cDBH
{
public:
    const char* getclassname()const {
        return "cMYSQL";
    }

    int query_counter;
    size_t query_total_size;
    MYSQL *mysql;
    bool connected;
    cMYSQL(MYSQL*m);
    cMYSQL() {
    }
    ~cMYSQL();
    void real_query(const string& query);
    vector < vector <string> >exec (const string &query);
};
#endif
