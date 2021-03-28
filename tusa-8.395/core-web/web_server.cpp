
#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <sys/stat.h>
#include "web_server.h"
#include "web_server_data.h"

#include "mutex_inspector.h"
#include "tcp_server_data.h"
#include "web_connection.h"

#include "file_set.h"
#include <set>
#include "str_lib.h"
#include "tcp_sender.h"
#include "file_set.h"
static char mime[]="YXBwbGljYXRpb24vYWN0aXZlbWVzc2FnZQphcHBsaWNhdGlvbi9hbmRyZXctaW5zZXQJZXoKYXBwbGljYXRpb2"
                   "4vYXBwbGVmaWxlCmFwcGxpY2F0aW9uL2F0b21pY21haWwKYXBwbGljYXRpb24vY3Utc2VlbWUJCWNzbSBjdQphcHBsaWNhdGlvbi9kY2"
                   "EtcmZ0CmFwcGxpY2F0aW9uL2RlYy1keAphcHBsaWNhdGlvbi9leGNlbAkJeGxzCmFwcGxpY2F0aW9uL2dob3N0dmlldwkJCmFwcGxpY2"
                   "F0aW9uL21hYy1iaW5oZXg0MAlocXgKYXBwbGljYXRpb24vbWFjLWNvbXBhY3Rwcm8JY3B0CmFwcGxpY2F0aW9uL21hY3dyaXRlaWkKYX"
                   "BwbGljYXRpb24vbXN3b3JkCQlkb2MgZG90IHdyZAphcHBsaWNhdGlvbi9uZXdzLW1lc3NhZ2UtaWQKYXBwbGljYXRpb24vbmV3cy10cm"
                   "Fuc21pc3Npb24KYXBwbGljYXRpb24vb2N0ZXQtc3RyZWFtCWJpbiBkbXMgbGhhIGx6aCBleGUgY2xhc3MKYXBwbGljYXRpb24vb2RhCQ"
                   "kJb2RhCmFwcGxpY2F0aW9uL3BkZgkJCXBkZgphcHBsaWNhdGlvbi9wZ3AJCQlwZ3AKYXBwbGljYXRpb24vcGdwLXNpZ25hdHVyZQlwZ3"
                   "AKYXBwbGljYXRpb24vcG9zdHNjcmlwdAkJYWkgZXBzIHBzCmFwcGxpY2F0aW9uL3Bvd2VycG9pbnQJCXBwdAphcHBsaWNhdGlvbi9yZW"
                   "1vdGUtcHJpbnRpbmcKYXBwbGljYXRpb24vcnRmCQkJcnRmCmFwcGxpY2F0aW9uL3NsYXRlCmFwcGxpY2F0aW9uL3dpdGEKYXBwbGljYX"
                   "Rpb24vd29yZHBlcmZlY3Q1LjEJd3A1CmFwcGxpY2F0aW9uL3gtMTIzCQl3awphcHBsaWNhdGlvbi94LVdpbmd6CQl3egphcHBsaWNhdG"
                   "lvbi94LWJjcGlvCQliY3BpbwphcHBsaWNhdGlvbi94LWNkbGluawkJdmNkCmFwcGxpY2F0aW9uL3gtY2hlc3MtcGduCQlwZ24KYXBwbGl"
                   "jYXRpb24veC1jb21wcmVzcwkJeiBaCmFwcGxpY2F0aW9uL3gtY3BpbwkJY3BpbwphcHBsaWNhdGlvbi94LWNzaAkJY3NoCmFwcGxpY2F"
                   "0aW9uL3gtZGViaWFuLXBhY2thZ2UJZGViCmFwcGxpY2F0aW9uL3gtZGlyZWN0b3IJCWRjciBkaXIgZHhyCmFwcGxpY2F0aW9uL3gtZHZp"
                   "CQlkdmkKYXBwbGljYXRpb24veC1ndGFyCQlndGFyIHRnegphcHBsaWNhdGlvbi94LWd1bnppcAkJZ3oKYXBwbGljYXRpb24veC1nemlw"
                   "CQlnegphcHBsaWNhdGlvbi94LWhkZgkJaGRmCmFwcGxpY2F0aW9uL3gtaHR0cGQtcGhwCQlwaHRtbCBwaHQgcGhwCmFwcGxpY2F0aW9u"
                   "L3gtamF2YXNjcmlwdAlqcwphcHBsaWNhdGlvbi94LWtvYW4JCXNrcCBza2Qgc2t0IHNrbQphcHBsaWNhdGlvbi94LWxhdGV4CQlsYXRle"
                   "AphcHBsaWNhdGlvbi94LW1ha2VyCQlmcm0gbWFrZXIgZnJhbWUgZm0gZmIgYm9vayBmYmRvYwphcHBsaWNhdGlvbi94LW1pZgkJbWlmCm"
                   "FwcGxpY2F0aW9uL3gtbXNkb3MtcHJvZ3JhbQljb20gZXhlIGJhdAphcHBsaWNhdGlvbi94LW5ldGNkZgkJbmMgY2RmCmFwcGxpY2F0aW9"
                   "uL3gtbnMtcHJveHktYXV0b2NvbmZpZwlwYWMKYXBwbGljYXRpb24veC1wZXJsCQlwbCBwbQphcHBsaWNhdGlvbi94LXNoCQlzaAphcHBs"
                   "aWNhdGlvbi94LXNoYXIJCXNoYXIKYXBwbGljYXRpb24veC1zdHVmZml0CQlzaXQK"
                   "YXBwbGljYXRpb24veC1zdjRjcGlvCQlzdjRjcGlvCmFwcGxpY2F0aW9uL3gtc3Y0Y3JjCQlzdjRjcmMKYXBwbGljYXRpb24veC10YXIJ"
                   "CXRhcgphcHBsaWNhdGlvbi94LXRjbAkJdGNsCmFwcGxpY2F0aW9uL3gtdGV4CQl0ZXgKYXBwbGljYXRpb24veC10ZXhpbmZvCQl0ZXhpb"
                   "mZvIHRleGkKYXBwbGljYXRpb24veC10cm9mZgkJdCB0ciByb2ZmCmFwcGxpY2F0aW9uL3gtdHJvZmYtbWFuCQltYW4KYXBwbGljYXRpb2"
                   "4veC10cm9mZi1tZQkJbWUKYXBwbGljYXRpb24veC10cm9mZi1tcwkJbXMKYXBwbGljYXRpb24veC11c3RhcgkJdXN0YXIKYXBwbGljYXRp"
                   "b24veC13YWlzLXNvdXJjZQlzcmMKYXBwbGljYXRpb24vemlwCQkJemlwCmF1ZGlvL2Jhc2ljCQkJYXUgc25kCmF1ZGlvL21pZGkJCQltaW"
                   "QgbWlkaSBrYXIKYXVkaW8vbXBlZwkJCW1wZ2EgbXAyIG1wMwphdWRpby94LWFpZmYJCQlhaWYgYWlmYyBhaWZmCmF1ZGlvL3gtcG4tcm"
                   "VhbGF1ZGlvCQlyYSByYW0KYXVkaW8veC1wbi1yZWFsYXVkaW8tcGx1Z2luCmF1ZGlvL3gtcmVhbGF1ZGlvCQlyYQphdWRpby94LXdhdgk"
                   "JCXdhdgpjaGVtaWNhbC94LXBkYgkJCXBkYiB4eXoKaW1hZ2UvZ2lmCQkJZ2lmCmltYWdlL2llZgkJCWllZgppbWFnZS9qcGVnCQkJanBl"
                   "ZyBqcGcganBlCmltYWdlL3BuZwkJCXBuZwppbWFnZS90aWZmCQkJdGlmZiB0aWYKaW1hZ2UveC1jbXUtcmFzdGVyCQlyYXMKaW1hZ2UveC"
                   "1wb3J0YWJsZS1hbnltYXAJCXBubQppbWFnZS94LXBvcnRhYmxlLWJpdG1hcAkJcGJtCmltYWdlL3gtcG9ydGFibGUtZ3JheW1hcAlwZ2"
                   "0KaW1hZ2UveC1wb3J0YWJsZS1waXhtYXAJCXBwbQppbWFnZS94LXJnYgkJCXJnYgppbWFnZS94LXhiaXRtYXAJCQl4Ym0KaW1hZ2UveC"
                   "14cGl4bWFwCQkJeHBtCmltYWdlL3gteHdpbmRvd2R1bXAJCXh3ZAptZXNzYWdlL2V4dGVybmFsLWJvZHkKbWVzc2FnZS9uZXdzCm1lc"
                   "3NhZ2UvcGFydGlhbAptZXNzYWdlL3JmYzgyMgptb2RlbC9pZ2VzCQkJaWdzIGlnZXMKbW9kZWwvbWVzaAkJCW1zaCBtZXNoIHNpbG8KbW9"
                   "kZWwvdnJtbAkJCXdybCB2cm1sCm11bHRpcGFydC9hbHRlcm5hdGl2ZQptdWx0aXBhcnQvYXBwbGVkb3VibGUKbXVsdGlwYXJ0L2RpZ2Vzd"
                   "AptdWx0aXBhcnQvbWl4ZWQKbXVsdGlwYXJ0L3BhcmFsbGVsCnRleHQvY3NzCQkJY3NzCnRleHQvaHRtbAkJCWh0bWwgaHRtCnRleHQvcGx"
                   "haW4JCQlhc2MgdHh0IGMgY2MgaCBoaCBjcHAgaHBwCnRleHQvcmljaHRleHQJCQlydHgKdGV4dC90YWItc2VwYXJhdGVkLXZhbHVlcwl0c"
                   "3YKdGV4dC94LXNldGV4dAkJCWV0eAp0ZXh0L3gtc2dtbAkJCXNnbWwgc2dtCnRleHQveC12Q2FsZW5kYXIJCXZjcwp0ZXh0L3gtdkNhcmQ"
                   "JCQl2Y2YKdGV4dC94bWwJCQl4bWwgZHRkCnZpZGVvL2RsCQkJZGwKdmlkZW8vZmxpCQkJZmxpCnZpZGVvL2dsCQkJZ2wKdmlkZW8vbXBlZ"
                   "wkJCW1wMiBtcGUgbXBlZyBtcGcKdmlkZW8vcXVpY2t0aW1lCQkJcXQgbW92CnZpZGVvL3gtbXN2aWRlbwkJCWF2aQp2aWRlby94LXNnaS1"
                   "tb3ZpZQkJbW92aWUKeC1jb25mZXJlbmNlL3gtY29vbHRhbGsJ"
                   "CWljZQp4LXdvcmxkL3gtdnJtbAkJCXdybCB2cm1sCg==";

void *web_server__resolver(web_server * ws);
void web_server::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{   XTRY;
    tcp_server::start(pn,prefix,m);
    max_multipart=3050000;
    max_post=200000;
    document_root="./www";
    doc_urls.assign(splitTOKs(",","/html/,/pics/"));

    vector<string> v=splitTOK("\r\n",Base64Decode(mime));
    for(unsigned int i=0; i<v.size(); i++) {
        XTRY;
        if(!v[i].size()) {
            continue;
        }
        vector<string> q=splitTOK("\t ",v[i].c_str());
        if(q.size()<1) {
            continue;
        }
        string val=q[0];
        for(unsigned int j=1; j<q.size(); j++) {
            if(!q[j].size()) {
                continue;
            }
            mime_types.set(q[j],val);
        }
        XPASS;
    }

    load_config(pn,prefix,m);
    XPASS;
}
web_server::web_server()
{
}



web_server::~web_server()
{
}


void web_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{   MUTEX_INSPECTOR;
    XTRY;
    set_cfg_i(m,	prefix+"max_multipart", pn,max_multipart,3050000);
    set_cfg_i(m,	prefix+"max_post", pn,max_post,200000);
    set_cfg_str(m,	prefix+"document_root", pn,document_root,"./www");
    if(m.count(prefix+"doc_urls"))
        doc_urls.assign(splitTOKs(",",m[prefix+"doc_urls"]));
    else
    {
        logErr2("%s: skipped '%sdoc_urls' parameter, using default '/html/,/pics/'",pn.c_str(),prefix.c_str());
        doc_urls.assign(splitTOKs(",","/html/,/pics/"));
    }
    m.erase(prefix+"doc_urls");

    if(!m.count(prefix+"mime_types"))
    {
        logErr2("%s: skipped '%smime_types' unable to continue",pn.c_str(),prefix.c_str());
    }
    string all;
    if(load_1_file(all, m[prefix+"mime_types"])!=-1)
    {
        vector<string> v=splitTOK("\r\n",all.c_str());
        for(unsigned int i=0; i<v.size(); i++) {
            if(!v[i].size()) {
                continue;
            }
            vector<string> q=splitTOK("\t ",v[i].c_str());
            if(q.size()<1) {
                continue;
            }
            string val=q[0];
            for(unsigned int j=1; j<q.size(); j++) {
                if(!q[j].size()) {
                    continue;
                }
                mime_types.set(q[j],val);
            }
        }
    }
    m.erase(prefix+"mime_types");


    XPASS;

}

