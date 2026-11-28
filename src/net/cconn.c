#include <net/cconn.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock.h>
#include <io.h>
#define clone _close
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#endif

// TODO: abstract socket functionality in
// future platform abstraction layer so we can support winsock
// (because we want to support older versions of Windows)


struct conn {
    int sock;
    connstate_t state;
};

static struct conn g_conn = {-1, CONNSTATE_HANDSHAKE};


bool net_connect_to_server(const char* ip, uint16_t port){
    #ifdef _WIN32
    WSADATA wsaData = {0};
    int wsaResult = WSAStartup(2, &wsaData);
    if(wsaResult != 0){
        DEBUG_FAIL("winsock initialization failed: %d\n", WSAGetLastError());
        return false;
    }
    #endif
    g_conn.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(g_conn.sock == -1){
        DEBUG_FAIL("couldn't create socket\n");
        #ifdef _WIN32
        DEBUG_FAIL("winsock error: %d\n", WSAGetLastError());
        WSACleanup();
        #endif
        return false;
    }
    struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(port)};
    int result = inet_pton(AF_INET, ip, &sa.sin_addr);
    if(result != 1){
        DEBUG_FAIL("couldn't convert to valid IPv4 address\n");
        close(g_conn.sock);
        g_conn.sock = -1;
        #ifdef _WIN32
        DEBUG_FAIL("winsock error: %d\n", WSAGetLastError());
        WSACleanup();
        #endif
        return false;
    }
    result = connect(g_conn.sock, (struct sockaddr*)&sa, sizeof(sa));
    if(result == -1){
        DEBUG_FAIL("couldn't connect\n");
        close(g_conn.sock);
        #ifdef _WIN32
        DEBUG_FAIL("winsock error: %d\n", WSAGetLastError());
        WSACleanup();
        #endif
        return false;
    }
    return true;
}

void net_kill_connection_to_server(){
    if(g_conn.sock != -1){
        close(g_conn.sock);
        #ifdef _WIN32
        WSACleanup();
        #endif
        g_conn.sock = -1;
    } else DEBUG_WARNING("trying to kill connection to server when we aren't connected?\n");
}

void net_change_state(connstate_t state){
    g_conn.state = state;
}

bool net_send_packet(bytebuf_t* buf){
    size_t sent = 0;
    DEBUG_INFO("sending packet!\n");
    do {
        ssize_t s = send(g_conn.sock, (const char*)buf->data + sent, (buf->size - sent), 0);
        if(s == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                DEBUG_FAIL("EAGAIN || EBLOCK\n");
                return false;
            } else {
                DEBUG_FAIL("error sending packet\n");
                return false;
            }
        }
        sent += s;
    } while(sent < buf->size);
    return true;
}

// you must pre-setup the bytebuf for this to work
bool net_receive_packet_raw(bytebuf_t* buf){
    size_t readFrom = 0;
    do {
        ssize_t s = recv(g_conn.sock, (char*)buf->data + readFrom, buf->size - readFrom, 0);
        if(s == -1){
            DEBUG_FAIL("couldn't receive packet\n");
            return false;
        }
        readFrom += s;
        buf->writeIndex += s;
    } while(readFrom < buf->size);
    return true;
}

uint8_t net_read_byte_from_server(bool* result){
    uint8_t data[1] = {0};
    bytebuf_t buf;
    buf.data = data;
    buf.size = 1;
    *result = net_receive_packet_raw(&buf);
    return data[0];
}

int32_t net_read_varint_from_server(bool* result){
    int32_t value = 0;

    for (int32_t position = 0; position < 32; position += 7){
        uint8_t currentByte = net_read_byte_from_server(result);
        if(result == false){
            return 0;
        }
        value |= (int32_t)(currentByte & 0x7F) << position;

        if ((currentByte & 0x80) == 0){
            return value;
        }
    }
    DEBUG_WARNING("position overflow on varint, broken packet?\n");
    *result = false;
    return 0;
}


bool net_receive_packet(bytebuf_t* buf){
    bool result = true;
    int length = net_read_varint_from_server(&result);
    if(result == false){
        DEBUG_FAIL("couldn't read length from server\n");
        return false;
    }
    if(length >= 2000000){
        DEBUG_FAIL("length is too large (returned %d)\n", length);
        return false;
    }
    //DEBUG_INFO("receiving packet with length of %d\n", length);
    net_create_bytebuf(buf, length);
    result = net_receive_packet_raw(buf);
    if(result == false){
        net_destroy_bytebuf(buf);
        DEBUG_FAIL("couldn't read packet\n");
        return false;
    }
    return true;
}
