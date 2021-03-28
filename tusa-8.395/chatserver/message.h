#ifndef CHAT_MESSAGE_H__
#define CHAT_MESSAGE_H__
#include "_mutex.h"
#include <time.h>
#include <vector>
#include <map>
#include "st_stuff.h"
#include <list>
#include <string>
#include "PTR.h"
#include "user.h"
#include "S_id.h"
#include "DB_id.h"
#include "CH_id.h"
#include "chat_channel_data.h"
#define MSG_VER		4


void chat_channels_backup_messages();



struct user_pair;

class chat_message;



class chat_user;
class chat_channel;



class chat_channel: public base_container
{
public:
    const char* getclassname()const {
        return "chat_channel";
    }

    chat_channel_data cd;
    chat_channel();
    ~chat_channel();

};






#endif
