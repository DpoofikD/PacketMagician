#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>

using std::string;

struct opts {
	bool help = false;
	bool usage = false;
	string target_ip = "192.168.0.1";
	string source_ip = "192.168.0.2";
	unsigned short target_port = 80;
	unsigned short source_port = 15243;
	short ttl = 255;
	short syn = 0;
	short fin = 0;
	short rst = 0;
	short psh = 0;
	short ack = 0;
	short urg = 0;
	string payload = "Hello, im about to send you some packets!";
	string protocol = "icmp";
	unsigned int window_size = 65536;
	long long time = -1;
	unsigned int delay = 0;
	long long pck_amount = -1;
	unsigned int timeout = 100;
	bool csum = 1;
};

struct TCP_header {
	unsigned int source_address;
	unsigned int dest_address;
	unsigned int placeholder;
	unsigned int protocol;
	unsigned int tcp_length;
};

struct opts cmd = { NULL };

const unsigned int microsecond = 1000;

#include "clipp.h"
#include "pckgen.h"
#include "pckgen.cpp"
