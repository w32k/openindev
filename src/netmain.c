#include <vermilion.h>
#include <string.h>
#include <net/cconn.h>
#include <net/cserverbound.h>


int main(){
    DEBUG_INFO("%s %s (implementing %s)\n", CLIENT_NAME, VER, IMPL_VER);
    const char* ip = "127.0.0.1";
    const uint16_t port = 25565;
    const char* username = "Sift";

    bool result = net_connect_to_server(ip, port);
    if(result == false){
        return -1;
    }

    result = net_sb_handshake(ip, port);
    if(result == false){
        return -2;
    }
    result = net_sb_login_start(username);
    if(result == false){
        return -3;
    }

    bytebuf_t buf = {0};
    bool running = true;
    while(running){
        result = net_receive_packet(&buf);
        if(result == false){
            return -4;
        }
        int id = net_read_varint_from_bytebuf(&buf);
        if(id == 0x0){
            int keepAliveId = net_read_u32_from_bytebuf(&buf);
            result = net_sb_keep_alive(keepAliveId);
            if(result == false){
                return -5;
            }
        }
        if(id == 0x40){
            DEBUG_INFO("was kicked\n");
            running = false;
        }
        net_destroy_bytebuf(&buf);
    }

    net_kill_connection_to_server();
    return 0;
}
