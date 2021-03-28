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
int main(int argc, char **argv)
// main(int argc, char * argv[])
{
	if(argc!=2){ printf("usage sign_cf number");exit(1);}
	unsigned int no=atoi(argv[1]);
try{
	st_rsa rsa;
	string key;
//	if(argc!=3){printf("usage sign licfile\n");return -1;}
//	use_mem_files.set(UMF_DISK_ONLY);
	if(load_1_file(key,"./rsa.priv")==-1)
	{
		printf("error loading key\n");
		abort();
	}
	
	rsa.priv_key_set(key);	
	rsa.init_from_priv_key();
	string lb=string((char*)&no,4);
//printf("kall1\n");
/*	string lb;
	if(load_1_file(lb,argv[1])==-1)
	{
		printf("error loading key %s",argv[0]);
		abort();
	}*/
//	logErr2("lb.size() %d\n",lb.size());
/*	string lo;
	for(unsigned i=0;i<lb.size();i++)
	{
		if(!isspace(lb[i]))lo+=lb[i];
	}*/
/*	printf("Belialov.com server license\n<\n");
	
	printf("%s\n",lb.c_str());
	printf(">\n");*/
	string sn;
	enc_string es;
	es=rsa.private_encrypt(/*Base64Encode(MD5(lo))*/lb);
	string out;
	unsigned short *pp1=(unsigned short *)&no;
	unsigned short *pp2=pp1+1;
//	unsigned short *p1=(unsigned short *)es.buf.data();
	char ss[1000];
	sprintf(ss,"%04X-%04X-",pp1[0],pp1[1]);
//	string sn;
	unsigned char ol=es.original_size;
//	unsigned char ol1=ol/256;
//	unsigned char ol2=ol&0xff;
	printf("es.original_size %d\n",es.original_size);
	
	sn+=string((char*)&ol,1);
//	sn+=string((char*)&ol2,1);
	sn+=es.buf;
	out+=ss;
	out+=bin2hex(sn,10);
	printf("%s\n",out.c_str());
//	string md5=MD5(es.buf);
//	unsigned short *p1=(unsigned short *)md5.data();
/*	unsigned char *p2=p1+2;
	unsigned char *p3=p2+2;
	unsigned char *p4=p3+2;*/
/*	unsigned short *is1=(unsigned short *)p1;
	unsigned short *is2=(unsigned short *)p2;
	unsigned short *is3=(unsigned short *)p3;
	unsigned short *is4=(unsigned short *)p4;
*/
//	unsigned short *pp1=(unsigned short *)&no;
//	unsigned short *pp2=pp1+1;
	
//	printf("%04X-%04X-%04X-%04X-%04X-%04X\n",pp1[0],pp1[1],p1[0],p1[1],p1[2],p1[3]);
	
/*	unsigned char *p1=(unsigned char *)md5.data();
	unsigned char *p2=p1+2;
	unsigned char *p3=p2+2;
	unsigned char *p4=p3+2;
	unsigned short *is1=(unsigned short *)p1;
	unsigned short *is2=(unsigned short *)p2;
	unsigned short *is3=(unsigned short *)p3;
	unsigned short *is4=(unsigned short *)p4;

	unsigned short *pp1=(unsigned short *)&no;
	unsigned short *pp2=pp1+1;
	
	printf("%04X-%04X-%04X-%04X-%04X-%04X\n",*pp1,*pp2,*is1,*is2,*is3,*is4);
	*/
/*	unsigned ol=es.original_size;
	unsigned char ol1=ol/256;
	unsigned char ol2=ol&0xff;
	sn+=string((char*)&ol1,1);
	sn+=string((char*)&ol2,1);
	sn+=es.buf;
	string out=bin2hex(sn,0);
	printf("sign=%s", out.c_str());
	
*/	

	
	
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
