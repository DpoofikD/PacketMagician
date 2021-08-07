#include "PackageMagician.h"
#include <iostream>
#include "clipp.h"

using namespace clipp; using std::cout;


int main(int argc, char *argv[])
{
	struct opts cmd = { NULL };

	auto cli = (
		option("-h", "--help").set(cmd.help, true),
		option("--protocol") & value("protocol to use", cmd.protocol),
		option("-tip", "--target-ip") & value("target ip", cmd.target_ip),
		option("--interface") & value("interface to use", cmd.interface),
		option("-t", "--time") & value("time to repeat in seconds, -1 for timeless ", cmd.time),
		option("-d", "--delay") & value("delay", cmd.time) );
	parse(argc, argv, cli);


	if (cmd.help) {
		cout << usage_lines(cli) << "\n";
		return 0;
	}
	return 0;
}
