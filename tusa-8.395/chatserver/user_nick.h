#ifndef USER_NICK___H
#define USER_NICK___H
#include <string>
#include "Double.h"
#include "DB_id.h"
using namespace std;
class user_nick
{
public:
    string name;
    unsigned int id;
    DB_id uid;
    user_nick();
    user_nick(const string &_name,unsigned int _id, const DB_id& _uid);
    bool operator!=(const user_nick&) const;
};
int operator < (const user_nick& a, const user_nick& b);

out_oscar_buffer & operator<<(out_oscar_buffer&b, const user_nick &n);
oscar_buffer & operator>>(oscar_buffer&b, user_nick &n);

struct user_pair;
string get_cnick(const user_pair&__U);
//string get_cnick_dbid(const DB_id& uid);
unsigned int get_cnick_id(const user_pair&__U);
user_nick get_nick(const DB_id &uid,unsigned int n);
user_nick get_nick(const DB_id &uid,const string & n);
bool NICK_EXISTS(const DB_id& uid,const string &nick);

#endif
