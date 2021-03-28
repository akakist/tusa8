#ifndef ST_RSA__H
#define ST_RSA__H
#include <string>
#include "_mutex.h"
typedef struct rsa_st RSA;
using namespace std;
struct enc_string
{
    string buf;
    size_t original_size;
    enc_string() {}
};
class st_rsa
{
    _mutex Mutex;
    RSA *rsa_xxx;
    int bits;
    string pub_key;
    string priv_key;
public:
    string public_key();
    string private_key();
    string pub_key_get();
    void pub_key_set(const string &);
    string priv_key_get();
    void priv_key_set(const string&);
    st_rsa();
    ~st_rsa();
    int size();
    void generate_key(int b);
    void init_from_pub_key();
    void init_from_priv_key();
    enc_string encrypt(const string &m, int (*func)(int, const unsigned char*, unsigned char*, RSA*,int));
    enc_string private_encrypt(const string &m);
    enc_string public_encrypt(const string &m);
    string decrypt(const enc_string &m, int (*func)(int, const unsigned char*, unsigned char*, RSA*,int));
    string private_decrypt(const enc_string &m);
    string public_decrypt(const enc_string &m);
};

class out_oscar_buffer;
class oscar_buffer;
out_oscar_buffer & operator<< (out_oscar_buffer& b,const enc_string &s);
oscar_buffer & operator>> (oscar_buffer& b,  enc_string &s);

#endif



