#ifndef WEB_SERVER_H__
#define WEB_SERVER_H__
#include "PTR.h"
#include "tcp_server.h"
#include "M_MAP.h"
#include "M_SET.h"
#include "file_set.h"
using namespace std;
class  params_map;
class web_server : public tcp_server
{

private:
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
    void load_mime_types();

protected:

public:

    virtual ~web_server();
    web_server();
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

    unsigned int max_multipart;
    unsigned int max_post;
    M_MAP<string,string>mime_types;
    m_var<string> document_root;
    M_SET<string> doc_urls;
};



#endif
