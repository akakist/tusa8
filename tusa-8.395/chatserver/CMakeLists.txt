cmake_minimum_required (VERSION 2.6)

add_executable(chatserver

CH_id.h
S_id.h
admin.cpp
admin.h
admin_ban.cpp
admin_old_ban.cpp
admin_other.cpp
admin_privs.cpp
admin_settings.cpp
app_con.cpp
app_con.h
app_def.h
app_server.cpp
app_server.h
channels.cpp
channels.h
chat_channel_data.h
chat_colors.cpp
chat_colors.h
chat_config.cpp
chat_config.h
chat_def.h
chat_globals.cpp
chat_globals.h
chat_msg.cpp
chat_msg.h
chat_stuff.h
chat_thread.cpp
chat_thread_2.cpp
chat_thread_event.h
chatserver.cpp
cm_container.h
config.cpp
contact_vec.cpp
contact_vec.h
cutils.cpp
cutils.h
file_dump.cpp
global_state.cpp
global_state.h
ignore_vec.cpp
ignore_vec.h
irc_admin.cpp
irc_login.cpp
irc_server.cpp
irc_server.h
irc_server_config.cpp
irc_server_config.h
irc_session.h
irc_tools.cpp
irc_user.cpp
irc_user_cred.cpp
irc_utils.h
levels.cpp
levels.h
local_user_handle.cpp
login.cpp
main.cpp
make_user_info.cpp
message.cpp
message.h
nick_vec.cpp
nick_vec.h
notes.cpp
notes.h
notes_containers.cpp
other.cpp
u_chat.cpp
update_cmd.cpp
update_cmd.h
url_change_nick.cpp
url_chat_messages.cpp
url_chat_sendmessage.cpp
url_ignore_user.cpp
url_new_nick.cpp
url_set_user_status.cpp
user.cpp
user.h
user_credits.cpp
user_credits.h
user_nick.cpp
user_nick.h
user_profile.cpp
user_profile.h
user_state.cpp
user_state.h
user_status.h
version.h


)
include_directories(../core ../core-web)

target_link_libraries(chatserver core core_web mysqlclient crypto z  pthread dl)

install(TARGETS chatserver DESTINATION ${BIN_TARGET_DIR})

