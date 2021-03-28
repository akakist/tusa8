#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>
#include "mutex_inspector.h"

#include "DB_id.h"
#include "copy_file.h"
#include "geoip.h"
#include "server_config.h"

class  __geoip_stuff
{
    _mutex Mutex;
    map<unsigned int,geoip_location_dump > m_loc;
    map<unsigned int,pair<unsigned int,unsigned int> > m_block;

};

oscar_buffer & operator>> (oscar_buffer& b,  geoip_location_dump &s)
{
    b>>s.country>>s.region>>s.city>>s.postalCode>>s.coord;
    return b;
}
out_oscar_buffer & operator<< (out_oscar_buffer& b,const geoip_location_dump &s)
{
    b<<s.country<<s.region<<s.city<<s.postalCode<<s.coord;
    return b;
}

static string get_v(const string &v)
{
    if(v.size())
    {
        if(v[0]!='\"') return v;
        return v.substr(1,v.size()-2);
    }
    return "";
}

void load_geoip_location_dump_from_csv(const string &fn, map<unsigned int,geoip_location_dump > &m_loc)
{
    int n=0;
    char s[1000];
    FILE *sin=fopen(fn.c_str(),"rb");
    if(!sin)
    {
        printf("cannot load argv[1]");
        exit(1);
    }
    while(fgets(s,sizeof(s),sin))
    {
        vector<string> v=splitTOK(",\r\n",s);
        if(v.size()>=7)
        {
            if(v[0]=="locId") continue;
            n++;
            if(n%1000==0) printf("%d\n",n);

            unsigned int locId;
            geoip_location_dump e;

            locId=atoi(get_v(v[0]));
            e.country=get_v(v[1]);
            e.region=get_v(v[2]);
            e.city=get_v(v[3]);
            e.postalCode=get_v(v[4]);
            double coord[2];
            coord[0]=atof(get_v(v[5]).c_str());
            coord[1]=atof(get_v(v[6]).c_str());
            e.coord=string((char*)&coord[0],sizeof(double)*2);
            m_loc[locId]=e;
        }
    }
}

void load_geoip_block_from_csv(const string& fn,map<unsigned int,pair<unsigned int,unsigned int> > &m_block)
{
    int n=0;
    char s[1000];
    FILE *sin=fopen(fn.c_str(),"rb");
    if(!sin)
    {
        printf("cannot load argv[1]");
        exit(1);
    }
    while(fgets(s,sizeof(s),sin))
    {
        vector<string> v=splitTOK(",",s);
        if(v.size()==3)
        {
            n++;
            if(n%1000==0) printf("%d\n",n);

            unsigned int startIpNum,endIpNum,locId;
            startIpNum=atoi(get_v(v[0]));
            endIpNum=atoi(get_v(v[1]));
            locId=atoi(get_v(v[2]));
            m_block[startIpNum]=make_pair(endIpNum,locId);


        }
    }
    printf("done\n");

}

