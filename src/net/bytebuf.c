#include <net/bytebuf.h>
#include <string.h>

#define SEGMENT_BIT 0x7F
#define CONTINUE_BIT 0x80

bool net_create_bytebuf(bytebuf_t* buf, size_t size){
    buf->data = (uint8_t*)calloc(size, sizeof(uint8_t));
    if (buf->data == NULL){
        DEBUG_FAIL("failure to allocate memory of size %zu\n", size * sizeof(uint8_t));
        buf->error = true;
        return false;
    }
    else{
        buf->size = size;
        buf->readIndex = 0;
        buf->writeIndex = 0;
        buf->error = false;
    }
    return true;
}

void net_destroy_bytebuf(bytebuf_t* buf){
    if(buf->data != NULL){
        free((void*)buf->data);
        buf->data = NULL;
    }

    buf->size = 0;
    buf->readIndex = 0;
    buf->writeIndex = 0;
    buf->error = false;
}


uint8_t net_read_byte_from_bytebuf(bytebuf_t* buf){
    if (buf->readIndex >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->readIndex, buf->size);
        buf->error = true;
        return 0;
    }
    else{
        if (buf->readIndex > buf->writeIndex){
            DEBUG_WARNING("warning! read index (%zu) is larger then write index (%zu), getting invalid data\n", buf->readIndex, buf->writeIndex);
        }
        uint8_t byte = buf->data[buf->readIndex];
        buf->readIndex++;
        return byte;
    }
}

void net_write_byte_to_bytebuf(bytebuf_t* buf, uint8_t byte){
    if (buf->writeIndex >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->writeIndex, buf->size);
    }
    else{
        buf->data[buf->writeIndex] = byte;
        buf->writeIndex++;
    }
}


uint16_t net_read_u16_from_bytebuf(bytebuf_t* buf){
    if (buf->readIndex + 1 >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->readIndex, buf->size);
        buf->error = true;
        return 0;
    }
    else{
        if (buf->readIndex + 1 > buf->writeIndex){
            DEBUG_WARNING("warning! read index (%zu) is larger then write index (%zu), getting invalid data\n", buf->readIndex, buf->writeIndex);
        }
        uint16_t value = ((uint64_t)buf->data[buf->readIndex + 1]) | ((uint64_t)buf->data[buf->readIndex] << 8);
        buf->readIndex += 2;
        return value;
    }
}

void net_write_u16_to_bytebuf(bytebuf_t* buf, uint16_t value){
    if (buf->writeIndex + 1 >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->writeIndex, buf->size);
        buf->error = true;
    }
    else{
        buf->data[buf->writeIndex + 1] = ((uint8_t)value);
        buf->data[buf->writeIndex] = ((uint8_t)(value >> 8));
        buf->writeIndex += 2;
    }
}

uint32_t net_read_u32_from_bytebuf(bytebuf_t* buf){
    if (buf->readIndex + 3 >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->readIndex, buf->size);
        buf->error = true;
        return 0;
    }
    else{
        if (buf->readIndex + 3 > buf->writeIndex){
            DEBUG_WARNING("warning! read index (%zu) is larger then write index (%zu), getting invalid data\n", buf->readIndex, buf->writeIndex);
        }
        uint32_t value = ((uint32_t)buf->data[buf->readIndex + 3]) | ((uint32_t)buf->data[buf->readIndex + 2] << 8)
            | ((uint32_t)buf->data[buf->readIndex + 1] << 16) | ((uint32_t)buf->data[buf->readIndex] << 24);
        buf->readIndex += 4;
        return value;
    }
}

void net_write_u32_to_bytebuf(bytebuf_t* buf, uint32_t value){
    if (buf->writeIndex + 3 >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->writeIndex, buf->size);
        buf->error = true;
    }
    else{
        buf->data[buf->writeIndex + 3] = ((uint8_t)value);
        buf->data[buf->writeIndex + 2] = ((uint8_t)(value >> 8));
        buf->data[buf->writeIndex + 1] = ((uint8_t)(value >> 16));
        buf->data[buf->writeIndex] = ((uint8_t)(value >> 24));
        buf->writeIndex += 4;
    }
}

uint64_t net_read_u64_from_bytebuf(bytebuf_t* buf){
    if (buf->readIndex + 7 >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->readIndex, buf->size);
        buf->error = true;
        return 0;
    }
    else {
        if (buf->readIndex + 7 > buf->writeIndex){
            DEBUG_WARNING("warning! read index (%zu) is larger then write index (%zu), getting invalid data\n", buf->readIndex, buf->writeIndex);
        }
        uint64_t value = ((uint64_t)buf->data[buf->readIndex + 7]) | ((uint64_t)buf->data[buf->readIndex + 6] << 8)
            | ((uint64_t)buf->data[buf->readIndex + 5] << 16) | ((uint64_t)buf->data[buf->readIndex + 4] << 24)
            | ((uint64_t)buf->data[buf->readIndex + 3] << 32) | ((uint64_t)buf->data[buf->readIndex + 2] << 40)
            | ((uint64_t)buf->data[buf->readIndex + 1] << 48) | ((uint64_t)buf->data[buf->readIndex + 0] << 56);
        buf->readIndex += 8;
        return value;
    }
}

void net_write_u64_to_bytebuf(bytebuf_t* buf, uint64_t value){
    if (buf->writeIndex + 7 >= buf->size){
        DEBUG_FAIL("buffer overflow on %zu (max was %zu)\n", buf->writeIndex, buf->size);
        buf->error = true;
    }
    else {
        buf->data[buf->writeIndex + 7] = ((uint8_t)value);
        buf->data[buf->writeIndex + 6] = ((uint8_t)(value >> 8));
        buf->data[buf->writeIndex + 5] = ((uint8_t)(value >> 16));
        buf->data[buf->writeIndex + 4] = ((uint8_t)(value >> 24));
        buf->data[buf->writeIndex + 3] = ((uint8_t)(value >> 32));
        buf->data[buf->writeIndex + 2] = ((uint8_t)(value >> 40));
        buf->data[buf->writeIndex + 1] = ((uint8_t)(value >> 48));
        buf->data[buf->writeIndex] = ((uint8_t)(value >> 56));
        buf->writeIndex += 8;
    }
}

int32_t net_read_varint_from_bytebuf(bytebuf_t* buf){
    int32_t value = 0;

    for (int32_t position = 0; position < 32; position += 7){
        uint8_t currentByte = net_read_byte_from_bytebuf(buf);
        value |= (int32_t)(currentByte & 0x7F) << position;

        if ((currentByte & 0x80) == 0){
            return value;
        }
    }
    DEBUG_WARNING("position overflow on varint, broken packet?\n");
    buf->error = true;
    return 0;
}

void net_write_varint_to_bytebuf(bytebuf_t* buf, int32_t value){
    uint32_t uValue = value;
    while ((uValue & ~0x7F) != 0){
        net_write_byte_to_bytebuf(buf, (uValue & 0x7F) | 0x80);
        uValue >>= 7;
    }
    net_write_byte_to_bytebuf(buf, uValue);
}

int64_t net_read_varlong_from_bytebuf(bytebuf_t* buf){
    int64_t value = 0;

    for (int64_t position = 0; position < 64; position += 7){
        uint8_t currentByte = net_read_byte_from_bytebuf(buf);
        value |= (int64_t)(currentByte & 0x7F) << position;

        if ((currentByte & 0x80) == 0){
            return value;
        }
    }
    DEBUG_WARNING("position overflow on varint, broken packet?\n");
    buf->error = true;
    return 0;
}

void net_write_varlong_to_bytebuf(bytebuf_t* buf, int64_t value){
    uint64_t uValue = value;
    while ((uValue & ~0x7F) != 0){
        net_write_byte_to_bytebuf(buf, (uValue & 0x7F) | 0x80);
        uValue >>= 7;
    }
    net_write_byte_to_bytebuf(buf, uValue);
}

void net_write_cstring_to_bytebuf(bytebuf_t* buf, const char* string, size_t maxLength){
    size_t length = strlen(string);
    if (length >= maxLength) length = maxLength;
    net_write_varint_to_bytebuf(buf, length);
    for (size_t i = 0; i < length; i++){
        net_write_byte_to_bytebuf(buf, (uint8_t)string[i]);
    }
}

int32_t net_get_size_of_varint(int32_t value){
    int32_t size = 1;
    while ((value & ~SEGMENT_BIT) != 0){
        value >>= 7;
        size++;
    }
    return size;
}
