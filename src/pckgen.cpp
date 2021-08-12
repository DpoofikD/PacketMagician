#include "pckgen.h"

static unsigned short cksum(void* b, int len) {

    unsigned short* buf = static_cast<unsigned short*>(b);
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}
static unsigned short cksum(void* b, int len);

static void prepare_icmp(ICMP_pck* pckICMP) {
    memset(pckICMP, 0, sizeof(ICMP_pck));
    int i = 0;
    for (; i < sizeof(pckICMP->data) - 1; ++i)
        pckICMP->data[i] = i + 'a';

    pckICMP->data[i] = 0;
    srand(time(NULL));
    pckICMP->hdr.icmp_type = ICMP_ECHO;
    pckICMP->hdr.icmp_hun.ih_idseq.icd_id = rand();
    pckICMP->hdr.icmp_hun.ih_idseq.icd_seq = 0;
    if (cmd.csum)
        pckICMP->hdr.icmp_cksum = cksum(pckICMP, sizeof(ICMP_pck));
}
static void prepare_icmp(ICMP_pck* pckICMP);

static unsigned int curtime() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    unsigned int time_ms = time.tv_sec * 1000 + (time.tv_nsec / 1000000);
    return time_ms;
}
static unsigned int curtime();