#include <net/cserverbound.h>
#include <net/cconn.h>
#include <net/bytebuf.h>
#include <string.h>

// TODO: check errors for *every* single bytebuf modification
// is it excessive? maybe, but i want to be safe


bool net_sb_handshake(const char* ip, uint16_t port){
    const int sizeOfIp = strlen(ip);
    bytebuf_t buf = {0};
    size_t size = net_get_size_of_varint(0x00) + net_get_size_of_varint(5) + net_get_size_of_varint(sizeOfIp)
                  + sizeOfIp + sizeof(uint16_t) + net_get_size_of_varint(2);
    net_create_bytebuf(&buf, size + net_get_size_of_varint(size));
    net_write_varint_to_bytebuf(&buf, size);
    net_write_varint_to_bytebuf(&buf, 0x00);
    net_write_varint_to_bytebuf(&buf, 5);
    net_write_cstring_to_bytebuf(&buf, ip, 255);
    net_write_u16_to_bytebuf(&buf, port);
    net_write_varint_to_bytebuf(&buf, 2);
    bool result = net_send_packet(&buf);
    if(result == false){
        return false;
    }
    net_destroy_bytebuf(&buf);
    return true;
}


bool net_sb_login_start(const char* username){
    const int sizeOfUsername = strlen(username);
    bytebuf_t buf = {0};
    size_t size = net_get_size_of_varint(0x00) + net_get_size_of_varint(sizeOfUsername) + sizeOfUsername;
    net_create_bytebuf(&buf, size + net_get_size_of_varint(size));
    net_write_varint_to_bytebuf(&buf, size);
    net_write_varint_to_bytebuf(&buf, 0x00);
    net_write_cstring_to_bytebuf(&buf, username, 16);
    bool result = net_send_packet(&buf);
    if(result == false){
        return false;
    }
    net_destroy_bytebuf(&buf);
    return true;
}


bool net_sb_keep_alive(int keepAliveId){
    DEBUG_INFO("keeping alive..\n");
    bytebuf_t buf = {0};
    size_t size = net_get_size_of_varint(0x00) + sizeof(uint32_t);
    net_create_bytebuf(&buf, size + net_get_size_of_varint(size));
    net_write_varint_to_bytebuf(&buf, size);
    net_write_varint_to_bytebuf(&buf, 0x00);
    net_write_u32_to_bytebuf(&buf, keepAliveId);
    bool result = net_send_packet(&buf);
    if(result == false){
        return false;
    }
    net_destroy_bytebuf(&buf);
    return true;
}
