#ifndef CSERVERBOUND_H_INCLUDED
#define CSERVERBOUND_H_INCLUDED



#include <vermilion.h>






bool net_sb_handshake(const char* ip, uint16_t port);
bool net_sb_login_start(const char* username);
bool net_sb_keep_alive(int keepAliveId);







#endif
