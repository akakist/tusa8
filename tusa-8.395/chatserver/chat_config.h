#ifndef chatWEBSERVER_CONFIGH
#define chatWEBSERVER_CONFIGH
using namespace std;
#include "M_MAP.h"
#include "M_SET.h"
#include "m_var.h"
#include "file_set.h"
#include "Double.h"

struct __chat_config
{

    m_var<unsigned int>chat_offline_timeout_sec;
    m_var<unsigned int> data_drop_timeout_sec;
    m_var<unsigned int>user_max_inactive_time_sec;
    m_var<string>time_format;
    m_var<string> bad_words;
    m_var<string> to_all;
    m_var<string> php_session_prefix;
    m_var<unsigned int>change_nick_timeout_sec;
    m_var<unsigned int> system_note_uid;
    m_var<unsigned int> iframe_buffer_max_size;

    m_var<unsigned int> max_msg_per_period;
    m_var<unsigned int> max_msg_period_sec;

    void load_config(const string&pn);

    __chat_config();
};
extern __chat_config chat_config;

#endif
