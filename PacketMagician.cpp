#include "PacketMagician.h"

unsigned int reply_global = 0;

using namespace clipp; using std::cout;

int ICMP(const char* ip, unsigned int timeout) {
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
            if (to_addr.sin_addr.s_addr == from_addr.sin_addr.s_addr
                && reply_id == ip_pkt.pckICMP.hdr.icmp_hun.ih_idseq.icd_id) {
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
		option("--protocol") & value("protocol to use (icmp only for now)", cmd.protocol),
		option("--tip", "--target-ip") & value("target ip (127.0.0.1)", cmd.target_ip),
		option("--interface") & value("interface to use", cmd.interfacei),
		option("-t", "--time") & value("time to repeat in seconds, -1 for timeless (-1)", cmd.time),
		option("--timeout") & value("time to wait for respond in ms (1000)", cmd.timeout),
		option("-d", "--delay") & value("delay in ms (0)", cmd.delay),
		option("--pck-amount") & value("amount of packets sent, if > 0, -t will be ignored. Use -1 for endless (-1)", cmd.pck_amount) );

	parse(argc, argv, cli);

	if (cmd.help) {
		cout << usage_lines(cli) << "\n";
		return 0;
	}
	const char* ip_chars = cmd.target_ip.c_str();

	if (cmd.protocol == "icmp") {
		if (cmd.pck_amount >= 0) {
			for (int i = 0; i < cmd.pck_amount; i++) {
				int result = ICMP(ip_chars, cmd.timeout);
				cout << "Querry... "; 
				switch (result) {
					case 0:
						cout << "OK! Answered in " << reply_global << " ms!\n";
						break;
					case 1:
						cout << "Timeout.\n";
						break;
					case -1:
						cout<<"Check your args!\n";
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
				now = curtime();
				int result = ICMP(ip_chars, cmd.timeout);
				switch (result) {
					case 0:
						cout << "Querry... OK! Answered in " << reply_global << " ms!\n";
						break;
					case 1:
						cout << "Timeout.\n";
						break;
					case -1:
						cout<<"Check your args!\n";
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
				int result = ICMP(ip_chars, cmd.timeout);
				switch (result) {
					case 0:
						cout << "Querry... OK! Answered in " << reply_global << " ms!\n";
						break;
					case 1:
						cout << "Timeout.\n";
						break;
					case -1:
						cout<<"Check your args!\n";
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
	return 0;
}
