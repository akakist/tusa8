#ifndef DBVER_CONFIGHDV
#define DBVER_CONFIGHDV
#include "M_MAP.h"
#include "M_SET.h"
#include "m_var.h"
using namespace std;
struct __db_config
{
    m_var<string>dbhost;
    m_var<string>dbuser;
    m_var<string>dbpasswd;
    m_var<string>dbname;
    m_var<unsigned int>dbport;
    m_var<string>dbsock;
    m_var<string>dbstart_code;
    void load_config(const string&pn);
};
extern __db_config db_config;


#endif
