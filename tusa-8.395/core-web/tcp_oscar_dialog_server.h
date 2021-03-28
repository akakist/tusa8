
#ifndef TCP_OSCAR_DIALOG_SERVER__H
#define TCP_OSCAR_DIALOG_SERVER__H

#include "tcp_dialog_server.h"
class tcp_oscar_dialog_server: public tcp_dialog_server
{

    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
public:
    tcp_oscar_dialog_server();
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

    // derived call to process buffer
    void process_buffer(const PTR_getter<c_socket>& __SD);

    // process command in inherited class
    virtual void parse_ch1(const PTR_getter<c_socket>& __SD,oscar_buffer & in_b)=0;
    virtual void parse_ch2(const PTR_getter<c_socket>& __SD,oscar_buffer & in_b)=0;
    virtual void parse_ch3(const PTR_getter<c_socket>& __SD,oscar_buffer & in_b)=0;
    virtual void parse_ch4(const PTR_getter<c_socket>& __SD,oscar_buffer & in_b)=0;
    virtual void parse_ch5(const PTR_getter<c_socket>& __SD,oscar_buffer & in_b)=0;
    virtual void parse_ch6(const PTR_getter<c_socket>& __SD,oscar_buffer & in_b)=0;


};


#ifdef WIN32
extern m_var<__int64>tcp_oscar_dialog_server_total_processed_requests;
#else
extern m_var<long long>tcp_oscar_dialog_server_total_processed_requests;
#endif

#endif
