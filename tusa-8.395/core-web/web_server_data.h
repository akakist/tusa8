#ifndef WEBSERVERDATA_HH
#define WEBSERVERDATA_HH
#include "M_MAP.h"
#include "M_DEQUE.h"
#include "PTR.h"
#include "m_var.h"
#include <c_conn.h>

class web_server_data
{
public:
    m_var<unsigned int> _max_multipart;
    m_var<unsigned int> _max_post;
    M_MAP<string,string>mime_types;
    web_server_data() {}
};

#endif
