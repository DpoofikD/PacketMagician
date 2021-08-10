#pragma once

#include "PacketMagician.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

typedef struct {
    struct icmp hdr;
    char data[64];
} ICMP_pck;

typedef struct {
    struct ip ip_hdr;
    ICMP_pck pckICMP;
} IP_pck;