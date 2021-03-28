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
#include "oscar_buffer.h"
string as_string(const hstring& h,  web_connection * cc);

m_var<time_t> start_date(time(NULL));
m_var<string> start_date_string(CTIME(time(NULL)));
web_connection::~web_connection()
{

}
web_connection::web_connection(const web_connection* s): tcp_server_connection(s),
    params(s->params),
    headers(s->headers),
    original_params(s->original_params),
    v_params(s->v_params),
    process_v_params(s->process_v_params),
    server_ref(s->server_ref),
    url(s->url),
    html_out(s->html_out),
    http_header_out(s->http_header_out),
    http_code(s->http_code),
    in_cookies(s->in_cookies),
    out_cookies(s->out_cookies),
    html_charset(s->html_charset),
    html_content_type(s->html_content_type),
    allow_build_response(s->allow_build_response),
    peer_ipaddress(s->peer_ipaddress)

{

}


web_connection::web_connection(const web_server * ws,const PTR_getter<c_socket> &s)
    :tcp_server_connection(s)
{


    process_v_params = true;
    server_ref = (web_server *)ws;
    http_code = 200;
    html_content_type = "text/html";
    allow_build_response = true;
}
string web_connection::rvfiles(map<string,string> &m, const string&s)
{


    return RVfiles(m,s.c_str());
}
string web_connection::rvfiles(map<string,string> &m, const char* s)
{

    return RVfiles(m,s);
}

string web_connection::rvfiles(const string & s)
{
    map<string,string>m;
    return rvfiles(m, s);
}
string web_connection::rvfiles(const char* s)
{
    map<string,string>m;
    return rvfiles(m, s);
}

/*-1 error*/ static int send_other_from_mem(web_connection * cc, const string & fn)
{

    string filebody;
    string out;
    {
        size_t res = load_1_file(filebody, fn);
        if (res == -1) {
            return -1;
        }
    }
    out = make_http_header();
    string ext = get_file_ext(fn);

    string mt=cc->server_ref->mime_types.get(ext);
    if(mt=="")mt="text/plain";
    out+="Last-Modified: "+start_date_string.get();
    out += "Content-length: " + std::to_string((int)filebody.size()) + "\r\n";
    out += "Content-type: " + mt + "\r\n\r\n";

    if(ext=="html"||ext=="htm") {
        map<string,string>m;

        out+= replace_vals(m, filebody);
    }
    else
    {
        out += filebody;
    }
    try {
        cc->write(out);
    } catch(...) {}
    return 0;
}

/*-1 error*/ int send_other_from_disk(web_connection * cc, const string & fn,bool use_rv)
{
    struct stat st;
    {
        if (stat(fn.c_str(), &st) == -1) {
            return -1;
        }

    }
    string ext = get_file_ext(fn);
    if((ext=="html"||ext=="htm") && use_rv)
    {
        string filebody;
        string out;
        size_t res = load_1_file(filebody, fn);
        if (res == -1) {
            return -1;
        }
        map<string,string>m;

        filebody=replace_vals(m, filebody);
        out = make_http_header();
        out += "Content-length: " + std::to_string((int)filebody.size()) + "\r\n";
        out += "Content-type: text/html\r\n\r\n";
        out+=filebody;
        try {
            cc->write(out);
        } catch(...) {}
        return 0;
    }


    unsigned int startb = 0;
    unsigned int endb = st.st_size;
    unsigned int max_per_read = 1024*1024;
    bool partial = false;
    if (cc->headers["RANGE"] != "") {
        string s = cc->headers["RANGE"];
        size_t n = s.find("=", 0);
        if (n != -1) {
            vector <string> v = splitTOK("-=", s.substr(n, s.size() - n));
            if (v.size() == 1) {
                startb = atoi(v[0]);
                partial = true;
            }
            if (v.size() == 2) {
                partial = true;
                int bb = atoi(v[1]);
                if (bb != 0)
                    endb = bb;
            }
        }
    }
    if (startb > st.st_size || startb < 0)
        startb = 0;
    if (endb > st.st_size || endb < 0)
        endb = st.st_size;
    if (endb < startb) {
        endb = st.st_size;
        startb = 0;
    }
    string out;
    map<string,string>m;
    if (partial) {
        m["Accept-Ranges"] = "bytes";
        m["Content-Range"] = "bytes " + std::to_string(startb) + "-" + std::to_string(endb) + "/" + std::to_string(st.st_size);
        out = make_http_header(206, m);
    }
    else {
        out = make_http_header();
    }
    out += "Content-length: " + std::to_string(endb - startb) + "\r\n";
    string mt=cc->server_ref->mime_types.get(ext);
    if(mt=="")mt="text/plain";
    if(ext!="html") out+="Last-Modified: Sat, 24 Jul 2000 06:00:00 GMT\r\n";
    out += "Content-type: " + cc->server_ref->mime_types.get(ext) + "\r\n\r\n";
    {
        try {
            cc->write(out);
        } catch(...) {}

        st_FILE f(fn, "rb");
        f.seek(startb);
        st_malloc buf(max_per_read+10);
        while (startb < endb) {
            int sz = max_per_read;
            if (startb + sz > endb) sz = endb - startb;
            f.read(buf.buf, sz);
            startb += sz;
            {
            }
            try {
                cc->write(string((char*)buf.buf, sz));
            } catch(...) {
                logErr2("catched here");
            }
        }
    }
    return 0;
}
pair<string,string> get_other_from_disk_ext(web_connection * cc, const string & fn,bool /*use_rv*/, const string& exten, bool & success)
{

    if (cc->headers["RANGE"] != "") throw cError("get_other_from_disk_ext: RANGE");

    struct stat st;
    {
        if (stat(fn.c_str(), &st) == -1) {
            success=false;
            return make_pair("","");
        }

    }

    if((exten=="html"||exten=="htm") )
    {
        string filebody;
        size_t res = load_1_file(filebody, fn);
        if (res == -1) {
            success=false;
            return make_pair("","");
        }
        map<string,string>m;

        filebody=replace_vals(m, filebody);
        success=true;
        return pair<string,string>(make_pair("text/html",filebody));
    }

    string mt=cc->server_ref->mime_types.get(exten);

    string filebody;
    size_t res = load_1_file(filebody, fn);
    if (res == -1) {
        success=false;
        return make_pair("","");
    }
    success=true;
    return make_pair(mt,filebody);
}
/*-1 error*/ int send_other_from_disk_ext(web_connection * cc, const string & fn,bool use_rv, const string& exten)
{
    MUTEX_INSPECTOR;
    struct stat st;
    {
        if (stat(fn.c_str(), &st) == -1) {
            return -1;
        }

    }
    if((exten=="html"||exten=="htm") && use_rv)
    {
        string filebody;
        string out;
        size_t res = load_1_file(filebody, fn);
        if (res == -1) {
            return -1;
        }
        map<string,string>m;

        filebody=replace_vals(m, filebody);
        out = make_http_header();
        out += "Content-length: " + std::to_string(filebody.size()) + "\r\n";
        out += "Content-type: text/html\r\n\r\n";
        out+=filebody;
        try {
            cc->write(out);
        } catch(...) {}
        return 0;
    }


    unsigned int startb = 0;
    unsigned int endb = st.st_size;
    unsigned int max_per_read = 500000;
    bool partial = false;
    if (cc->headers["RANGE"] != "") {

        string s = cc->headers["RANGE"];
        size_t n = s.find("=", 0);
        if (n != -1) {
            vector <string> v = splitTOK("-=", s.substr(n, s.size() - n));
            if (v.size() == 1) {
                startb = atoi(v[0]);
                partial = true;
            }
            if (v.size() == 2) {
                partial = true;
                int bb = atoi(v[1]);
                if (bb != 0)
                    endb = bb;
            }
        }
    }
    if (startb > st.st_size || startb < 0)
        startb = 0;
    if (endb > st.st_size || endb < 0)
        endb = st.st_size;
    if (endb < startb) {
        endb = st.st_size;
        startb = 0;
    }
    string out;
    map<string,string>m;
    if (partial) {
        m["Accept-Ranges"] = "bytes";
        m["Content-Range"] = "bytes " + std::to_string(startb) + "-" + std::to_string(endb) + "/" + std::to_string(st.st_size);
        out = make_http_header(206, m);
    }
    else {
        out = make_http_header();
    }
    out += "Content-length: " + std::to_string(endb - startb) + "\r\n";
    string mt=cc->server_ref->mime_types.get(exten);
    if(mt=="")mt="text/plain";
    if(exten!="html") out+="Last-Modified: Sat, 24 Jul 2000 06:00:00 GMT\r\n";
    out += "Content-type: " + cc->server_ref->mime_types.get(exten) + "\r\n\r\n";
    string sss="Content-type: " + cc->server_ref->mime_types.get(exten) + "\r\n\r\n";
    {
        try {
            cc->write(out);
        } catch(...) {}

        st_FILE f(fn, "rb");
        f.seek(startb);
        st_malloc buf(max_per_read+10);
        while (startb < endb) {
            int sz = max_per_read;
            if (startb + sz > endb) sz = endb - startb;
            f.read(buf.buf, sz);
            startb += sz;
            try {
                cc->write(string((char*)buf.buf, sz));
            } catch(...) {}
        }
    }
    return 0;
}


void web_connection::send_other(const string & fn)
{   MUTEX_INSPECTOR;

    int umf = global_config.use_mem_files.get();
    if (umf == __global_config::UMF_MEMORY_ONLY) {
        if (send_other_from_mem(this, fn) == -1) {
            string S = make_http_header() + replace_vals(RVfiles("404"));
            {
#ifdef DEBUG
                logErr("SendOther: file not exists [%s,%d] \"%s\"  [%s]",__FL__, fn.c_str(),  peer_ipaddress.c_str());
#else
                logErr("File not exists \"%s\"  [%s]", fn.c_str(),  peer_ipaddress.c_str());
#endif
                write(S);
            }
            return;
        }
    }
    else if (umf == __global_config::UMF_DISK_ONLY) {
        if (send_other_from_disk(this, fn, true) == -1) {
            string S = make_http_header() + replace_vals(RVfiles("404"));
            {
#ifdef DEBUG
                logErr("SendOther:send_other_from_disk: [%s,%d] \"%s\"  [%s]",__FL__, fn.c_str(),  peer_ipaddress.c_str());
#else
                logErr("File not exists \"%s\"  [%s]",fn.c_str(),  peer_ipaddress.c_str());
#endif
                write(S);
            }
            return;
        }
    }
    else if (umf == __global_config::UMF_1DISK_2MEMORY) {
        if (send_other_from_disk(this, fn, true) == -1) {
            if (send_other_from_mem(this, fn) == -1) {
                string S = make_http_header() + replace_vals(RVfiles("404"));
#ifdef DEBUG
                logErr("SendOther: [%s,%d] \"%s\"  [%s]", __FL__, fn.c_str(),  peer_ipaddress.c_str());
#else
                logErr("File not exists \"%s\"  [%s]",  fn.c_str(),  peer_ipaddress.c_str());
#endif
                write(S);
                return;
            }
        }
    }
    else if (umf == __global_config::UMF_1MEMORY_2DISK) {
        if (send_other_from_mem(this, fn) == -1) {
            if (send_other_from_disk(this, fn, true) == -1) {
                string S = make_http_header() + replace_vals(RVfiles("404"));
                {
#ifdef DEBUG
                    logErr("SendOther: [%s,%d] \"%s\" [%s]", __FL__,fn.c_str(), peer_ipaddress.c_str());
#else
                    logErr("File not exists \"%s\" [%s]", fn.c_str(), peer_ipaddress.c_str());
#endif
                    write(S);
                }
                return;
            }
        }
    }
}
void web_connection::split_params(const string & s)
{

    vector <string> pr = splitTOK("&", s);
    for (unsigned int i = 0; i < pr.size(); i++) {
        string p = pr[i];
        size_t pp = p.find("=", 0);
        if (pp == -1) {
            continue;
        }
        string val = repl_percent(p.substr(pp + 1, p.length() - pp - 1));
        string key = p.substr(0, pp);
        params[key] = val;

        if (process_v_params)
            v_params[key].push_back(val);
    }
}






string web_connection::build_html_response()
{
    MUTEX_INSPECTOR;
    string ret;
    ret += "HTTP/1.1 " + std::to_string(http_code) + " " + get_name_of_http_code(http_code) + "\r\n";
    if (html_content_type != "") {
        string r = html_content_type;
        if (html_charset.size())
            r += "; charset: " + html_charset;
        http_header_out["Content-Type"] = r;
    }
    if (http_header_out.find("Connection") == http_header_out.end())
        http_header_out["Connection"] = "close";

    http_header_out["Server"] = SERVERNAME.get();
    http_header_out["Content-Length"] = std::to_string(html_out.size());

    for (map<string,string>::const_iterator i = http_header_out.begin(); i != http_header_out.end(); i++) {
        ret += i->first + ": " + i->second + "\r\n";
    }
    if (out_cookies.size()) {
        for (map<string,string>::const_iterator i = out_cookies.begin(); i != out_cookies.end(); i++) {
            string r;
            r += "Set-Cookie: "+i->first;
            r += "=";
            r += i->second;
            r += "; path=/\r\n";
            ret+=r;

        }
    }

    ret += "\r\n";
    ret += as_string(html_out,this);


    return ret;
}


void web_connection::redirect(const string &url)
{
    http_code=302;
    http_header_out["Location"]=url;
    return;
}
string as_string(const hstring& h,  web_connection * cc)
{   MUTEX_INSPECTOR;
    string out;
    for(unsigned i=0; i<h.container.size(); i++)
    {
        if(h.container[i].___ptr->type==P_STRING)
        {
            out+=* ((string*)h.container[i].___ptr->data);
        }
        else if(h.container[i].___ptr->type==P_FREP)
        {
            string *ss=(string*) h.container[i].___ptr->data;
            out+=global_config.files_get(*ss);
        }
        else if(h.container[i].___ptr->type==P_PAIR_S_HS)
        {
            pair<map<string,hstring> ,string> *p=(pair<map<string,hstring> ,string> *)h.container[i].___ptr->data;
            map<string,string>m;
            for(map<string,hstring>::const_iterator K=p->first.begin(); K!=p->first.end(); K++)
            {
                m[K->first]=as_string(K->second,cc);
            }
            out+=cc->rvfiles(m,p->second);
        }
        else logErr2("invaklid deque element %d (%s %d)",h.container[i].___ptr->type,__FL__);
    }
    return out;
}
