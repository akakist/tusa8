
#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <sys/stat.h>
#include <list>
#include "sid_t.h"
#include "mutex_inspector.h"
#include "web_server.h"
#include "des_sid.h"
#include "str_lib.h"
#include "web_connection.h"
#include "mutex_inspector.h"
#include "file_set.h"
#include "web_tools.h"
#include "web_server_data.h"


#include "st_stuff.h"
#include "server_config.h"

#include "thread_descr.h"

class www_cache
{
    _mutex Mutex;
    map<string,pair<string,string> > container;
public:
    void add(const string& key, const pair<string,string> & val)
    {
        M_LOCK(Mutex);
        container[key]=val;
    }
    bool get(const string &key,pair<string,string> & val)// true if OK
    {
        M_LOCK(Mutex);
        if (container.count(key)==0) return false;
        val=container[key];
        return true;
    }
    void clear()
    {
        M_LOCK(Mutex);
        container.clear();
    }
};
www_cache ___www_cache;
void ___www_cache_clear()
{
    ___www_cache.clear();
}

string replace_utf8_cp1251(const string &m);
std::string __gets(const PTR_getter <c_socket> &__S, const string &delim);

void WRITES__(const string &S)
{
    string s=S+"\n";
    static _mutex Mutex("WRITES__MX");
    M_LOCK(Mutex);
    FILE * f=fopen("in.dmp","ab+");
    fwrite(s.data(),s.size(),1,f);
    printf("%s",s.c_str());
    fclose(f);
}
void web_connection::process_tcp()
{   MUTEX_INSPECTOR;

    void *conn_user=NULL;
    string auth_add_string;
    try {
        size_t total_recv=0;
        if (!socket)
        {
            return;
        }

        max_recv = 1024;
        {
            MUTEX_INSPECTOR;
            string fl=__gets(socket,"\r\n");
            total_recv+=fl.size();
            size_t pom = fl.find(" ", 0);
            if (pom == -1) {
                return;
            }


            METHOD = strupper(fl.substr(0, pom));
            url = fl.substr(pom + 1, fl.find(" ", pom + 1) - pom - 1);

            url = repl_percent(url);
            url=replace_utf8_cp1251(url);
            {
                PGLW(c_socket,s,socket);
                s->set_name("chatserver "+url);
            }
            if (url.find("..", 0) != -1) {
                return;
            }
            string New_str;
            {
                MUTEX_INSPECTOR;
                New_str = __gets(socket,"\r\n");
                total_recv+=New_str.size();
            }
            time_t start_t=time(NULL);
            while (New_str != "") {
                if (time(NULL)-start_t>5) {
                    return;
                }
                size_t pop = New_str.find(":", 0);
                if (pop == -1) {
                    return;
                }
                headers[strupper(New_str.substr(0, pop))] = New_str.substr(pop + 2, New_str.length() - pop - 2);
                {
                    MUTEX_INSPECTOR;
                    New_str = __gets(socket,"\r\n");
                }
                total_recv+=New_str.size();
            }
            if (METHOD != "GET" && METHOD != "POST") {
                return;
            }
            if (headers.count("COOKIE")) {

                vector <string> v = splitTOK("; ", headers["COOKIE"]);
                for (unsigned int i = 0; i < v.size(); i++) {
                    string q = v[i];
                    if (q == " ") {
                        continue;
                    }
                    size_t z;
                    z = q.find("=", 0);
                    if (z == -1) {
                        continue;
                    }
                    in_cookies[q.substr(0, z)] = q.substr(z + 1, q.size() - z - 1);
                }
            }

            original_url=url;
            {
                size_t qp = url.find("?", 0);
                if (qp != -1) {
                    original_params=url.substr(qp + 1, url.length() - qp - 1);
                    split_params(original_params);
                    url = url.substr(0, qp);
                }
            }

            if (METHOD == "POST") {

                if (headers["CONTENT-TYPE"].find("multipart/form", 0) == -1) {
                    //std method
                    int maxpost = server_ref->max_post;
                    max_recv = total_recv + maxpost;
                    int clen = atoi(headers["CONTENT-LENGTH"]);
                    if (clen <= 0 || clen > maxpost) {
                        return;
                    }
                    st_malloc b(clen + 10);
                    size_t rdlen=0;
                    time_t start_t=time(NULL);
                    while (1)
                    {
                        if (time(NULL)-start_t>10) {
                            PGLR(c_socket,s,socket);
                            throw cError("break loop"+_DMI());
                            return;
                        }
                        size_t res=0;
                        res=S_recv(socket,(char *)b.buf+rdlen, clen-rdlen,0);
                        if (res==-1) {
                            return;
                        }

                        rdlen+=res;
                        if (rdlen==clen) break;
                    }


                    string pd((char *)b.buf, clen);
                    split_params(pd);
                    POST_CONTENT=pd;
                }
                else {

                    //Multipart form
                    max_recv = total_recv + server_ref->max_multipart;

                    const string &t= headers["CONTENT-TYPE"];
                    size_t pz = t.find("boundary=", 0);
                    if (pz == -1) {

                        return;
                    }
                    string bound = "--" + t.substr(pz + 9, t.length() - pz - 9);
                    string ebound = bound + "--";
                    string sbuf;
                    string n;
                    bool is_data = false;
                    bool nfirst = false;
                    string data;


                    {
                        MUTEX_INSPECTOR;


                        sbuf = __gets(socket,ebound);

                        time_t start_t=time(NULL);
                        while (sbuf.size())
                        {
                            if (time(NULL)-start_t>5) {
                                PGLR(c_socket,s,socket);
                                throw cError("break loop"+_DMI());
                                return;
                            }

                            string s;
                            size_t pos=sbuf.find(bound);
                            if (pos==-1)
                            {
                                s=sbuf;
                                sbuf="";
                            }
                            else
                            {
                                s=sbuf.substr(0,pos);
                                sbuf=sbuf.substr(pos+bound.size(),sbuf.size()-pos-bound.size());
                            }


                            if (s.size()>1)
                            {
                                if (s[0]=='\r'&&s[1]=='\n') s=s.substr(2,s.size()-2);
                            }

                            if (s.size())
                            {
                                map<string,string> lparams;
                                string content;
                                time_t start_t=time(NULL);
                                while (1)
                                {
                                    if (time(NULL)-start_t>5) {
                                        PGLR(c_socket,s,socket);
                                        throw cError("break loop"+_DMI());
                                        return;
                                    }


                                    string sloc;
                                    size_t pp=s.find("\r\n");
                                    if (pp==-1) {
                                        logErr2("bad multipart %s %d",__FILE__,__LINE__);
                                        return;
                                    }
                                    sloc=s.substr(0,pp);
                                    s=s.substr(pp+2,s.size()-pp-2);
                                    if (!sloc.size()) {
                                        content=s.substr(0,s.size()-2);
                                        break;
                                    }
                                    size_t pz=sloc.find(": ");
                                    if (pz==-1) {
                                        logErr2("bad multipart %s %d",__FILE__,__LINE__);
                                        return;
                                    }
                                    string key=sloc.substr(0,pz);
                                    string val=sloc.substr(pz+2,sloc.size()-pz-2);
                                    lparams[key]=val;
                                }

                                if (lparams.count("Content-Disposition"))
                                {
                                    string cd=lparams["Content-Disposition"];
                                    vector<string> flds;
                                    time_t start_t=time(NULL);
                                    while (cd.size())
                                    {
                                        if (time(NULL)-start_t>5) {
                                            PGLR(c_socket,s,socket);
                                            throw cError("break loop"+_DMI());
                                            return;
                                        }
                                        size_t ps=cd.find("; ");
                                        if (ps==-1)
                                        {
                                            flds.push_back(cd);
                                            cd="";
                                        }
                                        else
                                        {
                                            flds.push_back(cd.substr(0,ps));
                                            cd=cd.substr(ps+2,cd.size()-2);
                                        }
                                    }
                                    string name;
                                    string filename;
                                    for (unsigned i=0; i<flds.size(); i++)
                                    {
                                        size_t pzz=flds[i].find("=");
                                        if (pzz!=-1)
                                        {
                                            string key=flds[i].substr(0,pzz);
                                            string val=flds[i].substr(pzz+1,flds[i].size()-1);
                                            if (val.size()>2)
                                            {
                                                if (val[0]=='\"' && val[val.size()-1]=='\"') val=val.substr(1,val.size()-2);
                                            }
                                            if (val=="\"\"") val="";
                                            if (key=="name") name=val;
                                            if (key=="filename") filename=val;
                                        }
                                    }
                                    params[name]=content;
                                    if (filename.size())
                                        params[name+"_FILENAME"]=filename;
                                    for (map<string,string>::const_iterator i=lparams.begin(); i!=lparams.end(); i++)
                                    {
                                        string q = strupper(i->first);
                                        string k=name + "_" + q;
                                        params[k] = i->second;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        deque<string> vu=splitTOKd("/",url);
        string or_url=url;
        if (vu.size())
        {

            if (vu[0]=="cna" )
            {
                url=url.substr(4,url.size()-4);
            }
            else if (vu[0]=="ca")
            {

                url=url.substr(3,url.size()-3);
            }
        }


        set<string> du=server_ref->doc_urls.get_container();
        for (set<string>::const_iterator i=du.begin(); i!=du.end(); i++)
        {

            if (url.substr(0, i->size()) == *i) {
                {
                    pair<string,string> fbody;
                    bool ok=true;
                    if (!___www_cache.get(url,fbody))
                    {
                        string fn=server_ref->document_root.get() + url;
                        string ext = get_file_ext(fn);

                        bool success;
                        fbody=get_other_from_disk_ext(this,fn,true,ext,success);
                        if (!success) ok=false;
                        ___www_cache.add(url,fbody);

                    }
                    if (ok)
                    {

                        string out;
                        out = make_http_header();
                        out += "Content-length: " + std::to_string(fbody.second.size()) + "\r\n";
                        out += "Content-type: "+fbody.first+"\r\n\r\n";
                        out+=fbody.second;
                        write(out);
                        return;
                    }
                }
            }

        }
        ///////////////
        string outtt;
        peer_ipaddress=tcp_peer_ipaddress;
        if (html_out.size()==0)
        {

            process_web();	//PROCESS !!!!

        }
        bool connected;
        {

            PGLW(c_socket, psock,socket);
            connected=psock->connected();
        }
        if (connected && allow_build_response) {

            outtt = build_html_response();
        }
        if (outtt.size() && allow_build_response) {

            write(outtt);

        }
        return;
    }
    catch (socketError e) {
#ifdef DEBUG
        if (e.error_string.find("reset by") == -1 && e.error_string.find("timeout") == -1 && e.error_string.find("send Broke") == -1)
            logErr("--socketError: wc_ptcp() - %s [%s]", e.error_string.c_str(), peer_ipaddress.c_str());
#endif
    }
    catch (cError e) {
#ifdef DEBUG
        if (e.error_string.find("reset by") == -1 && e.error_string.find("timeout") == -1 && e.error_string.find("send Broke") == -1)
            logErr("--cError: wc_ptcp() - %s [%s]", e.error_string.c_str(), peer_ipaddress.c_str());
#endif
    }
    catch (...) {
#ifdef DEBUG
        logErr("--Error: wc_ptcp() - unknown error [%s]", peer_ipaddress.c_str());
#endif
    }


}
