#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>
#include "DB_id.h"
#include "copy_file.h"
#include "geoip.h"
#define LIMIT 5000000
string str_nick(const string &n);
#define TRYOP(nnn) 	try{\
	nnn ; \
	}catch(cError e){printf("\n%s\n",e.what());};
#define SPLITTER "/"
void load_geoip_location_dump_from_csv(const string &fn, map<unsigned int,geoip_location_dump > &m_loc);
void load_geoip_block_from_csv(const string& fn,map<unsigned int,pair<unsigned int,unsigned int> > &m_block);

int main(int argc,char *argv[])
{
    map<unsigned int,geoip_location_dump > m_loc;
    map<unsigned int,pair<unsigned int,unsigned int> > m_block;
    load_geoip_block_from_csv(argv[1],m_block);
    load_geoip_location_dump_from_csv(argv[2],m_loc);

    out_oscar_buffer b;
    b<<m_block;
    b<<m_loc;
    FILE * f=fopen("geoip.bin","wb");
    fwrite(b.data(),b.size(),1,f);
    fclose(f);

    return 0;
}





