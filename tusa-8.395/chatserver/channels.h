#ifndef CHANNLES_H
#define CHANNLES_H
#include "PTR.h"
#include "user.h"
class __chat_channels
{
private:
    map<CH_id,PTR_getter<chat_channel > >_container;
    map<string,PTR_getter<chat_channel > >container_s;
    vector<CH_id> ___keys;
    vector<string> ___keys_s;
    _mutex Mutex;
public:
    void add(const CH_id&  channel,const PTR_getter<chat_channel> &p);
    PTR_getter<chat_channel> operator[](const CH_id&);
    PTR_getter<chat_channel> ___get(const CH_id&);
    PTR_getter<chat_channel> ___get_s(const string&);
    vector<CH_id> _keys();
    vector<string> _keys_s();
    set<CH_id> conf_keys();
    CH_id get_id(const string& irc_name);

    size_t count(const CH_id &);

    void clear();
    __chat_channels():Mutex("__chat_channels") {}

};
extern	 __chat_channels  chat_channels;
bool load_channels$$$();

#ifdef LOCKCTL_DBUG
#define __CH_LOCK(__ii__) PGLR(chat_channel, CH,__ii__);
#define __CH_LOCKW(__ii__) PGLW(chat_channel, CH,__ii__);
#else
#define __CH_LOCK(__ii__) PGLR(chat_channel, CH,__ii__);
#define __CH_LOCKW(__ii__) PGLW(chat_channel, CH,__ii__);
#endif

string make_select(const string &n,map<CH_id,string> &p,const CH_id &def,const string &add);
string make_select(const string &n,vector<pair<CH_id,string> > &p,const CH_id &def,const string &add);

#endif
