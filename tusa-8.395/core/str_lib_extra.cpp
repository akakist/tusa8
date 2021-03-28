#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <fstream>
#include <iostream>

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
#include <map>
#include <stdio.h>
#include "_mutex.h"
#include "errs.h"
#include "str_lib.h"
#include "st_stuff.h"
#include <errno.h>
#ifdef WIN32
#include <regex.h>
#else
#include <regex.h>
#endif
#ifndef WIN32
#endif
#include <sys/types.h>
#ifndef WIN32
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <vector>
#include <list>
#include "str_lib.h"
#ifndef WIN32
#include <sys/errno.h>
#include <zlib.h>
#endif
#include "errs.h"
#include <cerrno>
#include "str_lib.h"
#include <openssl/md5.h>
#include "st_stuff.h"
#include "mutex_inspector.h"
#include <cstdarg>
#include "st_stream_str.h"
#ifdef WIN32
#define snprintf _snprintf
#endif
int load_1_file(string & res, const string & fn);//-1 err

string MD5(const string & s)
{

    unsigned char p[MD5_DIGEST_LENGTH + 10];
    ::MD5((unsigned char *)s.data(), s.size(), (unsigned char *)p);
    string ret((char *)p, MD5_DIGEST_LENGTH);
    return ret;
}

#ifdef _WIN32
extern "C"
{
    int regcomp (regex_t *preg, const char *pattern, int cflags);
    int regexec(const regex_t *preg, const char *string, size_t nmatch,regmatch_t pmatch[], int eflags);
    size_t regerror(int errcode, const regex_t *preg, char *errbuf,size_t errbuf_size);
    void regfree (regex_t *preg);
};
#endif

void find(vector < rxfind_data > &res, const char *regexp, const char *buffer)
{


    int err;
    regex_t re;
    err = regcomp(&re, regexp, REG_EXTENDED);
    regmatch_t pm[10];
    ::memset(pm, 0, sizeof(regmatch_t));
    if (err) {
        char erbuf[111];
        regerror(err, &re, erbuf, sizeof(erbuf));
        throw cError((string)"regex "+regexp+": "+erbuf);
        return;
    }
    size_t lastpos = 0;
    while (!regexec(&re, buffer + lastpos, 10, pm, 0)) {
        const char *s = buffer + lastpos;
        size_t lpos=lastpos;
        for (int i = 0; i < 10; i++) {
            if (pm[i].rm_so != -1) {
                size_t matchlen = pm[i].rm_eo - pm[i].rm_so;
                string fs(&s[pm[i].rm_so], matchlen);
                rxfind_data fd;
                fd.str = fs;
                fd.so = pm[i].rm_so + lastpos;
                fd.eo = pm[i].rm_eo + lastpos;
                res.push_back(fd);
                lpos=pm[i].rm_eo;
            } else break;
        }
        lastpos += lpos;
    }
    regfree(&re);
}
string replace_vals(const string &src)
{
    map<string,string>m;
    return replace_vals(m,src);
}
string replace_vals(map<string,string> &p, const string &src)
{
    if(!p.size())return src;
    string s;
    map<string,string>::const_iterator it=p.begin();
    s+=it->first;
    it++;
    /*SER: ERROR zdes mozhet byt esli map pustoj*/
    for(; it!=p.end(); it++) {
        s+="|"+(*it).first;
    }
    vector<rxfind_data> r;
    string a;
    ::find(r,s.c_str(),src.c_str());
    int lpz=0;
    for(unsigned int i=0; i<r.size(); i++) {
        a+=src.substr(lpz,r[i].so-lpz);
        a+=p[r[i].str];
        lpz=r[i].eo;
    }
    a+=src.substr(lpz,src.size()-lpz);
    return a;
}


string replace_re_vals(const map<string,string> &p, const string &__src)
{
    if(!p.size())return __src;
    string worksrc=__src;
    for(map<string,string>::const_iterator i=p.begin(); i!=p.end(); i++)
    {
        vector<rxfind_data> r;
        ::find(r,i->first.c_str(),worksrc.c_str());
        string a;
        int lpz=0;
        for(unsigned int j=0; j<r.size(); j++)
        {

            a+=worksrc.substr(lpz,r[j].so-lpz);
            a+=i->second;
            lpz=r[j].eo;
        }
        a+=worksrc.substr(lpz,worksrc.size()-lpz);
        worksrc=a;

    }

    return worksrc;
}
string replace_smiles(const map<string,string> &p, const string &__src)
{
    string out;
    for(unsigned i=0; i<__src.size(); i++)
    {
        if(__src[i]==':')
        {
            int pz=__src.find(" ",i);
            string smile;
            if(pz==-1) smile=__src.substr(i,__src.size()-i);
            else smile=__src.substr(i,pz-i);
            map<string,string>::const_iterator it=p.find(smile);
            if(it!=p.end())
            {
                out+=it->second;
                i+=smile.size()-1;
                continue;
            }

        }
        out+=__src[i];
    }
    return out;
}

string str_replace(const string &s,const string &sr,const string &src)
{
    vector<rxfind_data> r;
    ::find(r,s.c_str(),src.c_str());
    int lpz=0;
    string a;
    for(unsigned int i=0; i<r.size(); i++) {
        int eo=r[i].so-lpz;
        a+=src.substr(lpz,eo);
        a+=sr;
        lpz=r[i].eo;
    }
    int eo=src.size()-lpz;
    a+=src.substr(lpz,eo);
    return a;
}

#ifndef WIN32
string z_compress(const string& s)
{
    out_oscar_buffer o;
    st_malloc out(s.size()*2+12);
    unsigned long outlen=s.size()*2+12;
    int err=compress((unsigned char*)out.buf,&outlen,(unsigned char*)s.data(),s.size());
    if(err==Z_OK)
    {
        out_oscar_buffer o;
        o<<string ((char*)out.buf,(int)outlen);
        return o.as_string();
    }
    else
        return "";
}
string z_expand(const string& s)
{
    oscar_buffer b(s.data(),s.size());
    string body;
    b>>body;
    int len=body.size()*2;
    st_malloc out(len+100);
    unsigned long outlen=len+100;
    int err=uncompress((unsigned char*)out.buf,&outlen,(unsigned char*)body.data(),body.size());
    if(err==Z_OK)
    {
        return string ((char*)out.buf,(int)outlen);
    }
    else
    {
        logErr2("z_expand failed");
    }
    return "";
//XPASS;
}
#endif

string replace_utf8_cp1251(const string &m)
{
    string r;

    for(int i=0; i<m.size(); i++)
    {
        unsigned char c=m[i];

        if(c<0x80)
        {
            r+=c;
            continue;
        }
        if(m.size())
        {
            if(i<m.size()-1)
                i++;
            unsigned char c2=(unsigned char )m[i];
            if(c==0xD0)
            {
                if(c2>=0x90 && c2<=0x9F)
                    r+=0xC0+(c2-0x90);
                if(c2>=0xA0 && c2<=0xAF)
                    r+=0xD0+(c2-0xA0);
                if(c2>=0xB0 && c2<=0xBF)
                    r+=0xE0+(c2-0xB0);
            }
            if(c==0xD1)
            {
                if(c2>=0x80 && c2<=0x8F)
                    r+=0xF0+(c2-0x80);
            }
        }

    }
    return r;
}

