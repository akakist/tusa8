#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "str_lib.h"
#include "st_stuff.h"
#include <openssl/des.h>
#include <map>
#include "_mutex.h"
#include "m_var.h"
#include "des_locl.h"
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include "sid_t.h"
#include "des_sid.h"

#define DES_CBLOCK unsigned char*
#define SIDLEN 8

void des_sid_cpp() {};
void des_cbc_encrypt1(DES_CBLOCK input, DES_CBLOCK output, long length, des_key_schedule schedule,
                      DES_CBLOCK ivec, int enc)
{
    register DES_LONG tin0, tin1;
    register DES_LONG tout0, tout1, xor0, xor1;
    register unsigned char *in, *out;
    register long l = length;
    DES_LONG tin[2];
    unsigned char *iv;

    in = (unsigned char *)input;
    out = (unsigned char *)output;
    iv = (unsigned char *)ivec;

    if (enc) {
        c2l(iv, tout0);
        c2l(iv, tout1);
        for (l -= 8; l >= 0; l -= 8) {
            c2l(in, tin0);
            c2l(in, tin1);
            tin0 ^= tout0;
            tin[0] = tin0;
            tin1 ^= tout1;
            tin[1] = tin1;
            des_encrypt1((DES_LONG *) tin, schedule, DES_ENCRYPT);
            tout0 = tin[0];
            l2c(tout0, out);
            tout1 = tin[1];
            l2c(tout1, out);
        }
        if (l != -8) {
            c2ln(in, tin0, tin1, l + 8);
            tin0 ^= tout0;
            tin[0] = tin0;
            tin1 ^= tout1;
            tin[1] = tin1;
            des_encrypt1((DES_LONG *) tin, schedule, DES_ENCRYPT);
            tout0 = tin[0];
            l2c(tout0, out);
            tout1 = tin[1];
            l2c(tout1, out);
        }
    }
    else {
        c2l(iv, xor0);
        c2l(iv, xor1);
        for (l -= 8; l >= 0; l -= 8) {
            c2l(in, tin0);
            tin[0] = tin0;
            c2l(in, tin1);
            tin[1] = tin1;
            des_encrypt1((DES_LONG *) tin, schedule, DES_DECRYPT);
            tout0 = tin[0] ^ xor0;
            tout1 = tin[1] ^ xor1;
            l2c(tout0, out);
            l2c(tout1, out);
            xor0 = tin0;
            xor1 = tin1;
        }
        if (l != -8) {
            c2l(in, tin0);
            tin[0] = tin0;
            c2l(in, tin1);
            tin[1] = tin1;
            des_encrypt1((DES_LONG *) tin, schedule, DES_DECRYPT);
            tout0 = tin[0] ^ xor0;
            tout1 = tin[1] ^ xor1;
            l2cn(tout0, tout1, out, l + 8);
        }
    }
    tin0 = tin1 = tout0 = tout1 = xor0 = xor1 = 0;
    tin[0] = tin[1] = 0;
}
static m_var<string> _des_key;
string get_des_hex_sid(sid_t & sid)
{
    try {
        if (_des_key.get().size() == 0) {
            _des_key.set(string("pizdec40"));
        }
        char tmp[SIDLEN * 2];

        ::memcpy(&tmp[0], &sid, sizeof(sid));
        string o(tmp, SIDLEN);


        unsigned char iv[8];
        des_cblock kk;
        des_key_schedule ks;

        char *d_key = strdup(_des_key.get().c_str());
        des_string_to_key(d_key, &kk);
        des_set_key((C_Block *) kk, ks);
        ::memset(iv, 0, sizeof(iv));
        free(d_key);
        char out[SIDLEN * 2];
        des_cbc_encrypt1((DES_CBLOCK) & tmp[0], (DES_CBLOCK) & out[0], SIDLEN, ks, (DES_CBLOCK) & iv, DES_ENCRYPT);

        string r((char *)out, SIDLEN);
        return bin2hex(r, 100);
    }
    STDCATCHS("get_des_hex_sid STDCATCHS");

    return "";
}
int  parse_des_hex_sid(const string &str,sid_t &sid)
{
    string tmp=hex2bin(str);
    if (tmp.size() != SIDLEN)
        return -1;

    if (_des_key.get().size() == 0) {
        _des_key.set(string("pizdec40"));
    }

    unsigned char iv[8];
    des_cblock kk;
    des_key_schedule ks;

    char *d_key = strdup(_des_key.get().c_str());
    des_string_to_key(d_key, &kk);
    des_set_key((C_Block *) kk, ks);
    ::memset(iv, 0, sizeof(iv));
    free(d_key);
    char out[SIDLEN*2];
    char tmpb[SIDLEN*2];
    ::memcpy(tmpb,tmp.data(),SIDLEN);
    ::memcpy(out,tmpb,SIDLEN)	;

    des_cbc_encrypt1((DES_CBLOCK) &tmpb[0], (DES_CBLOCK) &out[0], SIDLEN, ks, (DES_CBLOCK) & iv, DES_DECRYPT);
    string o((char*)out,SIDLEN);
    ::memcpy(&sid,&out,sizeof(sid));
    return 0;
}

string des_decrypt(const string&key, const string& str)
{

    unsigned char iv[8];
    des_cblock kk;
    des_key_schedule ks;

    char *d_key = strdup(key.c_str());
    des_string_to_key(d_key, &kk);
    des_set_key((C_Block *) kk, ks);
    ::memset(iv, 0, sizeof(iv));
    free(d_key);
    char *out=(char*)alloca(str.size()+10);
    char *tmp=(char*)alloca(str.size()+10);

    ::memcpy(tmp,str.data(),str.size());
    ::memcpy(out,str.data(),str.size())	;

    des_cbc_encrypt1((DES_CBLOCK) &tmp[0], (DES_CBLOCK) &out[0], str.size(), ks, (DES_CBLOCK) & iv, DES_DECRYPT);
    string o((char*)out,str.size());
    return o;
}

string des_encrypt(const string&key, const string& str)
{

    unsigned char iv[8];
    des_cblock kk;
    des_key_schedule ks;

    char *d_key = strdup(key.c_str());
    des_string_to_key(d_key, &kk);

    des_set_key((C_Block *) kk, ks);

    ::memset(iv, 0, sizeof(iv));
    free(d_key);
    char *out=(char*)alloca(str.size()*2+10);
    char *tmp=(char*)alloca(str.size()*2+10);
    des_cbc_encrypt1((DES_CBLOCK) & tmp[0], (DES_CBLOCK) & out[0], str.size(), ks, (DES_CBLOCK) & iv, DES_ENCRYPT);
    string r((char *)out, str.size());
    return r;
}

