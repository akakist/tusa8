#include <stdio.h>
#include "st_stuff.h"
#include "str_lib.h"
#include "mysql_extens.h"
#include <time.h>

int main()
{
	try{
	MYSQL *_dbh=new MYSQL;
	mysql_init(_dbh);
	MYSQL *res=mysql_real_connect(_dbh,NULL,NULL,NULL,"test",0,NULL,0);
	if(!res)
	{
		logErr("chat_sender: mysql_real_connect failed to %s","test");
		exit(1);
	}
	cMYSQL *dbh=new cMYSQL(_dbh);
	dbh->connected=true;
	int cnt=0;	
	time_t t=time(NULL);
	while(1)
	{
		string s=itostring(rand());
		dbh->real_queryQ((QUERY)"insert into t1 (n1,s1) values (?,?)"<<rand()<<s);
		cnt++;
		if(cnt%1000==0 && time(NULL)-t){printf("%d %d/sec\n",cnt,cnt/(time(NULL)-t));}
	}
/*	time_t t=time(NULL)-345600;	//older 4 days
	vector<string> v=dbh->select_1_column("select id from users where viscount=1 and lastdate<'2001-04-01' and id not in(223,331,2882,3307,3308,3309,3312,3432,3593,3595,3597,3726,3838,6975,6976,8822,8956,8961) order by id");
	for(int i=0;i<v.size();i++){
		if(atoi(v[i])<50){continue;}
	    dbh->real_query("delete from forum_messages where uid="+v[i]);
	    dbh->real_query("delete from forum_themes where uid="+v[i]);
	    dbh->real_query("delete from nicks where uid="+v[i]);
	    dbh->real_query("delete from notes where uid="+v[i]);
	    dbh->real_query("delete from photos where uid="+v[i]);
	    dbh->real_query("delete from user_settings where uid="+v[i]);
	    dbh->real_query("delete from user_status where uid="+v[i]);
	    dbh->real_query("delete from users where id="+v[i]);
	    printf("%s\n",v[i].c_str());
	}*/
	}catch(cError e){
		printf("Error: %s\n",e.error_string.c_str());
	}
	return 0;
}