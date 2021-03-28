#ifndef COMMAND__H
#define COMMAND__H

#include <string>
#include <misc.h>


void process_command(c_command &cmd);

void process_channel_command(c_command &cmd);
void process_private_command(c_command &cmd);


#endif

