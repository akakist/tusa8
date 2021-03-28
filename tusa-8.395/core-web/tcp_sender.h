#ifndef TCP__SENDER_H
#define TCP__SENDER_H
#ifndef WITH_SENDERS
#ifndef NO_SENDERS
#error "must define WITH_SENDERS or NO_SENDERS"
#endif
#endif
#ifdef WITH_SENDERS

void write_sender_data(const PTR_getter<c_socket> &socket, const string& buf);

#ifdef WIN32
__declspec(dllexport)
#endif
void init_senders(const string& pn,const string& prefix,map<string,string>&m);
void destroy_senders();
void hup_senders(const string& pn,const string& prefix,map<string,string>&m);

#endif
#endif

