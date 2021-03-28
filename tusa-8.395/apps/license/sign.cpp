#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>
#include <set>
#include "st_rsa.h"
#include "file_set.h"
#include <ctype.h>
int 
main(int argc, char **argv)
// main(int argc, char * argv[])
{
try{
	st_rsa rsa;
	string key;
	if(argc!=2){printf("usage sign licfile\n");return -1;}
	//use_mem_files.set(UMF_DISK_ONLY);
	if(load_1_file(key,"../../rsa_keys/rsa.priv")==-1)
	{
		printf("error loading key");
		abort();
	}
	rsa.priv_key_set(key);	
	rsa.init_from_priv_key();
	string lb;
	if(load_1_file(lb,argv[1])==-1)
	{
		printf("error loading key %s",argv[0]);
		abort();
	}
	string lo;
	for(unsigned i=0;i<lb.size();i++)
	{
		if(!isspace(lb[i]))lo+=lb[i];
	}
	printf("Belialov.com server license\n<\n");
	
	printf("%s\n",lb.c_str());
	printf(">\n");
	string sn;
	enc_string es;
	es=rsa.private_encrypt(Base64Encode(MD5(lo)));
	unsigned ol=es.original_size;
	unsigned char ol1=ol/256;
	unsigned char ol2=ol&0xff;
	sn+=string((char*)&ol1,1);
	sn+=string((char*)&ol2,1);
	sn+=es.buf;
	printf("sign=%s", bin2hex(sn,0).c_str());
	
}
catch(cError e)	
{
printf("catched %s\n",e.error_string.c_str());
}
catch(...)	
{
printf("catched\n");
}
/*	rsa.generate_key(1024);
	st_FILE f1("rsa.pub","wb");
	st_FILE f2("rsa.priv","wb");
	f1<<rsa.pub_key_get();
	f2<<rsa.priv_key_get();
*/
}
