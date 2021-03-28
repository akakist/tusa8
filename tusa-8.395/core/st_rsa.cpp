#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>
#include "st_stuff.h"
#include "_mutex.h"
#include <map>
#include "mutex_inspector.h"
#include "m_var.h"
#include "st_rsa.h"
#include "lockctl.h"
#include <openssl/ssl.h>
#ifdef WIN32
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#endif

void st_rsa_cpp() {}
void st_rsa::init_from_pub_key()
{
    M_LOCK(Mutex);
    const unsigned char *p = (const unsigned char *)pub_key.data();
    rsa_xxx = d2i_RSAPublicKey(NULL, &p, pub_key.size());
    if (!rsa_xxx) {
        throw cError("i2d_RSAPublicKey"+_DMI());
    }
}
void st_rsa::init_from_priv_key()
{
    M_LOCK(Mutex);
    const unsigned char *p = (const unsigned char *)priv_key.data();
    rsa_xxx = d2i_RSAPrivateKey(NULL, &p, priv_key.size());
    if (!rsa_xxx) {
        throw cError("i2d_RSAPrivateKey"+_DMI());
    }
}
string st_rsa::private_key()
{
    M_LOCK(Mutex);
    string s;
    st_malloc ppp(10000);
    char *b = (char *) ppp.buf;
    unsigned char *p = (unsigned char *) b;
    int len = 0;
    if (rsa_xxx && p) {
        len = i2d_RSAPrivateKey(rsa_xxx, &p);
        if (len < 0)
            return "";
        string s((char *) b, len);
        return s;
    } else {
        throw cError("i2d_RSAPrivateKey"+_DMI());
    }
    return s;
}

string st_rsa::public_key()
{
    M_LOCK(Mutex);
    string s;

    st_malloc b(10000);
    unsigned char *p = (unsigned char *) b.buf;

    int len = 0;
    if (rsa_xxx && p) {
        len = i2d_RSAPublicKey(rsa_xxx, &p);
        if (len < 0)
            return "";
        string s((char *) b.buf, len);
        return s;
    } else {
        throw cError("i2d_RSAPublicKey"+_DMI());
    }
    return s;
}
void st_rsa::generate_key(const int b)
{
    M_LOCK(Mutex);
    bits = b;
    rsa_xxx = RSA_generate_key(bits, RSA_F4, NULL, NULL);
    if (!rsa_xxx)
        throw cError("RSA_generate_key"+_DMI());
    pub_key = public_key();

    priv_key = private_key();
}


string st_rsa::private_decrypt(const enc_string & m)
{
    M_LOCK(Mutex);
    return decrypt(m, RSA_private_decrypt);
}

string st_rsa::public_decrypt(const enc_string & m)
{
    M_LOCK(Mutex);
    return decrypt(m, RSA_public_decrypt);
}

enc_string st_rsa::public_encrypt(const string & m)
{
    M_LOCK(Mutex);
    return encrypt(m, RSA_public_encrypt);
}

enc_string st_rsa::private_encrypt(const string & m)
{
    M_LOCK(Mutex);
    return encrypt(m, RSA_private_encrypt);
}

enc_string st_rsa::encrypt(const string & ms, int (*func) (int, const unsigned char *, unsigned char *, RSA *, int))
{
    int rsize = size();
    string mm = Base64Encode(ms);
    st_malloc vp(mm.size() + rsize);
    char *p = (char *) vp.buf;
    DUMP("new");
    memcpy(p, mm.data(), mm.size());
    int i;
    if(!rsa_xxx) throw cError("!rsa_xxx"+_DMI());
    for (i = 0; i < mm.size(); i += rsize) {
        func(rsize, (unsigned char *) &p[i], (unsigned char *) &p[i], rsa_xxx, RSA_NO_PADDING);
    }
    enc_string out;
    out.original_size = mm.size();
    out.buf = string((char *) p, i);
    return out;
}

string st_rsa::decrypt(const enc_string & m, int (*func) (int, const unsigned char *, unsigned char *, RSA *, int))
{

    st_malloc vp(m.buf.size() + 1000);
    {
        char *p = (char *) vp.buf;
        {
            memcpy(p, m.buf.data(), m.buf.size());
            {
                int rsize = size();

                if(!rsa_xxx) throw cError("!rsa_xxx"+_DMI());
                {
                    for (int i = 0; i < m.buf.size(); i += rsize) {
                        func(rsize, (unsigned char *) &p[i], (unsigned char *) &p[i], rsa_xxx, RSA_NO_PADDING);
                    }

                    string out(p, m.original_size);
                    string out1 = Base64Decode(out);
                    return out1;
                }
            }
        }
    }
    return "";
}
st_rsa::st_rsa():Mutex("st_rsa")
{
    rsa_xxx = NULL;
}
st_rsa::~st_rsa()
{
    M_LOCK(Mutex);
    if (rsa_xxx)
        RSA_free(rsa_xxx);
}
int st_rsa::size()
{
    M_LOCK(Mutex);
    if(!rsa_xxx) throw cError("!rsa_xxx"+_DMI());
    return RSA_size(rsa_xxx);
}
string st_rsa::pub_key_get()
{
    M_LOCK(Mutex);
    return pub_key;
}
void st_rsa::pub_key_set(const string &s)
{
    M_LOCK(Mutex);
    pub_key=s;
}
string st_rsa::priv_key_get()
{
    M_LOCK(Mutex);
    return priv_key;
}
void st_rsa::priv_key_set(const string &s)
{
    M_LOCK(Mutex);
    priv_key=s;
}

out_oscar_buffer & operator<< (out_oscar_buffer& b,const enc_string &s)
{
    b<<s.original_size<<s.buf;
    return b;

}
oscar_buffer & operator>> (oscar_buffer& b,  enc_string &s)
{
    b>>s.original_size>>s.buf;
    return b;
}
