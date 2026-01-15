#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
static int parse_hex32(const char *s, uint32_t *out)
{
    char *end = NULL;
    errno = 0;
    unsigned long val = strtoul(s, &end, 16);
    if (errno != 0 || end == s || *end != '\0') {
        return -1;
    }
    if (val > 0xFFFFFFFFul) return -1;
    *out = (uint32_t)val;
    return 0;
}
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex32>\nExample: %s C0A80101\n", argv[0], argv[0]);
        return 1;
    }
    uint32_t host_hex = 0;
    if (parse_hex32(argv[1], &host_hex) != 0) {
        fprintf(stderr, "Error: '%s' is not a valid 32-bit hex value.\n", argv[1]);
        return 1;
    }
    uint32_t net_val = htonl(host_hex);
    struct in_addr inaddr;
    inaddr.s_addr = net_val;          
    char buf[INET_ADDRSTRLEN];
    const char *ok = inet_ntop(AF_INET, &inaddr, buf, sizeof(buf));
    if (!ok) {
        perror("inet_ntop");
        return 1;
    } 
    printf("argv[1]        = %s\n", argv[1]);
    printf("inaddr.s_addr  = %u\n", (unsigned)inaddr.s_addr);
    printf("buf (IPv4 str) = %s\n", buf); 
    struct in_addr back;
    if (inet_aton(buf, &back)) {
        printf("back.s_addr    = %u\n", (unsigned)back.s_addr);
        if (back.s_addr == net_val) {
            printf("Round-trip check: OK\n");
        }
    }
	return 0;
}
