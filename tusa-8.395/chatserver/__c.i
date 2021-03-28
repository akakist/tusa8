OBJDIR=./objD
ROBJDIR=./objR
############# release config
#CFLAGS= -O2  -I/usr/include/mysql -c -I/usr/local/ssl/include -DLINUX -Dchat -Wall -static
#LIBS=../kall/_apache.a ../kall/libperl.a   /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/lib/mysql/libmysqlclient.a  -lpthread -lz -lgdbm -static
#############################


CFLAGS= `cat _CFLAGS_D` -DCOMPILE_TIME=`./gettime.pl` -DSVN_VERSION="\"`svnversion`\""

RCFLAGS= `cat _CFLAGS_R` -DCOMPILE_TIME=`./gettime.pl` -DSVN_VERSION="\"`svnversion`\""
LIBS= -lssl -lcrypto /usr/lib/libmysqlclient.a   -lpthread -lz 
RLIBS= -lssl -lcrypto /usr/lib/libmysqlclient.a -lpthread -lz 


#LIBS_DEP=core-web1/libcoreweb_d.a core1/libcommon_d.a 
#lib-cs/libcs_d.a 
#RLIBS_DEP=core-web1/libcoreweb_r.a core1/libcommon_r.a
#lib-cs/libcs_r.a 
#LIBS= /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/lib/mysql/libmysqlclient.a -lboost_regex-gcc-1_32  -lpthread -lz -lgdbm.a    
#RLIBS= /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/lib/mysql/libmysqlclient.a -lboost_regex-gcc-1_32  -lpthread -lz -lgdbm.a   

#LIBS= /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/lib/mysql/libmysqlclient.a -lboost_regex.a -lpthread  -lgdbm   -static
#RLIBS=  /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/lib/mysql/libmysqlclient.a -lboost_regex.a -lpthread  -lgdbm -static
##########################-lstlport_gcc.a 


EXT=exe
LIBNAME=libchat.a
RLIBNAME=libchatR.a
INC_DIRS= .  ../core ../core-web  ../stdafx data 
TARGETS= CS_tusa.cpp
	
OBJS=	\
	license.cpp	\
	admin.cpp	\
	admin_ban.cpp \
	admin_other.cpp \
	admin_privs.cpp\
	admin_settings.cpp\
	app_con.cpp\
	app_server.cpp \
	channels.cpp \
	chat_colors.cpp\
	chat_config.cpp \
	chat_globals.cpp \
	chat_msg.cpp \
	chat_thread.cpp \
	chat_thread_2.cpp \
	contact_vec.cpp \
	file_dump.cpp\
	ignore_vec.cpp \
	irc_admin.cpp \
	irc_login.cpp \
	irc_server.cpp     \
	irc_server_config.cpp \
	irc_tools.cpp \
	irc_user.cpp \
	levels.cpp	\
	local_user_handle.cpp\
	login.cpp\
	main.cpp  \
	make_user_info.cpp \
	message.cpp \
	nick_vec.cpp\
	notes_containers.cpp   \
	u_chat.cpp \
	update_cmd.cpp \
	url_new_nick.cpp  \
	user.cpp \
	user_credits.cpp \
	user_nick.cpp \
	user_profile.cpp \
	user_state.cpp \
	cutils.cpp \
 	other.cpp \
        notes.cpp\
	irc_user_cred.cpp\
	oscar_client.cpp\
	global_state.cpp\
	admin_old_ban.cpp\
        url_change_nick.cpp\
        url_chat_messages.cpp\
        url_chat_sendmessage.cpp\
        url_ignore_user.cpp\
        url_set_user_status.cpp\
        PTR.cpp _QUERY.cpp __main.cpp _mutex.cpp _mutex1.cpp copy_file_common.cpp core_main.cpp db_config.cpp  db_config2.cpp db_var.cpp\
    des_sid.cpp errs.cpp file_set.cpp file_set_init.cpp mutex_inspector.cpp mysql_extens.cpp mysql_extens2.cpp oscar_buffer.cpp rijndael.cpp\
    server_config.cpp st_FILE.cpp st_rsa.cpp st_savecwd.cpp st_stuff.cpp str_lib.cpp str_lib1.cpp str_lib_extra.cpp wrappers.cpp\
    threadable.cpp st_stream_str.cpp Double.cpp\
    c_conn.cpp html_out.cpp tcp_dialog_server.cpp tcp_oscar_dialog_server.cpp tcp_sender.cpp\
    tcp_server.cpp tcp_server_connection.cpp tcp_stats.cpp thread_info_report.cpp web_adminfo.cpp web_connection.cpp\
	web_cache.cpp    web_connection_ptcp.cpp web_server.cpp web_tools.cpp oscar_client.cpp\
	


#user_profile.cpp foto_profile.cpp 

#	web_adminfo.cpp

#tbl_user.cpp 
#        vote.cpp \
