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
#include "wrappers.h"

void st_FILE_cpp() {}
int st_FILE::readsome(void *s2, int n)
{
    int res;
    {
        res = my_fread(s2, 1, n, (FILE *) f);
    }
    return res;
}
int st_FILE::writesome(const void *s2, int n) const
{
    int res;
    {
        res = my_fwrite(s2, 1, n, (FILE *) f);
    }

    return res;
}
char *st_FILE::gets(char *s, int max)
{
    char *ss;
    {
        ss = fgets(s, max, (FILE *) f);
    }
    if (ss == NULL) {
        string _s=(string)"fgets failed with "+ strerror(errno) + " " +pathname+_DMI();
        throw cError(_s);
    }
    return ss;
}
char *st_FILE::getssome(char *s, int max)
{
    char *ss;
    {
        ss= fgets(s, max, (FILE *) f);
    }
    return ss;
}


st_FILE::st_FILE(const string & pn, const string & mode)
{
    pathname = pn;
    {
        f = fopen(to_mac_enc(pathname).c_str(), mode.c_str());
    }
    DUMP("fopen");
    if (f == NULL) {
        string _s="fopen failed with " + pathname + " " + strerror(errno)+_DMI();
        throw cError(_s);
    }
}

st_FILE::~st_FILE()
{
    if (f) {
        fclose((FILE *) f);
    }
    DUMP("fclose");
}

void st_FILE::seek(long offset)
{
    fseek((FILE*) f,offset,SEEK_SET);
}



