#ifndef CONTACTSDTATANICK_VEC___H
#define CONTACTSDTATANICK_VEC___H
#include <string>
#include <map>
#include "DB_id.h"
using namespace std;

class contact_vec : public db_row
{
public:
    unsigned int id;
    const char* getclassname()const {
        return "contact_vec ";
    }

    int load_from_db(const string & table_name);
    void on_id_not_exists(const string &tbname, unsigned int _id);

    contact_vec();
    ~contact_vec();
    set<DB_id> contact;
    void pack(out_oscar_buffer &b) const;
    void unpack(oscar_buffer &b);
};
extern db_map<unsigned int,contact_vec> contact_vecs;

#endif
