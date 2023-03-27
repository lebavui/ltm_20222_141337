#include <stdio.h>

int main()
{
    char packet_bytes[] = {
        0x45, 0x00, 0x01, 0x8e, 0x9c, 0x2e, 0x40, 0x00,
        0x40, 0x06, 0x64, 0x98, 0x0a, 0x0d, 0x38, 0x6a,
        0x6b, 0x16, 0x8b, 0x16
    };

    char b = packet_bytes[0];
    char ver = b >> 4;
    printf("version: %d\n", ver);

    char ihl = (b & 0x0f) * 4;
    printf("ihl = %d\n", ihl);

    unsigned int tl = packet_bytes[2] * 256 + packet_bytes[3];
    printf("tl = %d\n", tl);

    printf("IP nguon: %u.%u.%u.%u\n", 
        (unsigned char)packet_bytes[12], 
        (unsigned char)packet_bytes[13], 
        (unsigned char)packet_bytes[14], 
        (unsigned char)packet_bytes[15]);

    printf("IP dich: %u.%u.%u.%u\n", 
        (unsigned char)packet_bytes[16], 
        (unsigned char)packet_bytes[17], 
        (unsigned char)packet_bytes[18], 
        (unsigned char)packet_bytes[19]);
}