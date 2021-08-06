#include "PacketMagician.hpp"
#include "cmd.hpp"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	struct cmd opts = {0};
	parseflags(argc, argv, &opts);
	if (opts.help) {
		cout << "some help";
		return 0;
	}
	return 0;
}
