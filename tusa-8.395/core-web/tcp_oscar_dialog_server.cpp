#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <stdio.h>
#include <map>
#include "lockctl.h"
#include "tcp_oscar_dialog_server.h"
#include "oscar_buffer.h"
#include "str_lib.h"

#ifdef WIN32
m_var<__int64>tcp_oscar_dialog_server_total_processed_requests(0);
#else
m_var<long long>tcp_oscar_dialog_server_total_processed_requests(0);
#endif
void tcp_oscar_dialog_server::process_buffer(const PTR_getter<c_socket>& __SD)
{   MUTEX_INSPECTOR;

    unsigned char oscar_start_byte=OSCAR2_START_BYTE;

    while (1)
    {
        string command_buf;
        unsigned char channel;
        bool need_disconnect=false;
        {
            PGLW(c_socket, sd, __SD);

            size_t  bsz=sd->rd_buf.size();

            if (bsz) {
                oscar_buffer b(sd->rd_buf.data(), sd->rd_buf.size());
                oscar_start_byte=b.get_8();

                if (oscar_start_byte==OSCAR2_START_BYTE)
                {
                    if (bsz<FLAP2_HEADER_SIZE)
                    {
                        return;
                    }
                    channel=b.get_8();
                    unsigned int data_len=b.get_32LE();
                    int tl=FLAP2_HEADER_SIZE+data_len;
                    if (bsz>=tl)
                    {
                        command_buf=sd->rd_buf.substr(FLAP2_HEADER_SIZE,data_len);
                        if (bsz>tl)
                        {

                            sd->rd_buf=sd->rd_buf.substr(tl,bsz-tl);
                        }
                        else sd->rd_buf="";
                    }
                    else
                    {
                    }


                }
                else
                {
                    sd->rd_buf="";
                }
            }
        }
        if (need_disconnect)
        {
            MUTEX_INSPECTOR;
            logErr2("invalid start byte on FLAP %d %s %d",oscar_start_byte,__FILE__,__LINE__);
            disconnect(__SD);

            return;

        }
        /*if OK process command
        */
        if (command_buf.size()) {
            ++tcp_oscar_dialog_server_total_processed_requests;
            oscar_buffer command(command_buf.data(),command_buf.size());
            if (oscar_start_byte==OSCAR2_START_BYTE)
            {
                switch (channel)
                {
                case 1:
                    parse_ch1(__SD,command);
                    break;
                case 2:
                    parse_ch2(__SD,command);
                    break;
                case 3:
                    parse_ch3(__SD,command);
                    break;
                case 4:
                    parse_ch4(__SD,command);
                    break;
                case 5:
                    parse_ch5(__SD,command);
                    break;
                case 6:
                    parse_ch6(__SD,command);
                    break;
                }

            }
            else
                logErr2("invalid start byte on FLAP %s %d",__FILE__,__LINE__);
        }
        else
            break;
    }
}
tcp_oscar_dialog_server::tcp_oscar_dialog_server()
{
}

void tcp_oscar_dialog_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
}
void tcp_oscar_dialog_server::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    MUTEX_INSPECTOR;
    tcp_dialog_server::start(pn,prefix,m);
    load_config(pn,prefix,m);
}
