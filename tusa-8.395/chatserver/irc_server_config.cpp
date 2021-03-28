#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <signal.h>
#include <errno.h>
#include "file_set.h"
#include "mutex_inspector.h"
#include "str_lib.h"
#include "irc_server_config.h"
#include "cutils.h"
__irc_server_config irc_server_config;
static char __motd[]="OnR1c292a2EudG9tc2sucnUgMzcyIGFhYSA6LSAtPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LQ0KOnR1c292a2EudG9tc2sucnUgMzcyIGFhYSA6LSAgICBUdXNvdmthLlRvbXNrLlJ1ICAgICAgICAgICAgICAgICAgVG9tc2sgQ2hhdCBTZXJ2ZXIgKHR1c292a2EudG9tc2sucnUpDQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotID09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT0NCjp0dXNvdmthLnRvbXNrLnJ1IDM3MiBhYWEgOi0gTG9jYXRpb246ICAgICAgICBUb21zaywgUnVzc2lhIA0KOnR1c292a2EudG9tc2sucnUgMzcyIGFhYSA6LSBTdXBwb3J0OiAgICAgICAgIHN1cHBvcnRAdHVzb3ZrYS50b21zay5ydQ0KOnR1c292a2EudG9tc2sucnUgMzcyIGFhYSA6LSA9PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09DQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotICAgICAgICAgICAgICAgICAgICAgICAgICAgIFJ1bm5pbmcgYm90cyBhcmUgc3Ryb25nbHkgZm9yYmlkZGVuLg0KOnR1c292a2EudG9tc2sucnUgMzcyIGFhYSA6LSA9PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09DQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotICAgICAgICAgICAgIEZ1bGwgQ2hhdCB2ZXJzaW9uIHNlZSBmcm9tIFdFQjogAmh0dHA6Ly90dXNvdmthLnRvbXNrLnJ1Ag0KOnR1c292a2EudG9tc2sucnUgMzcyIGFhYSA6LSA9PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09PT09DQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgSG9zdGVkIGJ5IFN0YWNrICh3d3cuc3RhY2sucnUpDQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotIC09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tDQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotIE5ldyBjb21tYW5kOiACL0VZRSA8b24vb2ZmPgIgLSB0dXJuIG9uL29mZiBhZG1pbiBleWUuDQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotIE5ldyBjb21tYW5kOiACL01PREUgI2NoYW5uZWwgK0JJIDxuaWNrbmFtZT4CIC0gSVAgYmFuDQo6dHVzb3ZrYS50b21zay5ydSAzNzIgYWFhIDotIE5ldyBjb21tYW5kOiACL01PREUgI2NoYW5uZWwgK0JOIDxuaWNrbmFtZT4CIC0gTmljayBiYW4NCjp0dXNvdmthLnRvbXNrLnJ1IDM3MiBhYWEgOi0gTmV3IGNvbW1hbmQ6IAIvTU9ERSAjY2hhbm5lbCArQkwgPG5pY2tuYW1lPgIgLSBMb2dpbiBiYW4NCjp0dXNvdmthLnRvbXNrLnJ1IDM3MiBhYWEgOi0gLT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS09LT0tPS0NCg==";
__irc_server_config::__irc_server_config()
{
    msg001="Welcome to the Internet Relay Chat Network,";
    msg002="Your host is tusovka.tomsk.ru running version:";
    msg003="This server has created";
    msg004="-=-=-=-=-=-=-=-=-=-=-=-=tusovka.tomsk.ru-=-=-=-=-=-=-=-=-=-=-==-==-=-=-\r";
    msg254="channels formed.";
    msg255="I have %d users, %d online users, %d using WEB interface and %d using IRC, 0 services on 1 server.";
    msg375="- tusovka.tomsk.ru Message of the Day -";
    host="tusovka.tomsk.ru";
    location="Tomsk, Russia";
    motd=Base64Decode(__motd);

}
void __irc_server_config::load_config(const string&pn)
{
    MUTEX_INSPECTOR;
    map<string,string>	m=load_m_file(pn);

    set_cfg_i(m,"ping_kill_timeout", pn,ping_kill_timeout,200);
    set_cfg_i(m,"ping_time", pn,ping_time,40);
    set_cfg_i(m,"login_timeout", pn,login_timeout,20);

    set_cfg_str(m,"msg001", pn,msg001,"Welcome to the Internet Relay Chat Network,");
    set_cfg_str(m,"msg002", pn,msg002,"Your host is tusovka.tomsk.ru running version:");
    set_cfg_str(m,"msg003", pn,msg003,"This server has created");
    set_cfg_str(m,"msg004", pn,msg004,"-=-=-=-=-=-=-=-=-=-=-=-=tusovka.tomsk.ru-=-=-=-=-=-=-=-=-=-=-==-==-=-=-\r");
    set_cfg_str(m,"msg254", pn,msg254,"channels formed.");
    set_cfg_str(m,"msg255", pn,msg255,"I have %d users.");
    set_cfg_str(m,"msg375", pn,msg375,"- tusovka.tomsk.ru Message of the Day -");
    set_cfg_str(m,"host", pn,host,"tusovka.tomsk.ru");
    set_cfg_str(m,"location", pn,location,"Tomsk, Russia");
    set_cfg_str(m,"motd", pn,motd,Base64Decode(__motd));
    print_rest_cfg(m,pn);
}


string irc_nick$host(const string &n)
{

    return n+"@"+str_nick(n)+"."+irc_server_config.host.get();
}
