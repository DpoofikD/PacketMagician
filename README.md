# PacketMagician

Easy tool for constructing and sending custom packets.

# Compile:

~~~
git clone https://github.com/DpoofikD/PacketMagician
cd PacketMagician/
g++ -std=c++17 -o PacketMagician PacketMagician.cpp
~~~

# Usage:

~~~
./PacketMagician -h
        [-h] [--protocol <protocol to use (icmp only for now)>] [--tip <target ip (127.0.0.1)>]
        [--interface <interface to use>] [-t <time to repeat in seconds, -1 for timeless (-1)>]
        [--timeout <time to wait for respond in ms (1000)>] [-d <delay in ms (0)>] [--pck-amount
        <amount of packets sent, if > 0, -t will be ignored. Use -1 for endless (-1)>]
~~~
