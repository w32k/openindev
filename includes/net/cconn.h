#ifndef CLIENTCONN_H_INCLUDED
#define CLIENTCONN_H_INCLUDED







#include <vermilion.h>
#include <net/bytebuf.h>



typedef enum connstate {
    CONNSTATE_HANDSHAKE,
    CONNSTATE_LOGIN,
    CONNSTATE_PLAY,
    CONNSTATE_STATUS
} connstate_t;



bool net_connect_to_server(const char* ip, uint16_t port);
void net_kill_connection_to_server();
bool net_send_packet(bytebuf_t* buf);
bool net_receive_packet_raw(bytebuf_t* buf);
uint8_t net_read_byte_from_server(bool* result);
int32_t net_read_varint_from_server(bool* result);
bool net_receive_packet(bytebuf_t* buf);












#endif
