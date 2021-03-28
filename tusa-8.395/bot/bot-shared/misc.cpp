#include <string>
#include "misc.h"
#include "utils.h"
#include "config.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "errs.h"
#include <stdio.h>
#include "mutex_inspector.h"

bool odd;

void privmsg(const string &nick,const string &msg)
{MUTEX_INSPECTOR;
XTRY
	if(odd){
		sock.response("PRIVMSG "+nick+" :"+msg);
	}else{
		sock.response("PRIVMSG "+nick+" :"+msg+" ");
	}
	odd=!odd;
XPASS	
}

void channelmsg(const string &msg)
{MUTEX_INSPECTOR;
XTRY
	if(odd){
		sock.response("PRIVMSG "+params["CHANNEL"]+" :"+msg);
	}else{
		sock.response("PRIVMSG "+params["CHANNEL"]+" :"+msg+" ");
	}
	odd=!odd;
XPASS	
}

void action(const string &msg)
{MUTEX_INSPECTOR;
XTRY
	char c01=01;
	string s01;
	s01.assign(c01,1);
	if(odd){
		sock.response("PRIVMSG "+params["CHANNEL"]+" :"+s01+"ACTION "+msg+s01);
	}else{
		sock.response("PRIVMSG "+params["CHANNEL"]+" :"+s01+"ACTION "+msg+" "+s01);
	}
	odd=!odd;
XPASS	
}


c_command parse_string(const string &os)
{MUTEX_INSPECTOR;
	string s=os;
	c_command cmd;
XTRY
	if(!s.size()) return cmd;
	if(s.size()>9){
		if(s.substr(0,9)=="EUI_REPLY"){
		XTRY
			cmd.cmd="EUI_REPLY";
			int pz=s.find(" ",0);
			string ss=s.substr(pz+1,s.size()-pz-1);
			cmd.msg=ss;
			return cmd;
		XPASS	
		}
		if(s.substr(0,10)=="UTTC_REPLY"){
		XTRY
			cmd.cmd="UTTC_REPLY";
			int pz=s.find(" ",0);
			string ss=s.substr(pz+1,s.size()-pz-1);
			cmd.msg=ss;
			return cmd;
		XPASS	
		}
	}
	if(s.substr(0,1)==":"){
	XTRY
		if(s.find("!",0)!=-1 && s.find("@",0)!=-1){
			int pfn=-1;
			pfn=s.find("!");
			int pu=-1;
			if(pfn!=-1) pu=s.find("@",pfn+1);
			int ph=-1;
			if(pu!=-1) ph=s.find(" ",pu+1);
			int pc=-1;
			if(ph!=-1) pc=s.find(" ",ph+1);
			int pai=-1;
			if(pc!=-1) pai=s.find(":",pc+1);
			cmd.fromnick=s.substr(1,pfn-1);
			//cmd.user=s.substr(pfn+1,pu-pfn-1);
			cmd.host=s.substr(pu+1,ph-pu-1);
			if(pc!=-1){
			XTRY
				
				cmd.cmd=strupper(s.substr(ph+1,pc-ph-1));
				if(pai!=-1){
				XTRY
					cmd.msg=s.substr(pai+1,s.size()-pai-1);
				XPASS	
				}
				
				if(cmd.cmd=="KICK" && pai!=-1){
				XTRY
					int zpz=s.find(" ",pc+1);
					if(zpz!=-1) cmd.tonick=s.substr(zpz+1,pai-zpz-2);
				XPASS	
				}
				if(cmd.cmd=="PRIVMSG" || cmd.cmd=="MODE"){
				XTRY
					int cc=s.find(" ",pc+1);
					if(cc!=-1) cmd.tonick=s.substr(pc+1,cc-pc-1);
				XPASS	
				}
				if(cmd.cmd=="PRIVMSG"){
				XTRY
					if(cmd.msg.size()>6)
					{
					XTRY
					if(cmd.msg.substr(1,6)=="ACTION"){
					XTRY
						cmd.msg=cmd.msg.substr(8,cmd.msg.size()-9);
					XPASS	
					}
					XPASS
					}
					int l=params["NICKNAME"].size();
					{
					XTRY
					if(cmd.msg.size()>=l+2){
					XTRY
						if(cmd.msg.substr(1,l)==params["NICKNAME"]){
						XTRY
							if(cmd.msg.size()>l+2)
							{
							cmd.msg=cmd.msg.substr(l+3,cmd.msg.size()-l-4);
							}
							cmd.ppriv=true;
							
						XPASS	
						}
					XPASS	
					}
					XPASS
					}
				XPASS	
				}
			XPASS	
			}
			
		}else{
		
		}
	XPASS
	
	}
XPASS
	return cmd;
}


c_socket sock;

struct hostent *host_info;


void connect_to_server()
{MUTEX_INSPECTOR;
XTRY
	if(sock.sock==-1){
		sockaddr_in serv_addr;
		if ((host_info = gethostbyname(params["HOST_TO_CONNECT"].c_str()))){
			bcopy(host_info->h_addr, (char *)&serv_addr.sin_addr,host_info->h_length);
		}else{
			logErr("---Error: unable resolve host name %s",params["HOST_TO_CONNECT"].c_str());
		}
//		serv_addr.sin_addr.s_addr = inet_addr(params["IP_TO_CONNECT"].c_str());
		serv_addr.sin_port = htons(atoi(params["PORT_TO_CONNECT"]));
		serv_addr.sin_family = AF_INET;
		int sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sock.sock=sk;
		sock.connect((sockaddr*) &serv_addr,sizeof(serv_addr));
	}
XPASS	
}

