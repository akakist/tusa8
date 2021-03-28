#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>
#include <set>
#include "st_rsa.h"
int main(int argc, char **argv)
//main()
{
//    try{
	st_rsa rsa;
	rsa.generate_key(2048);
	st_FILE f1("rsa.pub","wb");
	st_FILE f2("rsa.priv","wb");
	f1<<rsa.pub_key_get();
	f2<<rsa.priv_key_get();
//    }STDCATCH;	

}
