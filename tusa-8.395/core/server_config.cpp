#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <signal.h>
#include <errno.h>
#include "mutex_inspector.h"
#include "str_lib.h"
#include "server_config.h"
#include "wrappers.h"
#include "st_stuff.h"
#include "file_set.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

__global_config global_config;
__global_config::__global_config():use_mem_files(UMF_DISK_ONLY)
{
}
void __global_config::load_config(const string& server_config, bool with_files_conf)
{
    MUTEX_INSPECTOR;

    map<string,string>	m=load_m_file(server_config);
    server_params.read(server_config);

#ifdef WIN32
    set_cfg_str(m,"var_dir_win32", server_config,var_dir,"" SPLITTER "var" SPLITTER "chat");
#else
    set_cfg_str(m,"var_dir", server_config,var_dir,"" SPLITTER "var" SPLITTER "chat");
#endif


#ifdef __WITH_FILES_CONF__
    set_cfg_str(m,"files_dir", server_config,files_dir,"." SPLITTER "www" SPLITTER "files" SPLITTER );
    load_files();

#endif
#ifdef __WITH_LICENSE
    if(server_params.exists("license"))
    {

        license_pathname=server_params["license"];

        load_license(true);
    }
    else
    {
        logErr2("license not exists %s",_DMI().c_str());
    }
#endif


}
size_t load_1_file_from_disk(string & res, const string & fn);
static size_t load_1_file_from_mem(string & res, const string & fn)
{
//-1 err
    if (!global_config.files_dump.exists(fn)) {
        return -1;
    }
    else {
        res = global_config.files_dump.get(fn);
        return res.size();
    }
    return -1;

}
size_t load_1_file(string & res, const string & fn)//-1 err
{
    try {
        int umf=global_config.use_mem_files.get();

        if(umf==__global_config::UMF_MEMORY_ONLY)
        {
            return load_1_file_from_mem(res,fn);
        }
        else if(umf==__global_config::UMF_DISK_ONLY)
        {
            return load_1_file_from_disk(res,fn);
        }
        else if(umf==__global_config::UMF_1MEMORY_2DISK)
        {
            size_t r=load_1_file_from_mem(res,fn);
            if(r!=-1)return r;
            return load_1_file_from_disk(res,fn);
        }
        else if(umf==__global_config::UMF_1DISK_2MEMORY)
        {
            size_t r=load_1_file_from_disk(res,fn);
            if(r!=-1)return r;
            return load_1_file_from_mem(res,fn);
        }
    }
    catch(...) {
        return -1;
    }
    return -1;
}
#ifdef __WITH_FILES_CONF__

#endif
#ifdef __WITH_FILES_CONF__
string __global_config::files_get(const string& key)
{
    if(files.exists(key))
    {
        return files[key];
    }
    {
        string body;
        string pn=files_dir.get()+key+".html";
        if (load_1_file(body,pn) == -1) {
            logErr2("--Error: Failed to read file %s (%s)", pn.c_str(), strerror(errno));
            return "";
        }
        files.set(key,body);
        return body;
    }
    logErr2("--Error: Failed to read file %s not defined in files_conf", key.c_str());
    return "";
}
void __global_config::load_files()
{

    files.clear();
    return;
}
#endif
void __global_config::save_cmd_args(int argc, char *argv[])
{
    for(int i=0; i<argc; i++)
    {
        __cmd_args.push_back(argv[i]);
    }
}
void __global_config::scan_opts(params_map& pset,M_LIST<string>&li)
{
    list<string> l=li.get_container();
    for(list<string> ::const_iterator i=l.begin(); i!=l.end(); i++)
    {
        size_t pz = i->find("=", 0);
        if (pz == -1) {
            continue;
        }
        pset.set(i->substr(0, pz), i->substr(pz + 1, i->length() - pz - 1));

    }
}
