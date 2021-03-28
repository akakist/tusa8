OBJDIR=/tmp/obj/chat/debugN/core
ROBJDIR=/tmp/obj/chat/releaseN/core

CFLAGS=   -O0 \
	-I/usr/local/include/stlport\ 
	-Wall  \
	-I/usr/local/include/mysql  -c   \
	-I/usr/local/ssl/include  \
	-DLINUX  -ftemplate-depth-30  -fmemoize-lookups -DDEBUG
	
RCFLAGS=  -O3 \
	-I/usr/local/include/stlport\ 
	-Wall \
	-I/usr/local/include/mysql  -c   \
	-I/usr/local/ssl/include  \
	-DLINUX  -ftemplate-depth-30  



LIBS=/usr/local/lib/libstlport_gcc.a  /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/local/lib/mysql/libmysqlclient.a /usr/local/lib/libboost_regex.a -lpthread -lz -lgdbm -shared
RLIBS=/usr/local/lib/libstlport_gcc.a  /usr/local/ssl/lib/libssl.a /usr/local/ssl/lib/libcrypto.a /usr/local/lib/mysql/libmysqlclient.a /usr/local/lib/libboost_regex.a -lpthread -lz -lgdbm -shared


LIBNAME=libcoreD.a
RLIBNAME=libcore.a
INC_DIRS= . ../core_web

#. ../core ../core_web ../udp ../zdes ../kall BX tracker utils _lib _icq

TARGETS=	dump_stream.cpp

# utils/bx_update_shows.cpp utils/bx_update_clicks.cpp

#			 web_server.cpp \
			 web_adminfo.cpp \
			 web_connection.cpp \
			 tcp_server_connection.cpp \
			 tcp_server.cpp \
			 web_connection_ptcp.cpp \
		 web_user.cpp  \
	     web_tools.cpp\
			 webusers.cpp\
	    _mutex.cpp\
		   _QUERY.cpp\
		    c_conn.cpp\
			 errs.cpp \
			 str_lib.cpp \
			 st_stuff.cpp \
		 server_pem.cpp	\
			 mysql_extens.cpp \
			file_set.cpp\
			file_set_init.cpp\
			load_config.cpp\
    mutex_inspector.cpp\
			get_opts.cpp\
			des_sid.cpp\
			st_stream_str.cpp st_rsa.cpp wrappers.cpp \
	 license.cpp copy_file_common.cpp\
	  PTR.cpp  Mtree.cpp st_FILE.cpp st_savecwd.cpp\


OBJS=PTR.cpp\
	_QUERY.cpp\
	_mutex.cpp\
	c_conn.cpp\
	copy_file_common.cpp\
	core_main.cpp\
	db_var.cpp\
	des_sid.cpp\
	errs.cpp\
	file_set.cpp\
	file_set_init.cpp\
	get_opts.cpp\
	license.cpp\
	load_config.cpp\
	m_containers.cpp\
	mutex_inspector.cpp\
	my_gdbm.cpp\
	mysql_extens.cpp\
	server_pem.cpp\
	st_FILE.cpp\
	st_gdbm.cpp\
	st_gzFile.cpp\
	st_rsa.cpp\
	st_savecwd.cpp\
	st_stream_str.cpp\
	st_stuff.cpp\
	str_lib.cpp\
	udp_conn.cpp\
	wrappers.cpp\
	