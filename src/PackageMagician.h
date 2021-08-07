// PackageMagician.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

using std::string;

struct opts {
	bool help = false;
	string target_ip = "127.0.0.1";
	string protocol = "icmp";
	string interface = "eth1";
	long long time = 10;
	unsigned int delay = 0;
};