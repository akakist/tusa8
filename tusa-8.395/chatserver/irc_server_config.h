#ifndef IRCSERVER_CONFIGH
#define IRCSERVER_CONFIGH
using namespace std;
#include "M_MAP.h"
#include "M_SET.h"
#include "m_var.h"
struct __irc_server_config
{
    m_var<unsigned int>ping_kill_timeout;
    m_var<unsigned int>ping_time;


    m_var<string>msg001;
    m_var<string>msg002;
    m_var<string>msg003;
    m_var<string>msg004;
    m_var<string>msg254;
    m_var<string>msg255;
    m_var<string>msg375;
    m_var<string>host;
    m_var<string>location;
    m_var<string>motd;
    m_var<unsigned int> login_timeout;

    __irc_server_config();
    void load_config(const string&pn);
};
extern __irc_server_config irc_server_config;
string irc_nick$host(const string &n);


#endif
