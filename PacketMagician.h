#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>

using std::string;

struct opts {
	bool help = false;
	string target_ip = "127.0.0.1";
	string protocol = "icmp";
	string interfacei = "eth1";
	long long time = -1;
	unsigned int delay = 0;
	long long pck_amount = -1;
	unsigned int timeout = 1000;
};

struct opts cmd = { NULL };

const unsigned int microsecond = 1000;

#include "clipp.h"
#include "pckgen.h"
#include "pckgen.cpp"
