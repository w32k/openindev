#ifndef BYTEBUF_H_INCLUDED
#define BYTEBUF_H_INCLUDED







#include <vermilion.h>




typedef struct bytebuf {
    size_t size;
    size_t readIndex;
    size_t writeIndex;
    uint8_t* data;
    bool error;
} bytebuf_t;

bool net_create_bytebuf(bytebuf_t* buf, size_t size);
void net_destroy_bytebuf(bytebuf_t* buf);
uint8_t net_read_byte_from_bytebuf(bytebuf_t* buf);
void net_write_byte_to_bytebuf(bytebuf_t* buf, uint8_t byte);
uint16_t net_read_u16_from_bytebuf(bytebuf_t* buf);
void net_write_u16_to_bytebuf(bytebuf_t* buf, uint16_t value);
uint32_t net_read_u32_from_bytebuf(bytebuf_t* buf);
void net_write_u32_to_bytebuf(bytebuf_t* buf, uint32_t value);
uint64_t net_read_u64_from_bytebuf(bytebuf_t* buf);
void net_write_u64_to_bytebuf(bytebuf_t* buf, uint64_t value);
int32_t net_read_varint_from_bytebuf(bytebuf_t* buf);
void net_write_varint_to_bytebuf(bytebuf_t* buf, int32_t value);
int64_t net_read_varlong_from_bytebuf(bytebuf_t* buf);
void net_write_varlong_to_bytebuf(bytebuf_t* buf, int64_t value);
void net_write_cstring_to_bytebuf(bytebuf_t* buf, const char* string, size_t maxLength);
int32_t net_get_size_of_varint(int32_t value);

#endif
