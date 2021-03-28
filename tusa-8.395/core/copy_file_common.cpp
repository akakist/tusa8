#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef WIN32
#include <sys/TYPES.h>
#endif
#ifndef WIN32
#include <sys/stat.h>
#include <dirent.h>
#endif


#include <errno.h>
#include "str_lib.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#	include <direct.h>
#	include <io.h>
#	include <malloc.h>
#	include <process.h>
#	include <time.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#endif
#include "m_var.h"
#include "M_MAP.h"
#include "errs.h"
#include "st_stuff.h"
#include "_mutex.h"
#include "copy_file.h"
#ifdef macintosh
#include <dirent.h>
#endif
#include "wrappers.h"
#include "mutex_inspector.h"

void copy_file_common_cpp() {}
void check_path_wfn(const string & _pathname)
{
    struct stat st;
    if(!stat(_pathname.c_str(),&st)) return;
    string pathname=_pathname;
#ifdef WIN32
    vector <string> ar=splitTOK("\\/",pathname);;
#else
    vector <string> ar=splitTOK(split_char.c_str(),pathname);
#endif
    vector <string> Newar;
    Newar.clear();
    {   for(int i=0; i<ar.size()-1; i++)
        {
            Newar.push_back(ar[i]);
        }
    }

    string dir=join(split_char,Newar);

    if(is_file_exists(dir)) return;
    string parts;
    if((pathname.substr(0,1)=="/") || (pathname.substr(0,1)=="\\")) parts+=split_char;
    for(unsigned i=0; i<Newar.size(); i++)
    {
        string par=parts+Newar[i];
        parts+=Newar[i]+split_char;
        if((!is_file_exists(parts))&&(!is_file_exists(par)))
        {
            try
            {
                my_mkdir(to_mac_enc(parts));
            }
            catch (...)
            {
                logErr2("mkdir error %s",_pathname.c_str());
            }

        }
    }
}


bool is_file_exists(const string &pathname)
{

    STAT st;
    bool res=false;
    if(my_stat(to_mac_enc(pathname),&st)) {
        res=false;
    }
    else res=true;
    return res;

}

long get_file_length(const string &pathname)
{
    STAT st;
    if(my_stat(pathname,&st)) {
        throw cError((string)"ENOEXISTS "+pathname);
        return -1;
    }
    long length=st.st_size;
    return length;
}

long get_file_length_with_RF(const string &pathname)
{
    long totsize=0;
#ifdef macintosh
    {
        st_macFile mfs;
        mfs.openFS(pathname);
        totsize+=mfs.size();
    }
    {
        st_macFile mrf;
        mrf.openRF(pathname);
        totsize+=mrf.size();

    }
#else
    STAT st;
    if(my_stat(pathname,&st))
    {
        throw cError("ENOEXISTS "+pathname);
    }
    totsize+=st.st_size;
#endif
    return totsize;
}
void rename_file(const string & pn, const string & pnto)
{
#ifndef WIN32
    int r = rename(to_mac_enc(pn).c_str(), to_mac_enc(pnto).c_str());
    if (r)
        throw cError("rename %s %s %s",pn.c_str(),pnto.c_str(),strerror(errno));
    return;
#else
    st_FILE f1(pn,"rb");
    st_FILE f2(pnto,"wb");
    char buf[10000];
    while(1)
    {
        int res=f1.readsome(buf,10000);
        if(res==0)break;
        f2.write(buf,res);
    }
#endif
}



#ifndef WIN32
int _readdir(vector <string> &result, const string & path)
{
    st_save_cwd kaka;
    dirent *de;
    check_path_wfn(path + split_char + "kaka");
    DIR *d = opendir(to_mac_enc(path).c_str());
    if (d == NULL) {
        return errno;
    };
    while (1) {


        de = readdir(d);
        if (de == NULL)
            break;
        string p = path + split_char + de->d_name;
        if (is_file_exists(p)) {
            vector <string> subdir_content;
            subdir_content.clear();
            _readdir(subdir_content, p);
            for (unsigned int i = 0; i < subdir_content.size(); i++) {
                result.push_back(string(de->d_name + split_char + subdir_content[i]));
            }
        }
        else {
            result.push_back(de->d_name);
        }
    }
    closedir(d);
    return 0;
}
#endif

#ifdef WIN32
int _readdir(vector <string> &ar, const string & _path)
{
    st_save_cwd kaka;
    WIN32_FIND_DATA fd;
    string path = _path;
    path += "\\*.*";

    HANDLE h = FindFirstFile(path.c_str(), &fd);
    string p = _path + split_char + fd.cFileName;
    if (is_file_exists(p))
    {
        vector <string> subdir_content;
        subdir_content.clear();
        string p = _path + split_char + fd.cFileName;
        _readdir(subdir_content, p);
        for (unsigned int i = 0; i < subdir_content.size(); i++) {
            ar.push_back(string(fd.cFileName + split_char + subdir_content[i]));
        }

    }
    else
        ar.push_back(fd.cFileName);
    while (FindNextFile(h, &fd)) {
        string p = _path + split_char + fd.cFileName;
        if (is_file_exists(p)) {
            vector <string> subdir_content;
            subdir_content.clear();

            string p = _path + split_char + fd.cFileName;
            _readdir(subdir_content, p);
            for (unsigned int i = 0; i < subdir_content.size(); i++) {
                ar.push_back(string(fd.cFileName + split_char + subdir_content[i]));
            }

        }
        else
            ar.push_back(fd.cFileName);
    }
    FindClose(h);
    return 0;
}
#endif

string GetHostIP(const string &host)
{
    bool is_alph=false;
    for(unsigned int i=0; i<host.size(); i++)
    {
        switch(host[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            break;
        default:
            is_alph=true;
            break;
        }
    }
    if(!is_alph) {
        return host;
    }
    struct hostent *hp;
    hp=gethostbyname(host.c_str());
    if(hp)
    {
        in_addr ad;
        memcpy(&ad,hp->h_addr, hp->h_length);
        return inet_ntoa(ad);
    }
    return host;
}

void mk_path(const string &path)
{

    vector <string> p;
    p=splitTOK(split_char.c_str(),path);
    string pn;
    for(unsigned int i=0; i<p.size(); i++)
    {
        pn+=split_char+p[i];
        struct stat st;
        if(stat(pn.c_str(),&st))
        {
#ifdef WIN32
            if(mkdir(pn.c_str()))
#else
            if(mkdir(pn.c_str(),0755))
#endif
            {
                throw cError("mkdir "+pn);
            }
        }
    }
}

#if defined(WIN32) || defined (macintosh)
#include "copy_file.h"


#endif
