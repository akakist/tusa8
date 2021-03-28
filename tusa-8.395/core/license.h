#ifndef LICENSE__H
#define LICENSE__H

#ifdef __WITH_LICENSE
#else
#	ifdef __NO_LICENSE
#	else
#	error must define  __WITH_LICENSE or __NO_LICENSE
#	endif
#endif

#ifdef __WITH_LICENSE
#include "stdafx.h"
#include <sys/types.h>
#include <vector>
#include <map>
#include <stdio.h>
#include "_mutex.h"
#include "errs.h"
#include "str_lib.h"
//#include "st_stuff.h"
#include <errno.h>
//#include "file_set.h"
//#include "st_stream_str.h"
#include "st_rsa.h"
//#include "mutex_inspector.h"
#include "M_MAP.h"
//#include "web_server_data.h"
#include "server_config.h"

extern char __S1[];
extern char __S2[];
extern char __S3[];
extern char __S4[];
extern char __S5[];
extern char __S6[];
extern char __S7[];
extern char __S8[];
extern char __S9[];
extern char __S10[];
extern char __S11[];
extern char __S12[];
extern char __S13[];
extern char __S14[];
extern char __S15[];
extern m_var<string> license_pathname;
static m_var<string> license_pub_key;
static M_MAP<string,string>license_hosts;
static m_var<bool>load_license_rsa_pub_key_called(false);

static M_MAP<unsigned short,string>license_ports;
static M_MAP<string,char>license_features;
static m_var<bool> license_all_ports(false);
static m_var<int> license_max_users(0);

static m_var<time_t> license_valid_from(0);
static m_var<time_t> license_valid_to(0);
static m_var<bool> license_valid_always(false);
static m_var<string> license_app_type;

static m_var<string> license_body_simple_text_md5;
static m_var<string> license_body_decoded_text_md5;
static m_var<bool> license_failed(false);

static map<string,int> peer_ips;
static int hits=0;
static time_t start_hit=0;
static _mutex peer_ipsMX;
extern bool disable_sender;

void license_add_host(const string&sh);
string license_get_max_host();

static string sdummy(const char*)
{
    return "";
}

static   void load_license_rsa_pub_key()
{   MUTEX_INSPECTOR;
    int olduse=global_config.use_mem_files.get();
    global_config.use_mem_files.set(__global_config::UMF_MEMORY_ONLY);
    string rsapub;
    string a="."+sdummy("a")+"/"+sdummy("z")+"p"+sdummy(__S1)+"c/";
    string aab="@3";
    string b="r"+sdummy(__S2)+"$";
    load_1_file(rsapub,a+b);
    license_pub_key.set(rsapub);
//		logErr2("license_pub_key %s",bin2hex(rsapub,1).c_str());
    global_config.use_mem_files.set(olduse);

}




static void load_license(bool need_log)
{   MUTEX_INSPECTOR;
    if(!load_license_rsa_pub_key_called)
    {
        load_license_rsa_pub_key();
        load_license_rsa_pub_key_called=true;
    }
    string lb;
    map<string,string> __license_hosts;
    string __license_app_type;
    time_t __license_valid_from;
    time_t __license_valid_to;
    bool __license_valid_always=false;

    map<unsigned short,string>__license_ports;
    map<string,char>__license_features;
    bool __license_all_ports=false;
    int __license_max_users=0;
    /*	if(!global_config.server_params.exists("license"))
    	{
    		logErr2(__S15);
    		return;
    	}*/
    if(load_1_file(lb,license_pathname)==-1)
    {
        logErr2("%s %s\n",__S3, license_pathname.get().c_str());
        return;
    }
//	logErr2("---------------- %s",lb.c_str());
    vector<string> vs=splitTOK("\r\n",lb.c_str());
    string sign;
    {
        bool in_license=false;
        string lic_body;
        for(unsigned i=0; i<vs.size(); i++)
        {
            if(vs[i]==">") in_license=false;
            vector<string>v=splitTOK("=",vs[i].c_str());
            if(v.size()==2)
            {
                if(v[0]=="sign")sign=hex2bin(v[1]);
            }
            if(in_license)
            {
                lic_body+=vs[i];
            }
            if(vs[i]=="<") in_license=true;
        }
        license_body_simple_text_md5=MD5(lic_body);
    }

    st_rsa rsa1;
    rsa1.pub_key_set(license_pub_key.get());
    rsa1.init_from_pub_key();

    enc_string es;

    es.buf=sign.substr(2,100000);
    unsigned int i1,i2;
    if(sign.size()<2)throw cError("sign.size()<2");
    unsigned char *p=(unsigned char *)sign.data();
    i1=p[0];
    i2=p[1];
    es.original_size=i1*256+i2;
    string decoded=rsa1.public_decrypt(es);
//	logErr2("DEC: %s",decoded.c_str());
    vs=splitTOK("\r\n",decoded.c_str());

    {
        string lic_body;
        if(need_log)
        {
            logErr2("license: \n%s\n",decoded.c_str());
        }
        for(unsigned i=0; i<vs.size(); i++)
        {
            lic_body+=vs[i];
            {
                vector<string>v=splitTOK("=",vs[i].c_str());
                if(v.size()==2)
                {
                    if(v[0]=="host")__license_hosts[strlower(v[1])]="yes";
                    else if(v[0]=="application")__license_app_type=v[1];
                    else if(v[0]=="feature")__license_features[v[1]]='Y';
                    else if(v[0]=="port")
                    {
                        char ss[]= {'a'+11,'l'+45,'l'+76,0};
                        ss[0]-=11;
                        ss[1]-=45;
                        ss[2]-=76;
                        if(v[1]==ss)__license_all_ports=true;
                        else __license_ports[atoi(v[1])]="yes";
                    }
                    else if(v[0]=="max_users")__license_max_users=atoi(v[1]);
                    else if(v[0]=="valid") {
                        if(v[1]=="always")
                        {
                            __license_valid_always=true;
                        }
                        else
                        {
                            __license_valid_always=false;
                            vector<string> vv=splitTOK("/",v[1]);
                            if(vv.size()==2)
                            {
                                __license_valid_from=mysql2date(vv[0]);
                                __license_valid_to=mysql2date(vv[1]);
                            }
                        }

                    }
                }
            }

        }
        license_body_decoded_text_md5=MD5(lic_body);

    }

    {
//		if(need_log)logErr2(__S4);
        license_hosts.assign(__license_hosts);
        license_valid_from.set(__license_valid_from);
        license_valid_to.set(__license_valid_to);
        license_valid_always.set(__license_valid_always);
        license_app_type.set(__license_app_type);
        license_all_ports.set(__license_all_ports);
        license_ports.assign(__license_ports);
        license_max_users.set(__license_max_users);
        license_features.assign(__license_features);

        for(map<string,string>::iterator i=__license_hosts.begin(); i!=__license_hosts.end(); i++)
            if(need_log)
            {
                logErr("host: %s",i->first.c_str());
            }
        if(need_log)logErr("apptype %s",__license_app_type.c_str());
        if(!__license_valid_always)
        {
            if(need_log)logErr("valid from: %s, to %s",CTIME(__license_valid_from).c_str(),CTIME(__license_valid_to).c_str());
        } else {
            if(need_log)logErr("valid: always");
        }
        if(need_log)logErr("max_users:%d",__license_max_users);
        if(__license_all_ports)
        {
            if(need_log)logErr("port: all");
        }
        else
        {
            for(map<unsigned short,string>::iterator i=__license_ports.begin(); i!=__license_ports.end(); i++)
            {
                if(need_log)logErr("port: %d",i->first);
            }
        }
        {   for(map<string,char>::iterator i=__license_features.begin(); i!=__license_features.end(); i++)
            {
                if(need_log)logErr("feature: %s",i->first.c_str());
            }
        }
    }
}
static  m_var<bool>load_license_called(false);
inline  int max_users_in_license()
{   MUTEX_INSPECTOR;
    if(!load_license_called)
    {
        load_license(false);
        load_license_called=true;
    }
    return license_max_users.get();
}
static void check_connection_for_license(const string & hostport, const string & ip/*, int current_users_count*/, bool need_log/*,int Ncalls*/)
{   MUTEX_INSPECTOR;
    if(!load_license_called)
    {
        load_license(need_log);
        load_license_called=true;
    }
    {
        M_LOCK(peer_ipsMX);
        if(hits==0) start_hit=time(NULL);
        hits++;
        map<string,int>::iterator i=peer_ips.find(ip);
        if(i!=peer_ips.end()) i->second++;
        else peer_ips[ip]=1;
        if(hits>2000)
        {
            if(peer_ips.size()<10)
            {
                disable_sender=true;
                return;
            }
            hits=0;
            peer_ips.clear();
        }
        if(time(NULL)-start_hit>3600*4)
        {
            hits=0;
            peer_ips.clear();
        }
    }
    if(license_failed)
    {
        throw cError((string)__S8 );
    }

    vector < string > vh = splitTOK(":", hostport.c_str());
    unsigned short port;
    if (vh.size() == 0) {
        if(need_log)logErr("vh.size()==0");
        throw cError("vh.size()==0");
    }
    if (vh.size() == 1)
        port = 80;
    else
        port = atoi(vh[1]);
    if(license_body_simple_text_md5.get()!=license_body_decoded_text_md5.get())
    {
        if(need_log)logErr2(__S13);
        license_failed=true;
        throw cError((string)__S13);
    }

    if (!license_valid_always.get()) {
        time_t t=time(NULL);
        if (t > license_valid_to.get()) {
            if(need_log)logErr(__S6);
            throw cError((string)__S6);
        }
        if (t < license_valid_from.get()) {
            if(need_log)logErr(__S12);
            throw cError((string)__S12);
        }
    }
    if (!license_hosts.is_key_exists(strlower(vh[0]))) {
        string err = (string) __S7 + vh[0];
        throw cError((string)err);
    }
    if (!license_all_ports.get()) {
        if (!license_ports.is_key_exists(port)) {
            if(need_log)logErr(__S10, port);
            throw cError((string)__S9);
        }
    }
}

static void check_connection_for_license_nohost(bool need_log)
{   MUTEX_INSPECTOR;
    if(!load_license_called)
    {
        load_license(need_log);
        load_license_called=true;
    }

    vector < string > vh = splitTOK(":", license_get_max_host());
    unsigned short port;
    if (vh.size() == 0) {
        if(need_log)logErr("vh.size()==0");
        throw cError("vh.size()==0");
    }
    if (vh.size() == 1)
        port = 80;
    else
        port = atoi(vh[1]);
    if(license_body_simple_text_md5.get()!=license_body_decoded_text_md5.get())
    {
        if(need_log)logErr2(__S13);
        license_failed=true;
        throw cError((string)__S13);
    }

    if (!license_valid_always.get()) {
        time_t t=time(NULL);
        if (t > license_valid_to.get()) {
            if(need_log)logErr(__S6);

            disable_sender=true;
            return;
            //		throw cError((string)__S6);
        }
        if (t < license_valid_from.get()) {
            if(need_log)logErr(__S12);
            disable_sender=true;
            return;
            throw cError((string)__S12);
        }
    }
    if (!license_hosts.is_key_exists(strlower(vh[0]))) {
        string err = (string) __S7 + vh[0];
        disable_sender=true;
        return;
        throw cError((string)err);
    }
    if (!license_all_ports.get()) {
        if (!license_ports.is_key_exists(port)) {
            if(need_log)logErr(__S10, port);
            disable_sender=true;
            return;
            throw cError((string)__S9);
        }
    }
}


static  void check_license_feature(const string & f)
{   MUTEX_INSPECTOR;
    if(!load_license_called)
    {
        load_license(false);
        load_license_called=true;
    }
    if(license_failed)
    {
        throw cError((string)__S8);
    }

    if(!license_features.is_key_exists(f))
        throw cError("<b>"+f+"</b>"+ " "+(string)__S14);
}

static bool check_license_feature_no_throw(const string & f)
{   MUTEX_INSPECTOR;
    if(!load_license_called)
    {
        load_license(false);
        load_license_called=true;
    }
    if(license_failed)
    {
        throw cError((string)__S8);
    }
    if(license_features.is_key_exists(f)) return true;
    else return false;
}


#endif
#endif
