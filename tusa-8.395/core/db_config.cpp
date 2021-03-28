#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <signal.h>
#include <errno.h>
#include "mutex_inspector.h"
#include "db_config.h"
#include "str_lib.h"
__db_config db_config;
void __db_config::load_config(const string&pn)
{   MUTEX_INSPECTOR;
    map<string,string>	m=load_m_file(pn);

    set_cfg_str(m,"dbhost", pn,dbhost,"NULL");
    set_cfg_str(m,"dbuser", pn,dbuser,"NULL");
    set_cfg_str(m,"dbpasswd", pn,dbpasswd,"NULL");
    set_cfg_str(m,"dbname", pn,dbname,"NULL");
    set_cfg_str(m,"dbsock", pn,dbsock,"NULL");
    set_cfg_i(m,"dbport", pn,dbport,0);

    set_cfg_str(m,"dbstart_code", pn,dbstart_code,"SET CHARSET cp1251");


    print_rest_cfg(m,pn);
}
