
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#else
#include "wrappers.h"
#endif
#include <vector>
#include <map>
#include <stdio.h>
#include "_mutex.h"
#include "errs.h"
#include "str_lib.h"
#include "st_stuff.h"
#include <errno.h>
#include "file_set.h"
#include "st_stream_str.h"
#include "st_rsa.h"
#include "copy_file.h"
#include "wrappers.h"
#include "mutex_inspector.h"
#include "server_config.h"
#include <ctype.h>
void file_set_init_cpp() {}
m_var<bool> files_dump_inited(false);

extern char files_dump_stream[];
extern int files_dump_stream_size;
extern char pub_key[];
extern int pub_key_size;
extern int original_size;

#ifdef __WITH_MEM_FILES
void init_mem_files_dump()
{
    MUTEX_INSPECTOR;
    string s(files_dump_stream,files_dump_stream_size);
    string pkey(pub_key,pub_key_size);
    st_rsa rsa;
    rsa.pub_key_set(pkey);
    rsa.init_from_pub_key();
    enc_string es;
    es.buf=s;
    es.original_size=original_size;
    s=rsa.public_decrypt(es);
    s=z_expand(s);
    map<string,string>v;
    oscar_buffer b(s.data(),s.size());

    b>>v;
    for(map<string,string>::iterator i=v.begin(); i!=v.end(); i++)
    {
        string key=i->first;
        string val=i->second;
        global_config.files_dump.set(key,val);
    }
    files_dump_inited.set(true);
}
#endif
