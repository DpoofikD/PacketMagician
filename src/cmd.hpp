#pragma once

struct cmd {
	unsigned int help : 1;

	char* cmdprotocol;
	unsigned int cmdtime;
	char* cmdinterface;
	char* cmdtargetip;
	unsigned int cmddelay;
};

void parseflags(int argc, char* argv[], struct cmd* opts);