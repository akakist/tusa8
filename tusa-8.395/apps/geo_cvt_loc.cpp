#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>
#include "DB_id.h"
#include "copy_file.h"
#define LIMIT 5000000
string str_nick(const string &n);
#define TRYOP(nnn) 	try{\
	nnn ; \
	}catch(cError e){printf("\n%s\n",e.what());};
#define SPLITTER "/"
string get_v(const string &v)
{
    if(v.size())
    {
	if(v[0]!='\"') return v;
	return v.substr(1,v.size()-2);
    }
    return "";
}
//map<string,string> params;

struct geoip_location
{
	    string country,region,city,postalCode;
	    string coord;
};
out_oscar_buffer & operator<< (out_oscar_buffer& b,const geoip_location &s)
{
    b<<s.country<<s.region<<s.city<<s.postalCode<<s.coord;                         
    return b;
}
oscar_buffer & operator>> (oscar_buffer& b,  geoip_location &s)
{ 
    b>>s.country>>s.region>>s.city>>s.postalCode>>s.coord;                         
     return b;
} 

/*struct en2
{
	    double latitude,longitude;
};*/

void load_loc(const string &fn)
{
int n=0;
    char s[1000];
//    char bb[100000];
    map<unsigned int,geoip_location > m;
//    map<unsigned int,en2 > m2;
//    printf("KALL %s %d\n",__FILE__,__LINE__);
    FILE *sin=fopen(fn.c_str(),"rb");
    if(!sin) 
    {
	printf("cannot load argv[1]");
	exit(1);
    }
    while(fgets(s,sizeof(s),sin))
    {
	//setbuffer(stdin,bb,sizeof(bb));
//    printf("KALL %s %d\n",__FILE__,__LINE__);
	vector<string> v=splitTOK(",\r\n",s);
//	if(n>100000) break;
	if(v.size()>=7)
	{
	    if(v[0]=="locId") continue;
//    printf("KALL %s %d\n",__FILE__,__LINE__);
	    n++;
	    if(n%1000==0) printf("%d\n",n);

	    unsigned int locId;
	    geoip_location e;
//	    en2 e2;
//	    unsigned int startIpNum,endIpNum,locId;
//    printf("KALL %s %d\n",__FILE__,__LINE__);

	    locId=atoi(get_v(v[0]));
	    e.country=get_v(v[1]);
	    e.region=get_v(v[2]);
	    e.city=get_v(v[3]);
	    e.postalCode=get_v(v[4]);
	    double coord[2];
//    printf("KALL %s %d\n",__FILE__,__LINE__);
	    coord[0]=atof(get_v(v[5]).c_str());
	    coord[1]=atof(get_v(v[6]).c_str());
	    e.coord=string((char*)&coord[0],sizeof(double)*2);
//    printf("KALL %s %d\n",__FILE__,__LINE__);
	    m[locId]=e;
//    printf("KALL %s %d\n",__FILE__,__LINE__);
	}
    }
    
    printf("_locks.bin\n");
    {
    out_oscar_buffer b;
    b<<m;
    FILE * f=fopen("_locks.bin","wb");
    fwrite(b.data(),b.size(),1,f);
    fclose(f);
    }
    printf("done\n");

}
int main(int argc,char *argv[])
{
    
    load_loc(argv[1]);
    return 0;
}





