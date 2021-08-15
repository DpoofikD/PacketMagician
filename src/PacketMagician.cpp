#include "PacketMagician.h"

unsigned int reply_global = 0;

using namespace clipp; using std::cout;

int TCP (long long pck_amount, unsigned int delay, long long time, struct iphdr *iph, struct tcphdr *tcph, struct sockaddr_in sin, struct TCP_header psh, char datagram[], char source_ip[], char *data , char *pseudogram) {
	int TCP_RS = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
	
	if(TCP_RS == -1) {
		perror("Failed to create socket");
		exit(1);
	}
	
	int psize = sizeof(struct TCP_header) + sizeof(struct tcphdr) + strlen(data);
	pseudogram = (char*)malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct TCP_header));
	memcpy(pseudogram + sizeof(struct TCP_header) , tcph , sizeof(struct tcphdr) + strlen(data));
	
	tcph->check = cksum( (unsigned short*) pseudogram , psize);
	int one = 1;
	const int *val = &one;
	if (setsockopt (TCP_RS, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
		perror("Error setting IP_HDRINCL");
		exit(0);
	}
	
	if (cmd.pck_amount >= 0) {
		for (int i = 0; i < cmd.pck_amount; i++) {
			if (sendto (TCP_RS, datagram, iph->tot_len ,	0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
					perror("sendto failed");
			else {
				printf ("Packet Send. Length : %d \n" , iph->tot_len);
				usleep(cmd.delay * microsecond);
			}
		}
		cout << "Complete!\n";
		return 0;
	}
	else if (time >= 0 && pck_amount == -1) {
		unsigned int start_time = curtime();
		unsigned int now = curtime();
		while ((now - start_time) < (cmd.time * 1000)) {
			now = curtime();
			if (sendto (TCP_RS, datagram, iph->tot_len ,	0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
					perror("sendto failed");
			else {
				printf ("Packet Send. Length : %d \n" , iph->tot_len);
				usleep(cmd.delay * microsecond);
			}
		}
		cout << "Complete!\n";
		return 0;
	}
	else if (time == -1 && pck_amount == -1) {
		while (true) {
			if (sendto (TCP_RS, datagram, iph->tot_len ,	0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
				perror("sendto failed");
			else {
				printf ("Packet Send. Length : %d \n" , iph->tot_len);
				usleep(cmd.delay * microsecond);
			}
		}
	}
	
	return 0;
}
int TCP (long long pck_amount, unsigned int delay, long long time, struct iphdr *iph, struct tcphdr *tcph, struct sockaddr_in sin, struct TCP_header psh, char datagram[], char source_ip[], char *data , char *pseudogram);

int UDP (const char* payload, char target_ip[], int target_port) {
	sockaddr_in kansei_dorifto;
	const int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock < 0)
		return -1;
	bzero(&kansei_dorifto,sizeof(kansei_dorifto));
    kansei_dorifto.sin_family = AF_INET;
    kansei_dorifto.sin_addr.s_addr = inet_addr(target_ip);
    kansei_dorifto.sin_port = htons(target_port);
	if (sendto (udp_sock, payload, strlen(payload) + 1, 0, (sockaddr*)&kansei_dorifto, sizeof(kansei_dorifto)) < 0)
		return 1;
	close (udp_sock);
	return 0;
}
int UDP (const char* payload, char target_ip[], char target_port[]);

int ICMP (const char* ip, unsigned int timeout) {
    if (ip == NULL || timeout == 0)
        return -1;

    ICMP_pck pckICMP;
    prepare_icmp(&pckICMP);
    const short reply_id = pckICMP.hdr.icmp_hun.ih_idseq.icd_id;

    struct sockaddr_in to_addr;
    to_addr.sin_family = AF_INET;
    if (!inet_aton(ip, (struct in_addr*)&to_addr.sin_addr.s_addr)) {
        printf("inet_aton\n");
        return -1;
    }

    if (!strcmp(ip, "255.255.255.255") || to_addr.sin_addr.s_addr == 0xFFFFFFFF)
        return -1;

    const int rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (rawsock < 0) {
        printf("socket() %s\n", strerror(errno));
        return -1;
    }

    const unsigned int timestart = curtime();
    const socklen_t socklen = sizeof(struct sockaddr_in);
    if (sendto(rawsock, &pckICMP, sizeof(ICMP_pck), 0, (struct sockaddr*)&to_addr, socklen) <= 0) {
        close(rawsock);
        return -1;
    }

    int result = -1;
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    for (;;) {
        fd_set rfd;
        FD_ZERO(&rfd);
        FD_SET(rawsock, &rfd);
        int n = select(rawsock + 1, &rfd, 0, 0, &tv);
        if (n == 0) {
            result = 1;
            break;
        }
        if (n < 0)
            break;
        const unsigned int totaltime = curtime() - timestart;
        if (totaltime > timeout) {
            result = 1;
            break;
        }
        else {
            const int new_timeout = timeout - totaltime;
            tv.tv_sec = new_timeout / 1000;
            tv.tv_usec = (new_timeout % 1000) * 1000;
        }

        if (FD_ISSET(rawsock, &rfd)) {
            IP_pck ip_pkt;
            struct sockaddr_in from_addr;
            socklen_t socklen = sizeof(struct sockaddr_in);
            if (recvfrom(rawsock, &ip_pkt, sizeof(IP_pck), 0, (struct sockaddr*)&from_addr, &socklen) <= 0) {
                break;
            }
            if (to_addr.sin_addr.s_addr == from_addr.sin_addr.s_addr && reply_id == ip_pkt.pckICMP.hdr.icmp_hun.ih_idseq.icd_id) {
                    reply_global = totaltime;
                	result = 0;
               		break;
            }
        }
    }
    close(rawsock);
    return result;
}
int ICMP(const char* ip, unsigned int timeout);

int main(int argc, char *argv[]) {
	auto cli = (
		option("-h", "--help").set(cmd.help, true),
		option("--usage").set(cmd.usage, true),
		option("--protocol") & value("icmp/tcp (icmp)", cmd.protocol),
		option("--tip") & value("target ip (192.168.0.1)", cmd.target_ip),
		option("--sip") & value("source ip (192.168.0.2)", cmd.source_ip),
		option("--tp") & value("target port (80)", cmd.target_port),
		option("--sp") & value("source port (15243)", cmd.source_port),
		option("--ttl") & value("ttl of packet (255)", cmd.ttl),
		option("--syn") & value("0/1 (0)", cmd.syn),
		option("--fin") & value("0/1 (0)", cmd.fin),
		option("--rst") & value("0/1 (0)", cmd.rst),
		option("--psh") & value("0/1 (0)", cmd.psh),
		option("--ack") & value("0/1 (0)", cmd.ack),
		option("--urg") & value("0/1 (0)", cmd.urg),
		option("--w-size") & value("window size", cmd.window_size),
		option("-t", "--time") & value("seconds/-1", cmd.time),
		option("--tout", "--timeout") & value("ms (100)", cmd.timeout),
		option("-d", "--delay") & value("ms (0)", cmd.delay),
		option("--cksum") & value("true/false (true)", cmd.csum),
		option("-p", "--payload") & value("payload string", cmd.payload),
		option("--pck-amount") & value("packets amount/-1", cmd.pck_amount) );
	parse(argc, argv, cli);
	if (cmd.usage) {
		cout << usage_lines(cli) << "\n";
		return 0;
	}
	if (cmd.help) {
		cout << "-h, --help - shows this message\n" \
		<< "--usage - shows the usage message\n" \
		<< "--protocol - which protocol would you like to use (currently icmp and tcp are avaliable)\n" \
		<< "--tip - Target IP\n" \
		<< "--sip - Source IP\n" \
		<< "--tp - Target Port\n" \
		<< "--sp - Source Port\n" \
		<< "--ttl - TTL of a packet\n" \
		<< "--syn - SYN flag\n" \
		<< "--fin - FIN flag\n" \
		<< "--rst - RST flag\n" \
		<< "--psh - PSH flag\n" \
		<< "--ack - ACK flag\n" \
		<< "--urg - URG flag\n" \
		<< "--w-size - Window size\n" \
		<< "-t, --time - Time to send packets\n" \
		<< "--pck-amount - Amount of packets to send\n" \
		<< "--tout, --timeout - Time to wait for response\n" \
		<< "-d, --delay - Delay between packets\n" \
		<< "--cksum - Whether to calculate the checksum or not\n" \
		<< "-p, --payload - Exact payload to send\n";
		return 0;
	}
	char* ip_chars_tar = (char*) cmd.target_ip.c_str();
	const char* ip_chars_source = cmd.source_ip.c_str();
	const char* payload_chars = cmd.payload.c_str();

	if (cmd.protocol == "icmp") {
		if (cmd.pck_amount >= 0) {
			for (int i = 0; i < cmd.pck_amount; i++) {
				int result = ICMP(ip_chars_tar, cmd.timeout);
				cout << "Querry... "; 
				switch (result) {
					case 0:
						cout << "OK! Answered in " << reply_global << " ms!\n";
						break;
					case 1:
						cout << "Timeout.\n";
						break;
					case -1:
						cout << "Check your args!\n";
						return 0;
						break;
				}
				usleep(cmd.delay * microsecond);
			}
			cout << "Complete successful!\n";
			return 0;
		}
		else if (cmd.time >= 0 && cmd.pck_amount == -1) {
			unsigned int start_time = curtime();
			unsigned int now = curtime();
			while ((now - start_time) < (cmd.time * 1000)) {
				cout << "Querry... "; 
				now = curtime();
				int result = ICMP(ip_chars_tar, cmd.timeout);
				switch (result) {
					case 0:
						cout << "OK! Answered in " << reply_global << " ms!\n";
						break;
					case 1:
						cout << "Timeout.\n";
						break;
					case -1:
						cout << "Check your args!\n";
						return 0;
						break;
				}
				usleep(cmd.delay * microsecond);
			}
			cout << "Complete successful!\n";
			return 0;
		}
		else if (cmd.time == -1 && cmd.pck_amount == -1) {
			while (true) {
				cout << "Querry... "; 
				int result = ICMP(ip_chars_tar, cmd.timeout);
				switch (result) {
					case 0:
						cout << "OK! Answered in " << reply_global << " ms!\n";
						break;
					case 1:
						cout << "Timeout.\n";
						break;
					case -1:
						cout << "Check your args!\n";
						return 0;
						break;
				}
				usleep(cmd.delay * microsecond);
			}
		}
		else {
			cout<<"Check your args!\n";
			return 0;
		}
	}
	// Now, water can flow, or it can crash... Be water, my friend. 
	else if (cmd.protocol == "tcp") {
		srand(time(NULL));
		char datagram[4096] , source_ip[32] , *data , *pseudogram;
		memset (datagram, 0, 4096);
		struct iphdr *iph = (struct iphdr *) datagram;
		struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
		struct sockaddr_in sin;
		struct TCP_header psh;
		data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
		strcpy(data, payload_chars);
		strcpy(source_ip , ip_chars_source);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(cmd.target_port);
		sin.sin_addr.s_addr = inet_addr(ip_chars_tar);
		iph->ihl = 5;
		iph->version = 4;
		iph->tos = 0;
		iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
		iph->id = htonl(rand());
		iph->frag_off = 0;
		iph->ttl = 255;
		iph->protocol = IPPROTO_TCP;
		iph->check = 0;
		iph->saddr = inet_addr ( source_ip );
		iph->daddr = sin.sin_addr.s_addr;
		if (cmd.csum)
			iph->check = cksum ((unsigned short *) datagram, iph->tot_len);
		tcph->source = htons (cmd.source_port);
		tcph->dest = htons (cmd.target_port);
		tcph->seq = 10;
		tcph->ack_seq = 10;
		tcph->doff = 5;
		tcph->fin = cmd.fin;
		tcph->syn = cmd.syn;
		tcph->rst = cmd.rst;
		tcph->psh = cmd.psh;
		tcph->ack = cmd.ack;
		tcph->urg = cmd.urg;
		tcph->window = htons(cmd.window_size);
		tcph->check = 0;
		tcph->urg_ptr = 0;
		psh.source_address = inet_addr( source_ip );
		psh.dest_address = sin.sin_addr.s_addr;
		psh.placeholder = 0;
		psh.protocol = IPPROTO_TCP;
		psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data));
		int result = TCP (cmd.pck_amount, cmd.delay, cmd.time, iph, tcph, sin, psh, datagram, source_ip, data, pseudogram);
		return 0;
	}
	else if (cmd.protocol == "udp") {
		if (cmd.pck_amount >= 0) {
			for (int i = 0; i < cmd.pck_amount; i++) {
				int result = UDP(payload_chars, ip_chars_tar, cmd.target_port);
				cout << "Sending... "; 
				switch (result) {
					case 0:
						cout << "OK!\n";
						break;
					case 1:
						cout << "Can't send.\n";
						break;
					case -1:
						cout << "Can't open the socket!\n";
						return 0;
						break;
				}
				usleep(cmd.delay * microsecond);
			}
			cout << "Complete successful!\n";
			return 0;
		}
		else if (cmd.time >= 0 && cmd.pck_amount == -1) {
			unsigned int start_time = curtime();
			unsigned int now = curtime();
			while ((now - start_time) < (cmd.time * 1000)) {
				cout << "Sending... "; 
				now = curtime();
				int result = UDP(payload_chars, ip_chars_tar, cmd.target_port);
				switch (result) {
					case 0:
						cout << "OK!\n";
						break;
					case 1:
						cout << "Can't send.\n";
						break;
					case -1:
						cout << "Can't open the socket!\n";
						return 0;
						break;
				}
				usleep(cmd.delay * microsecond);
			}
			cout << "Complete successful!\n";
			return 0;
		}
		else if (cmd.time == -1 && cmd.pck_amount == -1) {
			while (true) {
				cout << "Sending... "; 
				int result = UDP(payload_chars, ip_chars_tar, cmd.target_port);
				switch (result) {
					case 0:
						cout << "OK!\n";
						break;
					case 1:
						cout << "Can't send.\n";
						break;
					case -1:
						cout << "Can't open the socket!\n";
						return 0;
						break;
				}
				usleep(cmd.delay * microsecond);
			}
		}
		else
			cout << "Check your args!";
		return 0;
	}
}
