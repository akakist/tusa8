#ifndef CHAT_CONF_G_H11122
#define CHAT_CONF_G_H11122
#include "_mutex.h"
#include "M_MAP.h"
#include "M_SET.h"
#include "m_var.h"
#include "file_set.h"
struct __global_config
{
    m_var<int> use_mem_files;
    enum {
        UMF_DISK_ONLY=0,
        UMF_MEMORY_ONLY=100,
        UMF_1MEMORY_2DISK=101,
        UMF_1DISK_2MEMORY=102
    };


    m_var<string>var_dir;


#ifdef __WITH_LICENSE__
    m_var<string>license;
#endif


    params_map files_dump;/*TOTAL SUBDIR FILES BODIES*/
#ifdef	__WITH_FILES_CONF__
    params_map files;
    //params_map files_conf;
#endif
    params_map server_params;


    M_LIST<string> __cmd_args;/*copy of cmdline parameters*/

    void save_cmd_args(int argc, char *argv[]);

    void scan_opts(params_map& pset,M_LIST<string>&li);
#ifdef __WITH_FILES_CONF__
    void load_files();
    bool files_exists(const string& key);
    string files_get(const string& key);
    m_var<string> files_dir;
#endif




    void load_config( const string& pn, bool with_files_conf);
    __global_config();
};
extern __global_config global_config;


#endif







