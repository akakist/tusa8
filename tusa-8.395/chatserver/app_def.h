#ifndef APP_DEF_H
#define APP_DEF_H

#ifdef __WITH_LICENSE
#define LICENSE_CK(aaa) 		\
		if(local_users.size$()>10)\
		{ \
			if(rand()%5==0)\
			{\
			check_connection_for_license(cc->headers["HOST"],cc->peer_ipaddress,false);\
			if(local_users.size$()>=(unsigned)max_users_in_license()) throw cError(__S8);\
			}\
		}
#define LICENSE_CKNH(aaa) 		\
		if(local_users.size$()>10)\
		{ \
			check_connection_for_license_nohost(false);\
			if(local_users.size$()>=(unsigned)max_users_in_license()) throw cError(__S8);\
			}\
		}



#else
#define LICENSE_CK(aaa)
#endif

#endif
