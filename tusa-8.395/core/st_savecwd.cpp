#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/stat.h>
#endif
#include "st_stuff.h"
#include "str_lib.h"
#include "errs.h"
#include <errno.h>
#include <errno.h>
#include "_mutex.h"
#include "copy_file.h"
#include "mutex_inspector.h"
#ifdef WIN32
#include <direct.h>
#endif
void st_save_cwd_cpp() {}
st_save_cwd::st_save_cwd()
{
    char s[300];
    {
        getcwd(s,299);
    }
    cwd=s;
}
st_save_cwd::~st_save_cwd()
{
    int res;
    {
        res=chdir(cwd.c_str());
    }


    if(res)
        logErr("chdir error on: %s",cwd.c_str());
}
