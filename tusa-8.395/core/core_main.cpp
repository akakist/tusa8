#include "stdafx.h"
#include "server_config.h"
#include "file_set.h"
#include "errs.h"
#include "str_lib.h"
extern m_var<string> __APPNAME;

int app_main(int argc,char *argv[]);
#ifndef DEBUG
extern "C" {
    void	__apache__main();
    void	__perl__main();
}
#endif
extern "C"
int tusa_main(int argc, char *argv[])
{

#ifdef WIN32
    WORD wVersionRequested;

    WSADATA wsaData;

    int err;


    wVersionRequested = MAKEWORD(2, 2);


    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0) {

        logErr2("WSAStartup failed\n");

        return 0;

    }
#endif
    global_config.save_cmd_args(argc, argv);
#ifdef __WITH_MEM_FILES
    init_mem_files_dump();

#endif

    string an=argv[0];
    vector<string> v=splitTOK("/",an);
    if(v.size())
    {
        __APPNAME=v[v.size()-1];

    }

    app_main(argc, argv);
    return 0;
}
