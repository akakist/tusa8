#ifndef ignoreSDTATANICK_VEC___H
#define ignoreSDTATANICK_VEC___H
#include <string>
#include <map>
#include "DB_id.h"
#include <set>
using namespace std;

class ignore_vec : public db_row
{
public:
    unsigned int id;
    const char* getclassname()const {
        return "ignore_vec";
    }

    int load_from_db(const string & table_name);
    void on_id_not_exists(const string &tbname, unsigned int _id);


    ignore_vec();
    ~ignore_vec();
    set<DB_id> ignore;
    void pack(out_oscar_buffer &b) const;
    void unpack(oscar_buffer &b);

};
extern db_map<unsigned int,ignore_vec> ignore_vecs;

#endif
