#ifndef NICK_VEC___H
#define NICK_VEC___H
#include <string>
#include <map>
using namespace std;
#include "user_nick.h"
#include "db_var.h"
class nick_vec : public db_row
{
public:
    unsigned int id;
    const char* getclassname()const {
        return "nick_vec";
    }

    int load_from_db(const string & table_name);
    void on_id_not_exists(const string &tbname, unsigned int id);


    nick_vec();
    ~nick_vec();
    map<unsigned int,user_nick> u_nicks;

    void pack(out_oscar_buffer &b) const;
    void unpack(oscar_buffer &b);
};
extern db_map<unsigned int,nick_vec> nick_vecs;

#endif
