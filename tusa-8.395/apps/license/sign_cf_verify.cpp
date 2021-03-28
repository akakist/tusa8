#ifdef __GNUC__
#pragma implementation				// gcc: implement sql_udf.h
#endif

#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>
#include <set>
#include "st_rsa.h"
#include "file_set.h"
bool verify_serial(const string & serialbuf, const string &pub_key) // OK if matched
{
    vector<string> vs=splitTOK("-",serialbuf);
    if(vs.size()!=3)
    {
        throw cError("vs.size()!=3\n");
    }
    string sign=hex2bin(vs[2]);

    unsigned int no;
    {
        unsigned short is[2];
        for(int i=0;i<2;i++)
        {
            char *sa=strdup(vs[i].c_str());
            sscanf(sa,"%x",&is[i]);
        }
        unsigned int *p=(unsigned int *) &is;
        no=*p;
    }
    st_rsa rsa;
    rsa.pub_key_set(pub_key);
    rsa.init_from_pub_key();
    enc_string es;
    es.buf=sign.substr(1,sign.size()-1);

//    unsigned int i1,i2;
    if(sign.size()<2)
    {
        printf("sign.size()<2\n");
        throw cError("sign.size()<2");
    }
    unsigned char *p=(unsigned char *)sign.data();
    es.original_size=/*p[0]*256+*/p[1];
    string decoded=rsa.public_decrypt(es);

    {
        unsigned int *pz=(unsigned int *) decoded.data();
        int no_res=*pz;
        if(no==no_res)
            return true;
        else
            return false;
    }
    return false;
}
int main(int argc, char **argv)
{
    if(argc!=2){ printf("usage sign_cf_verify serial\n");exit(1);}
    string ser=argv[1];
    try{
        string key;
        if(load_1_file(key,"./rsa.pub")==-1)
        {
            printf("error loading key\n");
            abort();
        }
        if(verify_serial(ser,key))
        {
            printf("SER OK\n");
        }
        else
            printf("SER FAILED\n");

    }
    catch(cError e)
    {
        printf("catched %s\n",e.error_string.c_str());
    }
    catch(...)
    {
        printf("catched\n");
    }
}
